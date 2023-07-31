//============================================================================
// Name        : IWO.cpp
// Author      : pengfei he
// Version     : This code is used to implement IWO solving CTSP, multiple traveling salesman problem
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
// read_me: the data structure of this code for solution is an array whose value is the index of the next city
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <cmath>
#include <string>
#include <iterator>
#include <valarray>
#include <stdlib.h>
#include <algorithm>
using namespace std;
#define n_max 100 // the number of onlooker bee
#define neb1 100
#define Max 100000000
#define s_max   5
#define s_min   1
#define limit_scout 100
#define iter_max    1000



#define obj    2// i is objective 1 and 2 is objective 2



struct  solution{  // all dynamic array should be defined as gloal variables
	int **sol;
	int ** sol_r;
	double * list_cost;// this array is used to save the cost for each tour.
	double fit;
	int no_impro;
};
int neb;
int ex;
int num_v;
int v_num;
int time_length;
int seed;
int *x_y;// this variable is used to save the coordination of all nodes and characteristics which means which nodes could be visited by vehicles.
double ** Distance;
int step=1;
double P_method;
double Pmin;
double Pmax;
int count_sol;
int graph_type;
solution best_food;// to keep the best food
// all parameters which need to be adjusted should be added in basic.h.
solution food [neb1+neb1*s_max];//in this code, the number of employ bee is the number of food
solution new_food;
clock_t start;
int optimal_time;
string aa;
void release_memory(){
	for (int i=0;i<=num_v;i++){
		delete [] Distance[i];
	}
	for (int i=0;i<=v_num-1;i++){
		delete [] best_food.sol[i];
		delete [] best_food.sol_r[i];
		delete [] new_food.sol[i];
		delete [] new_food.sol_r[i];
	}
	delete [] Distance;
	delete [] x_y;
	delete [] best_food.sol;
	delete [] best_food.sol_r;
	delete [] new_food.sol;
	delete [] new_food.sol_r;
	delete [] new_food.list_cost;
	for (int i=0;i<=neb1+neb1*s_max-1;i++){
		for (int j=0;j<=v_num-1;j++)	{
			delete [] food[i].sol[j];
			delete [] food[i].sol_r[j];
		}
		delete [] food[i].sol;
		delete [] food[i].sol_r;
		delete [] food[i].list_cost;
	}
}
void output_function(){
	int pointer;
	char name_instance[1000]={0};
	cout<<aa<<endl;
	strcpy(name_instance, aa.c_str());
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
	ofstream out (name_instance);

	out<<"the best solution's fitness is "<<endl<<best_food.fit<<endl;
	out<< "the time get the best solution is "<<endl<<optimal_time<<endl;
	out<<"the sequence of best solution is following"<<endl;
	for (int i=0;i<=v_num-1;i++)	{
		pointer=0;
		while (1){
			out<<pointer<<" ";
			pointer=best_food.sol[i][pointer];
			if (pointer==0)
				break;
		}
		out<<endl;
	}
	out<<endl;
	int p;double cost;
	if (obj==1){
		cost=0;
		for (int i=0;i<=v_num-1;i++)	{
			p=0;
			while (1){
				cost+=Distance[p][best_food.sol[i][p]];
				if (best_food.sol[i][p]==0)
					break;
				p=best_food.sol[i][p];
			}
		}
		out<<"the actuall costs is "<<cost<<endl;
	}
	if (obj==2){
		double delta;cost=0;
		for (int i=0;i<=v_num-1;i++){
			p=0;delta=0;
			while (1){
				delta+=Distance[p][best_food.sol[i][p]];
				if (best_food.sol[i][p]==0)
					break;
				p=best_food.sol[i][p];
			}
			if (cost<delta)
				cost=delta;
		}
		out<<"the actuall costs is "<<cost<<endl;
	}
}
int build_instance(char *file_name){
	ifstream open_file(file_name);
	x_y=new int [num_v+1];// the first and second colmun is used to save the coordinate and the third is used to save the characteristics
	for (int i=0;i<=num_v;i++)
		x_y[i]=0;
	//char aa[1000];
	open_file>>aa;
	char bb[1000];
	//int num;
	while (1){
		open_file>>bb;open_file>>v_num;
		break;
	}
	int c;double x[num_v+1];double y[num_v+1];    //
	for (int i=0;i<=num_v;i++){
		open_file>>c;open_file>>x[i];open_file>>y[i];//
	}
	open_file.close();
	Distance =new double * [num_v+1];
	for (int i=0;i<=num_v;i++)
		Distance[i]=new double [num_v+1];
	int i,j;
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

	return 1;
}
void construct(int n){
	for (int j=0;j<=v_num-1;j++){
		for (int k=0;k<=num_v;k++){
			food[n].sol[j][k]=-1;
			food[n].sol_r[j][k]=-1;
		}
	}
	food[n].fit=Max;
	vector<int >node;// this vector is used to keep all
	for (int i=0;i<=num_v;i++)
		node.push_back(i);// the first node is 0 and the last node is num_v;
	random_shuffle(node.begin()+1, node.end());// except node 0, the remain nodes are random.
	for (int i=0;i<=v_num-1;i++){
		//cout<<node[i+1]<<endl;
		food[n].sol[i][0]=node[i+1];
		food[n].sol[i][node[i+1]]=0;
	}
	double delta;int pointer;double tem;int loc;int row;int n_1;
	if (obj==1){
		for (int i=v_num+1;i<=num_v;i++){
			if (((rand()%100)/100.0)<P_method){// method 1
				delta=Max;
				for (int j=0;j<=v_num-1;j++){
					pointer=0;
					while(1){
						tem=Distance[pointer][node[i]]+Distance[node[i]][food[n].sol[j][pointer]]-Distance[pointer][food[n].sol[j][pointer]];
						if (tem<delta){
							delta=tem;
							loc=pointer;
							row=j;
						}
						if (food[n].sol[j][pointer]==0)
							break;
						pointer=food[n].sol[j][pointer];
					}
				}
				n_1=food[n].sol[row][loc];
				food[n].sol[row][loc]=node[i];
				food[n].sol[row][node[i]]=n_1;
			}
			else {
				row=rand()%v_num;
				delta=Max;
				pointer=0;
				while(1){
					tem=Distance[pointer][node[i]]+Distance[node[i]][food[n].sol[row][pointer]]-Distance[pointer][food[n].sol[row][pointer]];
					if (tem<delta){
						delta=tem;
						loc=pointer;
					}
					if (food[n].sol[row][pointer]==0)
						break;
					pointer=food[n].sol[row][pointer];
				}
				n_1=food[n].sol[row][loc];
				food[n].sol[row][loc]=node[i];
				food[n].sol[row][node[i]]=n_1;
			}
		}
	}
	else {
		double delta_g,delta_r;food[n].fit=0;
		int opt_row;	int row_g, row_r, loc_g, loc_r;
		int longest_tour;
		for (int i=0;i<=v_num-1;i++){
			food[n].list_cost[i]=0;
			pointer=0;
			while (1){
				food[n].list_cost[i]+=Distance[pointer][food[n].sol[i][pointer]];
				pointer=food[n].sol[i][pointer];
				if (pointer==0)break;
			}
		}
		longest_tour=0;
		for (int i=1;i<=v_num-1;i++){
			if (food[n].list_cost[longest_tour]>food[n].list_cost[i])
				longest_tour=i;
		}
		for (int i=v_num+1;i<=num_v;){
			if (((rand()%100)/100.0)<P_method){
				delta_g=Max;delta_r=Max;
				for (int j=0;j<=v_num-1;j++){
					pointer=0;
					while (1){
						tem=Distance[pointer][node[i]]+Distance[node[i]][food[n].sol[j][pointer]]-Distance[pointer][food[n].sol[j][pointer]];
						if (tem<delta_g){
							delta_g=tem;
							row_g=j;
							loc_g=pointer;
						}
						if (j!=longest_tour && tem<delta_r){
							delta_r=tem;
							row_r=j;
							loc_r=pointer;
						}
						pointer=food[n].sol[j][pointer];
						if (pointer==0)break;
					}
				}
				if (row_g!=longest_tour){// case 1
					n_1=food[n].sol[row_g][loc_g];
					food[n].sol[row_g][loc_g]=node[i];
					food[n].sol[row_g][node[i]]=n_1;
					food[n].list_cost[row_g]+=delta_g;
					// update the food[n].fit
					for (int k=0;k<=v_num-1;k++){
						if (food[n].fit<food[n].list_cost[k])
							food[n].fit=food[n].list_cost[k];
					}
					i++;
				}
				if (row_g==longest_tour && (food[n].list_cost[row_r]+delta_r)>food[n].fit){// case 2
					n_1=food[n].sol[row_g][loc_g];
					food[n].sol[row_g][loc_g]=node[i];
					food[n].sol[row_g][node[i]]=n_1;
					food[n].list_cost[row_g]+=delta_g;
					// update the food[n].fit
					for (int k=0;k<=v_num-1;k++){
						if (food[n].fit<food[n].list_cost[k])
							food[n].fit=food[n].list_cost[k];
					}
					i++;
				}
				longest_tour=0;
				for (int k=0;k<=v_num-1;k++){
					if (food[n].list_cost[longest_tour]>food[n].list_cost[k])
						longest_tour=k;
				}
			}
			else {
				row=rand()%v_num;
				delta=Max;
				pointer=0;
				while(1){
					tem=Distance[pointer][node[i]]+Distance[node[i]][food[n].sol[row][pointer]]-Distance[pointer][food[n].sol[row][pointer]];
					if (tem<delta){
						delta=tem;
						loc=pointer;
					}
					if (food[n].sol[row][pointer]==0)
						break;
					pointer=food[n].sol[row][pointer];
				}
				n_1=food[n].sol[row][loc];
				food[n].sol[row][loc]=node[i];
				food[n].sol[row][node[i]]=n_1;
				food[n].list_cost[row]+=delta;
				i++;
				// update the food[n].fit
				for (int k=0;k<=v_num-1;k++){
					if (food[n].fit<food[n].list_cost[k])
						food[n].fit=food[n].list_cost[k];
				}
			}
		}
	}
	int p;
	if (obj==1){
		delta=0;// this variable is used to calculate the objective
		double delta_list;
		for (int i=0;i<=v_num-1;i++)
		{
			delta_list=0;
			p=0;
			while (1)
			{
				delta_list+=Distance[p][food[n].sol[i][p]];
				if (food[n].sol[i][p]==0)
					break;
				p=food[n].sol[i][p];
			}
			delta+=delta_list;
			food[n].list_cost[i]=delta_list;
		}
		food[n].fit=delta;
		food[n].no_impro=0;
		// reverse
	}
	vector<int >tour;
	for (int i=0;i<=v_num-1;i++)
	{
		p=food[n].sol[i][0];
		while (p!=0)
		{
			tour.push_back(p);
			p=food[n].sol[i][p];
		}
		food[n].sol_r[i][0]=tour.back();
		tour.pop_back();
		p=food[n].sol_r[i][0];
		while (tour.size())
		{
			food[n].sol_r[i][p]=tour.back();
			p=food[n].sol_r[i][p];
			tour.pop_back();
		}
		food[n].sol_r[i][p]=0;
	}
}
void initial_population(){
	// initial variables
	for (int i=0;i<=neb1+neb1*s_max-1;i++){
		food[i].sol=new int * [v_num];
		food[i].sol_r=new int * [v_num];
		food[i].list_cost=new double  [v_num];
		for (int j=0;j<=v_num-1;j++){
			food[i].sol[j]=new int [num_v+1];
			food[i].sol_r[j]=new int [num_v+1];
		}
	}
	for (int i=0;i<=neb1+neb1*s_max-1;i++){
		for (int j=0;j<=v_num-1;j++){
			for (int k=0;k<=num_v;k++){
				food[i].sol[j][k]=-1;
				food[i].sol_r[j][k]=-1;
			}
		}
	}
	best_food.sol=new int *[v_num];
	best_food.sol_r=new int *[v_num];
	new_food.sol=new int *[v_num];
	new_food.sol_r=new int *[v_num];
	new_food.list_cost=new double [v_num];
	for (int i=0;i<=v_num-1;i++){
		best_food.sol[i]=new int [num_v+1];
		best_food.sol_r[i]=new int [num_v+1];
		new_food.sol[i]=new int [num_v+1];
		new_food.sol_r[i]=new int [num_v+1];
	}
	for (int i=0;i<=v_num-1;i++){
		for (int j=0;j<=num_v;j++){
			best_food.sol[i][j]=-1;
			best_food.sol_r[i][j]=-1;
			new_food.sol[i][j]=-1;
			new_food.sol_r[i][j]=-1;
		}
	}
	for (int i=0;i<=v_num-1;i++)
		for (int j=0;j<=num_v;j++){
			best_food.sol[i][j]=-1;
		}
	best_food.fit=Max;
	// constructing initial solutions for all individuals
	for (int i=0;i<=neb-1;i++){
		construct(i);
	}
}
double determin_Pcp(int n){
	double Pcp;
	Pcp=((Pmax-Pmin)/iter_max)*step+Pmin;
/* 	if (step<iter_max)
		Pcp=((Pmax-Pmin)/iter_max)*step+Pmin;
	else
		Pcp=Pmax; */
	return Pcp;
}
void determine_best(double best_fit)// the function of this fun is to update the best solution from population.
{
	int index=-1;
	for (int i=0;i<=count_sol-1;i++){
		if (food[i].fit<best_fit){
			best_fit=food[i].fit;
			index=i;
		}
	}
	if (index!=-1){
		best_food.fit=food[index].fit;
		for (int i=0;i<=v_num-1;i++)
			for (int j=0;j<=num_v;j++)
				best_food.sol[i][j]=food[index].sol[i][j];
		optimal_time=ceil((clock() - start)/static_cast<double>(CLOCKS_PER_SEC));
	}
	if (count_sol>n_max){
		vector< vector<double> >sequence(count_sol);
		for (int i=0;i<=count_sol-1;i++){
			sequence[i].push_back(food[i].fit);
			sequence[i].push_back(i);
		}
		sort(sequence.begin(),sequence.end());
/* 	for (int i=0;i<=count_sol-1;i++){
		cout<<sequence[i][0]<<"         "<<sequence[i][1]<<endl;
	} */
		int check_in[n_max];
		for (int i=0;i<=n_max-1;i++){
			check_in[i]=0;
		}
		for (int i=0;i<=n_max-1;i++){
			if(int(sequence[i][1])<(n_max-1))
				check_in[int(sequence[i][1])]=1;
		}
//		cout<<sequence[0][1]<<endl;
		for (int i=0;i<=n_max-1;i++){
			if (int(sequence[i][1])>=n_max){
				for (int j=0;j<=n_max-1;j++){
					if (check_in[j]==0){
						check_in[j]=1;//cout<<int(sequence[i][1])<<endl;
						for(int k=0;k<=v_num-1;k++){
							for(int m=0;m<=num_v;m++){
								food[j].sol[k][m]=food[int(sequence[i][1])].sol[k][m];
								food[j].sol_r[k][m]=food[int(sequence[i][1])].sol_r[k][m];
							}
							food[j].list_cost[k]=food[int(sequence[i][1])].list_cost[k];
						}
						food[j].fit=food[int(sequence[i][1])].fit;
					}
				}
			}
		}
		count_sol=n_max;
	}

}
void extract_nodes(vector<int> &unassign,int n)
{
	for (int i=0;i<=v_num-1;i++)
		for (int j=0; j<=num_v;j++){
			new_food.sol[i][j]=food[n].sol[i][j];
			new_food.sol_r[i][j]=food[n].sol_r[i][j];
		}
	// delete nodes from new_food.       attention a case which all nodes are deleted.
	int p;int p_n;int p_p;int pp;
	double Pcp=determin_Pcp(n);
	for (int i=0;i<=v_num-1;i++){
		p=new_food.sol[i][0];
		while(1){
			if (p!=0){
				if ((rand()%10000)>=(ceil(10000*Pcp))){//this is an important probability determine by a function
					pp=new_food.sol[i][p];
					unassign.push_back(p);
					p_n=new_food.sol[i][p];
					p_p=new_food.sol_r[i][p];
					new_food.sol[i][p_p]=p_n;
					new_food.sol[i][p]=-1;
					new_food.sol_r[i][p_n]=p_p;
					new_food.sol_r[i][p]=-1;
					p=pp;
				}
				else
					p=new_food.sol[i][p];
			}
			else
				break;
		}
	}
	//cout<<unassign.size()-1<<endl;
}

