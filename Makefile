SRC = $(wildcard source/*.cpp)
LIB = -lGL -lGLU -lSDL -lSDLmain -lSDL_ttf

all: 
	g++ $(SRC) -o RoQubez $(LIB) 

clean:
	@echo Cleaning up...
	@rm RoQubez
	@echo Done.
