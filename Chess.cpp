/* 
Dear visitor...
    This is my first big project written in any language, i chose to write this Chess Game in C++ for several reasons,
        The biggest reason is that C++ was the first language which i started to learn.
            Also the syntax for it (mostly for simple tasks) was easy for me to understand and remember.
                I had to delete majority of the comments in this code because while writing this code i've used the on every single line,
                    Well not actually in every single line but definitely on every single function, i'll add the comments later for better understanding and future improvements.
                        
                        I know this is not the best code ever created, but i gotta say that this "Game" or small project wasn't created to be used by someone else, 
                            well as a good chess player i know that this "game" is far away from being called as GAME, even for me this is just bunch of logics and lines created for my own improvement in programming,

                                I gotta say that even tho i used AI in order to recheck function's errors and general game bugs this project really helped me to advance, 
                                    in future i'll try to not use AI and be fully depended on my own skills because i think that that's the best way to improve ourselfs... without depending on anyone or anything.
                                
*/

#include <iostream>
#include <string>
#include <vector>
#include <memory> 
#include <map>  
#include <algorithm>



#include "_ctype.h" 
#include "math.h"   
#include <cmath>  

using namespace std;

#define BLACK_ROOK "\u265C"
#define BLACK_KNIGHT "\u265E"
#define BLACK_BISHOP "\u265D"
#define BLACK_QUEEN "\u265B"
#define BLACK_KING "\u265A"
#define BLACK_PAWN "\u265F"
#define WHITE_ROOK "\u2656"
#define WHITE_KNIGHT "\u2658"
#define WHITE_BISHOP "\u2657"
#define WHITE_QUEEN "\u2655"
#define WHITE_KING "\u2654"
#define WHITE_PAWN "\u2659"
#define EMPTY_SQUARE "."

enum PIECE_COLOR { WHITE, BLACK, NONE };

class Board; 


class Piece {
public:
    PIECE_COLOR color;
    string symbol;
    bool hasMoved = false;

    Piece(PIECE_COLOR color, string symbol) : color(color), symbol(symbol) {}
    virtual ~Piece() {}
    virtual bool isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) = 0;
    virtual vector<pair<int, int>> getPossibleMoves(int row, int col, Board& board) = 0;
};


class Board {
public:
    vector<vector<shared_ptr<Piece>>> squares;
    Board();
    void print() const;
    bool movePiece(int startRow, int startCol, int endRow, int endCol, PIECE_COLOR currentPlayer);
    bool isInCheck(PIECE_COLOR color);
    bool isCheckmate(PIECE_COLOR color);
    bool isStalemate(PIECE_COLOR color);
    bool isValidPosition(int row, int col) const;
    void promotePawn(int row, int col);
    map<string, int> moveHistory; 
    string boardStateKey() const; 


    bool isSquareUnderAttack(int row, int col, PIECE_COLOR attackerColor);

private:
  
    bool isValidMoveIgnoringCheck(int startRow, int startCol, int endRow, int endCol, PIECE_COLOR currentPlayer);
};



class Pawn : public Piece {
public:
    Pawn(PIECE_COLOR color) : Piece(color, (color == WHITE) ? WHITE_PAWN : BLACK_PAWN) {}
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) override;
    vector<pair<int, int>> getPossibleMoves(int row, int col, Board& board) override;
};

class Knight : public Piece {
public:
    Knight(PIECE_COLOR color) : Piece(color, (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT) {}
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) override;
    vector<pair<int, int>> getPossibleMoves(int row, int col, Board& board) override;
};

class Bishop : public Piece {
public:
    Bishop(PIECE_COLOR color) : Piece(color, (color == WHITE) ? WHITE_BISHOP : BLACK_BISHOP) {}
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) override;
    vector<pair<int, int>> getPossibleMoves(int row, int col, Board& board) override;
};

class Rook : public Piece {
public:
    Rook(PIECE_COLOR color) : Piece(color, (color == WHITE) ? WHITE_ROOK : BLACK_ROOK) {}
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) override;
    vector<pair<int, int>> getPossibleMoves(int row, int col, Board& board) override;
};

class Queen : public Piece {
public:
    Queen(PIECE_COLOR color) : Piece(color, (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN) {}
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) override;
    vector<pair<int, int>> getPossibleMoves(int row, int col, Board& board) override;
};

