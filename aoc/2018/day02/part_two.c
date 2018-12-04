#include <malloc.h>
#include <stdio.h>
#include <string.h>

int main() {
    char* sums[260];
    int i=0;
    while(1) {
        sums[i] = malloc(sizeof(char) * 30);
        char* s = sums[i];
        if(scanf("%30s", s) == EOF) {
            break;
        }
        
        i++;
    }
    for(int j=0; j<i; ++j){
        int len = strnlen(sums[j], 30);
        for(int k=0; k<i; ++k){
            int differing_chars=0;
            for(int l=0; l<len; ++l) {
                if (sums[j][l] != sums[k][l] ){
                    differing_chars++;
                }
                if (differing_chars > 1) {
                    break;
                }
            }
            /*printf("differ = %d\t%s\t%s\n", differing_chars, sums[j], sums[k]);*/
            if (differing_chars == 1) {
                for(int l=0; l<len;++l){
                    if (sums[j][l] == sums[k][l] ){
                        printf("%c", sums[j][l]);
                    }
                }
                printf("\n");
                k=9999;
                j=9999;
            }
        }
    }
    for(int j=0; j<i; ++j){
        free(sums[j]);
    }
    return 0;

}
