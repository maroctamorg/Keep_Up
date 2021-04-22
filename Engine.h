#ifndef ENGINE_H
#define ENGINE_H

#include "global.h"

/*enum State {
    ON, 
    OFF,
};*/

class Character {
    private:
        std::string fCostume;
        SDL_Point pos;
        SDL_Point vel;
        SDL_Rect colBox;
        int counter {0};

    public:
        Character(char* file, SDL_Point *point);
        Character(SDL_Point point); //test version - replace costume by a square
        ~Character();

        SDL_Point getPos();

        SDL_Point getVel();

        SDL_Rect* getRect();

        void update_speed();

        void move();

        bool checkBounds();

        void draw(SDL_Renderer *renderer);

};

class Obstacle {
    private:
        SDL_Rect shape;
        int vel;
        bool initialized {false};

    public:
        Obstacle(int vel, SDL_Rect shape);
        Obstacle() = default;
        ~Obstacle();

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