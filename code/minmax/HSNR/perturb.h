/*
 * perturb.h
 *
 *  Created on: 10 Jul 2020
 *      Author: Peng
 */

#ifndef PERTURB_H_
#define PERTURB_H_
#include "read_data.h"
#include "Individual.h"
class perturb {
public:
	perturb();
	virtual ~perturb();
	void define();

	Individual * s;
	read_data * I_data;
	void perturbation();
	int no_improve;
private:
	void perturb_fun1();
	double determine_delta(int row,int city,int pointer);
	void insert_fun(int city,int row,int loc,double delta_min);
	void perturb_fun2();
	// variables
	int *s_t;
	double prob;
};

#endif /* PERTURB_H_ */