void insert_fun(vector<int> & node){// inserting all nodes in unassign to food.
	double delta;int row,loc;int pointer;double tem;
	int length=node.size();int n_1;
	int count=0;
	for (int i=0;i<=v_num-1;i++){
		if (new_food.sol[i][0]==0){
			new_food.sol[i][0]=node[count];
			new_food.sol[i][node[count]]=0;
			count++;
		}
	}
	if (obj==1){
		for (int i=count;i<=length-1;i++){
			if (((rand()%100)/100.0)<P_method){// method 1
				delta=Max;
				for (int j=0;j<=v_num-1;j++){
					pointer=0;
					while(1){
						tem=Distance[pointer][node[i]]+Distance[node[i]][new_food.sol[j][pointer]]-Distance[pointer][new_food.sol[j][pointer]];
						if (tem<delta){
							delta=tem;
							loc=pointer;
							row=j;
						}
						if (new_food.sol[j][pointer]==0)
							break;
						pointer=new_food.sol[j][pointer];
					}
				}
				n_1=new_food.sol[row][loc];
				new_food.sol[row][loc]=node[i];
				new_food.sol[row][node[i]]=n_1;
			}
			else {
				row=rand()%v_num;
				delta=Max;
				pointer=0;
				while(1){
					tem=Distance[pointer][node[i]]+Distance[node[i]][new_food.sol[row][pointer]]-Distance[pointer][new_food.sol[row][pointer]];
					if (tem<delta){
						delta=tem;
						loc=pointer;
					}
					if (new_food.sol[row][pointer]==0)
						break;
					pointer=new_food.sol[row][pointer];
				}
				n_1=new_food.sol[row][loc];
				new_food.sol[row][loc]=node[i];
				new_food.sol[row][node[i]]=n_1;
			}
		}
	}
	else {
		double delta_g,delta_r;new_food.fit=0;
		int opt_row;int row_g,row_r,loc_g,loc_r;int longest_tour;
		for (int i=0;i<=v_num-1;i++){
			new_food.list_cost[i]=0;
			pointer=0;
			while (1){
				new_food.list_cost[i]+=Distance[pointer][new_food.sol[i][pointer]];
				pointer=new_food.sol[i][pointer];
				if (pointer==0)break;
			}
		}
		longest_tour=0;
		for (int i=1;i<=v_num-1;i++){
			if (new_food.list_cost[longest_tour]>new_food.list_cost[i])
				longest_tour=i;
		}
		for (int i=count;i<=length-1;){
			if (((rand()%100)/100.0)<P_method){
				delta_g=Max;delta_r=Max;
				for (int j=0;j<=v_num-1;j++){
					pointer=0;
					while (1){
						//cout<<node[0]<<endl;
						tem=Distance[pointer][node[i]]+Distance[node[i]][new_food.sol[j][pointer]]-Distance[pointer][new_food.sol[j][pointer]];
						if (tem<delta_g){
							delta_g=tem;
							row_g=j;
							loc_g=pointer;
						}
						if (j!=longest_tour && tem<delta_r){
							delta_r=tem;
							row_r=j;
							loc_r=pointer;
						}
						pointer=new_food.sol[j][pointer];
						if (pointer==0)break;
					}
				}
				if (row_g!=longest_tour){// case 1
					n_1=new_food.sol[row_g][loc_g];
					new_food.sol[row_g][loc_g]=node[i];
					new_food.sol[row_g][node[i]]=n_1;
					new_food.list_cost[row_g]+=delta_g;
					// update the food[n].fit
					for (int k=0;k<=v_num-1;k++){
						if (new_food.fit<new_food.list_cost[k])
							new_food.fit=new_food.list_cost[k];
					}
					i++;
				}
				if (row_g==longest_tour && (new_food.list_cost[row_r]+delta_r)>new_food.fit){// case 2
					n_1=new_food.sol[row_g][loc_g];
					new_food.sol[row_g][loc_g]=node[i];
					new_food.sol[row_g][node[i]]=n_1;
					new_food.list_cost[row_g]+=delta_g;
					// update the food[n].fit
					for (int k=0;k<=v_num-1;k++){
						if (new_food.fit<new_food.list_cost[k])
							new_food.fit=new_food.list_cost[k];
					}
					i++;
				}
				longest_tour=0;
				for (int k=0;k<=v_num-1;k++){
					if (new_food.list_cost[longest_tour]>new_food.list_cost[k])
						longest_tour=k;
				}
			}
			else {
				row=rand()%v_num;
				delta=Max;
				pointer=0;
				while(1){
					tem=Distance[pointer][node[i]]+Distance[node[i]][new_food.sol[row][pointer]]-Distance[pointer][new_food.sol[row][pointer]];
					if (tem<delta){
						delta=tem;
						loc=pointer;
					}
					if (new_food.sol[row][pointer]==0)
						break;
					pointer=new_food.sol[row][pointer];
				}
				n_1=new_food.sol[row][loc];
				new_food.sol[row][loc]=node[i];
				new_food.sol[row][node[i]]=n_1;
				new_food.list_cost[row]+=delta;
				i++;
				// update the food[n].fit
				for (int k=0;k<=v_num-1;k++){
					if (new_food.fit<new_food.list_cost[k])
						new_food.fit=new_food.list_cost[k];
				}
			}
		}
	}
	int p;
	if (obj==1){
		delta=0;// this variable is used to calculate the objective
		double delta_list;
		for (int i=0;i<=v_num-1;i++)	{
			delta_list=0;
			p=0;
			while (1){
				delta_list+=Distance[p][new_food.sol[i][p]];
				if (new_food.sol[i][p]==0)
					break;
				p=new_food.sol[i][p];
			}
			delta+=delta_list;
			new_food.list_cost[i]=delta_list;
		}
		new_food.fit=delta;
		new_food.no_impro=0;
		// reverse
	}
	if (length==0 && obj==2){
		new_food.fit=0;
		for (int i=0;i<=v_num-1;i++){
			new_food.list_cost[i]=0;
			pointer=0;
			while (1){
				new_food.list_cost[i]+=Distance[pointer][new_food.sol[i][pointer]];
				pointer=new_food.sol[i][pointer];
				if (pointer==0)break;
			}
		}
		for (int i=0;i<=v_num-1;i++){
			if (new_food.fit<new_food.list_cost[i])
				new_food.fit=new_food.list_cost[i];
		}
	}
	for (int i=0;i<=v_num-1;i++)
		for (int j=0;j<=num_v;j++)
			new_food.sol_r[i][j]=-1;
	vector<int >tour;
	for (int i=0;i<=v_num-1;i++){
		p=new_food.sol[i][0];
		while (p!=0){
			tour.push_back(p);
			p=new_food.sol[i][p];
		}
		new_food.sol_r[i][0]=tour.back();
		tour.pop_back();
		p=new_food.sol_r[i][0];
		while (tour.size()){
			new_food.sol_r[i][p]=tour.back();
			p=new_food.sol_r[i][p];
			tour.pop_back();
		}
		new_food.sol_r[i][p]=0;
	}
}

