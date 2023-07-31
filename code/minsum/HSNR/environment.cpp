/*
 * environment.cpp
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */

#ifndef __ENVIRONMENT__
#include "environment.h"
#endif
#include "basic.h"
#include <math.h>
#include<iostream>
using namespace std;

void MakeRandSol(TEvaluator* eval, TIndi& indi);
void Make2optSol(TEvaluator* eval, TIndi& indi);

TEnvironment::TEnvironment(){
	fEvaluator = new TEvaluator();
}

TEnvironment::~TEnvironment(){
	delete [] fIndexForMating;
	delete [] tCurPop;
	int N = fEvaluator->totalCity;
	delete fEvaluator;
	delete tCross;
	delete tKopt;
	for( int i = 0; i < N; ++i ) delete [] fEdgeFreq[ i ];
	delete [] fEdgeFreq;
}
void TEnvironment::input_data(int * given_sol,int count,double *x_coor,double *y_coor)
{
	fEvaluator->setInstance1(given_sol, count,x_coor,y_coor);
	for ( int i = 0; i < Npop; ++i ) tCurPop[i].define1(count);
	tBest.define1(count);// define the best solution
	tCross->input_data(count);
	tKopt->input_data(count);
	tKopt->setInvNearList();
	tCross->Npop = Npop;
	num_nodes=count;
}

void TEnvironment::define(int count,int graph_type){
	fEvaluator->setInstance(count,graph_type);
	int N_c = fEvaluator->totalCity;//æ€»å…±ç‚¹çš„æ•°é‡�
	fIndexForMating = new int [ Npop + 1 ];
	tCurPop = new TIndi [ Npop ];// define the number of population
	for ( int i = 0; i < Npop; ++i ) tCurPop[i].define( N_c );
	tBest.define( N_c );// define the best solution
	tCross = new TCross( N_c );// define a crossover objective by using parameter N which is the number cities
	tCross->eval = fEvaluator;// probe directs it
	tKopt = new TKopt(N_c );
	tKopt->eval = fEvaluator;
	fEdgeFreq = new int* [ N_c ];
	for( int i = 0; i < N_c; ++i ) fEdgeFreq[ i ] = new int [ N_c ];
}

void TEnvironment::doIt(){
	this->fTimeStart = clock();// to calculate the initial time
	this->initPop();				// åˆ�å§‹åŒ–ç§�ç¾¤
	if (num_nodes<10){// in this place, if the number of cities is too small, the k-optis used
		this->setAverageBest();
		return;
	}
	this->fTimeInit = clock();    // initial time
	this->init();
	this->getEdgeFreq();
	no_improve=0;
	change=Max;
	while( 1 ){
		this->setAverageBest();
		if ((change-1.0e-7)>tBest.fEvaluationValue){// indicate the improvement
			change=tBest.fEvaluationValue;
			no_improve=0;
		}
		else
			no_improve++;
//		printf( "%d:\t%f\t%lf\n", fCurNumOfGen, fBestValue, fAverageValue );
		if( this->terminationCondition() || no_improve>100) break;

		this->selectForMating();
		for( int s =0; s < Npop; ++s ) this->generateKids( s );

		++fCurNumOfGen;
	}
	this->fTimeEnd = clock();
}

void TEnvironment::init(){
	fAccumurateNumCh = 0;
	fCurNumOfGen = 0;
	fStagBest = 0;
	fMaxStagBest = 0;
	fStage = 1;				// é˜¶æ®µ1
	fFlagC[ 0 ] = 4;		// ç»´æŒ�ç§�ç¾¤å¤šæ ·æ€§	1:Greedy, 2:---, 3:Distance, 4:Entropy
	fFlagC[ 1 ] = 1;		// Esetç±»åž‹			1:Single-AB, 2:Block2
}

