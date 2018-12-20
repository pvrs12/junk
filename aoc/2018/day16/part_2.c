#include <stdio.h>
#include <string.h>

#include <time.h>

const int OPS = 16;
const int REG = 4;

void run_instructions(const int before[REG], const int op[REG], int afters[OPS][REG]) {
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

void print_op(int o){
    char c[6];

    switch(o) {
        case 0:
            strncpy(c, "addr\n", 6);
            break;
        case 1:
            strncpy(c, "addi\n", 6);
            break;
        case 2:
            strncpy(c, "mulr\n", 6);
            break;
        case 3:
            strncpy(c, "muli\n", 6);
            break;
        case 4:
            strncpy(c, "banr\n", 6);
            break;
        case 5:
            strncpy(c, "bani\n", 6);
            break;
        case 6:
            strncpy(c, "borr\n", 6);
            break;
        case 7:
            strncpy(c, "bori\n", 6);
            break;
        case 8:
            strncpy(c, "setr\n", 6);
            break;
        case 9:
            strncpy(c, "seti\n", 6);
            break;
        case 10:
            strncpy(c, "gtir\n", 6);
            break;
        case 11:
            strncpy(c, "gtri\n", 6);
            break;
        case 12:
            strncpy(c, "gtrr\n", 6);
            break;
        case 13:
            strncpy(c, "eqir\n", 6);
            break;
        case 14:
            strncpy(c, "eqri\n", 6);
            break;
        case 15:
            strncpy(c, "eqii\n", 6);
            break;
    }
    printf("%d\t%6s", o, c);
}

int main() {
    clock_t start = clock();

    int total = 0;
    int count = 0;

    // int solved[] = {
    //     14, //0     addr
    //     0,  //1     addi x
    //     15, //2     mulr
    //     8,  //3     muli
    //     10, //4     banr
    //     1,  //5     bani x
    //     3,  //6     borr x
    //     5,  //7     bori
    //     7,  //8     setr
    //     9,  //9     seti
    //     2,  //10    gtir x
    //     11, //11    gtri
    //     6,  //12    gtrr
    //     12, //13    eqir
    //     13, //14    eqri
    //     4   //15    eqrr
    // };
    
    int solved[16];
    memset(solved, -1, sizeof(int) * 16);
    while(1){
        int before[REG];
        int afters[OPS][REG];
        int op[REG];
        if(scanf("Before: [%d, %d, %d, %d]\n", &before[0], &before[1], &before[2], &before[3]) == EOF) {
            break;
        }
        scanf("%d %d %d %d\n", &op[0], &op[1], &op[2], &op[3]);
        for(int i=0; i<OPS; ++i){
            for(int j=0; j< REG; ++j){
                afters[i][j] = before[j];
            }
        }

        run_instructions(before, op, afters);
        int after[REG];
        scanf("After:  [%d, %d, %d, %d]\n", &after[0], &after[1], &after[2], &after[3]);
        scanf("\n");

        int same_count = 0;
        int o = -1;
        for(int i=0; i<OPS; ++i){
            if(solved[i] != -1) {
                continue;
            }
            int match = 1;
            for(int j =0; j<REG; ++j){
                if(afters[i][j] != after[j]){
                    match = 0;
                }
            }
            if(match) {
                o = i;
                same_count++;
            }
        }

        if (same_count == 1) {
            total ++;
            // printf("op[%d] == ", op[0]);
            // print_op(o);
            solved[o] = op[0];
            // printf("%d %d %d %d\n", op[0], op[1], op[2], op[3]);
        }
        count++;
    }
    // printf("%d\n", count);
    // printf("%d\n", total);
    int new_solved[OPS];
    for(int i=0; i<OPS; ++i) {
        new_solved[solved[i]] = i;
    }

    //next bit
    FILE* f = fopen("input_2.txt", "r");
    int reg[REG];
    memset(reg, 0, sizeof(int) * REG);
    int op[REG];
    while(fscanf(f, "%d %d %d %d\n", &op[0], &op[1], &op[2], &op[3]) != EOF) {
        //printf("op = %d %d %d %d\n", op[0], op[1], op[2], op[3]);
        //printf("new_solved = ");
        //print_op(new_solved[op[0]]);
        int afters[OPS][REG];
        for(int i=0; i<OPS; ++i) {
            for(int j=0; j<REG; ++j){
                afters[i][j] = reg[j];
            }
        }
        run_instructions(reg, op, afters);
        // for(int i=0; i<OPS; ++i) {
        //     fprintf(stderr, "new_sovled[%d] == %d\n", i, new_solved[i]);
        // }
        // for(int i=0; i<OPS; ++i) {
        //     printf("\t%d %d %d %d %c\n", afters[i][0], afters[i][1], afters[i][2], afters[i][3], i == new_solved[op[0]] ? 'x' : ' ');
        // }
        for(int i=0; i<REG; ++i){
            reg[i] = afters[new_solved[op[0]]][i];
        }

    }
    printf("final = %d %d %d %d\n", reg[0], reg[1], reg[2], reg[3]);
    clock_t end = clock();
    printf("%.3fms", ((float)(end-start))/CLOCKS_PER_SEC * 1000.);
    fclose(f);
}