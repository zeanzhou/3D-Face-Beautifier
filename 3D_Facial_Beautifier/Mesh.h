// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class Vertex {
public:
	glm::vec3 Position;
	glm::vec3 VertexColor;

	Vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b) {
		this->Position = glm::vec3(x, y, z);
		this->VertexColor = glm::vec3(r, g, b);
	}
};

class Vertex2D {
public:
	glm::vec2 Position;
	glm::vec4 VertexColor;
	
	Vertex2D(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 0.85f) {
		this->Position = glm::vec2(x, y);
		this->VertexColor = glm::vec4(r, g, b, a);
	}

	Vertex2D(GLfloat x, GLfloat y) {
		this->Position = glm::vec2(x, y);
		this->VertexColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.85f);
	}
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;

	/*  Functions  */
	// Constructor
	Mesh(vector<Vertex> vertices, vector<GLuint> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setup();
	}

	// Render the mesh
	void Draw(Shader shader)
	{
		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_POINTS, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setup()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Colors
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, VertexColor));

		glBindVertexArray(0);
	}
};

class Area {
public:

	Area(vector<Vertex2D> vertices)
	{
		this->vertices = vertices;

		//for (GLuint i = 0; i < vertices.size(); ++i)
		//	this->indices.push_back(i);

		this->setup();
	}

	Area()
	{
		this->setup();
	}

	int Count()
	{
		return this->vertices.size();
	}

	void UpdateEnd(GLfloat xpos, GLfloat ypos)
	{
		if (this->vertices.size() > 0) {
			vertices.back().Position.x = xpos;
			vertices.back().Position.y = ypos;
		}
	}

	void Insert(vector<Vertex2D>::iterator iter, Vertex2D v)
	{
		this->vertices.insert(iter, v);
	}

	void InsertBeforeEnd(Vertex2D v)
	{
		auto iter = this->vertices.end();
		if (this->vertices.size() <= 1)
			iter = this->vertices.begin();
		else
			iter--;
		this->Insert(iter, v);
	}

	void Append(Vertex2D v)
	{
		this->vertices.push_back(v);
		//this->indices.push_back(this->indices.size()-1);
	}

	void Pop()
	{
		this->vertices.pop_back();
	}

	void Draw(Shader shader)
	{
		glBindVertexArray(this->VAO);

		if (this->vertices.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex2D), &this->vertices[0], GL_DYNAMIC_DRAW);

			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_DYNAMIC_DRAW);
		}

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);
		// Vertex Colors
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, VertexColor));
		
		glDrawArrays(GL_LINE_STRIP, 0, this->vertices.size());
		//glDrawElements(GL_LINE_STRIP, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	vector<Vertex2D> vertices;
	//vector<GLuint> indices;
	GLuint VAO, VBO;// , EBO;

	// Initializes all the buffer objects/arrays
	void setup()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		//glGenBuffers(1, &this->EBO);

		//glBindVertexArray(this->VAO);

		//// Load data into vertex buffers
		//if (this->vertices.size() > 0)
		//{
		//	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		//	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex2D), &this->vertices[0], GL_DYNAMIC_DRAW);

		//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_DYNAMIC_DRAW);
		//}

		//// Set the vertex attribute pointers
		//// Vertex Positions
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)0);
		//// Vertex Colors
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (GLvoid*)offsetof(Vertex2D, VertexColor));

		//glBindVertexArray(0);
	}
};