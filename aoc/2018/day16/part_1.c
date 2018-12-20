#include <stdio.h>
#include <time.h>

void run_instructions(const int before[4], const int op[4], int afters[16][4]) {
    //addr
    afters[0][op[3]] = before[op[1]] + before[op[2]];
    //addi
    afters[1][op[3]] = before[op[1]] + op[2];
    //mulr
    afters[2][op[3]] = before[op[1]] * before[op[2]];
    //muli
    afters[3][op[3]] = before[op[1]] * op[2];
    //banr
    afters[4][op[3]] = before[op[1]] & before[op[2]];
    //bani
    afters[5][op[3]] = before[op[1]] & op[2];
    //borr
    afters[6][op[3]] = before[op[1]] | before[op[2]];
    //bori
    afters[7][op[3]] = before[op[1]] | op[2];

    //setr
    afters[8][op[3]] = before[op[1]];
    //seti
    afters[9][op[3]] = op[1];

    //gtir
    afters[10][op[3]] = op[1] > before[op[2]] ? 1 : 0;
    //gtri
    afters[11][op[3]] = before[op[1]] > op[2] ? 1 : 0;
    //gtrr
    afters[12][op[3]] = before[op[1]] > before[op[2]] ? 1 : 0;

    //eqir
    afters[13][op[3]] = op[1] == before[op[2]] ? 1 : 0;
    //eqri
    afters[14][op[3]] = before[op[1]] == op[2] ? 1 : 0;
    //eqrr
    afters[15][op[3]] = before[op[1]] == before[op[2]] ? 1 : 0;
}

int main() {
    clock_t start = clock();

    int total = 0;
    int count = 0;
    while(1){
        int before[4];
        int afters[16][4];
        int op[4];
        if(scanf("Before: [%d, %d, %d, %d]\n", &before[0], &before[1], &before[2], &before[3]) == EOF) {
            break;
        }
        scanf("%d %d %d %d\n", &op[0], &op[1], &op[2], &op[3]);
        for(int i=0; i<16; ++i){
            for(int j=0; j< 4; ++j){
                afters[i][j] = before[j];
            }
        }
        run_instructions(before, op, afters);
        int after[4];
        scanf("After:  [%d, %d, %d, %d]\n", &after[0], &after[1], &after[2], &after[3]);
        scanf("\n");

        int same_count = 0;
        for(int i=0; i<16; ++i){
            int match = 1;
            for(int j =0; j<4; ++j){
                if(afters[i][j] != after[j]){
                    match = 0;
                    break;
                }
            }
            if(match) {
                same_count++;
            }
        }
        if (same_count >= 3) {
            total ++;
        }
        count++;
    }
    printf("%d\n", count);
    printf("%d\n", total);
    clock_t end = clock();
    printf("%.3fms", ((float)(end-start))/CLOCKS_PER_SEC * 1000.);
}