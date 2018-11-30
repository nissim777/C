#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Board.h"
#include "ErrorHandle.h"

const char EMPTY_SPACE = ' ', X = 'X', O = 'O', OUT_OF_BOUND_CHAR = 'E';
const bool PLAYER1_TURN = false, PLAYER2_TURN = true;
const int REFACTOR_SIZE = 2, INITALIZED = 0, WIN_SEQ = 5, SCAN_RADIUS = 2, DEAFULT_BOARD_SIZE = 10, NO_MOVE = 0,
        MOVE_RIGHT = 1, MOVE_DOWN = 1, MOVE_LEFT = -1;

//DECLARATIONS:
static unsigned char **allocateBoard(int rows, int cols);
static void initalizeValues(BoardP myBoard, unsigned char **b, int row, int col);
static void copyValues(ConstBoardP originalBoard, BoardP newBoard);
static void copyBoardValues(ConstBoardP originalBoard, BoardP newBoard);
static unsigned char ** resizeTry(ConstBoardP originalBoard, int newCol, int newRow);
static bool checkSquence(ConstBoardP board, int verticalMove, int horizenMove, char winChar);
static bool checkInBoard(ConstBoardP board, int row, int col);
static unsigned char findWinChar(ConstBoardP board);
static void changeTurn(BoardP board);
static unsigned char ** resizeBoard(BoardP theBoard, int row, int col);

// Allocate memory for a default empty board (size 10X10) and return it.
BoardP createNewDefaultBoard() {
    return createNewBoard(DEAFULT_BOARD_SIZE, DEAFULT_BOARD_SIZE);
}

// Allocate memory for a new board and return it.
BoardP createNewBoard(int rows, int cols) {
    unsigned char **p;
    BoardP myBoard = NULL;
    // allocates memory for the struct
    myBoard = (Board *) malloc(sizeof(Board));
    if (myBoard == NULL) {
        reportError(MEM_OUT);
        return NULL;
    }
    // allocates memory for the board itself
    p = allocateBoard(rows, cols);
    if (p == NULL) {
        free(myBoard);
        reportError(MEM_OUT);
        myBoard = NULL;
    }
    else {
        initalizeValues(myBoard, p, rows, cols);
    }
    return myBoard;
}

// Duplicate the original board and return a new one.
BoardP duplicateBoard(ConstBoardP originalBoard) {
    assert (originalBoard != NULL);
    // NOTICE: also reports error in case allocation doesn't succses
    BoardP newBoard = createNewBoard(originalBoard->rows, originalBoard->cols);
    if (newBoard == NULL) {
        return NULL;
    }
    copyValues(originalBoard, newBoard);
    return newBoard;
}

/**
* Check the value of the square row,col on the board, Return 'X' if X player owns the square,
* 'O' (big o, not a zero) if O player owns the square, and ' ' (a space) if it's an empty square.
*/
char getBoardSquare(ConstBoardP theBoard, int row, int col) {
    assert (theBoard != NULL);
    if (row < 0 || col < 0) {
        reportError(OUT_OF_BOUND);
        return OUT_OF_BOUND_CHAR;
    }
    if (row > theBoard->rows || col > theBoard->cols) {
        return EMPTY_SPACE;
    }
    if (theBoard->b[row][col] == X) {
        return X;
    }
    if (theBoard->b[row][col] == O) {
        return O;
    }
    return EMPTY_SPACE;
}

/**
 * Make a move: mark on location (row,col) the value 'val'.
 */
bool putBoardSquare(BoardP theBoard, int row, int col, char val) {
    assert (theBoard != NULL);
    if (row < 0 || col < 0) {
        reportError(OUT_OF_BOUND);
        return false;
    }
    if (val != X && val != O) {
        reportError(BAD_VAL);
        return false;
    }
    if ((val == X && theBoard->playerTurn == PLAYER2_TURN) || (val == O && theBoard->playerTurn == PLAYER1_TURN)) {
        reportError(WRONG_TURN);
        return false;
    }
    // Case the square is exceeding the board
    if (row >= theBoard->rows || col >= theBoard->cols) {
        unsigned  char ** newBoardTable;

        newBoardTable = resizeBoard(theBoard, row, col);
        // Case of reallocation failed
        if (newBoardTable == NULL) {
            free (newBoardTable);
            return false;
        }

        theBoard->b = newBoardTable;
        // update rows and cols
        if (row >=  theBoard->rows)
            theBoard->rows = row*REFACTOR_SIZE;
        if (col >=  theBoard->cols)
            theBoard->cols = col*REFACTOR_SIZE;
        assert (row <= theBoard->rows && col <= theBoard->cols);
    }
        // Case the square already full
    else if (theBoard->b[row][col] != EMPTY_SPACE) {
        reportError(SQUARE_FULL);
        return false;
    }
    theBoard->b[row][col] = val;
    theBoard->lastRowPlayed = row;
    theBoard->lastColPlayed = col;
    printf("%d",theBoard->rows);
    changeTurn(theBoard);
    return true;
}

