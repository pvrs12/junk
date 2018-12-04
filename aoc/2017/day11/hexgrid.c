#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sign(int a){
    if(a < 0){
        return -1;
    } else if(a > 0){
        return 1;
    } else {
        return 0;
    }
}

int max(int a, int b){
    return a > b ? a : b;
}

int main(){
    char s[3];
    int x_pos = 0;
    int y_pos = 0;

    while(scanf("%2s", s) != EOF){
        if(strnlen(s, 2) == 1){
            //n,s
            if(s[0] == 'n'){
                x_pos --;
            } else if(s[0] == 's'){
                x_pos ++;
            } else {
                printf("ERROR '%s'", s);
            }
        } else {
            //nw,ne,sw,se 
            if(s[0] == 'n'){
                if(s[1] == 'e'){
                    x_pos --;
                    y_pos ++;
                } else if(s[1] == 'w') {
                    y_pos --;
                } else {
                    printf("ERROR '%s'", s);
                }
            } else if(s[0] == 's'){
                if(s[1] == 'e'){
                    y_pos ++;
                } else if(s[1] == 'w') {
                    x_pos ++;
                    y_pos --;
                } else {
                    printf("ERROR %s", s);
                }
            } else {
                printf("ERROR '%s'", s);
            }
        }
        printf("%s\t(%d,%d)\n",s , x_pos, y_pos);
    }

    int dx = 0 - x_pos;
    int dy = 0 - y_pos;

    //if sign matches
    int dist = -1;
    if(sign(dx) == sign(dy)) {
        dist = abs(dx + dy);
    } else {
        dist = max(abs(dx), abs(dy));
    }
    printf("dist=%d", dist);
}