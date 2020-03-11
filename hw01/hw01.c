// EE3980 HW01 Quadratic Sorts 
// 105061110 周柏宇
// 2020/03/10

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void SelectionSort(char **list, int n);
void InsertionSort(char **list, int n);
void BubbleSort(char **list, int n);
void ShakerSort(char **list, int n);
// double GetTime(void);

int main(void)
{
    int i; // index

    // t = GetTime()
    for (i = 0; i < 500; i++) {
        ;
    }
    // printf("%f\n", t - GetTime());
    return 0;
}

// double GetTime(void)
// {
//     struct timeval tv;

//     gettimeofday(&tv, NULL);
//     return tv.tv_sec + 1e-6 * tv.tv_usec;
// }
