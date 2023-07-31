/*
 * Indi.h
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

class Individual {
public:
	Individual();
	virtual ~Individual();
	void define(int number_nodes,int number_vehicles);
	int ** sol;
	int ** sol_r;
	int num_v;
	int v_num;
	double *fit_tour;
	double fitness;
};

#endif /* INDI_H_ */
