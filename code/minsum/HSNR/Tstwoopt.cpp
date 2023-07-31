/*
 * Tstwoopt.cpp
 *
 *  Created on: 19 Apr 2020
 *      Author: Peng
 */

#include "Tstwoopt.h"
#include "basic.h"
#include "read_data.h"
using namespace std;
Tstwoopt::Tstwoopt(read_data * data) {
	tabu=new int *[data->num_v+1];
	for (int i=0;i<=data->num_v;i++)
		tabu[i]=new int [data->num_v+1];
	num_v=data->num_v;
}
Tstwoopt::~Tstwoopt() {
	for (int i=0;i<=num_v;i++)
		delete [] tabu[i];
	delete [] tabu;
}
double Tstwoopt::calculat_delta(int u1,int v1,int u2,int v2){
	return(I_data->Distance[u1][u2]+I_data->Distance[v1][v2]-I_data->Distance[u1][v1]-I_data->Distance[u2][v2]);
}

void Tstwoopt::reverse(int * s, int reverse_1,int reverse_2,int n_v){
	int temp;
	while (reverse_2 - reverse_1>0){
		temp=s[reverse_1%n_v];
		s[reverse_1%n_v]=s[reverse_2%n_v];
		s[reverse_2%n_v]=temp;
		reverse_1++;
		reverse_2--;
	}
}
void Tstwoopt::twoopt(int *s, int n_v){// the n is the number of cities in route s.
	int best_s[n_v];
	for (int i=0;i<=n_v-1;i++)
		best_s[i]=s[i];
	int reverse_1,reverse_2;
	double delta_1;
	int v1,v2,u1,u2;
	double min_delta;
	while (1){
		min_delta=Max;min_delta--;
		for (int i=0;i<=n_v-1;i++){
			u1=i;
			v1=(i+1)%n_v;
			for (int j=i+2;(j+1)%n_v!=i;j++){
				u2=j%n_v;
				v2=(j+1)%n_v;
				delta_1=calculat_delta(s[u1],s[v1],s[u2],s[v2]);
				if (min_delta>delta_1){
					min_delta=delta_1;
					reverse_1=i+1;
					reverse_2=j;
				}
			}
		}
		if (min_delta<-1.0e-7)
			reverse(s, reverse_1,reverse_2,n_v);
		else
			break;
	}
	int k;
	for (int i=0;i<=n_v-1;i++)
		best_s[i]=s[i];
	for (int i=0;i<=n_v-1;i++)
		if (best_s[i]==0){
			k=i;break;
			}
	s[0]=0;int c=1;
	for (int j=k+1;best_s[j%n_v]!=0;j++){
		s[c]=best_s[j%n_v];c++;
	}
}
double Tstwoopt::calculate_length_tour(int *s, int n_v){
	double path_length=0;
	for (int i=0;i<=n_v-2;i++)
		path_length+=I_data->Distance[s[i]][s[i+1]];
	path_length+=I_data->Distance[s[n_v-1]][s[0]];
	return path_length;
}
void Tstwoopt::tabu_search(int * s, int n_v)// this code employs 2-opt and tabu search to optimize a traditional TSP
{
	a=0;
	for (int i=0;i<=n_v-1;i++)
		if (s[i]>a)
			a=s[i];
	a++;
	for (int i=0;i<=a-1;i++)
		for (int j=0;j<=a-1;j++)
			tabu[i][j]=0;
//	double delta[num_v][num_v];// it should be noticed becasue the delta value is caltulated by exchanging edges, not nodes
	// in the first step a simple 2-opt algorithm should be employed.
	int best_s[n_v];
	for (int i=0;i<=n_v-1;i++)
		best_s[i]=s[i];
	double path_length=0;
	path_length=calculate_length_tour(s,n_v);
	double best_path=path_length;
	int reverse_1,reverse_2;
	double min_delta,min_tabu_delta;
	int num_best,num_tabu_best;
	double delta_1;
	int step=0;int v1,v2,u1,u2;
	int no_improve=0;
	int tabu_length=ceil (n_v*0.1);
	while (1){
		step++;
		min_delta=Max;min_tabu_delta=Max;num_best=0;num_tabu_best=0;min_delta--;min_tabu_delta--;
		for (int i=0;i<=n_v-1;i++)	{
			u1=i;
			v1=(i+1)%n_v;
			for (int j=i+2;(j+1)%n_v!=i;j++){
				u2=j%n_v;
				v2=(j+1)%n_v;
				delta_1=calculat_delta(s[u1],s[v1],s[u2],s[v2]);
				if (tabu[s[u1]][s[v1]]<=step && tabu[s[u2]][s[v2]]<=step)	{
					if (delta_1<min_delta){
						min_delta=delta_1;
						can_node_1[0]=i+1;
						can_node_2[0]=j;
						num_best=1;
					}
					else if (min_delta==delta_1 && num_best<100){
						can_node_1[num_best]=i+1;
						can_node_2[num_best]=j;
						num_best++;
					}
				}
				else if (tabu[s[u1]][s[v1]]>step || tabu[s[u2]][s[v2]]>step)	{
					if (min_tabu_delta>delta_1)	{
						min_tabu_delta=delta_1;
						can_tabu_node_1[0]=i+1;
						can_tabu_node_2[0]=j;
						num_tabu_best=1;
					}
					else if (min_tabu_delta==delta_1 && num_tabu_best<100)	{
						can_tabu_node_1[num_tabu_best]=i+1;
						can_tabu_node_2[num_tabu_best]=j;
						num_tabu_best++;
					}
				}
			}
		}
		int rand_order;
		if ((num_tabu_best>0 && min_tabu_delta+1.0e-7<min_delta && (path_length + min_tabu_delta+1.0e-7< best_path)) || num_best==0){
			path_length+=min_tabu_delta;
			rand_order=rand()% num_tabu_best;
			reverse_1=can_tabu_node_1[rand_order];
			reverse_2=can_tabu_node_2[rand_order];
			tabu[s[(reverse_1-1)%n_v]][s[reverse_2%n_v]]=step+tabu_length+rand()%tabu_length;
			tabu[s[(reverse_2+1)%n_v]][s[reverse_1%n_v]]=step+tabu_length+rand()%tabu_length;
			reverse(s, reverse_1,reverse_2,n_v);
		}
		else{
			path_length+=min_delta;
			rand_order=rand()%num_best;
			reverse_1=can_node_1[rand_order];
			reverse_2=can_node_2[rand_order];
			tabu[s[(reverse_1-1)%n_v]][s[reverse_2%n_v]]=step+tabu_length+rand()%tabu_length;
			tabu[s[(reverse_2+1)%n_v]][s[reverse_1%n_v]]=step+tabu_length+rand()%tabu_length;
			reverse(s,reverse_1,reverse_2,n_v);
		}
		if (best_path>path_length)	{
			best_path=path_length;
			for (int i=0;i<=n_v-1;i++)
				best_s[i]=s[i];
			no_improve=0;
		}
		else
			no_improve++;
		if (n_v<10){
			if (no_improve>=5)//10
				break;
			if (step>50)
				break;
		}
		else{
			if (no_improve>(n_v))//10   an important parameters
			break;
			if (step>n_v*5*10)
				break;
		}
	}
	int k;
	for (int i=0;i<=n_v-1;i++)
		if (best_s[i]==0)
		{	k=i;break;}
	s[0]=0;int c=1;
	for (int j=k+1;best_s[j%n_v]!=0;j++){
		s[c]=best_s[j%n_v];c++;
	}
}
