/**
* COUNT - a program which counts num of rows,words & characters in a specified file

* @file Count.c
* @author black_knight
*         nissim levy
* @date 02/11/15
*
* @section LICENSE
* This program is a free software!
*
* @section DESCRIPTION
* The program encrypts and decrypts text.
*
* Input  : any text, until EOF is pressed
* Output : number of chars, words and rows in the input text
*/


#include <stdio.h>

#define ON 1
#define OFF 0
#define SPACE ' '
#define ENTER '\n'
#define INI_CHAR_NUM 0
#define INI_WORD_NUM 0
#define INI_ROW_NUM 1

int main()
{
    char inputChar;
    // Comment: spaceFlag is ON when the last char was pressed is space
    int characters = INI_CHAR_NUM, words = INI_WORD_NUM, rows = INI_ROW_NUM, spaceFlag = ON;

    while ((inputChar = getchar()) != EOF)
    {
        characters++;
        // if enter was pressed
        if (inputChar == ENTER)
        {
            rows++;
            if (spaceFlag == OFF)
            {
                words++;
            }
            spaceFlag = ON;
        }
        // if space was pressed
        else if (inputChar == SPACE)
        {
            if(spaceFlag == OFF)
            {
                words++;
                spaceFlag = ON;
            }
        }
        // if another character
        else
        {
            spaceFlag = OFF;
        }
    }
    if (spaceFlag == OFF)
    {
        words++;
    }
    printf("Num of rows:%d words:%d characters: %d", rows, words, characters) ;

    return 0;
}