/*
 * perturbationfun.cpp
 *
 *  Created on: 19 Apr 2020
 *      Author: Peng
 */
#include "basic.h"
#include <math.h>
#include <iostream>
#include <vector>
#include<algorithm>
#include<stdlib.h>

#include "cross_ctsp.h"
using namespace std;
cross_ctsp::cross_ctsp(read_data * data) {
	num_v=data->num_v;
//	num_s=data->num_s;
	v_num=data->v_num;
	I_data=data;
	s_t_fa=new int [num_v+1];
	s_t_ma=new int [num_v+1];
	s_t_off=new int [num_v+1];
	father=new int *[v_num];
	mother=new int *[v_num];
	father_r=new int *[v_num];
	mother_r=new int *[v_num];
	for (int i=0;i<=v_num-1;i++){
		father[i]=new int [num_v+1];
		mother[i]=new int [num_v+1];
		father_r[i]=new int [num_v+1];
		mother_r[i]=new int [num_v+1];
	}
}
cross_ctsp::~cross_ctsp() {
	delete [] s_t_fa;
	delete [] s_t_ma;
	delete [] s_t_off;
	for (int i=0;i<=v_num-1;i++){
		delete [] father[i];
		delete [] mother[i];
		delete [] father_r[i];
		delete [] mother_r[i];
	}
	delete [] father;
	delete [] mother;
	delete [] father_r;
	delete [] mother_r;
}

