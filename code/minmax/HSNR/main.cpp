/*
 * main.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */
#include"solution.h"
#include"basic.h"
int main(int argc, char*argv[]){
	if (argc!=5){
		cout << "the input parameters in configuration are wrong" << endl;	return 1;
	}
	int rand_num;
	int time_length;
	int num_v;
	sscanf(argv[2],"%d",&num_v);
	sscanf(argv[3],"%d",&time_length);
	sscanf(argv[4],"%d",&rand_num);
	solution* sol=new solution();// assign a new object for class solution.
	sol->define(argv[1],num_v,time_length);// initialization all data and assigning memory
	srand(rand_num);
	sol->main_loop(rand_num);// in this place the running time could be input
	delete sol;
	return 0;
}
