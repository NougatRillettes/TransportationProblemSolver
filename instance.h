#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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


void print_inst(Instance const * const input);
