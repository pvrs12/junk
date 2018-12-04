#include "knot.h"

#include <string.h>
#include <stdio.h>

int count_bin_ones(int* list, int len){
	int i, count = 0;
	for(i=0; i < len; ++i){
		int j;
		for(j = 0; j < 4; ++j){
			count += (list[i] >> j)% 2;
		}
	}
	return count;
}

void print_bin(int* list, int len){
	int i;
	for(i=0; i<len; ++i){
		printf("%02x ", list[i]);
	}
}

int main(){
	int list[LIST_LENGTH];
	
	int i, total = 0;
	char c[13] = "";
	for(i=0; i< 128; ++i){
		sprintf(c, "flqrgnkx-%d", i);
		knot_hash(list, LIST_LENGTH, c);
		total += count_bin_ones(list, LIST_LENGTH);
		print_bin(list, LIST_LENGTH);
		printf("\n");
	}
	printf("total=%d\n", total);
	return 0;

}
