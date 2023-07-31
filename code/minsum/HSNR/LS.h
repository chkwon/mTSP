/*
 * LS.h
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#ifndef LS_H_
#define LS_H_
#include "Individual.h"
#include "read_data.h"
#include "Tstwoopt.h"
#include "environment.h"
class LS {
public:
	// methods
	LS(read_data * data);
	virtual ~LS();
	void local_search();
	void weak_perturb();
	// variables
	Individual* s;
	Individual*b_s;
	Individual * tem;
	read_data * I_data;
	Tstwoopt * each_route;
	TEnvironment * gEnv;
	int time_length;

private:
	void update_best_solution();
	void reverse_fun();
	void intra_route_optimization();
	void cross_exchange();
	// the methods for inter-routing methods 3
	double gain_fun(int st1,int end1,int row1,int st2,int end2,int row2,int k, int reverse);
	void replace_fun(int *move);
	void cross_exchange_1();
	void cross_exchange_2();
	void swap();
	double calculate_fit_swap(int row_out,int city_out,int row_in,int city_in);
	void replace_fun_swap(int row_out,int city_out,int row_in,int city_in);

	void insert();
	void insert_tabu();
	void replace_fun_insert(double min_delta,int row_out,int city_out,int row_in,int city_in);
	double calculate_fit_insert(int row_out,int city_out,int row_in,int city_in);
	void supply_edge();
	// general variables
	int num_v;
	int v_num;
	int* record_table;
	int * s_t;// this variable is used to record cities are belonged to which routes during in searching
	// variables for inter-routing methods 3
	int ** cand_str1;
	int ** cand_str2;
	double * cand_fit;
	int **near_cand1;
	double *near_fit;
	int *rever_list;
	// variables for tabu search.
	int iteration;
	int ** tabu_table;
	int **tabu_table_c;
	int iteration_c;
	int improve;
	//variables for tabu_strategry 3
	int c;

};
#endif /* LS_H_ */



/*

void LS::swap(){
	int pointer,pointer_2;
	int row_out,city_out,row_in,city_in;
	double delta,min_delta;
	while(1){
		min_delta=Max;
		for (int i=0;i<=v_num-1;i++){
			pointer=0;
			while(1){
				pointer=s->sol[i][pointer];
				if (pointer==0)break;
				for (int j=0;j<v_num-1;j++){
					if (j==i)continue;
					pointer_2=0;
					while(1){
						pointer_2=s->sol[j][pointer_2];
						if (pointer_2==0)break;
						delta=calculate_fit_swap(i,pointer,j,pointer_2);
						if (delta<min_delta){
							min_delta=delta;
							row_out=i;
							city_out=pointer;
							row_in=j;
							city_in=pointer_2;
						}
					}
				}
			}
		}
		if (min_delta+1.0e-7>0)break;
		s->fitness+=min_delta;
		replace_fun_swap(row_out,city_out,row_in,city_in);
		record_table[row_out]=1;
		record_table[row_in]=1;
	}
}
void LS::replace_fun_swap(int row_out,int city_out,int row_in,int city_in){
	int p_p,n_p;
	p_p=s->sol_r[row_out][city_out];
	n_p=s->sol[row_out][city_out];
	s->sol[row_out][p_p]=city_in;
	s->sol[row_out][city_in]=n_p;
	s->sol[row_out][city_out]=-1;
	s->sol_r[row_out][n_p]=city_in;
	s->sol_r[row_out][city_in]=p_p;
	s->sol_r[row_out][city_out]=-1;
	//
	p_p=s->sol_r[row_in][city_in];
	n_p=s->sol[row_in][city_in];
	s->sol[row_in][p_p]=city_out;
	s->sol[row_in][city_out]=n_p;
	s->sol[row_in][city_in]=-1;
	s->sol_r[row_in][n_p]=city_out;
	s->sol_r[row_in][city_out]=p_p;
	s->sol_r[row_in][city_in]=-1;
}
double LS::calculate_fit_swap(int row_out,int city_out,int row_in,int city_in){
	double delta=0;
	delta+=I_data->Distance[s->sol_r[row_out][city_out]][city_in]+I_data->Distance[city_in][s->sol[row_out][city_out]];
	delta=delta-I_data->Distance[s->sol_r[row_out][city_out]][city_out]-I_data->Distance[s->sol[row_out][city_out]][city_out];
	//
	delta+=I_data->Distance[s->sol_r[row_in][city_in]][city_out]+I_data->Distance[city_out][s->sol[row_in][city_in]];
	delta=delta-I_data->Distance[s->sol_r[row_in][city_in]][city_in]-I_data->Distance[s->sol[row_in][city_in]][city_in];
	return delta;
} */
