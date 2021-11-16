#pragma once

#include "Macros.h"
#include "SystemGL.h"

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
