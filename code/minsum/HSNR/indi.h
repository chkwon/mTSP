/*
 * indi.h
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */

#ifndef __INDI__
#define __INDI__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

class TIndi{
public:
	TIndi();
	~TIndi();
	void define1( int N );
	void define( int N );
	TIndi& operator = ( const TIndi& src );
	bool operator == (  const TIndi& indi2 );	// åˆ¤æ–­ä¸¤è·¯å¾„æ˜¯å�¦ç›¸å�Œ

	int total_fN;
	int fN;						// åŸŽå¸‚æ•°ç›®
	int** fLink;				// fLink[i][]è¡¨ç¤ºè·¯å¾„ä¸­içš„ä¸¤ä¸ªç›¸é‚»çš„åŸŽå¸‚     double linked list
	double fEvaluationValue;		// TSPè·¯å¾„é•¿åº¦
	//int fEvaluationValue;		// TSPè·¯å¾„é•¿åº¦
};

#endif
