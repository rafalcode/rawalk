/* get 0's, 1's, 2, and 3's randomly using as few calles to rand() as possible. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// macro defintions to easily change values/
#define SEED 112
#define N 100

void rfill16(int *p, int totnum)
{
    int i=0, j, rn;
    do {
        rn=rand();
        for(j=0;j<16;j++) {
            p[i]=(rn>>j)&0x03;
            i++;
            if(i>=totnum) // if totnum under a multiple of 16
                return;
        }
    } while(i<totnum); // check at end allows one extra loop even when i>totnum
    return;
}

int main (int argc, char *argv[])
{
    srand(SEED); // 
    int i, n=N;
    int *p=malloc(n*sizeof(int)); // allocate memory
    rfill16(p, n);
    for(i=0;i<n;++i) 
        printf("%i ", p[i]);
    printf("\n"); 
    free(p);
    return 0;
}
