#include "../include/Piece.hpp"
#include "../include/Pawn.hpp"
#include "../include/Knight.hpp"
#include "../include/Bishop.hpp"
#include "../include/Rook.hpp"
#include "../include/Queen.hpp"
#include "../include/King.hpp"
#include "../include/States.hpp"
#include "../include/GameState.hpp"
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

/**@brief Método construtor de estados do jogo
 *
 *Contrato na especificação: O método deve criar novas peças,
 *setando-as no tabuleiro, definindo suas posições e suas cores
 *
 */

States::States(void)
{
  int i;

  // Inicialização dos peões
  for (i = 0; i <= 7; i++)
  {
    white_pieces[i] = new Pawn(true, i, 6);
    black_pieces[i] = new Pawn(false, i, 1);
  }

  // Inicialização das peças brancas
  white_pieces[8] = new Rook(true, 0, 7);
  white_pieces[9] = new Knight(true, 1, 7);
  white_pieces[10] = new Bishop(true, 2, 7);
  white_pieces[11] = new Queen(true, 3, 7);
  white_pieces[12] = new King(true, 4, 7);
  white_pieces[13] = new Bishop(true, 5, 7);
  white_pieces[14] = new Knight(true, 6, 7);
  white_pieces[15] = new Rook(true, 7, 7);

  // Inicialização das peças pretas
  black_pieces[8] = new Rook(false, 0, 0);
  black_pieces[9] = new Knight(false, 1, 0);
  black_pieces[10] = new Bishop(false, 2, 0);
  black_pieces[11] = new Queen(false, 3, 0);
  black_pieces[12] = new King(false, 4, 0);
  black_pieces[13] = new Bishop(false, 5, 0);
  black_pieces[14] = new Knight(false, 6, 0);
  black_pieces[15] = new Rook(false, 7, 0);

  pieceTurn = true;
}

/**@brief Método que define se há uma peça no caminho
 */

