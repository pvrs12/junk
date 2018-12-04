#include <iostream>
#include <map>

int adjacent_sum( 

int main(){
	int input = 312051;

	std::map<std::pair<int, int>, int> spiral;

	int n = 1, x = 0, y = 0;

	while(spiral[std::make_pair(x, y)] < 312051){
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

		sprial[std::make_pair(x, y)] = n++;
	}
}
