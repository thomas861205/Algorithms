#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_REPEAT 500 // number of repetitions
#define D 1 // delete cost
#define I 1 // insert cost
#define C 2 // change cost

// data structure to store the line information, remember to memtion
typedef struct line {
	int nth; // nth line
	int len; // number of bytes in the line
	char *txt; // the line
	struct line *next;
} LINE;

int N; // number of lines
int n_change; // number of changes
int idx_T; // index for operations
int **cost; // table of transformation cost
LINE *P1; // paragraph 1
LINE *P2; // paragraph 2
char *T; // sequence of operations

LINE *readFile(char const *fname, int *n); // read paragraph line-by-line
void WagnerFischer(void); // generate the transformation cost table
void Trace(void); // trace the transformation sequence
void printAns(void); // print operations
void freeAll(void); // free allocated memory
double GetTime(void); // get local time in seconds

int main(int argc, char const *argv[])
{
	// expect the command line to be: ./a.out filename1 filename2
	int i; // loop index
	double start, end; // timestamp
	double sum_t = 0; // cumulated time

	P1 = readFile(argv[1], &N); // read first paragraph
	P2 = readFile(argv[2], &N); // read second paragraph
	cost = (int **)malloc(sizeof(int *) * (N + 1));
	for (i = 0; i <= N; i++) {
		cost[i] = (int *)malloc(sizeof(int) * (N + 1));
	}
	T = (char *)malloc(sizeof(char) * 2 * N);
	for (i = 0; i < N_REPEAT; i++) {
		start = GetTime(); // start the timer
		WagnerFischer(); // generate the transformation cost table
		Trace(); // trace the transformation sequence
		end = GetTime(); // stop the timer
		sum_t += end - start; // accumulate the CPU time
		if (!i) printAns(); // print the result just for once
	}
	printf("CPU time: %.5f sec\n", sum_t / N_REPEAT); // print average CPU time
	freeAll(); // free allocated memory
	return 0;
}

LINE *readFile(char const *fname, int *n) // read paragraph line-by-line
{
	FILE *fp; // file object
	char *line_buff = NULL; // buffer to store the line
	int buf_len = 0; // length of the buffer
	int n_line = 0; // number of lines
	int n_char; // number of bytes
	LINE *head = NULL; // first line
	LINE *new_node;
	LINE *tail; // last line

	fp = fopen(fname, "r"); // open the text file
	if (fp != NULL) {
		while ((n_char = getline(&line_buff, &buf_len, fp)) != -1) {
			n_line++; // number of lines increases by 1
			new_node = (LINE *)malloc(sizeof(LINE));
			new_node->nth = n_line; // the nth line
			new_node->len = n_char; // line length
			new_node->txt = (char *)malloc(sizeof(char) * (n_char + 1));
			strcpy(new_node->txt, line_buff); // line texts
			new_node->next = NULL;
			if (head == NULL) { // first line
				head = new_node;
			}
			else {
				tail->next = new_node; // connect it to last line
			}
			tail = new_node; // update last line
		}
		*n = n_line; // number of lines for this paragraph
		free(line_buff);
		fclose(fp); // close the file
		return head; // return the paragraph
	}
	else {
		*n = -1; // file open failed
		return NULL;
	}
}

void WagnerFischer(void) // generate the transformation cost table
{
	int i, j; // loop indices
	int m1, m2, m3; // costs
	LINE *p = P1; // paragraph 1
	LINE *q = P2; // paragraph 2

	// initialize the cost table
	cost[0][0] = 0;
	for (i = 1; i <=N; i++) {
		cost[i][0] = cost[i - 1][0] + D;
		cost[0][i] = cost[0][i - 1] + I;
	}
	p = P1;
	while (p != NULL) {
		q = P2;
		while (q != NULL) {
			i = p->nth; j = q->nth;
			// if two lines are identical
			if ((p->len == q->len) && !strcmp(p->txt, q->txt)) {
				cost[i][j] = cost[i - 1][j - 1];
			}
			else { // otherwise
				m1 = cost[i - 1][j - 1] + C; // cost for change
				m2 = cost[i - 1][j] + D; // cost for delete
				m3 = cost[i][j - 1] + I; // cost for insert
				// cost = min{m1, m2, m3}
				cost[i][j] = m1 < m2 ? m1 : m2;
				cost[i][j] = cost[i][j] < m3 ? cost[i][j] : m3;
			}
			q = q->next;
		}
		p = p->next;
	}
}

void Trace(void) // trace the transformation sequence
{
	int i = N; // loop index
	int j = N; // loop index
	LINE *p = P1; // paragraph 1
	LINE *q = P2; // paragraph 2

	n_change = 0; // number of delete, insert and change
	idx_T = 0; // index for the operation sequence (reversed)
	while ((i > 0) || (j > 0)) {
		// Start from the bottom right of the table, and see where do the
		// value comes from(left, up or upper left).
		if ((i > 0) && (j > 0) && (cost[i][j] == cost[i - 1][j - 1] + C)) {
			T[idx_T] = 'C'; // change
			i--;
			j--;
			n_change++;
		}
		else if ((i == 0) || (cost[i][j] == cost[i][j - 1] + I)) {
			T[idx_T] = 'I'; // insert
			j--;
			n_change++;
		}
		else if ((j == 0) || (cost[i][j] == cost[i - 1][j] + D)) {
			T[idx_T] = 'D'; // delete
			i--;
			n_change++;
		}
		else {
			T[idx_T] = '-'; // need not to change(keep)
			i--;
			j--;
		}
		idx_T++;
	}
}

void printAns(void) // print operations
{
	int i = idx_T - 1; // first operation
	int nth = 1; // nth line for paragraph 1
	LINE *p = P1; // paragraph 1
	LINE *q = P2; // paragraph 2

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

void freeAll(void) // free allocated memory
{
	int i; // loop index
	LINE *tmp;
	LINE *nxt;

	tmp = P1;
	while (tmp != NULL) {
		nxt = tmp->next; // save next node of the link list
		free(tmp->txt);
		free(tmp);
		tmp = nxt;
	}
	tmp = P2;
	while (tmp != NULL) {
		nxt = tmp->next; // save next node of the link list
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
