#include <stdio.h>

int main(){
    int garbage_mode = 0;

    int current_depth = 0;
    int total_value = 0;
    int character = 0;
    while(1){
        character ++;
        char c;
        c = getc(stdin);
        if(c == EOF){
            break;
        }

        if(c == '!'){
            character ++;
            char trash;
            trash = getc(stdin);
            if(trash == EOF){
                break;
            }
            continue;
        }
        if(!garbage_mode){
            if(c == '<'){
                printf("Garbage on @%d, depth=%d\n", character, current_depth);
                garbage_mode = 1;
            } else if(c == '{'){
                printf("Group on @%d, depth=%d\n", character, current_depth);
                current_depth += 1;
                total_value += current_depth;
            } else if(c == '}'){
                printf("Group off @%d, depth=%d\n", character, current_depth);
                current_depth -= 1;
                if(current_depth < 0){
                    printf("ERROR!\n");
                }
            }
        } else {
            if(c == '>'){
                printf("Garbage off @%d, depth=%d\n", character, current_depth);
                garbage_mode = 0;
            }
        }
    }
    printf("Total = %d\n", total_value);

}