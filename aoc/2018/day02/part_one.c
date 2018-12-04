#include <stdio.h>
#include <string.h>

int main() {
    char s[30], counts[26];
    int two_count=0, three_count=0;
    while(scanf("%30s", s) != EOF) {
        int twos_done=0, threes_done=0;
        for(int i=0; i<26; ++i){
            counts[i] = 0;
        }
        int len=strnlen(s, 30);
        for(int i=0; i<len; ++i){
            counts[s[i]-'a']++;
        }
        for(int i=0; i<26; ++i){
            if (counts[i] == 2 && !twos_done) {
                two_count++;
                twos_done = 1;
            } else if(counts[i] == 3 && !threes_done) {
                three_count++;
                threes_done = 1;
            }
        }
        printf("\n");
    }
    printf("%d * %d = %d\n", two_count, three_count, two_count * three_count);

}
