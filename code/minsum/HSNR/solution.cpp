/*
 * solution.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */
#include "basic.h"
#include "solution.h"
#include "initialsol.h"
#include "LS.h"
#include "EAX.h"
#include "environment.h"
#include "perturb.h"
solution::solution() {
	sol_data=new read_data();// initilization all starting date
}
solution::~solution(){
	delete sol_data;
	delete Init_sol;
	delete curr;
	delete best_s;
	delete global_s;
	delete ls;
	delete gEnv;
	delete per;
	
}
void solution::define(char* file_name,int num_v,int time_l){
	sol_data->read_fun(file_name,num_v);
	Init_sol=new initial_sol ();// initial initial solution object;
	Init_sol->I_data=sol_data;// probe is assigned to direct to previous data
	curr=new Individual();
	curr->define(sol_data->num_v,sol_data->v_num);
	best_s=new Individual();
	best_s->define(sol_data->num_v,sol_data->v_num);
	global_s=new Individual();
	global_s->define(sol_data->num_v,sol_data->v_num);
//	Pop=new Individual[num_pop+lambda];
//	for (int i=0;i<=num_pop+lambda-1;i++)Pop[i].define(sol_data->num_v,sol_data->v_num);
	//Init_sol->s=off;// the probe direct the new off.
	ls=new LS(sol_data);// local search phase
	gEnv = new TEnvironment();
	per = new perturb();
	per->I_data=sol_data;
	per->define();
//	ls->s=off;
	ls->b_s=best_s;
	ls->I_data=sol_data;
	ls->gEnv=gEnv;
	ls->time_length=time_l;
	time_length=time_l;
}
void solution::initial_pop(){// construct the initial population
	for (int i=0;i<=num_initial-1;i++){
		if(ceil((clock() - sol_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
		curr->fitness=Max;
		Init_sol->s=curr;
		Init_sol->greedy_fun();
		ls->s=curr;
		ls->local_search();
		cout<<i<<"  "<<ls->s->fitness<<endl;
		if (curr->fitness<global_s->fitness){
			sol_data->optimal_time=ceil((clock() - sol_data->start)/static_cast<double>(CLOCKS_PER_SEC));
			update_best_solution();
		}
	}
	for (int i=0;i<=sol_data->v_num-1;i++)
		for (int j=0;j<=sol_data->num_v;j++)
			curr->sol[i][j]=global_s->sol[i][j];
	curr->fitness=global_s->fitness;
}
void solution::update_best_solution(){
	for (int i=0;i<=sol_data->v_num-1;i++)
		for (int j=0;j<=sol_data->num_v;j++)
			global_s->sol[i][j]=curr->sol[i][j];
	global_s->fitness=curr->fitness;
}
void solution::main_loop(int seed){
	gEnv->Npop=100;						// number of items
	gEnv->Nch=30;						// number of offsprings
	gEnv->define(sol_data->num_v+1,sol_data->graph_type);//
	best_s->fitness=Max;
	global_s->fitness=Max;
	sol_data->start = clock();// start running time
	initial_pop();// define the initial solution
	best_s->fitness=curr->fitness;
	step=0;int no_improve=0;
	while (1){
		if(ceil((clock() - sol_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
		ls->s=curr;
		//
//		ls->local_search();
		ls->b_s=best_s;
		ls->weak_perturb();
		if (best_s->fitness<global_s->fitness){
			sol_data->optimal_time=ceil((clock() - sol_data->start)/static_cast<double>(CLOCKS_PER_SEC));
			for (int i=0;i<=sol_data->v_num-1;i++){
				for (int j=0;j<=sol_data->num_v;j++)
					global_s->sol[i][j]=best_s->sol[i][j];
			}
			global_s->fitness=best_s->fitness;
			no_improve=0;
		}
		else no_improve++;

		double cost=0;
		int pointer;
		for (int i=0;i<=sol_data->v_num-1;i++){
			pointer=0;
			while(1){
				cost+=sol_data->Distance[curr->sol[i][pointer]][pointer];
				pointer=curr->sol[i][pointer];
				if (pointer==0)break;
			}
		}
		cout<<step<<"      "<<cost<<"    "<<curr->fitness<<"     "<<best_s->fitness<<"    "<<global_s->fitness<<"      "<<no_improve<<endl;
		// enter disturbation phase

//		if (no_improve>perturbation_num){
//			per->s=curr;
//			per->no_improve=no_improve;
//			per->perturbation();
//			no_improve=0;
//		}
		step++;
	}
	sol_data->output_fun(global_s,seed);
}
