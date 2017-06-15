#include <stdio.h>

const int ITERATIONS = 10000;

int main(){

	double population;
	double initialInfected;

	double infectionRate;
	double recoveryRate;
	double vaccinationRate;

	scanf("%lf", &population);
	scanf("%lf", &initialInfected);
	scanf("%lf", &infectionRate);
	scanf("%lf", &recoveryRate);
	scanf("%lf", &vaccinationRate);
	/*scanf("%lf %lf %lf %lf %lf", &population, &initialInfected, &infectionRate, &recoveryRate, &vaccinationRate);*/
	double susceptibleCount = population - initialInfected;
	double infectedCount = initialInfected;
	double immuneCount = 0;
	for(int i=0; i < ITERATIONS; ++i){
		printf("%0.lf %0.lf %0.lf\n", susceptibleCount, infectedCount, immuneCount);

		double newInfections = susceptibleCount * newInfections;
		double newVacc = susceptibleCount * vaccinationRate;
		double newRecovered = infectedCount * recoveryRate;
		printf("infRate = %lf, recovRate = %lf, vaccRate = %lf\n", infectionRate, recoveryRate, vaccinationRate);
		printf("newInf = %0.lf, newVacc = %0.lf, newRecov = %0.lf\n", newInfections, newVacc, newRecovered);

		susceptibleCount -= newInfections - newVacc;
		immuneCount += newVacc + newRecovered;
		infectedCount += newInfections - newRecovered;
	}
	return 0;
}
