#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <algorithm>


using namespace std;

typedef vector<pair<string, string> > ATT;

struct Node {
    vector<ATT> data;
    vector<Node*> child;
    string label = ""; // 현재 노드에서 분기할 수 없을 때 정해진 최종 label
    string test_att = ""; //분리기준 test_attribute의 이름
    string test_att_value = ""; //test_attribute의 값은 무엇이었는지
};

Node* root = NULL;
Node* current_node;
Node* parent_node;
vector<Node*> next_node;
string label_name = "";
vector<string> result_label; //결정된 label을 담을 공간

Node* CreateNode(vector<ATT> data, vector<Node*>* child, string test_att_value, string test_att_name) {

    Node* node = new Node;
    if (node != NULL) {
        node->data = data;
        node->test_att = test_att_name;
        node->test_att_value = test_att_value;
        if (child != NULL) {
            for (int i = 0; i < child->size(); i++) {
                (node->child).push_back((*child)[i]);
            }
        }

    }
    return node;
}

struct isEmpty {
    isEmpty(string const& s) : _s(s) {}

    bool operator() (pair<string, string> const& p) {
        return p.first == _s;
    }

    string _s;
};
struct comp
{
    comp(std::string const& s) : _s(s) { }

    bool operator () (std::pair<std::string, string> const& p)
    {
        return (p.second == _s);
    }

    std::string _s;
};

vector<string> UniqueData(vector<string> data) { //해당 att의 값들을 unique로 만들어 리턴함

        sort(data.begin(), data.end());
        data.erase(unique(data.begin(), data.end()), data.end());
        
        return data;

}

vector<ATT> SplitData(int test_att_idx, string unique_test_att_data, vector<ATT> data) {
    vector<ATT> sub_data;

    /*unique_test_att_data의 값에 따라 data를 unique_test_att_data의 size만큼 가지치기. */
   //unique_test_att_data에 따라 data을 나눠 넣음       

    ATT split_data;
    for (int i = 0; i < data.size(); i++) {
        if ((data[i][test_att_idx].second).compare(unique_test_att_data) == 0) { //test_att를 unique 값에 따라 분기시킴.

            data[i].erase(data[i].begin() + test_att_idx);
            sub_data.push_back(data[i]); //unique와 일치할 때 split해서 넣어줌.
        }
    }
    return sub_data;
}

double GetEntropy(vector<string> data) { //data: 해당 att에 있는 모든 값들.


    int cnt, element_cnt = 0;
    double entropy = 0;

    //해당 집합을 unique로 만든 뒤, 그 개수를 저장함.
    vector<string> unique_data = UniqueData(data);
    cnt = unique_data.size();

    /*엔트로피  계산*/
    for (int i = 0; i < cnt; i++) {
        element_cnt = count(data.begin(), data.end(), unique_data[i]);
        entropy += -(double(element_cnt) / double(data.size()) * log2(double(element_cnt) / double(data.size())));
    }

    return entropy;
}

vector<double> GetInfoGain(vector<vector<string> > data, vector<string> label_data, vector<string> split_att_name, string label) {
    vector<double> result;
    double temp = 0;
    vector<double> weighted_entropy;
    vector<vector<string> > unique_att_data = data;

    /*전체 엔트로피 계산*/
    double total_entropy = GetEntropy(label_data);


    /*가중 엔트로피 계산*/

     //label unique를 구해줌
    vector<string> unique_label = UniqueData(label_data);

    for (int i = 0; i < split_att_name.size(); i++) {

        //각 attribute에 대해 unique를 구함
        sort(unique_att_data[i].begin(), unique_att_data[i].end());
        unique_att_data[i].erase(unique(unique_att_data[i].begin(), unique_att_data[i].end()), unique_att_data[i].end());


        /*unique_att_data의 값에 따라 data를 unique_att_data의 size만큼 가지치기. */
        vector<vector<string> > temp_data; //unique_att_data에 따라 lable을 나눠 넣음       

        for (int j = 0; j < unique_att_data[i].size(); j++) {
            vector<string> split_data;
            vector<string>::iterator iter_label = label_data.begin();
            for (vector<string>::iterator iter = data[i].begin(); iter != data[i].end(); iter++) {

                if ((*iter).compare(unique_att_data[i][j]) == 0) {
                    split_data.push_back(*iter_label); //unique와 일치할 때의 label값을 넣어줌.
                }
                iter_label++;
            }

            temp_data.push_back(split_data);
        }

        for (int j = 0; j < temp_data.size(); j++) { //2
            for (int k = 0; k < unique_label.size(); k++) {
                double element_cnt = count(temp_data[j].begin(), temp_data[j].end(), unique_label[k]);
                temp += double(element_cnt) / double(data[i].size()) * GetEntropy(temp_data[j]);
            }
        }

        weighted_entropy.push_back(temp);
        result.push_back(total_entropy - weighted_entropy[i]); //InfoGain 저장
        temp = 0;

    }


    return result;
}

