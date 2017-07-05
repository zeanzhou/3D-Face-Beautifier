// Std. Includes
#include <string>

// OpenCV
#include <opencv2/opencv.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PLYLoader.h"

// Properties
GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mousemv_callback(GLFWwindow* window, double xpos, double ypos);
vector<GLuint> func(vector<glm::vec2>allVertexs, vector<Vertex2D>selectedVertices);
void mousebtn_callback(GLFWwindow* window, int button, int action, int mods);
void Do_Movement();
GLuint modeSwitch(GLuint mode);

// Mode Macro
#define FB_MODE_UNCHANGED 0
#define FB_MODE_NORMAL 1
#define FB_MODE_SELECT_POLYGON 2
#define FB_MODE_BRUSH_CIRCLE 3

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 500.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Pointer to model
Mesh* pModel = nullptr;

// Create a vector for area to draw
vector<Area> drawable;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "3D Face Beautifier", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mousebtn_callback);
	glfwSetCursorPosCallback(window, mousemv_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader modelShader("./Shaders/model.vs", "./Shaders/model.frag");
	Shader areaShader("./Shaders/area.vs", "./Shaders/area.frag");

	// Load point cloud file
	PointCloudPLY plyFile("./Resources/face/szz.ply");

	// Convert it to model
	Mesh ourModel = plyFile.getMesh();
	pModel = &ourModel;

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Mode initialization
	modeSwitch(FB_MODE_NORMAL);

	cout << "Initialized" << endl;

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader.Use();
		
		// Transformation matrices
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(modelShader);


		areaShader.Use();
		

		//// Draw area border
		for (auto iter = drawable.begin(); iter != drawable.end(); ++iter)
			iter->Draw(areaShader);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.ProcessKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_LEFT_CONTROL])
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	if (keys[GLFW_KEY_P]) {
		cout << "Plz select a polygon area..." << endl;
		modeSwitch(FB_MODE_SELECT_POLYGON);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		pModel->genVertices2D(projection * view * model);

		vector<Vertex2D> v;
		v.push_back(Vertex2D(0.0f, 0.0f));
		Area a(v);
		drawable.push_back(a);
	}
}

void mousemv_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (modeSwitch(FB_MODE_UNCHANGED) == FB_MODE_SELECT_POLYGON) {
		drawable.back().UpdateEnd(xpos / (GLfloat)screenWidth * 2.0f - 1, (screenHeight - ypos) / (GLfloat)screenHeight * 2.0f - 1);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mousebtn_callback(GLFWwindow* window, int button, int action, int mods)
{
	static GLfloat lastClickTime;
	static double lastx, lasty;
	GLfloat currentClickTime = glfwGetTime(); // in seconds

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Double click detection
	bool DOUBLE_CLICK = false;
	if (action == GLFW_PRESS) {
		//cout << lastClickTime << " " << currentClickTime << endl;
		if (currentClickTime - lastClickTime < 0.25 && xpos == lastx && ypos == lasty) {
			DOUBLE_CLICK = true;
			lastClickTime -= 1.0f;
		}
		else {
			lastClickTime = currentClickTime;
			lastx = xpos;
			lasty = ypos;
		}
			
	}

	if (button == GLFW_MOUSE_BUTTON_1 && DOUBLE_CLICK) {
		if (modeSwitch(FB_MODE_UNCHANGED) == FB_MODE_SELECT_POLYGON) {
			modeSwitch(FB_MODE_NORMAL);
			drawable.back().Pop();
			cout << "Finish Selection" << endl;
			auto selectedIndices = func(pModel->vertices_2D, drawable.back().vertices);
			for (int i = 0; i < selectedIndices.size(); ++i) {
				pModel->vertices[selectedIndices[i]].VertexColor = glm::vec3(1.0f, 0.0f, 0.0f);
				//cout << "Vertex " << i << endl;
			}
		}
	}

	else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
		if (modeSwitch(FB_MODE_UNCHANGED) == FB_MODE_SELECT_POLYGON) {
			Vertex2D v = Vertex2D(xpos / (GLfloat)screenWidth * 2.0f - 1, (screenHeight - ypos) / (GLfloat)screenHeight * 2.0f - 1);
			cout << drawable.back().Count() << " " << v.Position.x << " " << v.Position.y << endl;
			drawable.back().InsertBeforeEnd(v);
		}
	}
}

vector<GLuint> func(vector<glm::vec2>allVertexs, vector<Vertex2D>selectedVertices) {
	vector <GLuint> output;
	for (GLuint j = 0; j<allVertexs.size(); j++) {
		int nCross = 0;
		glm::vec2 p;
		p = allVertexs[j];
		//printf("%d,(%f,%f)\n", allVertexs.size(),p.x,p.y);
		//system("pause");
		for (int i = 0; i < selectedVertices.size(); i++) {
			glm::vec2 p1, p2;
			p1 = selectedVertices[i].Position;
			p2 = selectedVertices[(i + 1) % selectedVertices.size()].Position;
			//printf("%d,(%f,%f),(%f,%f)\n", allVertexs.size(), p1.x, p1.y, p2.x, p2.y);

			if (p1.y == p2.y) // p1p2 与 y=p0.y平行 
				continue;
			if (p.y < min(p1.y, p2.y)) // 交点在p1p2延长线上 
				continue;
			if (p.y >= max(p1.y, p2.y)) // 交点在p1p2延长线上 
				continue;
			// 求交点的 X 坐标 -------------------------------------------------------------- 
			double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
			if (x > p.x)
				nCross++; // 只统计单边交点 

		}
		if (nCross % 2 == 1) {
			output.push_back(j);
			//printf("yes,%d",j);
			//system("pause");
		}
	}
	return output;
}

GLuint modeSwitch(GLuint mode = FB_MODE_UNCHANGED)
{
	// Mode Indicator
	static GLuint displayMode = mode;
	if (mode == 0)
		return displayMode;
	else
		return displayMode = mode;
}
#pragma endregion