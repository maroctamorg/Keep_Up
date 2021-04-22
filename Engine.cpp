#include "global.h"
#include "Engine.h"

Character::Character(SDL_Texture *texture0, SDL_Texture *texture1, SDL_Point *point)
    : texture0 {texture0}, texture1 {texture1}, animationState {0}, pos {*point}, vel {5, 0}, colBox {point->x, point->y, 25, 25} {}

//Character::Character(const SDL_Point point)
//    : pos {point}, vel {0, 0}, colBox{point.x, point.y, 50, 50} {}

Character::~Character(){}

SDL_Point Character::getPos() {
    return pos;
}

SDL_Point Character::getVel(){
    return vel;
}

void Character::update_speed() {
    
    if(vel.y >= 0) {
        vel.y -= 4; //3 / 5
    }
    
    //vel.y -= 3;
    
    /*while(vel.y > -3){
        vel.y -= 1;
    }*/
    //std::cout << "updated speed: " << vel.y << '\n';
}

void Character::move(){
    counter++;
    if(counter % 3 == 0) {
        animationState = (animationState + 1) % 2;
    }
    
    //std::cout << "current speed: " << vel.y << '\n';

    //pos.x += vel.x;
    if(pos.y > 0) {
        pos.y += vel.y;
    } else {
        pos.y += 1;
    }

    if(counter == 40) { //50 / 25
        counter = 0;
        if(vel.y < 2){
            vel.y += 1;
        }
    }
    colBox.x = pos.x - 10;
    colBox.y = pos.y - 10;
}

SDL_Rect* Character::getRect() {
    return &colBox;
}

bool Character::checkBounds() {
    return colBox.y < g::W_H;
}

void Character::draw(SDL_Renderer *renderer) {
    //std::cout << "drawing CHARACTER" << '\n';
    //SDL_Rect src_rect {1000, 400, 200, 200};
    SDL_Rect dest_rect {pos.x - 100, pos.y - 50, 250, 150};
    switch(animationState) {
        case 0:
            SDL_RenderCopy(renderer, texture0, NULL, &dest_rect);
            break;
        case 1:
            SDL_RenderCopy(renderer, texture1, NULL, &dest_rect);
            break;
    }
}

void Character::destroyTexture() {
    if(texture0 != nullptr && texture0 == NULL) {
        SDL_DestroyTexture(texture0);
        texture0 = nullptr;
    }

    if(texture1 != nullptr && texture1 == NULL) {
        SDL_DestroyTexture(texture1);
        texture1 = nullptr;
    }
}

Obstacle::Obstacle(int vel, SDL_Rect shape)
    : shape{shape}, vel{ vel }, initialized{ true } {}

//Obstacle::Obstacle() : initialized {false} {}

Obstacle::~Obstacle() {

}

SDL_Point Obstacle::getPos() {
    SDL_Point point {shape.x, shape.y};
    return point;
}

int Obstacle::getWidth() {
    return shape.w;
}

int Obstacle::getHeight() {
    return shape.h;
}

bool Obstacle::isInitialized() {
    return initialized;
}

bool Obstacle::deInitialize() {
    initialized = false;
}

int Obstacle::getVel() {
    return vel;
}

void Obstacle::move(){
    shape.x += vel;
}

void Obstacle::update_speed(int increment) {
    vel += increment;
}

bool Obstacle::checkCollision(SDL_Rect *character) {
    return SDL_HasIntersection(character, &shape);
}

void Obstacle::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderFillRect(renderer, &shape);

}


Obstacle_Generator::Obstacle_Generator(int rate, int beginRange, int endRange, int initialVel)
    : rate{ rate }, range{ beginRange, endRange }, obstacles(5), vel {initialVel}, count{0} {
        srand(time(NULL));
    }

Obstacle_Generator::~Obstacle_Generator() {}

void Obstacle_Generator::generateObstacles() {
    count++;
    
    //std::cout << "Call to object generator." << '\n';
    //std::cout << "rate: " << rate << ", count: " << count <<  ": " << rate*count % 50 << '\n';

    // need to fix this part of the code
    if(rate*count % 1000 == 0) {
        
        //std::cout << "Checking object generation... " << '\n';

        for(int i = 0; i < 5; i++) {
            if(!obstacles.at(i).isInitialized()) {
                std::cout << "Generating object" << '\n';
                int height = rand()%range[1] + range[0];
                int pos = ((rand()+1)%2)*(g::W_H-height);
                SDL_Rect shape {g::W_W, pos, 75, height};
                obstacles.at(i) = Obstacle(vel, shape);
                return;
            }
        }   
    }
    
    
}

void Obstacle_Generator::updateObstacles(){
    for(int i = 0; i < 5; i++) {
        if(obstacles.at(i).isInitialized()) {
            std::cout << "Moving object..." << '\n';
            obstacles.at(i).move();
            obstacles.at(i).update_speed(-count/50000); //50000
            if (obstacles.at(i).getPos().x + obstacles.at(i).getWidth() < 0 ) {
                std::cout << "Deinitializing object..." << '\n';
                obstacles.at(i).deInitialize();
            }
        }
    }
}

bool Obstacle_Generator::checkCollisions(SDL_Rect *character) {
    for(int i = 0; i < 5; i++) {
        if(obstacles.at(i).isInitialized()) {
            if(obstacles.at(i).checkCollision(character)){
                return true;
            }
        }
    }

    return false;
}

void Obstacle_Generator::draw(SDL_Renderer *renderer) {
    for(int i = 0; i < 5; i++) {
        if(obstacles.at(i).isInitialized()) {
            //std::cout << "drawing obstacle" << '\n';
            obstacles.at(i).draw(renderer);
        }
    }


}

/*
Obstacle_Generator::Obstacle_Generator(int length, int rate, int beginRange, int endRange)
    : rate{ rate }, range{ beginRange, endRange }, length {length}{
        assert(length >= 0);
        if (length > 0) {
            obstacles = new Obstacle[length]{};
        }
        srand(time(NULL));
    }

Obstacle_Generator::~Obstacle_Generator() {
    delete[] obstacles;
}

void Obstacle_Generator::generateObstacles() {
    count++;
    
    //std::cout << "Call to object generator." << '\n';
    //std::cout << "rate: " << rate << ", count: " << count <<  ": " << rate*count % 50 << '\n';

    // need to fix this part of the code
    if(rate*count % 50 == 0) {
        
        std::cout << "Checking object generation... " << '\n';

        for(int i = 0; i < length; i++) {
            if(!obstacles[i].isInitialized() || obstacles[i].getPos().x < 0) {
                std::cout << "Generating object... " << '\n';
                int height = rand()%range[1] + range[0];
                int pos = (rand()%1)*(g::W_H-height);
                SDL_Rect shape {g::W_W, pos, 50, height};
                obstacles[i] = Obstacle(-count/100, shape);
                break;
            }
        }   
    }
    
    
}

void Obstacle_Generator::updateObstacles(){
    for(int i = 0; i < 5; i++) {
        if(obstacles[i].isInitialized()) {
            obstacles[i].move();
        }
    }
}

void Obstacle_Generator::draw(SDL_Renderer *renderer) {
    for(int i = 0; i < 5; i++) {
        if(obstacles[i].isInitialized()) {
            std::cout << "drawing obstacle" << '\n';
            obstacles[i].draw(renderer);
        }
    }
}
*/