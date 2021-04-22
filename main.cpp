#include "text.h"
#include "button.h"
#include "menu.h"
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


/*

SDL_Window *window { nullptr };
SDL_Renderer *renderer { nullptr };
SDL_Texture *background_texture { nullptr };


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


bool Init()
{
    std::cout << "Call to Init().\n";
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g::W_W, g::W_H, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    bool success = true;

    if( window == nullptr )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
        }

    return (window && renderer && success);
}

void Quit(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *bakcground_texture)
{
    SDL_DestroyTexture(background_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    
    //renderer = nullptr
    //window = nullptr;
    //background_texture = nullptr;
    

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
    SDL_Texture* newTexture= nullptr;

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
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

*/

void checkBestScore(int score) {
    std::fstream savefile(g::saveFile);
    int best_score;
    savefile >> best_score;
    if(score > best_score) {
        savefile.clear();
        savefile << score;
    }
    savefile.close();
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
    SDL_Colour st_mn_clr {200, 200, 200, 150};
    SDL_Colour st_mn_ft_clr {250, 250, 250, 150};
    SDL_Colour st_mn_btt_clr {0, 0, 0, 150};
    Menu st_menu {st_menuRect, background_texture, st_mn_clr, renderer};

    //Initializing start menu button objects
    SDL_Rect st_mn_btt1_target_rect {200, g::W_H - 75, 150, 80};
    SDL_Rect st_mn_btt1_rect {200, g::W_H - 75, 150, 80};
    st_menu.addBtt(NULL, "Start", g::font, 16, &st_mn_ft_clr, &st_mn_btt1_target_rect, &st_mn_btt1_rect, &st_mn_btt_clr, true, 0, renderer);

    SDL_Rect st_mn_btt2_target_rect {400, g::W_H - 75, 150, 80};
    SDL_Rect st_mn_btt2_rect {400, g::W_H - 75, 150, 80};
    st_menu.addBtt(NULL, "Quit", g::font, 16, &st_mn_ft_clr, &st_mn_btt1_target_rect, &st_mn_btt1_rect, &st_mn_btt_clr, true, 1, renderer);

    //Initializing start menu text objects
    int best_score;
    std::cout << "About to load best_score from savefile";
    try {
        std::fstream Savefile(g::saveFile);
        Savefile >> best_score; 
        Savefile.close();
    } catch(int e) {
        std::cout << "Unable to load best score from savefile. Exception N. " << e << '\n';
    }
    SDL_Rect st_mn_txt1_target_rect {210, 160, 80, 60};
    st_menu.addTxt("Your best score: " + std::to_string(best_score), g::font, 16, &st_mn_ft_clr, &st_mn_txt1_target_rect, &st_mn_btt_clr, true, renderer);

    //Initializing outer loop variables
    GAME_STATE menu_state { NOINPUT };
    SDL_Rect st_srcRect {0, 0, g::W_W, g::W_H};
    SDL_Rect st_destRect {0, 0, g::W_W, g::W_H};
    bool done { false };

    while(!done){

        //Initializing menu loop variables
        bool menu_done {false};
        int id {-1};
        while(!menu_done && !done) {
            st_menu.display(renderer, &st_srcRect, &st_destRect);
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
                    case 1:
                        done = true;
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
    
    /*
        //Initializing game objects
        SDL_Point point {static_cast<int> (g::W_W/3), g::W_H/2};
        Character character { point };
        Obstacle_Generator generator { 5, 1, 50, 250 };
        Obstacle_Generator generator { 1, 100, 250, -1};

        int x_run {0};
        int score {0};
        Text scoreCounter {score + "", "Krungthep.ttf", 12, &g::scoreCounterTxtColour, &g::scoreCounterRect, &g::scoreCounterRectColour, true, renderer};
        bool game_done {false};
        while(!done && !game_done){
            generator.generateObstacles();
            
            // displays running background
            if(x_run % 100) {
                score++;
                scoreCounter.assignTxt(score+"", renderer);
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
                    if(event.key.keysym.sym == SDLK_SPACE) {
                        character.update_speed();
                    }
                    break;
                //case SDL_KEYUP:
                    //if(event.key.keysym.sym == SDLK_SPACE){
                    //    character.update_speed(0);
                    //}
            }

            x_run += 1;
            
            //std::cout << "Game Loop Length:\t" << gLoop.elapsed() << '\n';

        }
    */
    /*
        checkBestScore(x_run);

        //menu_done = false;
        bool menu_done = false;
        while(!done && menu_done) {
            SDL_Rect srcRect {x_run/10, 500, g::W_W + x_run/10, g::W_H};
            SDL_Rect tarRect {0, 0, g::W_W, g::W_H};
            SDL_RenderCopy(renderer, background_texture, &srcRect, &tarRect);
            SDL_Rect menu_window {100, 75, g::W_W - 200, g::W_H - 150};
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 150);
            SDL_RenderFillRect(renderer, &menu_window);
            SDL_RenderPresent(renderer);

            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT){
                done = true;
            }
        }
    */
    }

    st_menu.destroyTextures();
    //st_menu.~Menu();
    Quit();

    return 0;
}