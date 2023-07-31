/*
 * main.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */
#include"solution.h"
#include<iostream>
using namespace std;
int main(int argc, char*argv[]){
	if (argc!=5){
		cout << "the input parameters in configuration are wrong" << endl;
		return 0;
	}
	int rand_num;
	int time_length;
	int number;
	sscanf(argv[2],"%d",&number);
	sscanf(argv[3],"%d",&time_length);
	sscanf(argv[4],"%d",&rand_num);
	solution* sol=new solution();// assign a new object for class solution.
	sol->define(argv[1],time_length,number);// initialization all data and assigning memory
	srand(rand_num);
	sol->main_loop(rand_num);// in this place the running time could be input
	delete sol;
	return 0;
}
