include ./Variables.mk

# this variable isn't really needed, only because LDFLAGS splits the framework path into directory and file...
LIBRARIESDIR	:= $(LIBRARIESDIR)/macOS
SDL2			:= $(LIBRARIESDIR)/SDL2.framework

# Extend the makefile.variables
CXXFLAGS		+= -I $(SDL2)/Headers
LDFLAGS			+= -F $(LIBRARIESDIR) -framework SDL2 -rpath @executable_path/../Frameworks

BUILDDIR		:= $(BUILDDIR)/macOS

# output binary path
EXECUTABLE		:= $(WORKINGDIR)/Egg
APP				:= $(BUILDDIR)/Egg.app

# we need to have this here, as this will be evaluated with a platform inspecific builddir if put in variables.mk
# prepend BUILDDIR to each object file
OBJECTS			:= $(addprefix $(WORKINGDIR)/, $(OBJECTS))

.PHONY: run
run: $(APP)
#	@open $(APP)
	$(APP)/Contents/MacOS/Egg

# package the executable into a .app...
# Using -r to copy the SDL2 framework as .frameworks are actually directories
$(APP): $(EXECUTABLE)
	@mkdir -p $(APP)/Contents/Frameworks
	@mkdir -p $(APP)/Contents/MacOS
	@cp $(EXECUTABLE) $(APP)/Contents/MacOS/
	@cp -r $(SDL2) $(APP)/Contents/Frameworks/
	@cp -r $(RESOURCESDIR) $(APP)/Contents/
	@cp $(BUNDLEDDIR)/AppIcon.icns $(APP)/Contents/Resources/AppIcon.icns
	@cp $(BUNDLEDDIR)/Info.plist $(APP)/Contents/Info.plist

# TODO: move these basic build commands into a generic mk file?
# TODO: recompile on header file change (see Links folder)

# link the object files into the executable, if objects are newer than the executable
# order of libaries (i.e. LDFLAGS) is important: https://stackoverflow.com/a/35712317
$(EXECUTABLE): $(OBJECTS)
	@$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

# rules for module files
$(WORKINGDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-@rm -rf $(BUILDDIR)

.PHONY: setup
setup:
# call SDL's configure script?
# build a cross compiler and compile it's own SDL port?
