#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

// binds the VAO and the VBO with the vertices
void bind(unsigned int* VAO, unsigned int* VBO, float *vertices, int verticesSize);

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// First triangle
float first_triangle[] = {
	// positions			// colors
	-0.8f,  0.8f, 0.0f,     0.0f, 1.0f, 0.0f, // top left
	-0.8f, -0.8f, 0.0f,     0.0f, 1.0f, 0.0f, // bottom left
	 0.0f,  0.0f, 0.0f,     0.0f, 1.0f, 0.0f  // rigth side
};

// Second Triangle
float second_triangle[] = {
	 // positions			// colors
	 0.8f,  0.8f, 0.0f,     0.0f, 0.0f, 0.8f,  // top right
	 0.8f, -0.8f, 0.0f,		0.0f, 0.0f, 0.8f,  // botttom right
	 0.0f,  0.0f, 0.0f,		0.0f, 0.0f, 0.8f   // center
};

int main() {
	// init the GLFW
	glfwInit();

	// configure the GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Check if glad could be loaded
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// sets the portion which we will draw on 
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	Shader shader = Shader(".\\VertexShader.vs", ".\\FragmentShader.fs");

	unsigned int VAOs[2];
	unsigned int VBOs[2];
	
	// generate the VAO and the VBO
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	int first_triangle_size =  sizeof(first_triangle);
	int second_triangle_size = sizeof(second_triangle);

	/*float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

	// render loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// tells the gpu to use this shader program
		shader.use();
		shader.setFloat("offset", -0.1f);
		
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		bind(&VAOs[0], &VBOs[0], first_triangle, first_triangle_size);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// tells the gpu to use this shader program
		shader.use();
		shader.setFloat("offset", 0.1f);

		bind(&VAOs[1], &VBOs[1], second_triangle, second_triangle_size);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Check if the escape key was pressed, close the window if so
		processInput(window);
		
		// swaps the color buffer that is used to render to during iteration and show it as 
		// output on the screen
		glfwSwapBuffers(window);

		// checks if any events are triggered(keyboard input / mouse movement)
		// updates the window state and calls corresponding functions
		glfwPollEvents();
	}

	// clean up the glfw stuff
	glfwTerminate();
	return 0;
}

void bind(unsigned int* VAO, unsigned int* VBO, float* vertices, int verticesSize) {
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	
	// location, size, type, normalized, stride = byte offset between consecutive vertex attributes, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// location, size, type, normalized, stride = byte offset between consecutive vertex attributes, offset = starting pos
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	float move_offset = 0.00005;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		first_triangle[1] += move_offset;
		first_triangle[7] += move_offset;
		first_triangle[13] += move_offset;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		first_triangle[0] -= move_offset;
		first_triangle[6] -= move_offset;
		first_triangle[12] -= move_offset;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		first_triangle[0] += move_offset;
		first_triangle[6] += move_offset;
		first_triangle[12] += move_offset;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		first_triangle[1] -= move_offset;
		first_triangle[7] -= move_offset;
		first_triangle[13] -= move_offset;
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		second_triangle[1] += move_offset;
		second_triangle[7] += move_offset;
		second_triangle[13] += move_offset;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		second_triangle[0] -= move_offset;
		second_triangle[6] -= move_offset;
		second_triangle[12] -= move_offset;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		second_triangle[1] -= move_offset;
		second_triangle[7] -= move_offset;
		second_triangle[13] -= move_offset;
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		second_triangle[0] += move_offset;
		second_triangle[6] += move_offset;
		second_triangle[12] += move_offset;
	}
}
