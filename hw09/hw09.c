#include <stdio.h>
#include <stdlib.h>

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

NODE *bst_find(char ch);
void bst_insert(char ch);
void bst_traverse(NODE *node);
void bst_to_array(NODE *node);
void maxHeapify(NODE **list, int i, int n);
void minHeapify(NODE **list, int i, int n);
void array_to_minHeap(NODE **list, int n);
void HeapSort(NODE **list, int n); // nondecreasing
NODE *minHeapRemoveMin(NODE **list, int n);
void minHeapInsertion(NODE **list, int n, NODE *item);
void printHeap(NODE **list, int n);
void printHuffmanCode(NODE *node, int i, int bit);

int main(void)
{
	int i, j;
	char ch;
	int n_ch = 0;
	NODE *tmp, *tmp2, *new_node;

	while ((ch = getchar()) != EOF) {
		n_ch++;
		// printf("%d\n", ch);
		tmp = bst_find(ch);
		if (tmp == NULL) bst_insert(ch);
		else tmp->n_ch++;
	}
	// bst_traverse(bst);
	// printf("unique char: %d\n", n_node);
	bst_to_array(bst);
	// HeapSort(bst_array, n_node);
	array_to_minHeap(bst_array, n_node);
	// for (i = 0; i < n_node; i++) printf("%c: %d\n", bst_array[i]->ch, bst_array[i]->n_ch);
	n_heap = n_node;
	// for (i = 0; i < n_node; i++) {
	// 	tmp = minHeapRemoveMin(bst_array, n_heap--);
	// 	printHeap(bst_array, n_node);
	// 	printf("-> %d\n", tmp->n_ch);
	// }
	// printHeap(bst_array, n_heap);
	while (n_heap >= 2) {
		tmp = minHeapRemoveMin(bst_array, n_heap--);
		tmp2 = minHeapRemoveMin(bst_array, n_heap--);
		new_node = (NODE *)malloc(sizeof(NODE));
		new_node->ch = -1;
		new_node->n_ch = tmp->n_ch + tmp2->n_ch;
		new_node->l = tmp;
		new_node->r = tmp2;
		minHeapInsertion(bst_array, ++n_heap, new_node);
		// printHeap(bst_array, n_heap);
	}
	// printHeap(bst_array, n_heap);
	printHuffmanCode(bst_array[0], 0, -1);
	printf("Number of Chars read: %d\n", n_ch);
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

void bst_traverse(NODE *node)
{
	int ch;

	if (node != NULL) {
		ch = node->ch;
		if (ch == 32) printf("\'%c\'", ch);
		else if (ch == 10) printf("\'\\n\'");
		else printf("%c", ch);
		printf(": %d\n", node->n_ch);
		bst_traverse(node->l);
		bst_traverse(node->r);
	}
	else return;
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
	}
	else return;
}

void maxHeapify(NODE **list, int i, int n)
{
	int j;
	int done;
	NODE *tmp;

	j = 2 * (i + 1) - 1;
	tmp = list[i];
	done = 0;
	while ((j <= n - 1) && (!done)) {
		if ((j < n - 1) && (list[j]->n_ch < list[j + 1]->n_ch)) j++;
		if (tmp->n_ch > list[j]->n_ch) done = 1;
		else {
			list[(j + 1) / 2 - 1] = list[j];
			j = 2 * (j + 1) - 1;
		}
	}
	list[(j + 1) / 2 - 1] = tmp;
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
	NODE *tmp;

	for (i = n / 2 - 1; i >= 0; i--) {
		minHeapify(list, i, n);
	}
}

void HeapSort(NODE **list, int n)
{
	int i;
	NODE *tmp;

	for (i = n / 2 - 1; i >= 0; i--) {
		maxHeapify(list, i, n);
	}
	for (i = n - 1; i > 0; i--) {
		tmp = list[0];
		list[0] = list[i];
		list[i] = tmp;
		maxHeapify(list, 0, i);
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

void printHeap(NODE **list, int n) {
	int j;

	for (j = 0; j < n; j++) printf("%d ", list[j]->n_ch);
	printf("\n");
}

void printHuffmanCode(NODE *node, int i, int bit)
{
	int j;

	if (bit == -1) {
		code = (int *)malloc(sizeof(int) * (n_node - 1));
		for (j = 0; j < (n_node - 1); j++) code[j] = -1;
	}
	else code[i - 1] = bit;

	// printf("%d: ", node->n_ch);
	// for (j = 0; j < i; j++) printf("%d", code[j]);
	// printf("\n");
	if (node->ch != -1) {
		printf("%d: ", node->n_ch);
		for (j = 0; j < i; j++) printf("%d", code[j]);
		printf("\n");
	}
	else {
		printHuffmanCode(node->l, i + 1, 0);
		printHuffmanCode(node->r, i + 1, 1);
	}
}