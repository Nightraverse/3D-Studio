#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(){
    
	std::vector<float> vertices;
	std::vector<int> faces;

	string text; // current line for the file reader
	std::ifstream readFile("cube.obj");

	if(!readFile){
	  std::cerr << "File not found ." 
	  return 1;
	}
	
	
	// while theres still more lines to read in the file
	while(getline(readFile, text)){
		if(text.front() == '\0') return;
		
		if(text..substr(0, 2) == "vn"{
			normals.push_back(text);
		}
		
		else if(text.front() == 'v'){
			vertices.push_back(text);
		}
		
		else{
			faces.push_back(text);
		}
	}
	
readFile.close(); 

}
