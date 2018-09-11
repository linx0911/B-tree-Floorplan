#include <iostream>
#include <vector>
using namespace std;

class Net{
public:
    Net();
    Net(int, vector<string>);
    vector<string> connection;
    int degree;
};
