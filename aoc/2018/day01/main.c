#include <malloc.h>
#include <stdio.h>

int main() {
    int num, val=0, val_len=0, num_len=0;
    long long val_capacity=10L, num_capacity=10L;
    int *vals = malloc(sizeof(int) * val_capacity);
    int *nums = malloc(sizeof(int) * num_capacity);
    while(scanf("%d", &num) != EOF) {
        nums[num_len++] = num;
        if (num_len >= num_capacity) {
            num_capacity *= 2;
            nums = realloc(nums, sizeof(int) * num_capacity);
        }
        val += num;
        for (int i=0; i<val_len; ++i){
            if (vals[i] == val) {
                printf("part 2 answer=%d\n", val);
            }
        }
        /*printf("%d\n", val);*/
        vals[val_len++] = val;
        if (val_len >= val_capacity) {
            val_capacity *= 2;
            vals = realloc(vals, sizeof(int) * val_capacity);
        }
    }
    printf("part 1 answer=%d\n", val);
    while(1) {
        for(int i=0; i<num_len; ++i) {
            val += nums[i];
            for( int j=0; j<val_len; ++j){
                if (vals[j] == val) {
                    printf("part 2 answer=%d\n", val);
                    return 0;
                }
            }
            vals[val_len++] = val;

            if (val_len >= val_capacity) {
                val_capacity *= 2;
                val_capacity *= 2;
                vals = realloc(vals, sizeof(int) * val_capacity);
            }
        }
    }
    free(vals);
    free(nums);

    return 0;
}
