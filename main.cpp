//Подключаем библиотеки OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Прочие библиотеки 
#include <iostream>

//Библиотеки созданные мной для OpenGL
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

//Библиотеки для движка/игры
#include "Texture.h"

//Пространства имен
using namespace std;

const unsigned int width = 800;
const unsigned int height = 800;

//Массив вершин
GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f,		0.0f, 0.0f, //Нижний левый угол 
	-0.5f, 0.5f, 0.0f,          0.0f, 1.0f, 0.0f,		0.0f, 1.0f, //Нижний правый угол
	0.5f, 0.5f, 0.0f,           0.0f, 0.0f, 1.0f,		1.0f, 1.0f, //Верхний левый угол
	0.5f, -0.5f, 0.0f,          1.0f, 1.0f, 1.0f,		1.0f, 0.0f  //Верхний правый угол
};

GLuint indices[]{
	0, 2, 1, //Верхний треугольник
	0, 3, 2  //Нижний треугольник
};

//Главная функция
int main() {

	glfwInit(); // Инициализируем GLFW

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Главная версия
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Суб версия
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Используем Core профиль

	GLFWwindow* window = glfwCreateWindow(width, height, "Universe Engine", NULL, NULL); //Создаем окно

	//Если все пойдет по жопе
	if (window == NULL) {
		cout << "Failed to initialize GLFW!" << endl; //Вывод ошибки
		glfwTerminate(); //GLFW покинул чат
		return -1; //Ну и всё
	}

	glfwMakeContextCurrent(window); //Говорим, что наше окно главное

	gladLoadGL(); //Подключаем GLAD

	glViewport(0, 0, 800, 800); //Размер рабочей области окна

	Shader shaderProgram("default.vert", "default.frag");
	
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));

	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	Texture Hayato("Hayato.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	Hayato.texUnit(shaderProgram, "tex0", 0);

	//Основной цикл, или то, что происходит в окне
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //Фон окна
		glClear(GL_COLOR_BUFFER_BIT); //Вычищаем буфферы

		shaderProgram.Activate();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(800 / 800), 0.1f, 100.0f);

		glUniform1f(uniID, 0.5f);
		Hayato.Bind();
		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window); //Меняем буфферы

		glfwPollEvents(); //OpenGL работает
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	Hayato.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window); //Уничтожаем окно
	glfwTerminate(); //GLFW уходит окончательно (до следующего запуска программы)
	return 0; //Конец
}