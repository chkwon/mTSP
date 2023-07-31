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
	void inter_route_optimization();
	// the methods for inter-routing methods 3
	double gain_fun(int st1,int end1,int st2,int end2,int k, int reverse,int longest_third);
	void replace_fun(int *move);
	void cross_exchange_1();
	void cross_exchange_tabu();
//	void swap();
//	void swap_tabu();
	void calculate_fit(int st1,int end1,int st2,int end2,int k, int reverse);
	void insert_tabu();
	void insert();
	int replace_fun_insert(double min_delta,int row_out,int city_out,int row_in,int city_in);
	double calculate_fit_insert(int row_out,int city_out,int row_in,int city_in,int longest_third);
	void supply_edge();
//	double calculate_fit_swap(int row_out,int city_out,int row_in,int city_in,int longest_third);
//	int replace_fun_swap(double min_delta,int row_out,int city_out,int row_in,int city_in);






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
	int ot;// to record the best route;
	// variables for tabu search.
	int iteration;
	int ** tabu_table;
	int **tabu_table_c;
	int iteration_c;
	int improve;
	//
	int **t_t_swap;



};
#endif /* LS_H_ */





/* void LS::swap_tabu(){
	int pointer;improve=0;
	int pointer_2;
	int longest_tour=0;
	int longest_third;
	double delta,delta_tabu;
	int row_out,city_out,row_in,city_in;
	int can_node[100][4];
	int can_tabu_node[100][4];
	double min_delta,min_tabu_delta;
	int num_best,num_tabu_best;
	int rand_order;
	int i,j;
	for ( i=1;i<=v_num-1;i++){// determine the longest tour;
		if (s->fit_tour[longest_tour]<s->fit_tour[i])
			longest_tour=i;
	}
	while (1){
		min_delta=Max;min_tabu_delta=Max;
		pointer=0;
		if (s->sol[longest_tour][s->sol[longest_tour][pointer]]==0){
			I_data->determing_longest=1;
			return;
		}
		while(1){
			pointer=s->sol[longest_tour][pointer];
			if (pointer==0)break;
			for ( i=0;i<=v_num-1;i++){
				if (i==longest_tour)continue;
				longest_third=-1;
				for ( j=0;j<=v_num-1;j++){
					if (j!=longest_tour && j!=i && longest_third==-1)
						longest_third=j;
					if (longest_third>-1 && j!=longest_tour && j!=i && s->fit_tour[longest_third]<s->fit_tour[j])
						longest_third=j;
				}
				pointer_2=s->sol[i][0];
				while(1){
					delta=calculate_fit_swap(longest_tour,pointer,i,pointer_2,longest_third);
					if (tabu_table[i][pointer]<iteration && tabu_table[longest_tour][pointer_2]<iteration){
						if (delta<min_delta){
							min_delta=delta;
							can_node[0][0]=longest_tour;
							can_node[0][1]=pointer;
							can_node[0][2]=i;
							can_node[0][3]=pointer_2;
							num_best=1;
						}
						else if (min_delta+1.0e-7>delta && min_delta-1.0e-7<delta && num_best<100){
							can_node[num_best][0]=longest_tour;
							can_node[num_best][1]=pointer;
							can_node[num_best][2]=i;
							can_node[num_best][3]=pointer_2;
							num_best++;
						}
					}
					else {
						if (min_tabu_delta>delta){
							min_tabu_delta=delta;
							can_tabu_node[0][0]=longest_tour;
							can_tabu_node[0][1]=pointer;
							can_tabu_node[0][2]=i;
							can_tabu_node[0][3]=pointer_2;
							num_tabu_best=1;
						}
						else if (min_tabu_delta+1.0e-7>delta && min_tabu_delta-1.0e-7<delta && num_tabu_best<100)
						{
							can_tabu_node[num_tabu_best][0]=longest_tour;
							can_tabu_node[num_tabu_best][1]=pointer;
							can_tabu_node[num_tabu_best][2]=i;
							can_tabu_node[num_tabu_best][3]=pointer_2;
							num_tabu_best++;
						}
					}
					pointer_2=s->sol[i][pointer_2];
					if (pointer_2==0)break;
				}
			}
		}
		if ((num_tabu_best>0 && min_tabu_delta+1.0e-7<min_delta && (s->fitness+min_tabu_delta+1.0e-7 <b_s->fitness))|| num_best==0){
			rand_order=rand()%num_tabu_best;
			delta_tabu=min_tabu_delta;
			row_out=can_tabu_node[rand_order][0];
			city_out=can_tabu_node[rand_order][1];
			row_in=can_tabu_node[rand_order][2];
			city_in=can_tabu_node[rand_order][3];
		}
		else{
			rand_order=rand()%num_best;
			delta_tabu=min_delta;
			row_out=can_node[rand_order][0];
			city_out=can_node[rand_order][1];
			row_in=can_node[rand_order][2];
			city_in=can_node[rand_order][3];
		}
		tabu_table[row_out][city_out]=iteration+tabu_l+rand()%tabu_l;
		tabu_table[row_in][city_in]=iteration+tabu_l+rand()%tabu_l;
		longest_tour=replace_fun_swap(delta_tabu,row_out,city_out,row_in,city_in);
		record_table[row_out]=1;
		record_table[row_in]=1;
		if (s->fitness+1.0e-7<b_s->fitness){
			update_best_solution();
			improve=0;iteration_c=iteration;
//			return;
		}
		else
			improve++;

		if (improve>threshold)
			return;
		iteration++;
//		cout<<min_delta<<endl;
	}
} */
/* void LS::swap(){
	int pointer;
	int pointer_2;
	int longest_tour=0;
	int longest_third;
	double delta,min_delta;
	int row_out,city_out,row_in,city_in;
	for (int i=1;i<=v_num-1;i++){// determine the longest tour;
		if (s->fit_tour[longest_tour]<s->fit_tour[i])
			longest_tour=i;
	}
	while (1){
		min_delta=Max;
		pointer=0;
		while(1){
			pointer=s->sol[longest_tour][pointer];
			if (pointer==0)break;
			for (int i=0;i<=v_num-1;i++){
				if (i==longest_tour)continue;
				longest_third=-1;
				for (int j=0;j<=v_num-1;j++){
					if (j!=longest_tour && j!=i && longest_third==-1)
						longest_third=j;
					if (longest_third>-1 && j!=longest_tour && j!=i && s->fit_tour[longest_third]<s->fit_tour[j])
						longest_third=j;
				}
				pointer_2=s->sol[i][0];
				while(1){
					delta=calculate_fit_swap(longest_tour,pointer,i,pointer_2,longest_third);
					if (delta<min_delta){
						min_delta=delta;
						row_out=longest_tour;
						city_out=pointer;
						row_in=i;
						city_in=pointer_2;
					}
					pointer_2=s->sol[i][pointer_2];
					if (pointer_2==0)break;
				}
			}
		}
		if (min_delta+1.0e-7>0)break;
		longest_tour=replace_fun_swap(min_delta,row_out,city_out,row_in,city_in);
		record_table[row_out]=1;
		record_table[row_in]=1;
//		cout<<min_delta<<endl;
	}
} */
/* double LS::calculate_fit_swap(int row_out,int city_out,int row_in,int city_in,int longest_third){
	double delta1=0;
	double delta2=0;
	delta1+=I_data->Distance[s->sol_r[row_out][city_out]][city_in]+I_data->Distance[city_in][s->sol[row_out][city_out]];
	delta1=delta1-I_data->Distance[s->sol_r[row_out][city_out]][city_out]-I_data->Distance[city_out][s->sol[row_out][city_out]];
	//
	delta2+=I_data->Distance[s->sol_r[row_in][city_in]][city_out]+I_data->Distance[s->sol[row_in][city_in]][city_out];
	delta2=delta2-I_data->Distance[s->sol_r[row_in][city_in]][city_in]-I_data->Distance[s->sol[row_in][city_in]][city_in];
	//
	delta1=s->fit_tour[row_out]+delta1;
	delta2=s->fit_tour[row_in]+delta2;
	double delta=delta1;
	if (delta<delta2)
		delta=delta2;
	if (delta<s->fit_tour[longest_third])
		delta=s->fit_tour[longest_third];
	delta=delta-s->fitness;
	return delta;
}
int LS::replace_fun_swap(double min_delta,int row_out,int city_out,int row_in,int city_in){
	double delta1=0;
	double delta2=0;
	delta1+=I_data->Distance[s->sol_r[row_out][city_out]][city_in]+I_data->Distance[city_in][s->sol[row_out][city_out]];
	delta1=delta1-I_data->Distance[s->sol_r[row_out][city_out]][city_out]-I_data->Distance[city_out][s->sol[row_out][city_out]];
	//
	delta2+=I_data->Distance[s->sol_r[row_in][city_in]][city_out]+I_data->Distance[s->sol[row_in][city_in]][city_out];
	delta2=delta2-I_data->Distance[s->sol_r[row_in][city_in]][city_in]-I_data->Distance[s->sol[row_in][city_in]][city_in];
	//
	s->fit_tour[row_out]=s->fit_tour[row_out]+delta1;
	s->fit_tour[row_in]=s->fit_tour[row_in]+delta2;
	int longest_tour=0;
	for (int i=1;i<=v_num-1;i++)
		if (s->fit_tour[i]>s->fit_tour[longest_tour])
			longest_tour=i;
	s->fitness=s->fit_tour[longest_tour];
	//
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
	return longest_tour;
} */
