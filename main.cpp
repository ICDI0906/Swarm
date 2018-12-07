//
// Created by 张开顺 on 2018/12/7.
//

#include <iostream>
#include <vector>
#include <cstdio>
#include <set>
using namespace std;

set<int>DB[100][100]; // i - > time_id j - > cluster_id
int cluster_num[100];   //  i - > time_id value - > number of cluster
int object,timestamp;
int min_o = 2, min_t = 2;
void input(){
    cin >> object >> timestamp;
    for(int i = 0;i < timestamp; i++){
        int t_id,cluster_cnt,m;
        cin >> t_id >> cluster_cnt;
        cluster_num[t_id] = cluster_cnt;
        cin >> m;
        for(int j = 0 ;j < m; j++){
            int o_id,cluster_id;
            cin >> o_id >> cluster_id;
            DB[t_id][cluster_id].insert(o_id);
        }
    }
    /*
    cout<< "debug : "<<endl;
    for(int i = 0;i < timestamp;i++){
        for(int j = 0;j< cluster_num[i];j++){
            for(auto p:DB[i][j]){
                cout<< "i: "<< i << "j : " << j <<" o : "<< p <<endl;
            }

        }
    }*/
}
// object in t include cluster
vector<int> C_t_o(int o, int t){
    vector<int>result;
    for(int i=0; i< cluster_num[t];i++){
        if(DB[t][i].find(o) != DB[t][i].end()){
            result.push_back(i);
        }
    }
    return result;
}

bool BackwardPruning(int last_o,set<int> o,set<int>T_max){
    vector<int>::iterator iter;
    for(int i = 0;i < object; i++){
        if(o.find(i) == o.end() && i < last_o){ //
            vector<int>last_o_t,o_t;
            for(auto t : T_max){
                last_o_t.clear(),o_t.clear();
                last_o_t = C_t_o(i, t);
                vector<int>tmp;
                for(auto o_tmp:o){
                    tmp = C_t_o(o_tmp, t);
                    if(o_t.size() == 0){
                        o_t = tmp;
                    }
                    else{
                        vector<int>set_inter(tmp.size() + o_t.size());
                        iter = set_intersection(tmp.begin(),tmp.end(),o_t.begin(),o_t.end(),set_inter.begin()); // set_intersection
                        set_inter.resize(iter - set_inter.begin());
                        o_t = set_inter;
                    }
                }
                tmp.clear();
                vector<int>set_uni(last_o_t.size() + o_t.size());
                iter = set_union(last_o_t.begin(),last_o_t.end(),o_t.begin(),o_t.end(),set_uni.begin()); // C_t(last_o) <= C_t(o) return false
                set_uni.resize(iter - set_uni.begin());
                if(set_uni.size() != o_t.size()){
                    return true;
                }
            }
            return false;
        }
    }
    return true;
}
set<int> GenerateMaxTimeset(int o,int last_o,set<int> T_max){
    set<int>result;
    for(auto t: T_max){
        if(last_o == -1){
            vector<int> tmp = C_t_o(o,t);
            if(!tmp.empty()){
                result.insert(t);
            }
        }
        else{
            vector<int> tmp1 =  C_t_o(o,t);
            vector<int> tmp2 =  C_t_o(last_o,t);
            std::vector<int>::iterator it;
            it = set_intersection(tmp1.begin(),tmp1.end(),tmp2.begin(),tmp2.end(),tmp1.begin());
            tmp1.resize(it-tmp1.begin());
            if(!tmp1.empty()){
                result.insert(t);

            }
        }
    }
    return result;
}
void objectGrowth(set<int> O, set<int> T_max,int last_o){
    if(T_max.size() < min_t){
        return; // apriori pruning
    }
    if(BackwardPruning(last_o,O,T_max)){
        bool forward_closure = true;
        for(int add_o = last_o + 1;add_o < object; add_o ++){
            O.insert(add_o);
            set<int> T_max_tmp = GenerateMaxTimeset(add_o,last_o,T_max);
            if(T_max_tmp.size() == T_max.size()){
                forward_closure = false;
            }

            objectGrowth(O,T_max_tmp,add_o);
            O.erase(add_o);
        }
        if(forward_closure && O.size() >= min_o){
            cout << "answer: "<<endl;
            cout<< "  object: ";
            for(auto o_tmp : O){
                cout<< o_tmp + 1 << " ";
            }
            cout<< endl;
            cout<< "time: ";
            for(auto t_tmp : T_max){
                cout<< t_tmp + 1<< " ";
            }
            cout<< endl;
        }
    }

}
int main(){
    freopen("data.in","r",stdin);
    input();
    set<int> T_max;
    for(int i = 0; i < timestamp; i++){
        T_max.insert(i);
    }
    set<int>O;
    objectGrowth(O,T_max,-1);
    return 0;
}