/*
 * readdata.h
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#ifndef READ_DATA_H_
#define READ_DATA_H_
#include "Individual.h"
#include <time.h>
#include"basic.h"
class read_data {
public:
	read_data();
	virtual ~read_data();
	void define();
	void read_fun(char* file_name,int number);
	void output_fun(Individual *g_best_p ,int seed);// this function is employed to output the optimal results.
	int num_v;
	int v_num;
	int ex;
	double ** Distance;// distance between all cities
	int ** near_ns;// the nearest nodes of each node.
	double *x;
	double *y;
	clock_t start;// start computing time
	int graph_type;
	int optimal_time;// this variable is used to record the time when getting the best solution.
private:
	int i;
	int j;
	int k;
	string instance_n;
};

#endif /* READ_DATA_H_ */
