// EE3980 HW06 Trading Stock II
// 105061110, 周柏宇
// 2020/04/16

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
int R_hw6 = 500;
STKprice *data;		// array of stock info

void readData(void);	// read stock history
double GetTime(void);	// get local time in seconds
// find the maximum earning for one-buy-one-sell stock trading (brute-force)
double MaxSubArrayBF(int *low, int *high);
// find the maximum earning for one-buy-one-sell stock trading (divide&conquer)
double MaxSubArray(int begin, int end, int *low, int *high);
// find the maximum earning for one-buy-one-sell stock trading (cross boundary)
double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high);
// find the maximum earning for one-buy-one-sell stock trading (modified BF)
double MaxSubArrayMBF(int *low, int *high);
// find the maximum earning for one-buy-one-sell stock trading (Kadane's algo.)
double MaxSubArrayKadane(int *low, int *high);

int main(void)
{
	int i;				 // loop index
	int lowBF, highBF;	 // lower/higher index of the subarray (brute-force)
	int lowDnC, highDnC; // lower/higher index of the subarray (divide&conquer)
	int lowMBF, highMBF; // lower/higher index of the subarray (modified BF)
	int lowKadane, highKadane; // lower/higher index of the subarray (Kadane's)
	STKprice *bDayBF, *sDayBF; // buy/sell day (brute-force)
	STKprice *bDayDnC, *sDayDnC; // buy/sell day (divide&conquer)
	STKprice *bDayMBF, *sDayMBF; // buy/sell day (modified BF)
	STKprice *bDayKadane, *sDayKadane; // buy/sell day (Kadane's)
	double earningBF, earningDnC; // earning by brute-force and divide&conquer
	double earningMBF, earningKadane; // earning by modified BF and Kadane's
	double t0, t1, t2, t3, t4;	 // time stamp

	readData();		// read stock history

	// t0 = GetTime();	// record time
	for (i = 0; i < R_BF; i++) {
		earningBF = MaxSubArrayBF(&lowBF, &highBF);
	}
	// t1 = GetTime();	// record time
	for (i = 0; i < R_DnC; i++) {
		earningDnC = MaxSubArray(0, N - 1, &lowDnC, &highDnC);
	}
	// t2 = GetTime();	// record time
	for (i = 0; i < R_hw6; i++) {
		earningMBF = MaxSubArrayMBF(&lowMBF, &highMBF);
	}
	// t3 = GetTime();
	for (i = 0; i < R_hw6; i++) {
		earningKadane = MaxSubArrayKadane(&lowKadane, &highKadane);
	}
	// t4 = GetTime();
	// convert the index to the actual buy/sell date
	bDayBF = &data[(lowBF > 0) * (lowBF - 1)];
	sDayBF = &data[highBF];
	bDayDnC = &data[(lowDnC > 0) * (lowDnC - 1)];
	sDayDnC = &data[highDnC];
	bDayMBF = &data[lowMBF];
	sDayMBF = &data[highMBF];
	bDayKadane = &data[(lowKadane > 0) * (lowKadane - 1)];
	sDayKadane = &data[highKadane];
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

	printf("Modified brute-force approach: time %.5e s\n", (t3 - t2) / R_hw6);
	printf("  Buy: %d/%d/%d", bDayMBF->year, bDayMBF->month, bDayMBF->day);
	printf(" at %g\n", bDayMBF->price);
	printf("  Sell: %d/%d/%d", sDayMBF->year, sDayMBF->month, sDayMBF->day);
	printf(" at %g\n", sDayMBF->price);
	printf("  Earning: %g per share.\n", earningMBF);

	printf("Kadane's algorithm: time %.5f s\n", (t4 - t3) / R_hw6);
	printf("  Buy: %d/%d/%d", bDayKadane->year, bDayKadane->month, bDayKadane->day);
	printf(" at %g\n", bDayKadane->price);
	printf("  Sell: %d/%d/%d", sDayKadane->year, sDayKadane->month, sDayKadane->day);
	printf(" at %g\n", sDayKadane->price);
	printf("  Earning: %g per share.\n", earningKadane);

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

// double GetTime(void)						// get local time in seconds
// {
// 	struct timeval tv;						// variable to store time

// 	gettimeofday(&tv, NULL);				// get local time

// 	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
// }

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
			for (i = j; i <= k; i++) { // summation for data[j : k].change
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
	return xsum; // cross-boundary is the largest
}

// find the maximum earning for one-buy-one-sell stock trading (cross boundary)
double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high)
{
	int i;
	double lsum, rsum, sum;

	lsum = 0; // initialize for lower half
	*low = mid;
	sum = 0;
	// find low s.t. sum of A[low : mid].change is maximized
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
	// find high s.t. sum of A[mid + 1 : high].change is maximized
	for (i = mid + 1; i <= end; i++) {
		sum += data[i].change;
		if (sum > rsum) { // record if larger
			rsum = sum;
			*high = i;
		}
	}

	return lsum + rsum; // overall sum
}

// find the maximum earning for one-buy-one-sell stock trading (modified BF)
double MaxSubArrayMBF(int *low, int *high)
{
	int i, j, k;	// loop indices
	double max;		// maximal sum of subarray
	double diff;		// diff of subarray

	max = 0; // initialize
	*low = 0;
	*high = N - 1;
	for (j = 0; j < N; j++) { // try all possible subarrays
		for (k = j; k < N; k++) {
			diff = data[k].price - data[j].price;
			if (diff > max) { // record the maximal diff and range
				max = diff;
				*low = j;
				*high = k;
			}
		}
	}

	return max;
}

// find the maximum earning for one-buy-one-sell stock trading (Kadane's algo.)
double MaxSubArrayKadane(int *low, int *high)
{
	int i; // loop index
	int checkpoint; // temporary lower index
	double max; // current maximum sum
	double sum; // current sum

	*low = 0; // initialize
	*high = N - 1;
	checkpoint = 0;
	max = -1e10;
	sum = 0;
	for (i = 0; i < N; i++) {
		sum += data[i].change; // add the value to sum
		if (sum > max) { // record maximum up till now
			max = sum;
			*low = checkpoint; // record the lower index
			*high = i; // record the higher index
		}
		if (sum < 0) { // current sum < 0
			sum = 0; // resest
			checkpoint = i + 1; // start adding from the next index
		}
	}

	return max;
}