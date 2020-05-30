#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_REPEAT 500
#define D 1
#define I 1
#define C 2

typedef struct line {
	int nth;
	int len;
	char *txt;
	struct line *next;
} LINE;

int N;
int n_change;
int idx_T;
int **cost;
LINE *P1;
LINE *P2;
char *T;

LINE *readFile(char const *fname, int *n); // hash the line maybe?
void Transform(void);
void Trace(void);
void printAns(void);
void freeAll(void);
double GetTime(void); // get local time in seconds

int main(int argc, char const *argv[])
{
	int i;
	double start, end;
	double sum_t = 0;

	P1 = readFile(argv[1], &N);
	P2 = readFile(argv[2], &N);
	cost = (int **)malloc(sizeof(int *) * (N + 1));
	for (i = 0; i <= N; i++) {
		cost[i] = (int *)malloc(sizeof(int) * (N + 1));
	}
	T = (char *)malloc(sizeof(char) * 2 * N);
	for (i = 0; i < N_REPEAT; i++) {
		start = GetTime();
		Transform();
		Trace();
		end = GetTime();
		if (!i) printAns();
		sum_t += end - start;
	}
	printf("CPU time: %.5f sec\n", sum_t / N_REPEAT);
	freeAll();
	return 0;
}

LINE *readFile(char const *fname, int *n)
{
	FILE *fp;
	char *line_buff = NULL;
	int buf_len = 0;
	int n_line = 0;
	int n_char;
	LINE *head = NULL;
	LINE *new_node;
	LINE *tail;

	fp = fopen(fname, "r");
	if (fp != NULL) {
		while ((n_char = getline(&line_buff, &buf_len, fp)) != -1) {
			n_line++;
			new_node = (LINE *)malloc(sizeof(LINE));
			new_node->nth = n_line;
			new_node->len = n_char; // include '\n' not include '\0'
			new_node->txt = (char *)malloc(sizeof(char) * (n_char + 1));
			strcpy(new_node->txt, line_buff);
			new_node->next = NULL;
			if (head == NULL) {
				head = new_node;
			}
			else {
				tail->next = new_node;
			}
			tail = new_node;
		}
		*n = n_line;
		free(line_buff);
		fclose(fp);
		return head;
	}
	else {
		*n = -1;
		return NULL;
	}
}

void Transform(void)
{
	int i, j;
	int m1, m2, m3;
	LINE *p = P1;
	LINE *q = P2;

	cost[0][0] = 0;
	while (p != NULL) {
		cost[p->nth][0] = cost[p->nth - 1][0] + D;
		p = p->next;
	}
	while (q != NULL) {
		cost[0][q->nth] = cost[0][q->nth - 1] + I;
		q = q->next;
	}
	p = P1;
	while (p != NULL) {
		q = P2;
		while (q != NULL) {
			i = p->nth; j = q->nth;
			if ((p->len == q->len) && !strcmp(p->txt, q->txt)) { // faster?
				cost[i][j] = cost[i - 1][j - 1];
			}
			else {
				m1 = cost[i - 1][j - 1] + C;
				m2 = cost[i - 1][j] + D;
				cost[i][j] = m1 < m2 ? m1 : m2;
				m3 = cost[i][j - 1] + I;
				cost[i][j] = cost[i][j] < m3 ? cost[i][j] : m3;
			}
			q = q->next;
		}
		p = p->next;
	}
}

void Trace(void)
{
	int i = N;
	int j = N;
	LINE *p = P1;
	LINE *q = P2;

	n_change = 0;
	idx_T = 0;
	while ((i > 0) || (j > 0)) {
		if ((i > 0) && (j > 0) && (cost[i][j] == cost[i - 1][j - 1] + C)) {
			T[idx_T] = 'C';
			i--;
			j--;
			n_change++;
		}
		else if ((i == 0) || (cost[i][j] == cost[i][j - 1] + I)) {
			T[idx_T] = 'I';
			j--;
			n_change++;
		}
		else if ((j == 0) || (cost[i][j] == cost[i - 1][j] + D)) {
			T[idx_T] = 'D';
			i--;
			n_change++;
		}
		else {
			T[idx_T] = '-';
			i--;
			j--;
		}
		idx_T++;
	}
}

void printAns(void)
{
	int i = idx_T - 1;
	int nth = 1;
	LINE *p = P1;
	LINE *q = P2;

	printf("%d lines with %d changes:\n", N, n_change);
	while (i >= 0) {
		if (T[i] == 'I') {
			printf("Insert line %d:\n", nth);
			printf("%s", q->txt);
			nth++;
			q = q->next;
		}
		else if (T[i] == 'D') {
			printf("Delete line %d:\n", nth);
			printf("%s", p->txt);
			p = p->next;
		}
		else if (T[i] == 'C') {
			printf("Change line %d:\n", nth);
			printf("%s", q->txt);
			nth++;
			p = p->next;
			q = q->next;
		}
		else {
			nth++;
			p = p->next;
			q = q->next;
		}
		i--;
	}
}

void freeAll(void)
{
	int i;
	LINE *tmp;
	LINE *nxt;

	tmp = P1;
	while (tmp != NULL) {
		nxt = tmp->next;
		free(tmp->txt);
		free(tmp);
		tmp = nxt;
	}
	tmp = P2;
	while (tmp != NULL) {
		nxt = tmp->next;
		free(tmp->txt);
		free(tmp);
		tmp = nxt;
	}
	for (i = 0; i <= N; i++) {
		free(cost[i]);
	}
	free(cost);
	free(T);
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}
