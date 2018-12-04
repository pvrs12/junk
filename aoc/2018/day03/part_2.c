#include <stdio.h>
#include <stdlib.h>

const int CLAIMS=8;

int main() {
    int claim, left, top, width, height;
    int* fabric[1000][1000];
    for(int i=0; i<1000; ++i){
        for(int j=0; j<1000; ++j){
            fabric[i][j] = malloc(sizeof(int) * CLAIMS);
            fabric[i][j][0] = 1;
            for(int k=1; k<CLAIMS; ++k){
                fabric[i][j][k] = 0;
            }
        }
    }

    while(scanf("#%d @ %d,%d: %dx%d\n", &claim, &left, &top, &width, &height) != EOF) {
        for(int i=left; i<left+width; ++i){
            for(int j=top; j<top+height; ++j){
                fabric[i][j][fabric[i][j][0]++] = claim;
                if(fabric[i][j][0] >= CLAIMS){
                    printf("Gonna need a bigger boat %d\n", fabric[i][j][0]);
                }
            }
        }
    }

    int solo_claims[1296];
    solo_claims[0] = 0;
    for(int i=1; i<1296; ++i){
        solo_claims[i] = 1;
    }
    for(int i=0; i<1000; ++i){
        for(int j=0; j<1000; ++j){
            if(fabric[i][j][0] > 2) {
                /*printf("%03dx%03d: %d: ", i, j, fabric[i][j][0]);*/
                for(int k=1; k<fabric[i][j][0]; ++k){
                    /*printf("%d, ", fabric[i][j][k]);*/
                    solo_claims[fabric[i][j][k]] = 0;
                }
                /*printf("\n");*/
            }
        }
    }
    for(int i=1; i<1296; ++i){
        if(solo_claims[i]){
            printf("%d\n", i);
        }
    }
    for(int i=0; i<1000; ++i){
        for(int j=0; j<1000; ++j){
            free(fabric[i][j]);
        }
    }

    return 0;
}
