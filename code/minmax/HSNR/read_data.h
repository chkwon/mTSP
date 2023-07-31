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
#include "basic.h"
class read_data {
public:
	read_data();
	virtual ~read_data();
	void define();
	void read_fun(char* file_name,int number_node);
	void output_fun(Individual *g_best_p ,int seed);// this function is employed to output the optimal results.
	int num_v;
	int v_num;
	int determing_longest;
	double ** Distance;// distance between all cities
	int ** near_ns;// the nearest nodes of each node.
	double *x;
	double *y;
	int ** use_frequency;// this variable is used to record the using frequency of each operators in cross_exchange;
	clock_t start;// start computing time
	int graph_type;
	int optimal_time;// this variable is used to record the time when getting the best solution.
	// for spanning tree
	int **edge;
	int alpha=alpha_num;
	// parameters
private:
	int i;
	int j;
	int k;
	string instance_n;
	int ** near;
	/////////////////////////////////////////////////////////////////////////
	// define node structure
	int *check_city;
	struct node{
		int father;
		int *son;
		int order;
		int num_son;
	};
	node * city;
	double *low_cost;
	double **beta;
	int **record;
	void spanning_tree();
	void visit_tree(int root);
	void alpha_sparse_graph();
	void visit_tree_beta(int root);
	void post_travel_tree(int root);
	double get_max(double a,double b);
	void Qsort(int i,int low, int high);
	int key;
};

#endif /* READ_DATA_H_ */
