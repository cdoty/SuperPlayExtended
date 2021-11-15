// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Log.h"
#include "Program.h"

Program::Program()	:
	m_program(0)
{
}

Program::~Program()
{
	close();
}

Program::Ptr Program::create(const std::string& _strVertexShader, const std::string& _strFragmentShader)
{
	INSTANCE(pProgram, Program())

	if (false == pProgram->initialize(_strVertexShader, _strFragmentShader))
	{
		pProgram.reset();
	}

	return	pProgram;
}

bool Program::initialize(const std::string& _strVertexShader, const std::string& _strFragmentShader)
{
	return	true;
}

void Program::close()
{
}
