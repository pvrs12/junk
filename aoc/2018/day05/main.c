#include <stdio.h>
#include <ctype.h>

int main() {
    //do for all characters removed
    //and one more for no characters removed
    for (char a='a'; a<='z' + 1; ++a){
        int i=1;
        while(1) {
            char in_name[40], out_name[40];
            if (a == 'z' + 1){
                //if no characters removed, then no sub directory
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
                //if the first character is the EOF or newline, the file is at an end
                if (c==EOF || c == '\n') {
                    break;
                }
                //if the character is to be removed
                if (tolower(c)==a){
                    continue;
                }
                char d = fgetc(inf);
                //if the second character is EOF or newline, just put it back, we'll get it next go round
                if (d==EOF || d == '\n') {
                    ungetc(d, inf);
                }
                //if a bond should be made
                if ((d==toupper(c) && !isupper(c))
                    || (d==tolower(c) && !islower(c))) {
                    //don't print anything
                    //also don't put d back
                } else {
                    //if a bond should not be made
                    //print c and put d back for next round
                    fputc(c, outf);
                    ungetc(d, inf);
                }
            }
            //get the position within the files. if they are the same
            //then nothing was removed and we are at the shortest
            long ins = ftell(inf);
            long outs = ftell(outf);
            fclose(inf);
            fclose(outf);

            if(ins == outs) {
                //print out the length of the polymer and the filename
                printf("%ld\t%s\n", outs, out_name);
                break;
            }
        }
    }

    return 0;
}
