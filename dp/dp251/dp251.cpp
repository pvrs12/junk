#include <iostream>
#include <string>
#include <vector>


int main(){
	std::vector<std::string> table;
	std::vector<std::vector<int>> rows;
	std::vector<std::vector<int>> columns;

	std::string line;
	while(getline(std::cin,line)){
		table.push_back(line);
	}
	int count = 0;
	for(unsigned i=0;i<table.size();++i){
		std::vector<int> row;
		rows.push_back(row);
		for(unsigned j=0;j<table[i].size();++j){
			if(count){
				if(table[i][j]==' '){
					rows[rows.size()-1].push_back(count);
					count=0;
				} else {
					count++;
				}
			} else {
				if(table[i][j]=='*'){
					count++;
				}
			}
		}
	}
	count=0;
	//this is okay because all the strings are the same length
	for(unsigned i=0;i<table[0].size();++i){
		std::vector<int> column;
		columns.push_back(column);
		for(unsigned j=0;j<table.size();++j){
			if(count){
				if(table[j][i]==' '){
					columns[columns.size()-1].push_back(count);
					count=0;
				} else {
					count++;
				}
			} else {
				if(table[j][i]=='*'){
					count++;
				}
			}
		}
	}
	for(unsigned i=0;i<columns.size();++i){
		for(unsigned j=0;j<columns[i].size();++j){
			std::cout<<columns[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
	for(unsigned i=0;i<rows.size();++i){
		for(unsigned j=0;j<rows[i].size();++j){
			std::cout<<rows[i][j]<<" ";
		}
		std::cout<<std::endl;
	}

	return 0;
}
