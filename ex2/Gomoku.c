//
// Created by black_knight on 11/23/15.
// nissim levy
// THIS CODE IS NOT FINISHED AND NOT WORKING YET.


#include <assert.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Board.h"
#include <ctype.h>

# define PUT_SQUARE 1
# define DELETE_SQUARE 2
# define CHANGE_PRINT 3
# define EXIT_PROGRAM 4

const int ARGC_NUM = 3, EXIT_CODE = 1, INPUT_ARG = 1, OUTPUT_ARG = 2, COR_LINE = 1, ACTION_LINE = 0, ZERO = 0,
STARTING_LINE_NUM = 1, DEFAULT_X = 0, DEAFULT_Y =0, ACTION_DIGIT_PLACE = 0;

const char EMPTY_SQUARE = ' ';
static int convertCharToInt(char c);

static bool checkLineValidity(int lineType, char *line);

static bool checkActionLine(char *line);

static bool checkCordLine(char *line);

static bool checkFirstdigit(char *firstNumber);

static bool checkSeconddigit(char *firstNumber);


int main(int argc, char *argv[]) {
    return 0;

    //CHECKS BEFOE RUNNING
    // validate args num
    if (argc != ARGC_NUM) {
        fprintf(stderr, "Wrong parameters. Usage:\n Gomoku <input_file> <output_file>");
        exit(EXIT_CODE);
    }

    FILE *inputFile = fopen(argv[INPUT_ARG], "r");
    FILE *outputFile = fopen(argv[OUTPUT_ARG], "w");
    // validate input and outpout files
    if (inputFile == NULL || outputFile == NULL) {
        fprintf(stderr, "Can not open file: <file_name>");
        exit(EXIT_CODE);
    }
    BoardP theBoard = createNewDefaultBoard();

    if (theBoard == NULL) {
        exit(EXIT_CODE);
    }

    //PROGRAM FLOW
    int lineType = COR_LINE;
    int lineNum = STARTING_LINE_NUM;
    int action;
    int getX, getY, currentPlayer, curX = DEFAULT_X, curY = DEAFULT_Y;
    char line[80];
    fgets(line, 80, inputFile);

    while (line != NULL) {

        if (getWinner(theBoard) != EMPTY_SQUARE)
        {
            printBoard(outputFile, theBoard, curX, curY);
            fprintf(outputFile, "\n\nPlayer \'%c\' won!!!\n\n", getWinner(theBoard));
            freeBoard(theBoard);
            fclose(inputFile);
            fclose(outputFile);
            return ZERO;
        }
        fgets(line, 80, inputFile);

        if (checkLineValidity(lineType, line) == false) {
            fprintf(outputFile, "Wrong format input file at line %d\n", lineNum);
            exit(EXIT_CODE);
        }
        if (lineType == ACTION_LINE) {
            action = convertCharToInt(line[ACTION_DIGIT_PLACE]);
        }
        else {
            // NOT_FINISHED_CODE

            //getX =
            //getY =
            switch (action) {
                case PUT_SQUARE:
                    putBoardSquare(theBoard, getX, getY, currentPlayer);
                    break;
                case DELETE_SQUARE:
                    cancelMove(theBoard, getX, getY);
                    break;
                case CHANGE_PRINT:
                    curX = getX;
                    curY = getY;
                    break;
                case EXIT_PROGRAM:
                    break;
                default:
                    exit(EXIT_CODE);
            }
        }
        if (lineType == ACTION_LINE)
            lineType = COR_LINE;
        else
            lineType = ACTION_LINE;
    }
    putBoardSquare(theBoard, getX, getY, currentPlayer);
    fprintf(outputFile, "The game ended with a tie...\n");
    fclose(inputFile);
    fclose(outputFile);
    return (0);
}


static bool checkLineValidity(int lineType, char *line) {
    if (lineType == ACTION_LINE) {
        return checkActionLine(line);
    }
    else
        return checkCordLine(line);
    return true;

}

static bool checkActionLine(char *line) {
    if (strlen(line) != 2) {
        return false;
    }
    if ((line[1] != '\n') || (line[2] != '\0')) {
        return false;
    }
    if (0 == isdigit(convertCharToInt(line[0]))) {
        return false;
    }
    return true;
}

static bool checkCordLine(char *line) {
    const char del[1] = ",";
    char *firstNumber, *secondNumber;
    if (strlen(line) < 6) {
        return false;
    }

    if (line[strlen(line) - 2] != ')') {
        return false;
    }
    firstNumber = strtok(line, del);
    if (firstNumber == NULL) {
        return false;
    }

    secondNumber = strtok(NULL, del);
    if (secondNumber == NULL) {
        return false;
    }
    if (strtok(line, del) == NULL) {
        return false;
    }

    if ((!checkFirstdigit(firstNumber)) || (!checkSeconddigit(secondNumber)))
        return false;
    return true;
}

static int convertCharToInt(char c) {
    c = c - '0';
    int cVal = (int) c;
    return cVal;
}

static bool checkFirstdigit(char *firstNumber) {

    for (unsigned int i = 1; i < strlen(firstNumber); i++) {
        if (0 == isdigit(firstNumber[i])) {
            return false;
        }
    }
    return true;
}

static bool checkSeconddigit(char *secondNumber) {
    for (unsigned int i = 0; i < strlen(secondNumber) - 2; i++) {
        if (0 == isdigit(secondNumber[i])) {
            return false;
        }
    }
    return true;
}

