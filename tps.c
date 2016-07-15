#include <stdlib.h> 
#include <stdio.h>

#include "instance.h"
#include "core.h"


// Printing functions
// ------------------


int main(int argc, char * argv[]) {

	FILE * f = stdin;
	if(argc > 1) { 
		f = fopen(argv[1],"r");
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
	M_Entry * contiguous = calloc(n*m,sizeof(M_Entry));
	for(size_t i = 0; i < n; i++) {
		input.matrix[i] = contiguous + m*i;
	}
	uint64_t agents_sum = 0;
	for(size_t i = 0; i < n; i++) {
#ifdef LEGACY 
		fscanf(f," %ld ",&input.agents[i].discr);
#else
		fscanf(f," %ld:%ld ",&input.agents[i].tag,&input.agents[i].discr);
#endif
		agents_sum += input.agents[i].discr;
	}
	uint64_t jobs_sum = 0;
	for(size_t i = 0; i < m; i++) {
#ifdef LEGACY 
		fscanf(f," %ld ",&input.jobs[i].discr);
#else
		fscanf(f," %ld:%ld ",&input.jobs[i].tag,&input.jobs[i].discr);
#endif
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
	print_inst(&input);
	prelim(&input);
	print_inst(&input);
	while(!done(&input)) {
		print_inst(&input);
		step1(&input);
	}
	print_inst(&input);
	uint64_t cost = 0;
	for (size_t i = 0; i < input.height; i++) {
		for (size_t j = 0; j < input.width; j++) {
			cost += input.matrix[i][j].init_cost * input.matrix[i][j].quota;
		}
	}
	printf("Minimum cost: %ld\n",cost);
#ifndef LEGACY
	for (size_t i = 0; i < input.height; i++) {
		for (size_t j = 0; j < input.width; j++) {
			if (input.matrix[i][j].quota != 0) {
				printf("%ld %ld %ld\n", input.agents[i].tag, input.jobs[j].tag, input.matrix[i][j].quota);
			}
		}
	}
#endif
}
