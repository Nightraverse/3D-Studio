#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(){

    vector<double> vertices;
    vector<double> normals;
    vector<double> faces;

    string text;
    ifstream readFile("cube.obj");

    while(getline(readFile, text)){
        if(text.front() == 'v'){
            vertices.push_back(text);
        }
    }

    readFile.close(); 
}
