#include <iostream>
#include <string>
#include <cmath>
#include <map>

const std::map<char,std::pair<int,int>> numbers={
																{'1',{0,0}},{'2',{1,0}},{'3',{2,0}},
																{'4',{0,1}},{'5',{1,1}},{'6',{2,1}},
																{'7',{0,2}},{'8',{1,2}},{'9',{2,2}},
																{'.',{0,3}},{'0',{1,3}}
															};

double distance(char start,char end){
	auto startM = numbers.find(start);
	auto endM = numbers.find(end);
	if(startM==numbers.end() || endM==numbers.end()){
		return -1;
	}
	std::pair<int,int> startP = startM->second;
	std::pair<int,int> endP = endM->second;
	return std::sqrt(std::pow(endP.first - startP.first,2)
											+std::pow(endP.second - startP.second,2));	
}

int main(){
	std::string ip;
	std::getline(std::cin,ip);
	double dist=0;
	for(int i=1;i<ip.length();++i){
		dist+=distance(ip[i-1],ip[i]);
	}
	dist=std::round(dist*100)/100;
	std::cout<<dist<<"cm"<<std::endl;
}
