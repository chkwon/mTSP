/*
 * randomize.h
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */

#ifndef __RAND__
#define __RAND__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

extern void InitURandom( int );
extern void InitURandom( void );

class TRandom {
public:
	TRandom();
	~TRandom();
	int Integer( int minNumber, int maxNumber );
	double Double( double minNumber, double maxNumber );
	double normalDistribution( double mu, double sigma );
	void permutation( int *array, int numOfelement, int numOfSample );	// éš�æœºäº§ç”Ÿç§�ç¾¤çš„ä¸€ä¸ªæŽ’åˆ—
	void shuffle( int *array, int numOfElement );						// æ ¹æ�®äº§ç”Ÿçš„æŽ’åˆ—è°ƒæ•´ç§�ç¾¤ä¸­çš„ä¸ªä½“ä½�ç½®
};

extern TRandom* tRand;


#endif


