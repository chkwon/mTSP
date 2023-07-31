/*
 * sort.h
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */

#ifndef __SORT__
#define __SORT__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <algorithm>
using namespace std;

void InitSort();
void swap(int &x, int &y);						// äº¤æ�¢ä¸¤ä¸ªæ•°
void selectionSort(int* Arg, int l, int r);		// é€‰æ‹©æŽ’åº�
int partition(int* Arg, int l, int r);			// å¿«é€ŸæŽ’åº�åˆ†è§£å­�ç¨‹åº�
void quickSort(int* Arg, int l, int r);			// å¿«é€ŸæŽ’åº�

class TSort{
public:
	TSort();
	~TSort();
	void index( double* Arg, int numOfArg, int* indexOrderd, int numOfOrd );
	void index( int* Arg, int numOfArg, int* indexOrderd, int numOfOrd );
	void indexB( double* Arg, int numOfArg, int* indexOrderd, int numOfOrd );
	void indexB( int* Arg, int numOfArg, int* indexOrderd, int numOfOrd );
	void sort( int* Arg, int numOfArg );
};

extern TSort* tSort;

#endif
