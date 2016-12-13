#include <iostream>
#include <array>

#define EMPTY ' '
#define X 'X'
#define O 'O'
typedef std::array<std::array<char, 3>,3> TTTBoard;

void draw_board(const TTTBoard& board, bool machine = false){
	if(machine){
		std::cout<<"0"<<std::endl;
		for(auto itr = board.begin(); itr != board.end(); ++itr){
			for(auto jtr = itr->begin(); jtr != itr->end(); ++jtr){
				std::cout<<(*jtr)<<" ";
			}
			std::cout<<std::endl;
		}
	} else {
		std::cout<<board[0][0]<<"|"<<board[0][1]<<"|"<<board[0][2]<<"\n";
		std::cout<<"-+-+-\n";
		std::cout<<board[1][0]<<"|"<<board[1][1]<<"|"<<board[1][2]<<"\n";
		std::cout<<"-+-+-\n";
		std::cout<<board[2][0]<<"|"<<board[2][1]<<"|"<<board[2][2]<<"\n";
		std::cout<<std::flush;
	}
}

char check_winner(const TTTBoard& board){
	//check lefty-righty
	for(auto itr = board.begin(); itr != board.end(); ++itr){
		if(itr->at(0) == itr->at(1) && itr->at(0) == itr->at(2) && itr->at(0) != EMPTY){
			return itr->at(0);
		}
	}
	//check uppy-downy
	for(int i=0; i < board[0].size(); ++i){
		if(board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != EMPTY){
			return board[0][i];
		}
	}
	//check cornery leftyuppy to rightydowny
	if(board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != EMPTY){
		return board[0][0];
	}
	//check cornery rightyuppy to leftydowny
	if(board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != EMPTY){
		return board[0][2];
	}
	return EMPTY;
}

bool is_tie(const TTTBoard& board){
	for(auto itr = board.begin(); itr != board.end(); ++itr){
		for(auto jtr = itr->begin(); jtr != itr->end(); ++jtr){
			if((*jtr) == EMPTY){
				return false;
			}
		}
	}
	return true;
}

void test(TTTBoard& board){
	//test no-winner, empty board
	draw_board(board);
	std::cout<<"No Winner (empty): "<<check_winner(board)<<std::endl;
	//test no-winner, non-empty board
	board[0][0] = X;
	board[0][1] = O;
	board[1][1] = X;
	draw_board(board);
	std::cout<<"No Winner (non-empty): "<<check_winner(board)<<std::endl;
	//test winner, horizontal
	board[0][1] = X;
	board[0][2] = X;
	draw_board(board);
	std::cout<<"Winner (horizontal): "<<check_winner(board)<<std::endl;
	//test winner, vertical
	board[0][1] = EMPTY;
	board[0][2] = EMPTY;
	board[1][0] = O;
	board[2][0] = O;
	draw_board(board);
	std::cout<<"Winner (vertical): "<<check_winner(board)<<std::endl;
	//test winner, diagonal down
	board[1][0] = EMPTY;
	board[2][0] = EMPTY;
	board[0][0] = O;
	board[1][1] = O;
	board[2][2] = O;
	draw_board(board);
	std::cout<<"Winner (diagnonal-down): "<<check_winner(board)<<std::endl;
	//test winner, diagonal up
	board[2][2] = EMPTY;
	board[0][0] = EMPTY;
	board[2][0] = O;
	board[0][2] = O;
	draw_board(board);
	std::cout<<"Winner (diagonal-up): "<<check_winner(board)<<std::endl;
}

int main(int argc, char* argv[]){
	bool machine = argc>1;
	TTTBoard board;
	for(auto itr = board.begin(); itr != board.end(); ++itr){
		for(auto jtr = itr->begin(); jtr != itr->end(); ++jtr){
			(*jtr) = EMPTY;
		}
	}
	char turn = X;
	bool hasWinner = false;
	while(!hasWinner){
		int row,col;
		bool invalidGuess = true;
		while(invalidGuess){
			if(!machine){
				draw_board(board, machine);
			}
			std::cin>>row>>col;
			if(board[row][col] == EMPTY){
				invalidGuess = false;
			}
		}
		board[row][col] = turn;
		if(turn == X){
			turn = O;
		} else {
			turn = X;
		}
		char winner = check_winner(board);
		if(machine){
			std::cout<<"0 "<<board[row][col]<<" "<<row<<" "<<col<<std::endl;
		}
		if(winner != EMPTY){
			hasWinner = true;
			if(machine){
				std::cout<<"1\n"<<winner<<std::endl;
			} else {
				std::cout<<winner<<" has won!"<<std::endl;
			}
		} else if(is_tie(board)){
			hasWinner = true;
			if(machine){
				std::cout<<"2"<<std::endl;
			} else {
				std::cout<<"The game is a tie"<<std::endl;
			}
		}
	}
	if(!machine){
		draw_board(board, machine);
	}
	return 0;
}
