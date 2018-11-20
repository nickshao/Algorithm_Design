#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#define MAX 110
int length[MAX][MAX];
//int prev_move[MAX][MAX];
string lcs_1, lcs_2;
vector<string> final_ans;
void print_LCS(int i, int j, string tmp_str, int flag);
void cal_lcs_length(){    
    //int length[MAX][MAX] = {{0}};
    //int prev[MAX][MAX] = {{0}};
    int l_1 = lcs_1.size();
    int l_2 = lcs_2.size();
    for(int i= 1;i <= l_1; i++){
        for(int j = 1;j <= l_2; j++){
            if(lcs_1[i-1] == lcs_2[j-1]){
                length[i][j] = length[i-1][j-1] +1;
                //prev_move[i][j] = 0;
            }
            else{
                length[i][j] = max(length[i-1][j], length[i][j-1]); 
            }
        }
    }
    //cout<< "final result:" <<length[l_1][l_2]<<endl;
    print_LCS(l_1, l_2, "" , 1);
}
void print_LCS(int i,int j, string tmp_str, int flag){
    //cout << lcs_1[i-1] <<lcs_2[j-1] <<endl;
    if(length[i][j] == 0){
        //reverse(tmp_str.begin(), tmp_str.end());
        final_ans.push_back(tmp_str);
        //cout << "result:" << tmp_str <<endl;
        return;
    }
    if(length[i][j] == length[i][j-1] && length[i][j] == length[i-1][j]){
        if(flag == 1)
            print_LCS(i, j-1, tmp_str, flag);
        flag = 0;
        print_LCS(i-1, j, tmp_str, flag);
    }
    else if(length[i][j] == length[i][j-1]){
        //cout << "left" <<endl;
        //if(!flag)
        //    return;
        if(flag == 1)
            print_LCS(i, j-1, tmp_str, flag);
    }
    else if(length[i][j] == length[i-1][j]) {
        //cout << "top" <<endl;
        print_LCS(i-1,j, tmp_str, flag);
    }
    if(lcs_1[i-1] == lcs_2[j-1]){
        tmp_str.insert(tmp_str.begin(), lcs_1[i-1]);
        flag = 1;
        print_LCS(i-1, j-1, tmp_str, flag);
    }
}
int main(){
    cin >> lcs_1 >> lcs_2;
    //cout << "lcs_1 length:" << lcs_1.size() << endl;
    //cout << "lcs_2 length:" << lcs_2.size() << endl;
    cal_lcs_length();
    //cout << final_ans[0] <<endl;
    sort(final_ans.begin(), final_ans.end());
    //cout << final_ans[0] <<endl;
    cout << length[lcs_1.size()][lcs_2.size()] << " " <<final_ans.size() <<'\n';
    for(int i = 0;i < final_ans.size();i++)
        cout << final_ans[i] <<'\n';
    return 0;
}
