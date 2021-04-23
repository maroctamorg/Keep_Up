#ifndef ENGINE_H
#define ENGINE_H

#include "global.h"

/*enum State {
    ON, 
    OFF,
};*/

class Character {
    private:
        SDL_Texture *texture0;
        SDL_Texture *texture1;
        int animationState {0};
        SDL_Point pos;
        SDL_Point vel;
        SDL_Rect colBox;
        int counter {0};

    public:
        Character(SDL_Texture *texture0, SDL_Texture *texture1, SDL_Point *point);
        //Character(SDL_Point point); //test version - replace costume by a square
        ~Character();

        void reset(SDL_Point *point);

        SDL_Point getPos();

        SDL_Point getVel();

        SDL_Rect* getRect();

        void update_speed();

        void move();

        bool checkBounds();

        void draw(SDL_Renderer *renderer);

        void destroyTexture();

};

class Obstacle {
    private:
        SDL_Rect shape {g::W_W, g::W_H, 0, 0};
        int vel {0};
        bool initialized {false};

    public:
        Obstacle(int vel, SDL_Rect shape);
        Obstacle() = default;
        ~Obstacle();

        void assign(int vel, int pos_y, int height);
        void assign(int vel, int distance, int pos_y, int height);

        SDL_Point getPos();

        int getWidth();
        int getHeight();

        bool isInitialized();
        bool deInitialize();

        int getVel();
        void update_speed(int increment);

        void move();

        bool checkCollision(SDL_Rect *character);

        void draw(SDL_Renderer *renderer);
};

class Obstacle_Generator {
    private:
        std::vector<Obstacle> obstacles;
        int rate;
        int count;
        int range[2];
        int vel;


    public:
        Obstacle_Generator(int rate, int beginRange, int endRange, int initialVel);
        ~Obstacle_Generator();

        void reset();

        void generateObstacles();

        void updateObstacles();

        bool checkCollisions(SDL_Rect *character);

        void draw(SDL_Renderer *renderer);
};

/*
class Obstacle_Generator {
    private:
        Obstacle *obstacles;
        int length;
        int rate;
        int count;
        int range[2];


    public:
        Obstacle_Generator(int length, int rate, int beginRange, int endRange);
        ~Obstacle_Generator();

        void generateObstacles();

        void updateObstacles();

        void draw(SDL_Renderer *renderer);
};
*/
#endif