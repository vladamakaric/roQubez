SRC = $(wildcard source/*.cpp)
LIB = -lSDL2_ttf 

all: 
	g++ $(SRC) -o RoQubez $(LIB) `sdl2-config --cflags --libs` -framework OpenGL

clean:
	@echo Cleaning up...
	@rm RoQubez
	@echo Done.