#include <stdio.h>
#include <stdlib.h>
#define N 10

void print(int *A, int n);
double MaxSubArrayBF(int *A, int n, int *low, int *high);
double MaxSubArray(int *A, int n, int *low, int *high);
double maxsubarray(int *A, int begin, int end, int *low, int *high);
double maxsubarrayxb(int *A, int begin, int mid, int end, int *low, int *high);
double MaxSubArrayKadane(int *A, int n, int *low, int *high);

int main(void)
{
	int i;
	int low, high;
	int A[N];
	double max;

	for (i = 0; i < N; i++) A[i] = i * (i - 5);
	print(A, N);
	// max = MaxSubArrayBF(A, N, &low, &high);
	// max = MaxSubArray(A, N, &low, &high);
	max = MaxSubArrayBF(A, N, &low, &high);
	printf("%d %d %g\n", low, high, max);

	return 0;
}

void print(int *A, int n)
{
	int i;

	for (i = 0; i < N; i++) printf("%d ", A[i]);
	printf("\n");
}

double MaxSubArrayBF(int *A, int n, int *low, int *high)
{
	int i, j, k;
	double max;
	double sum;

	max = 0;
	*low = 0;
	*high = n - 1;
	for (j = 0; j < n; j++) { // try all possible subarrays
		for (k = j; k < n; k++) {
			sum = 0;
			for (i = j; i <= k; i++) {
				sum += A[i];
			}
			if (sum >= max) {
				max = sum;
				*low = j;
				*high = k;
			}
		}
	}

	return max;
}


double MaxSubArray(int *A, int n, int *low, int *high)
{
	return maxsubarray(A, 0, n - 1, low, high);
}

double maxsubarray(int *A, int begin, int end, int *low, int *high)
{
	int mid; // middle index
	int llow, lhigh, rlow, rhigh, xlow, xhigh; // optimal range of subarrays
	double lsum, rsum, xsum; // sum of subarrays

	if (begin == end) { // termination condition
		*low = begin;
		*high = end;
		return A[begin];
	}

	mid = (begin + end) / 2; // determine middle index
	lsum = maxsubarray(A, begin,   mid, &llow, &lhigh); // left region
	rsum = maxsubarray(A, mid + 1, end, &rlow, &rhigh); // right region
	xsum = maxsubarrayxb(A, begin, mid, end, &xlow, &xhigh); // cross boundary

	if(lsum >= rsum && lsum >= xsum) { // lsum is the largest
		*low = llow;
		*high = lhigh;
		return lsum;
	}
	else if (rsum >= lsum && rsum >= xsum) { // rsum is the largest
		*low = rlow;
		*high = rhigh;
		return rsum;
	}
	*low = xlow;
	*high = xhigh;
	return xsum; // cross-boundary is the largest
}

// find the maximum earning for one-buy-one-sell stock trading (cross boundary)
double maxsubarrayxb(int *A, int begin, int mid, int end, int *low, int *high)
{
	int i;
	double lsum, rsum, sum;

	lsum = 0; // initialize for lower half
	*low = mid;
	sum = 0;
	// find low s.t. sum of A[low : mid].change is maximized
	for (i = mid; i >= begin; i--) {
		sum += A[i];
		if (sum > lsum) { // record if larger
			lsum = sum;
			*low = i;
		}
	}

	rsum = 0; // initialize for upper half
	*high = mid + 1;
	sum = 0;
	// find high s.t. sum of A[mid + 1 : high].change is maximized
	for (i = mid + 1; i <= end; i++) {
		sum += A[i] ;
		if (sum > rsum) { // record if larger
			rsum = sum;
			*high = i;
		}
	}

	return lsum + rsum; // overall sum
}

double MaxSubArrayKadane(int *A, int n, int *low, int *high)
{
	int i; // loop index
	int checkpoint; // temporary lower index
	double max; // current maximal sum
	double sum; // current sum

	*low = 0; // initialize
	*high = n - 1;
	checkpoint = 0;
	max = 0;
	sum = 0;
	for (i = 0; i < n; i++) {
		sum += A[i]; // add the value to sum
		if (sum >= max) { // record the maximum
			max = sum;
			*low = checkpoint; // record the lower index
			*high = i; // record the higher index
		}
		else if (sum <= 0) { // current sum < 0
			sum = 0; // reset
			checkpoint = i + 1; // start adding from the next index
		}
	}

	return max;
}
