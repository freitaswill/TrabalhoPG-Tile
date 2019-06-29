#include "SceneManager.h"
#include <irrKlang.h>
using namespace irrklang;

//ISoundEngine *SoundEngine = createIrrKlangDevice();

//static controllers for mouse and keyboard

static bool keys[1024];
static bool resized;
static GLuint width, height;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = 800;
	height = 600;

	telaAtual = tMenu;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Position Mouse callback
	glfwSetCursorPosCallback(window, mouseCursorCallback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO

	resized = true; //para entrar no setup da câmera na 1a vez

	setupScene();
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::mouseCursorCallback(GLFWwindow* window, double x, double y)
{
	//cout << x << ": " << y << endl;
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);

}


void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

}

void SceneManager::render()
{
	srand(time(0));

	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	// Create transformations 
	model = glm::mat4();
	offsetX = float();

	// Get their uniform location
	// Pass them to the shaders

	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	tileWalking();

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS /*&& jumping == false*/) {
		/*jumping = true;*/
		for (int i = 0; i < 7; i++) {
			obstaculoX[i] += 0.035f;
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos -= width * 0.5f;
		ypos -= height * 0.5f;

		xpos *= -1;
		ypos *= -1;
	}
	mapaPintados[tileY][tileX] = 1;
	
	for (int i = 0; i < largura; ++i) {
		for (int j = 0; j < altura; ++j) {

			

			vTopLeft   = glm::vec2((((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j) - (((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1])) / 2),(((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i));
			vTopRight  = glm::vec2((((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j) + (((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1])) / 2),(((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i));
			vDownLeft  = glm::vec2((((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j),(((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i) - (((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1])) / 2));
			vDownRight = glm::vec2((((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j),(((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i) + (((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1])) / 2));

			if (-xpos / 400.0f >= (((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j) - (((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1])) / 2) && -xpos / 400.0f <= (((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j) + (((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1])) / 2) && ypos / 300.0f >= (((0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i) - (((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1])) / 2) && ypos / 300.0f <= (((0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i) + (((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1])) / 2) || mapaPintados[j][i] == 1){
				if (checkTriangleCollision(vTopLeft, vDownLeft, vDownRight, glm::vec2(-xpos/400.0f, ypos/300.0f)) == true){
					draw(glm::vec3(0 + ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j, ((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i, 0), texture[1], mapaX[j][i], glm::vec3(1.0f, 1.0f, 1), qtdSpritesX[texture[1] - 1], qtdSpritesY[texture[1] - 1], mapaY[j][i], 1);
				}
				else if (checkTriangleCollision(vTopRight, vDownLeft, vDownRight, glm::vec2(-xpos / 400.0f, ypos / 300.0f)) == true) {
					draw(glm::vec3(0 + ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j, ((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i, 0), texture[1], mapaX[j][i], glm::vec3(1.0f, 1.0f, 1), qtdSpritesX[texture[1] - 1], qtdSpritesY[texture[1] - 1], mapaY[j][i], 1);
				}
				else if (mapaPintados[j][i] == 1) {
					draw(glm::vec3(0 + ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j, ((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i, 0), texture[1], mapaX[j][i], glm::vec3(1.0f, 1.0f, 1), qtdSpritesX[texture[1] - 1], qtdSpritesY[texture[1] - 1], mapaY[j][i], 2);
				}
				else {
					draw(glm::vec3(0 + ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j, ((0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i, 0), texture[1], mapaX[j][i], glm::vec3(1.0f, 1.0f, 1), qtdSpritesX[texture[1] - 1], qtdSpritesY[texture[1] - 1], mapaY[j][i], 0);
				}
			}
			else{
				draw(glm::vec3(0 + ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2)*j, ((0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2)*j + (0 + scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) * i, 0), texture[1], mapaX[j][i], glm::vec3(1.0f, 1.0f, 1), qtdSpritesX[texture[1] - 1], qtdSpritesY[texture[1] - 1], mapaY[j][i], 0);
			}
		} 
	}



	if (velSprites % 15 == 0) {
		spritesheet += 1.0f;
	}

	draw(glm::vec3(characterPositionX, characterPositionY, 0), texture[4], 0, glm::vec3(2, 2, 1), 4, 4, spritesheet, 0);

	for (int i = 0; i < 7; i++)
		draw(glm::vec3(obstaculoX[i], obstaculoY[i], 0), texture[2], 0, glm::vec3(1, 1, 1), 1, 1 , 0, 0);

	for (int i = 0; i <7; i++)
		if (checkCollision(texture[2] - 1, texture[1] - 1, glm::vec3(obstaculoX[i], obstaculoY[i], 1))){
			telaAtual = tGameOver;
		}
}


void SceneManager::run(){

	lerArqTile("Tilemap.txt");
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames += 1;
		velSprites += 1;
		if (currentTime - lastTime >= 0.066f) { // If last prinf() was more than 1 sec ago
												// printf and reset timer
			//cout << "frames\n" << 1000.0 / double(nbFrames);
			nbFrames = 0;
			velSprites += 1;
			lastTime += 0.066f;

			//Play sound while game is running
			//SoundEngine->play2D("audio/bensound-happyrock.mp3", GL_TRUE);

			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			//Update method(s)
			do_movement();

			//Render scene
			render();

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
	} while (!glfwWindowShouldClose(window) && telaAtual != sair);

}

void SceneManager::finish(){
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene(){
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	texture[0] = setupTexture("../textures/tJogo.png");

	texture[1] = setupTexture("../textures/tile1.png");

	texture[2] = setupTexture("../textures/tile1.png");

	texture[3] = setupTexture("../textures/tMenu.png");

	texture[4] = setupTexture("../textures/george.png");

	texture[5] = setupTexture("../textures/tCreditos.png");

	texture[6] = setupTexture("../textures/tInstrucoes.png");

	texture[7] = setupTexture("../textures/tJogo1.png");

	texture[8] = setupTexture("../textures/tJogo2.png");

	texture[9] = setupTexture("../textures/tile.png");

}

void SceneManager::setupCamera2D(){
	//corrigindo o aspecto
	float ratio;
	float xMin = 0.0, xMax = 800.0, yMin = 600.0, yMax = 0.0, zNear = -1.0, zFar = 1.0;


	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

int SceneManager::setupTexture(GLchar *path){
	unsigned int text;

	// load and create a texture 
	// -------------------------
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	size[text - 1][0] = width;
	size[text - 1][1] = height;

	if (data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture" << std::endl;
	}

	this->scale[text - 1].x = size[text - 1][0] / 400.0f;
	this->scale[text - 1].y = size[text - 1][1] / 300.0f;
	this->scale[text - 1].z = 1;

	stbi_image_free(data);


	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return text;
}

void SceneManager::draw(glm::vec3 transform, int index, GLfloat offset, glm::vec3 scale, GLfloat qtdSpritesX, GLfloat qtdSpritesY, GLfloat offsetY, int cor){
	this->clique = cor;

	this->transform[index - 1] = transform;


	this->multScale[index - 1].x = this->scale[index - 1].x * scale.x / qtdSpritesX;
	this->multScale[index - 1].y = this->scale[index - 1].y * scale.y / qtdSpritesY;
	this->multScale[index - 1].z = 1;
	this->qtdSpritesX[index - 1] = qtdSpritesX;
	this->qtdSpritesY[index - 1] = qtdSpritesY;

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	model = glm::translate(model, glm::vec3(this->transform[index - 1]));
	model = glm::scale(model, glm::vec3(this->multScale[index - 1]));

	this->offset[index - 1] = offset * 1 / qtdSpritesX;

	GLint offsetXx = glGetUniformLocation(shader->Program, "offsetX");
	GLint offsetYy = glGetUniformLocation(shader->Program, "offsetY");
	offsetX = this->offset[index - 1];
	this->offsetY = offsetY / qtdSpritesY;

	GLint qtdSpritesXx = glGetUniformLocation(shader->Program, "qtdSpritesX");

	GLint qtdSpritesYy = glGetUniformLocation(shader->Program, "qtdSpritesY");

	GLint cliquee = glGetUniformLocation(shader->Program, "clique");

	glBindTexture(GL_TEXTURE_2D, index);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);


	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(offsetXx, offsetX);
	glUniform1f(offsetYy, this->offsetY);
	glUniform1f(qtdSpritesXx, qtdSpritesX);
	glUniform1f(qtdSpritesYy, qtdSpritesY);
	glUniform1f(cliquee, clique);
	// render container
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	model = glm::scale(model, glm::vec3(1.0f / this->multScale[index - 1].x, 1.0f / this->multScale[index - 1].y, 1.0f / this->multScale[index - 1].z));
	model = glm::translate(model, glm::vec3(-this->transform[index - 1]));
	glBindTexture(GL_TEXTURE_2D, index);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

bool SceneManager::checkCollision(int a, int b){
	if ((transform[a].x + 1) * 400 + size[a][0] / 2 * scale[a].x <= (transform[b].x + 1) * 400 - size[b][0] / 2 * scale[b].x ||
		(transform[a].x + 1) * 400 - size[a][0] / 2 * scale[a].x >= (transform[b].x + 1) * 400 + size[b][0] / 2 * scale[b].x ||
		(transform[a].y - 1) * -300 + size[a][1] / 2 * scale[a].y <= (transform[b].y - 1) * -300 - size[b][1] / 2 * scale[b].y ||
		(transform[a].y - 1) * -300 - size[a][1] / 2 * scale[a].y >= (transform[b].y - 1) * -300 + size[b][1] / 2 * scale[b].y) {
		return false;
	}
	else
		return true;
}

bool SceneManager::checkCollision(int a, int b, glm::vec3 trans){
	if ((trans.x + 1) * 400 + size[a][0] / 2 * scale[a].x <= (transform[b].x + 1) * 400 - (size[b][0] / qtdSpritesX[b]) / 2 * scale[b].x ||
		(trans.x + 1) * 400 - size[a][0] / 2 * scale[a].x >= (transform[b].x + 1) * 400 + (size[b][0] / qtdSpritesX[b]) / 2 * scale[b].x ||
		(trans.y - 1) * -300 + size[a][1] / 2 * scale[a].y <= (transform[b].y - 1) * -300 - size[b][1] / 2 * scale[b].y ||
		(trans.y - 1) * -300 - size[a][1] / 2 * scale[a].y >= (transform[b].y - 1) * -300 + size[b][1] / 2 * scale[b].y) {
		return false;
	}
	else
		return true;
}

int SceneManager::checkButton(double x, double y, int id){
	if ((x >= transform[id].x * 800 - size[id][0] / 2 * scale[id].x &&
		x <= transform[id].x * 800 + size[id][0] / 2 * scale[id].x) &&
		(y >= transform[id].y * 600 - size[id][1] / 2 * scale[id].y &&
			y <= transform[id].y * 600 + size[id][1] / 2 * scale[id].y)) {
		return 1;
	}
	else
		return 0;
}

void SceneManager::tileWalking(){

	cout << "Tile X " << tileX << " " << "Tile Y " << tileY << endl;

	if (tileX >= 0 && tileX < largura && tileY >= 0 && tileY < altura) {
		if ((glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) == GLFW_PRESS && direcao == 0 && /*mapaCaminhavel[tileX - 1][tileY + 1] == 1*/  tileX - 1 >= 0 && tileY + 1 < altura) {
			xAux = characterPositionX;
			yAux = characterPositionY;
			direcao = 3;
		}
		else if ((glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) == GLFW_PRESS && direcao == 0 && /*mapaCaminhavel[tileX + 1][tileY - 1] == 1*/ tileX + 1 < largura && tileY - 1 >= 0) {
			yAux = characterPositionY;
			xAux = characterPositionX;
			direcao = 4;
		}
		else if ((glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP)) == GLFW_PRESS && direcao == 0 && /*mapaCaminhavel[tileX][tileY + 1] == 1*/ tileY + 1 < altura) {
			yAux = characterPositionY;
			xAux = characterPositionX;
			direcao = 1;
		}
		else if ((glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)) == GLFW_PRESS && direcao == 0 && /*mapaCaminhavel[tileX][tileY - 1] == 1*/ tileY - 1 >= 0) {
			yAux = characterPositionY;
			xAux = characterPositionX;
			direcao = 2;
		}
		if (direcao == 1) {
			characterPositionY += ((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2);
			characterPositionX += ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2);
				direcao = 0;
				tileY += 1;
		}
		else if (direcao == 2) {
			characterPositionY -= ((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2);
			characterPositionX -= ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2);
				direcao = 0;
				tileY -= 1;
		}
		else if (direcao == 3) {
			characterPositionX += ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2);
			characterPositionY -= ((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2);
				direcao = 0;
				tileX -= 1;
				tileY += 1;
		}
		else if (direcao == 4) {
			characterPositionX -= ((scale[texture[1] - 1].x / qtdSpritesX[texture[1] - 1]) / 2);
			characterPositionY += ((scale[texture[1] - 1].y / qtdSpritesY[texture[1] - 1]) / 2);
				direcao = 0;
				tileX += 1;
				tileY -= 1;
		}
	}
	else {
		characterPositionX = 0;
		characterPositionY = 0;
		tileX = 0;
		tileY = 0;
	}


}

void SceneManager::lerArqTile(string caminho){
	ifstream tilemap(caminho, ios::in);

	if (!tilemap){
		cout << "Arquivo nao encontrado!" << endl;
	}

	else{
		int iToken;
		while (!tilemap.eof()){
			tilemap >> tile;
			tilemap >> qtdSpritesX[texture[1] - 1];
			tilemap >> qtdSpritesY[texture[1] - 1];
			tilemap >> largura;
			tilemap >> altura;

			for (int y = 0; y < largura; y++)
				for (int x = 0; x < altura; x++)
				{
					tilemap >> iToken;
					mapaX[x][y] = iToken;
				}
			for (int y = 0; y < largura; y++)
				for (int x = 0; x < altura; x++)
				{
					tilemap >> iToken;
					mapaY[x][y] = iToken;
				}
			for (int y = 0; y < largura; y++)
				for (int x = 0; x < altura; x++)
				{
					tilemap >> iToken;
					mapaCaminhavel[x][y] = iToken;
				}

			tilemap.close();
		}
	}
}

bool SceneManager::checkTriangleCollision(glm::vec2 vertA, glm::vec2 vertB, glm::vec2 vertD, glm::vec2 mousePos)
{
	if (calculaArea(vertA, vertB, vertD) == calculaArea(vertA, mousePos, vertB) + calculaArea(mousePos, vertB, vertD) + calculaArea(vertA, mousePos, vertD))
	{
		return true;
	}
	else
	{
		return false;
	}
}

float SceneManager::calculaArea(glm::vec2 vertA, glm::vec2 vertB, glm::vec2 vertD)
{	
	//(X2, X1) * (Y3, Y1) - (X3, X1) * (Y2, Y1)
	float area = abs(((vertB.x - vertA.x) * (vertD.y - vertA.y) - (vertD.x - vertA.x) * (vertB.y - vertA.y)) / 2);

	return area;
}
