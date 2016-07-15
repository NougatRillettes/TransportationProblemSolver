#include <stdio.h>

#include "instance.h"

// Printing functions
// ------------------

#define NUMLENGTH 3
#define NUMLENGTHSTR "3"
void print_inst(Instance const * const input) {
	for(int i = 0; i < NUMLENGTH + 0; i++)
		putchar(' ');
	putchar('|');
	for(size_t i = 0; i < input->width; i++) {
		printf(" %"NUMLENGTHSTR"lu |",input->jobs[i].discr);
	}
	putchar('\n');
	for(size_t i = 0; i < input->height; i++) {
		for(size_t i = 0; i < NUMLENGTH + 1 + input->width *(NUMLENGTH + 3); i++)
			putchar('-');
		putchar('\n');
		printf("%"NUMLENGTHSTR"lu|",input->agents[i].discr);
		for(size_t j = 0; j < input->width;j++) {
			printf("%"NUMLENGTHSTR"ld|%c|",input->matrix[i][j].val,input->matrix[i][j].starred ? '*' : ' ');
		}
		putchar('\n');
		for(int i = 0; i < NUMLENGTH + 0; i++)
			putchar(' ');
		putchar('|');
		for(size_t j = 0; j < input->width;j++) {
			printf("%"NUMLENGTHSTR"ld|%c|",input->matrix[i][j].quota,input->matrix[i][j].primed ? '\'' : ' ');
		}

		putchar('\n');
	}
	putchar('\n');
	putchar('\n');
}
