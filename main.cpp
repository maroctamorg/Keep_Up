#include "text.h"
#include "button.h"
#include "menu.h"
#include "Engine.h"
#include "global.h"

//Starts up SDL and creates window
bool Init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void Quit();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Current displayed texture
SDL_Texture* background_texture = NULL;

bool Init()
{
	//Initialization flag
	bool success = true;
    TTF_Init();

	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g::W_W, g::W_H, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	background_texture = loadTexture("resources/background.png");
	if( background_texture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void Quit()
{
	//Free loaded image
	if(background_texture != NULL && background_texture != nullptr) {
        SDL_DestroyTexture( background_texture );
        background_texture = NULL;
    }

	//Destroy window	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
    IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
 
	std::chrono::time_point<clock_t> m_beg;
 
public:
	Timer() : m_beg(clock_t::now())
	{
	}
 
	void reset()
	{
		m_beg = clock_t::now();
	}
 
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

int checkBestScore(int score) {
    std::fstream savefile(g::saveFile);
    int previous_score;
    int best_score;
    savefile >> previous_score;
    if(score > previous_score) {
        savefile.seekg(0);
        savefile << score;
        best_score = score;
    }
    best_score = previous_score;
    savefile.close();
    return best_score;
}

int main(int arc, char* argv[]) {

    //Initializing window and renderer
    if (!Init()) {
        std::cout << "Failure to initalise SDL window or renderer.\n";
        return 1;
    }
    
    std::cout << "Succesfully initialised SDL window and renderer.\n";

    if( !loadMedia() )
	{
		std::cout << "Unable to load background texture.\n";
        return 1;
	}

    std::cout << "Succesfully loaded background texture.\n";

    SDL_Point size;
    SDL_QueryTexture(background_texture, NULL, NULL, &size.x, &size.y);

    SDL_Event event;

    //Initializing start menu
    SDL_Rect st_menuRect {100, 75, g::W_W - 200, g::W_H - 150};
    SDL_Colour st_mn_clr {255, 255, 255, 150};
    SDL_Colour st_mn_ft_clr {250, 250, 250, 150};
    SDL_Colour st_mn_btt_clr {0, 0, 0, 150};
    Menu st_menu {st_menuRect, background_texture, st_mn_clr, renderer};


    //Initializing start menu button objects
    SDL_Rect st_mn_btt1_rect {200, st_menuRect.y + st_menuRect.h - 100, 150, 50};
    SDL_Rect st_mn_btt1_target_rect {225, st_mn_btt1_rect.y + 10, 100, 30};
    st_menu.addBtt(NULL, "Start", g::font, 16, &st_mn_ft_clr, &st_mn_btt1_target_rect, &st_mn_btt1_rect, &st_mn_btt_clr, true, 0, renderer);

    SDL_Rect st_mn_btt2_rect {400, st_menuRect.y + st_menuRect.h - 100, 150, 50};
    SDL_Rect st_mn_btt2_target_rect {425, st_mn_btt2_rect.y + 10, 100, 30};
    st_menu.addBtt(NULL, "Quit", g::font, 16, &st_mn_ft_clr, &st_mn_btt2_target_rect, &st_mn_btt2_rect, &st_mn_btt_clr, true, 1, renderer);


    //Initializing start menu text objects
    int best_score = checkBestScore(0);
    std::cout << "About to load best_score from savefile";
    try {
        std::fstream Savefile(g::saveFile);
        Savefile >> best_score; 
        Savefile.close();
    } catch(int e) {
        std::cout << "Unable to load best score from savefile. Exception N. " << e << '\n';
    }
    SDL_Rect st_mn_txt1_target_rect {200, 150, 400, 60};
    st_menu.addTxt("Your best score: " + std::to_string(best_score), g::font, 22, &st_mn_ft_clr, &st_mn_txt1_target_rect, &st_mn_btt_clr, true, renderer);

    //Initializing pause menu static objects
    SDL_Rect pmenuRect {100, 75, g::W_W - 200, g::W_H - 150};
    SDL_Colour pmn_clr {255, 255, 255, 150};
    SDL_Colour pmn_ft_clr {250, 250, 250, 150};
    SDL_Colour pmn_btt_clr {0, 0, 0, 150};

    //Initializing pause menu button objects
    SDL_Rect pmn_btt1_rect {200, pmenuRect.y + pmenuRect.h - 100, 150, 50};
    SDL_Rect pmn_btt1_target_rect {225, pmn_btt1_rect.y + 10, 150, 30};

    SDL_Rect pmn_btt2_rect {400, pmenuRect.y + pmenuRect.h - 100, 150, 50};
    SDL_Rect pmn_btt2_target_rect {425, pmn_btt2_rect.y + 10, 150, 30};


    //Initializing end menu static objects
    SDL_Rect end_menuRect {100, 75, g::W_W - 200, g::W_H - 150};
    SDL_Colour end_mn_clr {255, 255, 255, 150};
    SDL_Colour end_mn_ft_clr {250, 250, 250, 150};
    SDL_Colour end_mn_btt_clr {0, 0, 0, 150};

    //Initializing end menu button objects
    SDL_Rect end_mn_btt1_rect {200, end_menuRect.y + end_menuRect.h - 100, 150, 50};
    SDL_Rect end_mn_btt1_target_rect {225, end_mn_btt1_rect.y + 10, 150, 30};

    SDL_Rect end_mn_btt2_rect {400, end_menuRect.y + end_menuRect.h - 100, 150, 50};
    SDL_Rect end_mn_btt2_target_rect {425, end_mn_btt2_rect.y + 10, 150, 30};

    //Initializing outer loop variables
    GAME_STATE menu_state { NOINPUT };
    SDL_Rect srcRect {0, 0, g::W_W, g::W_H};
    SDL_Rect destRect {0, 0, g::W_W, g::W_H};
    bool done { false };

    //Initializing menu loop variables
    bool menu_done {false};
    int id {-1};
    //Start menu
    while(!menu_done && !done) {
        st_menu.display(renderer, &srcRect, &destRect);
        SDL_RenderPresent(renderer);
        menu_state = st_menu.get_uinput();

        //std::cout << "Menu state: " << menu_state << '\n';

        if (menu_state == UPDATE)
        {
            id = st_menu.update();
            //std::cout << "menu_done : " << menu_done << '\n';
            //check all button presses: call functions
            switch(id) {
                case 0:
                    menu_done = true;
                    break;
                case 1:
                    done = true;
                    break;
            }
        }
        
        if (menu_state == TERMINATE)
        {
            done = true;
        }
    
        if (event.type == SDL_QUIT){
            done = true;
        }
    }

    while(!done) {
    
    
        //Initializing game objects
        SDL_Point point {static_cast<int> (g::W_W/3), g::W_H/2};
        Character character { point };
        //Obstacle_Generator generator { 5, 1, 50, 250 };
        Obstacle_Generator generator { 2, 100, 250, -1}; //1, 100, 250, -1

        int x_run {0};
        int score {0};
        Text scoreCounter {std::to_string(score), g::font, 16, &g::scoreCounterTxtColour, &g::scoreCounterRect, &g::scoreCounterRectColour, true, renderer};
        bool game_done {false};
        bool pause {false};
        while(!done && !game_done) {
            generator.generateObstacles();
            
            // displays running background
            if(x_run % 1000) {
                score++;
                scoreCounter.assignTxt(std::to_string(score), renderer);
            }
            if(g::W_W + x_run/10 > size.x) {
                x_run = 0;
            }

            SDL_Rect srcRect {x_run/10, 500, g::W_W + x_run/10, g::W_H};
            SDL_Rect tarRect {0, 0, g::W_W, g::W_H};  
            SDL_RenderCopy(renderer, background_texture, &srcRect, &tarRect);
            //

            character.draw(renderer);
            generator.draw(renderer);
            scoreCounter.displayText(renderer);

            character.move();
            if(!character.checkBounds()) {
                break;
            }
            generator.updateObstacles();
            if(generator.checkCollisions(character.getRect())) {
                break;
            }

            SDL_RenderPresent(renderer);

            SDL_PollEvent(&event);
            switch(event.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        case SDLK_SPACE:
                            character.update_speed();
                            break;
                        case SDLK_RETURN:
                        case SDLK_ESCAPE:
                            pause = true;
                            break;
                    }
                    break;
            }
            
            x_run++;

            if(pause) {
                //Initializing pause menu
                Menu pmenu {pmenuRect, background_texture, pmn_clr, renderer};

                //Adding dynamic objects
                pmenu.addBtt(NULL, "Resume", g::font, 16, &pmn_ft_clr, &pmn_btt1_target_rect, &pmn_btt1_rect, &pmn_btt_clr, true, 0, renderer);
                pmenu.addBtt(NULL, "Quit", g::font, 16, &pmn_ft_clr, &pmn_btt2_target_rect, &pmn_btt2_rect, &pmn_btt_clr, true, 1, renderer);

                //Initializing pause menu dynamic objects
                best_score = checkBestScore(score);
                std::cout << "About to load best_score from savefile";
                try {
                    std::fstream Savefile(g::saveFile);
                    Savefile >> best_score; 
                    Savefile.close();
                } catch(int e) {
                    std::cout << "Unable to load best score from savefile. Exception N. " << e << '\n';
                }
                SDL_Rect pmn_txt1_target_rect {210, 150, 250, 50};
                pmenu.addTxt("Current score: " + std::to_string(score), g::font, 16, &pmn_ft_clr, &pmn_txt1_target_rect, &pmn_btt_clr, true, renderer);

                GAME_STATE pmenu_state {NOINPUT};

                while(pause) {
                    pmenu.display(renderer, &srcRect, &destRect);
                    SDL_RenderPresent(renderer);
                    pmenu_state = pmenu.get_uinput();

                    //std::cout << "Menu state: " << menu_state << '\n';

                    if (pmenu_state == UPDATE)
                    {
                        id = pmenu.update();
                        //std::cout << "menu_done : " << menu_done << '\n';
                        //check all button presses: call functions
                        switch(id) {
                            case 0:
                                pause = false;
                                break;
                            case 1:
                                pause = false;
                                done = true;
                                break;
                        }
                    }
                    
                    if (pmenu_state == TERMINATE)
                    {
                        done = true;
                    }
                
                    SDL_PollEvent(&event);

                    if (event.type == SDL_QUIT){
                        pause = false;
                        done = true;
                    }
                }
                pmenu.destroyTextures();
            }

        }
    
        scoreCounter.destroyTxtTexture();

        //Initializing end menu
        Menu end_menu {end_menuRect, background_texture, end_mn_clr, renderer};

        //Adding dynamic objects
        end_menu.addBtt(NULL, "Restart", g::font, 16, &end_mn_ft_clr, &end_mn_btt1_target_rect, &end_mn_btt1_rect, &end_mn_btt_clr, true, 0, renderer);
        end_menu.addBtt(NULL, "Quit", g::font, 16, &end_mn_ft_clr, &end_mn_btt2_target_rect, &end_mn_btt2_rect, &end_mn_btt_clr, true, 1, renderer);

        //Initializing end menu dynamic objects
        /*
        std::cout << "About to load best_score from savefile";
        try {
            std::fstream Savefile(g::saveFile);
            Savefile >> best_score; 
            Savefile.close();
        } catch(int e) {
            std::cout << "Unable to load best score from savefile. Exception N. " << e << '\n';
        }
        */

        best_score = checkBestScore(score);

        SDL_Rect end_mn_txt1_target_rect {end_menuRect.x + 100, end_menuRect.y + 50, 300, 50};
        SDL_Rect end_mn_txt2_target_rect {end_menuRect.x + 100, end_menuRect.y + 150, 300, 50};
        end_menu.addTxt("Your score: " + std::to_string(score), g::font, 16, &end_mn_ft_clr, &end_mn_txt1_target_rect, &end_mn_btt_clr, true, renderer);
        end_menu.addTxt("Your best score: " + std::to_string(best_score), g::font, 16, &end_mn_ft_clr, &end_mn_txt2_target_rect, &end_mn_btt_clr, true, renderer);

        //Initializing loop variables
        GAME_STATE end_menu_state { NOINPUT };

        //menu_done = false;
        bool menu2_done = false;
        while(!done && !menu2_done) {
            end_menu.display(renderer, &srcRect, &destRect);
            SDL_RenderPresent(renderer);
            end_menu_state = end_menu.get_uinput();

            //std::cout << "Menu state: " << menu_state << '\n';

            if (end_menu_state == UPDATE)
            {
                id = end_menu.update();
                //std::cout << "menu_done : " << menu_done << '\n';
                //check all button presses: call functions
                switch(id) {
                    case 0:
                        game_done = false;
                        menu2_done = true;
                        break;
                    case 1:
                        done = true;
                        break;
                }
            }
            
            if (end_menu_state == TERMINATE)
            {
                done = true;
            }
        
            if (event.type == SDL_QUIT){
                done = true;
            }
        }
        end_menu.destroyTextures();
    
    }

    st_menu.destroyTextures();
    Quit();

    return 0;
}