#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


struct faceVertex{
	// -1 is an initial value to indicate that the vertex has not been set yet 
	int v = -1;
	int vt = -1;
	int vn = -1;
};

int main(){
    // -- Stage 1: Set-up -----------------------------
	std::vector<float> vertices;
	std::vector<std::vector<faceVertex>> faceGroups; // Vector to hold groups of faces
	

	// -- Stage 2: Parse File --------------------------
	std::ifstream readFile("cube.obj");
	if(!readFile){
        std::cerr << "File not found." << std::endl; 
        return 1;
	}
	
	// current line for the file reader
	std::string currentLine; 
	
	// while theres still more lines to read in the file
	while(getline(readFile, currentLine)){
        // creates a string stream to read the current line
		std::istringstream ss(currentLine);

		// tag is the first word in the line, which tells us what type of line this is
        std::string tag;
        
        // reads the type of line this is then sends to the matching if statement to be processed
		ss >> tag; 

		if(tag == "v"){
			float x, y, z;
			ss >> x >> y >> z;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}


	else if(tag == "f"){
		// one separate face variable for every f line
		std::vector<faceVertex> face;
		
		// tokenize the rest of the line to get each vertex index
		std::string token;
		
		// while there are still tokens to read in the line
		while(ss >> token){
			// create a string stream for the token to split it by '/'
			std::istringstream tokenStream(token);

			// string to hold the index as a string before converting to int
			std::string indexString;

			// create a faceVertex to hold the indices for this vertex
			faceVertex fv;
			
			// vertex index case (always present, so getline is always successful)
			if(std::getline(tokenStream, indexString, '/')){
				// convert string to int, suctract 1 (1-based indexing in .obj files) 
				// and store in the corresponding variable faceVertex struct
				fv.v = std::stoi(indexString) - 1;
			}
			// vertex texture case (may be empty, so check if getline was successful)
			if(std::getline(tokenStream, indexString, '/')){
				// same as above but also checks in case index is empty (e.g., "1//3" case)
				if(!indexString.empty()) fv.vt = std::stoi(indexString) - 1;
			}
			// vertex normal case (may be empty, so check if getline was successful)
			if(std::getline(tokenStream, indexString, '/')){
				// same as vertex texture
				if(!indexString.empty()) fv.vn = std::stoi(indexString) - 1;
			}

			face.push_back(fv);         // ← into the inner vector
		}

		faceGroups.push_back(face);          // ← whole face goes into the outer vector
	}

	/* Not needed for now, kept as a stub for future implementation
	else if(tag == "vn"){}
	else if(tag == "vt"){	*/
	
}
	
	readFile.close(); 


	// -- Stage 3: Triangulate Faces ---------------------------

	// Flat list of triangles, each triangle is represented by 3 faceVertex structs
	std::vector<faceVertex> triangles;

	// Loop through each face group and triangulate it
	for (const auto& face : faceGroups) {
		// Fan method of triangulation 
		for (size_t i = 2; i < face.size(); i++) {
			triangles.push_back(face[0]);
			triangles.push_back(face[i - 1]);
			triangles.push_back(face[i]);
		}
	}

	// -- Stage 4: Output Parsed File --------------------------
	std::cout << "Vertices: " << vertices.size() / 3 << "\n";
	std::cout << "Faces: " << faceGroups.size() << "\n";
	std::cout << "Triangles: " << triangles.size() / 3 << "\n";

	return 0;
}
