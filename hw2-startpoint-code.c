#include <stdio.h>
#include <omp.h>

//**********************************************************
// Pseudo random number generator:
//     double random
//     void seed (lower_limit, higher_limit)
//**********************************************************
//
// A simple linear congruential random number generator
// (Numerical Recipies chapter 7, 1st ed.) with parameters
// from the table on page 198j.
//
//  Uses a linear congruential generator to return a value between
//  0 and 1, then scales and shifts it to fill the desired range.  This
//  range is set when the random number generator seed is called.
//
static long MULTIPLIER  = 1366;
static long ADDEND      = 150889;
static long PMOD        = 714025;
long random_last = 0;
double random_low, random_hi;

double drandom()
{
    long random_next;
    double ret_val;

// 
// compute an integer random number from zero to mod
//
    random_next = (MULTIPLIER  * random_last + ADDEND)% PMOD;
    random_last = random_next;

//
// shift into preset range
//
    ret_val = ((double)random_next/(double)PMOD)*(random_hi-random_low)+random_low;
    return ret_val;
}
//
// set the seed and the range
//
void seed(double low_in, double hi_in)
{
   if(low_in < hi_in)
   { 
      random_low = low_in;
      random_hi  = hi_in;
   }
   else
   {
      random_low = hi_in;
      random_hi  = low_in;
   }
   random_last = PMOD/ADDEND;  // just pick something

}
//**********************************************************
// end of pseudo random generator code.
//**********************************************************

//Hint: Decide whether you use the above random number generator or not

// 
// The monte carlo pi program
//

struct d_coordinate{
double x; double y;
};
static long num_trials = 10000;
long Ncirc = 0;

struct d_coordinate p;

//Wei: you can try inline this function
void testpoint(double r, struct d_coordinate pp)
{
  struct d_coordinate z;
  z = pp;
  if ((z.x*z.x+z.y*z.y) <= r*r) {
    #pragma omp atomic
    Ncirc++;
  }


}

int main ()
{
   long i; 
   double pi, x, y, test;
   double r = 1.0;   // radius of circle. Side of squrare is 2*r 

   seed(-r, r);  // The circle and square are centered at the origin
   #pragma omp parallel for default(shared) private(p) firstprivate(r)
   for(i=0;i<num_trials; i++)
   {
      p.x = drandom(); 
      p.y = drandom();
      testpoint(r,p);
    }

    pi = 4.0 * ((double)Ncirc/(double)num_trials);

    printf("\n %ld trials, pi is %lf \n",num_trials, pi);

    return 0;
}
	  





