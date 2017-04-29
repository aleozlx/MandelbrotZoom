#include <iostream>
#include <complex>
#include <cstdint>
#include <vector>
#include <fstream>
// https://github.com/robotjatek/MandelbrotSet/blob/master/MandelbrotSet/Main.cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern "C" {

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

}

#define SCREEN_SIZE_X 1280
#define SCREEN_SIZE_Y 720
#define MOVE_SPEED 0.02

uint8_t* screen = new uint8_t[SCREEN_SIZE_X * 3 * SCREEN_SIZE_Y];

SDL_Window* wnd;
SDL_Renderer* renderer;
SDL_Texture* screentex;
SDL_Event s_event;
SDL_GLContext context;
unsigned int WINDOW_WIDTH = SCREEN_SIZE_X;
unsigned int WINDOW_HEIGHT = SCREEN_SIZE_Y;

glm::mat4 modelmat = glm::mat4(1);
glm::mat4 viewmat = glm::mat4(1);
glm::mat4 projmat = glm::ortho(-1, 1, -1, 1);
glm::mat4 MVP = projmat*viewmat*modelmat;

double zoomfactor = 1.0f;
double centerx = 0.0f;
double centery = 0.0f;
int iterations = 28;

bool InsideMandelbrot(const std::complex<double>& Z)
{
	return std::abs(Z) < 2 ? true : false;
}

std::complex<double> CalculateComplex(const std::complex<double>& C, unsigned int Iteration)
{
	std::complex<double> ret(0, 0);
	for (unsigned int i = 0; i < Iteration; i++)
	{
		ret = ret*ret + C;
	}
	return ret;
}

uint8_t NumOfIterations(const std::complex<double>& num, unsigned int MaxIterations)
{
	unsigned int i = 1;
	while (InsideMandelbrot(CalculateComplex(num, i++)) && i < MaxIterations);
	return i - 1;
}

void InitScreen(unsigned int Iterations)
{
	const double stepx = 4.0 / SCREEN_SIZE_X;
	const double stepy = 4.0 / SCREEN_SIZE_Y;

	for (int i = 0; i < SCREEN_SIZE_X; i++)
	{
		for (int j = 0; j < SCREEN_SIZE_Y; j++)
		{
			uint8_t nit = NumOfIterations(std::complex<double>(stepx*i - 2, stepy*j - 2), Iterations);
			/*if (colors.size() < nit)
			{
			Color c;
			c.r =callnumber+=5;
			c.g = rand() % 256;
			c.b = rand() % 256;
			colors.push_back(c);
			}*/
			screen[i * 3 + j * SCREEN_SIZE_X * 3] = nit * 10;
			screen[i * 3 + j * SCREEN_SIZE_X * 3 + 1] = 0;
			screen[i * 3 + j * SCREEN_SIZE_X * 3 + 2] = 0;
		}
	}
}

void HandleKeys(SDL_Event& ev, bool& exit)
{
	if (ev.type == SDL_KEYDOWN)
	{
		if (ev.key.keysym.sym == SDLK_ESCAPE)
		{
			exit = true;
		}
		
		if (ev.key.keysym.sym == SDLK_KP_PLUS)
		{
			zoomfactor -= (MOVE_SPEED * zoomfactor);
		}
		else if (ev.key.keysym.sym == SDLK_KP_MINUS)
		{
			zoomfactor += (MOVE_SPEED * zoomfactor);
		}
		
		if (ev.key.keysym.sym == SDLK_w)
		{
			centery += (MOVE_SPEED * zoomfactor);
		}
		else if (ev.key.keysym.sym == SDLK_s)
		{
			centery -= (MOVE_SPEED * zoomfactor);
		}
		
		if (ev.key.keysym.sym == SDLK_d)
		{
			centerx += (MOVE_SPEED * zoomfactor);
		}
		else if (ev.key.keysym.sym == SDLK_a)
		{
			centerx -= (MOVE_SPEED * zoomfactor);
		}

		if (ev.key.keysym.sym == SDLK_q)
		{
			iterations--;
			if (iterations < 0)
			{
				iterations = 0;
			}
		}
		else if (ev.key.keysym.sym == SDLK_e)
		{
			iterations++;
			if (iterations < 0)
			{
				iterations = 0;
			}
		}
		else if (ev.key.keysym.sym == SDLK_r)
		{
			iterations = 0;
		}

	}
	else if (ev.type == SDL_WINDOWEVENT)
	{
		if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			WINDOW_WIDTH = ev.window.data1;
			WINDOW_HEIGHT = ev.window.data2;
			glViewport(0, 0, ev.window.data1, ev.window.data2);
			projmat = glm::ortho(-1, 1, -1, 1);
			MVP = projmat*viewmat* modelmat;
		}
	}
}

void MandelbrotCPU()
{
	SDL_Init(SDL_INIT_VIDEO);

	wnd = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	screentex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, SCREEN_SIZE_X, SCREEN_SIZE_Y);

	bool exit = false;
	SDL_Event ev;
	//system("pause");
	while (!exit)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				exit = true;
			}

			HandleKeys(ev, exit);
		}
		static unsigned int i = 1;
		//std::cout << i << "\n";
		if (i < 28)
		{
			InitScreen(i++);
		}

		SDL_UpdateTexture(screentex, nullptr, screen, SCREEN_SIZE_X * 3);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screentex, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(screentex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(wnd);
	SDL_Quit();
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}



void MandelbrotGPU()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	wnd = SDL_CreateWindow("Mandelbrot GPU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	context = SDL_GL_CreateContext(wnd);
	printf("%d\n", SDL_GL_SetSwapInterval(0));
	glewExperimental = GL_TRUE;
	glewInit();

	printf("%s\n",glGetString(GL_RENDERER));

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = { -1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f, };

	glClearColor(1.0f, 0, 0, 0);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint programID = LoadShaders("vertex.vert", "fs.frag");
	

	bool exit = false;
	SDL_Event ev;
	while (!exit)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				exit = true;
			}
			HandleKeys(ev, exit);
		}
		glClear(GL_COLOR_BUFFER_BIT);

		GLuint wndxuni = glGetUniformLocation(programID, "WINDOW_SIZE_X");
		glUniform1i(wndxuni, WINDOW_WIDTH);
		GLuint wndyuni = glGetUniformLocation(programID, "WINDOW_SIZE_Y");
		glUniform1i(wndyuni, WINDOW_HEIGHT);
		GLuint zoomuni = glGetUniformLocation(programID, "zoomfactor");
		glUniform1d(zoomuni, zoomfactor);
		GLuint centeryuni = glGetUniformLocation(programID, "centery");
		glUniform1d(centeryuni, centery);
		GLuint centerxuni = glGetUniformLocation(programID, "centerx");
		glUniform1d(centerxuni, centerx);
		GLuint userit = glGetUniformLocation(programID, "useriterations");
		glUniform1i(userit, iterations);
		GLuint mvpid = glGetUniformLocation(programID, "MVP");
		glUniformMatrix4fv(mvpid, 1, GL_FALSE, &MVP[0][0]);

		glUseProgram(programID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0,3,GL_FLOAT,	GL_FALSE,0,(void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);


		SDL_GL_SwapWindow(wnd);
	}

	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(wnd);
	SDL_Quit();
}

int main(void)
{
	//MandelbrotCPU();
	MandelbrotGPU();
	delete[] screen;
	return 0;
}


