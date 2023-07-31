/*
 * initialsol.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */
#include "basic.h"
#include "initialsol.h"
#include <vector>
#include<math.h>
#include <iostream>
#include <algorithm>
using namespace std;
initial_sol::initial_sol() {
}

initial_sol::~initial_sol() {
	// TODO Auto-generated destructor stub
}
void initial_sol::random_fun()// to construct the greedy solution
{
	for (int i=0;i<=I_data->v_num-1;i++){
		for (int j=1;j<=I_data->num_v;j++)
			s->sol[i][j]=-1;
		s->sol[i][0]=0;
		s->fit_tour[i]=0;
	}
	vector <  int > node;
	for (int i=1;i<=I_data->num_v;i++)
		node.push_back(i);
	random_shuffle(node.begin(), node.end());
	int last[I_data->v_num];// this array is used to save the last city for each routes
	for (int i=0;i<=I_data->v_num-1;i++){// determine that each route includes one city at least.
		s->sol[i][0]=node[i];
		s->sol[i][node[i]]=0;
		last[i]=node[i];
	}
	int row;
	int size_node1=node.size();
	for (int i=I_data->v_num;i<=size_node1-1;i++){// select randomly a route to insert the city i
		row=rand()%I_data->v_num;
		s->sol[row][last[row]]=node[i];
		s->sol[row][node[i]]=0;
		last[row]=node[i];
	}
	int pointer;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		while(1){
			s->fit_tour[i]+=I_data->Distance[pointer][s->sol[i][pointer]];
			pointer=s->sol[i][pointer];
			if (pointer==0)
				break;
		}
	}
	s->fitness=0;
	for (int i=0;i<=I_data->v_num-1;i++)
		if (s->fitness<s->fit_tour[i])
			s->fitness=s->fit_tour[i];
}
