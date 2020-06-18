
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readText(void);
void char2binary(char ch);
int det_nB(char ch); // determine n-byte char
unsigned int hash2(char *str); // unsigned int: 4 bytes

int n_1B = 0;
int n_2B = 0;
int n_3B = 0;
int n_4B = 0;

int main(void)
{
	readText();
	return 0;
}

void readText(void)
{
	char ch;
	char tmp[5];
	int idx_ch;
	int n_byte = 0;
	int type;
	unsigned long val;

	tmp[0] = '\0';
	while ((ch = getchar()) != EOF) {
		n_byte++;
		type = det_nB(ch);
		if (type) {
			val = hash2(tmp);
			printf("%s -> %u\n", tmp, val);
			idx_ch = 0;
			tmp[type] = '\0';
		}
		tmp[idx_ch++] = ch;
		char2binary(ch);
		// printf("%c", ch);
	}
	val = hash2(tmp);
	printf("%s -> %u\n", tmp, val);

	printf("\nTotal: %d bytes\n", n_byte);
	printf("1-byte char: %d\n", n_1B);
	printf("2-byte char: %d\n", n_2B);
	printf("3-byte char: %d\n", n_3B);
	printf("4-byte char: %d\n", n_4B);
	printf("# chars: %d\n", n_1B + n_2B + n_3B + n_4B);
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

unsigned int hash2(char *str) // unsigned int: 4 bytes
{
	unsigned int hash = 0;
	unsigned char c;

	while ((c = *str++)) {
		hash = (hash << 8) + c;
	}

	return hash;
}