# Absolute current directory
ABSCURDIR		:= $(strip $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))))
UNAME			:= $(shell uname)

# for objects and exe output
BUILDDIR		:= ../Build
# object file generation path (don't distribute this)
WORKINGDIR		= $(BUILDDIR)/Objects
# modules path
SOURCEDIR		:= ../Source
LIBRARIESDIR	:= ../Libraries
BUNDLEDDIR		:= ../Bundled
RESOURCESDIR	:= $(BUNDLEDDIR)/Resources

# executables
CC				:= $(CC)
CXX				:= $(CXX)
# LD shouldn't be invoked directly..? perhaps remove this...
LD				:= $(LD)

CFLAGS			:= -Wall -Wextra
CXXFLAGS		:= -Wall -Wextra -std=c++11 -Wunused-parameter
LDFLAGS			:= 

# TODO: specify source files then patsubst to find object files
OBJECTS			:= \
	Source.o \
	LoadTexture.o \
	Game.o \
	BMPF.o
