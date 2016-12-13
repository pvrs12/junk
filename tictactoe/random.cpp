#include <random>
#include <iostream>

int main(){
	std::random_device rd;
	std::mt19937_64 gen(rd());

	std::uniform_int_distribution<unsigned long long> dist;

	for(int i=0; i<400; ++i){
		std::cout<<dist(gen)%3<<" "<<dist(gen)%3<<std::endl;
	}
	std::cout<<std::endl;
}
