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

    long long int thread = 8;

    omp_set_num_threads(thread);
    #pragma omp parallel
    {
        int th_ind = omp_get_thread_num();
        ld th_pi = 0;

        for(long long int i=th_ind; i<n; i+=thread)
        {
            ld x = len*i + len/2;
            th_pi += (4.0l*len)/(1+x*x);
        }
        
        #pragma omp critical
        {
            pi += th_pi;
        }
    }

    cout.precision(50);
    cout<<realpi<<endl;
    cout<<pi<<endl;
    cout<<(omp_get_wtime() - time)<<endl;
}