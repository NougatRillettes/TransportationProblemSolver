#include <assert.h>
#include <stdlib.h>

#include "core.h"

#define MAX(a,b) \
	({ __typeof__ (a) _a = (a); \
	 __typeof__ (b) _b = (b); \
	 _a > _b ? _a : _b; })
#define MIN(a,b) \
	({ __typeof__ (a) _a = (a); \
	 __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })

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
