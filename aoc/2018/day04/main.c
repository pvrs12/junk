#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum log_type {
    ASLEEP,
    AWAKE,
    START
};

struct log {
    int year;
    int month;
    int day;
    int hour;
    int minute;

    enum log_type type;
    
    int guard_id;
};

struct guard {
    int id;
    int minutes_asleep;
    int minutes[60];
};

struct log* new_log(int year, int month, int day, int hour, int minute, enum log_type type) {
    struct log* l = malloc(sizeof(struct log));
    l->year = year;
    l->month = month;
    l->day = day;
    l->hour = hour;
    l->minute = minute;

    l->type = type;

    l->guard_id = -1;
    return l;
}

void free_log(struct log* l) {
    free(l);
    l = NULL;
}

void print_log(struct log* l) {
    char status[10];
    switch (l->type) {
        case ASLEEP:
            strncpy(status, "asleep", 10);
            break;
        case AWAKE:
            strncpy(status, "awake", 10);
            break;
        case START:
            strncpy(status, "start", 10);
            break;
    }
    printf("%04d-%02d-%02d %02d:%02d - %d - %s\n", l->year, l->month, l->day, l->hour, l->minute, l->guard_id, status);
}

int compare_logs(struct log* a, struct log* b) {
    if (a->year > b->year) {
        return 1;
    } 
    if (a->year < b->year) {
        return -1;
    }
    if (a->month > b->month) {
        return 1;
    }
    if (a->month < b->month) {
        return -1;
    }
    if (a->day > b->day) {
        return 1;
    }
    if (a->day < b->day) {
        return -1;
    }
    if (a->hour > b->hour) {
        return 1;
    }
    if (a->hour < b->hour) {
        return -1;
    }
    if (a->minute > b->minute) {
        return 1;
    }
    if (a->minute < b->minute) {
        return -1;
    }
    return 0;
}

void print_logs(struct log** logs, int logs_size) {
    for(int i=0; i<logs_size; ++i){
        print_log(logs[i]);
    }
}

///super lazy in place insertion sort
void sort_logs(struct log** logs, int logs_size) {
    for(int i=0; i<logs_size; ++i){
        for(int j=i; j<logs_size; ++j){
            if (compare_logs(logs[i], logs[j]) > 0){
                struct log* temp = logs[j];
                logs[j] = logs[i];
                logs[i] = temp;
                temp = NULL;
            }
        }
    }
}

void free_logs(struct log** logs, int logs_size) {
    for(int i=0; i<logs_size; ++i){
        free_log(logs[i]);
    }
    free(logs);
    logs = NULL;
}

struct guard* new_guard(int guard_id) {
    struct guard* g = malloc(sizeof(struct guard));
    g->id = guard_id;
    g->minutes_asleep=0;
    for(int i=0; i<60; ++i){
        g->minutes[i] = 0;
    }
    return g;
}

void free_guard(struct guard* g){
    free(g);
}

int find_guard(struct guard** guards, int guards_size, int id) {
    for(int i=0; i<guards_size; ++i) {
        if (guards[i]->id == id){
            return i;
        }
    }
    return -1;
}

void free_guards(struct guard** guards, int guards_size) {
    for(int i=0; i<guards_size; ++i){
        free_guard(guards[i]);
        guards[i] = NULL;
    }
    free(guards);
    guards=NULL;
}

int main() {
    int logs_cap = 100, logs_size = 0;
    struct log** logs = malloc(sizeof(struct log*) * logs_cap);

    int guards_cap = 100, guards_size = 0;
    struct guard** guards = malloc(sizeof(struct guard*) * guards_cap);

    while(1) {
        int year, month, day, hour, minute;
        enum log_type type;
        char s[30];
        char newline;
        if(scanf("[%d-%d-%d %d:%d] %[^\n]%c", &year, &month, &day, &hour, &minute, s, &newline) == EOF) {
            break;
        }
        if (s[0] == 'f') {
            type = ASLEEP;
        } else if (s[0] == 'w') {
            type = AWAKE;
        } else {
            type = START;
        }
        struct log* l = new_log(year, month, day, hour, minute, type);
        if (type == START) {
            sscanf(s, "Guard #%d begins shift", &(l->guard_id));
        }

        logs[logs_size++] = l;
        if (logs_size >= logs_cap) {
            logs_cap *= 2;
            logs = realloc(logs, sizeof(struct log*) * logs_cap);
        }
    }

    sort_logs(logs, logs_size);
    /*printf("\n");*/
    //get setup guard_ids
    int guard_id = -1;
    int guard_loc = -1;

    int sleep_start = 0;
    for(int i=0; i<logs_size; ++i){
        if(logs[i]->guard_id != -1){
            guard_id = logs[i]->guard_id;
            guard_loc = find_guard(guards, guards_size, guard_id);
            if(guard_loc == -1){
                //add guard to a list of guards
                struct guard* g = new_guard(guard_id);
                guard_loc = guards_size;
                guards[guards_size++] = g;
                if (guards_size >= guards_cap) {
                    guards_cap *= 2;
                    guards = realloc(guards, sizeof(struct guard*) * guards_cap);
                }
            }
        } else {
            //using the "current" guard
            if (logs[i]->type == ASLEEP) {
                sleep_start = logs[i]->minute;
            }
            if (logs[i]->type == AWAKE) {
                //if the guard woke, get how long were they asleep
                //and increase the frequency of that minute
                for(int j=sleep_start; j<logs[i]->minute;++j){
                    guards[guard_loc]->minutes[j] ++;
                }
                guards[guard_loc]->minutes_asleep += (logs[i]->minute - sleep_start);
                sleep_start = 0;
            }
            //set the id in this log (not acutally necessary, but was useful when debugging)
            logs[i]->guard_id = guard_id;
        }
    }

    int sleep_time = -1;
    int sleep_spot = -1;
    //identify the guard who slept for the longest total
    for(int i=0; i<guards_size; ++i){
        if(guards[i]->minutes_asleep > sleep_time) {
            sleep_time = guards[i]->minutes_asleep;
            sleep_spot = i;
        }
    }
    printf("Sleep Guy = %d\tTime = %d\n", guards[sleep_spot]->id, sleep_time);
    //identify during which minute this slep boi was asleep most frequently
    int common_minute = -1;
    int max_time = 0;
    for(int i=0; i<60; ++i){
        int minutes = guards[sleep_spot]->minutes[i];
        if(minutes > max_time){
            max_time = minutes;
            common_minute = i;
        }
    }
    printf("common minute = %d\n", common_minute);
    printf("part1 answer = %d\n", common_minute * guards[sleep_spot]->id);

    //identify the most frequently slept minute by a single guard
    int biggest_guard = -1;
    int biggest_minute = -1;
    int biggest_minute_size = -1;
    for(int i=0; i<guards_size; ++i){
        /*printf("guard = %d\n", guards[i]->id);*/
        for(int j=0;j<60;++j){
            /*printf("\t%02d: %d\n", j, guards[i]->minutes[j]);*/
            if (guards[i]->minutes[j] > biggest_minute_size){
                biggest_guard = i;
                biggest_minute = j;
                biggest_minute_size = guards[i]->minutes[j];
            }
        }
        /*printf("\n");*/
    }
    /*print_logs(logs, logs_size);*/
    printf("biggest guard = %d\tbiggest minute = %d\tbiggest minute size = %d\n", guards[biggest_guard]->id, biggest_minute, biggest_minute_size);
    printf("part2 answer = %d\n", biggest_minute * guards[biggest_guard]->id);

    free_logs(logs, logs_size);
    free_guards(guards, guards_size);
    return 0;
}
