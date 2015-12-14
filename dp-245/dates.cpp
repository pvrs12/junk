#include <iostream>
#include <cstdio>
#include <string>

int getNumber(){
	int number=0;
	char c;
	while(std::cin.get(c)){
		if(!isdigit(c)){
			break;	
		} else {
			number*=10;
			number+=c-'0';
		}
	}
	return number;
}

int main(){
	while(true){
		int year,month,day;
		int firstNum = getNumber();
		int secondNum = getNumber();
		int thirdNum = getNumber();

		if(firstNum > 12){
			year = firstNum;
			month = secondNum;
			day = thirdNum;
		} else {
			month = firstNum;
			day = secondNum;
			year = thirdNum;
			//if 2 digit year
			if(year < 1000){
				year+=2000;
			}
		}
		if(year==0 || month==0 || day==0){
			break;
		}
		printf("%4d-%02d-%02d\n",year,month,day);
		if(!std::cin || std::cin.eof()){
			break;
		}
	}
}