class King : public Piece {
public:
    King(PIECE_COLOR color) : Piece(color, (color == WHITE) ? WHITE_KING : BLACK_KING) {}
    bool isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) override;
    vector<pair<int, int>> getPossibleMoves(int row, int col, Board& board) override;
};

Board::Board() {
    squares = vector<vector<shared_ptr<Piece>>>(8, vector<shared_ptr<Piece>>(8, nullptr));


    squares[0][0] = make_shared<Rook>(BLACK);
    squares[0][1] = make_shared<Knight>(BLACK);
    squares[0][2] = make_shared<Bishop>(BLACK);
    squares[0][3] = make_shared<Queen>(BLACK);
    squares[0][4] = make_shared<King>(BLACK);
    squares[0][5] = make_shared<Bishop>(BLACK);
    squares[0][6] = make_shared<Knight>(BLACK);
    squares[0][7] = make_shared<Rook>(BLACK);
    for (int j = 0; j < 8; j++) {
        squares[1][j] = make_shared<Pawn>(BLACK);
    }


    squares[7][0] = make_shared<Rook>(WHITE);
    squares[7][1] = make_shared<Knight>(WHITE);
    squares[7][2] = make_shared<Bishop>(WHITE);
    squares[7][3] = make_shared<Queen>(WHITE);
    squares[7][4] = make_shared<King>(WHITE);
    squares[7][5] = make_shared<Bishop>(WHITE);
    squares[7][6] = make_shared<Knight>(WHITE);
    squares[7][7] = make_shared<Rook>(WHITE);
    for (int j = 0; j < 8; j++) {
        squares[6][j] = make_shared<Pawn>(WHITE);
    }
}

void Board::print() const {
    cout << "\n    a   b   c   d   e   f   g   h  \n";
    cout << "  +---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < 8; i++) {
        cout << (8 - i) << " |";
        for (int j = 0; j < 8; j++) {
            if (squares[i][j]) {
                cout << " " << squares[i][j]->symbol << " |";
            } else {
                cout << " " << EMPTY_SQUARE << " |";
            }
        }
        cout << " " << (8 - i) << "\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";
    }
    cout << "    a   b   c   d   e   f   g   h  \n\n";
}


bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool Board::isValidMoveIgnoringCheck(int startRow, int startCol, int endRow, int endCol, PIECE_COLOR currentPlayer) {
    if (!isValidPosition(startRow, startCol) || !isValidPosition(endRow, endCol)) {
        return false;
    }
    auto piece = squares[startRow][startCol];
    if (!piece || piece->color != currentPlayer) {
        return false;
    }
    if (!piece->isValidMove(startRow, startCol, endRow, endCol, *this)) {
        return false;
    }
    return true;
}

bool Board::movePiece(int startRow, int startCol, int endRow, int endCol, PIECE_COLOR currentPlayer) {
    if (!isValidMoveIgnoringCheck(startRow, startCol, endRow, endCol, currentPlayer)) {
        cout << "Invalid move for the piece." << endl;
        return false;
    }


    auto piece = squares[startRow][startCol];
    auto capturedPiece = squares[endRow][endCol];
    squares[endRow][endCol] = piece;
    squares[startRow][startCol] = nullptr;
    bool previousHasMoved = piece->hasMoved;
    piece->hasMoved = true;

    if (isInCheck(currentPlayer)) {
 
        squares[startRow][startCol] = piece;
        squares[endRow][endCol] = capturedPiece;
        piece->hasMoved = previousHasMoved;
        cout << "You cannot move into check." << endl;
        return false;
    }


    if (auto pawn = dynamic_pointer_cast<Pawn>(piece)) {
        if ((pawn->color == WHITE && endRow == 0) || (pawn->color == BLACK && endRow == 7)) {
            promotePawn(endRow, endCol);
        }
    }

 
    string stateKey = boardStateKey();
    moveHistory[stateKey]++;

    return true;
}


bool NOTATION_TO_INDEX(const string& notation, int& row, int& col) {
    if (notation.length() != 2) {
        return false;
    }
    char file = __tolower(notation[0]); 
    char rank = notation[1];
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return false;
    }
    col = file - 'a';
    row = 8 - (rank - '0');
    return true;
}


