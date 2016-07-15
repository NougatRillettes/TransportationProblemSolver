#include <stdlib.h> 
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>

#define MAX(a,b) \
	({ __typeof__ (a) _a = (a); \
	 __typeof__ (b) _b = (b); \
	 _a > _b ? _a : _b; })
#define MIN(a,b) \
	({ __typeof__ (a) _a = (a); \
	 __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })

typedef struct MatrixEntry {
	uint64_t init_cost;
	uint64_t val;
	uint64_t quota;
	bool starred;
	bool primed;
} M_Entry;

typedef struct AgentJobEntry {
	bool marked;
	uint64_t tag; //An arbitrary tag given to each agent or job
	uint64_t discr; //discrepancy
} AJ_Entry;

typedef struct Instance {
	const size_t height;
	const size_t width;
	M_Entry** const matrix;
	AJ_Entry* const agents; //lines
	AJ_Entry* const jobs; //columns
} Instance;

// Printing functions
// ------------------

#define NUMLENGTH 3
#define NUMLENGTHSTR "3"
void print_inst(Instance const * const input) {
	for(int i = 0; i < NUMLENGTH + 0; i++)
		putchar(' ');
	putchar('|');
	for(int i = 0; i < input->width; i++) {
		printf(" %"NUMLENGTHSTR"lu |",input->jobs[i].discr);
	}
	putchar('\n');
	for(int i = 0; i < input->height; i++) {
		for(int i = 0; i < NUMLENGTH + 1 + input->width *(NUMLENGTH + 3); i++)
			putchar('-');
		putchar('\n');
		printf("%"NUMLENGTHSTR"lu|",input->agents[i].discr);
		for(int j = 0; j < input->width;j++) {
			printf("%"NUMLENGTHSTR"ld|%c|",input->matrix[i][j].val,input->matrix[i][j].starred ? '*' : ' ');
		}
		putchar('\n');
		for(int i = 0; i < NUMLENGTH + 0; i++)
			putchar(' ');
		putchar('|');
		for(int j = 0; j < input->width;j++) {
			printf("%"NUMLENGTHSTR"ld|%c|",input->matrix[i][j].quota,input->matrix[i][j].primed ? '\'' : ' ');
		}

		putchar('\n');
	}
	putchar('\n');
	putchar('\n');
}

// Core logic
// ----------

void step1(Instance * const input);
void step2(Instance * const input,const size_t i, const size_t j);
void step3(Instance * const input);


void step1(Instance * const input) {
	bool something_changed = true;
	while (something_changed) {
		something_changed = false;
		for (size_t i = 0; i < input->height; i++) {
			for (size_t j = 0; j < input->width; j++) {
				if (input->matrix[i][j].val == 0 && !(input->agents[i].marked || input->jobs[j].marked)) { //Choose a non covered zeroargv
					something_changed = true;
					input->matrix[i][j].primed = true; //and prime it
					if (input->agents[i].discr > 0) { //if the discr. of this row is positive
						step2(input,i,j); //go at once to step2
						return;
					} else { //otherwise
						input->agents[i].marked = true; //cover the row
						for (size_t k = 0; k < input->width; k++) { 
							if (input->agents[i].marked && input->jobs[k].marked 
									&& input->matrix[i][k].quota > 0) {
								input->matrix[i][k].starred = true; //star each twice covered essential zero
								input->jobs[k].marked = false; //and uncover its column
							}
						}
					}
				}
			}
		}
	}
	step3(input);
}

void step2(Instance * input,const size_t i0, const size_t j0) {
	size_t current_j = j0;
	uint64_t min_quota = input->agents[i0].discr;

	size_t max_dim = MAX(input->width,input->height);

	M_Entry ** primed_seen = calloc(max_dim,sizeof(M_Entry*));
	M_Entry ** starred_seen = calloc(max_dim,sizeof(M_Entry*));
	size_t idx = 0;

	bool seen_star = true;
	while (seen_star) {
		seen_star = false;
		for (size_t i = 0; i < input->height; i++) {
			if (input->matrix[i][current_j].starred) {
				seen_star = true;
				if (min_quota > input->matrix[i][current_j].quota) {
					min_quota = input->matrix[i][current_j].quota;
				}
				starred_seen[idx] = &input->matrix[i][current_j];
				for(size_t j = 0; j < input->width;j++) {
					if (input->matrix[i][j].primed) {
						current_j = j;
						primed_seen[idx] = &input->matrix[i][j];
						idx++;
						break;
					}
				}
				break;
			}
		}
	}
	min_quota = MIN(min_quota,input->jobs[current_j].discr);

	input->matrix[i0][j0].quota += min_quota;
	//only changing discr are first row and last col.
	input->agents[i0].discr -= min_quota;
	input->jobs[current_j].discr -= min_quota;

	while	(idx > 0) {
		idx--;
		primed_seen[idx]->quota += min_quota;
		starred_seen[idx]->quota -= min_quota;
	}

	free(primed_seen);
	free(starred_seen);


	for(size_t i = 0; i < input->height; i++) {
		for(size_t j = 0; j < input->width; j++) {
			input->matrix[i][j].starred = false;
			input->matrix[i][j].primed = false;
		}
		input->agents[i].marked = false;
	}
	for(size_t j = 0; j < input->width; j++) {
		if (input->jobs[j].discr == 0) {
			input->jobs[j].marked = true;
		}
	}
}

void step3(Instance * input) {
	uint64_t h = UINT64_MAX;
	for(size_t i = 0; i < input->height; i++) {
		for(size_t j = 0; j < input->width; j++) {
			if (!input->agents[i].marked && !input->jobs[j].marked) {
				h = MIN(h,input->matrix[i][j].val);
			}
		}
	}
	assert(h>0);
	for(size_t i = 0; i < input->height; i++) {
		for(size_t j = 0; j < input->width; j++) {
			if (input->agents[i].marked) {
				input->matrix[i][j].val	+= h;
			}
			if (!input->jobs[j].marked) {
				input->matrix[i][j].val -= h;
			}
		}
	}
}

void prelim(Instance * const input) {
	for(size_t i = 0; i < input->height; i++) {
		uint64_t m = UINT64_MAX;	
		for(size_t j = 0; j < input->width; j++) {
			m = MIN(m,input->matrix[i][j].val);
		}
		for(size_t j = 0; j < input->width; j++) {
			input->matrix[i][j].val -= m;
		}
	}
	for(size_t j = 0; j < input->width; j++) {
		uint64_t m = UINT64_MAX;	
		for(size_t i = 0; i < input->height; i++) {
			m = MIN(m,input->matrix[i][j].val);
		}
		for(size_t i = 0; i < input->height; i++) {
			input->matrix[i][j].val -= m;
		}
	}
	for (size_t i = 0; i < input->height; i++) {
		for (size_t j = 0; j < input->width; j++) {
			if (input->matrix[i][j].val == 0 && input->agents[i].discr > 0 && input->jobs[j].discr > 0) {
				uint64_t m = MIN(input->agents[i].discr,input->jobs[j].discr);
				input->matrix[i][j].quota += m;
				input->agents[i].discr -= m;
				input->jobs[j].discr -= m;
			}
		}
	}
}

bool done(Instance const * const input) {
	for (size_t i = 0; i < input->height; i++) {
		if (input->agents[i].discr > 0) {
			return false;
		}
	}
	for (size_t j = 0; j < input->width; j++) {
		if (input->jobs[j].discr > 0) {
			return false;
		}
	}
	return true;
}




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
