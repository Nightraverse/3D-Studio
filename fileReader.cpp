#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(){
    
	std::vector<float> vertices;
	std::vector<int> faces;

	std::ifstream readFile("cube.obj");
	if(!readFile){
	  std::cerr << "File not found." << std::endl; 
	  return 1;
	}
	
	// current line for the file reader
	string currentLine; 
	
	// while theres still more lines to read in the file
	while(getline(readFile, currentLine)){
		std::istringstream ss(currentLine);
		std::string tag;
		ss >> tag;
		
		if(tag == "v"{
			float x, y, z;
			ss >> x >> y >> z;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		
		else if(tag == "f"){
			// work in progress
		}
		
		else if(tag == "vn"){
			// work in progress
		}


		else if(tag == "vt"){
			// work in progress
		}
		
		else if(tag == "#"){
			// work in progress, and two more for o and usemtl  
		}
	}
	
readFile.close(); 

}
