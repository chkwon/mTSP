/*
 * Tstwoopt.cpp
 *
 *  Created on: 19 Apr 2020
 *      Author: Peng
 */

#include "Tstwoopt.h"
#include "basic.h"
#include <iostream>
#include <math.h>
#include "read_data.h"
using namespace std;
Tstwoopt::Tstwoopt(read_data * data) {
	num_v=data->num_v;
}
Tstwoopt::~Tstwoopt() {

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

