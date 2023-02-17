#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <algorithm>
#include <time.h>


using namespace std;

struct object {
    int object_id;
    double x_coordinate;
    double y_coordinate;
};

bool cmp(object a, object b) {
    return  a.object_id < b.object_id;
}

vector<object> data_set; //input file에서 받은 data를 모두 저장
vector<object> cluster;
vector<object> used_data;

void GetPoint(int *p){
   /* data_set에서 랜덤으로 포인트값 뽑기 */ 

    srand(time(NULL));
    bool isExit = true;

    /* 이미 클러스터에 있는 포인터값이라면, 다시 난수추출 */
    while(isExit) {
        *p = rand() % data_set.size(); //0~data_set.size 의 범위에서 난수 생성

        if (cluster.size() == 0)
            isExit = false;
        else {
            isExit = false;
            for(vector<object>::iterator it = used_data.begin(); it!= used_data.end(); it++) {
                
                if(*p == it->object_id) {
                    isExit = true;
                    break;
                }
            }
        }
    }
}

void CheckDDR(int p, int eps, int minpts){
   int neighborhood_n = 0;
    vector<object> temp_cluster;

    for(vector<object>::iterator it = data_set.begin(); it!=data_set.end(); ++it) {
        if((it->x_coordinate >= data_set[p].x_coordinate - eps) && (it->x_coordinate <= data_set[p].x_coordinate + eps)) {
            if((it->y_coordinate >= data_set[p].y_coordinate - eps) && (it->y_coordinate <= data_set[p].y_coordinate + eps)) {
                neighborhood_n++;
                temp_cluster.push_back(*it);
            }
        }
    }
    
    //기존의 클러스터 뒤에 삽입
    
    vector<int> erase_num;

     for(vector<object>::iterator it = temp_cluster.begin(); it!=temp_cluster.end(); ++it) {
            for(vector<object>::iterator it_ = cluster.begin(); it_!=cluster.end(); ++it_) {
                if(it->object_id == it_->object_id) {
                    erase_num.push_back(it->object_id);
                }   
            }
     }
     for(int i=0; i<erase_num.size(); i++) {
         for(vector<object>::iterator it = temp_cluster.begin(); it!=temp_cluster.end(); ++it) {
             if(it->object_id == erase_num[i]) {
                 temp_cluster.erase(it);
                 break;
             }
         }
     }

    if(temp_cluster.size() != 0){
        cluster.insert(cluster.end(), temp_cluster.begin(), temp_cluster.end());
        used_data.insert(used_data.end(), temp_cluster.begin(), temp_cluster.end());


        if(neighborhood_n >= minpts){ //DDR 조건성립
            for(vector<object>::iterator it = temp_cluster.begin(); it!=temp_cluster.end(); ++it) {
                CheckDDR(it->object_id, eps, minpts);
            }   
        }
     }
}

void GetCluster(int p, int eps, int minpts){
    /* point가 core-point 조건을 만족하는지 확인 */
    //현재 p의 좌표에서 Eps값을 +- 한 내역 안에 있는 모든 점들의 개수를 구함. 그 개수가  minPts 이상이어야함.

    cluster.clear();

    int neighborhood_n = 0;
    vector<object> temp_cluster;
 

    for(vector< object>::iterator it = data_set.begin(); it!=data_set.end(); ++it) {
        if((it->x_coordinate >= data_set[p].x_coordinate - eps) && (it->x_coordinate <= data_set[p].x_coordinate + eps)) {
            if((it->y_coordinate >= data_set[p].y_coordinate - eps) && (it->y_coordinate <= data_set[p].y_coordinate + eps)) {
                neighborhood_n++;
                cluster.push_back(*it);
                used_data.push_back(*it);
            }
        }
    }

    if(neighborhood_n >= minpts) { //core-point 조건성립
        //temp_cluster 내에 있는 object도 DDR을 체크하여 cluster을 확장해나감
        int size = cluster.size();
        for(int i=0; i<size; i++)
            CheckDDR(cluster[i].object_id, eps, minpts);
    }

    //중복제거
    
}


int main(int argc, char** argv){ 

    int point; //랜덤하게 찍을 포인트 변수
    string filename(argv[1]);
    filename.erase(filename.size()-4, filename.size());

    /* 문자열 4개가 잘 전달 되었는지 확인 */
    if (argc != 5) {
        return 1;
    }

    /* 데이터를 읽기 위한 스트림 생성 */
    ifstream input_fin;
    input_fin.open(argv[1]);
    int n = atoi(argv[2]);
    int Eps = atoi(argv[3]);
    int MinPts = atoi(argv[4]);

    if (!input_fin) {
        cout << "Error!" << endl;
        return 1;
    }

    string line = "";
    
    /* input data를 한 줄씩 읽어 각각 data_set에 저장*/

    while(!input_fin.eof()) {
        object temp_object;
        getline(input_fin, line);

        if(line == "") 
            break;
        
        stringstream stream(line);
        stream >> temp_object.object_id >> temp_object.x_coordinate >> temp_object.y_coordinate;

        data_set.push_back(temp_object);
    }
    input_fin.close();

    for(int i=0; i<n; i++) {

        GetPoint(&point);
        GetCluster(point, Eps, MinPts);
        
        /* 클러스터 파일 출력 */
        sort(cluster.begin(), cluster.end(), cmp);
        ofstream output_fin(filename + "_cluster_" + to_string(i) + ".txt");

        if (!output_fin) {
            cout << "Error!" << endl;
            return 1;
        }

        if (output_fin.is_open()) {
            for(vector<object>::iterator it=cluster.begin(); it!=cluster.end(); it++){
                output_fin << it->object_id << endl;
            }

        }

        output_fin.close();
    }

    
    return 0;
}