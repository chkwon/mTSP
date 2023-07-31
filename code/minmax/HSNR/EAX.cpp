/*
 *
 *
 *
 *
 */

#ifndef __ENVIRONMENT__
#include "environment.h"
#endif
#include<iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void EAX(TEnvironment* gEnv, int * given_sol,int count, double *x_coor,double *y_coor){// given a initial tour and coordiantion
//	InitURandom(); //
	int maxNumOfTrial;
	maxNumOfTrial=1;					// repeated times
//	gEnv->Npop=100;						// number of items
//	gEnv->Nch=30;						// number of offsprings

	int opt_tour[count];
	int cc[count];
	for (int i=0;i<=count-1;i++)
		cc[i]=given_sol[i];
//	cout<<"Initializing ..."<<endl;
//	clock_t start = clock();
	gEnv->input_data(given_sol,count,x_coor,y_coor);
//	cout<<"Building solution ..."<<endl;
	for ( int n = 0; n < maxNumOfTrial; ++n ){
		gEnv->doIt();
		gEnv->writeBest(opt_tour);
	}
	int best_sol[count];
	for (int i=0;i<count;i++)
	{
		best_sol[i]=given_sol[opt_tour[i]-1];
	}
	for (int i=0;i<=count;i++)
		given_sol[i]=best_sol[i];
}
