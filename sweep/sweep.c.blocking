#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void print_field_labels(char** field, int rows, int cols){
	int i,j;
	printf("   | ");
	for(j=0;j<cols;++j){
		printf("%2d ",j);
	}
	printf("\n");
	printf("---+-");
	for(j=0;j<cols;++j){
		printf("----");
	}
	printf("\n");
	for(i=0;i<rows;++i){
		printf("%3d|  ",i);
		for(j=0;j<cols;++j){
			if(field[i][j]=='0'){
				printf("-  ");
			} else {
				printf("%c  ",field[i][j]);
			}
		}
		printf("\n");
	}
}

void print_field(char** field, int rows, int cols){
	int i,j;
	for(i=0;i<rows;++i){
		for(j=0;j<cols;++j){
			if(field[i][j]=='0'){
				printf(" ");
			} else {
				printf("%c",field[i][j]);
			}
		}
		printf("\n");
	}
}

int check_win(char** field, char** guess_field, int rows, int cols){
	int i,j;
	for(i=0;i<rows;++i){
		for(j=0;j<cols;++j){
			if(field[i][j]=='m'){
				continue;
			}
			if(field[i][j]!=guess_field[i][j]){
				return -1;
			}
		}
	}
	return 0;
}

char** new_guess_field(char** field, int rows, int cols){
	char** guess_field = (char**)malloc(sizeof(char*)*rows);
	int i,j;
	for(i=0;i<rows;++i){
		guess_field[i]=(char*)malloc(sizeof(char)*cols);
		for(j=0;j<cols;++j){
			guess_field[i][j]='*';
		}
	}
	return guess_field;
}

void uncover_others(char** field,char** guess_field, int rows, int cols, int fRow, int fCol){
	if(guess_field[fRow][fCol]!='*'){
		return;
	}
	guess_field[fRow][fCol]=field[fRow][fCol];
	if(field[fRow][fCol]!='0'){
		return;
	}
	if(fRow>0){
		uncover_others(field,guess_field,rows,cols,fRow-1,fCol);
		if(fCol>0){
			uncover_others(field,guess_field,rows,cols,fRow-1,fCol-1);
		}
		if(fCol<cols-1){
			uncover_others(field,guess_field,rows,cols,fRow-1,fCol+1);
		}
	}
	if(fRow<rows-1){
		uncover_others(field,guess_field,rows,cols,fRow+1,fCol);
		if(fCol>0){
			uncover_others(field,guess_field,rows,cols,fRow+1,fCol-1);
		}
		if(fCol<cols-1){
			uncover_others(field,guess_field,rows,cols,fRow+1,fCol+1);
		}
	}
	if(fCol>0){
		uncover_others(field,guess_field,rows,cols,fRow,fCol-1);
	}
	if(fCol<cols-1){
		uncover_others(field,guess_field,rows,cols,fRow,fCol+1);
	}
	return;
}

int guess_space(char** field, char** guess_field, int rows, int cols, int fRow, int fCol){
	if(field[fRow][fCol]=='m'){
		//mine
		return 0;
	} else if(field[fRow][fCol]!='0') {
		//number
		guess_field[fRow][fCol]=field[fRow][fCol];
		return 1;
	} else {
		uncover_others(field,guess_field,rows,cols,fRow,fCol);
		return 1;
	}
}

void flag_space(char** guess_field, int fRow, int fCol, int flagSet){
	if(flagSet){
		guess_field[fRow][fCol]='@';
	} else {
		guess_field[fRow][fCol]='*';
	}
}

