#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

int main() {
  float startTime = (float)clock()/CLOCKS_PER_SEC;
  size_t stack_cap=100;
  char* stack = malloc(sizeof(char) * stack_cap);
  for(char a='a'; a<='z'+1; ++a){
    // "empty" the stack
    size_t stack_len=0;
    while(1){
      char c = (char)fgetc(stdin);
      if(c == EOF){
        break;
      }
      char d = stack[stack_len-1];
      if((toupper(d) == c && !isupper(d)) ||
        (tolower(d) == c && !islower(d))) {
        //pop d
        stack_len--;
      } else {
        if (tolower(c) != a) {
          stack[stack_len++] = c;
          if (stack_len >= stack_cap) {
            stack_cap *= 2;
            stack = realloc(stack, sizeof(char) * stack_cap);
          }
        }
      }
    }
    fseek(stdin, 0, 0);
    printf("%c\t%lu\n", a, stack_len -1);
  }
  free(stack);
  float endTime = (float)clock()/CLOCKS_PER_SEC;
  printf("%f\n", (endTime - startTime)*1000);
}
