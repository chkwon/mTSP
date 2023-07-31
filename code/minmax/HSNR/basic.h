/*
 * basic.h
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#ifndef BASIC_H_
#define BASIC_H_
#include<algorithm>
#include <vector>
#include<math.h>
#include <iostream>
#include <time.h>
#include <string.h>
#include <fstream>
using namespace std;
///////////////////////////////////////////////////////////////////////////////

#define Max 100000000

#define num_inital 20
#define num_near_nodes 10
#define length_s 7 // the length of each string, the actual value is length_s-1
#define tabu_l  20
#define threshold 50
#define alpha_num 10



#define num_prunning     100

#define control_para 300

#define Probability 400


#define cross_exchange_identify 2

// the parameters to select operators for intra-route and inter-route optimization in local search
#define intra_opt 3                // the number 1, 2 and 3    are 2-opt, tabu search and EAX, respectively
#define inter_opt 3          // the number 1, 2 and 3 are insert operators, solution based tabu search and cross-exchange
#define perturbation_method 2


#endif /* BASIC_H_ */
