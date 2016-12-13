#include <iostream>
#include <string>
#include <array>

#include "libsqlite.hpp"

#define DATABASE "ttt.db"

typedef std::array<std::array<char, 3>,3> TTTBoard;

int main(){
	try{
		int state;
		TTTBoard board;

		sqlite::sqlite db(DATABASE);
		sqlite::statement_ptr s = db.get_statement();

		s->set_sql(R"(CREATE TABLE IF NOT EXISTS Game (
id INTEGER PRIMARY KEY AUTOINCREMENT,
winner CHAR(1),
turn_count INTEGER))");
		s->exec();
		s->reset();

		s->set_sql(R"(CREATE TABLE IF NOT EXISTS Turn (
id INTEGER PRIMARY KEY AUTOINCREMENT,
player CHAR(1),
turn_number INTEGER,
row INTEGER,
col INTEGER,
game_id INTEGER REFERENCES Game(id)))");
		s->exec();
		s->reset();
		
		s->set_sql("INSERT INTO Game (winner, turn_count) VALUES (' ', 0)");
		s->exec();
		s->reset();

		s->set_sql("SELECT id FROM Game ORDER BY id DESC LIMIT 1");
		s->prepare();
		s->step();
		int game_id = s->get_int(0);
		int turns = 0;
		s->reset();

		while(true){
			std::cin>>state;
			if(state == 0){
				//simply the board state, record the state and move on
				std::string player;
				int row, col;
				std::cin>>player>>row>>col;
				s->set_sql("INSERT INTO Turn (player, turn_number, row, col, game_id) VALUES (?1, ?2, ?3, ?4, ?5)");
				s->prepare();
				s->bind(1, player);
				s->bind(2, ++turns);
				s->bind(3, row);
				s->bind(4, col);
				s->bind(5, game_id);

				s->step();
				s->reset();

			} else if(state == 1){
				//someone has won the game
				std::string winner;
				std::cin >> winner;
				s->set_sql("UPDATE Game SET winner = ?1, turn_count = ?3 WHERE id = ?2");
				s->prepare();
				s->bind(1, winner);
				s->bind(2, game_id);
				s->bind(3, turns);
				s->step();
				s->reset();
				break;
			} else if(state == 2){
				//the game is a tie
				s->set_sql("UPDATE Game SET winner = 'T', turn_count = ?3 WHERE id = ?1");
				s->prepare();
				s->bind(1, game_id);
				s->step();
				s->reset();
				break;
			}

		}
	}catch(sqlite::exception& e){
		std::cerr<<e.what()<<std::endl;
	}
}
