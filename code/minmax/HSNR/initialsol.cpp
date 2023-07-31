/*
 * initialsol.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Pengfei He
 */
#include "basic.h"
#include "initialsol.h"
initial_sol::initial_sol() {
}

initial_sol::~initial_sol() {
	//
}
void initial_sol::greedy_fun(){
	for (int i=0;i<=I_data->v_num-1;i++){
		for (int j=0;j<=I_data->num_v;j++)
			s->sol[i][j]=-1;
		s->sol[i][0]=0;
		s->fit_tour[i]=0;
	}
	vector <  int > node;
	for (int i=1;i<=I_data->num_v;i++)
		node.push_back(i);
	random_shuffle(node.begin(), node.end());
	for (int i=0;i<=I_data->v_num-1;i++){
		s->sol[i][0]=node[i];
		s->sol[i][node[i]]=0;
		s->fit_tour[i]+=2*I_data->Distance[0][node[i]];
	}
	int pointer,tem;
	int row,pos;double delta,delta_tem;
	int size_node=node.size();
	for (int i=I_data->v_num;i<=size_node-1;i++){
		delta=Max;
		row=0;
		for (int j=1;j<=I_data->v_num-1;j++)
			if (s->fit_tour[row]>s->fit_tour[j])
				row=j;
		pointer=0;
		while (1){
			delta_tem=I_data->Distance[node[i]][pointer]+I_data->Distance[node[i]][s->sol[row][pointer]]-
				I_data->Distance[pointer][s->sol[row][pointer]];
			if (delta>delta_tem){
				pos=pointer;
				delta=delta_tem;
			}
			pointer=s->sol[row][pointer];
			if (pointer==0)break;
		}
		tem=s->sol[row][pos];
		s->sol[row][pos]=node[i];
		s->sol[row][node[i]]=tem;
		s->fit_tour[row]+=delta;
	}
	s->fitness=0;
	for (int i=0;i<=I_data->v_num-1;i++){
		if (s->fitness<s->fit_tour[i])
			s->fitness=s->fit_tour[i];
	}
	// check the solution
	double check_fit=0;
	double fit_route;

	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		fit_route=0;
		while(1){
			fit_route+=I_data->Distance[pointer][s->sol[i][pointer]];
			pointer=s->sol[i][pointer];
			if (pointer==0)break;
		}
		if (fit_route+1.0e-07> s->fit_tour[i] && fit_route-1.0e-07<s->fit_tour[i])
//			cout<<"  rignht "<<endl;
		if (check_fit<fit_route)
			check_fit=fit_route;
	}
	if (check_fit+1.0e-07> s->fitness && check_fit-1.0e-07<s->fitness)
		cout<<"the solution is right"<<endl;
	else
		cout<<"the initial solution is wrong"<<endl;

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
void initial_sol::rand_fun(){
	for (int i=0;i<=I_data->v_num-1;i++){
		for (int j=0;j<=I_data->num_v;j++)
			s->sol[i][j]=-1;
		//s->sol[i][0]=0;
		s->fit_tour[i]=0;
	}
	vector <  int > node;
	for (int i=1;i<=I_data->num_v;i++)
		node.push_back(i);
	random_shuffle(node.begin(), node.end());
	int point[I_data->v_num];
	for (int i=0;i<=I_data->v_num-1;i++){
		s->sol[i][0]=node[i];
		point[i]=node[i];
		//s->sol[i][node[i]]=0;
	}
	int row;
	int size_node=node.size();
	for (int i=I_data->v_num;i<=size_node-1;i++){
		row=rand()%I_data->v_num;
		s->sol[row][point[row]]=node[i];
		point[row]=node[i];
	}
	int pointer;
	for (int i=0;i<=I_data->v_num-1;i++){
		s->sol[i][point[i]]=0;
		pointer=0;
		while(1){
			s->fit_tour[i]+=I_data->Distance[pointer][s->sol[i][pointer]];
			pointer=s->sol[i][pointer];
			if (pointer==0)break;
		}
	}
	s->fitness=0;
	for (int i=0;i<=I_data->v_num-1;i++){
		if (s->fitness<s->fit_tour[i])
			s->fitness=s->fit_tour[i];
	}
	// check the solution
	double check_fit=0;
	double fit_route;

	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		fit_route=0;
		while(1){
			fit_route+=I_data->Distance[pointer][s->sol[i][pointer]];
			pointer=s->sol[i][pointer];
			if (pointer==0)break;
		}
		if (fit_route+1.0e-07> s->fit_tour[i] && fit_route-1.0e-07<s->fit_tour[i])
//			cout<<"  rignht "<<endl;
		if (check_fit<fit_route)
			check_fit=fit_route;
	}
	if (check_fit+1.0e-07> s->fitness && check_fit-1.0e-07<s->fitness)
		cout<<"the solution is right"<<endl;
	else
		cout<<"the initial solution is wrong"<<endl;

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
void initial_sol::initilization(){
	if (I_data->num_v==199 || I_data->num_v==317 || I_data->num_v==531)
		rand_fun();
	else
		greedy_fun();
}
