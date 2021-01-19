#include"Window.h"
#include"Context.h"

namespace ChoicePlus
{
	Window::Window(const std::string& title, uint32_t w, uint32_t h)
		:mTitle(title), mWidth(w), mHeight(h)
	{
		Create();
	}

	Window::~Window()
	{
		glfwTerminate();
		glfwDestroyWindow(mWindow);
	}

	void Window::Create()
	{
		cpassert(glfwInit());
		mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), 0, 0);
		cpassert(mWindow);
		glfwMaximizeWindow(mWindow);
		Context context;
		context.Create(mWindow);
	}
}