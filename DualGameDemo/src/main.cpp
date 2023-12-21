#include "../include/Piece.hpp"
#include "../include/Pawn.hpp"
#include "../include/Knight.hpp"
#include "../include/Bishop.hpp"
#include "../include/Rook.hpp"
#include "../include/Queen.hpp"
#include "../include/King.hpp"
#include "../include/States.hpp"

#include "../include/Graphics.hpp"
#include "common.hpp"

#ifndef SDL_INCLUDES                                                       
#define SDL_INCLUDES                                                       
#include <SDL2/SDL_image.h>                                                
#include <SDL2/SDL.h>                                                      
#endif    

#include <cstdio>
#include <string>


using namespace std;

int main( int argc, char* args[] ){

    Graphics *graphics = new Graphics();

    
    GameState *gm = new GameState(); 

	if( !graphics->init() ){
		printf( "Falha ao inicializar!\n" );
	}
	else{
		//Carrega mídia
		if( !graphics->loadMedia() ){
			printf( "Nao foi possivel carregar a midia!\n" );
		}
		else{
            gm->startStateMachine(GameMode::GAME_MODE_MAIN_MENU);            
    	}

    	//libera os recursos e finaliza o sdl
        graphics->close();

	    return 0;
    }
}

