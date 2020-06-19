
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define N_BUCKET 1000

typedef struct node {
	unsigned int val; // character
	int freq; // frequency
	char *symbol;
	// struct node *l; // left child
	// struct node *r; // right child
	struct node *next;
} NODE;


int n_1B = 0;
int n_2B = 0;
int n_3B = 0;
int n_4B = 0;
int n_byte = 0;
int n_uchar = 0; // # unique symbol
int H = -1; // tree height
NODE *bst = NULL;
NODE *bucket[N_BUCKET];

double GetTime(void); // get local time in seconds
void readText(void);
void char2binary(char ch);
int det_nB(char ch); // determine n-byte char
unsigned int hash(char *str);
unsigned int hash2(char *str); // unsigned int: 4 bytes

NODE *bst_find(unsigned int val); // find the node in bst
void bst_insert(unsigned int val); // insert the node in bst
void getTreeHeight(NODE *node, int h);

NODE *hash_find(unsigned int val);
void hash_insert(unsigned int val);

int main(void)
{
	int i;
	int cnt;
	NODE *tmp;
	double t0, t1;

	for (i = 0; i < N_BUCKET; i++) bucket[i] = NULL;
	readText();
	// getTreeHeight(bst, 0);
	// for (i = 0; i < N_BUCKET; i++) {
	// 	tmp = bucket[i];
	// 	cnt = 0;
	// 	while (tmp != NULL) {
	// 		cnt++;
	// 		tmp = tmp->next;
	// 	}
	// 	printf("%d ", cnt);
	// }
	return 0;
}

void readText(void)
{
	char ch;
	char symbol[5];
	int idx_ch;
	int type; // 0, 1, 2, 3, 4-byte symbol
	unsigned long val; // hash value
	NODE *tmp;

	symbol[0] = '\0';
	while ((ch = getchar()) != EOF) {
		n_byte++;
		type = det_nB(ch);
		if (type) {
			if (symbol[0] != '\0') {
				val = hash(symbol);
				// val = hash2(symbol);

				// tmp = bst_find(val);
				tmp = hash_find(val);
				if (tmp == NULL) {
					// bst_insert(val);
					hash_insert(val);
					n_uchar++; // number of nodes increase by one
				}
				else tmp->freq++;

				// if (val == 10) printf("0x0D -> %u\n", val);
				// else printf("%s -> %u\n", symbol, val);
			}
			idx_ch = 0;
			symbol[type] = '\0';
		}
		symbol[idx_ch++] = ch;
	}
	val = hash(symbol);
	// val = hash2(symbol);

	// tmp = bst_find(val);
	tmp = hash_find(val);
	if (tmp == NULL) {
		// bst_insert(val);
		hash_insert(val);
		n_uchar++; // number of nodes increase by one
	}
	else tmp->freq++;
	// if (val == 10) printf("0x0D -> %u\n", val);
	// else printf("%s -> %u\n", symbol, val);

	printf("\nTotal: %d bytes\n", n_byte);
	printf("1-byte chars: %d\n", n_1B);
	printf("2-byte chars: %d\n", n_2B);
	printf("3-byte chars: %d\n", n_3B);
	printf("4-byte chars: %d\n", n_4B);
	printf("# chars: %d\n", n_1B + n_2B + n_3B + n_4B);
	printf("# unique chars: %d\n", n_uchar);
}

void char2binary(char ch)
{
	int i;

	for (i = 0; i < 8; i++) {
		printf("%d", !!((ch << i) & 0x80)); // 0x80 = 1000 0000
	}
	printf(" ");
	// printf("\n");
}

int det_nB(char ch) // determine n-byte char
{
	if (!(ch & 0x80)) { // 0xxx xxxx
		n_1B++; return 1;
	}
	else { // 1xxx xxxx
		if (!(ch & 0x40)) { // 10xx xxxx
			return 0;
		}
		else { // 11xx xxxx
			if (!(ch & 0x20)) { // 110x xxxx
				n_2B++; return 2;
			}
			else { // 111x xxxx
				if (!(ch & 0x10)) { // 1110 xxxx
					n_3B++; return 3;
				}
				else { // 1111 xxxx
					n_4B++; return 4;
				}
			}
		}
	}
}

unsigned int hash(char *str) // hash function
{
	// djb2 hash function
	unsigned int hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

unsigned int hash2(char *str) // unsigned int: 4 bytes
{
	unsigned int hash = 0;
	unsigned char c;

	while ((c = *str++)) {
		hash = (hash << 8) + c;
	}

	return hash;
}

/*
NODE *bst_find(unsigned int val) // find the node in bst
{
	NODE *tmp = bst; // initialize it as tree root

	while (tmp != NULL) {
		if (val == tmp->val) return tmp; // found and return
		// not found, travel to proper child
		else if (val < tmp->val) tmp = tmp->l;
		else tmp = tmp->r;
	}
	return NULL; // node not found
}

void bst_insert(unsigned int val) // insert the node in bst
{
	NODE *tmp = bst; // initialize as tree root
	NODE *p = NULL; // parent of a node
	NODE *new_node;

	new_node = (NODE *)malloc(sizeof(NODE)); // allocate memory
	new_node->val = val;
	new_node->freq = 1; // initialize the frequency as one
	new_node->l = NULL;
	new_node->r = NULL;
	while (tmp != NULL) {
		p = tmp; // store the parent
		// travel to proper child
		if (val < tmp->val) tmp = tmp->l;
		else tmp = tmp->r; 
	}
	if (p == NULL) bst = new_node; // tree is empty
	else if (val < p->val) p->l = new_node; // smaller than parent
	else p->r = new_node; // larger than parent
}

void getTreeHeight(NODE *node, int h)
{
	if (node != NULL) {
		getTreeHeight(node->l, h + 1);
		getTreeHeight(node->r, h + 1);
	}
	else {
		if (h > H) H = h;
	}
}
*/

NODE *hash_find(unsigned int val)
{
	int rep = val % N_BUCKET; // representative
	NODE *tmp = bucket[rep];

	while (tmp != NULL) {
		if (tmp->val == val) return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

void hash_insert(unsigned int val)
{
	int rep = val % N_BUCKET; // representative
	NODE *new_node = (NODE *)malloc(sizeof(NODE));

	new_node->val = val;
	new_node->freq = 1;
	if (!bucket[rep]) { // start of the class
		new_node->next = NULL;
		bucket[rep] = new_node;
	}
	else {
		new_node->next = bucket[rep];
		bucket[rep] = new_node;
	}
}