char** new_field(int rows, int cols, int mines,int fRow,int  fCol){
	char** field = (char**)malloc(sizeof(char*)*rows);
	int i,j;
	for(i=0;i<rows;++i){
		field[i]=(char*)malloc(sizeof(char)*cols);
		for(j=0;j<cols;++j){
			field[i][j]='-';
			if(i==fRow && j==fCol){
				field[i][j]='g';
			}
		}
	}

	int count = rows*cols;
	int* mineLocs = (int*) malloc(sizeof(int)*rows*cols);
	for(i=0;i<rows*cols;++i){
		mineLocs[i]=-1;
	}
	for(i=0;i<mines;++i){
		int pos;
		//make sure the first guess isn't filled
		//and that mines dont overlap
		do {	
			pos = rand() % count;
		} while(mineLocs[pos]!=-1 || fRow*cols+fCol == pos);
		mineLocs[pos]=1;
		
		int r,c,p=0,placed=0;
		for(r=0;r<rows;++r){
			for(c=0;c<cols;++c){
				if(p==pos){
					field[r][c]='m';
					placed=1;
					break;
				}
				++p;
			}
			if(placed){
				break;
			}
		}
	}
	free(mineLocs);
	return field;
}

void number_field(char** field, int rows, int cols){
	int r,c;
	r=0;
	for(c=1;c<cols-1;++c){
		if(field[r][c]=='m'){
			continue;
		}
		int mc = 0;
		if(field[r][c-1]=='m'){
			mc++;
		}
		if(field[r][c+1]=='m'){
			mc++;
		}
		if(field[r+1][c-1]=='m'){
			mc++;
		}
		if(field[r+1][c]=='m'){
			mc++;
		}
		if(field[r+1][c+1]=='m'){
			mc++;
		}
		field[r][c]=mc+'0';
	}
	c=0;
	for(r=1;r<rows-1;++r){
		if(field[r][c]=='m'){
			continue;
		}
		int mc=0;
		if(field[r-1][c]=='m'){
			mc++;
		}
		if(field[r+1][c]=='m'){
			mc++;
		}
		if(field[r-1][c+1]=='m'){
			mc++;
		}
		if(field[r][c+1]=='m'){
			mc++;
		}
		if(field[r+1][c+1]=='m'){
			mc++;
		}
		field[r][c]=mc+'0';
	}
	r=rows-1;
	for(c=1;c<cols-1;++c){
		if(field[r][c]=='m'){
			continue;
		}
		int mc=0;
		if(field[r-1][c-1]=='m'){
			mc++;
		}
		if(field[r-1][c]=='m'){
			mc++;
		}
		if(field[r-1][c+1]=='m'){
			mc++;
		}
		if(field[r][c-1]=='m'){
			mc++;
		}
		if(field[r][c+1]=='m'){
			mc++;
		}
		field[r][c]=mc+'0';
	}
	c=cols-1;
	for(r=1;r<rows-1;++r){
		if(field[r][c]=='m'){
			continue;
		}
		int mc=0;
		if(field[r-1][c]=='m'){
			mc++;
		}
		if(field[r+1][c]=='m'){
			mc++;
		}
		if(field[r-1][c-1]=='m'){
			mc++;
		}
		if(field[r][c-1]=='m'){
			mc++;
		}
		if(field[r+1][c-1]=='m'){
			mc++;
		}
		field[r][c]=mc+'0';
	}
	//corners
	//top left
	if(field[0][0]!='m'){
		int mc=0;
		if(field[0][1]=='m'){
			mc++;
		}
		if(field[1][1]=='m'){
			mc++;
		}
		if(field[1][0]=='m'){
			mc++;
		}
		field[0][0]=mc+'0';
	}

	//bottom right
	if(field[rows-1][0]!='m'){
		int mc=0;
		if(field[rows-2][0]=='m'){
			mc++;
		}
		if(field[rows-1][1]=='m'){
			mc++;
		}
		if(field[rows-2][1]=='m'){
			mc++;
		}
		field[rows-1][0]=mc+'0';
	}

	//top left
	if(field[0][cols-1]!='m'){
		int mc=0;
		if(field[0][cols-2]=='m'){
			mc++;
		}
		if(field[1][cols-1]=='m'){
			mc++;
		}
		if(field[1][cols-2]=='m'){
			mc++;
		}
		field[0][cols-1]=mc+'0';
	}

	//bottom right
	if(field[rows-1][cols-1]!='m'){
		int mc=0;
		if(field[rows-2][cols-1]=='m'){
			mc++;
		}
		if(field[rows-1][cols-2]=='m'){
			mc++;
		}
		if(field[rows-2][cols-2]=='m'){
			mc++;
		}
		field[rows-1][cols-1]=mc+'0';
	}
	
	//main field
	for(r=1;r<rows-1;++r){
		for(c=1;c<cols-1;++c){
			if(field[r][c]=='m'){
				continue;
			}
			int mc = 0;
			if(field[r-1][c-1]=='m'){
				mc++;
			}
			if(field[r-1][c]=='m'){
				mc++;
			}
			if(field[r-1][c+1]=='m'){
				mc++;
			}
			if(field[r][c-1]=='m'){
				mc++;
			}
			if(field[r][c+1]=='m'){
				mc++;
			}
			if(field[r+1][c-1]=='m'){
				mc++;
			}
			if(field[r+1][c]=='m'){
				mc++;
			}
			if(field[r+1][c+1]=='m'){
				mc++;
			}
			field[r][c]=mc+'0';
		}
	}
}

