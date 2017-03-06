#include "range.h"

#include <iostream>

int main(){
	//test the range
	for(auto i: range(3)){
		std::cout<<i<<std::endl;
	}
	for(auto i: range(-4, 5)){
		std::cout<<i<<std::endl;
	}
}
