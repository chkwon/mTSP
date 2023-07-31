/*
 * readdata.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#include "read_data.h"
#include "basic.h"
#include "Individual.h"
#include <sstream>
read_data::read_data() {

}
read_data::~read_data() {
	for (i=0;i<=num_v;i++){
		delete [] Distance[i];
		delete [] edge[i];
		delete [] record[i];
		delete [] beta[i];
	}
	delete [] Distance;
	delete [] edge;
	delete [] beta;
	delete [] record;
	for (i=0;i<=num_v;i++)
		delete [] near_ns[i];
	delete [] near_ns;
	for ( i=0;i<=num_v;i++)
		delete [] near[i];
	delete [] near;
	for (i=0;i<=length_s-1;i++)
		delete [] use_frequency[i];
	delete [] use_frequency;
	delete [] x;
	delete [] y;
	for (int i=0;i<=num_v;i++)
		delete [] city[i].son;
	delete [] city;
	delete [] low_cost;
	delete [] check_city;
}
void read_data::define(){
	Distance=new double *[num_v+1];
	edge=new int *[num_v+1];
	record=new int *[num_v+1];
	beta=new double *[num_v+1];
	for ( i=0;i<=num_v;i++){
		Distance[i]=new double [num_v+1];
		edge[i]=new int [num_v+1];
		record[i]=new int [num_v+1];
		beta[i]=new double [num_v+1];
	}
	x=new double [num_v+1];
	y=new double [num_v+1];
	near_ns=new int *[num_v+1];
	near= new int *[num_v+1];
	for ( i=0;i<=num_v;i++)
		near[i]=new int	[num_near_nodes];
	for ( i=0;i<=num_v;i++)
		near_ns[i]=new int [num_v+1];
	use_frequency=new int *[length_s];
	for (i=0;i<=length_s-1;i++)
		use_frequency[i]=new int [length_s];
	for (i=0;i<=length_s-1;i++)
		for (j=0;j<=length_s-1;j++)
			use_frequency[i][j]=0;
	/////
	city=new node[num_v+1];
	for (int i=0;i<=num_v;i++)
		city[i].son=new int [num_v+1];
	low_cost=new double [num_v+1];
	check_city=new int [num_v+1];
}
void read_data::read_fun(char* file_name,int number_node){
//	instance_n=file_name;
	ifstream open_file(file_name);
	num_v=number_node;
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
	// determine nearest nodes
	double min_dis;int check_in[num_v+1];int cityNum;
	for ( i=1;i<=num_v;i++){
		for ( j=0;j<=num_v;j++)
			check_in[j]=0;
		check_in[i]=1;
		near[i][0]=i;// the nearest city is itself.
		for (j=1;j<=num_near_nodes-1;j++){
			min_dis=Max;
			for (k=0;k<=num_v;k++){
				if (Distance[i][k]<=min_dis && check_in[k]==0){
					cityNum=k;
					min_dis=Distance[i][k];
				}
			}
			near[i][j]=cityNum;
			check_in[cityNum]=1;
		}
	}
	for ( i=0;i<=num_v;i++)
		for ( j=0;j<=num_v;j++)
			near_ns[i][j]=0;
	for ( i=1;i<=num_v;i++){
		for ( j=1;j<=num_near_nodes-1;j++)
			near_ns[i][near[i][j]]=1;
	}
	// construct sparse graph
	spanning_tree();
	alpha_sparse_graph();
}

void read_data::spanning_tree(){
	//miniming spanning tree
	for (int i=0;i<=num_v;i++){
		city[i].order=i;
		city[i].father=-1;
		city[i].num_son=0;
		for (int j=0;j<=num_v;j++)
			city[i].son[i]=-1;
		check_city[i]=0;
	}
	// generate minimum spanning tree;
//	double low_cost[num_v];
	int v;int pre_v;
	check_city[1]=1;
	pre_v=1;
	for (int i=2;i<=num_v;i++)
		low_cost[i]=Distance[1][i];
	double min_dis;
	for (int i=2;i<=num_v;i++){
		min_dis=Max;
		v=-1;
		for (int j=1;j<=num_v;j++){
			if (check_city[j]==0 && min_dis>low_cost[j]){
				v=j;
				min_dis=low_cost[j];
			}
		}
		if (v!=-1){
			for (int k=1;k<=num_v;k++)
				if (min_dis+1.0e-7>Distance[k][v] && min_dis-1.0e-7<Distance[k][v] && check_city[k]==1)
					pre_v=k;
			check_city[v]=1;
			city[v].father=pre_v;
			city[pre_v].son[city[pre_v].num_son]=v;
			city[pre_v].num_son++;
			//
			for (int k=1;k<=num_v;k++){
				if (check_city[k]==0 && Distance[k][v]<low_cost[k])
					low_cost[k]=Distance[k][v];
			}
		}
	}
	// visit the spanning tree;
	for (int i=0;i<=num_v;i++)
		for (int j=0;j<=num_v;j++)
			edge[i][j]=0;
	visit_tree(1);
}
void read_data::visit_tree(int root){
	if (city[root].num_son!=0){
		for (int i=0;i<=city[root].num_son-1;i++){
			edge[city[root].order][city[root].son[i]]=1;
			edge[city[root].son[i]][city[root].order]=1;
		//	cout<<"edge "<<city[root].order<<","<<city[root].son[i]<<endl;
			visit_tree(city[root].son[i]);
		}
	}
}
void read_data::post_travel_tree(int root){
	if (city[root].num_son!=0){
		for (int i=0;i<=city[root].num_son-1;i++){
			key=city[root].son[i];
			visit_tree_beta(1);
			post_travel_tree(city[root].son[i]);
		}
	}
}
void read_data::visit_tree_beta(int root){
	if (city[root].num_son!=0){
		for (int i=0;i<=city[root].num_son-1;i++){
			if (key!=city[root].son[i] && edge[key][city[root].son[i]]!=1 && beta[key][city[root].son[i]]<0){
				beta[key][city[root].son[i]]=get_max(beta[key][city[root].order],Distance[city[root].son[i]][city[root].order]);
				beta[city[root].son[i]][key]=beta[key][city[root].son[i]];
			}
			if (edge[key][city[root].son[i]]==1 && key!=city[root].son[i] ){
				beta[key][city[root].son[i]]=Distance[city[root].son[i]][key];
				beta[city[root].son[i]][key]=beta[key][city[root].son[i]];
			}
			visit_tree_beta(city[root].son[i]);
		}
	}
}
double read_data:: get_max(double a,double b){
	if (a>b)return a;
	else return b;
}
void read_data::alpha_sparse_graph(){
	int city_1=1;
	int city_2=2;
	double min_edge=Distance[0][city_1];
	double sub_min_edge=Distance[0][city_2];
	for (int i=3;i<=num_v;i++){
		if (Distance[0][i]<min_edge){
			min_edge=Distance[0][i];
			city_1=i;
		}
		if (Distance[0][i]>min_edge && Distance[0][i]<sub_min_edge){
			sub_min_edge=Distance[0][i];
			city_2=i;
		}
	}
	city[0].son[0]=city_1;
	city[0].son[1]=city_2;
	edge[0][city_1]=1;
	edge[0][city_2]=1;
	edge[city_1][0]=1;
	edge[city_2][0]=1;
	///////////////////////////////////////////////////////////////////////////////////////
	//compute alpha for each node and insert it into graph edge
	for (int i=0;i<=num_v;i++)
		for (int j=0;j<=num_v;j++)
			beta[i][j]=-Max;
	// min_edge,sub_min_edge;
	for (int i=1;i<=num_v;i++){
		if (i==city[0].son[0] || i== city[0].son[1])
			continue;
		beta[0][i]=Distance[0][i]-Distance[0][city[0].son[1]];
		beta[i][0]=beta[0][i];
	}
	// compute for root
	key=1;
	visit_tree_beta(1);
	for (int i=0;i<=num_v;i++)
		for (int j=0;j<=num_v;j++)
			if (edge[i][j]==1)
				beta[i][j]=Distance[i][j];
	// compute for leaf
	post_travel_tree(1);
	// generate sparse graph
	for (int i=1;i<=num_v;i++)
		for (int j=1;j<=num_v;j++)
			beta[i][j]=Distance[i][j]-beta[i][j];
	// in this place, beta is matrix alpha
	// sort all value in beta and construct sparse graph
	for (int i=0;i<=num_v;i++)
		for (int j=0;j<=num_v;j++)
			record[i][j]=j;
	for (int i=0;i<=num_v;i++){
		Qsort(i,0,num_v);
	}
	for (int i=0;i<=num_v;i++)
		for (int j=1;j<=alpha;j++){
			edge[i][record[i][j]]=1;
			edge[record[i][j]][i]=1;
		}
}
void read_data::Qsort(int i,int low, int high){
	if (high<=low)return;
	int m=low;
	int j=high+1;
	int k=beta[i][low];
	while (true){
		while(beta[i][++m]<k)
			if (m==high)
				break;
		while (beta[i][--j]>k)
			if (j==low)
				break;
		if (m>=j)
			break;
		double temp=beta[i][m];
		int tem=record[i][m];
		beta[i][m]=beta[i][j];
		beta[i][j]=temp;
		record[i][m]=record[i][j];
		record[i][j]=tem;
	}
	double temp=beta[i][low];
	int tem=record[i][low];
	beta[i][low]=beta[i][j];
	record[i][low]=record[i][j];
	beta[i][j]=temp;
	record[i][j]=tem;
	Qsort(i,low,j-1);
	Qsort(i,j+1,high);
}
/////////////////////////////////////////////////////////////////////////////////////////////////



































void read_data::output_fun(Individual *g_best_p ,int seed){// this function is used to
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
	if (intra_opt==3){
		out<<"the using frequency of each operators in EAX is following"<<endl;
		double sum=0;
		for (i=0;i<=length_s-1;i++)
			for (j=0;j<=length_s-1;j++)
				sum+=use_frequency[i][j];
		double sum_line1=0;
		for (i=0;i<=length_s-1;i++)
			sum_line1+=use_frequency[0][i];
		out<<"the percentage of or-insert is "<< sum_line1/sum<<endl;
		out<<"the percentage of or-swap is "<< 1-sum_line1/sum<<endl;
		out<<"all percentage of each operator is following "<<endl;
		for (i=0;i<=length_s-1;i++){
			for (j=0;j<=length_s-1;j++)
				out<< double(use_frequency[i][j])/sum <<"  ";
			out<<endl;
		}
	}
	out<<endl<<endl;
}


