CFLAGS = -std=c++11 -Wall
INCLUDE = -F/Library/Frameworks/ -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer
FILES = main.cpp menu.cpp button.cpp text.cpp global.cpp Engine.cpp

all: compile run
	compile && run

compile:
	sudo g++ -o main ${FILES} ${INCLUDE} ${CFLAGS}

run:
	./main > log.txt

cleanup:
	rm main.o && rm main
