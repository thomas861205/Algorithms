// EE3980 HW09 Encoding ASCII Texts
// 105061110, 周柏宇
// 2020/05/15

#include <stdio.h>
#include <stdlib.h>
#define b2B(x) (x % 8 ? x / 8 + 1 : x / 8) // convert bits to bytes needed

// data structure to store the char and its frequency
typedef struct node {
	int ch; // character
	int n_ch; // frequency
	struct node *l; // left child
	struct node *r; // right child
} NODE;

NODE *bst = NULL; // binary search tree (bst) root
NODE **minHeap; // min heap
int n_node = 0; // number of nodes in bst
int bst_idx = 0; // index
int n_heap; // number of nodes in heap
int *code; // array for Huffman code
int n_bit = 0; // number of bits needed using Huffman code

NODE *bst_find(char ch); // find the node in bst
void bst_insert(char ch); // insert the node in bst
void bst_to_array(NODE *node); // store the bst in an array
void minHeapify(NODE **list, int i, int n); // enforce min heap property
void array_to_minHeap(NODE **list, int n); // make the array a min heap
NODE *minHeapRemoveMin(NODE **list, int n); // remove minimum from the min heap
void minHeapInsertion(NODE **list, int n, NODE *new_node);
                                                   // insert a node to min heap
void printHuffmanCode(NODE *node, int i, int bit); // print Huffman code
void freeHeap(NODE *node); // free allocated memory for nodes in a heap

int main(void)
{
	char ch;
	int n_ch = 0; // number of chars read
	NODE *tmp, *tmp2, *new_node;

	while ((ch = getchar()) != EOF) { // read the paragraph
		n_ch++; // number of chars read increase by one
		tmp = bst_find(ch); // find the char in bst
		if (tmp == NULL) bst_insert(ch); // add the char if not in bst
		else tmp->n_ch++; // increase the frequency of the char by one
	}
	bst_to_array(bst); // store the bst in an array
	array_to_minHeap(minHeap, n_node); // make the array a min heap
	n_heap = n_node; // initialize as number of nodes in bst
	while (n_heap >= 2) { // stop when only one node in heap
		// select the first and second smallest nodes
		tmp = minHeapRemoveMin(minHeap, n_heap--);
		tmp2 = minHeapRemoveMin(minHeap, n_heap--);
		new_node = (NODE *)malloc(sizeof(NODE)); // allocate memory
		new_node->ch = -1; // non leaf node
		new_node->n_ch = tmp->n_ch + tmp2->n_ch; // add up the frequency
		new_node->l = tmp; // smaller node goes to left child
		new_node->r = tmp2; // larger node goes to right child
		// insert the merged node back to heap
		minHeapInsertion(minHeap, ++n_heap, new_node);
	}
	printHuffmanCode(minHeap[0], 0, -1); // print Huffman code
	printf("Number of Chars read: %d\n", n_ch);
	printf("  Huffman Coding needs %d bits, %d bytes\n", n_bit, b2B(n_bit));
	// print the ratio of bits needed with and without using Huffman code
	printf("  Ratio = %.4f %%\n", n_bit / (8.0 * n_ch) * 100);

	free(code); // free allocated memory storing the Huffman code
	freeHeap(minHeap[0]); // free allocated memory for nodes in a heap
	free(minHeap);

	return 0;
}

NODE *bst_find(char ch) // find the node in bst
{
	NODE *tmp = bst; // initialize it as tree root

	while (tmp != NULL) {
		if (ch == tmp->ch) return tmp; // found and return
		// not found, travel to proper child
		else if (ch < tmp->ch) tmp = tmp->l;
		else tmp = tmp->r;
	}
	return NULL; // node not found
}

void bst_insert(char ch) // insert the node in bst
{
	NODE *tmp = bst; // initialize as tree root
	NODE *p = NULL; // parent of a node
	NODE *new_node;

	n_node++; // number of nodes increase by one
	new_node = (NODE *)malloc(sizeof(NODE)); // allocate memory
	new_node->ch = ch;
	new_node->n_ch = 1; // initialize the frequency as one
	new_node->l = NULL;
	new_node->r = NULL;
	while (tmp != NULL) {
		p = tmp; // store the parent
		// travel to proper child
		if (ch < tmp->ch) tmp = tmp->l;
		else tmp = tmp->r; 
	}
	if (p == NULL) bst = new_node; // tree is empty
	else if (ch < p->ch) p->l = new_node; // smaller than parent
	else p->r = new_node; // larger than parent
}

