#include "Display.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include <iostream>

using namespace std;

Display::Display(int width, int height, const std::string& title)
{
	m_width = width;
	m_height = height;

	// TODO: Look into reducing the number of items included
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Enable multisampling
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL);
	SDL_SetWindowResizable(m_window, SDL_TRUE);
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cerr << "Derydoca Engine" << std::endl;
	}

	m_isClosed = false;

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

/*
	// Print information about the current instance of OpenGL
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "Vendor: " << vendor << endl;
	cout << "Version: " << version << endl;
	cout << "GLSL Version: " << glslVersion << endl;
	cout << "Max Vertex Attributes: " << GL_MAX_VERTEX_ATTRIBS << endl;

	GLint nExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
	cout << "Supported extensions (" << nExtensions << "):" << endl;
	for (GLint i = 0; i < nExtensions; i++)
	{
		cout << "    " << i << ": " << glGetStringi(GL_EXTENSIONS, i) << endl;
	}
*/

	m_keyboard = InputManager::getInstance().getKeyboard();

	DisplayManager::getInstance().addDisplay(this);
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

bool Display::isClosed() {
	return m_isClosed;
}

void Display::bindAsRenderTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Display::init(int width, int height, const std::string & title)
{
}

void Display::update() {
	SDL_GL_SwapWindow(m_window);

	m_keyboard->update();

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			m_isClosed = true;
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				m_isClosed = true;
				m_keyboard->setKeyState(e.key.keysym.sym, true);
				break;
			default:
				m_keyboard->setKeyState(e.key.keysym.sym, true);
				break;
			}
			break;
		case SDL_KEYUP:

			m_keyboard->setKeyState(e.key.keysym.sym, false);
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				m_width = e.window.data1;
				m_height = e.window.data2;
				SDL_SetWindowSize(m_window, m_width, m_height);
				//SDL_RenderSetLogicalSize(m_window, m_width, m_height);
				//SDL_GL_DeleteContext(m_glContext);
				//m_glContext = SDL_GL_CreateContext(m_window);
				
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}