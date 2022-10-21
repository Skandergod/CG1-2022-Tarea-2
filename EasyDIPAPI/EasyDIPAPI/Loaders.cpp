#include "Loaders.h"
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <math.h>

#define UPDATE_BB(xmax, xmin, x) if (x > xmax) {\
	xmax = x;\
}\
if (x < xmin) {\
	xmin = x;\
}\


namespace CG
{ 
	void Load(const std::string path, Object *a)
	{
		//if path termina en .obj
		LoadOff(path, a);
		//else if path termina en .off
	}

	void LoadObj(const std::string path, Object* a) {
		std::ifstream ifs;
		std::regex decimal("^[-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?$");
		std::regex case1("^[0-9]+[/]+[0-9]+$");
		std::regex case2("^[0-9]+[/]+[0-9]+[/]+[0-9]+$");
		std::regex case3("^[0-9]+[/][/]+[0-9]+$");
		std::regex integer("^[+-]?[0-9]+$");
		std::vector<glm::vec3> vertex;
		std::vector<glm::vec3> texture;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> spaceVertex;
		glm::vec3 maxvec;
		glm::vec3 minvec;
		glm::vec3 center;
		std::vector<std::vector<int>> faceIndex;
		std::vector<std::vector<int>> polyLine;
		std::string token = "c";
		glm::vec3 centroid(0.0f, 0.0f, 0.0f);
		std::vector<glm::vec3> &fullvertex = a->vertex;
		std::vector<glm::vec3> &linesvertex = a->vertexLines;
		std::vector<int> temp;
		float normalizer;
		float x, y, z, xmax = -99999999990.0f, ymax = -9999999990.0f, zmax = -999999990.0f, xmin = 9999999999990.0f, ymin = 999999999990.0f, zmin = 999999999990.0f;

		ifs.open("C:/Users/Danielito/Desktop/Workbench/CG1-2022-Tarea-2/EasyDIPClient/EasyDIPClient/Objects/Batman.obj", std::ifstream::in);
		//std::cout << ifs.is_open() << std::endl;

		if (token == "c") {
			ifs >> token;
		}

		//std::cout << token;
		while (!(ifs.eof())) {

			//std::cout << token;

			if (token == "v") {
				
				glm::vec3 temp(0.0f, 0.0f, 0.0f);
				ifs >> token;
				temp.x = std::strtof((token).c_str(), 0);
				ifs >> token;
				temp.y = std::strtof((token).c_str(), 0);
				ifs >> token;
				temp.z = std::strtof((token).c_str(), 0);

				//std::cout << temp.x << " " << temp.y << " " << temp.z << std::endl;
				vertex.push_back(temp);	
				ifs >> token;
				if (std::regex_match(token, decimal)) {
					/*temp.z = std::strtof((token).c_str(), 0);*/
					//vertex.push_back(temp);
					ifs >> token;
				}
				else {
					//vertex.push_back(temp);
				}
				//UPDATE_BB(xmax, xmin, buffer.x);
				xmax = std::max(xmax, temp.x);
				xmin = std::min(xmin, temp.x);
				//UPDATE_BB(ymax, ymin, buffer.y);
				ymax = std::max(ymax, temp.y);
				ymin = std::min(ymin, temp.y);
				//UPDATE_BB(zmax, zmin, buffer.z);
				zmax = std::max(zmax, temp.z);
				zmin = std::min(zmin, temp.z);

			}

			if (token == "vt") {
				glm::vec3 temp(0.0f, 0.0f, 0.0f);
				//std::cout << "vertex Texture Loaded" << std::endl;
				ifs >> token;
				temp.x = std::strtof((token).c_str(), 0);
				ifs >> token;
				if (std::regex_match(token, decimal) || std::regex_match(token, integer)) {
					temp.y = std::strtof((token).c_str(), 0);
					ifs >> token;
					if (std::regex_match(token, decimal) || std::regex_match(token, integer)) {
						temp.z = std::strtof((token).c_str(), 0);
						texture.push_back(temp);
						ifs >> token;
					}
					else {
						texture.push_back(temp);
					}
				}
				else {
					texture.push_back(temp);
				}
			}

			if (token == "vn") {
				glm::vec3 temp(0.0f, 0.0f, 0.0f);
				//std::cout << "Normal Vertex" << std::endl;
				ifs >> token;
				temp.x = std::strtof((token).c_str(), 0);
				ifs >> token;
				temp.y = ::strtof((token).c_str(), 0);
				ifs >> token;
				temp.z = ::strtof((token).c_str(), 0);
				normals.push_back(temp);
				ifs >> token;
			}

			if (token == "vp") {
				glm::vec3 temp(0.0f, 0.0f, 0.0f);
				//std::cout << "Vertex Pointer" << std::endl;
				ifs >> token;
				temp.x = std::strtof((token).c_str(), 0);
				ifs >> token;
				temp.y = ::strtof((token).c_str(), 0);
				ifs >> token;
				temp.z = ::strtof((token).c_str(), 0);
				normals.push_back(temp);
				ifs >> token;
			}

			if (token == "f") {
				std::vector<int> temp;
				int first;
				//std::cout << "Face Loaded" << std::endl;
				//std::stoi(token);
				ifs >> token;

				if (std::regex_match(token, integer)) {
					first = std::stoi(token) - 1;
					temp.push_back(first);
					ifs >> token;
					while (std::regex_match(token, integer)) {
						//std::cout << token << std::endl;
						temp.push_back(std::stoi(token)-1);
						ifs >> token;
					}
					//faceIndex.push_back(temp);
					//std::cout << token << std::endl;
					faceIndex.push_back(temp);
					ifs >> token;
				}
				
				if (std::regex_match(token, case1)) {

					std::string a = "", b = "";
					bool d = true, c = true;
					for (int i = 0; i < token.size(); i++) {
						if (token[i] == '/') {
							c = false;
							continue;
						}

						if (c) {
							a += token[i];
						}

						else {

							b += token[i];
						}

					}

					//std::cout << a << " " << b << std::endl;
					//std::cout << "Baina Loca" << std::endl;

					while (std::regex_match(a, integer)) {
						temp.push_back(std::stoi(a) - 1);
						//std::cout << a << std::endl;
						a = "";
						b = "";
						c = true;
						ifs >> token;
						for (int i = 0; i < token.size(); i++) {
							if (token[i] == '/') {
								c = false;
								continue;
							}

							if (c) {
								a += token[i];
							}
							else {
								b += token[i];
							}
						}

						if (ifs.eof()) {
							//temp.push_back(std::stoi(a) - 1);
							a = "a";
						}
					}
					//
					//std::cout << a << std::endl;
					faceIndex.push_back(temp);
					std::cout << "Face Loaded" << std::endl;
					//ifs >> token;*/
				}

				if (std::regex_match(token, case2)) {

					std::string a = "", b = "";
					bool d = true, c = true;
					for (int i = 0; i < token.size(); i++) {
						if (token[i] == '/') {
							c = false;
							continue;
						}

						if (c) {
							a += token[i];
						}

						else {

							b += token[i];
						}

					}

					//std::cout << a << " " << b << std::endl;
					//std::cout << "Baina Loca" << std::endl;

					while (std::regex_match(a, integer)) {
						temp.push_back(std::stoi(a) - 1);
						//std::cout << a << std::endl;
						a = "";
						b = "";
						c = true;
						ifs >> token;
						for (int i = 0; i < token.size(); i++) {
							if (token[i] == '/') {
								c = false;
								continue;
							}

							if (c) {
								a += token[i];
							}
							else {
								b += token[i];
							}
						}
						if (ifs.eof()) {
							//temp.push_back(std::stoi(a) - 1);
							a = "a";
						}
					}
					//
					//std::cout << a << std::endl;

					faceIndex.push_back(temp);
					//std::cout << "Face Loaded" << std::endl;
					//ifs >> token;*/
				}

				if (std::regex_match(token, case3)) {

					std::string a = "", b = "";
					bool d = true, c = true;
					for (int i = 0; i < token.size(); i++) {
						if (token[i] == '/') {
							c = false;
							continue;
						}

						if (c) {
							a += token[i];
						}

						else {

							b += token[i];
						}

					}

					//std::cout << a << " " << b << std::endl;
					//std::cout << "Baina Loca" << std::endl;

					while (std::regex_match(a, integer)) {
						temp.push_back(std::stoi(a) - 1);
						//std::cout << a << std::endl;
						a = "";
						b = "";
						c = true;
						ifs >> token;
						for (int i = 0; i < token.size(); i++) {
							if (token[i] == '/') {
								c = false;
								continue;
							}

							if (c) {
								a += token[i];
							}
							else {
								b += token[i];
							}
						}
						if (ifs.eof()) {
							//temp.push_back(std::stoi(a) - 1);
							a = "a";
						}
					}
					//
					//std::cout << a << std::endl;
					faceIndex.push_back(temp);
					//std::cout << "Face Loaded" << std::endl;
					//ifs >> token;*/
				}
				
			}

			if (token == "l") {
				std::vector<int> temp;
				//std::cout << "Line Loaded" << std::endl;
				ifs >> token;
				while (std::regex_match(token, integer)) {
					temp.push_back(std::stoi(token));
					ifs >> token;
				}
				polyLine.push_back(temp);
			}


			if (token != "l" && token != "f" && token != "vp" && token != "vn" && token != "vt" && token != "v") {
				ifs >> token;
			}

			
		}

		maxvec = glm::vec3(xmax, ymax, zmax);
		minvec = glm::vec3(xmin, ymin, zmin);
		center = (maxvec + minvec) / 2.0f;

		normalizer = std::pow( pow(xmax - xmin, 2) + pow(ymax - ymin, 2) + pow(zmax - zmin, 2), 0.5);

		//std::cout << normalizer << std::endl;


		for (int it = 0; it < vertex.size(); ++it) {

			//std::cout << "(" << vertex[it].x << "," << vertex[it].y << "," << vertex[it].z << ")" << std::endl;
			vertex[it] = vertex[it]  / normalizer;
			//std::cout << "(" << vertex[it].x << "," << vertex[it].y << "," << vertex[it].z << ")" << std::endl; 

		}

		int n = faceIndex.size();

		//std::cout << "Almost Done" << std::endl;

		//std::cout << n << std::endl;

		/*for (int i = 0; i < n; i++) {
			std::vector<int> temp = faceIndex[i];
			for (int j = 1; j < temp.size() - 1; j++) {
				int d = temp[0];
				int b = temp[j];
				int c = temp[j+1];

				std::cout << "(" << d+1 << "," << b+1 << "," << c+1 << ")" << std::endl;
				std::cout << "(" << vertex[d].x << "," << vertex[d].y << "," << vertex[d].z << ")" << std::endl;
				std::cout << "(" << vertex[b].x << "," << vertex[b].y << "," << vertex[b].z << ")" << std::endl;
				std::cout << "(" << vertex[c].x << "," << vertex[c].y << "," << vertex[c].z << ")" << std::endl;
				fullvertex.push_back(vertex[d]);
				fullvertex.push_back(vertex[b]);
				fullvertex.push_back(vertex[c]);
				centroid = centroid + vertex[temp[j]-1];

					glm::vec3 normaloid = glm::cross(vertex[c] - vertex[b], vertex[c] - vertex[d]);
					a->normalCenter.push_back(centroid);
					centroid = (vertex[d] + vertex[b] + vertex[c]) / 3.0f;
					if (normaloid != glm::vec3(0.0f, 0.0f, 0.0f)) {
						a->normalCenter.push_back(0.1f * (glm::normalize(normaloid)));
					}
					else {
						a->normalCenter.push_back(0.1f * (normaloid));
					}x

			}
			centroid = glm::vec3(0.0f, 0.0f, 0.0f);
		}*/
		
		temp = faceIndex[0];

		for (int i = 0; i < faceIndex.size(); i++) {
			int first = faceIndex[i][0];
			for (int j = 1; j < faceIndex[i].size() - 1; j++) {
				glm::vec3 temp;
				temp = vertex[first];
				fullvertex.push_back(temp);
				temp = vertex[faceIndex[i][j]];
				fullvertex.push_back(temp);
				temp = vertex[faceIndex[i][j+1]];
				fullvertex.push_back(temp);
				
				//std::cout << "f " << first+1 << " " << faceIndex[i][j]+1 << " " << faceIndex[i][j + 1]+1 << std::endl;

			}
		}

		fullvertex = fullvertex;
		glm::vec3 first = fullvertex[0];
		int counter = 1;
		for (int i = 0; i < fullvertex.size() - 1; i++) {
			
			

			if (counter % 3 == 0) {

				linesvertex.push_back(first);
				linesvertex.push_back(fullvertex[i - 1]);
				first = fullvertex[i];
				counter = 1;

			}
			else {

				linesvertex.push_back(fullvertex[i]);
				linesvertex.push_back(fullvertex[i + 1]);
				counter++;

			}

			if (i == fullvertex.size() - 2) {

				linesvertex.push_back(fullvertex[i + 1]);
				linesvertex.push_back(first);
			}
		}

		fullvertex = fullvertex;
		linesvertex = linesvertex;
		
		std::cout << "Done" << std::endl;

		/*for (int i = 0; i < fullvertex.size(); i++) {
			std::cout << "(" << fullvertex[i].x << "," << fullvertex[i].y << "," << fullvertex[i].z << ")" << std::endl;
			a->vertex.push_back(fullvertex[i]);
		}*/
	
	}

