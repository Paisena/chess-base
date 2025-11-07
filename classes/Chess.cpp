#include "Chess.h"
#include <limits>
#include <cmath>

Chess::Chess()
{
    _grid = new Grid(8, 8);
}

Chess::~Chess()
{
    delete _grid;
}

char Chess::pieceNotation(int x, int y) const
{
    const char *wpieces = { "0PNBRQK" };
    const char *bpieces = { "0pnbrqk" };
    Bit *bit = _grid->getSquare(x, y)->bit();
    char notation = '0';
    if (bit) {
        notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag()-128];
    }
    return notation;
}

Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;

    _grid->initializeChessSquares(pieceSize, "boardsquare.png");
    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    startGame();
}

void Chess::FENtoBoard(const std::string& fen) {
    // convert a FEN string to a board
    // FEN is a space delimited string with 6 fields
    // 1: piece placement (from white's perspective)
    // NOT PART OF THIS ASSIGNMENT BUT OTHER THINGS THAT CAN BE IN A FEN STRING
    // ARE BELOW
    // 2: active color (W or B)
    // 3: castling availability (KQkq or -)
    // 4: en passant target square (in algebraic notation, or -)
    // 5: halfmove clock (number of halfmoves since the last capture or pawn advance)
    
    int x = 0;
    int y = 7;
    bool boardReading = true;

    std::string digits = "12345678";

    for (int i = 0; i < fen.length(); i++)
    {
        if(fen[i] == ' ')
        {
            boardReading = false;
        }
        if(fen[i] == '/')
        {
            y--;
            x = -1;
        }



        if(!boardReading)
        {
            continue;
        }
        if(fen[i] == '1' || fen[i] == '2' ||fen[i] == '3' ||fen[i] == '4' || fen[i] == '5' || fen[i] == '6' || fen[i] == '7' || fen[i] == '8')
        {
            int num = (int)fen[i];
            x += num;
        }

        if(fen[i] == 'r')
        {
            Bit* piece = PieceForPlayer(1, Rook);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'R')
        {
            Bit* piece = PieceForPlayer(0, Rook);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'n')
        {
            Bit* piece = PieceForPlayer(1, Knight);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'N')
        {
            Bit* piece = PieceForPlayer(0, Knight);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }

        if(fen[i] == 'b')
        {
            Bit* piece = PieceForPlayer(1, Bishop);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'B')
        {
            Bit* piece = PieceForPlayer(0, Bishop);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'q')
        {
            Bit* piece = PieceForPlayer(1, Queen);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'Q')
        {
            Bit* piece = PieceForPlayer(0, Queen);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'k')
        {
            Bit* piece = PieceForPlayer(1, King);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'K')
        {
            Bit* piece = PieceForPlayer(0, King);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'p')
        {
            Bit* piece = PieceForPlayer(1, Pawn);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        if(fen[i] == 'P')
        {
            Bit* piece = PieceForPlayer(0, Pawn);
            ChessSquare* square = _grid->getSquare(x,y);
            piece->setPosition(square->getPosition());
            square->setBit(piece);
        }
        x++;
    }
}

bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    return false;
}

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // need to implement friendly/unfriendly in bit so for now this hack
    int currentPlayer = getCurrentPlayer()->playerNumber() * 128;
    int pieceColor = bit.gameTag() & 128;
    if (pieceColor == currentPlayer) return true;
    return false;
}

bool Chess::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    return true;
}

void Chess::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Chess::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return nullptr;
    }

    auto square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* Chess::checkForWinner()
{
    return nullptr;
}

bool Chess::checkForDraw()
{
    return false;
}

std::string Chess::initialStateString()
{
    return stateString();
}

std::string Chess::stateString()
{
    std::string s;
    s.reserve(64);
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
            s += pieceNotation( x, y );
        }
    );
    return s;}

void Chess::setStateString(const std::string &s)
{
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 8 + x;
        char playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1, Pawn));
        } else {
            square->setBit(nullptr);
        }
    });
}
