/*
 * initialsol.h
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#ifndef INITIALSOL_H_
#define INITIALSOL_H_
#include "read_data.h"
#include "Individual.h"
class initial_sol {
public:
	initial_sol();
	virtual ~initial_sol();
	void random_fun();
	Individual * s;
	read_data * I_data;
};

#endif /* INITIALSOL_H_ */
