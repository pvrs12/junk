#include <stdio.h>
#include <time.h>

struct node {
    char id;
    struct node* prereqs[10];
    size_t prereqs_len;

    int complete;
    int seconds_remaining;
    int processing;
};

void new_node(struct node* n, char id) {
    n->id = id;
    n->prereqs_len = 0;
    n->complete = 0;
    n->processing = 0;

    n->seconds_remaining = id - 'A' + 61;
}

#ifdef SHORT
const int WORKER_COUNT=2;
const int NODE_COUNT=6;
#else
const int WORKER_COUNT=5;
const int NODE_COUNT=26;
#endif


int main(){
    clock_t start = clock();
    struct node nodes[NODE_COUNT];
    for(size_t i = 0; i<NODE_COUNT; ++i){
       new_node(&nodes[i], i+'A');
    }

    struct node* workers[WORKER_COUNT];
    for(size_t i=0; i<WORKER_COUNT; ++i){
        workers[i] = NULL;
    }
#ifdef DOT
    fprintf(stderr, "digraph foo {\n");
#endif

    while(1) {
        char id_1, id_2;
        if(scanf("Step %c must be finished before step %c can begin.", &id_1, &id_2) == EOF) {
            break;
        }
        getc(stdin);
        size_t pos_1 = id_1 - 'A', pos_2 = id_2 - 'A';
        struct node* n = &nodes[pos_2];
        n->prereqs[n->prereqs_len++] = &nodes[pos_1];
#ifdef DOT
        fprintf(stderr, "%c -> %c;\n", id_1, id_2);
#endif
    }
#ifdef DOT
    fprintf(stderr, "}\n");
#endif

    int seconds = 0;
    int complete = 0;
    while(!complete) {
        for(size_t i = 0; i<NODE_COUNT; ++i){
            struct node* node_i = &nodes[i];

            if (node_i->complete || node_i->processing) {
                continue;
            }
            int ready = 1;
            for(int j = 0; j<node_i->prereqs_len; ++j){
                if (!node_i->prereqs[j]->complete){
                    ready = 0;
                    break;
                }
            }

            if(ready){
                for(int j=0; j<WORKER_COUNT; ++j){
                    if (!workers[j]) {
                        node_i->processing=1;
                        workers[j] = node_i;
                        /*i=-1;*/
                        break;
                    }
                }
            }
        }

        int run_workers = 0;
        for(int i = 0; i<WORKER_COUNT; ++i){
            if(workers[i]) {
                run_workers = 1;
                break;
            }
        }
        if(run_workers){
            printf("%d\t", seconds);
            for(int i=0; i<WORKER_COUNT; ++i){
                if(!workers[i]){
                    printf(".\t");
                    continue;
                }
                printf("%c\t", workers[i]->id);
                workers[i]->seconds_remaining--;
                if(workers[i]->seconds_remaining == 0) {
                    workers[i]->complete = 1;
                    fprintf(stderr,"%c", workers[i]->id);
                    workers[i]->processing = 0;
                    workers[i] = NULL;
                    /*i=-1;*/
                }
            }
            seconds++;
            printf("\n");
        }
        int done = 1;
        for (int j =0; j<NODE_COUNT;++j){
            if (!nodes[j].complete){
                done = 0;
                break;
            }
        }
        complete = done;
    }

    fprintf(stderr,"\n");
    clock_t stop = clock();
    printf("seconds=%d\n", seconds);
    printf("time=%.3f\n", ((float)(stop - start)) / CLOCKS_PER_SEC * 1000);

    return 0;
}
