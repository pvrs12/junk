#include <stdio.h>

int main(){
	char first = 0;
	char prev = 0;

	int sum = 0;
	while(1){
		char c;
		if((c = fgetc(stdin)) != EOF){
			if(!isdigit(c)){
				continue;
			}
			if(!first){
				first = c;
				prev = c;
				continue;
			}
			if(prev == c){
				sum += prev - '0';
			}
			prev = c;
		} else {
			break;
		}
	}
	if(prev == first){
		sum += prev - '0';
	}
	printf("%d\n", sum);
}	
