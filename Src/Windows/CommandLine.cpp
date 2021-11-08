// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <vector>

#include "CommandLine.h"
#include "CommandLineFlags.h"
#include "Functions.h"

void processCommandLine(const std::string& _strCommandLine)
{
	std::vector<std::string>	vecParameters	= Functions::tokenize(_strCommandLine, ' ');

	size_t	t_c	= vecParameters.size();

	for (size_t iLoop = 0; iLoop < t_c; ++iLoop)
	{
		const std::string&	strParameter	= vecParameters[iLoop];

		if ("TestMode" == strParameter)
		{
			setFlags(TestModeFlag);
		}
	}
}
