/*
 * LS.cpp
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#include "LS.h"
#include "basic.h"
#include "Tstwoopt.h"
//#include "EAX.h"
#include <iostream>
#include <vector>
#include "Individual.h"
using namespace std;
LS::LS(read_data * data) {
	num_v=data->num_v;
	v_num=data->v_num;
//	num_s=data->num_s;
	tem=new Individual();
	tem->define(num_v,v_num);
	each_route=new Tstwoopt (data);
	each_route->I_data=data;
	record_table=new int [v_num];
	s_t=new int [num_v+1];

}
LS::~LS() {
	delete tem;
	delete each_route;
	delete [] record_table;
	delete [] s_t;
}
void LS::update_best_solution(){
	if (s->fitness < b_s->fitness){
		for (int i=0;i<=v_num-1;i++){
			for (int j=0;j<=num_v;j++){
				b_s->sol[i][j]=s->sol[i][j];
				b_s->sol_r[i][j]=s->sol_r[i][j];
			}
			b_s->fit_tour[i]=s->fit_tour[i];
		}
		b_s->fitness=s->fitness;
	}
}
void LS::reverse_fun(){// get value for sol_r;
	int pointer;
	vector<int >tour;
	for (int i=0;i<=v_num-1;i++)	{
		pointer=s->sol[i][0];
		while (pointer!=0)		{
			tour.push_back(pointer);
			pointer=s->sol[i][pointer];
		}
		s->sol_r[i][0]=tour.back();
		tour.pop_back();
		pointer=s->sol_r[i][0];
		while (tour.size())		{
			s->sol_r[i][pointer]=tour.back();
			pointer=s->sol_r[i][pointer];
			tour.pop_back();
		}
		s->sol_r[i][pointer]=0;
	}
}
void LS::intra_route_optimization(){// this method will include many methods to optimize each route, such as 2-opt, Lk and EAX.
	tem->fitness=s->fitness;
	int pointer;
	for (int i=0;i<=v_num-1;i++)
		for (int j=0;j<=num_v;j++){
			tem->sol[i][j]=s->sol[i][j];
			tem->sol_r[i][j]=s->sol_r[i][j];
		}
	int number_routes[v_num];
	int route[num_v+1];
	for (int i=0;i<=v_num-1;i++){
		if (record_table[i]==1){
			record_table[i]=0;
			pointer=s->sol[i][0];number_routes[i]=0;
			route[number_routes[i]]=0;number_routes[i]++;
			while (1){
				if (pointer==0)
					break;
				else{
					route[number_routes[i]]=pointer;
					number_routes[i]++;
					pointer=s->sol[i][pointer];
				}
			}
			int sub_route[number_routes[i]];
			for (int j=0;j<=number_routes[i]-1;j++)
				sub_route[j]=route[j];
			if (number_routes[i]<=4)continue;
			if (opt_2==1)
				each_route->twoopt(sub_route,number_routes[i]);
			pointer=0;


			for (int j=1;j<=number_routes[i]-1;j++){
				tem->sol[i][pointer]=sub_route[j];
				pointer=tem->sol[i][pointer];
			}
			tem->sol[i][pointer]=0;
			pointer=0;
			for (int j=number_routes[i]-1;j>=1;j--){
				tem->sol_r[i][pointer]=sub_route[j];
				pointer=tem->sol_r[i][pointer];
			}
			tem->sol_r[i][pointer]=0;
		}
		else
			continue;
	}
	double current_cost=0;
	for (int i=0;i<=v_num-1;i++){
		pointer=0;tem->fit_tour[i]=0;
		while (1){
			tem->fit_tour[i]+=I_data->Distance[pointer][tem->sol[i][pointer]];
			if (tem->sol[i][pointer]==0)
				break;
			else
				pointer=tem->sol[i][pointer];
		}
	}
	for (int i=0;i<=v_num-1;i++){
		if (tem->fit_tour[i]>current_cost)
			current_cost=tem->fit_tour[i];
	}
	if (current_cost<s->fitness){
		s->fitness=current_cost;
		for (int i=0;i<=v_num-1;i++){
			for (int j=0;j<=num_v;j++){
				s->sol[i][j]=tem->sol[i][j];
				s->sol_r[i][j]=tem->sol_r[i][j];
			}
			s->fit_tour[i]=tem->fit_tour[i];
		}
	}
	update_best_solution();
}
void LS::get_s_t(){
	int pointer=0;
	for (int i=0;i<=num_v;i++)
		s_t[i]=-1;
	for (int i=0;i<=v_num-1;i++){
		pointer=s->sol[i][pointer];
		while(1){
			s_t[pointer]=i;
			pointer=s->sol[i][pointer];
			if (pointer==0)
				break;
		}
	}
}
double LS::calculate_data(int long_tour,int pointer,int p_end,int tour,int pos_l){
	double delta,delta1,delta2;
	delta1=I_data->Distance[s->sol_r[long_tour][pointer]][s->sol[long_tour][p_end]]-
			I_data->Distance[s->sol_r[long_tour][pointer]][pointer]-
			I_data->Distance[s->sol[long_tour][p_end]][p_end];
	delta2=	I_data->Distance[pos_l][pointer]+I_data->Distance[s->sol[tour][pos_l]][p_end]-
			I_data->Distance[pos_l][s->sol[tour][pos_l]];
	double reduce=delta1+s->fit_tour[long_tour];
	double reduce2=delta2+s->fit_tour[tour];
	if (reduce>reduce2)
		delta=reduce;
	else
		delta=reduce2;
	delta=delta-s->fitness;
	return delta;
}
void LS::replace(int long_tour,int pointer,int p_end,int tour,int pos_l,int iden){
	int p_pre,p_next;
	if (iden==1){
		p_pre=s->sol_r[long_tour][pointer];
		p_next=s->sol[long_tour][pointer];
		s->sol[long_tour][p_pre]=p_next;
		s->sol[long_tour][pointer]=-1;
		s->sol_r[long_tour][p_next]=p_pre;
		s->sol_r[long_tour][pointer]=-1;
		//
		p_next=s->sol[tour][pos_l];
//		p_pre=s->sol_r[tour][pos_l];
		s->sol[tour][pos_l]=pointer;
		s->sol[tour][pointer]=p_next;
		s->sol_r[tour][p_next]=pointer;
		s->sol_r[tour][pointer]=pos_l;
	}
	if (iden>1){
		vector <int >substring;
		int p=pointer;substring.push_back(p);
		while (1){
			p=s->sol[long_tour][p];
			substring.push_back(p);
			if (p==p_end)
				break;
		}
		p_pre=s->sol_r[long_tour][pointer];
		p_next=s->sol[long_tour][p_end];
		s->sol[long_tour][p_pre]=p_next;
		s->sol_r[long_tour][p_next]=p_pre;
		int size_node=substring.size();
		for (int i=0;i<=size_node-1;i++){
			s->sol[long_tour][substring[i]]=-1;
			s->sol_r[long_tour][substring[i]]=-1;
		}
		//
//		for (int i=0;i<=size_node-1;i++)
//			cout<<substring[i]<<"  ";
//		cout<<endl;
		p_next=s->sol[tour][pos_l];
		s->sol[tour][pos_l]=substring[0];
		for ( int i=1;i<=size_node-1;i++){
			s->sol[tour][substring[i-1]]=substring[i];
		}
		s->sol[tour][substring[size_node-1]]=p_next;

		s->sol_r[tour][p_next]=substring[size_node-1];
		for (int j=size_node-1;j>=1;j--){
			s->sol_r[tour][substring[j]]=substring[j-1];
		}
		s->sol_r[tour][substring[0]]=pos_l;
	}


	int p;
	for (int i=0;i<=v_num-1;i++){
		if (i==long_tour ||i==tour){
			p=0;s->fit_tour[i]=0;
			while(1){
				s->fit_tour[i]+=I_data->Distance[p][s->sol[i][p]];
				p=s->sol[i][p];
				if (p==0)break;
			}
		}
	}
	s->fitness=0;
	for (int i=0;i<=v_num-1;i++){
		if (s->fitness<s->fit_tour[i])
			s->fitness=s->fit_tour[i];
	}
}
void LS::one_point(){
	get_s_t();// get each node's information
	int pointer,long_tour,tour,pos_l;
	double delta;
	long_tour=0;
	for (int i=1;i<=v_num-1;i++){
		if (s->fit_tour[long_tour]<s->fit_tour[i]){
			long_tour=i;
		}
	}
	int pointer_2;
	pointer=s->sol[long_tour][0];
	while (1){
		for (int i=0;i<=v_num-1;i++){
			if (i==long_tour)continue;
			pointer_2=0;
			while(1){
				delta=calculate_data(long_tour,pointer,pointer,i,pointer_2);// pointer inserts into row i after city pointer_2
				if (delta<0){
					tour=i;pos_l=pointer_2;break;
				}
				pointer_2=s->sol[i][pointer_2];
				if (pointer_2==0)break;
			}
			if (delta<0)
				break;
		}
		if (delta<0)break;
		pointer=s->sol[long_tour][pointer];
		if (pointer==0)break;
	}
	if (delta<0)
		replace(long_tour,pointer,pointer,tour,pos_l,1);
}
void LS::or_opt2(){
	int pointer,p_end,long_tour,tour,pos_l;
	double delta;
	long_tour=0;
	for (int i=1;i<=v_num-1;i++){
		if (s->fit_tour[long_tour]<s->fit_tour[i]){
			long_tour=i;
		}
	}
	int pointer_2;
	pointer=s->sol[long_tour][0];
	p_end=s->sol[long_tour][pointer];if (p_end==0)return;
	while (1){
		for (int i=0;i<=v_num-1;i++){
			if (i==long_tour)continue;
			pointer_2=0;
			while(1){
				delta=calculate_data(long_tour,pointer,p_end,i,pointer_2);// pointer inserts into row i after city pointer_2
				if (delta<0){
					tour=i;pos_l=pointer_2;break;
				}
				pointer_2=s->sol[i][pointer_2];
				if (pointer_2==0)break;
			}
			if (delta<0)
				break;
		}
		if (delta<0)break;
		pointer=s->sol[long_tour][pointer];
		p_end=s->sol[long_tour][p_end];
		if (p_end==0 )break;
	}
	if (delta<0)
		replace(long_tour,pointer,p_end,tour,pos_l,2);
}
void LS::or_opt3(){
	int pointer,p_end,long_tour,tour,pos_l;
	double delta;
	long_tour=0;
	for (int i=1;i<=v_num-1;i++){
		if (s->fit_tour[long_tour]<s->fit_tour[i]){
			long_tour=i;
		}
	}
	int pointer_2;
	pointer=s->sol[long_tour][0];
	p_end=s->sol[long_tour][pointer];if (p_end==0)return;
	p_end=s->sol[long_tour][p_end];
	if (p_end==0)return;
	while (1){
		for (int i=0;i<=v_num-1;i++){
			if (i==long_tour)continue;
			pointer_2=0;
			while(1){
				delta=calculate_data(long_tour,pointer,p_end,i,pointer_2);// pointer inserts into row i after city pointer_2
				if (delta<0){
					tour=i;pos_l=pointer_2;break;
				}
				pointer_2=s->sol[i][pointer_2];
				if (pointer_2==0)break;
			}
			if (delta<0)
				break;
		}
		if (delta<0)break;
		pointer=s->sol[long_tour][pointer];
		p_end=s->sol[long_tour][p_end];
		if (p_end==0)break;
	}
	if (delta<0)
		replace(long_tour,pointer,p_end,tour,pos_l,3);
}
void LS::or_opt4(){
	int pointer,p_end,long_tour,tour,pos_l;
	double delta;
	long_tour=0;
	for (int i=1;i<=v_num-1;i++){
		if (s->fit_tour[long_tour]<s->fit_tour[i]){
			long_tour=i;
		}
	}
	int pointer_2;
	pointer=s->sol[long_tour][0];
	p_end=s->sol[long_tour][pointer];if (p_end==0)return;
	p_end=s->sol[long_tour][p_end];if (p_end==0)return;
	p_end=s->sol[long_tour][p_end];
	if (p_end==0)return;
	while (1){
		for (int i=0;i<=v_num-1;i++){
			if (i==long_tour)continue;
			pointer_2=0;
			while(1){
				delta=calculate_data(long_tour,pointer,p_end,i,pointer_2);// pointer inserts into row i after city pointer_2
				if (delta<0){
					tour=i;pos_l=pointer_2;break;
				}
				pointer_2=s->sol[i][pointer_2];
				if (pointer_2==0)break;
			}
			if (delta<0)
				break;
		}
		if (delta<0)break;
		pointer=s->sol[long_tour][pointer];
		p_end=s->sol[long_tour][p_end];
		if (p_end==0 )break;
	}
	if (delta<0)
		replace(long_tour,pointer,p_end,tour,pos_l,3);
}
void LS::local_search(int algorithm){
	for (int i=0;i<=v_num-1;i++)
		record_table[i]=1;
	if (algorithm==1){// go to 2-opt
		opt_2=1;
		intra_route_optimization();
		reverse_fun();
		return;
	}
	else
		opt_2=0;

	reverse_fun();// get value for sol_r;
	opt_2=1;
	one_point();
	or_opt2();
	or_opt3();
	or_opt4();
	intra_route_optimization();
	//cout<<ceil((clock() - I_data->start)/static_cast<double>(CLOCKS_PER_SEC))<<endl;
}

