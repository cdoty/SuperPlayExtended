#pragma once

#ifdef __APPLE__
#include <TargetConditionals.h>
#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#include <OpenGLES/ES1/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#elif defined __ANDROID__ || defined ANGLE || defined ROKU || defined STEAMLINK
#include <GLES2/gl2.h>
#else
#include <GL/gl.h>
#endif

#include "Macros.h"

class Program
{
	public:
		PTR(Program)

		// Destructor
		~Program();

		// Create
		static Ptr create(const std::string& _strVertexShader, const std::string& _strFragmentShader);

		// Initialize
		bool initialize(const std::string& _strVertexShader, const std::string& _strFragmentShader);

		// Close
		void close();

		// Get program handle
		GLuint getProgramHandle() const {return m_program;}

	private:
		GLuint	m_program;	// Program 

		// Constructor
		Program();
};