/**
 * Cancel a move (not necessarily the last move) - turn the square back to be empty.
 * If everything is OK, return true. Otherwise (an error accure) return false.
 */
bool cancelMove(BoardP theBoard, int x, int y) {
    assert (theBoard != NULL);
    if (x < 0 || y < 0) {
        reportError(OUT_OF_BOUND);
        return false;
    }
    // Case already empty or player trying to cancle his own move
    if ((theBoard->b[x][y] == EMPTY_SPACE) || (theBoard->b[x][y] == X && theBoard->playerTurn == PLAYER1_TURN) ||
        (theBoard->b[x][y] == O && theBoard->playerTurn == PLAYER2_TURN)) {
        reportError(ILLEGAL_CANCELLATION);
        return false;
    }
    // cancle the move
    theBoard->b[x][y] = EMPTY_SPACE;
    changeTurn(theBoard);
    return true;
}

/**
 * Check if there's a winner on the board - a player that owns 5 squares in a line (vertical, horizontal or diag)
 * Return the player ('X' / 'O') is such exists, or ' ' (space) otherwise.
 */
char getWinner(ConstBoardP board) {
    assert (board != NULL);
    char winChar = findWinChar(board);
    if (winChar == EMPTY_SPACE) {
        return winChar;
    }
    // uses checkSquence function, when (MOVE,MOVE) indicates the direction of the check on axis (x,y) resp.
    if (checkSquence(board, MOVE_RIGHT, MOVE_DOWN, winChar) || checkSquence(board, MOVE_DOWN, NO_MOVE, winChar)
        || checkSquence(board, NO_MOVE, MOVE_RIGHT, winChar) ||
        checkSquence(board, MOVE_LEFT, MOVE_DOWN, winChar)) {
        return winChar;
    }
    return EMPTY_SPACE;
}

/**
 * Free all the memory allocated for the board. It's the user responsibility to call this function before
   exiting the program.
 */
void freeBoard(BoardP board) {
    assert (board != NULL);
    int i;
    for (i = 0; i < board->rows; i++) {
        free(board->b[i]);
    }
    free(board->b);
    free(board);
}

/**
* Print the board into stream  position (row,col).
* If row or col are negative, report OUT_OF_BOUND and do nothing.
* Squares outside of the current memory location should be printed as empty squares.
*/
void printBoard(FILE *stream, ConstBoardP board, int row, int col) {
    assert (board != NULL);
    if (row < 0 || col < 0) {
        reportError(OUT_OF_BOUND);
        return;
    }
    printf("   ");
    // prints col numbers
    int i;
    for (i = col; i < col + 10; i++) {
        printf("+%d ", i);
    }
    printf("\n");

    for (int i = row; i < row + 10; i++) {
        // prints row number
        printf("+%d ", i);
        // prints row values
        for (int k = col; k < col + 10; k++) {
            // case exceed board
            if (i >= board->rows || k >= board->cols) {
                fprintf(stream, " %c ", EMPTY_SPACE);
            }
            else {
                fprintf(stream, " %c ", board->b[i][k]);
            }
        }
        fprintf(stream, "\n");
    }
    fprintf(stream, "\n");
}

/**
 * return the actual memory size that was allocated to the
 */
int getAllocatedSize(ConstBoardP board) {
    assert (board != NULL);
    return (board->rows) * (board->cols);
}

/**
* tries to allocate the board itself, according to table size required
*/
static unsigned char **allocateBoard(int rows, int cols) {
    unsigned char **p;
    p = (unsigned char **) malloc(rows * sizeof(unsigned char *));
    if (!p) {
        return NULL;
    }
    // allocation for sub pointers
    for (int i = 0; i < rows; i++) {
        p[i] = (unsigned char *) malloc(cols * sizeof(unsigned char));
        if (!p[i]) {
            for (int k = 0; k < i; k++) {
                free(p[k]);
            }
            free(p);
            return NULL;
        }
    }
    return p;
}

