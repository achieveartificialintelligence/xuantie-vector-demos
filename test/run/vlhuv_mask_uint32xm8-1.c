#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<riscv-vector.h>

#define vlen 128 
#define sew 32
#define vmul 8
#define ISUNSIGN 0
#define VCTYPE "uint32"

int main(int argc, char **argv) {
        int vlmax = vmul*vlen/sew;  // vmax <= 8/1
        int N=6;
        int avl[6] = {vlmax, vlmax-1, 2*vlmax-1, 2*vlmax+1000, 2*vlmax, 0};
	unsigned int input1[vlmax];
        unsigned short int input2[vlmax];
	unsigned int input3[vlmax];
        unsigned int expect;
        uint32xm8_t bb;
        int rte = 0;
        int gvl=0;
        unsigned int vst = 0;
        long long max = 0;
        long long min = 0;
        char *name = argv[0];
        if (argc != 3*vlmax+1)
        {
                for(int j=0;j<vlmax;j++){
                        input1[j] = j;
                        input2[j] = j;
                        input3[j] = j;
                }
        } else {
        	for(int j=0;j<vlmax;j++){
        	        input1[j] = strtol(argv[j+1],0,10);
        	        input2[j] = strtol(argv[j+1+vlmax],0,10);
        	        input3[j] = strtol(argv[j+1+2*vlmax],0,10);
        	}
	}
        uint32xm8_t *a1 = (uint32xm8_t *)input1;
	const unsigned int *a2 = (const unsigned int *)input2;
	uint32xm8_t *a8 = (uint32xm8_t *)input3;
	e32xm8_t a3;

        for(int t=0; t<N; t++) {
                gvl = vsetvli(avl[t], RVV_E32, RVV_M8);
		a3 = vmsnevi_e32xm8_uint32xm8(*a8, 0, gvl);
                bb=vlhuv_mask_uint32xm8(*a1, a2, a3, gvl);
                for(int i=0; i<vlmax; i++) {
			if ( i < avl[t] )  {
				if( input3[i%vlmax]!=0 ) {
                       		        expect = input2[i];
				  } else {
					expect = input1[i];
				  }
			} else {
                       		      expect =  0;
                       	}

                        if ( bb[i]- expect != 0) {
                                rte = -1;
                        }
                        printf("Expect=%d,Actual=%d, avl=%d\n",expect, bb[i], avl[t]);
                }
        }

        if ( rte != 0 )
                printf("FAIL: %s\n",name);
        else
                printf("PASS: %s\n",name);
        return rte;
}
