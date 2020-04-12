// EE3980 HW05 Trading Stock
// 105061110, 周柏宇
// 2020/04/12

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


// data structure to store stock info
typedef struct sSTKprice {
	int year, month, day;
	double price, change;
} STKprice;

int N;				// input size
int R_BF = 1;		// repetition for brute-force approach
int R_DnC = 1000;	// repetition for divide and conquer approach
STKprice *data;		// array of stock info

void readData(void);	// read stock history
double GetTime(void);	// get local time in seconds
// find the maximum earning for one-buy-one-sell stock trading (brute-force)
double MaxSubArrayBF(int *low, int *high);
// find the maximum earning for one-buy-one-sell stock trading (divide&conquer)
double MaxSubArray(int begin, int end, int *low, int *high);
// find the maximum earning for one-buy-one-sell stock trading (cross boundary)
double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high);

int main(void)
{
	int i;				 // loop index
	int lowBF, highBF;	 // lower/higher index of the subarray (brute-force)
	int lowDnC, highDnC; // lower/higher index of the subarray (divide&conquer)
	STKprice *bDayBF, *sDayBF;	  // buy/sell day (brute-force)
	STKprice *bDayDnC, *sDayDnC;  // buy/sell day (divide&conquer)
	double earningBF, earningDnC; // earning by brute-force and divide&conquer
	double t0, t1, t2;	 // time stamp

	readData();		// read stock history

	t0 = GetTime();	// record time
	for (i = 0; i < R_BF; i++) {
		earningBF = MaxSubArrayBF(&lowBF, &highBF);
	}
	t1 = GetTime();	// record time
	for (i = 0; i < R_DnC; i++) {
		earningDnC = MaxSubArray(0, N - 1, &lowDnC, &highDnC);
	}
	t2 = GetTime();	// record time

	// convert the index to the ctaul buy/sell date
	bDayBF = &data[(lowBF > 0) * (lowBF - 1)];
	sDayBF = &data[highBF];
	bDayDnC = &data[(lowDnC > 0) * (lowDnC - 1)];
	sDayDnC = &data[highDnC];
	// print input size, buy/sell date and price and earning of both approaches
	printf("N = %d\n", N);
	printf("Brute-force approach: time %.5e s\n", (t1 - t0) / R_BF);
	printf("  Buy: %d/%d/%d", bDayBF->year, bDayBF->month, bDayBF->day);
	printf(" at %g\n", bDayBF->price);
	printf("  Sell: %d/%d/%d", sDayBF->year, sDayBF->month, sDayBF->day);
	printf(" at %g\n", sDayBF->price);
	printf("  Earning: %g per share.\n", earningBF);
	printf("Divide and Conquer: time %.5e s\n", (t2 - t1) / R_DnC);
	printf("  Buy: %d/%d/%d", bDayDnC->year, bDayDnC->month, bDayDnC->day);
	printf(" at %g\n", bDayDnC->price);
	printf("  Sell: %d/%d/%d", sDayDnC->year, sDayDnC->month, sDayDnC->day);
	printf(" at %g\n", sDayDnC->price);
	printf("  Earning: %g per share.\n", earningDnC);

	free(data);	// release allocated memory

	return 0;
}

void readData(void) // read stock history
{
	int i; // loop index

	scanf("%d", &N); // input size
	data = (STKprice *)malloc(sizeof(STKprice) * N); // allocate memory
	for (i = 0; i < N; i++) { // input stock info
		scanf("%d", &data[i].year);
		scanf("%d", &data[i].month);
		scanf("%d", &data[i].day);
		scanf("%lf", &data[i].price);
		// calculate price change
		data[i].change = data[i].price - data[(i > 0) * (i - 1)].price;
	}
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}

// find the maximum earning for one-buy-one-sell stock trading (brute-force)
double MaxSubArrayBF(int *low, int *high)
{
	int i, j, k;	// loop indices
	double max;		// maximal sum of subarray
	double sum;		// sum of subarray

	max = 0; // initialize
	*low = 0;
	*high = N - 1;
	for (j = 0; j < N; j++) { // try all possible subarrays
		for (k = j; k < N; k++) {
			sum = 0;
			for (i = j; i <= k; i++) { // summation for data[j:k].change
				sum += data[i].change;
			}
			if (sum > max) { // record the maximal sum and range
				max = sum;
				*low = j;
				*high = k;
			}
		}
	}

	return max;
}

// find the maximum earning for one-buy-one-sell stock trading (divide&conquer)
double MaxSubArray(int begin, int end, int *low, int *high)
{
	int mid; // middle index
	int llow, lhigh, rlow, rhigh, xlow, xhigh; // optimal range of subarrays
	double lsum, rsum, xsum; // sum of subarrays

	if (begin == end) { // termination condition
		*low = begin;
		*high = end;
		return data[begin].change;
	}

	mid = (begin + end) / 2; // determine middle index
	lsum = MaxSubArray(begin, mid, &llow, &lhigh); // left region
	rsum = MaxSubArray(mid + 1, end, &rlow, &rhigh); // right region
	xsum = MaxSubArrayXB(begin, mid, end, &xlow, &xhigh); // cross boundary

	if (lsum >= rsum && lsum >= xsum) { // lsum is the largest
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
	return xsum; // cross boundary is the largest
}

// find the maximum earning for one-buy-one-sell stock trading (cross boundary)
double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high)
{
	int i;
	double lsum, rsum, sum;

	lsum = 0; // initialize for lower half
	*low = mid;
	sum = 0;
	// find low s.t. sum of A[low:mid].change is maximized
	for (i = mid; i >= begin; i--) {
		sum += data[i].change;
		if (sum > lsum) { // record if larger
			lsum = sum;
			*low = i;
		}
	}

	rsum = 0; // initialize for upper half
	*high = mid + 1;
	sum = 0;
	// find high s.t. sum of A[mid+1:high].change is maximized
	for (i = mid + 1; i <= end; i++) {
		sum += data[i].change;
		if (sum > rsum) { // record if larger
			rsum = sum;
			*high = i;
		}
	}

	return lsum + rsum; // overall sum
}
