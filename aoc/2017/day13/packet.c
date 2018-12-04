#include <stdio.h>

int cycle_depth(int step, int depth){
	int c_depth = (2 * depth - 2);
	int j;
	if(c_depth < 1){
		j = step % depth; 
	} else {
		j = step % c_depth;
		if(j >= depth) {
			j = -1 * (j - c_depth);
		}
	}
	return j;
}
 
int main(){
	int step = 0, layer, depth, badness = 0, cd;
	while(scanf("%d %d", &layer, &depth) != EOF){
		while(layer != step){
			step++;
		}
		cd = cycle_depth(step, depth);
		if(!cd){
			badness += depth * step;
		}
	}
	printf("%d\n", badness);

	return 0;
}