bool classify(Node* tree, ATT input_data) {
    Node* temp_tree = tree;
    ATT temp_input_data = input_data;

    if (temp_tree->label != "") { //더이상 내려갈 att가 없다면 부모노드의 label값으로 저장

        result_label.push_back(temp_tree->label);
        return 0;
    }
    else {
        for (int i = 0; i < temp_input_data.size(); i++) {
            //tree와 input_data의 att 비교
            if (temp_tree->child[0]->test_att == temp_input_data[i].first) { //자식의 test_att와 들어온 data의 att가 같다면 그것을 기준으로 값 비교.
                for (int j = 0; j < temp_tree->child.size(); j++) { //input_data의 att값이 일치하는 자식노드로 가서 최종 label이 나올 때까지 반복 
                    if (temp_tree->child[j]->test_att_value == temp_input_data[i].second)
                    {
                        temp_input_data.erase(temp_input_data.begin() + i);
                        classify(temp_tree->child[j], temp_input_data);
                        return 0;
                    }
                }
            }
        }
        //현재 node의 정해진 label은 없고, 정보가 저장되지 않은 새로운 데이터임
        // 현재 노드에서 같은 att의 다수결로 label값을 정함
        vector<string> current_label;
        vector<string> unique_current_label = UniqueData(current_label);

        //각 label이 얼마나 있는지 카운트.
        vector<int> element_cnt;
        for (int i = 0; i < unique_current_label.size(); i++) {
            element_cnt.push_back(count(current_label.begin(), current_label.end(), unique_current_label[i]));
        }
        //parent label 중 카운트가 가장 많은 수의 인덱스로 리턴
        result_label.push_back(unique_current_label[max_element(element_cnt.begin(), element_cnt.end()) - element_cnt.begin()]);
        return 0;

    }


}

