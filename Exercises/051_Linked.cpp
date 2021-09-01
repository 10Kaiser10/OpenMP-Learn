#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

#define numT 8

struct node
{
    int data;
    int fibdata;
    struct node *next;
};

int fib(int n)
{
    int x, y;
    if (n < 2)
    {
        return (n);
    }
    else
    {
        x = fib(n - 1);
        y = fib(n - 2);
        return (x + y);
    }
}

void processwork(struct node *p)
{
    int n;
    n = p->data;
    p->fibdata = fib(n);
}

struct node *init_list(struct node *p)
{
    int i;
    struct node *head = NULL;
    struct node *temp = NULL;

    head = (struct node *)malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i = 0; i < N; i++)
    {
        temp = (struct node *)malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[])
{
    double start, end;
    struct node *p = NULL;
    struct node *temp = NULL;
    struct node *head = NULL;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

    start = omp_get_wtime();
    omp_set_num_threads(numT);

    int threads;
    int* tasks;
    node **starts;
    
    #pragma omp parallel
    {
        #pragma omp master
        {
            threads = omp_get_num_threads();
            tasks = (int *)malloc(threads*sizeof(int));
            starts = (node**)malloc(threads*sizeof(node*));
            int len = 0;

            node *tra = head;

            while(tra!=NULL)
            {
                len++;
                tra = tra->next;
            }

            for(int i=0; i<threads; i++)tasks[i] = len/threads;
            for(int i=0; i<len%threads; i++)tasks[i]++;
            
            tra = head;
            for(int i=0; i<threads; i++)
            {
                starts[i] = tra;
                for(int j=0; j<tasks[i] && tra!=NULL; j++)
                {
                    tra = tra->next;
                }
            }
        }
        #pragma omp barrier

        int id = omp_get_thread_num();
        node* curr = starts[id];
        
        for(int i=0; i<tasks[id]; i++)
        {
            processwork(curr);
            if(curr!=NULL)curr = curr->next;
        }
    }

    end = omp_get_wtime();
    p = head;
    while (p != NULL)
    {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Compute Time: %f seconds\n", end - start);

    return 0;
}