bool GET_PLAYER_MOVE(int& startRow, int& startCol, int& endRow, int& endCol) {
    string input;
    getline(cin, input);


    if (input.length() < 5) {
        cout << "Invalid input format. Please use the format 'e2 e4'." << endl;
        return false;
    }


    string startPos = input.substr(0, 2);
    string endPos = input.substr(3, 2);


    if (!NOTATION_TO_INDEX(startPos, startRow, startCol) ||
        !NOTATION_TO_INDEX(endPos, endRow, endCol)) {
        cout << "Invalid notation. Please use the format 'e2 e4'." << endl;
        return false;
    }

    return true;
}

void Board::promotePawn(int row, int col) {
    cout << "Pawn promotion! Choose a piece (Q, R, B, N): ";
    char choice;
    cin >> choice;
    choice = __toupper(choice); 
    PIECE_COLOR color = squares[row][col]->color;
    switch (choice) {
    case 'Q':
        squares[row][col] = make_shared<Queen>(color);
        break;
    case 'R':
        squares[row][col] = make_shared<Rook>(color);
        break;
    case 'B':
        squares[row][col] = make_shared<Bishop>(color);
        break;
    case 'N':
        squares[row][col] = make_shared<Knight>(color);
        break;
    default:
        cout << "Invalid choice, promoting to Queen by default." << endl;
        squares[row][col] = make_shared<Queen>(color);
    }
    cin.ignore();
}

bool Board::isSquareUnderAttack(int row, int col, PIECE_COLOR attackerColor) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = squares[i][j];
            if (piece && piece->color == attackerColor) {
                if (piece->isValidMove(i, j, row, col, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::isInCheck(PIECE_COLOR color) {

    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8 && kingRow == -1; i++) {
        for (int j = 0; j < 8; j++) {
            auto king = dynamic_pointer_cast<King>(squares[i][j]);
            if (king && king->color == color) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }
    if (kingRow == -1) {
        return true; 
    }
    PIECE_COLOR opponentColor = (color == WHITE) ? BLACK : WHITE;
    return isSquareUnderAttack(kingRow, kingCol, opponentColor);
}

bool Board::isCheckmate(PIECE_COLOR color) {
    if (!isInCheck(color)) {
        return false;
    }
  
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = squares[i][j];
            if (piece && piece->color == color) {
                auto moves = piece->getPossibleMoves(i, j, *this);
                for (auto move : moves) {
                    auto capturedPiece = squares[move.first][move.second];
                    squares[move.first][move.second] = piece;
                    squares[i][j] = nullptr;
                    bool stillInCheck = isInCheck(color);
                    // Undo the move
                    squares[i][j] = piece;
                    squares[move.first][move.second] = capturedPiece;
                    if (!stillInCheck) {
                        return false; 
                    }
                }
            }
        }
    }
    return true; 
}

bool Board::isStalemate(PIECE_COLOR color) {
    if (isInCheck(color)) {
        return false;
    }
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto piece = squares[i][j];
            if (piece && piece->color == color) {
                auto moves = piece->getPossibleMoves(i, j, *this);
                for (auto move : moves) {
                    auto capturedPiece = squares[move.first][move.second];
                    squares[move.first][move.second] = piece;
                    squares[i][j] = nullptr;
                    bool stillInCheck = isInCheck(color);
                 
                    squares[i][j] = piece;
                    squares[move.first][move.second] = capturedPiece;
                    if (!stillInCheck) {
                        return false; 
                    }
                }
            }
        }
    }
    return true; 
}

string Board::boardStateKey() const {
    string key;
    for (auto& row : squares) {
        for (auto& piece : row) {
            if (piece) {
                key += piece->symbol;
            } else {
                key += EMPTY_SQUARE;
            }
        }
    }
    return key;
}



bool Pawn::isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) {
    int direction = (color == WHITE) ? -1 : 1;
    int rowDiff = endRow - startRow;
    int colDiff = endCol - startCol;
    auto& squares = board.squares;


    if (colDiff == 0 && squares[endRow][endCol] == nullptr) {
        if (rowDiff == direction) {
            return true;
        }

        if (!hasMoved && rowDiff == 2 * direction && squares[startRow + direction][startCol] == nullptr) {
            return true;
        }
    }
    // Capture move
    if (std::abs(colDiff) == 1 && rowDiff == direction) { // Use organization's internal abs
        if (squares[endRow][endCol] && squares[endRow][endCol]->color != color) {
            return true;
        }
        // En passant
        // I'll add en-passant handler in future :)
    }
    return false;
}

