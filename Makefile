CFLAGS = -std=c++11 -Wall
INCLUDE = -F/Library/Frameworks/SDL2.framework/headers -framework SDL2 -F/Library/Frameworks/SDL2_image.framework/headers -framework SDL2_image -F/Library/Frameworks/SDL2_ttf.framework/headers -framework SDL2_ttf
FILES = main.cpp menu.cpp button.cpp text.cpp global.cpp Engine.cpp

all: compile run
	compile && run

compile:
	g++ -o main ${FILES} ${INCLUDE} ${CFLAGS}

run:
	./main > log.txt

cleanup:
	rm main.o && rm main