Obstacles States::IsInTheWay(Piece *piece, int position_X, int position_Y)
{
  int i, j;
  Piece **aux;

  switch (piece->GetName())
  {
  case PieceName::Knight:
    return Obstacles::Empty;
    break;

  default:
    int displacement_x = position_X - piece->GetPositionX();
    int displacement_y = position_Y - piece->GetPositionY();
    int displacement_loop, vertical, horizontal;

    for (i = 0; i <= 15; i++)
    {
      aux = white_pieces;
      for (j = 0; j <= 1; j++)
      {
        displacement_loop = 0;
        vertical = 0;
        horizontal = 0;

        displacement_x != 0 ? displacement_loop = displacement_x : displacement_loop = displacement_y;
        displacement_loop = abs(displacement_loop);

        while (displacement_loop > 1)
        {
          if (abs(displacement_x) != abs(displacement_y))
          {
            displacement_y > 0 ? vertical++ : (displacement_y != 0 ? vertical-- : vertical = 0);
            displacement_x > 0 ? horizontal++ : (displacement_x != 0 ? horizontal-- : horizontal = 0);
          }
          else
          {
            displacement_y > 0 ? vertical++ : (displacement_y != 0 ? vertical-- : vertical = 0);
            displacement_x > 0 ? horizontal++ : (displacement_x != 0 ? horizontal-- : horizontal = 0);
          }

          if ((aux[i]->GetPositionX() == (piece->GetPositionX() + horizontal)) &&
              (aux[i]->GetPositionY() == (piece->GetPositionY() + vertical)))
          {
            if ((piece->GetColor() && aux[i]->GetColor()) || (!piece->GetColor() && !aux[i]->GetColor()))
            {
              return Obstacles::Friend;
            }
            else
            {
              return Obstacles::Enemy;
            }
          }
          displacement_loop--;
        }
        aux = black_pieces;
      }
    }
    return Obstacles::Empty;
    break;
  }
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

Obstacles States::IsInTheSpot(Piece *piece, int position_X, int position_Y)
{
  int i, j;
  Piece **aux;

  for (i = 0; i <= 15; i++)
  {
    aux = white_pieces;
    for (j = 0; j <= 1; j++)
    {
      if ((aux[i]->GetPositionX() == position_X) && (aux[i]->GetPositionY() == position_Y))
      {
        if ((piece->GetColor() && aux[i]->GetColor()) || (!piece->GetColor() && !aux[i]->GetColor()))
        {
          return Obstacles::Friend;
        }
        else
        {
          return Obstacles::Enemy;
        }
      }
      aux = black_pieces;
    }
  }
  return Obstacles::Empty;
}

/**@brief Método que verifica se o rei está em cheque
 *
 *Parâmetros: Um booleano que indica a cor do rei e duas
 *posições que mostram sua posição no tabuleiro
 *
 *Tratamento de Erros: É verificado se o movimento até o
 *rei é possível e se não existem movimentos possíveis para o rei
 *
 *Descrição: Este método verifica se o rei está em cheque, verificando se uma peça pode
 *
 *Assertivas de entrada: um booleano que indica a cor do rei
 *e dois inteiros que indicam sua posição no tabuleiro
 *
 *Requisitos: as posições do rei devem pertencer ao tabuleiros
 *
 *Hipóteses: Se o rei estiver em cheque, então o método deve retornar true
 *
 *Assertivas de saida: É retornado um booleano true or false
 * indicando se o rei está em cheque
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação: Se
 *
 */

bool States::IsCheck(bool kingColor, int position_X, int position_Y)
{
  Piece **aux;
  kingColor ? aux = black_pieces : aux = white_pieces;

  for (int i = 0; i < 16; i++)
  {
    SetPawnDiagonalEnemies(true, aux[i], position_X, position_Y); // Bota os inimigos laterais do peão
    if (aux[i]->IsMovementPossible(position_X, position_Y) &&
        (IsInTheWay(aux[i], position_X, position_Y) == Obstacles::Empty)) // Verifica se os inimigos podem ir até o Rei, e se o caminho para ir para o Rei está livre.
      return true;
    SetPawnDiagonalEnemies(false, aux[i], -1, -1); // Remove os inimigos diagonais do peão
  }

  return false;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

bool States::MovePiece(Piece *piece, int position_X, int position_Y)
{
  SetPawnDiagonalEnemies(true, piece, -1, -1); // Bota os inimigos laterais do peão
  Obstacles isIntheSpot = IsInTheSpot(piece, position_X, position_Y);
  if (piece->IsMovementPossible(position_X, position_Y) &&
      (IsInTheWay(piece, position_X, position_Y) == Obstacles::Empty) &&
      (isIntheSpot != Obstacles::Friend) && (pieceTurn == piece->GetColor()))
  {
    SetPawnDiagonalEnemies(false, piece, -1, -1); // Remove os inimigos diagonais do peão

    if (piece->GetName() == PieceName::King)
      if (IsCheck(piece->GetColor(), position_X, position_Y))
        return false;

    if (piece->GetName() == PieceName::Pawn) // Caso tenha um inimigo na frete do peao, e ele esteja tentando ir pra frente (nao pode comer nem andar)
      if (isIntheSpot == Obstacles::Enemy && (position_X - piece->GetPositionX() == 0))
        return false;

    if (isIntheSpot == Obstacles::Enemy)
      EatPiece(position_X, position_Y);

    piece->SetPosition(position_X, position_Y);
    TransformPawn(piece);
    pieceTurn = !pieceTurn;
    return true;
  }
  SetPawnDiagonalEnemies(false, piece, -1, -1); // Remove os inimigos diagonais do peão
  return false;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

void States::EatPiece(int position_X, int position_Y)
{
  Piece **aux;
  int i, j;
  aux = white_pieces;
  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 16; j++)
    {
      if ((aux[j]->GetPositionX() == position_X) && (aux[j]->GetPositionY() == position_Y))
      {
        aux[j]->SetDead();
      }
    }
    aux = black_pieces;
  }
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

void States::SetPawnDiagonalEnemies(bool check, Piece *piece, int futureEnemyX, int futureEnemyY)
{
  int i, verticalDirection, horizontalDirection;
  bool left = false, right = false;

  if (piece->GetName() == PieceName::Pawn)
  {
    if (check)
    {
      piece->GetColor() ? (verticalDirection = (piece->GetPositionY() - 1)) : (verticalDirection = (piece->GetPositionY() + 1));
      horizontalDirection = (piece->GetPositionX() + 1);
      for (i = 0; i < 2; i++)
      {
        if (IsInTheSpot(piece, horizontalDirection, verticalDirection) == Obstacles::Enemy)
          (i == 0) ? (piece->GetColor() ? right = true : left = true) : (piece->GetColor() ? left = true : right = true);

        horizontalDirection = (piece->GetPositionX() - 1);
      }

      if ((abs(futureEnemyX - piece->GetPositionX()) == 1) && (abs(futureEnemyY - piece->GetPositionY()) == 1) && (futureEnemyX != -1) && (futureEnemyY != -1))
      {
        piece->GetColor() ? ((futureEnemyX - piece->GetPositionX()) == 1 ? right = true : left = true) : ((futureEnemyX - piece->GetPositionX()) == -1 ? right = true : left = true);
      }
    }
    piece->SetDiagonalEnemy(left, right);
  }
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

bool States::IsCheckMate(bool kingColor)
{
  Piece **aux;
  Piece **aux2;
  int i, j, k, x, y;

  if (kingColor)
  {
    aux = white_pieces;
    aux2 = black_pieces;
  }
  else
  {
    aux = black_pieces;
    aux2 = white_pieces;
  }

  if (IsCheck(aux[12]->GetColor(), aux[12]->GetPositionX(), aux[12]->GetPositionY()))
  {
    x = aux[12]->GetPositionX();
    y = aux[12]->GetPositionY();

    // Movimentos possíveis do Rei
    for (i = -1; i < 2; i++)
      for (j = -1; j < 2; j++)
        for (k = 0; k < 16; k++)
          if (!IsCheck(kingColor, x + i, y + j) && IsInTheSpot(aux[12], x + i, y + j) != Obstacles::Friend &&
              (!aux2[k]->IsMovementPossible(x + i, y + j) || IsInTheWay(aux2[k], x + i, y + j) != Obstacles::Empty) &&
              aux[12]->IsMovementPossible(x + i, y + j))
            return false;

    return true;
  }
  return false;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

GameResult States::WhoWon(void)
{
  bool white = false, black = false;
  int i;

  white = IsCheckMate(true) || !white_pieces[12]->GetIsAlive();
  black = IsCheckMate(false) || !black_pieces[12]->GetIsAlive();

  if (white && black)
    return GameResult::Draw;

  if (!white && !black)
  {
    for (i = 0; i < 16; i++)
    {
      if ((white_pieces[i]->GetIsAlive() && (white_pieces[i]->GetName() != PieceName::King)) || (black_pieces[i]->GetIsAlive() && (black_pieces[i]->GetName() != PieceName::King)))
        return GameResult::NoContest;
    }
    return GameResult::Draw;
  }

  if (white)
    return GameResult::BlackWins;

  if (black)
    return GameResult::WhiteWins;

  return GameResult::NoContest;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

bool States::IsPositionValid(Piece *piece, int position_X, int position_Y)
{
  Obstacles obstacle;
  if (piece->GetName() == PieceName ::King)
  {
    if (IsCheck(piece->GetColor(), position_X, position_Y))
      return false;
  }
  SetPawnDiagonalEnemies(true, piece, -1, -1); // Bota os inimigos laterais do peão
  obstacle = IsInTheSpot(piece, position_X, position_Y);
  if (piece->IsMovementPossible(position_X, position_Y) &&
      (IsInTheWay(piece, position_X, position_Y) == Obstacles::Empty) &&
      (obstacle != Obstacles::Friend))
  {
    SetPawnDiagonalEnemies(false, piece, -1, -1); // Remove os inimigos laterais do peão
    if ((piece->GetName() == PieceName ::Pawn) && (obstacle == Obstacles::Enemy))
    {
      if (piece->GetPositionX() - position_X != 0)
        return true;
    }
    else
    {
      return true;
    }
  }

  SetPawnDiagonalEnemies(false, piece, -1, -1); // Remove os inimigos laterais do peão
  return false;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

Piece *States::GetPiece(int position_X, int position_Y)
{
  if ((position_X >= 0) && (position_Y >= 0) && (position_X < 8) && (position_Y < 8))
  {
    for (int i = 0; i < 16; i++)
    {
      if (white_pieces[i]->GetPositionX() == position_X && white_pieces[i]->GetPositionY() == position_Y)
        return white_pieces[i];
      if (black_pieces[i]->GetPositionX() == position_X && black_pieces[i]->GetPositionY() == position_Y)
        return black_pieces[i];
    }
  }
  return emptyPiece;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

bool States::SetPiece(Piece *piece, int position_X, int position_Y)
{
  if ((position_X >= 0) && (position_Y >= 0) && (position_X < 8) && (position_Y < 8))
    if (IsInTheSpot(piece, position_X, position_Y) == Obstacles::Empty)
    {
      piece->WakeFromDead();
      piece->SetPosition(position_X, position_Y);
      return true;
    }

  return false;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

void States::PlayBestMove(bool color, Level difficulty)
{
  Piece **piece;
  PiecesValues *bestMoves;
  int values[16], i, n, levelSelected;
  bool result;

  if (color)
  {
    piece = white_pieces;
    bestMoves = white_values;
  }
  else
  {
    piece = black_pieces;
    bestMoves = black_values;
  }

  UpdateBestMoves();

  for (i = 0; i < 16; i++)
  {
    if ((bestMoves[i].max_Value_X != -1) && (bestMoves[i].max_Value_Y != -1))
    {
      values[i] = bestMoves[i].value;
    }
    else
    {
      values[i] = -2000000000;
    }
  }

  n = sizeof(values) / sizeof(values[0]);
  sort(values, values + n);

  switch (difficulty)
  {
  case Level::Hard:
    levelSelected = 15;
    break;

  case Level::Medium:
    levelSelected = 14;
    break;

  case Level::Easy:
    levelSelected = 13;
    break;
  }

  for (i = 0; i < 16; i++)
  {
    if ((bestMoves[i].value == values[levelSelected]) && (bestMoves[i].max_Value_X != -1) && (bestMoves[i].max_Value_Y != -1))
    {
      result = MovePiece(piece[i], bestMoves[i].max_Value_X, bestMoves[i].max_Value_Y);
      return;
    }
  }
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

void States::UpdateBestMoves(void)
{
  int i, x, y, l, maxValue = -20;
  Obstacles obstacle;
  PiecesValues *aux;
  Piece **aux2;

  // Deixa ambos os vetores de melhores movimentos em branco.
  for (i = 0; i < 16; i++)
  {
    white_values[i].max_Value_X = -1;
    white_values[i].max_Value_Y = -1;
    white_values[i].value = -20;
    black_values[i].max_Value_X = -1;
    black_values[i].max_Value_Y = -1;
    black_values[i].value = -20;
  }

  // Calcula os melhores movimentos para cada peça
  for (i = 0; i < 16; i++)
  {
    aux = white_values;
    aux2 = white_pieces;
    for (l = 0; l < 2; l++)
    {
      for (x = 0; x < 8; x++)
      {
        for (y = 0; y < 8; y++)
        {
          if (IsPositionValid(aux2[i], x, y))
          {
            obstacle = IsInTheSpot(aux2[i], x, y);
            obstacle == Obstacles::Enemy ? maxValue = GetPiece(x, y)->PieceValue - aux2[i]->PieceValue : maxValue = -aux2[i]->PieceValue;
            if (maxValue > aux[i].value)
            {
              aux[i].max_Value_X = x;
              aux[i].max_Value_Y = y;
              aux[i].value = maxValue;
            }
          }
        }
      }
      maxValue = -10;
      aux = black_values;
      aux2 = black_pieces;
    }
  }
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

void States::SetPieceTurn(bool pieceTurn)
{
  this->pieceTurn = pieceTurn;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

bool States::GetPieceTurn(void)
{
  return this->pieceTurn;
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Não há interface implícita
 *
 *Contrato na especificação:
 *
 */

void States::SaveGame(GameMode mode)
{
  FILE *fp = NULL;
  int i, j;
  Piece **aux;
  char print;

  if (mode == GameMode::GAME_MODE_PVP)
    fp = fopen("output_PvP.pgn", "w");
  if (mode == GameMode::GAME_MODE_CPU)
    fp = fopen("output_CPU.pgn", "w");
  if (mode == GameMode::GAME_MODE_EDIT)
    fp = fopen("output_EDIT.pgn", "w");

  if (fp == NULL)
    return;

  fprintf(fp, "%s", "[Piece Turn]:");
  fprintf(fp, "%c%c", pieceTurn ? 'w' : 'b', '\n');
  fprintf(fp, "%s", "[White Piece Board]:");
  aux = white_pieces;
  for (j = 0; j < 2; j++)
  {
    for (i = 0; i < 16; i++)
    {
      if (aux[i]->GetIsAlive())
      {
        switch (aux[i]->GetName())
        {
        case PieceName::Pawn:
          print = 'P';
          break;
        case PieceName::Bishop:
          print = 'B';
          break;
        case PieceName::Rook:
          print = 'R';
          break;
        case PieceName::Knight:
          print = 'N';
          break;
        case PieceName::Queen:
          print = 'Q';
          break;
        case PieceName::King:
          print = 'K';
          break;
        }
        fprintf(fp, "%c,%d,%d,%c", print, aux[i]->GetPositionX(), aux[i]->GetPositionY(), '|');
      }
      else
      {
        fprintf(fp, "%c%c", 'x', '|');
      }
    }
    if (j == 0)
    {
      fprintf(fp, "%c", '\n');
      fprintf(fp, "%s", "[Black Piece Board]:");
      aux = black_pieces;
    }
  }
  fclose(fp);
}

/**@brief
 *
 *Parâmetros: Não há parâmetros
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Não há interface explicita
 *
 *Interface implicita: Manipulações através de ponteiros do objeto piece
 *
 *Contrato na especificação:
 *
 */

void States::LoadGame(GameMode mode)
{
  FILE *fp = NULL;
  int i, j;
  char read[86];
  Piece **aux;

  if (mode == GameMode::GAME_MODE_PVP)
    fp = fopen("output_PvP.pgn", "r");
  if (mode == GameMode::GAME_MODE_CPU)
    fp = fopen("output_CPU.pgn", "r");
  if (mode == GameMode::GAME_MODE_EDIT)
    fp = fopen("output_EDIT.pgn", "r");

  if (fp == NULL)
    return;

  fscanf(fp, "%86[^\n].", read);

  read[13] == 'w' ? pieceTurn = true : pieceTurn = false;

  aux = white_pieces;
  for (j = 0; j < 2; j++)
  {
    fscanf(fp, "%86[^:].", read);
    read[0] = fgetc(fp);

    for (i = 0; i < 16; i++)
    {
      fscanf(fp, "%86[^|].", read);
      if (read[0] != 'x')
      {
        aux[i]->WakeFromDead();
        aux[i]->SetPosition(atoi(&read[2]), atoi(&read[4]));
      }
      else
        aux[i]->SetDead();

      read[0] = fgetc(fp);
    }
    aux = black_pieces;
  }

  fclose(fp);
}

/**@brief Método que retorna o melhor movimento para as peças
 *
 *Parâmetros: Ponteiro para um objeto da classe Piece
 *
 *Tratamento de Erros: Não há tratamento de erros
 *
 *Descrição:
 *
 *Assertivas de entrada: Não há assertivas de entrada
 *
 *Requisitos:
 *
 *Hipóteses:
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Piece * piece
 *
 *Interface implicita: Manipulações através de ponteiros do objeto piece
 *
 *Contrato na especificação:
 *
 */

PiecesValues States::GetPieceBestMove(Piece *piece)
{
  int i;
  Piece **aux;
  PiecesValues *pieceValue;

  UpdateBestMoves();

  if (piece->GetColor())
  {
    aux = white_pieces;
    pieceValue = white_values;
  }
  else
  {
    aux = black_pieces;
    pieceValue = black_values;
  }

  for (i = 0; i < 16; i++)
  {
    if ((piece->GetPositionX() == aux[i]->GetPositionX()) && (piece->GetPositionY() == aux[i]->GetPositionY()))
      return pieceValue[i];
  }
}

/**@brief Método que transforma o peão em rainha
 *
 *Parâmetros: Ponteiro para um objeto da classe Piece
 *
 *Tratamento de Erros: É verificado se o nome da peça é o de um peão,
 *em seguida verifica-se se este peão atravessou todo o tabuleiro
 *
 *Descrição: Este método transforma um peão que atravessou todo o tabuleiro em uma rainha
 *
 *Assertivas de entrada: Ponteiro para um objeto da classe Piece
 *
 *Requisitos: O peão tenha atravessado todo o tabuleiro
 *e que haja memória para alocar uma nova rainha
 *
 *Hipóteses: A peça seja um peão e que esteja na posição final do tabuleiro
 *
 *Assertivas de saida: Não há assertivas de saída
 *
 *Interface explicita: Piece * piece
 *
 *Interface implicita: Manipulações através de ponteiros do objeto piece
 *
 *Contrato na especificação: Este método transforma um peão em rainha,
 * verificando se este peão foi capaz de atravessar todo o tabuleiro
 *e chegar na última posição vertical do tabuleiro
 *
 */

void States::TransformPawn(Piece *piece)
{
  int x, y, i;
  bool color;
  Piece **aux;

  if (piece->GetName() == PieceName::Pawn)
  {
    color = piece->GetColor();
    y = piece->GetPositionY();
    if ((color && (y == 0)) || (!color && (y == 7)))
    {
      x = piece->GetPositionX();
      color ? aux = white_pieces : aux = black_pieces;
      for (i = 0; i < 16; i++)
      {
        if ((aux[i]->GetPositionX() == x) && (aux[i]->GetPositionY() == y))
        {
          aux[i] = new Queen(color, x, y);
          return;
        }
      }
    }
  }
}

void States::KillAllPieces(void)
{
  int i;
  for (i = 0; i < 16; i++)
  {
    white_pieces[i]->SetDead();
    black_pieces[i]->SetDead();
  }
}