/**
* initializes the board with default values
*/
static void initalizeValues(BoardP myBoard, unsigned char **b, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            b[i][j] = EMPTY_SPACE;
        }
    }
    myBoard->b = b;
    myBoard->cols = cols;
    myBoard->rows = rows;
    myBoard->playerTurn = PLAYER1_TURN;
    myBoard->lastRowPlayed = INITALIZED;
    myBoard->lastRowPlayed = INITALIZED;
}

/**
* copies all value from original board into new board
*/
static void copyValues(ConstBoardP originalBoard, BoardP newBoard) {
    assert (originalBoard->rows <= newBoard->rows);
    assert (originalBoard->cols <= newBoard->cols);
    copyBoardValues(originalBoard, newBoard);
  //  copyPrimitiveValues(originalBoard, newBoard);
}

/**
* copies all board value from original board into new board
*/
static void copyBoardValues(ConstBoardP originalBoard, BoardP newBoard) {
    for (int i = 0; i < originalBoard->rows; i++) {
        for (int j = 0; j < originalBoard->cols; j++) {
            if (originalBoard->b[i][j] != EMPTY_SPACE) {
                newBoard->b[i][j] = originalBoard->b[i][j];
            }
        }
    }
}

/**
* resizes the board, in case a square is exceeding
*/
static unsigned char ** resizeBoard(BoardP theBoard, int row, int col) {
    int newRow = theBoard->rows, newCol = theBoard->cols;
    if (row > theBoard->rows) {
        newRow = row * REFACTOR_SIZE;
    }
    if (col > theBoard->cols) {
        newCol = col * REFACTOR_SIZE;
    }
    return resizeTry(theBoard, newRow, newCol);
}

/**
* making an attempt for resizing the board
*/
static unsigned char ** resizeTry(ConstBoardP originalBoard, int newCol, int newRow) {
    BoardP newBoard = NULL;
    newBoard = createNewBoard(newCol, newRow);
    if (newBoard == NULL) {
        return NULL;
    }
    copyValues(originalBoard, newBoard);
    return newBoard->b;
}

/**
* used by getWinner to check if there is a winning sequence
*/
static bool checkSquence(ConstBoardP board, int verticalMove, int horizenMove, char winChar) {
    int winCounter = INITALIZED;
    int startRowIndex = board->lastRowPlayed - ((WIN_SEQ - 1) * verticalMove);
    int startColIndex = board->lastColPlayed - ((WIN_SEQ - 1) * horizenMove);
    int currentRowIndex, currentColIndex;

    for (int i = 0; i < (WIN_SEQ * SCAN_RADIUS) - 1; i++) {
        currentRowIndex = startRowIndex + i * verticalMove;
        currentColIndex = startColIndex + i * horizenMove;

        // Case out of board
        if (checkInBoard(board, currentRowIndex, currentColIndex) == false) {
            if (winCounter != INITALIZED) {
                return false;
            }
        }
        else {
            if (board->b[currentRowIndex][currentColIndex] == winChar) {
                winCounter++;
                if (winCounter == WIN_SEQ) {
                    return true;
                }
            }
            else
                winCounter = INITALIZED;
        }
    }
    return false;
}

/**
* finds out which val might win at that point (last played char)
*/
static unsigned char findWinChar(ConstBoardP board) {
    if (board->b[board->lastRowPlayed][board->lastColPlayed] == X ||
        board->b[board->lastRowPlayed][board->lastColPlayed] == O) {
        return board->b[board->lastRowPlayed][board->lastColPlayed];
    }
    else {
        return EMPTY_SPACE;
    }
}

/**
* checks if a certain (row, col) squere is inside the board
*/
static bool checkInBoard(ConstBoardP board, int row, int col) {
    if (board->rows <= row || board->cols <= col || row < 0 || col < 0) {
        return false;
    }
    return true;
}

/**
* changes player turn
*/
static void changeTurn(BoardP board) {
    if (board->playerTurn == PLAYER1_TURN) {
        board->playerTurn = PLAYER2_TURN;
    }
    else {
        board->playerTurn = PLAYER1_TURN;
    }
}