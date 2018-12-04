#include <stdio.h>

int main() {
    int claim, left, top, width, height, count=0;
    int fabric[1000][1000];
    for(int i=0; i<1000; ++i){
        for(int j=0; j<1000; ++j){
            fabric[i][j]=0;
        }
    }

    while(scanf("#%d @ %d,%d: %dx%d\n", &claim, &left, &top, &width, &height) != EOF) {
        for(int i=left; i<left+width; ++i){
            /*printf("i=%d\n", i);*/
            for(int j=top; j<top+height; ++j){
                /*printf("j=%d\n", j);*/
                fabric[i][j]++; 
                if (fabric[i][j] == 2) {
                    count++;
                }
            }
        }
    }
    printf("%d\n", count);
    return 0;

}
