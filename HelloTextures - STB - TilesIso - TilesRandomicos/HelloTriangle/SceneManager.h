#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ctime>

//irrKlang



enum Tela { tJogo, tMenu, tCreditos, tInstrucoes, tGameOver, sair };

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouseCursorCallback(GLFWwindow* window, double x, double y);
	static void resize(GLFWwindow* window, int width, int height);

	void do_movement();
	void render();
	void run();
	void finish();


	// scene
	void setupScene();
	void setupCamera2D();
	int setupTexture(GLchar *path);
	void draw(glm::vec3 transform, int index, GLfloat offset, glm::vec3 scale, GLfloat qtdSpritesX, GLfloat qtdSpritesY, GLfloat offsetY, int cor);
	//static void tMenu();
	bool checkCollision(int a, int b);
	bool checkCollision(int a, int b, glm::vec3 trans);
	int checkButton(double x, double y, int id);
	void tileWalking();
	void lerArqTile(string caminho);

	bool checkTriangleCollision(glm::vec2 vertA, glm::vec2 vertB, glm::vec2 vertD, glm::vec2 mousePos);
	float calculaArea(glm::vec2 vertA, glm::vec2 vertB, glm::vec2 vertD);

	string tile;
	int largura, altura;
	Tela telaAtual = tMenu;

private:

	//espero que seja isso
	int direcao = 0;
	int tileX = 6, tileY = 0, xAux = 0, yAux = 0;
	int mapaX[20][20]; //o que deveria estar entre os colchetes?
	int mapaY[20][20];
	int mapaPintados[20][20]{};
	int mapaCaminhavel[20][20];
	int vel = 0.1f;
	float xMin = 0.0, xMax = 800.0, yMin = 600.0, yMax = 0.0, zNear = -1.0, zFar = 1.0;

	int telaGameOver;
	GLchar *path;
	GLfloat characterPositionX = -1, characterPositionY = -1, offsetBG1, offsetBG2;
	GLfloat characterPositionXAux, characterPositionYAux;
	GLfloat obstaculoX[7]{ 7, 8, 9, 3, 4, 5, 6 }, obstaculoY[7]{ 0, 1, 2, 3, 4, 5, 6 };
	double xpos, ypos;
	int mouse_grid_x, mouse_grid_y;
	GLfloat qtdSpritesX[9];
	GLfloat qtdSpritesY[9];
	int velSprites = 0;
	float spritesheet = 0;
	GLfloat clique;
	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shader;

	//scene attributes
	GLuint VAO;

	//Transformations - Model Matrix
	glm::mat4 model;

	//2D Camera - Projection matrix
	glm::mat4 projection;

	//Texture index
	unsigned int texture[9];

	float offsetX = 0, offset[9]{}, offsetY = 0;

	//Transform index
	glm::vec3 transform[9];
	glm::vec3 scale[9];

	glm::vec3 multScale[9];

	int size[9][2];

	glm::vec2 vTopLeft, vDownLeft, vTopRight, vDownRight;
};

