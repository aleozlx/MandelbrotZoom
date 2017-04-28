#include <iostream>
#include <complex>
#include <cstdint>
#include <vector>
#include <fstream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

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

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
