include ./Variables.mk

# perhaps have a windows 32-bit port?

CXX				:= x86_64-w64-mingw32-g++

# We be building for x64 because screw 32-bit systems!
# this variable isn't really needed, only because LDFLAGS splits the framework path into directory and file...
SDL2DIR			:= $(LIBRARIESDIR)/SDL2-devel-2.26.1-mingw/x86_64-w64-mingw32

# Extend the makefile.variables
CXXFLAGS		+= -I $(SDL2DIR)/include/SDL2
# I would run $(SDL2DIR)/sdl2-config --static-libs in here but it prints flags for the current system (macOS atm) and not the target system (Windows)
# The second line is what it would output on Windows (https://stackoverflow.com/a/61572482)
LDFLAGS			+= -L $(SDL2DIR)/lib -static -static-libgcc -static-libstdc++ -lpthread \
-lmingw32 -lSDL2main -lSDL2 -mwindows  -Wl,--no-undefined -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

BUILDDIR		:= $(BUILDDIR)/Windows

# output binary path
EXECUTABLE		:= $(BUILDDIR)/Egg.exe

# prepend BUILDDIR to each object file
OBJECTS			:= $(addprefix $(WORKINGDIR)/, $(OBJECTS))
RESOURCESCRIPT	:= $(WORKINGDIR)/WindowsResourceScript.o

.PHONY: run
run: $(EXECUTABLE)
	$(EXECUTABLE)

# link the object files into the executable, if objects are newer than the executable
# the executable is statically linked, so no need to copy over the SDL2.dll
$(EXECUTABLE): $(OBJECTS) $(RESOURCESCRIPT)
	@$(CXX) -o $@ $(OBJECTS) $(RESOURCESCRIPT) $(LDFLAGS)
#	@cp -r $(RESOURCESDIR) $(BUILDDIR)

# rules for module files
$(WORKINGDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(RESOURCESCRIPT):
	@x86_64-w64-mingw32-windres $(BUNDLEDDIR)/WindowsResourceScript.rc $(RESOURCESCRIPT)

.PHONY: clean
clean:
	-@rm -rf $(BUILDDIR)
