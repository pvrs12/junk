#include <iostream>

#include "dungeon.h"

int main(int argc, char* argv[]){
	if(argc < 3){
		std::cerr<<"You must provide the dimesions of the dungeon to build"<<std::endl;
		std::cerr<<"\t"<<argv[0]<<" <rows> <cols>"<<std::endl;
		return 1;
	}

	int rows = atoi(argv[1]);
	int cols = atoi(argv[2]);

	Dungeon d(rows, cols);
	std::cout<<d.to_string()<<std::endl;
	d.generate();
	std::cout<<d.to_string()<<std::endl;

}

