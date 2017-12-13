#include <stdio.h>
#include <stdint.h>

#define LIST_LENGTH 256

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

int main(){
    int skip = 0;
    int position = 0;

    int list[LIST_LENGTH];
    int list_length = LIST_LENGTH;
    int i;

    for(i = 0; i< list_length; ++i){
        list[i] = i;
    }

    while(scanf("%d", &i) != EOF){
        //printf("length=%d\n", i);
        //printf("position=%d\n", position);
        //printf("skip=%d\n", skip);
        //print_list(list, list_length);
        reverse_portion(list, list_length, position, i);
        //printf("\n");
        position += i + skip;
        position %= list_length;
        skip++;
    }
    //printf("\n\n");
    print_list(list, list_length);

    return 0;
}