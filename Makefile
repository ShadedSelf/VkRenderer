
#export VK_LOADER_DEBUG=all
export VK_INSTANCE_LAYERS = VK_LAYER_LUNARG_standard_validation

NAME = main

INC = -I ${VK_SDK}/macos/include -I ./glfw/include -I ./glfw/deps -I ./renderer/helpers -I ./renderer/classes -I ./vectors -I ./camera
SHADERS = ./shaders
CC = g++-8 -std=c++17
OB = ./obs
FLAGS = -g3

all:
	@${VK_SDK}/macOS/bin/glslc -fshader-stage=fragment 	$(SHADERS)/shader.frag 	-o $(OB)/frag.spv -Werror
	@${VK_SDK}/macOS/bin/glslc -fshader-stage=vertex 	$(SHADERS)/shader.vert 	-o $(OB)/vert.spv -Werror

	@${VK_SDK}/macOS/bin/glslc -fshader-stage=compute 	$(SHADERS)/set.comp		-o $(OB)/set.spv -Werror
	@${VK_SDK}/macOS/bin/glslc -fshader-stage=compute 	$(SHADERS)/shader.comp	-o $(OB)/comp.spv -Werror


	@$(CC) $(FLAGS) $(INC) -o $(OB)/testmain.cpp.o -c testmain.cpp
	@$(CC) -Wl,-search_paths_first -Wl,-headerpad_max_install_names  $(OB)/*.o  -o $(NAME)		\
		-Wl,-rpath,${VK_SDK}/macos/lib ./glfw/src/libglfw3.a									\
		${VK_SDK}/macos/lib/libvulkan.dylib -framework Cocoa -framework IOKit					\
		-framework CoreFoundation -framework CoreVideo $(FLAGS)
	@./main
donotdothis:
	@rm -rf $(OB)/*
re: donotdothis all
run:
	@./main


