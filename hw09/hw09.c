#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int ch;
	int n_ch;
	struct node *l;
	struct node *r;
} NODE;

NODE *bst = NULL;

NODE *bst_find(char ch);
void bst_insert(char ch);
void bst_traverse(NODE *node);

int main(void)
{
	char ch;
	int n_ch = 0;
	NODE *tmp;

	while ((ch = getchar()) != EOF) {
		n_ch++;
		// printf("%d\n", ch);
		tmp = bst_find(ch);
		if (tmp == NULL) bst_insert(ch);
		else tmp->n_ch++;
	}
	// bst_traverse(bst);
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