bool TEnvironment::terminationCondition(){
	if ( fAverageValue - fBestValue < 0.00001 )  return true;
	if( fStage == 1 ){
		if( fStagBest == int(1500/Nch) && fMaxStagBest == 0 )			// 1500/Nch
			fMaxStagBest =int( fCurNumOfGen / 10 );						// fMaxStagBest = G/10
		else if( fMaxStagBest != 0 && fMaxStagBest <= fStagBest ){
			fStagBest = 0;
			fMaxStagBest = 0;
			fCurNumOfGen1 = fCurNumOfGen;
			fFlagC[ 1 ] = 2;
			fStage = 2;
		}
		return false;
	}
	if( fStage == 2 ){
		if( fStagBest == int(1500/Nch) && fMaxStagBest == 0 )			// 1500/Nch
			fMaxStagBest = int( (fCurNumOfGen - fCurNumOfGen1) / 10 );	// fMaxStagBest = G/10
		else if( fMaxStagBest != 0 && fMaxStagBest <= fStagBest ) return true;
		return false;
	}

	return true;
}

void TEnvironment::setAverageBest(){
	double stockBest = tBest.fEvaluationValue;
	//int stockBest = tBest.fEvaluationValue;
	fAverageValue = 0.0;
	fBestIndex = 0;
	fBestValue = tCurPop[0].fEvaluationValue;
	for(int i = 0; i < Npop; ++i ){
		fAverageValue += tCurPop[i].fEvaluationValue;
		if( tCurPop[i].fEvaluationValue < fBestValue ){
			fBestIndex = i;
			fBestValue = tCurPop[i].fEvaluationValue;
		}
	}
	tBest = tCurPop[ fBestIndex ];
	fAverageValue /= (double)Npop;
	if( tBest.fEvaluationValue < stockBest ){
		fStagBest = 0;
		fBestNumOfGen = fCurNumOfGen;
		fBestAccumeratedNumCh = fAccumurateNumCh;
	}
	else ++fStagBest;
}

void TEnvironment::initPop(){
	for ( int i = 0; i < Npop; ++i ){
		tKopt->makeRandSol( tCurPop[ i ] );		// è®¾ç½®ä¸€ä¸ªéš�æœºè·¯ç¨‹
		tKopt->doIt( tCurPop[ i ] );			// å±€éƒ¨æ�œç´¢(2-opt neighborhood)
	}
}

void TEnvironment::selectForMating(){
	tRand->permutation( fIndexForMating, Npop, Npop );
	fIndexForMating[ Npop ] = fIndexForMating[ 0 ];
}

void TEnvironment::generateKids( int s ){
	// tCurPop[fIndexForMating[s]] è¢«æ›¿æ�¢ä¸ºtCross->DoIt()ä¸­å­�ä»£çš„ä¸€ä¸ªæœ€ä¼˜è§£
	// fEdgeFreq[][] å�Œæ—¶è¢«æ›´æ–°
	tCross->setParents( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], fFlagC, Nch );
	tCross->doIt( tCurPop[fIndexForMating[s]], tCurPop[fIndexForMating[s+1]], Nch, 1, fFlagC, fEdgeFreq );
	fAccumurateNumCh += tCross->fNumOfGeneratedCh;
}

void TEnvironment::getEdgeFreq(){
	int  k0, k1, N = fEvaluator->Ncity;
	for( int j1 = 0; j1 < N; ++j1 )
		for( int j2 = 0; j2 < N; ++j2 )
			fEdgeFreq[ j1 ][ j2 ] = 0;

	for( int i = 0; i < Npop; ++i )
		for(int j = 0; j < N; ++j ){
			k0 = tCurPop[ i ].fLink[ j ][ 0 ];
			k1 = tCurPop[ i ].fLink[ j ][ 1 ];
			++fEdgeFreq[ j ][ k0 ];
			++fEdgeFreq[ j ][ k1 ];
		}
}

void TEnvironment::printOn( int n ){
	printf( "n = %d val = %f Gen = %d Time = %d %d\n" , n, tBest.fEvaluationValue, fCurNumOfGen,
		(int)((double)(this->fTimeInit - this->fTimeStart)/(double)CLOCKS_PER_SEC),
		(int)((double)(this->fTimeEnd - this->fTimeStart)/(double)CLOCKS_PER_SEC) );
//	fflush(stdout);

}

void TEnvironment::writeBest(int *given_sol){
//	FILE *fp;
//	char filename[ 80 ];

//	sprintf( filename, "bestSolution.txt" );
//	fp = fopen( filename, "a");
	fEvaluator->writeTo(given_sol, tBest );
//	fclose( fp );
}
