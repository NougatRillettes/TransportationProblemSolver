#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "instance.h"
#include "core.h"


int main(int argc, char * argv[]) {
	bool DEBUG = false;
	bool LEGACY = false;

	int c;
	while ((c = getopt(argc , argv, "dl")) != -1) {
    switch (c) {
			case 'd':
				DEBUG = true;
				break;
			case 'l':
				LEGACY = true;
				break;
		}
	}

	//file given on cmd line, defaul to stdin
	FILE * f = stdin;
	if(argc > optind) {
		f = fopen(argv[optind],"r");
	}

	size_t n, m;
	fscanf(f," %zd %zd ",&n,&m);

	Instance input = {
		.height = n,
		.width = m,
		.agents = calloc(n,sizeof(AJ_Entry)),
		.jobs = calloc(m,sizeof(AJ_Entry)),
		.matrix = malloc(n*sizeof(M_Entry*)),
	};
	//store the matrix in a contiguous region of memory
	M_Entry * contiguous = calloc(n*m,sizeof(M_Entry));
	for(size_t i = 0; i < n; i++) {
		input.matrix[i] = contiguous + m*i;
	}

	uint64_t agents_sum = 0;
	for(size_t i = 0; i < n; i++) {
		if(LEGACY)
			fscanf(f," %ld ",&input.agents[i].discr);
		else
			fscanf(f," %ld:%ld ",&input.agents[i].tag,&input.agents[i].discr);
		agents_sum += input.agents[i].discr;
	}
	uint64_t jobs_sum = 0;
	for(size_t i = 0; i < m; i++) {
		if(LEGACY)
			fscanf(f," %ld ",&input.jobs[i].discr);
		else
			fscanf(f," %ld:%ld ",&input.jobs[i].tag,&input.jobs[i].discr);
		jobs_sum += input.jobs[i].discr;
	}
	if (jobs_sum != agents_sum) {
		fprintf(stderr,"Error, different sum of agents and jobs: %ld vs %ld !",agents_sum,jobs_sum);
		exit(1);
	}
	for(size_t i = 0; i < n; i++) {
		for(size_t j = 0; j < m; j++) {
			uint64_t tmp;
			fscanf(f," %ld ",&tmp);
			input.matrix[i][j].init_cost = tmp;
			input.matrix[i][j].val = tmp;
		}
	}
	if(DEBUG) {
		printf("Input:\n");
		print_inst(&input);
		printf("-----------\n");
	}
	prelim(&input);
	if(DEBUG) {
		printf("After preliminary work:\n");
		print_inst(&input);
		printf("-----------\n");
	}
	while(!done(&input)) {
		if(DEBUG)
			print_inst(&input);
		step1(&input);
	}
	if(DEBUG)
		print_inst(&input);
	uint64_t cost = 0;
	for (size_t i = 0; i < input.height; i++) {
		for (size_t j = 0; j < input.width; j++) {
			cost += input.matrix[i][j].init_cost * input.matrix[i][j].quota;
		}
	}
	printf("Minimum cost: %ld\n",cost);
	if(!LEGACY) {
		for (size_t i = 0; i < input.height; i++) {
			for (size_t j = 0; j < input.width; j++) {
				if (input.matrix[i][j].quota != 0) {
					printf("%ld %ld %ld\n", input.agents[i].tag, input.jobs[j].tag, input.matrix[i][j].quota);
				}
			}
		}
	}
}
