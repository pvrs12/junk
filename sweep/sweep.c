#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ncurses.h>

#define ESC '\033'

struct guess{
	char type;
	int row;
	int col;
};

int guess_space(char** field, char** guess_field, int rows, int cols, int fRow, int fCol);

void print_field_labels_ncurses(char** field, int rows, int cols, int curr, int curc){
	int i,j;
	move(0,0);
	printw("   | ");
	for(j=0;j<cols;++j){
		printw("%2d ",j);
	}
	printw("\n");
	printw("---+-");
	for(j=0;j<cols;++j){
		printw("---");
	}
	printw("\n");
	for(i=0;i<rows;++i){
		printw("%3d|  ",i);
		for(j=0;j<cols;++j){
			if(i==curr && j==curc){
				attron(A_REVERSE);
			}
			if(field == NULL){
				printw("*");
			} else {
				if(field[i][j]=='0'){
					printw("-");
				} else {
					printw("%c",field[i][j]);
				}
			}
			attroff(A_REVERSE);
			printw("  ");
		}
		printw("\n");
	}
	refresh();
}

//draws the field
void print_field_labels(char** field, int rows, int cols, int curr,int curc){
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
			if(i==curr && j==curc){
				//invert color
				printf("%c[7m",ESC);
			} 
			if(field == NULL){
				printf("*");
			} else {
				if(field[i][j]=='0'){
					printf("-");
				} else {
					printf("%c",field[i][j]);
				}
			}
			printf("%c[0m",ESC);  
			printf("  ");
		}
		printf("\n");
	}
}

int count_adjacent_of(char** field, int rows, int cols, int row, int col, char c){
	//count anything if c==' '
	int any=FALSE;
	if(c==' '){
		any=TRUE;
	}
	int count=0;
	if(row!=0){
		if(col!=0){
			if(field[row-1][col-1]==c || any){
				count++;	
			}
		}
		if(col!=cols-1){
			if(field[row-1][col+1]==c || any){
				count++;
			}
		}
		if(field[row-1][col]==c || any){
			count++;
		}
	} 
	if(row != rows-1){
		if(col!=0){
			if(field[row+1][col-1]==c || any){
				count++;	
			}
		}
		if(col!=cols-1){
			if(field[row+1][col+1]==c || any){
				count++;
			}
		}
		if(field[row+1][col]==c || any){
			count++;
		}
	} 
	if(col!=0){
		if(field[row][col-1]==c || any){
			count++;
		}
	}
	if(col!=cols-1){
		if(field[row][col+1]==c || any){
			count++;
		}
	}
	return count;
}

int count_adjacent(char** field, int rows, int cols, int row, int col){
	return count_adjacent_of(field,rows,cols,row,col,' ');
}


//checks if all non-mine squares are uncovered
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

//recursive flood fill of adjacent non-mine spaces from spaces touching 0 mines
void uncover_others(char** field,char** guess_field, int rows, int cols, int fRow, int fCol){
	//if already uncovered do nothing
	if(guess_field[fRow][fCol]!='*'){
		return;
	}
	//uncover this cell
	guess_field[fRow][fCol]=field[fRow][fCol];
	if(field[fRow][fCol]!='0'){
		return;
	}
	//recursively uncover all 8 surrounding cells
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

int chord(char** field, char** guess_field, int rows, int cols, int fRow, int fCol){
	int count = count_adjacent_of(guess_field,rows,cols,fRow,fCol,'@');
	int ret = 1;
	//if the number of flaged == number on cell
	if(count+'0'==guess_field[fRow][fCol]){
		//uncover adjacent squares
		if(fRow!=0){
			if(fCol!=0){
				if(guess_field[fRow-1][fCol-1]=='*'){
					ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow-1,fCol-1);			
				}
			}
			if(fCol!=cols-1){
				if(guess_field[fRow-1][fCol+1]=='*'){
					ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow-1,fCol+1);			
				}
			}
			if(guess_field[fRow-1][fCol]=='*'){
				ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow-1,fCol);
			}
		}
		if(fRow!=rows-1){
			if(fCol!=0){
				if(guess_field[fRow+1][fCol-1]=='*'){
					ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow+1,fCol-1);			
				}
			}
			if(fCol!=cols-1){
				if(guess_field[fRow+1][fCol+1]=='*'){
					ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow+1,fCol+1);			
				}
			}
			if(guess_field[fRow+1][fCol]=='*'){
				ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow+1,fCol);
			}
		}
		if(fCol!=0){
			if(guess_field[fRow][fCol-1]=='*'){
				ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow,fCol-1);
			}
		}
		if(fCol!=cols-1){
			if(guess_field[fRow][fCol+1]=='*'){
				ret = !ret ? ret:guess_space(field,guess_field,rows,cols,fRow,fCol+1);
			}
		}
	}	
	return ret;
}

