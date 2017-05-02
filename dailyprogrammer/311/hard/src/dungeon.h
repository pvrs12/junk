#ifndef DUNGEON_H
#define DUNGEON_H

#include <random>
#include <string>
#include <tuple>
#include <vector>
#include <memory>

const unsigned ROOM_COUNT = 10;

const char FILLED = '#';
const char EMPTY = ' ';

class Dungeon;

class Room {
private:
	Dungeon* _dungeon;

	std::pair<unsigned, unsigned> _base;

	std::vector<std::pair<unsigned, unsigned>> _tiles;
	std::vector<std::pair<unsigned, unsigned>> _perimeter;

	void _add_tile(std::pair<unsigned, unsigned> tile);
	void _purge_perimeter();

public:
	std::pair<unsigned, unsigned> get_base() const;

	Room(unsigned row, unsigned col, Dungeon* dungeon)
			: Room(std::make_pair(row, col), dungeon) {
	}
	Room(std::pair<unsigned, unsigned> base, Dungeon* dungeon)
			: _base(base), _dungeon(dungeon) {
	}
	~Room();

	void grow_room();
};

class Dungeon {
private:
	unsigned _roomCount;

	unsigned _rows;
	unsigned _cols;

	std::vector<std::vector<char>> _grid;
	std::vector<Room> _rooms;

	bool _is_connected(const Room& start, const Room& end);
	bool _is_connected(std::pair<unsigned, unsigned> start, std::pair<unsigned, unsigned> end);

public:
	Dungeon(unsigned rows, unsigned cols)
			: _rows(rows), _cols(cols), _grid(_rows, std::vector<char>(_cols, FILLED)), _roomCount(ROOM_COUNT) {
	}
	virtual ~Dungeon();

	std::string to_string();

	void generate();

	unsigned get_rows() const;
	unsigned get_cols() const;

	char get_tile(std::pair<unsigned, unsigned> loc) const;
	char get_tile(unsigned row, unsigned col) const;
};

#endif /* DUNGEON_H */
