/*
 * perturbationfun.h
 *
 *  Created on: 19 Apr 2020
 *      Author: Peng
 */

#ifndef CROSS_CTSP_H_
#define CROSS_CTSP_H_
#include "Individual.h"
#include "read_data.h"
#include <vector>
class cross_ctsp {
public:
	cross_ctsp(read_data * data);
	virtual ~cross_ctsp();
	void mTour();
	void mutation(int iter);
	read_data * I_data;
	Individual * fa;
	Individual * ma;
	Individual * off;
private:
	// these methods are used by Edge_nn();
	double get_delta(int left,int insert,int right);
	double determine_prob(int iter);
	void repair();
	int *s_t_fa;// to record the cities are included in which route
	int *s_t_ma;
	int *s_t_off;
	int **father;
	int **mother;
	int **father_r;
	int **mother_r;
	int num_v;
	int v_num;
};

#endif /* CROSS_CTSP_H_ */