	void LoadOff(const std::string path, Object *a) {
		std::ifstream ifs;
		std::regex decimal("^[-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?$");
		std::regex integer("^[+-]?[0-9]+$");
		std::string token;
		glm::vec3 buffer;
		glm::vec3 center;
		glm::vec3 maxvec;
		glm::vec3 minvec;
		std::vector<glm::vec3> normalCenter;
		std::vector<glm::vec3> vertex;
		std::vector<glm::vec3> faces;
		std::vector<glm::vec3> colors;
		std::vector<glm::vec3> normals;
		std::vector<std::vector<int>> faceIndex;
		std::vector<glm::vec3> faceColors;
		std::vector<glm::vec3> fullvertex;
		std::vector<glm::vec3> centers;
		std::map<int, std::vector<glm::vec3>> normalmap;
		std::string pend;
		glm::vec3 centroid;
		int act = 1, r, v, f, e = 0;
		float  normalizerX, normalizerY, normalizerZ;
		float x, y, z, xmax = 0.0f, ymax = 0.0f, zmax = 0.0f, xmin = 0.0f, ymin = 0.0f, zmin = 0.0f;

		//ifs.open("C:/Users/skandergod/Desktop/Helipuerto/CG1-Tarea-3/EasyDIPClient/EasyDIPClient/Objects/seashell.off", std::ifstream::in);
		ifs.open("C:/Users/Daniel/Desktop/proyectos/CG1-Tarea-3/EasyDIPClient/EasyDIPClient/Objects/Apple.off", std::ifstream::in);
		//C:\Users\skandergod\Desktop\Helipuerto\CG1-Tarea-3\EasyDIPClient\EasyDIPClient\Objects
		//C:\Users\Daniel\Desktop\proyectos\CG1-Tarea-3\EasyDIPClient\EasyDIPClient\Objects

		std::cout << "Mierda abierta " << ifs.is_open() << std::endl;
		while (act) {
			ifs >> token;
			std::cout << "ignoring line: " << token << std::endl;
			std::cout << std::regex_match(token, integer) << std::endl;
			if (std::regex_match(token,integer)) {
				act = 0;
			}
		}
		std::cout << "ignored comments" << std::endl;
		v = std::stoi(token);
		ifs >> token;
		f = std::stoi(token);
		ifs >> token;

		
		// ifs >> line;
		

			/*act = 1;	
			x = std::strtof((line).c_str(),0);
			xmax = x;
			xmin = x;
			ifs >> line;
			y = std::strtof((line).c_str(), 0);
			ymax = y;
			ymin = y;
			ifs >> line;
			z = std::strtof((line).c_str(), 0);

			zmax = z;
			zmin = z;

			buffer = glm::vec3(x, y, z);
			vertex.push_back(buffer);*/

		

		for (;  v--; ) {

	/*		while (!(std::regex_match(token, decimal)) || !(std::regex_match(token, integer))){
				ifs >> token;
			}*/

			//CLEAR_COMMENTS();
			buffer = glm::vec3();

			ifs >> token;
			buffer.x = std::strtof((token).c_str(), 0);
			ifs >> token;
			buffer.y = std::strtof((token).c_str(), 0);
			ifs >> token;
			buffer.z = std::strtof((token).c_str(), 0);


			//UPDATE_BB(xmax, xmin, buffer.x);
			xmax = std::max(xmax, buffer.x);
			xmin = std::min(xmin, buffer.x);
			//UPDATE_BB(ymax, ymin, buffer.y);
			ymax = std::max(ymax, buffer.y);
			ymin = std::min(ymin, buffer.y);
			//UPDATE_BB(zmax, zmin, buffer.z);
			zmax = std::max(zmax, buffer.z);
			zmin = std::min(zmin, buffer.z);

			vertex.push_back(buffer);

			//std::cout << "vertex " << i+1 << " loaded" << std::endl;
		}

		std::cout << "vertexs loaded" << std::endl;

		act = 1;
		ifs >> token;
		for (int i = 0; i < f; ++i) {
			std::vector<int> temp;
			glm::vec3 color;
			
			/*while (act) {
				ifs >> token;
				std::cout << token << std::endl;
				std::cout << i << std::endl;
				std::cout << std::regex_match(token, integer) << std::endl;
				//std::cout << "ignoring line: " << line << std::endl;
				//std::cout << std::regex_match(line, integer) << std::endl;
				if (std::regex_match(token, integer)) {
					act = 0;
				}
			}*/

			//ifs >> token;
			r = std::stoi(token);
			act = 1;

			for (int i = 0; i < r; ++i) {
				
				/*while (act) {
					ifs >> line;
					//std::cout << "ignoring line: " << line << std::endl;
					std::cout << std::regex_match(line, integer) << std::endl;
					if (std::regex_match(line, integer)) {
						act = 0;
					}
				}*/
				//act = 1;
				ifs >> token;
				temp.push_back(std::stoi(token));	

				//faceIndex.push_back(temp);
			}
			faceIndex.push_back(temp);

			ifs >> token;
			if (std::regex_match(token, integer)) {
				continue;
			}
			else {
				color.x = std::strtof((token).c_str(), 0);
				ifs >> token;
				color.y = std::strtof((token).c_str(), 0);
				ifs >> token;
				color.z = std::strtof((token).c_str(), 0);


				faceColors.push_back(color);
			}
			/*while (act) {
				ifs >> token;
				std::cout << i << std::endl;
				std::cout << token << std::endl;
				std::cout << std::regex_match(token, decimal) << std::endl;
				//std::cout << "ignoring line: " << line << std::endl;
				//std::cout << std::regex_match(line, integer) << std::endl;
				if (std::regex_match(token, integer)) {
					act = 0;
				}
				else {
					glm::vec3 temp2;
					temp2.x = std::strtof((token).c_str(), 0);
					ifs >> token;
					temp2.y = std::strtof((token).c_str(), 0);
					ifs >> token;
					temp2.z = std::strtof((token).c_str(), 0);
					faceColors.push_back(temp2);
					act = 0;
				}
			}*/

		}

		//generating normal vectors
		std::cout << "Finished loading Faces" << std::endl;

	

		//std::cout << std::regex_match(token, decimal) << std::endl;

		maxvec = glm::vec3(xmax, ymax, zmax);
		minvec = glm::vec3(xmin, ymin, zmin);
		center = (maxvec + minvec) / 2.0f;
		
		normalizerX = glm::max(abs(maxvec.x), abs(minvec.x));
		normalizerY = glm::max(abs(maxvec.y), abs(minvec.y));
		normalizerZ = glm::max(abs(maxvec.z), abs(minvec.z));

		normalizerX = glm::max(normalizerX, normalizerY);
		normalizerX = glm::max(normalizerX, normalizerZ);
		
		
		

		for (int it = 0; it < vertex.size(); ++it) {

			
			vertex[it] = (vertex[it] - center) / normalizerX;

		}

		
		/*for (auto it : faceIndex) {

		}*/
		



		//std::cout << ifs.is_open() << std::endl;
		//std::cout << ifs.good() << std::endl;
		center.x = 0.0f;
		center.y = 0.0f;
		center.z = 0.0f;

		int n = faceIndex.size();
		
		for (int i = 0; i < n; i++) {
			std::vector<int> temp = faceIndex[i];
			for (int j = 0; j < temp.size(); j++) {
				centroid = centroid + vertex[temp[j]];
				fullvertex.push_back(vertex[temp[j]]);
				if (j == 2) {
					glm::vec3 normaloid = glm::cross(vertex[temp[1]] - vertex[temp[0]], vertex[temp[2]] - vertex[temp[0]]);
					a->normalCenter.push_back(centroid);
					if (normaloid != glm::vec3(0.0f, 0.0f, 0.0f)) {
						a->normalCenter.push_back(0.1f*(glm::normalize(normaloid)));
					}
					else {
						a->normalCenter.push_back(0.1f*(normaloid));
					}
				}
			}
			centroid = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		for (int i = 0; i < fullvertex.size(); i++) {
			a->vertex.push_back(fullvertex[i]);
		}

		/*for (int i = 0; i < normalmap.size(); i++) {
			std::vector<glm::vec3> temp;
			temp = normalmap[i];
			a->normalmap[i] = temp;
		}*/

	
		

		std::cout << "Done!" << std::endl;
		
	}
}
