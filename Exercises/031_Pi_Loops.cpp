#include<iostream>
#include<omp.h>

using namespace std;
#define ld long double

int main()
{
    double time = omp_get_wtime();
    ld realpi = 3.141592653589793238462643383279l;
    long long int n = 1000000000;
    ld len = 1.0l/n;
    ld pi = 0;

    #pragma omp parallel for reduction(+: pi)
    for(long long int i=0; i<n; i++)
    {
        ld x = len*i + len/2;
        pi += (4.0l*len)/(1+x*x);
    }

    cout.precision(50);
    cout<<realpi<<endl;
    cout<<pi<<endl;
    cout<<(omp_get_wtime() - time)<<endl;
}