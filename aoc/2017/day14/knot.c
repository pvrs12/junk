#include "knot.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

void print_list(int* list, int len){
    int i;
    for(i = 0; i < len; ++i){
        printf("%d\n", list[i]);
    }
}

void reverse_portion(int* list, int list_length, int start, int length) {
    int i;
    //length --;
    int swaps = length / 2;
    for(i = 0; i < swaps; ++i){
        int a = i + start;
        a %= list_length;
        int b = start + length - i - 1;
        b %= list_length;
        //printf("Swapping %d and %d\n", a, b);

        int temp = list[a]; 
        list[a] = list[b];
        list[b] = temp;
    }
}

void knot_hash(int* list, int list_len, const char* input){
	int skip = 0;
	int position = 0;
	int i, len = strnlen(input, 256);

	for(i = 0; i< list_len; ++i){
			list[i] = i;
	}

	for(i=0; i < len; ++i){
		char j = input[i];
		reverse_portion(list, list_len, position, j);
		position += j + skip;
		position %= list_len; 
		skip ++;
	}
}

/*int main(){*/
    /*int list[LIST_LENGTH];*/
    /*int list_length = LIST_LENGTH;*/

		/*knot_hash(list, list_length, "flqrgnkx-127");*/

    /*print_list(list, list_length);*/

    /*return 0;*/
/*}*/
