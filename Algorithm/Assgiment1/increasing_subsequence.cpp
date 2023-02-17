#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {

    vector<char> sequence;
    string input = "";
    string output = "";

    cin >> input;
    output.push_back(input.at(0));

    for(int i=1; i<input.length(); i++){
        if( output.back() < input.at(i)) {
            output.push_back(input.at(i));
        }else{
            string::iterator it = lower_bound(output.begin(), output.end(),input.at(i));
            *it = input.at(i);
        }

    }
    cout << output << endl;

    return 0;
}