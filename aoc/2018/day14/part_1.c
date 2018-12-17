#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

const int ELVES = 2;
const int COUNT = 825401;

int sum_scores(int* elves, int* scores) {
    int sum = 0;
    for(int i=0; i<ELVES; ++i) {
        sum += scores[elves[i]];
    }
    return sum;
}

void append_sum(int sum, int** scores, int* scores_len, int* scores_cap) {
    char buffer[3];
    sprintf(buffer, "%d", sum);
    int s = strnlen(buffer, 3);
    for(int i=0; i< s; ++i) {
        (*scores)[(*scores_len)++] = buffer[i] - '0';
        if(*scores_len >= *scores_cap) {
            *scores_cap *= 2;
            /*printf("growing %d\n", *scores_cap);*/
            *scores = realloc(*scores, sizeof(int) * (*scores_cap));
            /*print_scores(*scores, *scores_len);*/
        }
    }
}

void print_scores_section(int start, int end, int* scores) {
    for(int i=start; i<end; ++i){
        printf("%d", scores[i]);
    }
    printf("\n");
}

int main() {
		clock_t start = clock();
    int elves[2] = {0, 1};
    int scores_len = 0, scores_cap=100;
    int* scores = malloc(sizeof(int) * scores_cap);
    scores[scores_len ++] = 3;
    scores[scores_len ++] = 7;

    for(int i=0; i<COUNT + 10; ++i) {
        int sum = sum_scores(elves, scores);
        append_sum(sum, &scores, &scores_len, &scores_cap);
        for(int j = 0; j<ELVES; ++j) {
            elves[j] = (elves[j] + scores[elves[j]] + 1) % scores_len;
        }
    }
    print_scores_section(COUNT, COUNT+10, scores);

    free(scores);
		clock_t end = clock();
		printf("%.3fms\n", ((float)(end - start))/CLOCKS_PER_SEC * 1000);
}
