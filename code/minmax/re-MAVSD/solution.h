/*
 * solution.h
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_
#include "cross_ctsp.h"
#include"initialsol.h"
#include "read_data.h"
#include "Individual.h"
#include "LS.h"
class solution {
public:
	solution();
	virtual ~solution();
	void define(char* file_name,int time_l,int number);
	void main_loop(int seed);
	read_data * sol_data;
	initial_sol * Init_sol;
	Individual * off;
	Individual * best_s;
	Individual * global_s;
	Individual * Pop;
	Individual *can_pop;
	LS * ls;
	cross_ctsp * crossover;

private:
	void update_best_solution(int index);
	void initial_pop();
	void replace_fun();
	void pop_diversity(int replace);
	void pop_distance();
	int dis_calculate(int a, int b);

	// variables
	int num_ind;// is used to record the number of individuals in iterative process
	int step;
	int time_length;
	int pointer;
};

#endif /* SOLUTION_H_ */


