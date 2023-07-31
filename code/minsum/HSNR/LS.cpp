/*
 * LS.cpp
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#include "LS.h"
#include "basic.h"
#include "Tstwoopt.h"
#include "EAX.h"
#include "environment.h"
#include "Individual.h"
LS::LS(read_data * data) {
	num_v=data->num_v;
	v_num=data->v_num;
	tem=new Individual();
	tem->define(num_v,v_num);
	each_route=new Tstwoopt (data);
	each_route->I_data=data;
	record_table=new int [v_num];
	s_t=new int [num_v+1];
	cand_str1=new int *[length_s*length_s];
	cand_str2=new int *[length_s*length_s];
	for (int i=0;i<=length_s*length_s-1;i++){
		cand_str1[i]=new int [length_s];
		cand_str2[i]=new int [length_s];
	}
	cand_fit=new double [length_s*length_s];
	near_cand1=new int *[num_near_nodes];
	for (int i=0;i<=num_near_nodes-1;i++)
		near_cand1[i]=new int [6];
	near_fit=new double [num_near_nodes];
	rever_list=new int [length_s*length_s];
	// tabu search variables
	tabu_table=new int *[v_num];
	tabu_table_c=new int *[v_num];
	for (int i=0;i<=v_num-1;i++){
		tabu_table[i]=new int [num_v+1];
		tabu_table_c[i]=new int [num_v+1];
	}
}
LS::~LS() {
	delete tem;
	delete each_route;
	delete [] record_table;
	delete [] s_t;
	for (int i=0;i<=length_s*length_s-1;i++){
		delete [] cand_str1[i];
		delete [] cand_str2[i];
	}
	delete [] cand_str1;
	delete [] cand_str2;
	delete [] cand_fit;
	for (int i=0;i<=num_near_nodes-1;i++)
		delete [] near_cand1[i];
	delete [] near_cand1;
	delete [] near_fit;
	delete [] rever_list;
	for (int i=0;i<=v_num-1;i++){
		delete [] tabu_table[i];
		delete [] tabu_table_c[i];
	}
	delete [] tabu_table;
	delete [] tabu_table_c;
}
void LS::update_best_solution(){
	if (s->fitness < b_s->fitness){
		for (int i=0;i<=v_num-1;i++)
			for (int j=0;j<=num_v;j++){
				b_s->sol[i][j]=s->sol[i][j];
				b_s->sol_r[i][j]=s->sol_r[i][j];
			}
		b_s->fitness=s->fitness;
	}
}
void LS::supply_edge(){
	int p;
	for (int i=0;i<v_num;i++){
		p=0;
		while(1){
			I_data->edge[p][b_s->sol[i][p]]=1;
			p=b_s->sol[i][p];
			if (p==0)break;
		}
	}
}
void LS::reverse_fun(){// get value for sol_r;
	int pointer;
	vector<int >tour;
	for (int i=0;i<=v_num-1;i++)	{
		pointer=s->sol[i][0];
		while (pointer!=0)		{
			tour.push_back(pointer);
			pointer=s->sol[i][pointer];
		}
		s->sol_r[i][0]=tour.back();
		tour.pop_back();
		pointer=s->sol_r[i][0];
		while (tour.size())		{
			s->sol_r[i][pointer]=tour.back();
			pointer=s->sol_r[i][pointer];
			tour.pop_back();
		}
		s->sol_r[i][pointer]=0;
	}
}
void LS::replace_fun(int *move){
	int string1[length_s];
	int string2[length_s];
	for (int i=0;i<=length_s-1;i++){
		string1[i]=-1;
		string2[i]=-1;
	}
	int count1=0;
	int row1=move[5];
	int row2=s_t[move[2]];
	while(1){
		if (move[0]!=move[1]){
			string1[count1]=move[0];
			count1++;
			move[0]=s->sol[row1][move[0]];
		}
		else{
			string1[count1]=move[1];count1++;
			break;
		}
	}
	int count2=0;
	while (1){
		if (move[2]!=move[3]){
			string2[count2]=move[2];
			count2++;
			if (move[4]==1)
				move[2]=s->sol[row2][move[2]];
			else
				move[2]=s->sol_r[row2][move[2]];
		}
		else{
			string2[count2]=move[3];count2++;
			break;
		}
	}
	I_data->use_frequency[count1-1][count2-1]=I_data->use_frequency[count1-1][count2-1]+1;
	if (cross_exchange_identify==1){
		for (int i=1;i<=count1-1;i++){
			s_t[string1[i]]=row2;
		}
		for (int i=0;i<=count2-1;i++){
			s_t[string2[i]]=row1;
		}
	}
	if (cross_exchange_identify==2){
		for (int i=1;i<=count1-1;i++){
			tabu_table[s_t[string1[i]]][string1[i]]=iteration + tabu_l+rand()%tabu_l;// tabu
			s_t[string1[i]]=row2;
		}
		for (int i=0;i<=count2-1;i++){
			tabu_table[s_t[string2[i]]][string2[i]]=iteration + tabu_l+rand()%tabu_l;// tabu
			s_t[string2[i]]=row1;
		}
	}

	int start1=string1[0];
	int start11=start1;
	int end1=s->sol[row1][string1[count1-1]];
	int end11=end1;
	int start2,end2,start22;
	if (move[4]==1){//whether reverse 1 is not and 0 is true
		start2=s->sol_r[row2][string2[0]];
		end2=s->sol[row2][string2[count2-1]];
		start22=start2;
		for (int i=1;i<=count1-1;i++){
			s->sol[row1][string1[i]]=-1;
			s->sol_r[row1][string1[i]]=-1;
		}
		for (int i=0;i<=count2-1;i++){
			s->sol[row2][string2[i]]=-1;
			s->sol_r[row2][string2[i]]=-1;
		}
		for (int i=0;i<=count2-1;i++){
			s->sol[row1][start1]=string2[i];
			start1=string2[i];
		}
		s->sol[row1][start1]=end11;
		for(int i=1;i<=count1-1;i++){
			s->sol[row2][start2]=string1[i];
			start2=string1[i];
		}
		s->sol[row2][start2]=end2;
		// for sol_r
		for (int i=count2-1;i>=0;i--){
			s->sol_r[row1][end1]=string2[i];
			end1=string2[i];
		}
		s->sol_r[row1][end1]=start11;
		for (int i=count1-1;i>=1;i--){
			s->sol_r[row2][end2]=string1[i];
			end2=string1[i];
		}
		s->sol_r[row2][end2]=start22;
	}
	else{
		start2=s->sol[row2][string2[0]];
		end2=s->sol_r[row2][string2[count2-1]];
		start22=start2;
		for (int i=1;i<=count1-1;i++){
			s->sol[row1][string1[i]]=-1;
			s->sol_r[row1][string1[i]]=-1;
		}
		for (int i=0;i<=count2-1;i++){
			s->sol[row2][string2[i]]=-1;
			s->sol_r[row2][string2[i]]=-1;
		}
		for (int i=0;i<=count2-1;i++){
			s->sol[row1][start1]=string2[i];
			start1=string2[i];
		}
		s->sol[row1][start1]=end11;
		for(int i=1;i<=count1-1;i++){
			s->sol_r[row2][start2]=string1[i];
			start2=string1[i];
		}
		s->sol_r[row2][start2]=end2;
		// for sol_r
		for (int i=count2-1;i>=0;i--){
			s->sol_r[row1][end1]=string2[i];
			end1=string2[i];
		}
		s->sol_r[row1][end1]=start11;
		for (int i=count1-1;i>=1;i--){
			s->sol[row2][end2]=string1[i];
			end2=string1[i];
		}
		s->sol[row2][end2]=start22;
	}
}
void LS::intra_route_optimization(){// this method will include many methods to optimize each route, such as 2-opt, Lk and EAX.
	int pointer;
	tem->fitness=s->fitness;
	for (int i=0;i<=v_num-1;i++)
		for (int j=0;j<=num_v;j++){
			tem->sol[i][j]=s->sol[i][j];
			tem->sol_r[i][j]=s->sol_r[i][j];
		}
	int number_routes[v_num];
	int route[num_v+1];
	for (int i=0;i<=v_num-1;i++){
		if (record_table[i]==1){
			record_table[i]=0;
			pointer=s->sol[i][0];number_routes[i]=0;
			route[number_routes[i]]=0;number_routes[i]++;
			while (1){
				if (pointer==0)
					break;
				else{
					route[number_routes[i]]=pointer;
					number_routes[i]++;
					pointer=s->sol[i][pointer];
				}
			}
			int sub_route[number_routes[i]];
			for (int j=0;j<=number_routes[i]-1;j++)
				sub_route[j]=route[j];
			if (intra_opt==1)
				each_route->twoopt(sub_route,number_routes[i]);
			if (intra_opt==3 && number_routes[i]>4){
					EAX(gEnv, sub_route,number_routes[i],I_data->x,I_data->y);
			}
			pointer=0;
			for (int j=1;j<=number_routes[i]-1;j++){
				tem->sol[i][pointer]=sub_route[j];
				pointer=tem->sol[i][pointer];
			}
			tem->sol[i][pointer]=0;
			pointer=0;
			for (int j=number_routes[i]-1;j>=1;j--){
				tem->sol_r[i][pointer]=sub_route[j];
				pointer=tem->sol_r[i][pointer];
			}
			tem->sol_r[i][pointer]=0;
		}
		else
			continue;
	}
	double current_cost=0;
	for (int i=0;i<=v_num-1;i++){
		pointer=0;
		while (1){
			current_cost+=I_data->Distance[pointer][tem->sol[i][pointer]];
			if (tem->sol[i][pointer]==0)
				break;
			else
				pointer=tem->sol[i][pointer];
		}
	}
	if (current_cost<s->fitness){
		s->fitness=current_cost;
		for (int i=0;i<=v_num-1;i++)
			for (int j=0;j<=num_v;j++){
				s->sol[i][j]=tem->sol[i][j];
				s->sol_r[i][j]=tem->sol_r[i][j];
			}
	}
	//update_best_solution();
}
void LS::replace_fun_insert(double min_delta,int row_out,int city_out,int row_in,int city_in){
	int n_p,p_p;
	s->fitness+=min_delta;
	n_p=s->sol[row_out][city_out];
	p_p=s->sol_r[row_out][city_out];
	s->sol[row_out][p_p]=n_p;
	s->sol_r[row_out][n_p]=p_p;
	s->sol[row_out][city_out]=-1;
	s->sol_r[row_out][city_out]=-1;
	//
	n_p=s->sol[row_in][city_in];
	s->sol[row_in][city_in]=city_out;
	s->sol[row_in][city_out]=n_p;
	s->sol_r[row_in][n_p]=city_out;
	s->sol_r[row_in][city_out]=city_in;
}
double LS::calculate_fit_insert(int row_out,int city_out,int row_in,int city_in){
	double delta=0;
	delta+=I_data->Distance[s->sol_r[row_out][city_out]][s->sol[row_out][city_out]];
	delta=delta-I_data->Distance[s->sol_r[row_out][city_out]][city_out];
	delta=delta-I_data->Distance[s->sol[row_out][city_out]][city_out];
	//
	delta+=I_data->Distance[city_in][city_out]+I_data->Distance[city_out][s->sol[row_in][city_in]];
	delta=delta-I_data->Distance[city_in][s->sol[row_in][city_in]];
	return delta;
}
void LS::insert(){// this function is used to implement a simple inserting operation
	int pointer,pointer_2;
	int row_out,city_out,row_in,city_in;
	double delta,min_delta;
	while (1){
		min_delta=Max;
		for (int i=0;i<=v_num-1;i++){
			pointer=s->sol[i][0];
			if (s->sol_r[i][pointer]==0 && s->sol[i][pointer]==0)
				continue;
			while(1){
				for (int j=0;j<=v_num-1;j++){
					if (j==i)continue;
					pointer_2=0;
					while(1){
						//////////////
						if (I_data->edge[pointer][pointer_2]!=1){
							pointer_2=s->sol[j][pointer_2];
							if (pointer_2==0)break;
							else continue;
						}
						//////////////
						delta=calculate_fit_insert(i,pointer,j,pointer_2);
						if (delta<min_delta){
							min_delta=delta;
							row_out=i;
							city_out=pointer;
							row_in=j;
							city_in=pointer_2;
						}
						pointer_2=s->sol[j][pointer_2];
						if (pointer_2==0)break;
					}
				}
				pointer=s->sol[i][pointer];
				if (pointer==0)break;
			}
		}
		if (min_delta+1.0e-7>0)break;
		replace_fun_insert(min_delta,row_out,city_out,row_in,city_in);
		record_table[row_out]=1;
		record_table[row_in]=1;
		if (ceil((clock()-I_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
	}
}

double LS::gain_fun(int st1,int end1,int row1,int st2,int end2,int row2,int k, int reverse){
	double fitn=0;
	if (k==0 && reverse==1)// forward
		fitn+=I_data->Distance[st2][s->sol_r[s_t[st2]][st2]]+I_data->Distance[end2][s->sol[s_t[end2]][end2]]-
			I_data->Distance[s->sol_r[s_t[st2]][st2]][s->sol[s_t[end2]][end2]]-I_data->Distance[end2][s->sol[row1][st1]];
	if (k==0 && reverse==0)// reverse
		fitn+=I_data->Distance[st2][s->sol[s_t[st2]][st2]]+I_data->Distance[end2][s->sol_r[s_t[end2]][end2]]-
			I_data->Distance[s->sol[s_t[st2]][st2]][s->sol_r[s_t[end2]][end2]]-I_data->Distance[end2][s->sol[row1][st1]];
	if (k!=0 && reverse==1)
		fitn+=I_data->Distance[st2][s->sol_r[s_t[st2]][st2]]+I_data->Distance[end2][s->sol[s_t[end2]][end2]]+
			I_data->Distance[end1][s->sol[s_t[end1]][end1]]-I_data->Distance[s->sol_r[s_t[st2]][st2]][s->sol[row1][st1]]-
			I_data->Distance[s->sol[s_t[end2]][end2]][end1]-I_data->Distance[end2][s->sol[s_t[end1]][end1]];
	if (k!=0 && reverse==0)
		fitn+=I_data->Distance[st2][s->sol[s_t[st2]][st2]]+I_data->Distance[end2][s->sol_r[s_t[end2]][end2]]+
			I_data->Distance[end1][s->sol[s_t[end1]][end1]]-I_data->Distance[s->sol[s_t[st2]][st2]][s->sol[row1][st1]]-
			I_data->Distance[s->sol_r[s_t[end2]][end2]][end1]-I_data->Distance[end2][s->sol[s_t[end1]][end1]];
	return fitn;
}

void LS:: cross_exchange_1(){
	int pointer;
	for (int i=0;i<=v_num-1;i++){
		pointer=s->sol[i][0];
		while (pointer!=0){
			s_t[pointer]=i;
			pointer=s->sol[i][pointer];
		}
	}
	double gi;int j;int in_num;
	int end2,end1;int count;double tem;
	int better_move[6];double better_fit;
	int reverse;
	int step=0;
	int identify;int identify2;
	while (1){
		better_fit=0;step++;
		for (int i=0;i<=v_num-1;i++){
			//pointer=s->sol[i][0];
			pointer=0;
			identify=1;
			while(1){
				if (pointer==0 && identify==0){identify=0;break;}
				else{
					c=0;
					//////////////////////////////////////////////////////
					for (j=1;j<=num_v;j++){
						in_num=-1;
						if(I_data->edge[pointer][j]==1){
							if (i!=s_t[j])
								in_num=j;
						}
					//////////////////////////////////////////////////////
						if (in_num==-1)continue;
						count=0;
						gi=0;//
						gi+=I_data->Distance[pointer][s->sol[i][pointer]];
						gi=gi-I_data->Distance[pointer][in_num];
						if (1){// case 1
							end1=pointer;identify2=1;
							for (int k=0;k<=length_s-1;k++){// carry the out row
								reverse=1;
								if (k>0){
									end1=s->sol[i][end1];identify2=0;
								}
								if (end1==0 &&identify2==0)break;//
								end2=in_num;
								for (int n=0;n<=length_s-1;n++){
									if (n==1){// determine the right or left
										if (s->sol[s_t[in_num]][in_num]==0)reverse=0;
										if (s->sol_r[s_t[in_num]][in_num]==0)reverse=1;
										if (s->sol_r[s_t[in_num]][in_num]!=0 && s->sol[s_t[in_num]][in_num]!=0){
											if (gain_fun(pointer,end1,i,in_num,s->sol[s_t[in_num]][in_num],s_t[in_num],k,1)>
												gain_fun(pointer,end1,i,in_num,s->sol_r[s_t[in_num]][in_num],s_t[in_num],k,0))
												reverse=1;
											else
												reverse=0;
										}
									}
									if (reverse==1 && n!=0){
										end2=s->sol[s_t[end2]][end2];
										}
									if (reverse==0 && n!=0){
										end2=s->sol_r[s_t[end2]][end2];
										}
									if (end2==0)break;
									if (pointer==end1 && (s->sol[s_t[end2]][end2]==0 || s->sol_r[s_t[end2]][end2]==0))break;
									if (gi+gain_fun(pointer,end1,i,in_num,end2,s_t[in_num],k,reverse)>0){
										count++;
										cand_str1[count-1][0]=pointer;
										cand_str1[count-1][1]=end1;
										cand_str1[count-1][2]=i;
										cand_str2[count-1][0]=in_num;
										cand_str2[count-1][1]=end2;
										rever_list[count-1]=reverse;
										cand_fit[count-1]=gi+gain_fun(pointer,end1,i,in_num,end2,s_t[in_num],k,reverse);
									}
								}
							}
						}
						if (count==0){continue;}
						tem=0;
						for (int k=0;k<=count-1;k++)
							if (tem<cand_fit[k]){
								near_cand1[c][0]=cand_str1[k][0];
								near_cand1[c][1]=cand_str1[k][1];
								near_cand1[c][2]=cand_str2[k][0];
								near_cand1[c][3]=cand_str2[k][1];
								near_cand1[c][4]=rever_list[k];
								near_cand1[c][5]=cand_str1[k][2];
								near_fit[c]=cand_fit[k];
								tem=cand_fit[k];////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							}
						c++;
					}
					for (j=1;j<=c-1;j++){
						if (better_fit<near_fit[j]){
							better_fit=near_fit[j];
							better_move[0]=near_cand1[j][0];
							better_move[1]=near_cand1[j][1];
							better_move[2]=near_cand1[j][2];
							better_move[3]=near_cand1[j][3];
							better_move[4]=near_cand1[j][4];
							better_move[5]=near_cand1[j][5];
						}
					}
				}
				pointer=s->sol[i][pointer];
				if (pointer==0)break;
			}
		}

		if (better_fit<1.0e-7)
			break;
		s->fitness=s->fitness-better_fit;
		record_table[better_move[5]]=1;
		record_table[s_t[better_move[2]]]=1;
		replace_fun(better_move);
		if (ceil((clock()-I_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
	}
	//update_best_solution();
}

void LS::cross_exchange(){
	insert();

	cross_exchange_1();
}
void LS::local_search(){
	reverse_fun();// get value for sol_r;
	iteration_c=0;
	// to iterate all steps
	for (int i=0;i<=v_num-1;i++)
		record_table[i]=1;
	double local_optima;
	//intra_route_optimization();
	//cross_exchange();
	local_optima=s->fitness;
	//intra_route_optimization();

	while (1){
		cross_exchange();
		if (local_optima-1.0e-7<s->fitness && local_optima+1.0e-7>s->fitness)
			break;
		else{
			local_optima=s->fitness;
			intra_route_optimization();
		}
		if(ceil((clock() - I_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
	}
	for (int i=0;i<=v_num-1;i++){
		for (int j=0;j<=num_v;j++){
			b_s->sol[i][j]=s->sol[i][j];
			b_s->sol_r[i][j]=s->sol_r[i][j];
		}
	}
	b_s->fitness=s->fitness;
}




//////////////////////////////////////////////////////////////////
void LS:: cross_exchange_2(){

	int pointer;improve=0;
	for (int i=0;i<=v_num-1;i++){
		pointer=s->sol[i][0];
		while (pointer!=0){
			s_t[pointer]=i;
			pointer=s->sol[i][pointer];
		}
	}
	double gi;int j;int in_num;int rand_order;
	int end2,end1;
	int better_move[6];
	int reverse;
	int identify;int identify2;
	double delta,min_delta,min_tabu_delta;
	int can_node[100][6];
	int can_tabu_node[100][6];
	int num_best;
	int num_tabu_best;
	while (1){
		min_delta=-Max;
		min_tabu_delta=-Max;
		num_best=0;
		num_tabu_best=0;
		for (int i=0;i<=v_num-1;i++){
			pointer=0;
			identify=1;
			while(1){
				if (pointer==0 && identify==0){identify=0;break;}
				else{
					//////////////////////////////////////////////////////
					for (j=1;j<=num_v;j++){
						in_num=-1;
						if(I_data->edge[pointer][j]==1){
							if (i!=s_t[j])
								in_num=j;
						}
					//////////////////////////////////////////////////////
						if (in_num==-1)continue;
						gi=0;//
						gi+=I_data->Distance[pointer][s->sol[i][pointer]];
						gi=gi-I_data->Distance[pointer][in_num];

						end1=pointer;identify2=1;
						for (int k=0;k<=length_s-1;k++){// carry the out row
							reverse=1;
							if (k>0){
								end1=s->sol[i][end1];identify2=0;
							}
							if (end1==0 &&identify2==0)break;//
							end2=in_num;
							for (int n=0;n<=length_s-1;n++){
								if (n==1){// determine the right or left
									if (s->sol[s_t[in_num]][in_num]==0)reverse=0;
									if (s->sol_r[s_t[in_num]][in_num]==0)reverse=1;
									if (s->sol_r[s_t[in_num]][in_num]!=0 && s->sol[s_t[in_num]][in_num]!=0){
										if (gain_fun(pointer,end1,i,in_num,s->sol[s_t[in_num]][in_num],s_t[in_num],k,1)>
											gain_fun(pointer,end1,i,in_num,s->sol_r[s_t[in_num]][in_num],s_t[in_num],k,0))
											reverse=1;
										else
											reverse=0;
									}
								}
								if (reverse==1 && n!=0)
									end2=s->sol[s_t[end2]][end2];
								if (reverse==0 && n!=0)
									end2=s->sol_r[s_t[end2]][end2];
								if (end2==0)break;
								if (pointer==end1 && (s->sol[s_t[end2]][end2]==0 || s->sol_r[s_t[end2]][end2]==0))break;
								if (pointer==0 && s->sol[i][end1]==0 && ( (s->sol[s_t[in_num]][end2]==0 &&s->sol_r[s_t[in_num]][in_num]==0 ) ||
										(s->sol_r[s_t[in_num]][end2]==0 &&s->sol[s_t[in_num]][in_num]==0)))
									break;
								delta=gi+gain_fun(pointer,end1,i,in_num,end2,s_t[in_num],k,reverse);
								if ((end1==0 && tabu_table[i][end2]<=iteration)
									|| (tabu_table[i][end2]<=iteration && tabu_table[s_t[end2]][end1]<=iteration)){
									if (delta>min_delta){
										min_delta=delta;
										can_node[0][0]=pointer;
										can_node[0][1]=end1;
										can_node[0][2]=in_num;
										can_node[0][3]=end2;
										can_node[0][4]=reverse;
										can_node[0][5]=i;
										num_best=1;
									}
									else if (min_delta+1.0e-7>delta && min_delta-1.0e-7<delta && num_best<100){
										can_node[num_best][0]=pointer;
										can_node[num_best][1]=end1;
										can_node[num_best][2]=in_num;
										can_node[num_best][3]=end2;
										can_node[num_best][4]=reverse;
										can_node[num_best][5]=i;
										num_best++;
									}
								}
								else{
									if (min_tabu_delta<delta){
										min_tabu_delta=delta;
										can_tabu_node[0][0]=pointer;
										can_tabu_node[0][1]=end1;
										can_tabu_node[0][2]=in_num;
										can_tabu_node[0][3]=end2;
										can_tabu_node[0][4]=reverse;
										can_tabu_node[0][5]=i;
										num_tabu_best=1;
									}
									else if (min_tabu_delta+1.0e-7>delta && min_tabu_delta-1.0e-7<delta && num_tabu_best<100){
										min_tabu_delta=delta;
										can_tabu_node[num_tabu_best][0]=pointer;
										can_tabu_node[num_tabu_best][1]=end1;
										can_tabu_node[num_tabu_best][2]=in_num;
										can_tabu_node[num_tabu_best][3]=end2;
										can_tabu_node[num_tabu_best][4]=reverse;
										can_tabu_node[num_tabu_best][5]=i;
										num_tabu_best++;
									}
								}
							}
						}
					}
				}
				pointer=s->sol[i][pointer];
				if (pointer==0)break;
			}
		}
		if ((num_tabu_best>0 && min_tabu_delta+1.0e-7>min_delta && (s->fitness-min_tabu_delta - 1.0e-7 <b_s->fitness)) || num_best==0){
			s->fitness=s->fitness-min_tabu_delta;
			rand_order=rand()%num_tabu_best;
			better_move[0]=can_tabu_node[rand_order][0];
			better_move[1]=can_tabu_node[rand_order][1];
			better_move[2]=can_tabu_node[rand_order][2];
			better_move[3]=can_tabu_node[rand_order][3];
			better_move[4]=can_tabu_node[rand_order][4];
			better_move[5]=can_tabu_node[rand_order][5];
		}
		else{
			s->fitness=s->fitness-min_delta;
			rand_order=rand()%num_best;
			better_move[0]=can_node[rand_order][0];
			better_move[1]=can_node[rand_order][1];
			better_move[2]=can_node[rand_order][2];
			better_move[3]=can_node[rand_order][3];
			better_move[4]=can_node[rand_order][4];
			better_move[5]=can_node[rand_order][5];
		}
		record_table[better_move[5]]=1;
		record_table[s_t[better_move[2]]]=1;
		replace_fun(better_move);
		if (s->fitness+1.0e-7<b_s->fitness){
			update_best_solution();
	//		break;
			improve=0;iteration_c=iteration;
			supply_edge();
		}
		else
			improve++;
		if (improve>threshold)
			break;
		iteration++;
		if (ceil((clock()-I_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
	}
}
void LS::insert_tabu(){
	int pointer;improve=0;
	int pointer_2;
	int longest_tour=0;
	int longest_third;
	double delta;
	int row_out,city_out,row_in,city_in;
	int can_node[100][4];
	int can_tabu_node[100][4];
	double min_delta,min_tabu_delta;
	int num_best,num_tabu_best;
	int rand_order; double delta_tabu;
	while (1){
		min_delta=Max;
		min_tabu_delta=Max;
		num_best=0;
		num_tabu_best=0;
		for (int i=0;i<=v_num-1;i++){
			pointer=s->sol[i][0];
			if (s->sol_r[i][pointer]==0 && s->sol[i][pointer]==0)
				continue;
			while(1){
				for (int j=0;j<=v_num-1;j++){
					if (j==i)continue;
					pointer_2=0;
					while(1){
						/////////////////////////////////////////////////////////////
						if (I_data->edge[pointer][pointer_2]!=1){
							pointer_2=s->sol[j][pointer_2];
							if (pointer_2==0)break;
							else continue;
						}
						//////////////////////////////////////////////////////////////
						delta=calculate_fit_insert(i,pointer,j,pointer_2);
						if(tabu_table[i][pointer]<iteration){
							min_delta=delta;
							can_node[0][0]=i;
							can_node[0][1]=pointer;
							can_node[0][2]=j;
							can_node[0][3]=pointer_2;
							num_best=1;
						}
						else if (min_delta+1.0e-7>delta && min_delta-1.0e-7<delta && num_best<100){
							can_node[num_best][0]=i;
							can_node[num_best][1]=pointer;
							can_node[num_best][2]=j;
							can_node[num_best][3]=pointer_2;
							num_best++;
						}
						else {
							if (min_tabu_delta<delta){
								min_tabu_delta=delta;
								can_tabu_node[0][0]=i;
								can_tabu_node[0][1]=pointer;
								can_tabu_node[0][2]=j;
								can_tabu_node[0][3]=pointer_2;
								num_tabu_best=1;
							}
							else if (min_tabu_delta+1.0e-7 >delta && min_tabu_delta-1.0e-7<delta && num_tabu_best<100){
								can_tabu_node[num_tabu_best][0]=i;
								can_tabu_node[num_tabu_best][1]=pointer;
								can_tabu_node[num_tabu_best][2]=j;
								can_tabu_node[num_tabu_best][3]=pointer_2;
								num_tabu_best++;
							}
						}
						pointer_2=s->sol[j][pointer_2];
						if (pointer_2==0)break;
					}
				}
				pointer=s->sol[i][pointer];
				if (pointer==0)break;
			}
		}
		if ((num_tabu_best>0 && min_tabu_delta+1.0e-7<min_delta && (s->fitness+min_tabu_delta+1.0e-7 <b_s->fitness))|| num_best==0){
			rand_order=rand()%num_tabu_best;
			delta_tabu=min_tabu_delta;
			row_out=can_tabu_node[rand_order][0];
			city_out=can_tabu_node[rand_order][1];
			row_in=can_tabu_node[rand_order][2];
			city_in=can_tabu_node[rand_order][3];
		}
		else{
			rand_order=rand()%num_best;
			delta_tabu=min_delta;
			row_out=can_node[rand_order][0];
			city_out=can_node[rand_order][1];
			row_in=can_node[rand_order][2];
			city_in=can_node[rand_order][3];
		}
		tabu_table[row_out][city_out]=iteration + tabu_l+rand()%tabu_l;
		replace_fun_insert(min_delta,row_out,city_out,row_in,city_in);
		record_table[row_out]=1;
		record_table[row_in]=1;
		if (s->fitness+1.0e-7<b_s->fitness){
			update_best_solution();
			improve=0;
			supply_edge();
//			break;
		}
		else
			improve++;
		if (improve>threshold)
			break;
		iteration++;
		if (ceil((clock()-I_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
	}
}

void LS::weak_perturb(){
	reverse_fun();// get value for sol_r;
	for (int i=0;i<=v_num-1;i++)
		for (int j=0;j<=num_v;j++)
			tabu_table[i][j]=0;
	iteration=1;
	double local_optima=s->fitness;
	int no_improve=0;
	while(1){
		if (ceil((clock()-I_data->start)/static_cast<double>(CLOCKS_PER_SEC))>=time_length)
			break;
		insert_tabu();
		iteration+=tabu_l*2;
		cross_exchange_2();
		iteration+=tabu_l*2;
		intra_route_optimization();
		if (s->fitness+1.0e-7<b_s->fitness)
			update_best_solution();
		if (local_optima+1.0e-7>b_s->fitness && local_optima-1.0e-7<b_s->fitness)
			no_improve++;
		else{
			local_optima=b_s->fitness;no_improve=0;
		}
		if (no_improve>50)break;
		cout<<b_s->fitness<<"  "<<s->fitness<<"   "<<no_improve<<endl;
	}
}
