#include <iostream>
#include <string>
#include <sstream>
#include <vector>

struct pixel {
	int r,g,b;
	pixel(int red,int green,int blue):
		r(red),g(green),b(blue)
	{}

	pixel():
		r(0),g(0),b(0)
	{}
};

typedef std::vector<std::vector<pixel> > image ;

int main(){
	std::string s;
	int rows,cols;
	//read P3
	std::getline(std::cin,s);
	while(std::getline(std::cin,s)){
		if(s[0]=='#' || s==""){
			continue;
		}
		std::stringstream ss(s);
		ss>>rows>>cols;
		std::cout<<rows<<" "<<cols<<"\n";
		break;
	}
	int max;
	while(std::getline(std::cin,s)){
		if(s[0]=='#' || s==""){
			continue;
		}
		std::stringstream ss(s);
		ss>>max;
		break;
	}
	int r=0,c=0;
	int red,green,blue;
	while(std::getline(std::cin,s)){
		if(s[0]=='#' || s==""){
			continue;
		}
		std::stringstream ss(s);
		ss>>red>>green>>blue;
		std::cout<<"point "<<red<<" "<<green<<" "<<blue<<" "<<r<<" "<<c<<"\n";
		c++;
		if(c==cols){
			c=0;
			r++;
		}
	}
	std::cout<<std::flush;
}
