#include <cilk/cilk.h>
#include <stdio.h>
#include <stdlib.h>

long num_step;
double delta;
long nb;
double compute_pi(long lower, long upper) {
    double sum[1] = {0.0};
    double x = 0.0;
   
    if((lower+nb) >= upper ) {
        for(long i=lower;i<upper;i++) {
            x = (i+0.5) * delta;
            sum[0] += 4.0/(1+x*x) * delta;
        }
        return sum[0];
    } 
    else {
        double *left = sum;
        double right = cilk_spawn compute_pi(lower,(lower+upper)/2);
        *left = compute_pi((lower+upper)/2,upper);
        cilk_sync;
        *left = *left+right; 
        return sum[0];
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
