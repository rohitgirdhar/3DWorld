all:
	g++ house/*.cpp car/glm/*.c *.cpp terrain/*.cpp car/*.cpp car/wheel/*.cpp -lglut -lGLU -lGL -lopenal -lalut -O2 -g
run:
	optirun ./a.out
clean:
	rm -rf a.out