void free_field(char** field,int rows, int cols){
	int r;
	for(r=0;r<rows;++r){
		free(field[r]);
	}
	free(field);
}

int main(int argc, char* argv[]){
	if(argc<4){
		fprintf(stderr,"%s <rows> <cols> <mines>\n",argv[0]);
		return 1;
	}

	srand(time(0));

	int rows,cols,mines;
	rows = atoi(argv[1]);
	cols = atoi(argv[2]);
	mines = atoi(argv[3]);
	if(mines > rows * cols-1){
		fprintf(stderr,"Too many mines (%d) you can only have up to %d mines\n",mines,rows*cols-1);
		return 2;
	}
	
	int guesses_count=0;
guess:
	printf("Enter your first guess (safe) <row> <col>: ");
	int gRow,gCol;
	char flag;
	scanf("%d %d",&gRow,&gCol);
	if(gRow>=rows || gCol >= cols){
		printf("Guess outside bounds!, guess again\n");
		goto guess;
	}
	guesses_count++;

	char** field = new_field(rows,cols,mines,gRow,gCol);
	number_field(field,rows,cols);

	char** guess_field = new_guess_field(field,rows,cols);

	int good_guess=guess_space(field,guess_field,rows,cols,gRow,gCol);

	int start_time=time(0);
	while(good_guess){
		print_field_labels(guess_field,rows,cols);
		int win = check_win(field,guess_field,rows,cols);
		if(win==0){
			printf("Yon won Charlie!\n");
			printf("Guesses: %d\n",guesses_count);
			printf("Time: %d seconds\n",(int)(time(0)-start_time));
			return 0;
		}
		printf("Enter your guess <g|f|u> <row> <col>: ");
		scanf(" %c %d %d",&flag,&gRow,&gCol);
		if(gRow>=rows || gCol >= cols){
			printf("\tGuess outside bounds!, guess again\n");
			continue;
		}
		if(flag!='f'&&flag!='g'&&flag!='u'){
			printf("\tYou must select a flag type, g, f, or u\n(g)uess\t(f)lag\t(u)nflag\n");
			continue;
		}
		if(flag=='g'&&guess_field[gRow][gCol]=='@'){
			printf("\tLocation is flagged, choose another\n");
			continue;
		}
		if(flag=='f'){
			if(guess_field[gRow][gCol]!='*'){
				printf("\tLocation uncovered or already flagged or uncovered, choose another\n");
				continue;
			}
			flag_space(guess_field,gRow,gCol,1);
			continue;
		}
		if(flag=='u'){
			flag_space(guess_field,gRow,gCol,0);
			continue;
		}
		if(flag=='g'){
			good_guess = guess_space(field,guess_field,rows,cols,gRow,gCol);
			guesses_count++;
		}
	}
	printf("Boom.\n");
	printf("Guesses: %d\n",guesses_count);
	printf("Time: %d seconds\n",(int)(time(0)-start_time));
	print_field_labels(field,rows,cols);
	return 1;
}
