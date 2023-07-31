/*
 * evaluator.h
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */

#ifndef __EVALUATOR__
#define __EVALUATOR__

#ifndef __INDI__
#include "indi.h"
#endif

#include <string.h>
#include <assert.h>

class TEvaluator{
public:
	TEvaluator();
	~TEvaluator();
	void setInstance(int count,int graph_type);		// è®¾ç½®å�˜é‡�
	void setInstance1( int * given_sol,int count,double *x_coor,double *y_coor);		// è®¾ç½®å�˜é‡�
	//void setInstance( char filename[] );		// è®¾ç½®å�˜é‡�
	void doIt( TIndi& indi );					// è®¾ç½®indi.fEvaluationValue
	void writeTo( int * given_sol, TIndi& indi );		// è¾“å‡ºTSPè·¯å¾„
	bool checkValid( int* array, double value );	// æ£€æŸ¥TSPè·¯å¾„å�ˆæ³•æ€§

	int fNearNumMax;	// è¿‘é‚»ç‚¹æ•°ç›®çš„æœ€å¤§å€¼
	int **fNearCity;	// NearCity[i][k]è¡¨ç¤ºè·�ç¦»iæœ€è¿‘çš„ç¬¬kä¸ªåŸŽå¸‚
	double **fEdgeDis;		// EdgeDis[i][j]è¡¨ç¤ºåŸŽå¸‚iåˆ°jçš„è·�ç¦»
	int Ncity;			// åŸŽå¸‚æ•°ç›®
	int totalCity;
	double *x;			// x[i]è¡¨ç¤ºç¬¬iä¸ªåŸŽå¸‚çš„xå��æ ‡
	double *y;			// y[i]è¡¨ç¤ºç¬¬iä¸ªåŸŽå¸‚çš„yå��æ ‡

	int graph_t;
//	int *Array;			// æœ€ä¼˜è§£çš„èŠ‚ç‚¹æ ‡å�·
};

#endif
