// Q3 EE3980 Algorithms midterm exam
// u105061110, 周柏宇
// 2020/04/28

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define cb(x) x*x*x

int N;                      // number of numbers
double *A;                  // array of numbers
int R = 1;                  // Repetitions

double GetTime(void);       // current time in sec
double D3(double *A, int N, int *low, int *high);   // to find max range
double msa(int begin, int end, int *low, int *high);
double msaXB(int begin, int mid, int end, int *low, int *high);

int main(void)
{
	int i;
	int low, high;
	double ans;

	scanf("%d", &N);
	A = (double *)malloc(sizeof(double) * N);
	for (i = 0; i < N; i++) scanf("%lf", &A[i]);

	//for (i = 0; i < N; i++) printf("%d %g\n", i, cb(A[i]));
	// printf("%lf\n", cb(20));
	//ans = msa(0, N - 1, &low, &high);
	ans = D3(A, N, &low, &high);
	printf("sum=%g, low= %d, high= %d\n", ans, low+1, high+1);
	return 0;
}

double D3(double *A, int N, int *low, int *high)    // to find max range
{
	return msa(0, N - 1, low, high);
}

double msa(int begin, int end, int *low, int *high)
{
	int mid;
	int llow, lhigh;
	int rlow, rhigh;
	int xlow, xhigh;
	double lsum, rsum, xsum;

	if (begin == end) {
		*low = begin;
		*high = end;
		return cb(A[begin]);
	}

	mid = (begin + end) / 2;
	lsum = msa(begin, mid, &llow, &lhigh);
	rsum = msa(mid + 1, end, &rlow, &rhigh);
	xsum = msaXB(begin, mid, end, &xlow, &xhigh);

	if (lsum >= rsum && lsum >= xsum) {
		*low = llow;
		*high = lhigh;
		return lsum;
	}
	else if (rsum >= lsum && rsum >= xsum) {
		*low = rlow;
		*high = rhigh;
		return rsum;
	}
	*low = xlow;
	*high = xhigh;
	return xsum;
}

double msaXB(int begin, int mid, int end, int *low, int *high)
{
	int i;
	double lsum, rsum, sum;

	lsum = 0;
	*low = mid;
	sum = 0;
	for (i = mid; i >= begin; i--) {
		sum += cb(A[i]);
		if (sum > lsum) {
			lsum = sum;
			*low = i;
		}
	}

	rsum = 0;
	*high = mid + 1;
	sum = 0;
	for (i = mid + 1; i <= end; i++) {
		sum += cb(A[i]);
		if (sum > rsum) {
			rsum = sum;
			*high = i;
		}
	}
	return lsum + rsum;
}

double GetTime(void)        // current time in sec
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}
