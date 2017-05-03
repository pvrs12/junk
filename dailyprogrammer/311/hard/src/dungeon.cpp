#include "dungeon.h"

#include <random>
#include <sstream>

#include <iostream>

std::pair<unsigned, unsigned> Room::get_base() const {
	return _base;
}

Room::~Room() {
}

void Room::grow_room() {
	//find a tile on the perimeter, pick a valid FILLED location, expand there
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> perimeter_dis(0, _perimeter.size() - 1);

	std::pair<unsigned, unsigned> tile = _perimeter[perimeter_dis(gen)];

	//find a FILLED tile nearby
	std::vector<Direction> available_directions;
	if (tile.first > 0 && _dungeon->get_tile(tile.first - 1, tile.second) == FILLED) {
		available_directions.push_back(Direction::NORTH);
	}
	if (tile.second > 0 && _dungeon->get_tile(tile.first, tile.second - 1) == FILLED) {
		available_directions.push_back(Direction::WEST);
	}
	if (tile.first < _dungeon->get_rows() - 1 && _dungeon->get_tile(tile.first + 1, tile.second) == FILLED) {
		available_directions.push_back(Direction::SOUTH);
	}
	if (tile.second < _dungeon->get_cols() - 1 && _dungeon->get_tile(tile.first, tile.second + 1) == FILLED) {
		available_directions.push_back(Direction::EAST);
	}

	std::uniform_int_distribution<> direction_dis(0, available_directions.size() - 1);
	Direction d = available_directions[direction_dis(gen)];
	switch (d) {
	case NORTH: {
		std::cout << "adding north" << std::endl;
		std::cout << "(" << tile.first - 1 << ", " << tile.second << ")" << std::endl;
		_add_tile(std::make_pair(tile.first - 1, tile.second));
		break;
	}
	case WEST: {
		std::cout << "adding west" << std::endl;
		std::cout << "(" << tile.first << ", " << tile.second - 1 << ")" << std::endl;
		_add_tile(std::make_pair(tile.first, tile.second - 1));
		break;
	}
	case SOUTH: {
		std::cout << "adding south" << std::endl;
		std::cout << "(" << tile.first + 1 << ", " << tile.second << ")" << std::endl;
		_add_tile(std::make_pair(tile.first + 1, tile.second));
		break;
	}
	case EAST: {
		std::cout << "adding east" << std::endl;
		std::cout << "(" << tile.first << ", " << tile.second + 1 << ")" << std::endl;
		_add_tile(std::make_pair(tile.first, tile.second + 1));
		break;
	}
	default: {
		throw DungeonException();
	}
	}
}

void Room::_add_tile(std::pair<unsigned, unsigned> tile) {
	_tiles.push_back(tile);
	_perimeter.push_back(tile);
	_dungeon->set_tile(tile, EMPTY);

	_purge_perimeter();
}

void Room::_purge_perimeter() {
	std::vector<int> to_remove;
	for (unsigned i = 0; i < _perimeter.size(); ++i) {
		std::pair<unsigned, unsigned> p = _perimeter[i];
		int open_sides = 0;
		if (p.first > 0 && _dungeon->get_tile(p.first - 1, p.second) == FILLED) {
			open_sides++;
		}
		if (p.second > 0 && _dungeon->get_tile(p.first, p.second - 1) == FILLED) {
			open_sides++;
		}
		if (p.first < _dungeon->get_rows() - 1 && _dungeon->get_tile(p.first + 1, p.second) == FILLED) {
			open_sides++;
		}
		if (p.second < _dungeon->get_cols() - 1 && _dungeon->get_tile(p.first, p.second + 1) == FILLED) {
			open_sides++;
		}
		if (open_sides == 0) {
			to_remove.push_back(i);
		}
	}
	for (int i = 0; i < to_remove.size(); ++i) {
		_perimeter.erase(_perimeter.begin() + to_remove[i]);
	}
}

std::vector<std::vector<bool>> visited;

bool Dungeon::_is_connected(const Room& start, const Room& end) {
	return _is_connected(start.get_base(), end.get_base());
}

bool Dungeon::_is_connected(std::pair<unsigned, unsigned> start, std::pair<unsigned, unsigned> end) {
	if (start == end) {
		return true;
	}
	visited[start.first][end.first] = true;
	if (start.first != 0) {
		if (_grid[start.first - 1][start.second] == EMPTY && !visited[start.first - 1][start.second]) {
			return _is_connected(std::make_pair(start.first - 1, start.second), std::make_pair(end.first, end.second));
		}
	}
	if (start.second != 0) {
		if (_grid[start.first][start.second - 1] == EMPTY && !visited[start.first][start.second - 1]) {
			return _is_connected(std::make_pair(start.first, start.second - 1), std::make_pair(end.first, end.second));
		}
	}
	if (start.first != _rows) {
		if (_grid[start.first + 1][start.second] == EMPTY && !visited[start.first + 1][start.second]) {
			return _is_connected(std::make_pair(start.first + 1, start.second), std::make_pair(end.first, end.second));
		}
	}
	if (start.second != _cols) {
		if (_grid[start.first][start.second + 1] == EMPTY && !visited[start.first][start.second + 1]) {
			return _is_connected(std::make_pair(start.first, start.second + 1), std::make_pair(end.first, end.second));
		}
	}
	return false;
}

std::string Dungeon::to_string() {
	std::stringstream ss;
	for (unsigned row = 0; row < _rows; ++row) {
		for (unsigned col = 0; col < _cols; ++col) {
			ss << _grid[row][col] << " ";
		}
		ss << "\n";
	}
	return ss.str();
}

void Dungeon::generate() {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> row_dis(0, _rows - 1);
	std::uniform_int_distribution<> col_dis(0, _cols - 1);

	for (unsigned i = 0; i < _roomCount; ++i) {
		unsigned row = row_dis(gen);
		unsigned col = col_dis(gen);

		_grid[row][col] = ' ';

		_rooms.emplace_back(row, col, this);
	}
	for (unsigned j = 0; j < 5; ++j) {
		std::cout << j << std::endl;
		std::cout << to_string() << std::endl;
		for (unsigned i = 0; i < _rooms.size(); ++i) {
			_rooms[i].grow_room();
		}
	}
}

unsigned Dungeon::get_rows() const {
	return _rows;
}

unsigned Dungeon::get_cols() const {
	return _cols;
}

char Dungeon::get_tile(std::pair<unsigned, unsigned> loc) const {
	return get_tile(loc.first, loc.second);
}

char Dungeon::get_tile(unsigned row, unsigned col) const {
	return _grid[row][col];
}

void Dungeon::set_tile(std::pair<unsigned, unsigned> loc, char c) {
	_grid[loc.first][loc.second] = c;
}

Dungeon::~Dungeon() {
}
