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
        free(guards[i]);
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

    /*print_logs(logs, logs_size);*/
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
                struct guard* g = malloc(sizeof(struct guard));
                g->id = guard_id;
                g->minutes_asleep = 0;
                guard_loc = guards_size;
                guards[guards_size++] = g;
                if (guards_size >= guards_cap) {
                    guards_cap *= 2;
                    guards = realloc(guards, sizeof(struct guard*) * guards_cap);
                }
            }
        } else {
            if (logs[i]->type == ASLEEP) {
                sleep_start = logs[i]->minute;
            }
            if (logs[i]->type == AWAKE) {
                guards[guard_loc]->minutes_asleep += (logs[i]->minute - sleep_start);
                sleep_start = 0;
            }
            logs[i]->guard_id = guard_id;
        }
    }

    int sleep_time = -1;
    int sleep_spot = -1;
    for(int i=0; i<guards_size; ++i){
        if(guards[i]->minutes_asleep > sleep_time) {
            sleep_time = guards[i]->minutes_asleep;
            sleep_spot = i;
        }
    }
    printf("Sleep Guy = %d\tTime = %d\n", guards[sleep_spot]->id, sleep_time);

    /*print_logs(logs, logs_size);*/
    free_logs(logs, logs_size);
    free_guards(guards, guards_size);
    return 0;
}
