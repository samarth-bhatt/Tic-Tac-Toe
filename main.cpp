#include <iostream>
#include <list>
#include <utility>
#include <limits> 

using namespace std;

// Enum to represent the types of playing pieces
enum PieceType { EMPTY, X, O };

// A class representing a playing piece
class PlayingPiece {
public:
    PieceType type;

    // Default constructor (for EMPTY type)
    PlayingPiece() : type(EMPTY){}

    // Constructor to set the piece type
    PlayingPiece(PieceType pieceType) : type(pieceType){}
};

// A class representing the game board
class Board {
public:
    int size;
    PlayingPiece** board;

    // Constructor to create a board with a specified size and default piece type
    Board(int boardSize, PieceType defaultPieceType = EMPTY) : size(boardSize){
        board = new PlayingPiece*[size];
        for(int i = 0; i < size; i++){
            board[i] = new PlayingPiece[size];
        }
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                board[i][j] = PlayingPiece(defaultPieceType); // Initialize each cell with the default piece type
            }
        }
    }

    // Destructor to free memory
    ~Board(){
        for(int i = 0; i < size; i++){
            delete[] board[i];
        }
        delete[] board;
    }

    // Place a piece on the board at the specified row and column
    bool placePiece(int row, int column, PlayingPiece piece){
        if(board[row][column].type == EMPTY){
            board[row][column] = piece; // Place the piece on the board
            return true;
        }
        return false; // Piece placement failed (cell is not empty)
    }

    // Get a list of empty cells on the board
    list<pair<int, int>> getEmptyCells(){
        list<pair<int, int>> emptyCells;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(board[i][j].type == EMPTY){
                    emptyCells.push_back(make_pair(i, j)); // Add empty cell coordinates to the list
                }
            }
        }
        return emptyCells;
    }

    // Print the current state of the board
    void printBoard(){
        for(int i = 0; i < size; i++){
            cout << "|";
            for(int j = 0; j < size; j++){
                if(board[i][j].type != EMPTY){
                    cout << (board[i][j].type == X ? "X" : "O") << " "; // Print 'X' or 'O' for player pieces
                } else{
                    cout << "  "; // Print empty cell as spaces
                }
                cout << "|";
            }
            cout << endl;
        }
    }
};

// A class representing a player
class Player {
public:
    PlayingPiece piece;
    string name;

    // Constructor to initialize player name and piece
    Player(const string& playerName, PlayingPiece playerPiece)
        : name(playerName), piece(playerPiece){}

    // Get the player's name
    string getName(){ return name; }

    // Set the player's name
    void setName(const string& playerName){ name = playerName; }

    // Get the player's piece
    PlayingPiece getPiece(){ return piece; }

    // Set the player's piece
    void setPiece(PlayingPiece playerPiece){ piece = playerPiece; }
};

// A class representing the game
class Game {
public:
    list<Player> players;
    Board* gameBoard;

    // Initialize the game by creating players and the game board
    void initializeGame(){
        // Create the playing pieces
        PlayingPiece crossPiece(X);
        PlayingPiece noughtPiece(O);

        // Create players with names and pieces
        Player player1("Player 1", crossPiece);
        Player player2("Player 2", noughtPiece);

        // Add players to the list of players
        players.push_back(player1);
        players.push_back(player2);

        // Create the game board with a size of 3x3
        int boardSize = 3;
        Board* board = new Board(boardSize);
        gameBoard = board;
    }

    // Start and manage the game
    string startGame(){
        bool noWinner = true;
        while (noWinner){
            Player currentPlayer = players.front();
            players.pop_front();

            gameBoard->printBoard();
            list<pair<int, int>> emptyCells = gameBoard->getEmptyCells();
            if(emptyCells.empty()){
                noWinner = false; // Game ends in a tie
                continue;
            }

            cout << "Player " << currentPlayer.getName() << " - Enter row and column (e.g., '1 2'): ";
            int inputRow, inputColumn;

            if(!(cin >> inputRow >> inputColumn) || inputRow < 0 || inputColumn < 0 || inputRow >= gameBoard->size || inputColumn >= gameBoard->size){
                cout << "Invalid input. Please enter valid row and column values." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                players.push_front(currentPlayer);
                continue;
            }

            bool piecePlacedSuccessfully = gameBoard->placePiece(inputRow, inputColumn, currentPlayer.getPiece());

            if(!piecePlacedSuccessfully){
                cout << "Invalid position, try again." << endl;
                players.push_front(currentPlayer);
                continue;
            }
            players.push_back(currentPlayer);

            bool winner = checkForWinner(inputRow, inputColumn, currentPlayer.getPiece().type);
            if(winner){
                return currentPlayer.getName();
            }
        }

        return "Tie";
    }

    // Check for a winner on the board after each move
    bool checkForWinner(int row, int column, PieceType pieceType){
        bool rowMatch = true;
        bool columnMatch = true;
        bool diagonalMatch = true;
        bool antiDiagonalMatch = true;

        // Check the row
        for(int i = 0; i < gameBoard->size; i++){
            if(gameBoard->board[row][i].type == EMPTY || gameBoard->board[row][i].type != pieceType){
                rowMatch = false;
            }
        }

        // Check the column
        for(int i = 0; i < gameBoard->size; i++){
            if(gameBoard->board[i][column].type == EMPTY || gameBoard->board[i][column].type != pieceType){
                columnMatch = false;
            }
        }

        // Check the main diagonal
        for(int i = 0, j = 0; i < gameBoard->size; i++, j++){
            if(gameBoard->board[i][j].type == EMPTY || gameBoard->board[i][j].type != pieceType){
                diagonalMatch = false;
            }
        }

        // Check the anti-diagonal
        for(int i = 0, j = gameBoard->size - 1; i < gameBoard->size; i++, j--){
            if(gameBoard->board[i][j].type == EMPTY || gameBoard->board[i][j].type != pieceType){
                antiDiagonalMatch = false;
            }
        }

        return rowMatch || columnMatch || diagonalMatch || antiDiagonalMatch;
    }
};

int main(){
    // Game manual
    cout << "\n=== Game Manual ===" << endl;
    cout << "Welcome to Tic-Tac-Toe!" << endl;
    cout << "Instructions:" << endl;
    cout << "1. Players take turns to place their 'X' or 'O' on the board." << endl;
    cout << "2. Enter the row and column (e.g., '1 2') to place your piece." << endl;
    cout << "3. The first player to get three of their pieces in a row, column, or diagonal wins." << endl;
    cout << "4. If all cells are filled and no winner is determined, the game ends in a tie." << endl;
    cout << "5. Have fun and enjoy the game!" << endl;
    
    Game newGame;
    newGame.initializeGame();
    cout << "Tic-Tac-Toe Game Started" << endl;

    string result = newGame.startGame();
    cout << "Winner: " << result << endl;
}
