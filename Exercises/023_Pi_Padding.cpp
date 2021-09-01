#include<iostream>
#include<omp.h>
#define PAD 8
#define thread 8

using namespace std;
#define ld long double

int main()
{
    double time = omp_get_wtime();
    ld realpi = 3.141592653589793238462643383279l;
    long long int n = 1000000000;
    ld len = 1.0l/n;
    ld pi = 0;

    ld th_res[thread][PAD];

    omp_set_num_threads(thread);
    #pragma omp parallel
    {
        int th_ind = omp_get_thread_num();
        th_res[th_ind][0] = 0;
        ld th_pi = 0;

        for(long long int i=th_ind; i<n; i+=thread)
        {
            ld x = len*i + len/2;
            th_pi += (4.0l*len)/(1+x*x);
        }
        
        th_res[th_ind][0] = th_pi;
    }

    for(int i=0; i<thread; i++)
    {
        pi += th_res[i][0];
    }

    cout.precision(50);
    cout<<realpi<<endl;
    cout<<pi<<endl;
    cout<<(omp_get_wtime() - time)<<endl;
}