#include <stdio.h>
#include <stdlib.h>

struct configuration{
	int blocks[16];
};

int compare_configs(struct configuration* a, struct configuration* b){
	int same = 1, i;
	for(i=0; i<16; ++i){
		if(a->blocks[i] != b->blocks[i]) {
			same = 0;
			break;
		}
	}
	return same;
}

void copy_configuration(struct configuration* a, struct configuration* b){
	int i;
	for(i=0; i<16; ++i){
		b->blocks[i] = a->blocks[i];
	}
}

void print_configuration(struct configuration* c){
	int i;
	for(i=0; i<16; ++i){
		printf("%d ", c->blocks[i]);
	}
	printf("\n");
}

int main(){
	struct configuration current;
	int i;
	for(i=0;i<16;++i){
		scanf("%d", &current.blocks[i]);
	}
	struct configuration* past;
	int capacity = 16;
	past = malloc(sizeof(struct configuration) * capacity);
	int len = 0;
	int step_count = 0;
	
	while(1){

		copy_configuration(&current, &past[len++]);
		if(len > capacity / 2){
			capacity *= 2;
			past = realloc(past, sizeof(struct configuration) * capacity);
		}
		//redistribute current
		int max = -1, max_pos = 0;
		for(i=0; i< 16; ++i){
			if(current.blocks[i] > max){
				max = current.blocks[i];
				max_pos = i;
			}
		}
		/*printf("\n");*/
		/*print_configuration(&current);*/
		printf("Redist @ %d\n", max_pos);

		current.blocks[max_pos] = 0;
		for(i=max; i> 0; --i){
			current.blocks[++max_pos % 16] ++;
			/*print_configuration(&current);*/
		}
		step_count ++;
		//TODO
		/*if(step_count >= 16){*/
			/*break;*/
		/*}*/

		int matches = 0;
		for(i=0; i<len;++i){
			if(compare_configs(&current, &past[i])){
				matches = 1;
				break;
			}
		}
		if(matches){
			break;
		}
	}
	free(past);
	printf("Reconfigured %d times before loop\n", step_count);
	return 0;
}
