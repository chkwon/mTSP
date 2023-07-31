/*
 * readdata.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#include "read_data.h"
#include "basic.h"
#include "Individual.h"
#include <string.h>
#include <fstream>
#include<sstream>
#include <cmath>
#include<iostream>
using namespace std;
read_data::read_data() {

}
read_data::~read_data() {
	for ( i=0;i<=num_v;i++)
	{
		delete [] Distance[i];
	}
	delete [] Distance;

	delete [] x;
	delete [] y;
}
void read_data::define(){
	Distance=new double *[num_v+1];
	for ( i=0;i<=num_v;i++){
		Distance[i]=new double [num_v+1];
	}
	x=new double [num_v+1];
	y=new double [num_v+1];

}
void read_data::read_fun(char* file_name,int number){
	//	instance_n=file_name;
		ifstream open_file(file_name);
		num_v=number;
	//	char aa[100];
		char bb[1000];
		open_file>>instance_n;open_file>>bb;
		open_file>>v_num;
		define();
		int c;  //
		for ( i=0;i<=num_v;i++) {
			open_file>>c;open_file>>x[i];open_file>>y[i];//
		}
		open_file.close();
		if ( strcmp( bb, "EUC_2D" ) == 0  ){
			graph_type=1;
			for ( i=0;i<=num_v;i++)
				for ( j=0;j<=num_v;j++)
						Distance[i][j]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
		}
		if ( strcmp( bb, "ATT" ) == 0  ){
			graph_type=2;
			for( int i = 0; i <=num_v; ++i ){
				for( int j = 0; j <=num_v; ++j ) {
					double r = (sqrt(((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]))/10.0));
					int t = (int)r;
					if( (double)t < r ) {
						Distance[ i ][ j ] = t+1;
					}
					else {
						Distance[ i ][ j ] = t;
					}
				}
			}
		}
		if ( strcmp( bb, "CEIL_2D" ) == 0  ){
			graph_type=3;
			for ( i=0;i<=num_v;i++)
				for ( j=0;j<=num_v;j++)
					Distance[i][j]=ceil(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
		}
}
void read_data::output_fun(Individual *g_best_p ,int seed){// this function is used to
	//void read_data::output_fun(Individual *g_best_p ,int seed,int ** s_node){// this function is used to
	int pointer;
	char name_instance[100]={0};
	strcpy(name_instance, instance_n.c_str());
	int size=strlen (name_instance);
	name_instance[size]='_';
	size=strlen( name_instance);
	char num_vehcile[10];
	sprintf(num_vehcile, "%d", v_num);
	int size_1=strlen(num_vehcile);
	for (int i=0;i<=size_1-1;i++)	{
		name_instance[size]=num_vehcile[i];size++;
	}
	name_instance[size]='_';size++;
	char seed_name[10];
	sprintf(seed_name, "%d", seed);
	size_1=strlen( seed_name);
	for (int i=0;i<=size_1-1;i++)	{
		name_instance[size]=seed_name[i];size++;
	}
	char tx[4]={'.','t','x','t'};
	for (int i=0;i<=3;i++){
		name_instance[size]=tx[i];size++;
	}
	cout<<name_instance<<endl;
	ofstream out (name_instance);

	out<<"the best solution's fitness is "<<endl<<g_best_p->fitness<<endl;

	out<<"the time when getting the optimal solution is "<<endl<<optimal_time<<endl;

	//out<< "the time get the best solution is "<<endl<<optimal_time<<endl;
	out<<"the sequence of best solution is following"<<endl;
	for (int i=0;i<=v_num-1;i++){
		pointer=0;
		while (1){
			out<<pointer<<" ";
			pointer=g_best_p->sol[i][pointer];
			if (pointer==0)
				break;
		}
		out<<endl;
	}
	out<<endl;
		double cost=0;int p;double cost_tem;
		for (int i=0;i<=v_num-1;i++){
			p=0;cost_tem=0;
			while (1){
				cost_tem+=Distance[p][g_best_p->sol[i][p]];
				if (g_best_p->sol[i][p]==0)
					break;
				p=g_best_p->sol[i][p];
			}
			if (cost<cost_tem)
				cost=cost_tem;
		}
		out<<"the actuall costs is "<<cost<<endl;
}


