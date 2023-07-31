/*
 * solution.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */
#include <iostream>
#include <time.h>
#include "basic.h"
#include "solution.h"
#include "cross_ctsp.h"
#include "initialsol.h"
#include "LS.h"
using namespace std;
solution::solution() {
	sol_data=new read_data();// initilization all starting date
}
solution::~solution(){
	delete sol_data;
	delete Init_sol;
	delete can_pop;
	delete off;
	delete best_s;
	delete global_s;
	delete [] Pop;
	delete ls;
	delete crossover;

}
void solution::define(char* file_name,int time_l,int number){
	sol_data->read_fun(file_name,number);
//	cout<<sol_data<<endl;
	Init_sol=new initial_sol ();// initial initial solution object;
	Init_sol->I_data=sol_data;// probe is assigned to direct to previous data
	off=new Individual();// initial two solutions
	off->define(sol_data->num_v,sol_data->v_num);
	can_pop=new Individual();
	can_pop->define(sol_data->num_v,sol_data->v_num);
	best_s=new Individual();
	best_s->define(sol_data->num_v,sol_data->v_num);
	global_s=new Individual();
	global_s->define(sol_data->num_v,sol_data->v_num);
	Pop=new Individual[num_pop];
	for (int i=0;i<=num_pop-1;i++)Pop[i].define(sol_data->num_v,sol_data->v_num);
	//Init_sol->s=off;// the probe direct the new off.
	ls=new LS(sol_data);// local search phase
//	ls->s=off;
	ls->b_s=best_s;
	ls->I_data=sol_data;
	ls->time_length=time_l;
	// crossover
	crossover=new cross_ctsp (sol_data);// perturbation phase
	// define array in heap used in this class.
	time_length=time_l;
}
void solution::initial_pop(){// construct the initial population
	int repeat;double cc;
	for (int i=0;i<=num_pop-1;i++){
		Pop[i].fitness=Max;
	}
	for (int i=0;i<=num_pop-1;){
		repeat=0;
		Init_sol->s=Pop+i;
		Init_sol->random_fun();
		ls->s=Pop+i;
		cc=ls->s->fitness;
		ls->local_search(1);// where the number 1 represents 2-opt
		cout<<i<<"    "<<cc<<"    "<<ls->s->fitness<<endl;
		for (int j=0;j<i;j++){
			if (Pop[i].fitness+1.0e-7>Pop[j].fitness && Pop[i].fitness-1.0e-7<Pop[j].fitness){
				repeat=1;
				break;
			}
		}
		if (repeat==0)
			i++;
		if(ceil((clock() - sol_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
	}
}
void solution::update_best_solution(int index){
	for (int i=0;i<=sol_data->v_num-1;i++)
		for (int j=0;j<=sol_data->num_v;j++)
			global_s->sol[i][j]=Pop[index].sol[i][j];
	global_s->fitness=Pop[index].fitness;
	sol_data->optimal_time=ceil((clock() - sol_data->start)/static_cast<double>(CLOCKS_PER_SEC));
}
void solution::replace_fun(){
		for (int i=0;i<=num_pop-1;i++){
			if ((off->fitness+1.0e-7>Pop[i].fitness) && (off->fitness-1.0e-7<Pop[i].fitness)){// the same with offspring
				return;
			}
		}
	// replace a individual by the off.
		double min_fit=0; int index;
		for (int i=0;i<=num_pop-1;i++){
			if (min_fit<Pop[i].fitness){
				min_fit=Pop[i].fitness;
				index=i;
			}
		}
		if (min_fit>off->fitness){
			for (int i=0;i<=sol_data->v_num-1;i++){
				for (int j=0;j<=sol_data->num_v;j++){
					Pop[index].sol[i][j]=off->sol[i][j];
					Pop[index].sol_r[i][j]=off->sol_r[i][j];
				}
			}
			Pop[index].fitness=off->fitness;
		}
		if (global_s->fitness>off->fitness)
			update_best_solution(index);
}
void solution::main_loop(int seed){
	best_s->fitness=Max;
	global_s->fitness=Max;
	sol_data->start = clock();// start running time
	initial_pop();// define the initial solution
	num_ind=num_pop;// update the initial number of individuals
	// update the best solution
	double delta=Max;int best_index;
	for (int i=0;i<=num_pop-1;i++){
		if (delta>Pop[i].fitness){
			delta=Pop[i].fitness;
			best_index=i;
		}
	}
	update_best_solution(best_index);// update the global best solution
	step=0;int father, mother,other;// which is used to determine the parent for producing offspring
	while (1){
		if(ceil((clock() - sol_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
		can_pop->fitness=Max;
		for (int iter=0;iter<num_pop;iter++){
			if (rand()%1000 <P_c){
				father=rand()%num_pop;
				mother=rand()%num_pop;
				while(father==mother){
					mother=rand()%num_pop;
				}
				if (Pop[father].fitness<Pop[mother].fitness){
					if (rand()%1000 >P_better){
						father=mother;
					}
				}
				else{
					if (rand()%1000 <P_better)
						father=mother;
				}
				mother=rand()%num_pop;
				other=rand()%num_pop;
				while(mother==other){
					other=rand()%num_pop;
				}
				if (Pop[mother].fitness<Pop[other].fitness){
					if (rand()%1000 >P_better){
						mother=other;
					}
				}
				else{
					if (rand()%1000 <P_better)
						mother=other;
				}
				crossover->off=off;
				crossover->fa=Pop+father;
				crossover->ma=Pop+mother;
				crossover->mTour();
			}
			else{
				father=rand()%num_pop;
				mother=rand()%num_pop;
				while(father==mother){
					mother=rand()%num_pop;
				}
				other=rand()%num_pop;
				while ((other==father) || other==mother){
					other=rand()%num_pop;
				}
				if (Pop[father].fitness>Pop[mother].fitness)
					father=mother;
				if (Pop[father].fitness>Pop[other].fitness)
					father=other;
				crossover->off=off;
				crossover->fa=Pop+father;
				crossover->mutation(step);
			}
			if (off->fitness<can_pop->fitness){// record this individual
				for (int i=0;i<=sol_data->v_num-1;i++){
					for (int j=0;j<=sol_data->num_v;j++){
						can_pop->sol[i][j]=off->sol[i][j];
						can_pop->sol_r[i][j]=off->sol_r[i][j];
					}
					can_pop->fit_tour[i]=off->fit_tour[i];
				}
				can_pop->fitness=off->fitness;
			}
		}
		ls->s=can_pop;

		ls->local_search(0);
		step++;
		for (int i=0;i<=sol_data->v_num-1;i++){
			for (int j=0;j<=sol_data->num_v;j++){
				off->sol[i][j]=can_pop->sol[i][j];
				off->sol_r[i][j]=can_pop->sol_r[i][j];
			}
			off->fit_tour[i]=can_pop->fit_tour[i];
		}
		off->fitness=can_pop->fitness;
		//off=can_pop;
		replace_fun();
		cout<<step<<"    "<<global_s->fitness<<endl;
	}
 	sol_data->output_fun(global_s,seed);
}
