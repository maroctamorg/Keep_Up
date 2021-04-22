#include "global.h"

namespace g{
    extern const int W_W { 800 };
    extern const int W_H { 500 };

    extern const char* saveFile {"resources/savefile.txt"};
    extern const std::string font {"resources/Krungthep.ttf"};
    extern const SDL_Colour scoreCounterTxtColour { 250, 250, 250, 250 };
    extern const SDL_Rect scoreCounterRect {W_W - 75, W_H - 50, 75, 50};
    extern const SDL_Colour scoreCounterRectColour { 0, 0, 0, 250};
}

