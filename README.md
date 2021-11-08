# SuperPlay Extended:
This uses parts of the Super Play codebase for GLES2 based software development on ANGLE and Roku.
* An ANGLE based Visual Studio 2017 project is provided to allow comfortable debugging under ANGLE.
* A makefile is provided to build for Roku.
* A Visual Studio Code configuration is provided for making and cleaning the Roku project.

# Building:
In Makefile.roku:
* Change ROKU_DIR to the directory where the Roku IDK is installed.  
  This will be the directory that contains the version.txt file.
* Change HOST_DIR to the destination directory. This will copy the squashfs.bin file to the destination directory.  
  This is useful, under WSL, to copy the file outside of the WSL environment.
  It can be commented out if not needed.

To build the project, type "make -f Makefile.roku" or use the build key combination in Visual Studio Code.
To clean the project, type "make -f Makefile.roku clean" or use the clean key combination in Visual Studio Code.

# Customizing the project
* ROKU set the location of the Roku build rules and assets.
* BINARY sets the name of the executable and the squashfs file.
* RESOURCE_PATH sets the location of the icon and splash screen files.
* SRCS contains the directories containing source code.
* INCLUDES contains the project specific include paths.