void cross_ctsp::mTour(){// the new crossover
	int check_in[num_v+1];
	int pointer;
	for (int i=0;i<=v_num-1;i++){
		for (int j=0;j<=num_v;j++){
			father[i][j]=fa->sol[i][j];
			mother[i][j]=ma->sol[i][j];
			father_r[i][j]=fa->sol_r[i][j];
			mother_r[i][j]=ma->sol_r[i][j];
			off->sol[i][j]=-1;
		}
		off->sol[i][0]=0;
	}
	for (int i=0;i<=num_v;i++){
		s_t_fa[i]=-1;
		s_t_ma[i]=-1;
	}
	for (int i=0;i<=v_num-1;i++){
		pointer=father[i][0];
		while (pointer!=0){
			s_t_fa[pointer]=i;
			pointer=father[i][pointer];
		}
	}
	for (int i=0;i<=v_num-1;i++){
		pointer=mother[i][0];
		while (pointer!=0){
			s_t_ma[pointer]=i;
			pointer=mother[i][pointer];
		}
	}
	vector<vector<double> >orderF(v_num);
	vector<vector<double> >orderM(v_num);
	for (int i=0;i<=v_num-1;i++){
		orderF[i].push_back(fa->fit_tour[i]);
		orderF[i].push_back(i);
		orderM[i].push_back(ma->fit_tour[i]);
		orderM[i].push_back(i);
	}
	sort(orderF.begin(),orderF.end());
	sort(orderM.begin(),orderM.end());
	int p_next;int p_previous;
	for (int i=0;i<=v_num-1;i++){
		if (rand()%2==1){// father
			for (int j=0;j<=num_v;j++)
				off->sol[i][j]=father[i][j];
			pointer=father[i][0];if (pointer==0)continue;
			while (1){
				if (s_t_ma[pointer]!=i){// the city "pointer" should be deleted from route s_t_ma[pointer] of ma
					p_next=mother[s_t_ma[pointer]][pointer];
					p_previous=mother_r[s_t_ma[pointer]][pointer];
					mother[s_t_ma[pointer]][p_previous]=p_next;
					mother_r[s_t_ma[pointer]][p_next]=p_previous;
					mother[s_t_ma[pointer]][pointer]=-1;
					mother_r[s_t_ma[pointer]][pointer]=-1;
				}
				pointer=father[i][pointer];
				if (pointer==0)break;
			}
		}
		else{// mother
			for (int j=0;j<=num_v;j++)
				off->sol[i][j]=mother[i][j];
			pointer=mother[i][0];if (pointer==0)continue;
			while (1){
				if (s_t_fa[pointer]!=i){// the city "pointer" should be deleted from route s_t_ma[pointer] of ma
					p_next=father[s_t_fa[pointer]][pointer];
					p_previous=father_r[s_t_fa[pointer]][pointer];
					father[s_t_fa[pointer]][p_previous]=p_next;
					father_r[s_t_fa[pointer]][p_next]=p_previous;
					father[s_t_fa[pointer]][pointer]=-1;
					father_r[s_t_fa[pointer]][pointer]=-1;
				}
				pointer=mother[i][pointer];
				if (pointer==0)
					break;
			}

		}
	}
	// enters the stage two. inserting all unassigning cities
	for (int i=0;i<=num_v;i++)
		check_in[i]=0;
	for (int i=0;i<=v_num-1;i++){
		pointer=off->sol[i][0];
		while(pointer!=0){
			check_in[pointer]=1;
			pointer=off->sol[i][pointer];
		}
	}
	for (int i=0;i<=v_num-1;i++){
		off->fit_tour[i]=0;
		pointer=0;
		while (1){
			off->fit_tour[i]+=I_data->Distance[pointer][off->sol[i][pointer]];
			pointer=off->sol[i][pointer];
			if (pointer==0)break;
		}
	}
	off->fitness=0;
	int best_index;
	for (int i=0;i<=v_num-1;i++)
		if (off->fitness<off->fit_tour[i]){
			off->fitness=off->fit_tour[i];
			best_index=i;
		}
	double delta_l,delta_b;double delta;int row_l,pos_l,row_b,pos_b,tem;
	for (int i=1;i<=num_v;i++){
		if (check_in[i]==0){
			delta_l=Max;delta_b=Max;
			for (int j=0;j<=v_num-1;j++){
				pointer=0;
				while (1){
					p_next=off->sol[j][pointer];
					delta=I_data->Distance[pointer][i]+I_data->Distance[i][p_next]-I_data->Distance[pointer][p_next];
					if (j!=best_index){
						if (delta<delta_l){
							delta_l=delta;
							row_l=j;
							pos_l=pointer;
						}
						if (delta_b>delta){
							delta_b=delta;
							row_b=j;
							pos_b=pointer;
						}
					}
					else{
						if (delta_b>delta){
							delta_b=delta;
							row_b=j;
							pos_b=pointer;
						}
					}
					pointer=p_next;
					if(pointer==0)
						break;
				}
			}
			if (row_b==best_index && off->fit_tour[row_l]+delta_l > off->fitness){
				tem=off->sol[row_b][pos_b];
				off->sol[row_b][pos_b]=i;
				off->sol[row_b][i]=tem;
				off->fit_tour[row_b]+=delta_b;
				off->fitness=off->fit_tour[row_b];
			}
			else{
				tem=off->sol[row_l][pos_l];
				off->sol[row_l][pos_l]=i;
				off->sol[row_l][i]=tem;
				off->fit_tour[row_l]+=delta_l;
				if (off->fit_tour[row_l]>off->fitness){
					off->fitness=off->fit_tour[row_l];
					best_index=row_l;
				}
			}
		}
	}
	repair();
}
double cross_ctsp::determine_prob(int iter){
	double prob=(P_max-P_min)*iter/iter_Max + P_min;
	return prob;
}
void cross_ctsp::mutation(int iter){
			for (int i=0;i<=v_num-1;i++)
				for (int j=0;j<=num_v;j++){
					father[i][j]=fa->sol[i][j];
					father_r[i][j]=fa->sol_r[i][j];
				}



	int check_in[num_v+1];
	for (int i=0;i<=num_v;i++)
		check_in[i]=1;
	double P_copy=determine_prob(iter);
	int pointer;int rand_num;int off_pointer;
	for (int i=0;i<=v_num-1;i++){
		pointer=father[i][0];
		off_pointer=0;
		while (1){
			rand_num=rand()%1000;
			if (rand_num<P_copy*1000){
				off->sol[i][off_pointer]=pointer;
				off_pointer=off->sol[i][off_pointer];
			}
			else{
				check_in[pointer]=0;
			}
			pointer=father[i][pointer];
			if (pointer==0)break;
		}
		off->sol[i][off_pointer]=0;
	}
	off->fitness=0;
	int best_index;
	for (int i=0;i<=v_num-1;i++)
		if (off->fitness<off->fit_tour[i]){
			off->fitness=off->fit_tour[i];
			best_index=i;
		}

	double delta_l,delta_b;double delta;int row_l,pos_l,row_b,pos_b,tem;int p_next;
	for (int i=1;i<=num_v;i++){
		if (check_in[i]==0){
			delta_l=Max;delta_b=Max;
			for (int j=0;j<=v_num-1;j++){
				pointer=0;
				while (1){
					p_next=off->sol[j][pointer];
					delta=I_data->Distance[pointer][i]+I_data->Distance[i][p_next]-I_data->Distance[pointer][p_next];
					if (j!=best_index){
						if (delta<delta_l){
							delta_l=delta;
							row_l=j;
							pos_l=pointer;
						}
						if (delta_b>delta){
							delta_b=delta;
							row_b=j;
							pos_b=pointer;
						}
					}
					else{
						if (delta_b>delta){
							delta_b=delta;
							row_b=j;
							pos_b=pointer;
						}
					}
					pointer=p_next;
					if(pointer==0)
						break;
				}
			}
			if (row_b==best_index && off->fit_tour[row_l]+delta_l > off->fitness){
				tem=off->sol[row_b][pos_b];
				off->sol[row_b][pos_b]=i;
				off->sol[row_b][i]=tem;
				off->fit_tour[row_b]+=delta_b;
				off->fitness=off->fit_tour[row_b];
			}
			else{
				tem=off->sol[row_l][pos_l];
				off->sol[row_l][pos_l]=i;
				off->sol[row_l][i]=tem;
				off->fit_tour[row_l]+=delta_l;
				if (off->fit_tour[row_l]>off->fitness){
					off->fitness=off->fit_tour[row_l];
					best_index=row_l;
				}
			}
		}
	}
	repair();
}

