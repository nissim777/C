
/**
* SHIFT - Encryption / Decryption program;
* - Contains encryption and decryption functions.

* @file Shift.c
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
* Input  : any text.
* Output : decrypted text or encrypted text respectively.
*/

#include <stdio.h>

#define LETTERS_RANGE 26
#define NUM_RANGE 10
#define INPUT_LENGTH 100
#define INPUT_END '\0'
#define ENTER_SIGN '\n'
#define MIN_SHIFT 0
#define MAX_SHIFT 50
#define DECRYPT 'd'
#define ENCRYPT 'e'
#define ORIGINAL_STRING "\"%s\" -> \""

//DECLAITATIONS:
char decrypt(int shift, char c);
char encrypt(int shift, char c);
int reciveShift();
char recieveOperation();
void printDecodedString(int operationChar, int i, int shift, char * str);

/** Decrypt the given char
* @param shift the offset for decrypt
* @param c the char to decrypt
* @return the decrypted char
*/
char decrypt(int shift, char c)
{
    // if a number between 0-9
    if (c >= '0' && c <= '9')
    {
        shift %= NUM_RANGE;
        return '0' + ((c - '0') + (NUM_RANGE -shift)) % NUM_RANGE;
    }
    // if a letter between A - Z
    else if (c >= 'A' && c <= 'Z')
    {
        shift %= LETTERS_RANGE;
        return 'A' + ((c - 'A') + (LETTERS_RANGE -shift)) % LETTERS_RANGE;
    }
    // if a letter between a - z
    else if (c >= 'a' && c <= 'z')
    {
        shift %= LETTERS_RANGE;
        return 'a' + ((c - 'a') + (LETTERS_RANGE -shift)) % LETTERS_RANGE;
    }
    // if another char
    return c;
}

/**
* @brief Encrypt the given char
* @param c the char to encrypt
* @param shift the offset for encrypt
* @return encrypted char
*/
char encrypt(int shift, char c)
{
    // if a number between 0-9
    if (c >= '0' && c <= '9')
    {
        return '0' + ((c - '0') + shift) % NUM_RANGE;
    }
    // if a letter between A - Z
    else if (c >= 'A' && c <= 'Z')
    {
         return 'A' + ((c - 'A') + shift) % LETTERS_RANGE;
    }
    // if a letter between a - z
    else if (c >= 'a' && c <= 'z')
    {
        return 'a' + ((c - 'a') + shift) % LETTERS_RANGE;
    }
    // if another char
    return c;
}

/**
* @brief recives required shift from user
* @return required shift
*/
int reciveShift()
{
    int shift;
    do
    {
        printf("Please enter the shift amount:\n");
        scanf ("%d", &shift);
        if ( shift < MIN_SHIFT || shift > MAX_SHIFT )
        {
            printf("ERROR: Shift amount should be number between 0 to 50\n");
        }
    }
    while  ( shift < MIN_SHIFT || shift > MAX_SHIFT );
    return shift;
}

/**
* @brief recives required operation from user ('d' for decrypt, 'e' for encrypt)
* @return required shift
*/
char recieveOperation()
{
    char operationChar;
    do
    {
        printf("Would you like to encrypt (e) or decrypt (d)?\n");
        scanf(" %c ", &operationChar);
        if (! (operationChar == DECRYPT || operationChar == ENCRYPT ))
        {
            printf("ERROR: You should type e or d\n");
        }
    }
    while (! (operationChar == DECRYPT || operationChar == ENCRYPT ));
    return operationChar;
}

/**
* @brief prints the decoded string, after proccess of encrypt/descrypt
* @param opeartionChar if needed to decrypt or encrypt
* @param shift the offset for decrypt
* @param i index of the end of the String
* @param str original string needed to be decoded
*/
void printDecodedString(int operationChar, int i, int shift, char * str)
{
    if ( operationChar == DECRYPT)
    {
        for (int k = 0; k < i; k++)
        {
            printf("%c", decrypt(shift, str[k]));
        }
    }
    else
    {
        for (int k = 0; k < i; k++)
        {
            printf("%c", encrypt(shift, str[k]));
        }
    }
    printf("\"\n");
}

int main()
{
    char operationChar, ch, str[INPUT_LENGTH + 1];
    int shift , i = 0;

    shift = reciveShift();
    operationChar = recieveOperation();

    // recives the required line to enc/dec, while chars being ignored after exceed 100 chars
    do
    {
        ch = getchar();
        if ( i < INPUT_LENGTH && ch != ENTER_SIGN && ch!= EOF)
        {
               str[i] = ch;
               i++;
        }
    }
    while ( ch != ENTER_SIGN && ch != EOF);
    str[i] = INPUT_END;

    printf( ORIGINAL_STRING, str);
    printDecodedString( operationChar,  i, shift, str);
    return 0;
}