void neighbor(int n,int num_nei){//this function is used to construct a new solution for food[n] and update it
	for (int i=0;i<=num_nei-1;i++){
		vector<int>unassign;// this vector is used to keep all nodes deleted from new_food
		extract_nodes(unassign,n);
		random_shuffle(unassign.begin(), unassign.end());// random all nodes in unassign
		// insert all nodes.
		insert_fun(unassign);
		for (int i=0;i<=v_num-1;i++){
			for (int j=0;j<=num_v;j++){
				food[count_sol].sol[i][j]=new_food.sol[i][j];
				food[count_sol].sol_r[i][j]=new_food.sol_r[i][j];
			}
			food[count_sol].list_cost[i]=new_food.list_cost[i];
		}
		food[count_sol].fit=new_food.fit;
		count_sol++;
	}
}
int determine_neighborhood(int index){
	int number;
	for (int i=1;i<=(s_max-s_min);i++){
		if (i==1 && index<=((neb*i)/(s_max-s_min+1))){
			number=s_max;break;
		}
		if (i>1 && index<=((neb*i)/(s_max-s_min+1)) && index>(((neb*(i-1))/(s_max-s_min+1)))){
			number=s_max-(i-1);break;
		}
		if (i==(s_max-s_min) && index>=((neb*i)/(s_max-s_min+1)))
			number=s_min;
	}
	return number;
}
void ABC(){
	int p;step=1;
	count_sol=neb;
	determine_best(best_food.fit);// update best solution from initial population
	int index;int num_nei;
	while(1){
		vector< vector<double> >sequence(neb);
		for (int i=0;i<=neb-1;i++){
			sequence[i].push_back(food[i].fit);
			sequence[i].push_back(i);
		}
		sort(sequence.begin(),sequence.end());
		for (int i=0;i<=neb-1;i++)	{
			for (int j=0;j<=neb-1;j++)
				if (int(sequence[j][1])==i)
					index=j;
			num_nei=determine_neighborhood(index);
			neighbor(i,num_nei);
		}
		determine_best(best_food.fit);
//		cout<<step<<"    "<<best_food.fit<<endl;
		step++;
	//	if(ceil((clock() - start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
		if(ceil((clock() - start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length || step>iter_max)
			break;
		cout<<step<<"    "<<best_food.fit<<endl;
	}
	cout<<"running time is "<<ceil((clock() - start)/static_cast<double>(CLOCKS_PER_SEC))<<endl;
}

int main(int argc, char*argv[]){
	if (argc!=5){
		cout << "the input parameters in configuration are wrong" << endl;
		return 1;
	}
	sscanf(argv[2],"%d",&num_v);
	sscanf(argv[3],"%d",&time_length);
	sscanf(argv[4],"%d",&seed);
	if(build_instance(argv[1])!=1){
		cout<<"can't open instance file"<<endl;
		return -1;
	}
	srand(seed);// generate a random number;
	start=clock();
	if (obj==1){
		P_method=1;
		Pmin=0;
		Pmax=1;
		neb=100;
	}
	else{
		P_method=0.95;
		Pmin=0.2;
		Pmax=0.8;
		neb=50;
	}
	initial_population();
	ABC();
	output_function();
	release_memory();
	return 0;
}
