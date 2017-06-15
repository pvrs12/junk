#include <stdio.h>

int main() {
	double a;
	double b;
	
	scanf("%lf", &a);
	scanf("%lf", &b);

	double c = a - 5;
	for(int i=0; i< 100; ++i){
		printf("a=%0.lf b=%0.lf c=%0.lf\n", a, b, c);
		
	}
}