vector<pair<int, int>> Pawn::getPossibleMoves(int row, int col, Board& board) {
    vector<pair<int, int>> moves;
    int direction = (color == WHITE) ? -1 : 1;
    auto& squares = board.squares;

    // Forward move
    int nextRow = row + direction;
    if (board.isValidPosition(nextRow, col) && squares[nextRow][col] == nullptr) {
        moves.push_back({ nextRow, col });
        // Double move from starting position
        int doubleNextRow = nextRow + direction;
        if (!hasMoved && board.isValidPosition(doubleNextRow, col) && squares[doubleNextRow][col] == nullptr) {
            moves.push_back({ doubleNextRow, col });
        }
    }
    // Capture moves
    for (int deltaCol = -1; deltaCol <= 1; deltaCol += 2) {
        int nextCol = col + deltaCol;
        if (board.isValidPosition(nextRow, nextCol)) {
            auto targetPiece = squares[nextRow][nextCol];
            if (targetPiece && targetPiece->color != color) {
                moves.push_back({ nextRow, nextCol });
            }
            // En passant can be added here if implemented
        }
    }
    return moves;
}

bool Knight::isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) {
    int rowDiff = std::abs(endRow - startRow); // Use organization's internal abs
    int colDiff = std::abs(endCol - startCol);
    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
        auto& squares = board.squares;
        auto targetPiece = squares[endRow][endCol];
        if (!targetPiece || targetPiece->color != color) {
            return true;
        }
    }
    return false;
}

vector<pair<int, int>> Knight::getPossibleMoves(int row, int col, Board& board) {
    vector<pair<int, int>> moves;
    int rowOffsets[] = { -2, -1, 1, 2, 2, 1, -1, -2 };
    int colOffsets[] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    auto& squares = board.squares;

    for (int i = 0; i < 8; i++) {
        int newRow = row + rowOffsets[i];
        int newCol = col + colOffsets[i];
        if (board.isValidPosition(newRow, newCol)) {
            auto targetPiece = squares[newRow][newCol];
            if (!targetPiece || targetPiece->color != color) {
                moves.push_back({ newRow, newCol });
            }
        }
    }
    return moves;
}

bool Bishop::isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) {
    int rowDiff = std::abs(endRow - startRow); // Use organization's internal abs
    int colDiff = std::abs(endCol - startCol);
    if (rowDiff == colDiff && rowDiff != 0) {
        int rowStep = (endRow > startRow) ? 1 : -1;
        int colStep = (endCol > startCol) ? 1 : -1;
        int currentRow = startRow + rowStep;
        int currentCol = startCol + colStep;
        auto& squares = board.squares;
        while (currentRow != endRow && currentCol != endCol) {
            if (squares[currentRow][currentCol]) {
                return false; // Path is blocked
            }
            currentRow += rowStep;
            currentCol += colStep;
        }
        auto targetPiece = squares[endRow][endCol];
        if (!targetPiece || targetPiece->color != color) {
            return true;
        }
    }
    return false;
}

vector<pair<int, int>> Bishop::getPossibleMoves(int row, int col, Board& board) {
    vector<pair<int, int>> moves;
    int directions[4][2] = { {1, 1}, {1, -1}, {-1, -1}, {-1, 1} };
    auto& squares = board.squares;

    for (auto& dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        while (board.isValidPosition(newRow, newCol)) {
            auto targetPiece = squares[newRow][newCol];
            if (targetPiece) {
                if (targetPiece->color != color) {
                    moves.push_back({ newRow, newCol });
                }
                break; // Blocked by a piece
            } else {
                moves.push_back({ newRow, newCol });
            }
            newRow += dir[0];
            newCol += dir[1];
        }
    }
    return moves;
}

bool Rook::isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) {
    if (startRow == endRow || startCol == endCol) {
        int rowStep = (endRow - startRow == 0) ? 0 : ((endRow > startRow) ? 1 : -1);
        int colStep = (endCol - startCol == 0) ? 0 : ((endCol > startCol) ? 1 : -1);
        int currentRow = startRow + rowStep;
        int currentCol = startCol + colStep;
        auto& squares = board.squares;
        while (currentRow != endRow || currentCol != endCol) {
            if (squares[currentRow][currentCol]) {
                return false; // Path is blocked
            }
            currentRow += rowStep;
            currentCol += colStep;
        }
        auto targetPiece = squares[endRow][endCol];
        if (!targetPiece || targetPiece->color != color) {
            return true;
        }
    }
    return false;
}

