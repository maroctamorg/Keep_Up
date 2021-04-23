# Keep Up

## TO DO LIST

// + figure out obstacles

- - velocity of obstacles should increase together with velocity of background?

* - makes sense if it doesn't increase at same rate since background is farther
    away // + implement collision system: // + implement death : display white
    screen // + collision with obstacles // + boundaries: zero velocity when you
    reach the top; death when you reach bottom
* implement score counter
  - why isn't the score counter showing up in the game??
* implement menu (transparent white layer on top of background) for starting
  game and restarting game once dead (displays previous and highest score: write
  to file) // + implement button_functionality as part of game_loop: query menu
  object for selected buttons from vector, based on // button id implement
  functionality (as if they were keys being pressed on the keyboard) // - try to
  reimplement button and text with copy assignments rather than pointers: figure
  out how to delete duplicate objects afterwards // - check if some protected
  pointer is begin returned : perhaps change that? // - what is happening when I
  pass Button(...) or Text(...) objects to the std::vector? What is begin
  stored? // could try to default construct and then assign // + redefine second
  menu as a second menu object implementation // + setup pause menu // + fix the
  2 menus: button dimensions, colour and text
* have scoreCounter rectangle query text texture size for redimensioning
* add music and sound effects (collisions, buttons in menu)
* polish background animation
* polish physics of motion
  - for both the above points - time code instead of relying on gameloop
    iterations
  - replace all position instances in Character with the SDL_Rect variables ->x
    and ->y
  - change velocity to a single int (not a point)
    - maybe NOT, since we might want to accelerate in x sometimes
    - on the other hand, this should be done with the background
* polish app design
* optimise:
  - substitute long integer divisions by small iterations?
  - pull everything that can be pulled out of the game loop from the game loop
  - unify repetitive/duplicate resources
  - compartmentalise game loop: only do what is necessary on any given loop
* implement coin system? boosts (temporary permeability, increase velocity)
* port to Switch and iPad
