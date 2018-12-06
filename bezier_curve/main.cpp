#include "bezierCurve.h"
#include "point.h"

#include <iostream>
#include <algorithm>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define BACKGROUND_COLOR  0.2f, 0.3f,  0.3f
#define POINT_COLOR  153.0f , 0.0f   , 153.0f
#define POINT_WIDTH 27
#define LINE2_WIDTH  4
#define LINE2_COLOR  255.0f , 128.0f , 0.0f

static int SCR_WIDTH  = 800;
static int SCR_HEIGHT = 600;

static std::vector<Point> points;
static BezierCurve curve;
static int DRAG_POINT = -1;
static bool DRAW_LINES = true;

static void updateDraggingPoint(GLFWwindow*);
static void setCallback(GLFWwindow*);
static bool inScreen(const Point&);
static Point getMousePoint(GLFWwindow*);

void erasePointsNearTo(const Point& p)
{
	points.erase(std::remove_if(points.begin(), points.end(), [p](const Point& t) {
		return p.isNear(t, POINT_WIDTH / 3);
	}), points.end());

	curve.createCurve(points);
}

int main()
{
	if ( !glfwInit() ) return -1;

	auto* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bezier Curve", nullptr, nullptr);
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

	while ( !glfwWindowShouldClose(window) )
	{
		glClearColor(BACKGROUND_COLOR, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		updateDraggingPoint(window);

		if (DRAW_LINES) {
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(sizeof(Point) / sizeof(Point::type), Point::typeFlag, 0, points.data());

			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_LINE_STIPPLE);
			glPushAttrib(GL_LINE_BIT);
			glLineWidth(LINE2_WIDTH);
			glColor3f(LINE2_COLOR);
			glDrawArrays(GL_LINE_STRIP, 0, points.size());
			glPopAttrib();
			glDisable(GL_LINE_SMOOTH);

			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPointSize(POINT_WIDTH);
			glColor3f(POINT_COLOR);
			glDrawArrays(GL_POINTS, 0, points.size());
			glDisable(GL_POINT_SMOOTH);

			glDisableClientState(GL_VERTEX_ARRAY);
		}

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
		scalingPoints(points, Point::type(width) / Point::type(SCR_WIDTH),
			Point::type(height) / Point::type(SCR_HEIGHT));
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

				auto it = std::find_if(points.cbegin(), points.cend(), [temp](auto& p) {
					return temp.isNear(p, 10.0);
				});

				if (it == points.cend())
					points.push_back(temp);
				else
					DRAG_POINT = std::distance(points.cbegin(), it);

				curve.createCurve(points);
				return;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				erasePointsNearTo(temp);
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

				DRAW_LINES = !DRAW_LINES;

				return;

			case GLFW_KEY_C:
				points.clear();
				curve.createCurve(points);
			default:
				return;
			}
		}
		return;
	});
}

static void updateDraggingPoint(GLFWwindow *window)
{
	if (DRAG_POINT != -1)
	{
		auto temp = getMousePoint(window);

		if (inScreen(temp)) {
			points[DRAG_POINT] = temp;
			curve.createCurve(points);
		}
	}
}

static bool inScreen(const Point& p)
{
	return p.p.first >= 0 && p.p.first <= Point::type(SCR_WIDTH) &&
		p.p.second >= 0 && p.p.second <= Point::type(SCR_HEIGHT);
}

static Point getMousePoint(GLFWwindow *window)
{
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	return Point(x, y);
}