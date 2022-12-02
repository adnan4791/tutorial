#include <cilk/cilk.h>
#include <stdio.h>
#include <stdlib.h>

long num_step;
double delta;
long nb;
double compute_pi(long lower, long upper) {
    double sum = 0.0;
    double x = 0.0;
    double left = 0.0;
    double right = 0.0;
   
    if((lower+nb) >= upper ) {
        for(long i=lower;i<upper;i++) {
            x = (lower+0.5) * delta;
            sum += 4.0/(1+x*x) * delta;
        }
        return sum;
    } 
    else {
        right = cilk_spawn compute_pi(lower,(lower+upper)/2);
        left = compute_pi((lower+upper)/2,upper);
        cilk_sync;
        sum = left+right; // where is the reducer ?
        return sum;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please add problem size argument\n Program exited...\n");   
        exit(0);
    }
    num_step = atol(argv[1]);
    delta = 1.0/num_step;
    nb = 1000;  
    double pi = compute_pi(0,num_step);
    printf("%f\n",pi);
    return 0;
}