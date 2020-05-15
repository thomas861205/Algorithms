// EE3980 HW09 Encoding ASCII Texts
// 105061110, 周柏宇
// 2020/05/15

#include <stdio.h>
#include <stdlib.h>
#define bit2byte(x) (x % 8 ? x / 8 + 1: x / 8)

typedef struct node {
	int ch;
	int n_ch;
	struct node *l;
	struct node *r;
} NODE;

NODE *bst = NULL;
NODE **bst_array;
int n_node = 0;
int bst_idx = 0;
int n_heap;
int *code;
int n_bit = 0;

NODE *bst_find(char ch);
void bst_insert(char ch);
void bst_to_array(NODE *node);
void minHeapify(NODE **list, int i, int n);
void array_to_minHeap(NODE **list, int n);
NODE *minHeapRemoveMin(NODE **list, int n);
void minHeapInsertion(NODE **list, int n, NODE *item);
void printHuffmanCode(NODE *node, int i, int bit);
void freeHeap(NODE *node);

int main(void)
{
	int i;
	char ch;
	int n_ch = 0;
	NODE *tmp, *tmp2, *new_node;

	while ((ch = getchar()) != EOF) {
		n_ch++;
		tmp = bst_find(ch);
		if (tmp == NULL) bst_insert(ch);
		else tmp->n_ch++;
	}
	bst_to_array(bst);
	array_to_minHeap(bst_array, n_node);
	n_heap = n_node;
	while (n_heap >= 2) {
		tmp = minHeapRemoveMin(bst_array, n_heap--);
		tmp2 = minHeapRemoveMin(bst_array, n_heap--);
		new_node = (NODE *)malloc(sizeof(NODE));
		new_node->ch = -1;
		new_node->n_ch = tmp->n_ch + tmp2->n_ch;
		new_node->l = tmp;
		new_node->r = tmp2;
		minHeapInsertion(bst_array, ++n_heap, new_node);
	}
	printHuffmanCode(bst_array[0], 0, -1);
	printf("Number of Chars read: %d\n", n_ch);
	printf("  Huffman Coding needs %d bits, %d bytes\n", n_bit, bit2byte(n_bit));
	printf("  Ratio = %.4f %%\n", n_bit / (8.0 * n_ch) * 100);

	free(code);
	freeHeap(bst_array[0]);
	free(bst_array);

	return 0;
}

NODE *bst_find(char ch)
{
	NODE *tmp = bst;

	while (tmp != NULL) {
		if (ch == tmp->ch) return tmp;
		else if (ch < tmp->ch) tmp = tmp->l;
		else tmp = tmp->r; 
	}
	return NULL;
}

void bst_insert(char ch)
{
	NODE *tmp = bst;
	NODE *p = NULL;
	NODE *new_node;

	n_node++;
	new_node = (NODE *)malloc(sizeof(NODE));
	new_node->ch = ch;
	new_node->n_ch = 1;
	new_node->l = NULL;
	new_node->r = NULL;
	while (tmp != NULL) {
		p = tmp;
		if (ch < tmp->ch) tmp = tmp->l;
		else tmp = tmp->r; 
	}
	if (p == NULL) bst = new_node;
	else if (ch < p->ch) p->l = new_node;
	else p->r = new_node;
}

void bst_to_array(NODE *node)
{
	if (node == bst) {
		bst_array = (NODE **)malloc(sizeof(NODE *) * n_node);
		bst_idx = 0;
	}
	if (node != NULL) {
		bst_array[bst_idx++] = node;
		bst_to_array(node->l);
		bst_to_array(node->r);
		node->l = NULL;
		node->r = NULL;
	}
	else return;
}

void minHeapify(NODE **list, int i, int n)
{
	int j;
	int done;
	NODE *tmp;

	j = 2 * (i + 1) - 1;
	tmp = list[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if ((j < n - 1) && (list[j]->n_ch > list[j + 1]->n_ch)) j++;
		if (tmp->n_ch < list[j]->n_ch) done = 1;
		else {
			list[(j + 1) / 2 - 1] = list[j];
			j = 2 * (j + 1) - 1;
		}
	}
	list[(j + 1) / 2 - 1] = tmp;
}


void array_to_minHeap(NODE **list, int n)
{
	int i;

	for (i = n / 2 - 1; i >= 0; i--) {
		minHeapify(list, i, n);
	}
}

NODE *minHeapRemoveMin(NODE **list, int n) {
	NODE *tmp;

	if (list == NULL) return NULL;
	tmp = list[0];
	list[0] = list[n - 1];
	minHeapify(list, 0, n - 1);

	return tmp;
}

void minHeapInsertion(NODE **list, int n, NODE *item)
{
	int i;

	i = n - 1;
	list[n - 1] = item;
	while ((i > 0) && (list[(i + 1) / 2 - 1]->n_ch > item->n_ch)) {
		list[i] = list[(i + 1) / 2 - 1];
		i = (i + 1) / 2 - 1;
	}
	list[i] = item;
}

void printHuffmanCode(NODE *node, int i, int bit)
{
	int j;

	if (bit == -1) {
		code = (int *)malloc(sizeof(int) * (n_node - 1));
		for (j = 0; j < (n_node - 1); j++) code[j] = -1;
		printf("Huffman coding:\n");
	}
	else code[i - 1] = bit;

	if (node->ch != -1) {
		if (node->ch == 32) printf("\' \': ");
		else if (node->ch == 10) printf("\'\\n\': ");
		else printf("%c: ", node->ch);
		for (j = 0; j < i; j++) printf("%d", code[j]);
		printf("\n");
		n_bit += node->n_ch * i;
	}
	else {
		printHuffmanCode(node->l, i + 1, 0);
		printHuffmanCode(node->r, i + 1, 1);
	}
}

void freeHeap(NODE *node)
{
	if (node != NULL) {
		freeHeap(node->l);
		freeHeap(node->r);
		free(node);
	}
}
