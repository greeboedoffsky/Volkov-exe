/*
* Volkov-exe.c
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


double  get_random_value() {
    return rand()*1.0/RAND_MAX;
}

/*void metropolis_step(double x, double y){

}*/

int main(void){
    double x, y;
    long n = 0;
    long N = 1000000;

    srand(time(NULL));

    for (long i = 0; i < N; i++)
    {
         x = get_random_value();
         y = get_random_value();

        if (y*y <= (1-x*x))
        {
        n++;
        }
    }
    printf("pi = %lf", (n*4.0)/N);

    return 0;
}