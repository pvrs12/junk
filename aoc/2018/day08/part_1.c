#include <stdio.h>
#include <inttypes.h>
#include <time.h>

uint16_t sum_metadata(uint16_t sum) {
    uint16_t child_count, meta_count;
    scanf("%" SCNu16 " %" SCNu16 " ", &child_count, &meta_count);
    for(uint16_t i=0; i< child_count; ++i){
        sum = sum_metadata(sum);
    }
    for(uint16_t i=0; i< meta_count; ++i){
        uint16_t meta;
        scanf("%" SCNu16 " ", &meta);
        sum += meta;
    }
    return sum;
}

int main(){
    clock_t start = clock();
    uint16_t sum = sum_metadata(0);
    printf("%u\n", sum);
    clock_t end = clock();
    printf("time = %.3f\n", ((float)(end-start)/CLOCKS_PER_SEC * 1000));
    return 0;
}
