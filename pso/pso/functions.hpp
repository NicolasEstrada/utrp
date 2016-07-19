//
//  functions.hpp
//  pso
//
//  A set of funcitons helpers for random numbers
//
//  Created by Nicolás Estrada on 7/18/16.
//  Copyright © 2016 Nicolás Estrada. All rights reserved.
//

#ifndef functions_h
#define functions_h


#define SWAPROWPROB 70
#define SWAPCOLPROB 30
#define INITNODEPROB 20
#define ENDNODEPROB 40
#define REPNODEPROB 60
#define REMNODEPROB 100

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

/** randChoice()
 *
 *
 *
 */
int randChoice(){
    int p = rand()%100 + 1;
    if (p <= INITNODEPROB)
        return 0;
    else
        if (p <= ENDNODEPROB)
            return 1;
        else
            if (p <= REPNODEPROB)
                return 2;
            else
                return 3;
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


#endif /* functions_h */