void cross_ctsp::repair(){
	int row,pointer;int p_next;
	int count=0;
	for (int i=0;i<=v_num-1;i++){
		pointer=0;
		while (1){
			pointer=off->sol[i][pointer];
			if (pointer>0)
				count++;
			if (pointer==0)break;
		}
	}
	if (count!=num_v){
		cout<<"Here is a problem "<<endl;
		exit(0);
	}
	for (int i=0;i<=v_num-1;i++){
		if (off->sol[i][0]<0 || off->sol[i][0]>num_v ){
			cout<<"Here is a abnormal operation"<<endl;
			exit(0);
		}
		if (off->sol[i][0]==0){// select a city from other route to insert into this route.
			while (1){
				row=rand()%v_num;
				if (row==i)continue;
				pointer=off->sol[row][0];
				if (off->sol[row][pointer]!=0 && row!=i)
					break;
			}
			pointer=off->sol[row][0];
			p_next=off->sol[row][pointer];
			off->sol[i][0]=pointer;
			off->sol[i][pointer]=0;
			off->sol[row][0]=p_next;
			off->sol[row][pointer]=-1;
		}
	}
	off->fitness=0;
	for (int i=0;i<=v_num-1;i++){
		pointer=0;off->fit_tour[i]=0;
		while (1){
			off->fit_tour[i]+=I_data->Distance[pointer][off->sol[i][pointer]];
			pointer=off->sol[i][pointer];
			if (pointer==0)break;
		}
		if (off->fit_tour[i] > off->fitness){
			off->fitness=off->fit_tour[i];
		}
	}
}



