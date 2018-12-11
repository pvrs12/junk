#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include <time.h>

struct point {
    int x;
    int y;

    int vx;
    int vy;
};

void move_point(struct point* p){
    p->x += p->vx;
    p->y += p->vy;
}

void reverse_point(struct point* p){
    p->x -= p->vx;
    p->y -= p->vy;
}

int max_dist(struct point* points, int point_len){
    int maxx=-50000, maxy=-50000;
    int minx=50000, miny=50000;

    for(int i=0; i<point_len; ++i){
        if(points[i].x > maxx) {
            maxx = points[i].x;
        }
        if(points[i].y > maxy) {
            maxy = points[i].y;
        }
        if(points[i].x < minx) {
            minx = points[i].x;
        }
        if(points[i].y < miny) {
            miny = points[i].y;
        }
    }
    return abs(maxx - minx) + abs(maxy - miny);
}

void print_points(struct point* points, int point_len){
    int maxx=-50000, maxy=-50000;
    int minx=50000, miny=50000;

    for(int i=0; i<point_len; ++i){
        if(points[i].x > maxx) {
            maxx = points[i].x;
        }
        if(points[i].y > maxy) {
            maxy = points[i].y;
        }
        if(points[i].x < minx) {
            minx = points[i].x;
        }
        if(points[i].y < miny) {
            miny = points[i].y;
        }
    }
    for(int y = miny; y <= maxy; ++y){
        for(int x = minx; x <= maxx; ++x){
            int draw_point = -1;
            for(int i=0;i<point_len;++i){
                if (points[i].x == x && points[i].y == y){
                    draw_point = i;
                    break;
                }
            }
            if (draw_point != -1){
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main() {
    clock_t start = clock();
    struct point points[360];
    int point_len = 0;
    while(1) {
        struct point* p = &points[point_len++];
        if(scanf("position=<%d, %d> velocity=<%d, %d>\n", &(p->x), &(p->y), &(p->vx), &(p->vy)) == EOF) {
            break;
        }
    }
    /* last point is :trashbag: for some reason */
    point_len --;

    int seconds = 0;
    int min_dist = INT_MAX;
    while(1) {
        int dist = max_dist(points, point_len);
        if (dist < min_dist){
            min_dist = dist;
        }
        /* if we start going back up then we found our message*/
        if (dist > min_dist){
            for(int i=0; i<point_len; ++i){
                reverse_point(&points[i]);
            }
            printf("seconds = %d\n", seconds - 1);
            print_points(points, point_len);
            break;
        }
        for(int i=0; i<point_len; ++i){
            move_point(&points[i]);
        }
        
        seconds++;
    }
    clock_t end = clock();
    printf("%.3fms\n", ((float)(end - start))/ CLOCKS_PER_SEC * 1000);
    return 0;
}
