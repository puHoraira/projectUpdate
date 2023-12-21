#include "../include/GUIBoard.hpp"
#include "../include/GameState.hpp"

void GUIBoard::updateFocus(int x, int y){

    //Fora das extremidades do tabuleiro
    if((x < 20) || (y < 20) || (x>660) || (y>660) ){
        focus.x = -1;
        focus.y = -1;
        return;
    } else{

        focus.x = (int) (x-20)/80;
        focus.y = (int) (y-20)/80;
    }
}

int GUIBoard::indexToPixel(int index){

    if((index < 0) || (index > 7)){
        printf("Indice invalido");
        return -1;
    }

    return (index * 80 + 20);

}

void GUIBoard::renderPieceOnBoard(PieceName piece, int cor, int ix, int iy){

    if(ix < 0 || iy < 0){
        return;
    }

    if(cor == 0){
        whitePieces[(int) piece].render(indexToPixel(ix), indexToPixel(iy));
    } else{
        blackPieces[(int) piece].render(indexToPixel(ix), indexToPixel(iy));
    }
}


void GUIBoard::renderAllPieces(States* states){
    int i = 0;
    //Renderiza brancas
    for(i = 0 ; i < 16 ; i++){
        renderPieceOnBoard(states->white_pieces[i]->GetName(), 0, states->white_pieces[i]->GetPositionX(), states->white_pieces[i]->GetPositionY());
    }

    //renderiza as pretas
    for(i = 0 ; i < 16 ; i++){
        renderPieceOnBoard(states->black_pieces[i]->GetName(), 1, states->black_pieces[i]->GetPositionX(), states->black_pieces[i]->GetPositionY());
    }
}

bool GUIBoard::checkMovement(States* states){
    bool move = false;
    if(focusedPiece != NULL){
        if(focusedPiece->GetName() != PieceName::Empty){
            move = states->MovePiece(focusedPiece,focus.x,focus.y);
        }
    }
    return move;
}

void GUIBoard::renderPossibleMoves(States *states){
    int i = 0, j = 0;
    if(focusedPiece != NULL){
        if(focusedPiece->GetName() != PieceName::Empty){
            for(i = 0 ; i < 8 ; i++){
                for(j = 0 ; j < 8 ; j++){
                    if(states->IsPositionValid(focusedPiece,i,j))
                    {
                      casaVerde.render(indexToPixel(i),indexToPixel(j)+2);
                    }
                }
            }
        }

    }
}

void GUIBoard::renderBestMove(States *states){

    if(focusedPiece != NULL){
        if(focusedPiece->GetName() != PieceName::Empty){

            PiecesValues bestPosition = states->GetPieceBestMove(focusedPiece);
            if(bestPosition.max_Value_X < 0 || bestPosition.max_Value_Y < 0){
                return;
            }
            bestMove.render(indexToPixel(bestPosition.max_Value_X), indexToPixel(bestPosition.max_Value_Y)+2);
        }
    }
}

//escolhe qual lado começar
bool GUIBoard::choosePieceTurn(GameState *gm, States *states){
    bool selected = false;
    bool white = true;
    int x=-1,y=-1;
    SDL_Event e;

    while(selected == false){

        while(SDL_PollEvent(&e) != 0){
        
            //Usuario pede pra sair
            if( e.type == SDL_QUIT ) {
                gm->setGameState(GameMode::GAME_MODE_QUIT);
            } else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION){
                switch(e.type){
                    case SDL_MOUSEBUTTONDOWN:
                        selected = true;
                        if(y > 340){
                            white = true;
                        } else{
                            white = false;
                        } 
                        break;
                    case SDL_MOUSEMOTION: 
                        SDL_GetMouseState(&x,&y);
                        break;
                }
            } 

            


           SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
           SDL_RenderClear( gRenderer );

           //renderiza tabuleiro
           gBoard.render(0,0);

           renderAllPieces(states);

           //renderiza selecao
           if(y < 340){
               selection.render(20,22);
           } else{
               selection.render(20,342);
           }

            SDL_RenderPresent( gRenderer );
        } 
    }
    return white;
}

void GUIBoard::editBoard(GameState *gm, States* states){
   
    bool done = false;
    Piece *currentPiece = NULL;
    states->KillAllPieces();
    int indexPiece = 1;
    bool black = false;
    int x = -1, y=-1;
    SDL_Event e;

    while(done == false){ 
        while(SDL_PollEvent(&e) != 0){
        
            //Usuario pede pra sair
            if( e.type == SDL_QUIT ) {
                gm->setGameState(GameMode::GAME_MODE_QUIT);
            } else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION){
                switch(e.type){
                    case SDL_MOUSEBUTTONDOWN:
                        SDL_GetMouseState(&x,&y);

                        updateFocus((int)x,(int)y);
                        
                        if(black == false){
                            states->SetPiece(states->white_pieces[indexPiece],focus.x,focus.y);
                        } else{
                            states->SetPiece(states->black_pieces[indexPiece],focus.x,focus.y); 
                        }


                        //done = true;
                        break;
                    case SDL_MOUSEMOTION: 
                        SDL_GetMouseState(&x,&y);
                        break;
                }
            } else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        black = false;
                        break;
                    case SDLK_DOWN:
                        black = true;
                        break;
                    case SDLK_LEFT:
                        if(indexPiece >0){
                            indexPiece--;
                        }
                        else{
                            indexPiece = 15;
                        }
                        break;
                    case SDLK_RIGHT:
                        if(indexPiece <15){
                            indexPiece++;
                        }else{
                            indexPiece = 0;
                        }
                        break;
                    case SDLK_d:
                        done = true;
                        break;

                }
            }

           

           SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
           SDL_RenderClear( gRenderer );




           //renderiza tabuleiro
           gBoard.render(0,0);

           if(black == false){
               whitePieces[(int)states->white_pieces[indexPiece]->GetName()].render(x,y);
           } else{
               blackPieces[(int)states->black_pieces[indexPiece]->GetName()].render(x,y);
           }

           renderAllPieces(states);

            SDL_RenderPresent( gRenderer );
        } 
    }

    return;
}
