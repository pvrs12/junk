#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int REG = 6;
const int OPS = 3;
const int OP_LEN = 5;
const int OP_COUNT = 50;

struct op {
    char op[5];
    int ops[3];
};

void run_op(struct op* operation, long reg[REG]){
    if(strncmp(operation->op, "addr", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] + reg[operation->ops[1]];
    } else 
    if(strncmp(operation->op, "addi", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] + operation->ops[1];
    } else 
    if(strncmp(operation->op, "mulr", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] * reg[operation->ops[1]];
    } else 
    if(strncmp(operation->op, "muli", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] * operation->ops[1];
    } else 
    if(strncmp(operation->op, "banr", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] & reg[operation->ops[1]];
    } else 
    if(strncmp(operation->op, "bani", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] & operation->ops[1];
    } else 
    if(strncmp(operation->op, "borr", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] | reg[operation->ops[1]];
    } else 
    if(strncmp(operation->op, "bori", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]] | operation->ops[1];
    } else 
    if(strncmp(operation->op, "setr", OP_LEN) == 0) {
        reg[operation->ops[2]] = reg[operation->ops[0]];
    } else 
    if(strncmp(operation->op, "seti", OP_LEN) == 0) {
        reg[operation->ops[2]] = operation->ops[0];
    } else 
    if(strncmp(operation->op, "gtir", OP_LEN) == 0) {
        reg[operation->ops[2]] = (operation->ops[0] > reg[operation->ops[1]]) ? 1 : 0;
    } else 
    if(strncmp(operation->op, "gtri", OP_LEN) == 0) {
        reg[operation->ops[2]] = (reg[operation->ops[0]] > operation->ops[1]) ? 1 : 0;
    } else 
    if(strncmp(operation->op, "gtrr", OP_LEN) == 0) {
        reg[operation->ops[2]] = (reg[operation->ops[0]] > reg[operation->ops[1]]) ? 1 : 0;
    } else 
    if(strncmp(operation->op, "eqir", OP_LEN) == 0) {
        reg[operation->ops[2]] = (operation->ops[0] == reg[operation->ops[1]]) ? 1 : 0;
    } else 
    if(strncmp(operation->op, "eqri", OP_LEN) == 0) {
        reg[operation->ops[2]] = (reg[operation->ops[0]] == operation->ops[1]) ? 1 : 0;
    } else 
    if(strncmp(operation->op, "eqrr", OP_LEN) == 0) {
        reg[operation->ops[2]] = (reg[operation->ops[0]] == reg[operation->ops[1]]) ? 1 : 0;
    } else {
        printf("Failure!!!\n");
        exit(1);
    }
    reg[0] ++;
}

int main() {
    long reg[REG];
    memset(reg, 0, sizeof(long long) * REG);
    scanf("#ip %ld\n", &reg[0]);

    struct op operations [OP_COUNT];
    int op_len = 0;
    while(1) {
        struct op* o = &operations[op_len++];
        if(scanf("%5s", o->op) == EOF) {
            op_len --;
            break;
        }
        if(o->op[0] == '#') {
            op_len--;
            //comment line
            char c;
            printf("comment: ");
            while((c = getc(stdin)) != '\n') {
                printf("%c", c);
            }
            printf("\n");
        } else {
            if(scanf(" %d %d %d\n", &(o->ops[0]), &(o->ops[1]), &(o->ops[2])) == EOF) {
                break;
            }
        }
    }
    for(int i=0; i<op_len; ++i){
        struct op* o = &operations[i];
        printf("op = %5s %d %d %d\n", o->op, o->ops[0], o->ops[1], o->ops[2]);
    }
    printf("===============\n");
    //if out of range then stop
    while(reg[0] >=0 && reg[0] < op_len) {
        printf("reg = ");
        for(int i=0; i<REG; ++i){
            printf("%ld ", reg[i]);
        }
        printf("\n");
        struct op* o = &operations[reg[0]];
        printf("op = %5s %d %d %d\n", o->op, o->ops[0], o->ops[1], o->ops[2]);
        run_op(o, reg);
    }
    printf("reg = ");
    for(int i=0; i<REG; ++i){
        printf("%ld ", reg[i]);
    }
    printf("\n");
}