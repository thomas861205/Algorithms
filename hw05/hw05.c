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
int R_DnC = 1; // 1000
STKprice *data;

void readData(void);
double MaxSubArrayBF(int *low, int *high);
double MaxSubArray(int begin, int end, int *low, int *high);
double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high);

int main(void)
{
	int i;
	int lowBF, highBF;
	int lowDnC, highDnC;
	STKprice *bDay, *sDay;
	double earningBF, earningDnC;

	readData();
	printf("N = %d\n", N);
	for (i = 0; i < R_BF; i++) {
		earningBF = MaxSubArrayBF(data, N, &lowBF, &highBF);
	}
	bDay = &data[(lowBF > 0) * (lowBF - 1)];
	sDay = &data[highBF];
	printf("Brute-force approach: time ? s\n");
	printf("  Buy: %d/%d/%d", bDay->year, bDay->month, bDay->day);
	printf(" at %g\n", bDay->price);
	printf("  Sell: %d/%d/%d", sDay->year, sDay->month, sDay->day);
	printf(" at %g\n", sDay->price);
	printf("  Earning: %g per share.\n", earningBF);
	// for (i = 0; i < R_DnC; i++) {
	// 	;
	// }
	// printf("Divide and Conquer approach: time ? s\n");
	// printf("  Buy: %d/%d/%d at %g\n", );
	// printf("  Sell: %d/%d/%d at %g\n", );
	// printf("  Earning: %g per share.\n", );

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

	// for (i = 0; i < N; i++) {
	// 	printf("%d ", data[i].year);
	// 	printf("%d ", data[i].month);
	// 	printf("%d ", data[i].day);
	// 	printf("%g ", data[i].price);
	// 	printf("%g\n", data[i].change);
	// }
}

double MaxSubArrayBF(int *low, int *high)
{
	int i, j, k;
	double max = 0;
	double sum;

	*low = 0;
	*high = n - 1;
	for (j = 0; j < n; j++) {
		for (k = j; k < n; k++) {
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
	double lsum, rsum, xsum;

	if (begin == end) {
		*low = begin;
		*high = end;
		return data[begin];
	}
	mid = (begin + end) / 2;
}
double MaxSubArrayXB(int begin, int mid, int end, int *low, int *high);