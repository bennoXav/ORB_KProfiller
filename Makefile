SOURCEDIR :=src
HEADERDIR :=include
EXENAME :=game.exe
CXX := gcc
CXXFLAGS := testes3.c -o orbKprofiller.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm 

run:
	@$(CXX) $(CXXFLAGS)
	@echo ".exe created"
	
