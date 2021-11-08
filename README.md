# SuperPlay Extended:
This is an updated version of the Super Play codebase extended to support other GLES2 based systems.
* An ANGLE based Visual Studio 2017 project is provided to allow comfortable debugging under ANGLE.
* A makefile is provided to build for Roku.
* A Visual Studio Code configuration is provided for making and cleaning the Roku project.

# Building:
In Makefile.roku:
Change ROKU_DIR to the directory where the Roku IDK is installed.
  This will be the directory that contains the version.txt file.
Change HOST_DIR to the destination directory. This will copy the squashfs.bin file to the destination directory.
  This is useful, under WSL, to copy the file outside of the WSL environment.
  It can be commented out if not needed.
