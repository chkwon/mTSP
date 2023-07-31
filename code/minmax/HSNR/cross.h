/*
 * cross.h
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */

#ifndef __Cross__
#define __Cross__

#ifndef __RAND__
#include "randomize.h"
#endif

#ifndef __Sort__
#include "sort.h"
#endif

#ifndef __INDI__
#include "indi.h"
#endif

#ifndef __EVALUATOR__
#include "evaluator.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

class TCross{
public:
	TCross( int N );
	~TCross();
	void input_data(int N);
	void doIt( TIndi& tKid, TIndi& tPa2, int numOfKids, int flagP, int flagC[ 10 ], int** fEdgeFreq );	// EAXÃ¤Â¸Â»Ã§Â¨â€¹Ã¥Âºï¿½
	void setParents( const TIndi& tPa1, const TIndi& tPa2, int flagC[ 10 ], int numOfKids );			// Ã¨Â®Â¾Ã§Â½Â®Ã§Ë†Â¶Ã¦Å“Â¬Ã¤Â¸Å½Ã¦Â¯ï¿½Ã¦Å“Â¬
	void setABcycle( const TIndi& parent1, const TIndi& parent2, int flagC[ 10 ], int numOfKids );		// Ã¨Â®Â¾Ã§Â½Â®ABcycle

	void swap(int &x, int &y);
	void formABcycle();																// Ã¤Â¿ï¿½Ã¥Â­ËœABcycle
	void changeSol( TIndi& tKid, int ABnum, int type );								// Ã§â€�Â¨ABcycleÃ¤ÂºÂ§Ã§â€�Å¸Ã¤Â¸Â­Ã©â€”Â´Ã¨Â§Â£

	void makeCompleteSol( TIndi& tKid );					// EAXÃ§Â¬Â¬5Ã¦Â­Â¥
	void makeUnit();										// EAXÃ§Â¬Â¬5-1Ã¦Â­Â¥
	void backToPa1( TIndi& tKid );							// Ã¥â€ºÅ¾Ã¦Â»Å¡p_A
	void goToBest( TIndi& tKid );							// Ã¤Â¿Â®Ã¦â€�Â¹tKidÃ¤Â¸ÂºÃ¥Â­ï¿½Ã¤Â»Â£Ã¦Å“â‚¬Ã¤Â¼ËœÃ¨Â§Â£

	void incrementEdgeFreq( int **fEdgeFreq );				// Ã¥Â¢Å¾Ã¥Å Â fEdgeFreq[][]
	int calAdpLoss( int **fEdgeFreq );						// Ã¨Â®Â¡Ã§Â®â€”Ã¥Â¹Â³Ã¥ï¿½â€¡Ã¨Â·Â¯Ã§Â¨â€¹Ã§Å¡â€žÃ¥Â·Â®
	double calEntLoss( int **fEdgeFreq );					// Ã¨Â®Â¡Ã§Â®â€”Ã¨Â¾Â¹Ã§â€ ÂµÃ§Å¡â€žÃ¥Â·Â®

	void setWeight( const TIndi& parent1, const TIndi& parent2 );	// Block2
	int	calCNaive();
	void searchEset( int num );
	void addAB( int num );
	void deleteAB( int num );

	int fNumOfGeneratedCh;
	TEvaluator* eval;
	int Npop;

private:
//	TIndi tBestTmp;
	int fFlagImp;
	int fN;
	int total_fN;////////////////////////////////
	int r;
	int exam;
	int examFlag;
	int flagSt;
	int flagCycle;
	int prType;
	int chDis;
	int koritsuMany;
	int bunkiMany;
	int st;
	int ci;
	int pr;
	int stock;
	int stAppear;
	int fEvalType;
	int fEsetType;
	int fNumOfABcycleInESet;
	int fNumOfABcycle;
	int fPosiCurr;
	int fMaxNumOfABcycle;

	int *koritsu;
	int *bunki;
	int *koriInv;
	int *bunInv;
	int *checkKoritsu;
	int *fRoute;
	int *fPermu;
	int *fC;
	int *fJun;
	int *fOrd1;
	int *fOrd2;

	int **nearData;
	int **fABcycle;

	// Speed Up Start
	int fNumOfUnit;
	int fNumOfSeg;
	int fNumOfSPL;
	int fNumOfElementInCU;
	int fNumOfSegForCenter;
	double fGainModi;
	//int fGainModi;
	int fNumOfModiEdge;
	int fNumOfBestModiEdge;
	int fNumOfAppliedCycle;
	int fNumOfBestAppliedCycle;

	int *fOrder;
	int *fInv;
	int *fSegUnit;
	int *fSegPosiList;
	int *LinkAPosi;
	int *fPosiSeg;
	int *fNumOfElementInUnit;
	int *fCenterUnit;
	int *fListOfCenterUnit;
	int *fSegForCenter;
	//int *fGainAB;
	double *fGainAB;
	int *fAppliedCylce;
	int *fBestAppliedCylce;

	int **fSegment;
	int **LinkBPosi;
	int **fModiEdge;
	int **fBestModiEdge;
	// Speed Up End

	// Block2
	int fNumOfUsedAB;
	int fNumC;
	int fNumE;
	int fTmax;
	int fMaxStag;
	int fNumOfABcycleInEset;
	int fDisAB;
	int fBestNumC;
	int fBestNumE;

	int *fNumOfElementINAB;
	int *fWeightSR;
	int *fWeightC;
	int *fUsedAB;
	int *fMovedAB;
	int *fABcycleInEset;

	int **fInEffectNode;
	int **fWeightRR;
};

#endif
