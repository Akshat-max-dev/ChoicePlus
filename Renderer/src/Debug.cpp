#ifdef DEBUG

#include"Debug.h"
#include<glad/glad.h>
#include"src/Log.h"

namespace ChoicePlus
{
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
		GLenum severity, GLsizei length, const char* message, const void* userParam)
	{
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		CONSOLE("---------------------------------------------------------");
		std::string msg = message;
		CONSOLE("---------------------------------------------------------");

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               CONSOLE("Type: Error{w}"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: CONSOLE("Type: Deprecated Behaviour{w}");break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  CONSOLE("Type: Undefined Behaviour{w}"); break;
		case GL_DEBUG_TYPE_PORTABILITY:         CONSOLE("Type: Portability{w}"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:         CONSOLE("Type: Performance{w}"); break;
		case GL_DEBUG_TYPE_MARKER:              CONSOLE("Type: Marker{w}"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          CONSOLE("Type: Push Group{w}"); break;
		case GL_DEBUG_TYPE_POP_GROUP:           CONSOLE("Type: Pop Group{w}"); break;
		case GL_DEBUG_TYPE_OTHER:               CONSOLE("Type: Other{w}"); break;
		} 

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         msg.append("{e}"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       msg.append("{e}"); break;
		case GL_DEBUG_SEVERITY_LOW:          break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: break;
		}

		CONSOLE(msg.c_str());
	}

	void EnableOpenGLDebugging()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}

#endif // DEBUG