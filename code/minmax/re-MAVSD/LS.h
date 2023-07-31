/*
 * LS.h
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#ifndef LS_H_
#define LS_H_
#include "Individual.h"
#include "read_data.h"
#include "Tstwoopt.h"
class LS {
public:
	// methods
	LS(read_data * data);
	virtual ~LS();
	void local_search(int algorithm);
	// variables
	Individual* s;
	Individual*b_s;
	Individual * tem;
	read_data * I_data;
	Tstwoopt * each_route;
	int time_length;

private:
	void one_point();
	void or_opt2();
	void or_opt3();
	void or_opt4();
	void get_s_t();
	void update_best_solution();
	void reverse_fun();
	void intra_route_optimization();
	void replace(int long_tour,int pointer,int p_end,int tour,int pos_l,int iden);
	double calculate_data(int long_tour,int pointer,int p_end,int tour,int pos_l);
	// general variables
	int num_v;
	int v_num;
	int* record_table;
	int * s_t;// this variable is used to record cities are belonged to which routes during in searching
	int opt_2;
};
#endif /* LS_H_ */
