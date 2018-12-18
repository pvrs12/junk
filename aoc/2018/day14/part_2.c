#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

const int ELVES = 2;
const int COUNT = 825401;

int check_pattern_section(int start, int end, int* scores);

int sum_scores(int* elves, int* scores) {
  int sum = 0;
  for(int i=0; i<ELVES; ++i) {
    sum += scores[elves[i]];
  }
  return sum;
}

int append_sum(int sum, int** scores, int* scores_len, int* scores_cap) {
  char buffer[3];
  sprintf(buffer, "%d", sum);
  int s = strnlen(buffer, 3);
  for(int i=0; i< s; ++i) {
    (*scores)[(*scores_len)++] = buffer[i] - '0';
		int p = check_pattern_section(*scores_len - 6, *scores_len, *scores);
		if(p) {
			/*printf("#%d\n", c);*/
			return 1;
		}
    if(*scores_len >= *scores_cap) {
      *scores_cap *= 2;
			/*printf("growing %d\n", *scores_cap);*/
      *scores = realloc(*scores, sizeof(int) * (*scores_cap));
      /*print_scores(*scores, *scores_len);*/
    }
  }
	return 0;
}

void print_scores_section(int start, int end, int* scores) {
  for(int i=start; i<end; ++i){
    printf("%d", scores[i]);
  }
  printf("\n");
}

int check_pattern_section(int start, int end, int* scores) {
	int pattern[] = {8, 2, 5, 4, 0, 1};
	/*print_scores_section(0, 6, pattern);*/
	for(int i=start; i<end; ++i){
		if (pattern[i-start] != scores[i]) {
			return 0;
		}
	}
	return 1;
}

int main() {
	clock_t start = clock();
  int elves[2] = {0, 1};
  int scores_len = 0, scores_cap=100;
  int* scores = malloc(sizeof(int) * scores_cap);
  scores[scores_len ++] = 3;
  scores[scores_len ++] = 7;

	int c = 0;
	while(1) {
		++c;
    int sum = sum_scores(elves, scores);
    int p = append_sum(sum, &scores, &scores_len, &scores_cap);
		if(p){
			printf("count = %d\n", c);
			printf("res = %d\n", scores_len -6);
			/*print_scores_section(0, scores_len-6, scores);*/
			break;
		}
    for(int j = 0; j<ELVES; ++j) {
      elves[j] = (elves[j] + scores[elves[j]] + 1) % scores_len;
    }
		/*printf("-%d\n", c);*/
		/*print_scores_section(scores_len -6, scores_len, scores);*/
	}

  free(scores);
	clock_t end = clock();
	printf("%.3fms\n", ((float)(end - start))/CLOCKS_PER_SEC * 1000);
}
