#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <future>
#include <chrono>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <boost/algorithm/string/predicate.hpp>

using boost::asio::ip::tcp;

class client{
private:
	tcp::resolver resolver;
	tcp::socket socket;

	boost::asio::streambuf request;
	boost::asio::streambuf response;

	const std::string nickname;
	std::mutex request_mutex;

	size_t len;

public:
	client(boost::asio::io_service& io_service, const std::string& server, const std::string& port, const std::string& nick):
		resolver(io_service),socket(io_service),nickname(nick),len(0){
			tcp::resolver::query query(server,port);
			resolver.async_resolve(query,
				boost::bind(&client::handle_resolve,this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator));
	}
private:
	void handle_resolve(const boost::system::error_code& err,
		tcp::resolver::iterator endpoint_itr){
		if(!err){
			tcp::endpoint endpoint = *endpoint_itr;
			socket.async_connect(endpoint,
				boost::bind(&client::handle_connect,this,
					boost::asio::placeholders::error,++endpoint_itr));
		}
	}

	void handle_connect(const boost::system::error_code& err,
		tcp::resolver::iterator endpoint_itr){
		if(!err){
			on_connect_complete();
		} else if(endpoint_itr != tcp::resolver::iterator()){
			socket.close();
			tcp::endpoint endpoint = *endpoint_itr;
			socket.async_connect(endpoint,
				boost::bind(&client::handle_connect,this,
					boost::asio::placeholders::error,++endpoint_itr));
		} else {
			std::cout<<"Error: "<<err.message() <<"\n";
		}
	}

	void handle_write_request(const boost::system::error_code& err){
		if(!err){
			std::thread th([this](){
				std::string line;
				std::getline(std::cin,line);
				{
					std::lock_guard<std::mutex> lock(request_mutex);
					std::ostream request_stream(&request);
					request_stream<<line<<"\r\n";
					request_stream.flush();

					boost::asio::async_write(socket,request,
						boost::bind(&client::handle_write_request,this,
							boost::asio::placeholders::error));
				}
			});
			th.detach();
		}
	}

	void handle_read_response(const boost::system::error_code& err){
		if(!err){
			std::string line = {buffers_begin(response.data())+len,buffers_end(response.data())};
			len+=line.size();
			if(boost::starts_with(line,"PING")){
				std::cout<<"Writing Ping for:"<<line;
				write_pong(line);
			} else {
				std::cout<<line;
				boost::asio::async_read(socket,response,
					boost::asio::transfer_at_least(1),
						boost::bind(&client::handle_read_response,this,
							boost::asio::placeholders::error));
			}
		} else if(err != boost::asio::error::eof){
			std::cout<<"Error: "<<err<<"\n";
		}
	}

	void after_pong(const boost::system::error_code& err){
		if(!err){
			boost::asio::async_read(socket,response,
				boost::asio::transfer_at_least(1),
					boost::bind(&client::handle_read_response,this,
						boost::asio::placeholders::error));
		}

	}
	
	void write_pong(std::string ping){
		std::lock_guard<std::mutex> lock(request_mutex);
		std::ostream request_stream(&request);
		ping[1]='O';
		request_stream<<ping<<"\r\n";
		request_stream.flush();
		boost::asio::async_write(socket,request,
			boost::bind(&client::after_pong,this,
				boost::asio::placeholders::error));

	}

	void on_connect_complete(){
		std::ostream request_stream(&request);
		request_stream<<"NICK "<<nickname<<"\r\n";
		request_stream<<"USER "<<nickname<<" 0 * :"<<nickname<<"\r\n";
		request_stream.flush();
		
		boost::asio::async_write(socket,request,
			boost::bind(&client::handle_write_request,this,
				boost::asio::placeholders::error));	
			
		boost::asio::async_read(socket,response,
			boost::asio::transfer_at_least(1),
			boost::bind(&client::handle_read_response,this,
				boost::asio::placeholders::error));
	}

};

int main(int argc, char* argv[]){
	try{
		if(argc != 4){
			std::cerr<<"Usage "<<argv[0]<<" <host> <port> <nickname>"<<std::endl;
			return 1;
		}
		boost::asio::io_service io_service;

		client c(io_service,argv[1],argv[2],argv[3]);
		io_service.run();

	}catch(std::exception& e){
		std::cerr<<"Exception: "<<e.what()<<std::endl;
	}
	return 0;
}
