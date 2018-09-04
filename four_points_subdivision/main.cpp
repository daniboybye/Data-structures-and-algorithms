#include "subdivision.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define BACKGROUND_COLOR  0.2f, 0.3f,  0.3f

static volatile int SCR_WIDTH  = 800;
static volatile int SCR_HEIGHT = 600;

static Subdivision curve;
static volatile int DRAG_POINT = -1;

inline void updateDraggingPoint(GLFWwindow*);
inline void setCallback(GLFWwindow*);
inline bool inScreen(const Point&);
inline Point getMousePoint(GLFWwindow*);

int main()
{
	if ( !glfwInit() ) return -1;

	auto* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Four-point subdivision", nullptr, nullptr);
	if ( !window )
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	glOrtho(0, SCR_WIDTH, SCR_HEIGHT, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	setCallback(window);
	
	curve.setIsClose(true);

	while ( !glfwWindowShouldClose(window) )
	{
		glClearColor(BACKGROUND_COLOR, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		updateDraggingPoint(window);

		curve.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

inline void setCallback(GLFWwindow *window)
{
	glfwSetWindowSizeCallback(window, [](GLFWwindow *window, const int width, const int height)
	{
		curve.scaling(Point::type(width) / Point::type(SCR_WIDTH), 
			Point::type(height) / Point::type(SCR_HEIGHT));

		SCR_WIDTH = width;
		SCR_HEIGHT = height;

		glOrtho(0, width, height, 0, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		return;
	});


	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, const int width, const int height)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		return;
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, const int button, const int action, const int)
	{
		switch (action)
		{
		case GLFW_RELEASE:
		{
			DRAG_POINT = -1;
			return;
		}
		case GLFW_PRESS:
		{
			const auto temp = getMousePoint(window);

			if ( !inScreen(temp) )
				return;

			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
			{
				assert(DRAG_POINT == -1);

				auto it = std::find_if(curve.cbegin(), curve.cend(), [temp](auto& p) {
					return temp.isNear(p, 10.0);
				});

				if (it == curve.cend())
					curve.addPoint(temp);
				else
					DRAG_POINT = std::distance(curve.cbegin(), it);

				return;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				curve.erasePointsNearTo(temp);
				return;
			}
			default:
				return;
			}
		}
		}
	});

	glfwSetKeyCallback(window, [](GLFWwindow *window, const int key, const int, const int action, const int)
	{
		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE :

				glfwSetWindowShouldClose(window, GLFW_TRUE);
				return;

			case GLFW_KEY_Q :

				curve.nextIteration();

				return;

			case GLFW_KEY_C:
				
				curve.clear();
				
				return;

			case GLFW_KEY_A:

				curve.setIsClose(!curve.curveIsClose());

				return;

			default:
				return;
			}
		}
		return;
	});
}

inline void updateDraggingPoint(GLFWwindow *window)
{
	if (DRAG_POINT != -1)
	{
		auto temp = getMousePoint(window);

		if (inScreen(temp))
			curve.setPoint(DRAG_POINT, temp);
	}
}

inline bool inScreen(const Point& p)
{
	return p.p.first >= 0 && p.p.first <= Point::type(SCR_WIDTH) &&
		p.p.second >= 0 && p.p.second <= Point::type(SCR_HEIGHT);
}

inline Point getMousePoint(GLFWwindow *window)
{
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	return Point(x, y);
}