vector<pair<int, int>> Rook::getPossibleMoves(int row, int col, Board& board) {
    vector<pair<int, int>> moves;
    int directions[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
    auto& squares = board.squares;

    for (auto& dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];
        while (board.isValidPosition(newRow, newCol)) {
            auto targetPiece = squares[newRow][newCol];
            if (targetPiece) {
                if (targetPiece->color != color) {
                    moves.push_back({ newRow, newCol });
                }
                break; // Blocked by a piece
            } else {
                moves.push_back({ newRow, newCol });
            }
            newRow += dir[0];
            newCol += dir[1];
        }
    }
    return moves;
}

bool Queen::isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) {
    Bishop bishop(color);
    Rook rook(color);
    if (bishop.isValidMove(startRow, startCol, endRow, endCol, board) ||
        rook.isValidMove(startRow, startCol, endRow, endCol, board)) {
        return true;
    }
    return false;
}

vector<pair<int, int>> Queen::getPossibleMoves(int row, int col, Board& board) {
    vector<pair<int, int>> moves;
    Bishop bishop(color);
    Rook rook(color);
    auto bishopMoves = bishop.getPossibleMoves(row, col, board);
    auto rookMoves = rook.getPossibleMoves(row, col, board);
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    return moves;
}

bool King::isValidMove(int startRow, int startCol, int endRow, int endCol, Board& board) {
    int rowDiff = std::abs(endRow - startRow); // Use organization's internal abs
    int colDiff = std::abs(endCol - startCol);
    auto& squares = board.squares;

    if ((rowDiff <= 1 && colDiff <= 1) && (rowDiff + colDiff > 0)) {
        auto targetPiece = squares[endRow][endCol];
        if (!targetPiece || targetPiece->color != color) {
            // Ensure the king is not moving into check
            PIECE_COLOR opponentColor = (color == WHITE) ? BLACK : WHITE;
            if (!board.isSquareUnderAttack(endRow, endCol, opponentColor)) {
                return true;
            }
        }
    }
    // Castling can be implemented here if needed
    return false;
}

vector<pair<int, int>> King::getPossibleMoves(int row, int col, Board& board) {
    vector<pair<int, int>> moves;
    auto& squares = board.squares;
    PIECE_COLOR opponentColor = (color == WHITE) ? BLACK : WHITE;

    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if (deltaRow == 0 && deltaCol == 0) continue;
            int newRow = row + deltaRow;
            int newCol = col + deltaCol;
            if (board.isValidPosition(newRow, newCol)) {
                auto targetPiece = squares[newRow][newCol];
                if (!targetPiece || targetPiece->color != color) {
                    if (!board.isSquareUnderAttack(newRow, newCol, opponentColor)) {
                        moves.push_back({ newRow, newCol });
                    }
                }
            }
        }
    }
    // Castling moves can be added here if implemented
    return moves;
}

// Main function

int main() {
    Board board;
    PIECE_COLOR currentPlayer = WHITE;
    bool gameOver = false;

    while (!gameOver) {
        board.print();
        string playerStr = (currentPlayer == WHITE) ? "White" : "Black";
        cout << playerStr << "'s move." << endl;

        // Check for checkmate or stalemate
        if (board.isCheckmate(currentPlayer)) {
            cout << "Checkmate! " << ((currentPlayer == WHITE) ? "Black" : "White") << " wins!" << endl;
            gameOver = true;
            break;
        } else if (board.isStalemate(currentPlayer)) {
            cout << "Stalemate! It's a draw." << endl;
            gameOver = true;
            break;
        } else if (board.moveHistory[board.boardStateKey()] >= 3) {
            cout << "Draw by threefold repetition." << endl;
            gameOver = true;
            break;
        }

        // Check if the player is in check
        if (board.isInCheck(currentPlayer)) {
            cout << "You are in check!" << endl;
        }

        // Get and process player move
        int startRow, startCol, endRow, endCol;
        if (!GET_PLAYER_MOVE(startRow, startCol, endRow, endCol)) {
            continue; // Invalid input, prompt again
        }
        if (!board.movePiece(startRow, startCol, endRow, endCol, currentPlayer)) {
            continue; // Invalid move, prompt again
        }

        // Switch players
        currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
    }
    return 0;
}
