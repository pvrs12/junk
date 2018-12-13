#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int GRID_LEN = 150;

enum turn_direction {
	LEFT,
	STRAIGHT,
	RIGHT
};

enum facing {
	NORTH = '^',
	EAST = '>',
	SOUTH = 'v',
	WEST = '<'
};

struct cart {
	int id;

	int x;
	int y;
	int enabled;

	enum turn_direction next_turn;
	enum facing current_facing;
};

struct cart* new_cart(int id, int x, int y, char ch) {
	struct cart* c = malloc(sizeof(struct cart));
	c->id = id;
	c->x = x;
	c->y = y;
	c->enabled = 1;

	c->next_turn = LEFT;
	c->current_facing = ch;
	return c;
}

void free_cart(struct cart* c) {
	free(c);
}

void turn_cart(struct cart* c) {
	if(c->next_turn == LEFT) {
		c->next_turn = STRAIGHT;

		if(c->current_facing == NORTH){
			c->current_facing = WEST;
		} else if(c->current_facing == EAST) {
			c->current_facing = NORTH;
		} else if(c->current_facing == SOUTH) {
			c->current_facing = EAST;
		} else if(c->current_facing == WEST) {
			c->current_facing = SOUTH;
		}
	} else if(c->next_turn == STRAIGHT) {
		c->next_turn = RIGHT;
		
	} else if(c->next_turn == RIGHT) {
		c->next_turn = LEFT;

		if(c->current_facing == NORTH){
			c->current_facing = EAST;
		} else if(c->current_facing == EAST) {
			c->current_facing = SOUTH;
		} else if(c->current_facing == SOUTH) {
			c->current_facing = WEST;
		} else if(c->current_facing == WEST) {
			c->current_facing = NORTH;
		}

	}
}

void move_cart(struct cart* c, char** grid) {
	if(!c->enabled){
		return;
	}
	int next_x = c->x;
	int next_y = c->y;

	if(c->current_facing == NORTH){
		next_y--;
	} else if(c->current_facing == EAST) {
		next_x++;
	} else if(c->current_facing == SOUTH) {
		next_y++;
	} else if(c->current_facing == WEST) {
		next_x--;
	}
	c->x = next_x;
	c->y = next_y;

	char next_c = grid[next_x][next_y];
	if(next_c == '/'){
		if(c->current_facing == NORTH){
			c->current_facing = EAST;
		} else if(c->current_facing == EAST) {
			c->current_facing = NORTH;
		} else if(c->current_facing == SOUTH) {
			c->current_facing = WEST;
		} else if(c->current_facing == WEST) {
			c->current_facing = SOUTH;
		}
	}
	if(next_c == '\\') {
		if(c->current_facing == NORTH){
			c->current_facing = WEST;
		} else if(c->current_facing == EAST) {
			c->current_facing = SOUTH;
		} else if(c->current_facing == SOUTH) {
			c->current_facing = EAST;
		} else if(c->current_facing == WEST) {
			c->current_facing = NORTH;
		}
	}

	if(next_c == '+') {
		turn_cart(c);
	}
}

int compare_carts(struct cart* a, struct cart* b){
	if(a->y < b-> y){
		return -1;
	}
	if (a->y > b->y){
		return 1;
	}
	if (a->x < b->x){
		return -1;
	}
	if (a->x > b->x ){
		return 1;
	}
	return 0;
}

/* insertion sort */
void sort_carts(struct cart** carts, int cart_len){
	for(int i=0; i<cart_len; ++i){
		for(int j=0; j<cart_len; ++j){
			if(compare_carts(carts[i], carts[j]) < 0){
				struct cart* c = carts[i];
				carts[i] = carts[j];
				carts[j] = c;
			}
		}
	}
}

int check_collision(struct cart** carts, int cart_len){
	for(int i=0; i<cart_len; ++i){
		if(!carts[i]->enabled){
			continue;
		}
		for(int j=0; j<cart_len; ++j){
			if(i==j){
				continue;
			}
			if(!carts[j]->enabled){
				continue;
			}
			if(compare_carts(carts[i], carts[j]) == 0){
				return i;
			}
		}
	}
	return -1;
}

