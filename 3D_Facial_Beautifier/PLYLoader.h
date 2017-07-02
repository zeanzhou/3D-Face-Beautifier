#include <iostream>
#include <fstream>
#include <vector>
#include "Mesh.h"

using namespace std;


class PointCloudPLY {
public:
	Vertex **v;
	int vIndex;
	int current_max_verticies;

	bool isFlat;


	PointCloudPLY(char *filename) : DEFAULT_SIZE(16) {
		vIndex = -1;

		isFlat = false;

		char *line = new char[200];
		char *line_back = new char[200];
		char wspace[] = { ' ', '\t' };
		char separator[] = { '/' };
		char *token;
		int indices[3];
		double x, y, z, r, g, b;

		ifstream file(filename);
		if (!file) {
			cerr << "Cannot open file: " << filename << " exiting." << endl;
			exit(-1);
		}
		bool is_data = 0;
		while (!file.eof()) {
			file.getline(line, 199);
			// first, strip off comments
			if (is_data == 1) {
				strcpy_s(line_back, sizeof(line_back) + 200, line); // strtok destroys line.
				char*pNext = NULL;
				x = atof(strtok_s(line, wspace, &pNext));
				y = atof(strtok_s(NULL, wspace, &pNext));
				z = atof(strtok_s(NULL, wspace, &pNext));
				r = atof(strtok_s(NULL, wspace, &pNext));
				g = atof(strtok_s(NULL, wspace, &pNext));
				b = atof(strtok_s(NULL, wspace, &pNext));
				addVertex(x, y, z, r, g, b);


			}
			if (is_data == 0) {
				if (!strcmp(line, ""))
					continue;

				else {
					strcpy_s(line_back, sizeof(line_back) + 200, line); // strtok destroys line.
					char*pNext = NULL;
					token = strtok_s(line, wspace, &pNext);
					if (!strcmp(token, "element")) {
						strtok_s(NULL, wspace, &pNext);
						token = strtok_s(NULL, wspace, &pNext);

						current_max_verticies = atoi(token);
					}
					else if (!strcmp(token, "end_header")) {       // blank line
						is_data = 1;
						printf("start read data!");
					}
					else {
						//cout << line_back << endl;
					}
				}
			}
		}
	}

	void addVertex(double x, double y, double z, double r, double g, double b) {
		Vertex *vert = new Vertex((GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)r, (GLfloat)g, (GLfloat)b);
		addVertex(vert);
	}

	void addVertex(Vertex *vert) {
		if (vIndex < 0) {
			v = new Vertex*[DEFAULT_SIZE];
			vIndex = 0;
			current_max_verticies = DEFAULT_SIZE;
		}
		v[vIndex] = vert;
		vIndex += 1;
		if (vIndex == current_max_verticies) {
			current_max_verticies = 2 * vIndex;
			Vertex **newV = new Vertex*[current_max_verticies];
			for (int i = 0; i < vIndex; i++)
				newV[i] = v[i];
			delete[] v;
			v = newV;
		}
	}

	Mesh getMesh() {
		vector<Vertex> vertices;
		vector<GLuint> indices;

		for (int i = 0; i < vIndex; ++i) {
			Vertex thisVertex = *this->v[i];
			thisVertex.VertexColor /= 255.0f;
			vertices.push_back(thisVertex);
			indices.push_back(i);
		}
		return Mesh(vertices, indices);
	}

private:
	const int DEFAULT_SIZE;
};