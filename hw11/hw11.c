#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_REPEAT 1
#define D 1
#define I 1
#define C 2

typedef struct line {
	int nth;
	int len;
	char *txt;
	struct line *next;
} LINE;

typedef struct operation {
	char opr;
	char *txt;
	struct operation *next;
} OPR;

int N, M;
int **cost;
LINE *P1 = NULL;
LINE *P2 = NULL;
OPR *T = NULL;

LINE *readFile(char const *fname, int *n); // hash the line maybe?
void Transform(void);
void Trace(void);
void freeAll(void);
double GetTime(void); // get local time in seconds

int main(int argc, char const *argv[])
{
	int i;
	LINE *l;
	double start, end;

	P1 = readFile(argv[1], &N);
	P2 = readFile(argv[2], &M);
	start = GetTime();
	for (i = 0; i < N_REPEAT; i++) {
		Transform();
		Trace();
	}
	end = GetTime();
	printf("CPU time: %.5f sec\n", (end - start) / N_REPEAT);
	freeAll();
	return 0;
}

LINE *readFile(char const *fname, int *n)
{
	FILE *fp;
	char *line = NULL;
	int len = 0;
	int n_line = 0;
	int n_char;
	LINE *head = NULL;
	LINE *new_node;
	LINE *tail;

	fp = fopen(fname, "r");
	if (fp != NULL) {
		while ((n_char = getline(&line, &len, fp)) != -1) {
			n_line++;
			new_node = (LINE *)malloc(sizeof(LINE));
			new_node->nth = n_line;
			new_node->len = n_char; // include '\n' not include '\0'
			new_node->txt = (char *)malloc(sizeof(char) * (n_char + 1));
			strcpy(new_node->txt, line);
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
		free(line);
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

	cost = (int **)malloc(sizeof(int *) * (N + 1));
	for (i = 0; i <= N; i++) {
		cost[i] = (int *)malloc(sizeof(int) * (M + 1));
	}
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
	int i, j;
	int n_change = 0;
	int nth = 1;
	OPR *tmp;
	LINE *p = P1;
	LINE *q = P2;

	i = N;
	j = M;
	while ((i > 0) || (j > 0)) {
		tmp = (OPR *)malloc(sizeof(OPR));
		if ((i > 0) && (j > 0) && (cost[i][j] == cost[i - 1][j - 1] + C)) {
			tmp->opr = 'C';
			i--;
			j--;
			n_change++;
		}
		else if ((i == 0) || (cost[i][j] == cost[i][j - 1] + I)) {
			tmp->opr = 'I';
			j--;
			n_change++;
		}
		else if ((j == 0) || (cost[i][j] == cost[i - 1][j] + D)) {
			tmp->opr = 'D';
			i--;
			n_change++;
		}
		else {
			tmp->opr = '-';
			i--;
			j--;
		}
		if (T == NULL) {
			tmp->next = NULL;
		}
		else {
			tmp->next = T;
		}
		T = tmp;
	}
	printf("%d lines with %d changes:\n", M, n_change);
	tmp = T;
	while (tmp != NULL) {
		if (tmp->opr == 'I') {
			printf("Insert line %d:\n", nth);
			printf("%s", q->txt);
			nth++;
			q = q->next;
		}
		else if (tmp->opr == 'D') {
			printf("Delete line %d:\n", nth);
			printf("%s", p->txt);
			p = p->next;
		}
		else if (tmp->opr == 'C') {
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
		tmp = tmp->next;
	}
}

void freeAll(void)
{
	int i;
	LINE *tmp;
	LINE *nxt;
	OPR *tmp2;
	OPR *nxt2;

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
	for (i = 0; i <= N; i++) free(cost[i]);
	free(cost);
	tmp2 = T;
	while (tmp2 != NULL) {
		nxt2 = tmp2->next;
		free(tmp2);
		tmp2 = nxt2;
	}
}

double GetTime(void)						// get local time in seconds
{
	struct timeval tv;						// variable to store time

	gettimeofday(&tv, NULL);				// get local time

	return tv.tv_sec + 1e-6 * tv.tv_usec;	// return local time in seconds
}
