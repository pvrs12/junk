#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int calc_power(int x, int y, int serial) {
	int rack = x+10;
	int power = rack*y;
	power += serial;
	power *= rack;
	power = power / 100 % 10;
	power -= 5;
	return power;
}

const int SIZE = 301;
const int SERIAL = 5235;

void run() {
	int* grid[SIZE][SIZE];

	/* base case */
	for(int x = 1; x < SIZE; ++x) {
		for(int y = 1; y < SIZE; ++y){
			grid[x][y] = malloc(sizeof(int) * (SIZE - x + 1));
			grid[x][y][0] = calc_power(x, y, SERIAL);
		}
	}

	int max_power = 0;

	/* case 1 -> 300 */
	for (int r = 1; r < SIZE; ++r){
		for(int x = 1; x < SIZE - r; ++x){
			for(int y = 1; y < SIZE - r; ++y){
				int power = (
						  grid[x + 1][y    ][r - 1] /* top right */
						+ grid[x    ][y + 1][r - 1] /* bottom left */
						+ grid[x    ][y    ][0]     /* top left 1 square */
						+ grid[x + r - 1][y + r - 1][0] /* bottom right 1 square */
					);
				if (r > 1) {
						/* don't need to subtract for case #1 */
						power -= grid[x + 1][y + 1][r - 2]; /* uncount middle */
				}
				grid[x][y][r] = power;

				if(power > max_power) {
					max_power = power;
					printf("%d,%d,%d\t%d\n", x, y, r, power);
				}
			}
		}
	}

	for(int x=1; x<SIZE; ++x){
		for(int y =1; y<SIZE;++y){
			free(grid[x][y]);
			grid[x][y] = NULL;
		}
	}
}

int main() {
	clock_t start = clock();
	run();
	clock_t end = clock();
	printf("time = %.3f ms\n", ((float)(end - start)) / CLOCKS_PER_SEC * 1000);

	return 0;
}
