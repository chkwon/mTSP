/*
 * perturb.cpp
 *
 *  Created on: 10 Jul 2020
 *      Author: Peng
 */

#include "perturb.h"
#include "read_data.h"
#include "Individual.h"
#include "basic.h"
perturb::perturb() {
}

perturb::~perturb() {
	delete [] s_t;
}
void perturb::define(){
	s_t=new int [I_data->num_v+1];
}
double perturb::determine_delta(int row,int city,int pointer){
	double delta=0;
	delta=I_data->Distance[pointer][city]+I_data->Distance[city][s->sol[row][pointer]];
	delta=delta-I_data->Distance[pointer][s->sol[row][pointer]];
	return delta;
}
void perturb::insert_fun(int city,int row,int loc,double delta_min){
	int n_p=s->sol[row][loc];
	s->sol[row][loc]=city;
	s->sol[row][city]=n_p;
	s->fitness+=delta_min;
}

void perturb::perturb_fun1(){
	int pointer;int n_p,p_p;
	vector<int >node;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=s->sol[i][0];
		while (1){
			if (pointer==0)break;
			if (rand()%1000<prob){// a route should include at least a city
				node.push_back(pointer);
				n_p=s->sol[i][pointer];
				p_p=s->sol_r[i][pointer];
				s->sol[i][p_p]=n_p;
				s->sol[i][pointer]=-1;
				s->sol_r[i][n_p]=p_p;
				s->sol_r[i][pointer]=-1;
				pointer=n_p;
			}
			else
				pointer=s->sol[i][pointer];
		}
	}
	random_shuffle(node.begin(), node.end());
	int node_size=node.size();
	double delta,delta_min;int row,loc;
	for (int i =0;i<=node_size-1;i++){
		delta_min=Max;
		for (int j=0;j<=I_data->v_num-1;j++){
			pointer=0;
			if (s->sol[j][pointer]==0 && pointer==0){
				delta=determine_delta(j,node[i],pointer);
				row=j;
				loc=pointer;
				delta_min=delta;
				break;
			}
			else{
				pointer=0;
				while(1){
					delta=determine_delta(j,node[i],pointer);
					if (delta_min>delta){
						delta_min=delta;
						row=j;
						loc=pointer;
					}
					pointer=s->sol[j][pointer];
					if (pointer==0)
						break;
				}
			}
		}
		insert_fun(node[i],row,loc,delta_min);
	}
	// check;
	int count=0;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		while(1){
			pointer=s->sol[i][pointer];
			if (pointer>0)count++;
			else break;
		}
	}
	if (!(count==I_data->num_v)){
		cout<<"initial solution is wrong"<<endl;
		exit(0);
	}
}
void perturb::perturb_fun2(){
	int pointer;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		while (1){
			pointer=s->sol[i][pointer];
			if (pointer==0)break;
			s_t[pointer]=i;
		}
	}
	int n_p,p_p;
	vector<int >node;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=s->sol[i][0];
		while (1){
			if (pointer==0)break;
			if (rand()%1000<prob){// a route should include at least a city
				node.push_back(pointer);
				n_p=s->sol[i][pointer];
				p_p=s->sol_r[i][pointer];
				s->sol[i][p_p]=n_p;
				s->sol[i][pointer]=-1;
				s->sol_r[i][n_p]=p_p;
				s->sol_r[i][pointer]=-1;
				pointer=n_p;
			}
			else
				pointer=s->sol[i][pointer];
		}
	}
	random_shuffle(node.begin(), node.end());
	int node_size=node.size();
	double delta,delta_min;int row,loc;
	for (int i =0;i<=node_size-1;i++){
		delta_min=Max;
		for (int j=0;j<=I_data->v_num-1;j++){
			if (s_t[node[i]==j])continue;
			pointer=0;
			if (s->sol[j][pointer]==0 && pointer==0){
				delta=determine_delta(j,node[i],pointer);
				row=j;
				loc=pointer;
				delta_min=delta;
				break;
			}
			else{
				pointer=0;
				while(1){
					delta=determine_delta(j,node[i],pointer);
					if (delta_min>delta){
						delta_min=delta;
						row=j;
						loc=pointer;
					}
					pointer=s->sol[j][pointer];
					if (pointer==0)
						break;
				}
			}
		}
		insert_fun(node[i],row,loc,delta_min);
	}
	// check;
	int count=0;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		while(1){
			pointer=s->sol[i][pointer];
			if (pointer>0)count++;
			else break;
		}
	}
	if (!(count==I_data->num_v)){
		cout<<"initial solution is wrong"<<endl;
		exit(0);
	}
}

void perturb:: perturbation(){

//	cout<<prob<<"   ";

//	prob=Probability;
	if (perturbation_method==1)
		perturb_fun1();
	if (perturbation_method==2)
		perturb_fun2();
}