bool DecisionTree(vector<ATT>data, vector<string> att, string parent_label) {
    vector<vector<string> > datas_of_att; //att_name별로 데이터를 새로 저장
    vector<string> data_of_label;

    vector<string> att_name;
    vector<double> gain;

    //1. 종료 : 데이터가 존재하지 않을 때 = 원본데이터에서 최대값을 가지는 label 반환
    if (data.size() == 0) {
        current_node->label = parent_label;
        return 0;
    }

    for (int i = 0; i < att.size(); i++) {
        vector<string> temp_data;

        for (int j = 0; j < data.size(); j++) {
            for (int k = 0; k < data[j].size(); k++) {
                if (data[j][k].first == att[i]) {
                    temp_data.push_back(data[j][k].second);
                }
            }
        }

        if (i == att.size() - 1)//label 분리
            data_of_label = temp_data;
        else
            datas_of_att.push_back(temp_data);
    }
    //att_name으로부터 label_name 분리
    label_name = att[att.size() - 1];

    for (int i = 0; i < att.size() - 1; i++) { //att_name만 따로 저장
        att_name.push_back(att[i]);
    }

    vector<string> temp = UniqueData(data_of_label);


    //2. 종료 : 대상 label이 모두 같을 때 = data_of_label 의 unique 데이터의 사이즈가 1일 때
    if (temp.size() == 1) {
        current_node->label = temp[0];//해당 label 반환
        return 0;
    }
    //3. 종료 : 더이상 분기할 test_attribute가 존재하지 않을 때
    else if (att_name.size() == 0) {
        current_node->label = parent_label;       
        return 0;
    }


    //tree 생성
    else {
        //부모 label 정의
        //각 label이 얼마나 있는지 카운트.
        vector<int> element_cnt;
        for (int i = 0; i < temp.size(); i++) {
            element_cnt.push_back(count(data_of_label.begin(), data_of_label.end(), temp[i]));
        }
        //parent label 중 카운트가 가장 많은 수의 인덱스로 리턴함
        parent_label = temp[max_element(element_cnt.begin(), element_cnt.end()) - element_cnt.begin()];

        /* step2. test_attribute 선정하기 */
        gain = GetInfoGain(datas_of_att, data_of_label, att_name, label_name);

        //gain의 최댓값을 가진 attribute를 test_attribute로 선정
        int test_attribute_idx = max_element(gain.begin(), gain.end()) - gain.begin();
        string test_attribute = att_name[test_attribute_idx];


        //test_attribute의 unique개수만큼 자식노드 생성
        vector<Node*> child_node;
        vector<string> unique_test_attribute = UniqueData(datas_of_att[test_attribute_idx]);

        for (int i = 0; i < unique_test_attribute.size(); i++) {
            Node* sub_child = CreateNode(SplitData(test_attribute_idx, unique_test_attribute[i], data), NULL, unique_test_attribute[i], att_name[test_attribute_idx]);
            child_node.push_back(sub_child);
        }

        //tree 생성
        Node* node;
        att.erase(att.begin() + test_attribute_idx); //test_attribute를 후보에서 제거


        if (root == NULL) { //재귀함수 1회차라면, 현재 노드를 root로 지정
            node = CreateNode(data, &child_node, "", "");
            root = node;
            current_node = node;
        }
        else {
            node = CreateNode(data, &child_node, current_node->test_att_value, current_node->test_att);
            current_node->data = node->data;
            current_node->child = node->child;
            current_node->label = node->label;
            current_node->test_att = node->test_att;
            current_node->test_att_value = node->test_att_value;
        }


        //재귀
        for (int i = 0; i < unique_test_attribute.size(); i++) {
            vector<ATT> sub_data = SplitData(test_attribute_idx, unique_test_attribute[i], data);

            current_node = node->child[i]; //다음으로 넘길 노드
            DecisionTree(sub_data, att, parent_label);
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    vector<ATT> data_set; // line 단위로 저장
    vector<string> att_name;
    vector<vector<string> > datas_of_att; //att_name별로 데이터를 새로 저장
    vector<string> data_of_label;


    int i = 0;
    int j = 0;

    /* 문자열 4개가 잘 전달 되었는지 확인 */
    if (argc != 4) {
        return 1;
    }

    /* 데이터를 읽기 위한 스트림 생성 */
    ifstream training_fin(argv[1]);
    ifstream test_fin(argv[2]);
    ofstream fout(argv[3]);

    if (!training_fin || !test_fin || !fout) {
        cout << "Error!" << endl;
        return 1;
    }

    /* 파일에 있는 데이터를 한 줄씩 읽어 1번째 줄은 name으로 저장하고, 나머지는 DataSet에 초기화함 */
    string line = "";

    while (!training_fin.eof()) {
        string temp;
        ATT temp_att;
        getline(training_fin, line);

        if (line == "")
            break;

        for (stringstream stream(line); (stream >> temp);) {
            if (i == 0) { //1번째 줄인 경우 name으로 저장
                att_name.push_back(temp);
            }
            else {
                temp_att.push_back(make_pair(att_name[j], temp)); //나머지 줄은 해당하는 attribute name의 vlaue로 넣음
                j++;
            }
        }
        if (i != 0)
            data_set.push_back(temp_att);
  
        i++;
        j = 0;
    }
    //여기까지 data_set에 모든 데이터를 넣었음. 2차원 벡터방식. data_set[n][4] = classlabe임.

    /* Tree 구축 */
    DecisionTree(data_set, att_name, "");
    
    vector<string> test_att_name;
    vector<ATT> test_data_set;
    i = 0; j = 0;
    /* 예측하기 */
    while (!test_fin.eof()) {
        line = "";
        string temp;

        ATT temp_att;


        getline(test_fin, line);
        stringstream stream(line);
        if (line == "")
            break;
        while (stream >> temp) {
            if (i
                == 0) { //1번째 줄인 경우 name으로 저장
                test_att_name.push_back(temp);
            }
            else {
                temp_att.push_back(make_pair(test_att_name[j], temp)); //나머지 줄은 해당하는 attribute name의 vlaue로 넣음
                j++;
            }
        }
        if (i != 0)
            test_data_set.push_back(temp_att);

        i++; j = 0;
    }

    for (int i = 0; i < test_data_set.size(); i++) {
        classify(root, test_data_set[i]);
        test_data_set[i].push_back(make_pair(label_name, result_label[i]));
    }
    test_att_name.push_back(label_name);

    //결정된 label을 파일에 써서 출력함
    if (fout.is_open()) {
        //test_att_name을 나열
        for (int i = 0; i < test_att_name.size(); i++) {
            if (i == test_att_name.size() - 1)
                fout << test_att_name[i];
            else
                fout << test_att_name[i] << "\t";
        }
        fout << endl;

        for (int i = 0; i < test_data_set.size(); i++) {
            for (int j = 0; j < test_data_set[i].size(); j++) {
                if (j == test_data_set[i].size() - 1)
                    fout << test_data_set[i][j].second;
                else
                    fout << test_data_set[i][j].second << "\t";
            }
            fout << endl;
        }

        fout.close();
    }
    training_fin.close();
    test_fin.close();


    return 0;
}