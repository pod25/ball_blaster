/* file: ball_blaster.cpp */
#include "common.h"

SDL_Surface *load_image( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );

//main function
int main(int argc, char* args[]) {

	//The attributes of the screen
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int SCREEN_BPP = 32;

	//The surfaces that will be used
	SDL_Surface *message = NULL;
	SDL_Surface *background = NULL;
	SDL_Surface *screen = NULL;

	//The event structure that will be used
	SDL_Event event;


	// TODO: Temporary SDL implementation
	// TODO: Remove

	//Initialize all SDL subsystems
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        return 1;    
    }

	//Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//If there was an error in setting up the screen
    if ( screen == NULL ) {
        return 1;    
    }

	//Set the window caption
    SDL_WM_SetCaption( "Hello World", NULL );

	//Load the images
    message = load_image( "hello.bmp" );
    background = load_image( "background.bmp" );

	//Apply the background to the screen
    apply_surface( 0, 0, background, screen );
	apply_surface( 320, 0, background, screen );
    apply_surface( 0, 240, background, screen );
    apply_surface( 320, 240, background, screen );

	//Apply the message to the screen
    apply_surface( 180, 140, message, screen );

	//Update the screen
    if(  SDL_Flip( screen ) == -1 ) {
        return 1;    
    }

	//Wait 2 seconds
    //SDL_Delay( 2000 );

	//While the user hasn't quit
	bool quit = false;
    while( quit == false ) {
		//While there's an event to handle
        while( SDL_PollEvent( &event ) )
        {
			//If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }    
        }
    }

	//Free the surfaces
    SDL_FreeSurface( message );
    SDL_FreeSurface( background );
    
    //Quit SDL
    SDL_Quit();
    
    //Return
    return 0;
}

SDL_Surface *load_image( std::string filename ) 
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;
    
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );
    
    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
    }
    
    //Return the optimized image
    return optimizedImage;
}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

	//Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

