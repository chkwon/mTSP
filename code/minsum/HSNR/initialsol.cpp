/*
 * initialsol.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Pengfei He
 */
#include "basic.h"
#include "initialsol.h"
initial_sol::initial_sol() {
}

initial_sol::~initial_sol() {
	// TODO Auto-generated destructor stub
}
void initial_sol::greedy_fun(){
	for (int i=0;i<=I_data->v_num-1;i++){
		for (int j=0;j<=I_data->num_v;j++)
			s->sol[i][j]=-1;
		s->sol[i][0]=0;
	}
	vector <  int > node;
	for (int i=1;i<=I_data->num_v;i++)
		node.push_back(i);
	random_shuffle(node.begin(), node.end());
	for (int i=0;i<=I_data->v_num-1;i++){
		s->sol[i][0]=node[i];
		s->sol[i][node[i]]=0;
	}
	int pointer,tem;
	int row,pos;double delta,delta_tem;
	int size_node=node.size();
	for (int i=I_data->v_num;i<=size_node-1;i++){
		delta=Max;
		int j=0;
//		for (int j=0;j<=I_data->v_num-1;j++){
			pointer=0;
			while(1){
				delta_tem=I_data->Distance[node[i]][pointer]+I_data->Distance[node[i]][s->sol[j][pointer]]-
						I_data->Distance[pointer][s->sol[j][pointer]];
				if (delta>delta_tem){
					row=j;
					pos=pointer;
					delta=delta_tem;
				}
				pointer=s->sol[j][pointer];
				if (pointer==0)break;
			}
//		}
		tem=s->sol[row][pos];
		s->sol[row][pos]=node[i];
		s->sol[row][node[i]]=tem;
	}
	s->fitness=0;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		while (1){
			s->fitness+=I_data->Distance[s->sol[i][pointer]][pointer];
	//		cout<<pointer<<"  ";
			pointer=s->sol[i][pointer];
			if (pointer==0)
				break;
		}
//		cout<<endl;
	}
//	cout<<endl;





//	cout<<"the actuall cost is "<<s->fitness<<endl;

	// check the solution
	int count=0;
	for (int i=0;i<=I_data->v_num-1;i++){
		pointer=0;
		while(1){
			pointer=s->sol[i][pointer];
			if (pointer>0)count++;
			else break;
		}
	}
	if (!(count==I_data->num_v)){
		cout<<"initial solution is wrong"<<endl;
		exit(0);
	}
}
