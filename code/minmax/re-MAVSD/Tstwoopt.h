/*
 * Tstwoopt.h
 *
 *  Created on: 19 Apr 2020
 *      Author: Peng
 */

#ifndef TSTWOOPT_H_
#define TSTWOOPT_H_
#include "read_data.h"
class Tstwoopt {
public:
	Tstwoopt(read_data * data);
	virtual ~Tstwoopt();
	void twoopt(int *s, int n);
	void tabu_search(int *s, int n_v);
	read_data * I_data;

private:
	double calculate_length_tour(int* s,int n_v);
	double calculat_delta(int u1,int v1,int u2,int v2);
	void reverse(int * s, int reverse_1,int reverse_2,int n_v);
	int num_v;
};

#endif /* TSTWOOPT_H_ */
