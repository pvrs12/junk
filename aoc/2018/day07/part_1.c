#include <stdio.h>
#include <time.h>

struct node {
    char id;
    struct node* prereqs[10];
    size_t prereqs_len;

    int complete;
};

void new_node(struct node* n, char id) {
    n->id = id;
    n->prereqs_len = 0;
    n->complete = 0;
}

const int NODE_COUNT=26;

int main(){
    /*clock_t start = clock();*/
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    struct node nodes[NODE_COUNT];
    for(size_t i = 0; i<NODE_COUNT; ++i){
       new_node(&nodes[i], i+'A');
    }

    while(1) {
        char id_1, id_2;
        if(scanf("Step %c must be finished before step %c can begin.", &id_1, &id_2) == EOF) {
            break;
        }
        getc(stdin);
        size_t pos_1 = id_1 - 'A', pos_2 = id_2 - 'A';
        struct node* n = &nodes[pos_2];
        n->prereqs[n->prereqs_len++] = &nodes[pos_1];
    }
    for(size_t i = 0; i<NODE_COUNT; ++i){
        struct node* node_i = &nodes[i];
        if (i == NODE_COUNT -1 && node_i->complete) {
            break;
        }
        if (node_i->complete) {
            continue;
        }
        int complete = 1;
        for(int j = 0; j<node_i->prereqs_len; ++j){
            if (!node_i->prereqs[j]->complete){
                complete = 0;
                break;
            }
        }
        if(complete){
            node_i->complete = 1;
            printf("%c", node_i->id);
            i = -1;
        }
    }
    printf("\n");
    /*clock_t stop = clock();*/
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    /*printf("time=%.3fms\n", ((float)(stop - start)) / CLOCKS_PER_SEC * 1000.);*/
    long long int delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec)/ 1000.;
    printf("time=%lld us\n", delta_us);

    return 0;
}
