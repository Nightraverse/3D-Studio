#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


// -- Stage 1: Set-up -----------------------------
struct faceVertex{
	// -1 is an initial value to indicate that the vertex has not been set yet 
	int v = -1;
	int vt = -1;
	int vn = -1;
};

// Holds all the data for a 3D model, including vertices, face groups, and triangles
struct Model{
	std::vector<float> vertices;
	std::vector<std::vector<faceVertex>> faceGroups;
	std::vector<faceVertex> triangles;
};

// Function declarations
int loadObj(Model& model);
std::vector<faceVertex> triangulate(const std::vector<std::vector<faceVertex>>& faceGroups);
int test(const Model& model);
void scaleToUnitCube(std::vector<float>& vertices);

int main(){
	Model model;
	loadObj(model);
	test(model);
	return 0;
}
    


int loadObj(Model& model){
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
			model.vertices.push_back(x);
			model.vertices.push_back(y);
			model.vertices.push_back(z);
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

		model.faceGroups.push_back(face);          // ← whole face goes into the outer vector
		}
	
	}
	
	readFile.close(); 
	scaleToUnitCube(model.vertices);
	model.triangles = triangulate(model.faceGroups);
	return 0;
}


// -- Stage 3: Triangulate Faces ---------------------------
std::vector<faceVertex> triangulate(const std::vector<std::vector<faceVertex>>& faceGroups) {
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
	return triangles;
}

// -- Stage 4: Output Parsed File --------------------------
int test(const Model& model){
	std::cout << "Vertices: " << model.vertices.size() / 3 << "\n";
	std::cout << "Faces: " << model.faceGroups.size() << "\n";
	std::cout << "Triangles: " << model.triangles.size() / 3 << "\n";
	return 0;
}

// -- Stage 5: Scale to Unit Cube --------------------------
void scaleToUnitCube(std::vector<float>& vertices){
	if(vertices.empty()) return;
	float minX, minY, minZ, maxX, maxY, maxZ;

	minX = maxX = vertices[0];
	minY = maxY = vertices[1];
	minZ = maxZ = vertices[2];

	for (size_t i = 3; i < vertices.size(); i += 3) {
		if (vertices[i] < minX) minX = vertices[i];
		if (vertices[i] > maxX) maxX = vertices[i];
		if (vertices[i + 1] < minY) minY = vertices[i + 1];
		if (vertices[i + 1] > maxY) maxY = vertices[i + 1];
		if (vertices[i + 2] < minZ) minZ = vertices[i + 2];
		if (vertices[i + 2] > maxZ) maxZ = vertices[i + 2];
	}

	float extentX = maxX - minX;
	float extentY = maxY - minY;
	float extentZ = maxZ - minZ;

	float maxScale = std::max({ extentX, extentY, extentZ });
	if (maxScale == 0) return;

	float cx = (minX + maxX) / 2.0f;
	float cy = (minY + maxY) / 2.0f;
	float cz = (minZ + maxZ) / 2.0f;

	for (size_t i = 0; i < vertices.size(); i += 3) {
		vertices[i] = (vertices[i] - cx) / maxScale;
		vertices[i + 1] = (vertices[i + 1] - cy) / maxScale;
		vertices[i + 2] = (vertices[i + 2] - cz) / maxScale;
	}
}
