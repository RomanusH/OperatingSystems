#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <time.h>//library for exercise c
//let's declare the Thread Arguments
typedef struct
{
    int start, end, id, verbose;
    //range start number -s, range end number -e, number of threads -t, trace information -v
}ThreadArgs; 
//First let's make program that will find perfect digital invariant numbers
int pdi(int num) {
    int sum=0, temp=num, digits=0;

    while (temp > 0){
        temp /= 10;
        digits++;
    }

    temp = num;

    while (temp > 0){
        int digit = temp%10;
        sum += pow(digit, digits);
        temp /= 10;
    }
    return sum == num;
}
void *pdi_find(void *args){
    ThreadArgs *targs = (ThreadArgs *)args;
    int start = targs->start;
    int end = targs->end;
    int id = targs->id;
    int verbose = targs->verbose;
    if(verbose){ //Theese are the requirement of exercise b, giving information about threads id and thread range and etc.
    //the beginning search info
        fprintf(stderr, "pdi-numbers: t%d searching [%d,%d]\n", id, start, end);
    }
    for(int i = start; i <= end; i++){
        if (pdi(i)){
            printf("%d\n", i);
        }
    }
    if(verbose){
        //final report about all threads have been finished search
        fprintf(stderr, "pfi-numbers: t%d finishing\n", id);
    }
    return NULL;
}
int main(int argc, char * argv[]){
    int start = 1, end = 10000, num_threads = 1, verbose = 0;//theese are default values
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            start = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            end = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            num_threads = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        }
    }

    pthread_t threads[num_threads];
    ThreadArgs targs[num_threads];
    int range = (end - start + 1) / num_threads;
    clock_t begin = clock();
    for (int i = 0; i < num_threads; i++) {
        targs[i].start = start + i * range;
        targs[i].end = (i == num_threads - 1) ? end : targs[i].start + range - 1;
        targs[i].id = i;
        targs[i].verbose = verbose;

        pthread_create(&threads[i], NULL, pdi_find, &targs[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_t finish = clock();//exercise c requirement about time elapsed
    double elapsedtime = (double) (finish - begin) / CLOCKS_PER_SEC;//During the execution -s 1 -e 100000 I am getting 0.02 secs
    printf("Time ellapsed while execution:%.2f \n", elapsedtime);
}




