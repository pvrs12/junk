#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

const int MAX_X = 32;
const int MAX_Y = 32;

const int CREATURE_COUNT = 30;

const int GOB_HEALTH = 200;
const int GOB_POWER = 3;
const int ELF_HEALTH = 200;
// const int ELF_POWER = 3;

char DIRS  = 0b00000000;
char NORTH = 0b00001000;
char EAST  = 0b00000100;
char SOUTH = 0b00000010;
char WEST  = 0b00000001;

struct creature {
    char type;
    int x;
    int y;

    int health;
    int power;
    int alive;
};

void print_grid(const char grid[MAX_X][MAX_Y], const struct creature creatures[CREATURE_COUNT]) {
    for(int y=0; y<MAX_Y; ++y){
        for(int x=0; x<MAX_X; ++x){
            int creature_print = 0;
            for (int i=0; i<CREATURE_COUNT; ++i ){
                if (creatures[i].x == x && creatures[i].y == y && creatures[i].alive){
                    if(creature_print) {
                        printf("oh no!");
                    }
                    printf("%c", creatures[i].type);
                    creature_print = 1;
                }
            }
            if(!creature_print) {
                printf("%c", grid[x][y]);
            } else {
                if(grid[x][y] != '.'){
                    printf("oh no!");
                }
            }
        }
        printf("\n");
    }
}

int compare_creatures(const struct creature* a, const struct creature * b) {
    if(a->y < b->y) {
        return -1;
    }
    if(b->y < a-> y) {
        return 1;
    }
    if(a->x < b->x) {
        return -1;
    }
    if(b->x < a->x) {
        return 1;
    }
    return 0;
}

void sort_creatures(struct creature creatures[CREATURE_COUNT]) {
    for(int i=0; i<CREATURE_COUNT; ++i) {
        for(int j=0; j<CREATURE_COUNT; ++j) {
            if(compare_creatures(&creatures[i], &creatures[j]) < 0) {
                struct creature c = creatures[j];
                creatures[j] = creatures[i];
                creatures[i] = c;
            }
        }
    }
}

int creature_at(int x, int y, const struct creature creatures[CREATURE_COUNT]) {
    for(int i=0; i<CREATURE_COUNT; ++i) {
        if(creatures[i].x == x && creatures[i].y == y && creatures[i].alive){
            return i;
        }
    }
    return -1;
}

char open_adjacents(int x, int y, const char grid[MAX_X][MAX_Y], const struct creature creatures[CREATURE_COUNT]) {
    char dirs = 0;

    if (x > 0) {
        if (grid[x-1][y] == '.' && creature_at(x-1, y, creatures) == -1) {
            dirs |= WEST;
        }
    }
    if (x < MAX_X - 1) {
        if (grid[x+1][y] == '.' && creature_at(x+1, y, creatures) == -1) {
            dirs |= EAST;
        }
    }
    if (y > 0) {
        if (grid[x][y-1] == '.' && creature_at(x, y-1, creatures) == -1) {
            dirs |= NORTH;
        }
    }
    if (y < MAX_Y - 1) {
        if (grid[x][y+1] == '.' && creature_at(x, y+1, creatures) == -1) {
            dirs |= SOUTH;
        }
    }
    return dirs;
}

char open_adjacent_targets(int x, int y, char target, const struct creature creatures[CREATURE_COUNT]) {
    char dirs = 0;

    if (x > 0) {
        int i = creature_at(x-1, y, creatures);
        if (i != -1 && creatures[i].type == target) {
            dirs |= WEST;
        }
    }
    if (x < MAX_X - 1) {
        int i = creature_at(x+1, y, creatures);
        if (i != -1 && creatures[i].type == target) {
            dirs |= EAST;
        }
    }
    if (y > 0) {
        int i = creature_at(x, y-1, creatures);
        if (i != -1 && creatures[i].type == target) {
            dirs |= NORTH;
        }
    }
    if (y < MAX_Y - 1) {
        int i = creature_at(x, y+1, creatures);
        if (i != -1 && creatures[i].type == target) {
            dirs |= SOUTH;
        }
    }
    return dirs;
}

