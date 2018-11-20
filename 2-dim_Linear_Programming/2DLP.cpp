#include<iostream>
#include<vector>
#include <stdlib.h>
#include <math.h>
#include <iomanip> 
#include <algorithm>
using namespace std;
typedef struct{
	double Xcoef;
	double Ycoef;
	double constant;
} LinearFunction;
int Find_X_star(vector<LinearFunction> &negative, vector<LinearFunction> &positive, double &mid_x){
	double t_max, t_min, s_max, s_min;
	double n_max = -1e10; 
	double p_min = 1e10;
	double n_cur_y, n_slope, p_cur_y, p_slope;

	for(int i = 0;i < negative.size();i++){
		n_cur_y = ((-1)*negative[i].Xcoef*mid_x+negative[i].constant)/negative[i].Ycoef;
		n_slope = ((-1)*negative[i].Xcoef)/negative[i].Ycoef;
		if(i == 0){
			n_max = n_cur_y;
			s_max = n_slope;
			s_min = n_slope;			
		}
		else{
			if((n_cur_y - n_max) > 1e-5){
				n_max = n_cur_y;
				s_max = n_slope;
				s_min = n_slope;
			}
			else if(abs(n_cur_y - n_max) <= 1e-5){
				if(n_slope - s_max > 1e-5)
					s_max = n_slope;
				else if(s_min - n_slope > 1e-5)
					s_min = n_slope;
			}
		}
	}
	//cout << endl;
	for(int i = 0;i < positive.size();i++){
		p_cur_y = ((-1)*positive[i].Xcoef*mid_x+positive[i].constant)/positive[i].Ycoef;
		p_slope = ((-1)*positive[i].Xcoef)/positive[i].Ycoef;
		if(i == 0){
			p_min = p_cur_y;
			t_max = p_slope;
			t_min = p_slope;			
		}
		else{
			if((p_min - p_cur_y) > 1e-5){
				p_min = p_cur_y;
				t_max = p_slope;
				t_min = p_slope;
			}
			else if(abs(p_cur_y - p_min) <= 1e-5){
				if(p_slope - t_max > 1e-5)
					t_max = p_slope;
				else if(t_min - p_slope > 1e-5)
					t_min = p_slope;
			}
		}
	}
	/* Now to determine 6cases. */
	if(n_max - p_min < 1e-5){
		if(s_max < 0)  // x_m < x_*
			return 1;
		else if(s_min > 0) //x_* < x_m
			return 0;
		else if(s_min <= 0 && s_max >= 0){ //x_m = x_*
			cout << int(round(n_max)) << endl;
			return -1;
		}
		else return -1;
	}
	else{
		if(t_min - s_max > 1e-5) //x_m < x_*
			return 1;
		else if(s_min - t_max> 1e-5) // x_* < x_m
			return 0;
		else if(s_max - t_min > -1e5 && s_min - t_max < 1e-5){ // no feasible region
			cout << "NA" <<endl;
			return -2;
		}
		else return -2;
	}
}
int Find_X_range(vector<LinearFunction> &zero, double &Xleft, double &Xright){
	int x_value = 0;
	for(int i = 0;i < zero.size();i++){
		x_value = zero[i].constant/zero[i].Xcoef;
		if(zero[i].Xcoef < 0){
			if(x_value > Xleft)
				Xleft = x_value;
		}
		else{
			if(x_value < Xright)
				Xright = x_value;
		}
	}
	if(Xleft > Xright) return -1;
	else return 0;

}
double find_intersect_X(LinearFunction &v, LinearFunction &w){
	double x_value;
	LinearFunction tmp1, tmp2;
	tmp1.Xcoef = v.Xcoef * w.Ycoef;
	tmp1.Ycoef = v.Ycoef * w.Ycoef;
	tmp1.constant = v.constant * w.Ycoef;
	tmp2.Xcoef = w.Xcoef * v.Ycoef;
	tmp2.Ycoef = w.Ycoef * v.Ycoef;
	tmp2.constant = w.constant * v.Ycoef;
	x_value = (tmp2.constant - tmp1.constant)/(tmp2.Xcoef - tmp1.Xcoef);
	return x_value;
}
double find_intersect_Y(LinearFunction &v, LinearFunction &w){
	double y_value;
	LinearFunction tmp1, tmp2;
	tmp1.Xcoef = v.Xcoef * w.Xcoef;
	tmp1.Ycoef = v.Ycoef * w.Xcoef;
	tmp1.constant = v.constant * w.Xcoef;
	tmp2.Xcoef = w.Xcoef * v.Xcoef;
	tmp2.Ycoef = w.Ycoef * v.Xcoef;
	tmp2.constant = w.constant * v.Xcoef;
	y_value = (tmp2.constant - tmp1.constant)/(tmp2.Ycoef - tmp1.Ycoef);
	return y_value;
}
int is_parallel(LinearFunction &v, LinearFunction &w, int flag){ // return 1 : delete the first line , 0: second line
	if(v.Xcoef == 0){
		if(w.Xcoef == 0){
			if(flag == 1)
				return (v.constant/v.Ycoef < w.constant/w.Ycoef);
			else
				return (v.constant/v.Ycoef > w.constant/w.Ycoef);
		}
		else return -1;
	}
	else if(w.Xcoef/v.Xcoef == w.Ycoef/v.Ycoef){
		if(v.constant == 0){
			if(w.constant == 0)
				return 1;
			else{
				if(flag == 1)	return (w.constant/w.Ycoef > 0);
				else 	return (w.constant/w.Ycoef < 0);
			}
		}
		if(w.Xcoef/v.Xcoef == w.constant/v.constant)
			return 1;
		else{
			if(flag == 1) // parallel line in I^-	
				return (v.constant/v.Ycoef < w.constant/w.Ycoef);  
			else // parallel line in I^+
				return (v.constant/v.Ycoef > w.constant/w.Ycoef);
		}	
	}
	else return -1;
}
int examine(LinearFunction &v, LinearFunction &w, double &x, double &y, double &Xleft, double &Xright){
	if(x*w.Xcoef + y*w.Ycoef > w.constant || x*v.Xcoef + y*v.Ycoef > v.constant) return -1;
	if(x < Xleft || x > Xright) return -1;
	else return 1;
}
void Brute_force(vector<LinearFunction> &negative, vector<LinearFunction> &positive, double Xleft, double Xright){
	vector<double> inter_x; vector<double> inter_y; double min_y = 1e10; double min_x;
	double val_y; double val_x; 
	double val_dis_x = 1e-3;
	int flag = 0;
	if(positive.size() == 0){
		if(negative[0].Xcoef == 0){
			cout << int(round(negative[0].constant/negative[0].Ycoef));
			return;
		}
		else if((-1)*negative[0].Xcoef/negative[0].Ycoef > 0){
			if(Xleft == -1e7){
				cout << "-INF" <<endl;
				return;
			}
			else{
				cout << int(round((negative[0].constant- negative[0].Xcoef*Xleft)/negative[0].Ycoef)) <<endl;
				return;
			}
		}
		else{
			if(Xright == 1e7){
				cout << "-INF" <<endl;
				return;
			}
			else{
				cout << int(round((negative[0].constant- negative[0].Xcoef*Xright)/negative[0].Ycoef)) <<endl;
				return;
			}
		}
	}
	else{
		if(Xleft != -1e7){
			inter_x.push_back(Xleft);
			inter_x.push_back(Xleft);
			inter_y.push_back((negative[0].constant - negative[0].Xcoef*Xleft)/negative[0].Ycoef);
			inter_y.push_back((positive[0].constant - positive[0].Xcoef*Xleft)/positive[0].Ycoef);
		}
		if(Xright != 1e7){
			inter_x.push_back(Xleft);
			inter_x.push_back(Xleft);
			inter_y.push_back((negative[0].constant - negative[0].Xcoef*Xright)/negative[0].Ycoef);
			inter_y.push_back((positive[0].constant - positive[0].Xcoef*Xright)/positive[0].Ycoef);
		}
		if(positive[0].Xcoef == 0){
			if(negative[0].Xcoef == 0){
				if(positive[0].constant/positive[0].Ycoef < negative[0].constant/negative[0].Ycoef){
					cout << "NA" <<endl;
				}
				else{
					cout << int(round(negative[0].constant/negative[0].Ycoef)) << endl;
				}
			}
		}
		else if(negative[0].Xcoef/positive[0].Xcoef == negative[0].Ycoef/positive[0].Ycoef){
			if(negative[0].constant/negative[0].Ycoef == positive[0].constant/positive[0].Ycoef){
			//if(negative[0].Xcoef/positive[0].Xcoef == negative[0].constant/positive[0].constant){
				if(negative[0].Xcoef/(-1)*negative[0].Ycoef < 0){
					if(Xright != 1e7){
						cout << int(round(Xright)) <<endl;
					}
					else cout << "-INF" <<endl;
				}
				else{
					if(Xleft != -1e7){
						cout << int(round(Xleft)) << endl;
					}
					else cout << "-INF" << endl;
				}
				return;
			}
			else{
				if(negative[0].constant/negative[0].Ycoef > positive[0].constant/positive[0].Ycoef){
					cout << "NA" <<endl;
					return;
				}
				else{
					if(negative[0].Xcoef/(-1)*negative[0].Ycoef < 0){
						if(Xright == 1e7) cout << "-INF" <<endl;
						else 
							cout << int(round((negative[0].constant - negative[0].Xcoef*Xright)/negative[0].Ycoef)) <<endl;
					}
					else{
						if(Xleft == -1e7) cout << "-INF" <<endl;
						else
							cout << int(round((negative[0].constant - negative[0].Xcoef*Xleft)/negative[0].Ycoef)) <<endl;
					}
					return;
				}
			}
		}
		else{
			inter_x.push_back(find_intersect_X(negative[0], positive[0]));
			inter_y.push_back(find_intersect_Y(negative[0], positive[0]));
		}
	}
	for(int i = 0;i < inter_x.size();i++){
		flag = examine(positive[0], negative[0], inter_x[i], inter_y[i], Xleft, Xright);
		if(flag == 1){
			if(inter_y[i] < min_y){
				min_y = inter_y[i];
				min_x = inter_x[i];
			}
		}
	}
	val_x = min_x - val_dis_x;
	val_y = (negative[0].constant - negative[0].Xcoef * val_x)/negative[0].Ycoef;
	if(val_y < min_y){
		if(examine(positive[0], negative[0], val_x, val_y, Xleft, Xright) == 1){
			cout << "-INF" <<endl;
			return;
		}
	}
	val_x = min_x + val_dis_x;
	val_y = (negative[0].constant - negative[0].Xcoef * val_x)/negative[0].Ycoef;
	if(val_y < min_y){
		if(examine(positive[0], negative[0], val_x, val_y, Xleft, Xright) == 1){
			cout << "-INF" <<endl;
			return;
		}
	}
	if(min_y == 1e10){
		cout << "NA" <<endl;
		return;
	}
	else{
		cout << int(round(min_y)) <<endl;
		return;
	}

}
void Prune_Search(vector<LinearFunction> &negative, vector<LinearFunction> &positive, double Xleft, double Xright){
	if(negative.size() == 0){
		cout << "-INF" <<endl;
		return;
	}
	vector<LinearFunction> tmp_negative; vector<LinearFunction> tmp_positive;
	int n_size = negative.size();
	int p_size = positive.size();
	//cout << "n_size: " <<n_size <<endl;
	//cout << "p_size: " <<p_size <<endl;
	
	if(n_size <= 1 && p_size <= 1){
		Brute_force(negative, positive, Xleft, Xright);
		return;
	}

	vector<double> total_x_value;
	int n_pair = floor(n_size/double(2));
	int p_pair = floor(p_size/double(2)); 
	double x_value;
	int *table_n = new int[n_pair];
	int *table_p = new int[p_pair];
	for(int i = 0;i < n_pair;i++){
		int parallel_value = is_parallel(negative[i*2], negative[i*2+1], 1);
		//cout << "parallel value: "<<parallel_value <<endl;
		if(parallel_value >= 0){
			tmp_negative.push_back(negative[i*2+parallel_value]);
			table_n[i] = 1;
			continue;
		}
		x_value = find_intersect_X(negative[i*2], negative[i*2+1]);


		/*if(x_value < Xleft || x_value > Xright){
			table_n[i] = 1;
			if((negative[i*2].constant-negative[i*2].Xcoef*Xleft)/negative[i*2].Ycoef > (negative[i*2+1].constant-negative[i*2+1].Xcoef*Xleft)/negative[i*2+1].Ycoef){
				tmp_negative.push_back(negative[i*2]);
			}
			else tmp_negative.push_back(negative[i*2+1]);
		}
		else */
		total_x_value.push_back(x_value);
	}
	for(int i = 0;i < p_pair;i++){
		int parallel_value = is_parallel(positive[i*2], positive[i*2+1], 0);
		//cout << "parallel value: "<<parallel_value <<endl;
		if(parallel_value >= 0){
			tmp_positive.push_back(positive[i*2+parallel_value]);
			table_p[i] = 1;
			continue;
		}
		x_value = find_intersect_X(positive[i*2], positive[i*2+1]);


		/*if(x_value < Xleft || x_value > Xright){
			table_p[i] = 1;
			if((positive[i*2].constant-positive[i*2].Xcoef*Xleft)/positive[i*2].Ycoef > (positive[i*2+1].constant-positive[i*2+1].Xcoef*Xleft)/positive[i*2+1].Ycoef){
				tmp_positive.push_back(positive[i*2+1]);
			}
			else tmp_positive.push_back(positive[i*2]);
		}
		else */
		total_x_value.push_back(x_value);
	}
	/* for x_value test */
	/*for(int i = 0;i < total_x_value.size();i++)
		cout << "x value: " << total_x_value[i] << endl;*/
	double mid_x = 0;
	if(total_x_value.size() >= 3){
		nth_element(total_x_value.begin(), total_x_value.begin()+total_x_value.size()/2, total_x_value.end());
		mid_x = total_x_value[total_x_value.size()/2];
		//cout << "mid_x: " <<mid_x <<endl;
	}
	else if(total_x_value.size() != 0){
		mid_x = total_x_value[total_x_value.size()/2];
		//cout << "mid_x: "<<mid_x <<endl;
	}
	double x_star_value = 0;
	if(total_x_value.size()!= 0){
		x_star_value = Find_X_star(negative, positive, mid_x);
		//cout << "x star value: " <<x_star_value <<endl;
		if(x_star_value == -2 || x_star_value == -1) return; // no feasible region.
		for(int i = 0;i < n_pair;i++){
			
			if(table_n[i] == 1)	continue;
			else{
				x_value = find_intersect_X(negative[i*2], negative[i*2+1]);
				//cout << "their intersect x value: " <<x_value <<endl;
				if(x_star_value == 1){ // x_m < x_*
					if(x_value <= mid_x){
						if((-1)*negative[i*2].Xcoef/negative[i*2].Ycoef > (-1)*negative[i*2+1].Xcoef/negative[i*2+1].Ycoef){
							tmp_negative.push_back(negative[i*2]);
						}
						else	tmp_negative.push_back(negative[i*2+1]);
					}
					else{
						tmp_negative.push_back(negative[i*2]);
						tmp_negative.push_back(negative[i*2+1]);
					}
				}
				else if(x_star_value == 0){  //x_* < x_m
					if(x_value >= mid_x){
						if((-1)*negative[i*2].Xcoef/negative[i*2].Ycoef < (-1)*negative[i*2+1].Xcoef/negative[i*2+1].Ycoef){
							tmp_negative.push_back(negative[i*2]);
						}
						else	tmp_negative.push_back(negative[i*2+1]);
					}
					else{
						tmp_negative.push_back(negative[i*2]);
						tmp_negative.push_back(negative[i*2+1]);
					}
				}
			}
		}
		for(int i = 0;i < p_pair;i++){
			if(table_p[i] == 1)	continue;
			else{
				x_value = find_intersect_X(positive[i*2], positive[i*2+1]);
				if(x_star_value == 1){ //x_m < x_*
					if(x_value <= mid_x){
						if((-1)*positive[i*2].Xcoef/positive[i*2].Ycoef > (-1)*positive[i*2+1].Xcoef/positive[i*2+1].Ycoef){
							tmp_positive.push_back(positive[i*2+1]);
						}
						else	tmp_positive.push_back(positive[i*2]);
					}
					else{
						tmp_positive.push_back(positive[i*2]);
						tmp_positive.push_back(positive[i*2+1]);
					}
				}
				else if(x_star_value == 0){ // x_* < x_m
					if(x_value >= mid_x){
						if((-1)*positive[i*2].Xcoef/positive[i*2].Ycoef < (-1)*positive[i*2+1].Xcoef/positive[i*2+1].Ycoef){
							tmp_positive.push_back(positive[i*2+1]);
						}
						else	tmp_positive.push_back(positive[i*2]);
					}
					else{
						tmp_positive.push_back(positive[i*2]);
						tmp_positive.push_back(positive[i*2+1]);
					}
				}
			}
		}
	}
	if(n_size%2 != 0)
		tmp_negative.push_back(negative[n_size-1]);
	if(p_size%2 != 0)
		tmp_positive.push_back(positive[p_size-1]);

	vector<LinearFunction>().swap(negative);
	vector<LinearFunction>().swap(positive);
	delete []table_n;
	delete []table_p;
	Prune_Search(tmp_negative, tmp_positive, Xleft, Xright);
}
int main(){
	int n_point; double Xleft = -1e7; double Xright = 1e7;
	vector <LinearFunction> zero; vector<LinearFunction> positive; vector<LinearFunction> negative;

	cin >> n_point;
	LinearFunction* data = new LinearFunction[n_point];
	for(int i = 0;i < n_point;i++){

		cin >> data[i].Xcoef >> data[i].Ycoef >> data[i].constant;

		if(data[i].Ycoef == 0)		zero.push_back(data[i]);

		else if(data[i].Ycoef < 0)	negative.push_back(data[i]);

		else		positive.push_back(data[i]);
	} 
	if(Find_X_range(zero, Xleft, Xright) == -1) {
		cout<<"NA" <<endl;
		return 0;
	}
	if(negative.size() == 0 && positive.size() == 0){
		cout << "-INF" <<endl;
		return 0;
	}
	Prune_Search(negative, positive, Xleft, Xright);

	return 0;
}