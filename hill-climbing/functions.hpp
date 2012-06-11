#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define SWAPROWPROB 70
#define SWAPCOLPROB 30

using namespace std;

/** 
    * Function that generates random numbers.
    * 
    * @param void
    *
    * @return A random number in 0~1 range.
    */
double saRandom(void){

	return (double)(rand()%10000 + 1)/10000;
}

/**
    * Math stochastic function
    * 
    * @param delta Value to be performed.
    * @param t Recent value.
    *
    * @return A reference to this object.
    */
int isAccepted(double delta, double t){

	double random, prob, exponent;
	random = saRandom();
	exponent = (double)-delta/t;
	prob = exp(exponent);
	//cout << "Random = " << random << ",\t Prob = "<< prob << ",\t ";
	if (random < prob)
			return 1;
	else
			return 0;
}

/** moveRand
    * 
    * Random between 0 and 1.
    *
    * @return 0 swapRow, 1 swapColumn
    */
int moveRand()
{
	if ((rand()%100 + 1) < SWAPROWPROB)
        return 0;
    else
        return 1;
}

/** moveRand
    * 
    * Function that choose random
    *number between 1 and n.
    *
    * @param n Upper limit.
    * 
    * @return number betweeen 1 y n.
    */
int nRand(int n)
{
	return (rand()%n +1);
}
#endif