void dijk(const char grid[MAX_X][MAX_Y], const struct creature* source, const struct creature creatures[CREATURE_COUNT], int dist[MAX_X][MAX_Y], char prev[MAX_X][MAX_Y]) {
    int qdist[MAX_X][MAX_Y];

    for(int y=0; y < MAX_Y; ++y) {
        for(int x=0; x < MAX_X; ++x) {
            dist[x][y] = INT_MAX;
            prev[x][y] = 0;

            qdist[x][y] = 1;
        }
    }
    dist[source->x][source->y] = 0;
    while(1) {
        int empty = 1;
        int min_dist = INT_MAX;
        int tx = -1;
        int ty = -1;
        for(int y=0; y < MAX_Y; ++y) {
            for(int x=0; x < MAX_X; ++x) {
                if (qdist[x][y]) {
                    empty = 0;
                    if(dist[x][y] < min_dist) {
                        min_dist = dist[x][y];
                        tx = x;
                        ty = y;
                    }
                }
            }
        }
        if(empty || (tx == -1 && ty == -1)){
            break;
        }
        qdist[tx][ty] = 0;

        char dirs = open_adjacents(tx, ty, grid, creatures);
        if((dirs & NORTH) == NORTH) {
            if (dist[tx][ty] + 1 < dist[tx][ty-1]){
                dist[tx][ty-1] = dist[tx][ty] + 1;
                prev[tx][ty-1] = NORTH;
            }
        }
        if((dirs & WEST) == WEST) {
            if (dist[tx][ty] + 1 < dist[tx-1][ty]){
                dist[tx-1][ty] = dist[tx][ty] + 1;
                prev[tx-1][ty] = WEST;
            }
        }
        if((dirs & EAST) == EAST) {
            if (dist[tx][ty] + 1 < dist[tx+1][ty]){
                dist[tx+1][ty] = dist[tx][ty] + 1;
                prev[tx+1][ty] = EAST;
            }
        }
        if((dirs & SOUTH) == SOUTH) {
            if (dist[tx][ty] + 1 < dist[tx][ty+1]){
                dist[tx][ty+1] = dist[tx][ty] + 1;
                prev[tx][ty+1] = SOUTH;
            }
        }

    }
}

