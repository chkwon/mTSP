/*
 * solution.h
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_
#include"initialsol.h"
#include "read_data.h"
#include "Individual.h"
#include "LS.h"
#include "environment.h"
#include "perturb.h"
class solution {
public:
	solution();
	virtual ~solution();
	void define(char * file_name,int num_v,int time_length);
	void main_loop(int seed);
	read_data * sol_data;
	initial_sol * Init_sol;
	Individual * curr;
	Individual * best_s;
	Individual * global_s;
//	Individual * Pop;
	LS * ls;
	TEnvironment * gEnv;
	perturb * per;

private:
	void update_best_solution();
	void initial_pop();
	int num_ind;// is used to record the number of individuals in iterative process
	int step;
	int time_length;
};

#endif /* SOLUTION_H_ */




/* void solution::pop_diversity(int replace){// this function is used to calculate the
	if (replace==0){
		div_degree[step]=div_degree[step-1];
		return;
	}
	int pointer;int tem;
	double total_distance=0;double edge=0;
	for (int i=0;i<=num_pop-2;i++)
		for (int j=i+1;j<=num_pop-1;j++){
			for (int i=0;i<=sol_data->v_num-1;i++){
				pointer=Pop[j].sol[i][0];
				while (pointer!=0){
					s_t[pointer]=i;
					pointer=Pop[j].sol[i][pointer];
				}
			}
			for (int k=0;k<=sol_data->v_num-1;k++){
				pointer=0;
				while (1){// the edge with two endpoints: pointer, Pop[i].sol[k][pointer].
					tem=Pop[i].sol[k][pointer];
					if (tem>0){
						if (Pop[j].sol[s_t[tem]][tem]==pointer || Pop[j].sol_r[s_t[tem]][tem]==pointer)
							total_distance++;
						else
							edge++;
						pointer=Pop[i].sol[k][pointer];
					}
					if (tem==0){
						if (Pop[j].sol[s_t[pointer]][pointer]==tem || Pop[j].sol_r[s_t[pointer]][pointer]==tem)
							total_distance++;
						else
							edge++;
						break;
					}
				}
			}
		}
	total_distance=(total_distance*2)/(num_pop*(num_pop-1));
	edge=(edge*2)/(num_pop*(num_pop-1));
	div_degree[step]=edge;
} */
