/*
 * Indi.cpp
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#include "Individual.h"

Individual::Individual() {

}
Individual::~Individual() {
	for (int i=0;i<=v_num-1;i++)
	{
		delete [] sol[i];
		delete [] sol_r[i];
	}
	delete [] sol;
	delete [] sol_r;
	delete [] fit_tour;
}
void Individual::define(int number_nodes,int number_vehicles){
	num_v=number_nodes;
	v_num=number_vehicles;
	sol=new int *[v_num];
	sol_r=new int *[v_num];
	fit_tour=new double [v_num];
	for (int i=0;i<=v_num-1;i++)
	{
		sol[i]=new int [num_v+1];
		sol_r[i]=new int [num_v+1];
	}
}