void bst_to_array(NODE *node) // store the bst in an array
{
	if (node == bst) { // if node is the root
		minHeap = (NODE **)malloc(sizeof(NODE *) * n_node); // allocate memory
		bst_idx = 0; // initialize the index
	}
	if (node != NULL) {
		minHeap[bst_idx++] = node; // add the node to array
		bst_to_array(node->l); // travel to left child
		bst_to_array(node->r); // travel to right child
		// become a leaf node for merging the tree later
		node->l = NULL;
		node->r = NULL;
	}
}

void minHeapify(NODE **list, int i, int n) // enforce min heap property
{
	int j; // index
	int done; // loop flag
	NODE *tmp;

	j = 2 * (i + 1) - 1; // initialize j to be left child of i
	tmp = list[i]; // copy root element
	done = 0;
	while ((j <= n - 1) && (!done)) {
		// let list[j] to be the smaller child
		if ((j < n - 1) && (list[j]->n_ch > list[j + 1]->n_ch)) j++;
		if (tmp->n_ch < list[j]->n_ch) done = 1; // exit if root is smaller
		else {
			list[(j + 1) / 2 - 1] = list[j]; // replace j's parent with list[j]
			j = 2 * (j + 1) - 1; // move j to its left child
		}
	}
	list[(j + 1) / 2 - 1] = tmp; // move original root to proper place
}


void array_to_minHeap(NODE **list, int n) // make the array a min heap
{
	int i; // index

	// enforce min heap property to non leaf node in bottom-up order
	for (i = n / 2 - 1; i >= 0; i--) minHeapify(list, i, n);
}

NODE *minHeapRemoveMin(NODE **list, int n) // remove minimum from the min heap
{
	NODE *tmp;

	if (list == NULL) return NULL; // empty heap
	tmp = list[0]; // minimum is the root
	list[0] = list[n - 1]; // move last node to root
	minHeapify(list, 0, n - 1); // restore min heap property

	return tmp;
}

// insert a node to min heap
void minHeapInsertion(NODE **list, int n, NODE *new_node)
{
	int i; // index

	i = n - 1; // start at last node
	list[n - 1] = new_node; // put new node at last node
	// stop going up the tree when new node is smaller than its parent
	while ((i > 0) && (list[(i + 1) / 2 - 1]->n_ch > new_node->n_ch)) {
		list[i] = list[(i + 1) / 2 - 1]; // overwrite list[i] with its parent
		i = (i + 1) / 2 - 1; // move up one layer
	}
	list[i] = new_node; // put new node at proper place
}

void printHuffmanCode(NODE *node, int i, int bit) // print Huffman code
{
	int j; // index

	if (bit == -1) { // initial call
		code = (int *)malloc(sizeof(int) * (n_node - 1)); // allocate memory
		printf("Huffman coding:\n");
	}
	else code[i - 1] = bit; // set the bit

	if (node->ch != -1) { // leaf nodes
		// print chars
		if (node->ch == 32) printf("\' \': ");
		else if (node->ch == 10) printf("\'\\n\': ");
		else printf("%c: ", node->ch);
		// print the code for the char
		for (j = 0; j < i; j++) printf("%d", code[j]);
		printf("\n");
		n_bit += node->n_ch * i; // accumulate number of bits needed
	}
	else { // non leaf nodes
		printHuffmanCode(node->l, i + 1, 0); // left child, next bit is 0
		printHuffmanCode(node->r, i + 1, 1); // right child, next bit is 1
	}
}

void freeHeap(NODE *node) // free allocated memory of nodes in a heap
{
	// post-order traversal
	if (node != NULL) {
		freeHeap(node->l);
		freeHeap(node->r);
		free(node);
	}
}
