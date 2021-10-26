#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <cmath>
#include <chrono>
using namespace std;
using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;


double lDistance(pair<double,double> f, pair<double,double> s, pair<double,double> temp){
    return abs((temp.second-f.second)*(s.first-f.first)-(s.second-f.second)*(temp.first-f.first));
}

bool findSide(pair<double,double> f, pair<double,double>s,pair<double,double> temp){
    double d = (temp.second-f.second)*(s.first-f.first)-(s.second-f.second)*(temp.first-f.first);
    if(d>0.000) return true;
    return false;
}

void quickHull(vector<pair<double,double> > input, int f, int s, bool side, vector<int>& answer){
    int pointI = -1;
    double maxD = 0.000;

    for(int i=0;i<input.size();i++){
        double tempD = lDistance(input[f],input[s],input[i]);
        if(findSide(input[f],input[s],input[i])==side&&tempD>maxD){
            pointI=i;
            maxD = tempD;
        }
    }

    if(pointI==-1){
        answer.push_back(f);
        answer.push_back(s);
        return;
    }

    quickHull(input,pointI, f, !findSide(input[pointI],input[f],input[s]),answer);
    quickHull(input,pointI, s, !findSide(input[pointI],input[s],input[f]),answer);
}

vector<int> findHull(vector<pair<double,double> > input){
    vector<int> answer;
    int minX = 0;
    int maxX = 0;
    for(int i=0;i<input.size();i++){
        if(input[i].first<input[minX].first) minX = i;
        if(input[i].first>input[maxX].first) maxX=i;
    }

    quickHull(input, minX, maxX, true, answer);

    quickHull(input, minX, maxX, false, answer);

    return answer;
}

int main(int argc, char* argv[]){
    ifstream myFile(argv[1]);
    if(!myFile.is_open()) throw runtime_error("could not open file");
    string line;
    getline(myFile, line);
    vector<pair<double, double>> cHull;
    int count = stoi(line);
    for(int i=0; i< count; i++){
        getline(myFile,line, ',');
        double x = stod(line);
        getline(myFile,line);
        double y = stod(line);
        cHull.push_back(make_pair(x,y));
    }

    //time_point<Clock> start = Clock::now();

    vector<int> answer= findHull(cHull);

    unordered_set<int> s;
    for(int i=0;i<answer.size();i++){
        if(s.count(answer[i])){
            answer.erase(answer.begin()+i);
            i--;
        }
        else s.insert(answer[i]);
    }

    cout<<answer.size()<<endl;

    pair<double,double> center = make_pair(0.0, 0.0);

    for(int i:answer){
        center.first += cHull[i].first;
        center.second += cHull[i].second;
    }

    center.first /= answer.size();
    center.second /= answer.size();

    sort(answer.begin(), answer.end(), [cHull](int v1, int v2){
        if(cHull[v1].second == cHull[v2].second){
            return cHull[v1].first < cHull[v2].first;
        }
        return cHull[v1].second < cHull[v2].second;
    });

    sort(answer.begin()+1,answer.end(),[cHull, center](int v1, int v2){
        pair<double, double> p1 = cHull[v1];
        pair<double, double> p2 = cHull[v2];
        double a1 = -1 * atan2(p1.first - center.first, p1.second - center.second);
        double a2 = -1 * atan2(p2.first - center.first, p2.second - center.second);

        return a1 < a2;
    });
    /*time_point<Clock> end = Clock::now();
    nanoseconds diff = duration_cast<nanoseconds>(end - start);

    ofstream myfile("output.txt", fstream::app);
    if (myfile.is_open())
    {
        myfile << diff.count() << "," << count << endl;
    }
    else cout << "Unable to open file";*/

    for(int i=0;i<answer.size();i++){
        cout<<answer[i]<<", "<<cHull[answer[i]].first<<", "<<cHull[answer[i]].second<<endl;
    }
    return 0;
}
