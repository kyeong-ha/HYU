#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

vector<string> StringTokenize(const string& s, const char delimiter = ' ') {
    vector<string> result;
    string token;
    stringstream ss(s);

    while(getline(ss, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}
int main(){
    string input = "";
    int i = 0;

    getline(cin, input);
    vector<string> data = StringTokenize(input, ' ');
    int min_add = atoi(data[0].c_str())+atoi(data[1].c_str());
    
    for(vector<string>::size_type j=0; j<data.size()-1; j++) { 
        if(min_add > atoi(data[j].c_str())+atoi(data[j+1].c_str())){
            min_add = atoi(data[j].c_str())+atoi(data[j+1].c_str());
            i = j;
        }
    }

    for(vector<string>::size_type j=0; j<data.size()-2; j++) {
        if(&data[i] == &data[0]) {
            
        }
    }

    cout << min_add;

    return 0;
}