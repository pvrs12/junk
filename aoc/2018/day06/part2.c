#include <stdio.h>

struct point {
	int x;
	int y;
	int area;
};

int manhattan_total(int x, int y, const struct point *points, size_t points_len) {
	int total_dist =0;
	for(int i=0; i<points_len; ++i){
		total_dist += abs(points[i].x - x) + abs(points[i].y - y);
		if (total_dist > 10000) {
			return 0;
		}
	}
	return 1;
}

int manhattan(int x, int y, const struct point *points, size_t points_len) {
	int min_dist = 10000;
	int closest[5];
	size_t closest_len =0;
	for (int i=0; i< points_len; ++i){
		int dist = abs(points[i].x - x) + abs(points[i].y - y);
		if (dist == min_dist){
			closest[closest_len++] = i;
		}
		if (dist < min_dist){
			closest_len = 0;
			closest[closest_len++] = i;
			min_dist = dist;
		}
	}
	int closest_point = closest[0];
	if (closest_len > 1) {
		return -1;
	} else {
		return closest_point;
	}
}

int main() {
	size_t points_len = 0, points_cap=100;
	struct point points[51]; 

	int minx=100, maxx=0, miny=100, maxy=100;
	while(1) {
		int x, y;
		if (scanf("%d, %d", &x, &y) == EOF) {
			break;
		}
		if (x < minx){
			minx = x;
		}
		if (x > maxx) {
			maxx = x;
		}
		if (y < miny) {
			miny =y;
		}
		if (y > maxy){
			maxy = y;
		}

		struct point p;
		p.x= x;
		p.y=y;
		p.area = 0;
		points[points_len++] = p;
		
	}

	int area = 0;
	for(int x=minx; x<= maxx; ++x){
		for(int y=miny; y<= maxy; ++y){
			if (manhattan_total(x, y, points, points_len)) {
				area += 1;
			}
			int closest_point = manhattan(x, y, points, points_len);
			if (closest_point != -1 && points[closest_point].area != -1) {
				points[closest_point].area += 1;
			}
			if (x == minx || x == maxx || y == miny || y == maxy) {
				points[closest_point].area = -1;
			}
		}
	}

	int max_area = 0;
	for(int i=0; i<points_len; ++i){
		if (points[i].area > max_area) {
			max_area = points[i].area;
		}
	}
	printf("part 1 = %d\n",max_area);
	printf("part 2 = %d\n", area);

	return 0;
}
