#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct marble {
    int number;

    struct marble* next;
    struct marble* prev;
};

void remove_marble(struct marble* m) {
    struct marble* prev = m->prev;
    m->prev->next = m->next;
    m->next->prev = prev;

    free(m);
    m = NULL;
}

struct marble* new_marble(int number, struct marble* current, unsigned long long* player_score) {
    if (number % 23 == 0) {
        (*player_score) += number;
        struct marble* seven = current -> prev -> prev -> prev -> prev -> prev -> prev -> prev;
        struct marble* current = seven -> next;
        (*player_score) += seven->number;
        remove_marble(seven);
        return current;
    }
    struct marble* new = malloc(sizeof(struct marble));

    struct marble* new_prev = current -> next;
    struct marble* new_next = new_prev -> next;

    new_prev -> next = new;
    new_next -> prev = new;

    new->number = number;

    new->next = new_next;
    new->prev = new_prev;

    return new;
}

void free_marbles(struct marble* m){
    struct marble* start = m;
    m = m->next;
    while(m->number != start->number) {
        /*printf("freeing marble: %d\n", m->number);*/
        struct marble* next = m->next;
        free(m);
        m = next;
    }
    free(start);
}

void print_chain(struct marble* current) {
    struct marble* start = current;
    printf("%d ", start->number);
    current = current -> next;
    while(start->number != current->number){
        printf("%d ", current->number);
        current = current -> next;
    }
    printf("\n");
}

const int PLAYERS = 452;
const int MARBLES = 70784*100;
/*const int PLAYERS = 10;*/
/*const int MARBLES = 1618;*/
/*const int PLAYERS = 9;*/
/*const int MARBLES = 25;*/

int main() {
    clock_t start = clock();
    struct marble* current = malloc(sizeof(struct marble));
    /*struct marble* zero = current;*/

    current -> number = 0;

    current -> next = current;
    current -> prev = current;
    
    unsigned long long players[PLAYERS];
    for(int i=0; i<PLAYERS; ++i){
        players[i] = 0;
    }

    for(int i=0; i<MARBLES; ++i){
        current = new_marble(i + 1, current, &players[i % PLAYERS]);
        /*print_chain(zero);*/
    }
    unsigned long long max_score = 0;
    for(int i=0;i<PLAYERS; ++i){
        if(players[i] > max_score) {
            max_score = players[i];
        }
    }
    printf("score = %llu\n", max_score);
    free_marbles(current);
    clock_t end = clock();
    printf("%.3fms\n", ((float)(end - start))/CLOCKS_PER_SEC * 1000);
}
