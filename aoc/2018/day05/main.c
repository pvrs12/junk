#include <stdio.h>
#include <ctype.h>

int main() {
    /*char s[50005];*/
    /*scanf("%50005s", s);*/
    /*printf("%s\n", s);*/

    for (char a='a'; a<='z' + 1; ++a){
        int i=1;
        while(1) {
            char in_name[40], out_name[40];
            if (a == 'z' + 1){
                sprintf(in_name, "inputs/input_%03d", i);
                sprintf(out_name, "inputs/input_%03d", ++i);
            } else {
                sprintf(in_name, "inputs/%c/input_%03d", a, i);
                sprintf(out_name, "inputs/%c/input_%03d", a, ++i);
            }
            /*printf("%s\t%s\n", in_name, out_name);*/
            FILE* inf = fopen(in_name, "r");
            FILE* outf = fopen(out_name, "w");

            while(1) {
                char c = fgetc(inf);
                if (c==EOF || c == '\n') {
                    break;
                }
                if (tolower(c)==a){
                    continue;
                }
                char d = fgetc(inf);
                if (d==EOF || d == '\n') {
                    ungetc(d, inf);
                    /*continue;*/
                }
                if ((d==toupper(c) && !isupper(c))
                    || (d==tolower(c) && !islower(c))) {
                    //bond
                    /*printf("removing %c%c.\n", c, d);*/
                    /*fprintf(stderr, "removing %c%c\n", c, d);*/
                } else {
                    /*fputc(c, stdout);*/
                    fputc(c, outf);
                    ungetc(d, inf);
                }
            }
            long ins = ftell(inf);
            long outs = ftell(outf);
            /*printf("\n");*/
            /*printf("%ld\t%ld\n", ins, outs);*/
            fclose(inf);
            fclose(outf);

            if(ins == outs) {
                printf("%ld\t%s\n", outs, out_name);
                break;
            }
        }
    }

    return 0;
}
