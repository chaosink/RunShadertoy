// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "shader.hpp"

int window_width = 960;
int window_height = 540;
//int window_width = 1920;
//int window_height = 1080;

int main(int argc, char** argv)
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	if(argc >= 3) {
		window_width = atoi(argv[1]);
		window_height = atoi(argv[2]);
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( window_width, window_height, "Tutorial 02 - Red triangle", NULL, NULL);
	glfwSetWindowPos(window, 1920 - window_width, 1080 - window_height);
//	window = glfwCreateWindow( window_width, window_height, "Tutorial 02 - Red triangle", glfwGetPrimaryMonitor(), NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "vertex.glsl", "fragment.glsl" );

	GLfloat g_vertex_buffer_data[12] = {-1, -1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1};
/*	GLfloat g_vertex_buffer_data[24] = {
		-1.0 / 2 - 0.5, -1.0 / 2 - 0.5, -1.0 / 2 - 0.5, 1.0 / 2 - 0.5, 1.0 / 2 - 0.5, 1.0 / 2 - 0.5, -1.0 / 2 - 0.5, -1.0 / 2 - 0.5, 1.0 / 2 - 0.5, 1.0 / 2 - 0.5, 1.0 / 2 - 0.5, -1.0 / 2 - 0.5,
		-1.0 / 2 + 0.5, -1.0 / 2 + 0.5, -1.0 / 2 + 0.5, 1.0 / 2 + 0.5, 1.0 / 2 + 0.5, 1.0 / 2 + 0.5, -1.0 / 2 + 0.5, -1.0 / 2 + 0.5, 1.0 / 2 + 0.5, 1.0 / 2 + 0.5, 1.0 / 2 + 0.5, -1.0 / 2 + 0.5};*/
/*	GLfloat g_vertex_buffer_data[24] = {
		-1,-1,
		 0,-1,
		 0, 0,
		-1,-1,
		 0, 0,
		-1, 0,
		 1, 1,
		 0, 1,
		 0, 0,
		 1, 1,
		 0, 0,
		 1, 0};*/
/*	GLfloat g_vertex_buffer_data[window_width][window_height][2];
	for(int i = 0; i < window_width; i++)
		for(int j = 0; j < window_height; j++) {
			g_vertex_buffer_data[i][j][0] = 2.0f / window_width * i - 1;
			g_vertex_buffer_data[i][j][1] = 2.0f / window_height * j - 1;
		}*/

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	do{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0/double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,					// size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glUniform1f(glGetUniformLocation(programID, "iGlobalTime"), glfwGetTime());
		glUniform3f(glGetUniformLocation(programID, "iResolution"), window_width, window_height, 0);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data) / 8); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

