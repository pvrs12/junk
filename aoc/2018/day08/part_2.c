#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct node {
    struct node* children[10];
    int child_count;

    int metas[12];
    int meta_count;
    int sum;
};

void new_node(struct node* n) {
    n->child_count = 0;
    n->sum = -1;
    n->meta_count = 0;
}

int sum_node(struct node* n) {
    int sum = 0;
    if(n->child_count < 1){
        for(int i=0; i<n->meta_count;++i){
            sum += n->metas[i];
        }
    } else {
        for(int i=0; i<n->meta_count; ++i){
            int meta = n->metas[i] - 1;
            if (meta >= n->child_count){
                sum += 0;
            } else {
                int child_sum = n->children[meta]->sum;
                if(child_sum < 0) {
                    child_sum = sum_node(n->children[meta]);
                }
                sum += child_sum;
            }
        }
    }
    return sum;
}

struct node* build_tree(struct node* parent) {
    int child_count, meta_count;
    scanf("%d %d ", &child_count, &meta_count);
    struct node* self = malloc(sizeof(struct node));
    new_node(self);
    if(parent != NULL){
        parent->children[parent->child_count++] = self;
    }

    for(int i=0; i< child_count; ++i){
        build_tree(self);
    }
    for(int i=0; i< meta_count; ++i){
        int meta;
        scanf("%d ", &meta);
        self->metas[self->meta_count++] = meta;
    }
    return self;
}

void free_nodes(struct node* root) {
    for(int i=0; i<root->child_count; ++i){
        free_nodes(root->children[i]);
    }
    free(root);
    root = NULL;
}

int main(){
    clock_t start = clock();
    struct node* root = build_tree(NULL);
    int sum = sum_node(root);
    printf("%u\n", sum);
    free_nodes(root);
    clock_t end = clock();
    printf("time = %.3f\n", ((float)(end-start)/CLOCKS_PER_SEC * 1000));
    return 0;
}
