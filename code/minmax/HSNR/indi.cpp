/*
 * indi.cpp
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */


#ifndef __INDI__
#include "indi.h"
#endif

TIndi::TIndi(){
	fN = 0;
	fLink = NULL;
	fEvaluationValue = 0;
}

TIndi::~TIndi(){
	for ( int i = 0; i < total_fN; ++i ) delete[] fLink[ i ];
	delete[] fLink;
}
void TIndi::define1( int N ){
	fN = N;
}
void TIndi::define( int N ){
	total_fN=N;
	fLink = new int* [ total_fN ];
	for( int i = 0; i < total_fN; ++i ) fLink[ i ] = new int [ 2 ];
}

TIndi& TIndi::operator = ( const TIndi& src ){
	fN = src.fN;
	for ( int i = 0; i < fN; ++i )
		for ( int j = 0; j < 2; ++j ) fLink[i][j] = src.fLink[i][j];
	fEvaluationValue = src.fEvaluationValue;
	return *this;
}

bool TIndi::operator == ( const TIndi& src ){
	int curr, next, pre, flag_identify;

	if( fN != src.fN ) return false;
	if( fEvaluationValue != src.fEvaluationValue ) return false;

	curr = 0;
	pre = -1;
	for( int i = 0; i < fN; ++i ){
		if( fLink[curr][0] == pre ) next = fLink[curr][1];
		else next = fLink[curr][0];

		if( src.fLink[curr][0] != next && src.fLink[curr][1] != next ) return false;
		pre = curr;
		curr = next;
	}
	return true;
}

