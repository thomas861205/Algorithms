#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

LINE *readFile(char const *fname, int *n);
void print(LINE *P);
void Transform(void);
void Trace(void);

int main(int argc, char const *argv[])
{
	int i;
	LINE *l;

	P1 = readFile(argv[1], &N);
	P2 = readFile(argv[2], &M);
	// print(P1);
	// print(P2);
	Transform();
	Trace();
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
			// printf("Retrieved %dth line of length %d :\n", n_line, n_char);
			// printf("%s", tail->txt);
		}
		*n = n_line;
		fclose(fp);
		return head;
	}
	else {
		// printf("Fail to open the file.\n");
		*n = -1;
		return NULL;
	}
}

void print(LINE *P)
{
	LINE *l = P;

	while (l != NULL) {
		printf("%d.[%d]%s", l->nth, l->len, l->txt);
		l = l->next;
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
			// printf("%d %d\n", p->nth, q->nth);
			i = p->nth; j = q->nth;
			if (!strcmp(p->txt, q->txt)) {
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
	// for (i = 0; i <= N; i++) {
	// 	for (j = 0; j <= M; j++) printf("%d ", cost[i][j]);
	// 	printf("\n");
	// }
}

void Trace(void)
{
	int i, j, k;
	int nth = 1;
	OPR *tmp;

	i = N;
	j = M;
	k = 0;
	while ((i > 0) || (j > 0)) {
		tmp = (OPR *)malloc(sizeof(OPR));
		if ((i > 0) && (j > 0) && (cost[i][j] == cost[i - 1][j - 1] + C)) {
			tmp->opr = 'C';
			i--;
			j--;
			k++;
		}
		else if ((i == 0) || (cost[i][j] == cost[i][j - 1] + I)) {
			tmp->opr = 'I';
			j--;
			k++;
		}
		else if ((j == 0) || (cost[i][j] == cost[i - 1][j] + D)) {
			tmp->opr = 'D';
			i--;
			k++;
		}
		else {
			tmp->opr = '-';
			i--;
			j--;
			k++;
		}
		if (T == NULL) {
			tmp->next = NULL;
		}
		else {
			tmp->next = T;
		}
		T = tmp;
	}
	tmp = T;
	while (tmp != NULL) {
		printf("%c %d\n", tmp->opr, nth);
		if (tmp->opr == 'I') nth += 1;
		else if (tmp->opr == 'D') ;
		else nth++;
		tmp = tmp->next;
	}
}