#include <stdio.h>

int main(){
	int jump_table[1100];

	int len=0;
	while(1){
		if(scanf("%d", &jump_table[len]) == EOF){
			break;
		}
		len++;
	}
	int pointer = 0;
	int step_count = 0;
	while(pointer >= 0 && pointer < len){
		printf("Pointer=%d\tJump=%d\n", pointer, jump_table[pointer]);
		pointer = pointer + jump_table[pointer]++;
		step_count++;
	}
	printf("step_count=%d\n",step_count);

	return 0;
}
