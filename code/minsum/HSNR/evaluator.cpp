/*
 * evaluator.cpp
 *   created on: April 24, 2013
 * last updated: June 13, 2013
 *       author: liushujia
 */

#ifndef __EVALUATOR__
#include "evaluator.h"
#endif
TEvaluator::TEvaluator(){
	fEdgeDis = NULL;
	fNearCity = NULL;
	Ncity = 0;
	fNearNumMax = 50;
}

TEvaluator::~TEvaluator(){
	for ( int i = 0; i < totalCity; ++i ) delete[] fEdgeDis[ i ];
	delete[] fEdgeDis;

	for ( int i = 0; i < totalCity; ++i ) delete[] fNearCity[ i ];
	delete[] fNearCity;

	delete [] x;
	delete [] y;
}
void TEvaluator::setInstance(int count ,int graph_type){
	totalCity=count;
	graph_t=graph_type;
	x = new double [ totalCity ];
	y = new double [ totalCity ];
	fEdgeDis = new double * [totalCity];
	for( int i = 0; i < totalCity; ++i ) fEdgeDis[ i ] = new double [ totalCity ];//fEdgeDis[ i ] = new int [ Ncity ];
	fNearCity = new int* [ totalCity ];
	for( int i = 0; i < totalCity; ++i ) fNearCity[ i ] = new int [ fNearNumMax+1 ];
}
void TEvaluator::setInstance1( int * given_sol,int count,double *x_coor,double *y_coor){
	Ncity=count;
	int *checkedN = new int[Ncity];// dynamic array with a probe
	for (int i=0;i<Ncity;++i)
	{
		x[i]=x_coor[given_sol[i]];
		y[i]=y_coor[given_sol[i]];
	}
	// calculate the distance matrix
	if (graph_t==1){
		for( int i = 0; i < Ncity ; ++i )
			for( int j = 0; j < Ncity ; ++j )
				fEdgeDis[ i ][ j ]=(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
	}
	if (graph_t==2){
		for( int i = 0; i < Ncity ; ++i )
			for( int j = 0; j < Ncity ; ++j ){
				double r = (sqrt(((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]))/10.0));
				int t = (int)r;
				if( (double)t < r ) {
					fEdgeDis[ i ][ j ] = t+1;
				}
				else {
					fEdgeDis[ i ][ j ] = t;
				}
				//fEdgeDis[ i ][ j ]=ceil(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
			}
	}
	if (graph_t==3){
		for( int i = 0; i < Ncity ; ++i )
			for( int j = 0; j < Ncity ; ++j )
				fEdgeDis[ i ][ j ]=ceil(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
	}

	if (graph_t==4){
		double q1,q2,q3;
		double RRR = 6378.388;
		double latitude[Ncity];
		double longitude[Ncity];
		double min;
		double deg;
		double PI=3.141592;
		for (int i=0;i<Ncity;i++){
			deg=int(x[i]);
			min=x[i]-deg;
			latitude[i]=PI * (deg + 5.0 * min / 3.0 ) / 180.0;
			deg=int(y[i]);
			min=y[i]-deg;
			longitude[i]=PI * (deg + 5.0 * min / 3.0 ) / 180.0;
		}
		for (int i=0;i<Ncity;i++)
			for (int j=0;j<Ncity;j++){
				q1 = cos( longitude[i] - longitude[j] );
				q2 = cos( latitude[i] - latitude[j] );
				q3 = cos( latitude[i] + latitude[j] );
				fEdgeDis[i][j] = (int) ( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);
			}
	}


	int ci, j1, j2, j3;
	int cityNum = 0;
	double minDis;
	//int minDis;
	for( ci = 0; ci < Ncity; ++ci ){      // find the neareat nodes for all nodes
		for( j3 = 0; j3 < Ncity; ++j3 ) checkedN[ j3 ] = 0;
		checkedN[ ci ] = 1;
		fNearCity[ ci ][ 0 ] = ci;
		for( j1 = 1; j1 <= fNearNumMax; ++j1 ) {
			minDis = 100000000;
			for( j2 = 0; j2 < Ncity; ++j2 ){
				if( fEdgeDis[ ci ][ j2 ] <= minDis && checkedN[ j2 ] == 0 ){
					cityNum = j2;
					minDis = fEdgeDis[ ci ][ j2 ];
				}
			}
			fNearCity[ ci ][ j1 ] = cityNum;
			checkedN[ cityNum ] = 1;
		}
	}
	delete [] checkedN;
}

void TEvaluator::doIt( TIndi& indi ){
	double d = 0;
	//int d = 0;
	for( int i = 0; i < Ncity; ++i ) d += fEdgeDis[ i ][ indi.fLink[i][0] ] + fEdgeDis[ i ][ indi.fLink[i][1] ];
	indi.fEvaluationValue = d/2;
}

void TEvaluator::writeTo( int * given_sol, TIndi& indi ){
	int *Array=new int[Ncity];
	int curr=0, st=0, count=0, pre=-1, next;
	while( 1 ){
		Array[ count++ ] = curr + 1;
		if( count > Ncity ){
			printf( "Invalid\n" );
			return;
		}
		if( indi.fLink[ curr ][ 0 ] == pre ) next = indi.fLink[ curr ][ 1 ];
		else next = indi.fLink[ curr ][ 0 ];

		pre = curr;
		curr = next;
		if( curr == st ) break;
	}
	if( this->checkValid( Array, indi.fEvaluationValue ) == false )
		printf( "Individual is invalid \n" );
	for( int i = 0; i < indi.fN; ++i )
		given_sol[i]=Array[i];
	delete [] Array;
}

bool TEvaluator::checkValid( int* array, double value ){
	int *check=new int[Ncity];
	for( int i = 0; i < Ncity; ++i ) check[ i ] = 0;
	for( int i = 0; i < Ncity; ++i ) ++check[ array[ i ]-1 ];
	for( int i = 0; i < Ncity; ++i )
		if( check[ i ] != 1 ) return false;
	double distance = 0;
	for( int i = 0; i < Ncity-1; ++i )
		distance += fEdgeDis[ array[ i ]-1 ][ array[ i+1 ]-1 ];

	distance += fEdgeDis[ array[ Ncity-1 ]-1 ][ array[ 0 ]-1 ];

	delete [] check;
	if( abs(distance - value)>1.0e-7 ) return false;
	//if( distance != value ) return false;
	return true;
}