int guess_space(char** field, char** guess_field, int rows, int cols, int fRow, int fCol){
	if(field[fRow][fCol]=='m'){
		//mine
		return 0;
	} else if(field[fRow][fCol]!='0') {
		//if guess is on an uncovered number, chord
		if(guess_field[fRow][fCol]==field[fRow][fCol]){
			return chord(field,guess_field,rows,cols,fRow,fCol);	
		} else {
			//number
			guess_field[fRow][fCol]=field[fRow][fCol];
			return 1;
		}
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

//generate a new mine field pased on the first row/col guessed
//this is because the first guess is always safe
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

//fill in the non-mined squares with the number of adjacent mines
void number_field(char** field, int rows, int cols){
	int r,c;
	r=0;
	for(r=0;r<rows;++r){
		for(c=0;c<cols;++c){
			if(field[r][c]=='m'){
				continue;
			}
			int mc= count_adjacent_of(field,rows,cols,r,c,'m');
			//printf("(%d,%d)=%d\n",r,c,mc);
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

struct guess get_guess(int rows, int cols){
	struct guess g;
	while(1){
		printf("Enter your guess <g|f|u> <row> <col>: ");
		scanf(" %c %d %d",&g.type,&g.row,&g.col);
		if(g.row>=rows || g.col >= cols){
			printf("\tGuess outside bounds!, guess again\n");
			continue;
		}
		if(g.type!='f'&&g.type!='g'&&g.type!='u'){
			printf("\tYou must select a flag type, g, f, or u\n(g)uess\t(f)lag\t(u)nflag\n");
			continue;
		}
		return g;
	}
}

struct guess get_guess_ncurses(int rows,int cols, int gRow, int gCol, char** field){
	struct guess g;
	g.row = gRow;
	g.col = gCol;

	char ch;
	while(TRUE){
		print_field_labels_ncurses(field,rows,cols,g.row,g.col);
		printw("Use wasd to move. Press g/f/u/q to guess/flag/unflag/quit\n");
		ch = getch();
		if(ch=='q'){
			g.type=ch;
			return g;
		}
		if(ch==' '){
			ch='g';
		}
		if(ch=='g' || ch=='f' || ch=='u'){
			break;
		}
		if(ch == 'w'){
			if (g.row == 0){
				g.row=rows;
			}
			g.row--;
		} else if(ch== 'd'){
			g.col++;
			if(g.col==cols){
				g.col=0;
			}
		} else if(ch == 's'){
			g.row++;
			if(g.row==rows){
				g.row = 0;
			}
		} else if(ch == 'a'){
			if(g.col ==0){
				g.col=cols;
			}
			g.col--;
		}
	}
	g.type=ch;

	return g;
}

void stop_ncurses(int ncurses){
	if(ncurses){
		endwin();
	}
}

int main(int argc, char* argv[]){
	if(argc<4){
		fprintf(stderr,"%s <rows> <cols> <mines>\n",argv[0]);
		return 1;
	}
	int ncurses = argc > 4;
	if(ncurses){
		initscr();
		raw();
		keypad(stdscr,TRUE);
		noecho();
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
	struct guess g;
first:
	if(ncurses){
		g = get_guess_ncurses(rows,cols,0,0,NULL);
	} else {
		print_field_labels(NULL,rows,cols,-1,-1);
		g= get_guess(rows,cols);
	}
	if(g.type != 'g'){
		if(ncurses){
			printw("Your first choice must be a guess! (You can't lose on the first pick!)\n");
			refresh();
		} else {
			printf("Your first choice must be a guess! (You can't lose on the first pick!)\n");
		}
		goto first;
	}
	guesses_count++;
	char** field = new_field(rows,cols,mines,g.row,g.col);
	number_field(field,rows,cols);

	char** guess_field = new_guess_field(field,rows,cols);

	int good_guess=guess_space(field,guess_field,rows,cols,g.row,g.col);
	int start_time=time(0);

	int win = check_win(field,guess_field,rows,cols);
	if(win==0){
		stop_ncurses(ncurses);
		print_field_labels(field,rows,cols,g.row,g.col);
		printf("Yon won Charlie!\n");
		printf("Guesses: %d\n",guesses_count);
		printf("Time: %d seconds\n",(int)(time(0)-start_time));
		return 0;
	}

	while(good_guess){
		if(ncurses){
			print_field_labels_ncurses(guess_field,rows,cols,g.row,g.col);
		} else {
			print_field_labels(guess_field,rows,cols,g.row,g.col);
		}
		win = check_win(field,guess_field,rows,cols);
		if(win==0){
			stop_ncurses(ncurses);
			print_field_labels(field,rows,cols,g.row,g.col);
			printf("Yon won Charlie!\n");
			printf("Guesses: %d\n",guesses_count);
			printf("Time: %d seconds\n",(int)(time(0)-start_time));
			return 0;
		}
		if(ncurses){
			g = get_guess_ncurses(rows,cols,g.row,g.col,guess_field);
		} else {
			g= get_guess(rows,cols);
		}
		if(g.type=='g' && guess_field[g.row][g.col]=='@'){
			if(ncurses){
				printw("Location is flagged, choose another\n");
				refresh();
			} else {
				printf("\tLocation is flagged, choose another\n");
			}
			continue;
		}
		if(g.type=='f'){
			if(guess_field[g.row][g.col]!='*'){
				if(ncurses){
					printw("Location uncovered or already flagged or uncovered, choose another\n");
					refresh();
				} else {
					printf("\tLocation uncovered or already flagged or uncovered, choose another\n");
				}	
				continue;
			}
			flag_space(guess_field,g.row,g.col,1);
			continue;
		}
		if(g.type=='u'){
			flag_space(guess_field,g.row,g.col,0);
			continue;
		}
		if(g.type=='g'){
			good_guess = guess_space(field,guess_field,rows,cols,g.row,g.col);
			guesses_count++;
		}
		if(g.type=='q'){
			stop_ncurses(ncurses);
			exit(0);
		}
	}
	stop_ncurses(ncurses);
	print_field_labels(field,rows,cols,g.row,g.col);
	printf("Boom.\n");
	printf("Guesses: %d\n",guesses_count);
	printf("Time: %d seconds\n",(int)(time(0)-start_time));
	return 0;
}