void print_cart(struct cart* c) {
		printf("id=%2d at %3d,%3d\t%c\n", c->id, c->x, c->y, c->current_facing);
}

void print_carts(struct cart** carts, int cart_len){
	for(int i=0; i<cart_len; ++i){
		if(!carts[i]->enabled){
			continue;
		}
		print_cart(carts[i]);
	}
	printf("\n");
}

int find_cart(int x, int y, struct cart** carts, int cart_len){
	for(int i=0; i<cart_len; ++i){
		if(carts[i]->x == x && carts[i]->y == y && carts[i]->enabled){
			return i;
		}
	}
	return -1;
}

void print_grid(char** grid){
	for(int y=0;y<GRID_LEN;++y){
		for(int x=0;x<GRID_LEN;++x){
			printf("%c", grid[x][y]);
		}
		printf("\n");
	}
}

void print_grid_carts(char** grid, struct cart** carts, int cart_len){
	for(int y=0;y<GRID_LEN;++y){
		for(int x=0;x<GRID_LEN;++x){
			int cart = find_cart(x,y, carts, cart_len);
			if(cart != -1){
				printf("%c", carts[cart]->current_facing);
			} else {
				printf("%c", grid[x][y]);
			}
		}
		printf("\n");
	}
}

int main() {
	clock_t start = clock();
	char** grid = malloc(sizeof(char *) * GRID_LEN);
	for(int i=0; i<GRID_LEN;++i){
		grid[i] = malloc(sizeof(char) * GRID_LEN);	
	}
	struct cart* carts[20];
	int cart_len = 0;

	for(int y=0; y<GRID_LEN; ++y){
		for(int x=0; x<GRID_LEN; ++x){
			char c;
			scanf("%c", &c);
			if(c == '^' || c=='v'){
				carts[cart_len] = new_cart(cart_len, x, y, c);
				cart_len++;
				grid[x][y] = '|';
			} else if( c=='>' || c=='<') {
				carts[cart_len] = new_cart(cart_len, x, y, c);
				cart_len++;
				grid[x][y] = '-';
			} else {
				grid[x][y] = c;
			}
		}
		getc(stdin);//trash newline
	}

	int tick = 1;
	int running =1;
	int first_collision = 1;
	while(running) {	
		sort_carts(carts, cart_len);
		/*printf("===============================================================================================================================================\n");*/
		/*print_grid_carts(grid, carts, cart_len);*/
		/*printf("===============================================================================================================================================\n");*/
		for(int i=0; i<cart_len; ++i){
			if(!carts[i]->enabled){
				continue;
			}
			move_cart(carts[i], grid);
			int collided = check_collision(carts, cart_len);
			if(collided != -1){
				/*print_carts(carts, cart_len);*/

				int x = carts[collided]->x;
				int y = carts[collided]->y;
				if(first_collision){
					printf("Collision at %d,%d\ttick=%d\n", x, y, tick);
					first_collision = 0;
				}
				/*remove the collided carts */
				while(1) {
					int ca = find_cart(x, y, carts, cart_len);
					if(ca == -1){
						break;
					}
					/*printf("removing ");*/
					/*print_cart(carts[ca]);*/
					carts[ca]->enabled = 0;
				}
				/*printf("==========\n");*/
			}
		}
		int enabled_cart = -1;
		int enabled_count = 0;
		for(int i=0; i<cart_len;++i){
			if(carts[i]->enabled){
				enabled_count++;
				enabled_cart = i;
			}
		}
		if(enabled_count == 1){
			printf("last cart at %d,%d\n", carts[enabled_cart]->x, carts[enabled_cart]->y);
			running = 0;
		}
		tick ++;
	}
	clock_t end = clock();
	printf("%.3fms\n", ((float)(end-start))/CLOCKS_PER_SEC * 1000.);

	for(int i=0; i<cart_len;++i){
		free_cart(carts[i]);
	}
	for(int i=0; i<GRID_LEN;++i){
		free(grid[i]);
	}
	free(grid);

}

