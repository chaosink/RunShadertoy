GLFW = `pkg-config --cflags glfw3` `pkg-config --libs --static glfw3`
GLEW = `pkg-config --cflags glew` `pkg-config --libs glew`
common = shader.cpp
target = RunWebGL

all: $(target)

$(target): $(target).cpp
	clang++ $(target).cpp -o $(target) $(common) $(GLFW) $(GLEW)