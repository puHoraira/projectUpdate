#pragma once

#include <iostream>
#include <string>
#include "chess.h"
using namespace std;

/**@brief Classe do Torre que herda de Piece
 *
 *Parâmetros: Construtor e destrutor de Torre, IsMovementPossible e
 *funções herdadas de Piece como GetColor, GetName, GetPositionX, GetPositionY
 *
 *Descrição: Esta classe define o Torre em um jogo de Xadrez padrão, aqui são
 *definidos alguns dos métodos básicos para manipular um objeto dessa classe,
 *como o método de construir e destruir um objeto desta classe, um método para
 *retornar sua cor, seu nome e sua posição no tabuleiro e outro para validar se a
 *jogada é possível
 *
 */

int chessGameStarts();