int main(int argc, char * argv[]) {
    if (argc < 2) {
        return 1;
    }
    int ELF_POWER = atoi(argv[1]);

    char grid[MAX_X][MAX_Y];
    int dist[MAX_X][MAX_Y];
    char prev[MAX_X][MAX_Y];

    struct creature creatures[CREATURE_COUNT];
    int gob_count = 0;
    int elf_count = 0;
    int creature_count = 0;

    /* read the input */
    for(int y=0; y<MAX_Y; ++y) {
        for(int x=0; x<MAX_X; ++x) {
            char c;
            scanf("%c", &c);
            if (c == 'G') {
                struct creature* g = &creatures[creature_count++];
                g->type = c;
                g->x = x;
                g->y = y;
                g->health = GOB_HEALTH;
                g->power = GOB_POWER;
                g->alive = 1;
                gob_count ++;

                c = '.';
            } else if (c == 'E') {
                struct creature* e = &creatures[creature_count++];
                e->type = c;
                e->x = x;
                e->y = y;
                e->health = ELF_HEALTH;
                e->power = ELF_POWER;
                e->alive = 1;
                elf_count ++;

                c = '.';
            }
            grid[x][y] = c;
        }
        /*chomp newlin*/
        getc(stdin);
    }

    /* tick rounds */
    int round = 0;
    int last_hp = 0;
    while(1) {
        sort_creatures(creatures);
        // printf("Round %d:\n", round);
        // print_grid(grid, creatures);
        // for(int i=0; i< creature_count; ++i) {
        //     if(creatures[i].alive) {
        //         printf("\t%c - (%d, %d): hp = %d\n", creatures[i].type, creatures[i].x, creatures[i].y, creatures[i].health);
        //     }
        // }

        int target_found = 0;
        int elf_found = 1;
        for(int i=0; i<creature_count; ++i) {
            struct creature* c = &creatures[i];
            if (!c->alive) {
                continue;
            }
            char target;
            if(c->type == 'G') {
                target = 'E';
            }
            if(c->type == 'E') {
                target = 'G';
            }

            /* check if target is adjacent */
            int adj = open_adjacent_targets(c->x, c->y, target, creatures);
            if(!adj) {
                /* move */
                dijk(grid, c, creatures, dist, prev);

                int min_dist = INT_MAX;
                int target_x = -1;
                int target_y = -1;

                for(int j=0; j<creature_count; ++j) {
                    struct creature* ct = &creatures[j];
                    if(ct->type != target || !ct->alive) {
                        continue;
                    }
                    target_found = 1;
                    int x = ct->x;
                    int y = ct->y;
                    char dirs = open_adjacents(x, y, grid, creatures);

                    if((dirs & NORTH) == NORTH) {
                        if (dist[x][y-1] < min_dist){
                            min_dist = dist[x][y-1];
                            target_x = x;
                            target_y = y-1;
                        }
                    }
                    if((dirs & WEST) == WEST) {
                        if (dist[x-1][y] < min_dist){
                            min_dist = dist[x-1][y];
                            target_x = x-1;
                            target_y = y;
                        }
                    }
                    if((dirs & EAST) == EAST) {
                        if (dist[x+1][y] < min_dist){
                            min_dist = dist[x+1][y];
                            target_x = x+1;
                            target_y = y;
                        }
                    }
                    if((dirs & SOUTH) == SOUTH) {
                        if (dist[x][y+1] < min_dist){
                            min_dist = dist[x][y+1];
                            target_x = x;
                            target_y = y+1;
                        }
                    }
                }

                if(target_x != -1) {
                    //move the creature as per the path determined by dijkstra
                    int xs = target_x;
                    int ys = target_y;
                    char dir;
                    //trace the path to find the first step
                    while(dist[xs][ys] > 0) {
                        dir = prev[xs][ys];

                        if((dir & NORTH) == NORTH) {
                            ys++;
                        }
                        if((dir & EAST) == EAST) {
                            xs--;
                        }
                        if((dir & SOUTH) == SOUTH) {
                            ys--;
                        }
                        if((dir & WEST) == WEST) {
                            xs++;
                        }
                    }

                    if((dir & NORTH) == NORTH) {
                        ys--;
                    }
                    if((dir & EAST) == EAST) {
                        xs++;
                    }
                    if((dir & SOUTH) == SOUTH) {
                        ys++;
                    }
                    if((dir & WEST) == WEST) {
                        xs--;
                    }

                    //move dat ish
                    c->x = xs;
                    c->y = ys;
                }
            } else {
                //keep running even if everyone is stopped
                target_found = 1;
            }
            /* check adjancent again after moving */
            adj = open_adjacent_targets(c->x, c->y, target, creatures);
            if (adj) {
                int min_hp = INT_MAX;
                int tgt = -1;
                int t;
                if((adj & NORTH) == NORTH) {
                    t = creature_at(c->x, c->y -1, creatures);
                    if (creatures[t].health < min_hp) {
                        min_hp = creatures[t].health;
                        tgt = t;
                    }
                }
                if((adj & WEST) == WEST) {
                    t = creature_at(c->x -1, c->y, creatures);
                    if (creatures[t].health < min_hp) {
                        min_hp = creatures[t].health;
                        tgt = t;
                    }
                }
                if((adj & EAST) == EAST) {
                    t = creature_at(c->x +1, c->y, creatures);
                    if (creatures[t].health < min_hp) {
                        min_hp = creatures[t].health;
                        tgt = t;
                    }
                }
                if((adj & SOUTH) == SOUTH) {
                    t = creature_at(c->x, c->y +1, creatures);
                    if (creatures[t].health < min_hp) {
                        min_hp = creatures[t].health;
                        tgt = t;
                    }
                }
                if(tgt != -1) {
                    // attack the lowest health adjacent creature
                    struct creature* ct = &creatures[tgt];
                    ct->health -= c->power;
                    if (ct->health <= 0) {
                        if(ct->type == 'E'){
                            elf_found = 0;
                        }
                        ct->alive = 0;
                    }
                }
            }
        }

        if(!elf_found) {
            printf("Elf died!\n");
            break;
        }

        // printf("=================\n");
        int hp_total = 0;
        for(int i=0; i< creature_count; ++i) {
            if(creatures[i].alive) {
                // printf("\t(%d, %d): hp = %d\n",creatures[i].x, creatures[i].y, creatures[i].health);
                hp_total += creatures[i].health;
            }
        }
        // printf("outcome = %d\n", hp_total * round);
        // printf("=================\n");
        if (!target_found) {
            break;
        }
        last_hp = hp_total;
        // break;
        round++;
    }
    // print_grid(grid, creatures);
    int hp_total = 0;
    for(int i=0; i< creature_count; ++i) {
        if(creatures[i].alive) {
            printf("\t%c - (%d, %d): hp = %d\n", creatures[i].type, creatures[i].x, creatures[i].y, creatures[i].health);
            hp_total += creatures[i].health;
        }
    }
    if (last_hp == hp_total) {
        round --;
    }
    printf("outcome = %d\n", hp_total * round);


    return 0;
}