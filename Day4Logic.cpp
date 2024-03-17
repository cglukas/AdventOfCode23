#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>

using namespace std;


set<string> getIndividualNumbers(string numbers);

int main(int argc, char** argv){
    
    if (argc != 2){
        cout << "Please provide input file as argument.\n";
        return 1;
    }
    ifstream text(argv[1]);

    double all_points = 0;
    if(!text.is_open()){
        cout << "Can not open file.\n";
        return 1;
    }

    for(string line; getline(text, line);){
        size_t start = line.find(':');
        size_t middle = line.find('|');
        string winningNumbers = line.substr(start+1, middle-start-1 );
        string myNumbers = line.substr(middle+1, line.size() - middle);

        cout << "Winning: >" << winningNumbers << "<\n";
        cout << "Mine: >" << myNumbers << "<\n";
        set<string> winning = getIndividualNumbers(winningNumbers);
        set<string> my = getIndividualNumbers(myNumbers);

        cout << "WinningSet: " << *winning.begin() << "\n";
        cout << "MySet: " << *my.begin() << "\n";
        
        vector<string> intersection;
        set_intersection(winning.begin(), winning.end(), my.begin(), my.end(), back_inserter(intersection));
        if(intersection.size() > 0){
            all_points += pow(2, intersection.size()-1);
        }

    }
    cout << "The final result is: " << all_points << "\n";
    return 0;
}

set<string> getIndividualNumbers(string numbers)
{
    stringstream individuals(numbers);
    string num;
    set<string> result;
    while (individuals >> num){
        result.insert(num);
    }
    return result;
}
