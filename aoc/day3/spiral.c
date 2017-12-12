#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

int min(int first, int second, int third, int fourth){
	if(first <= second && first <= third && first <= fourth){
		return first;
	}
	if(second <= first && second <= third && second <= fourth){
		return second;
	}
	if(third <= first && third <= second && third <= fourth) {
		return third;
	}
	if(fourth <= first && fourth <= second && fourth < third) {
		return fourth;
	}
	return 0xFFFFFF;
}

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("You must provide n\n");
		exit(1);
	}
	int n = atoi(argv[1]);

	int r = ceil((sqrt(n)-1)/2);

	int b_r = pow(2 * r + 1, 2);
	int side_size = (int)sqrt(b_r);
	int b_l = b_r - side_size + 1;
	int t_l = b_l - side_size + 1;
	int t_r = t_l - side_size + 1;

	int b_m = b_r - side_size / 2;
	int l_m = b_l - side_size / 2;
	int t_m = t_l - side_size / 2;
	int r_m = t_r - side_size / 2;

	int to_mid = min(abs(n - b_m), abs(n - l_m),abs( n - t_m), abs(n - r_m));
	int to_center = r;

	printf("B_R=%d\tB_L=%d\tT_L=%d\tT_R=%d\n", b_r, b_l, t_l, t_r);
	printf("B_M=%d\tL_M=%d\tT_M=%d\tR_M=%d\n", b_m, l_m, t_m, r_m);
	printf("Mid=%d\tCent=%d\n", to_mid, to_center);
	printf("%d\n", to_mid + to_center);
	return 0;
}
