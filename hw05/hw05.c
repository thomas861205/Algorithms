// EE3980 HW05 Trading Stock
// 105061110, 周柏宇
// 2020/04/12

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct sSTKprice {
	int year, month, day;
	double price, change;
} STKprice;

int N;
int R_BF = 1;
int R_DnC = 1000;
STKprice *data;

void readData(void);
double GetTime(void);	// get local time in seconds
double MaxSubArrayBF(int *low, int *high);
double MaxSubArray(int begin, int end, int *low, int *high);
double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high);

int main(void)
{
	int i;
	int lowBF, highBF;
	int lowDnC, highDnC;
	STKprice *bDayBF, *sDayBF;
	STKprice *bDayDnC, *sDayDnC;
	double earningBF, earningDnC;
	double t0, t1, t2;

	readData();

	t0 = GetTime();
	for (i = 0; i < R_BF; i++) {
		earningBF = MaxSubArrayBF(&lowBF, &highBF);
	}
	t1 = GetTime();
	for (i = 0; i < R_DnC; i++) {
		earningDnC = MaxSubArray(0, N - 1, &lowDnC, &highDnC);
	}
	t2 = GetTime();

	bDayBF = &data[(lowBF > 0) * (lowBF - 1)];
	sDayBF = &data[highBF];
	bDayDnC = &data[(lowDnC > 0) * (lowDnC - 1)];
	sDayDnC = &data[highDnC];
	printf("N = %d\n", N);
	printf("Brute-force approach: time %.5e s\n", (t1 - t0) / R_BF);
	printf("  Buy: %d/%d/%d", bDayBF->year, bDayBF->month, bDayBF->day);
	printf(" at %g\n", bDayBF->price);
	printf("  Sell: %d/%d/%d", sDayBF->year, sDayBF->month, sDayBF->day);
	printf(" at %g\n", sDayBF->price);
	printf("  Earning: %g per share.\n", earningBF);
	printf("Divide and Conquer approach: time %.5e s\n", (t2 - t1) / R_DnC);
	printf("  Buy: %d/%d/%d", bDayDnC->year, bDayDnC->month, bDayDnC->day);
	printf(" at %g\n", bDayDnC->price);
	printf("  Sell: %d/%d/%d", sDayDnC->year, sDayDnC->month, sDayDnC->day);
	printf(" at %g\n", sDayDnC->price);
	printf("  Earning: %g per share.\n", earningDnC);

	free(data);

	return 0;
}

void readData(void)
{
	int i;

	scanf("%d", &N);
	data = (STKprice *)malloc(sizeof(STKprice) * N);
	for (i = 0; i < N; i++) {
		scanf("%d", &data[i].year);
		scanf("%d", &data[i].month);
		scanf("%d", &data[i].day);
		scanf("%lf", &data[i].price);
		data[i].change = data[i].price - data[(i > 0) * (i - 1)].price;
	}
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}

double MaxSubArrayBF(int *low, int *high)
{
	int i, j, k;
	double max = 0;
	double sum;

	*low = 0;
	*high = N - 1;
	for (j = 0; j < N; j++) {
		for (k = j; k < N; k++) {
			sum = 0;
			for (i = j; i <= k; i++) {
				sum += data[i].change;
			}
			if (sum > max) {
				max = sum;
				*low = j;
				*high = k;
			}
		}
	}

	return max;
}

double MaxSubArray(int begin, int end, int *low, int *high)
{
	int mid;
	int llow, lhigh, rlow, rhigh, xlow, xhigh;
	double lsum, rsum, xsum;

	if (begin == end) {
		*low = begin;
		*high = end;
		return data[begin].change;
	}

	mid = (begin + end) / 2;
	lsum = MaxSubArray(begin, mid, &llow, &lhigh);
	rsum = MaxSubArray(mid + 1, end, &rlow, &rhigh);
	xsum = MaxSubArrayXB(begin, mid, end, &xlow, &xhigh);

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

double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high)
{
	int i;
	double lsum, rsum, sum;

	lsum = 0;
	*low = mid;
	sum = 0;
	for (i = mid; i >= begin; i--) {
		sum += data[i].change;
		if (sum > lsum) {
			lsum = sum;
			*low = i;
		}
	}

	rsum = 0;
	*high = sum + 1;
	sum = 0;
	for (i = mid + 1; i <= end; i++) {
		sum += data[i].change;
		if (sum > rsum) {
			rsum = sum;
			*high = i;
		}
	}

	return lsum + rsum;
}
