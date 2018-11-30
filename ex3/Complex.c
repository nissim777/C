//
// Created by black_knight on 11/28/15.
// nissim levy

#include "Complex.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
# include <string.h>
#include <ctype.h>
#include<math.h>

const char DELIMETER[3] = ",i", DOT_SIGN = '.', MINUS_SIGN = '-', IMAGINARY_CHAR = 'i', COMMA_SIGN = ',' ;
const int FIRST_GREATER = 1, EQUAL = 0, SECOND_GREATER = -1, ZERO = 0, NEGAITVE = -1;

//DEC:
static bool checkValidity(char *);
static bool checkDigit(char *);
static int compareFunc (ComplexP, ComplexP);

ComplexP fromCharFunc(char *s) {
    ComplexP myComplex = NULL;
    myComplex = (ComplexP) malloc(sizeof(Complex));

    if (myComplex == NULL) {
        return NULL;
    }
    if (!checkValidity(s)) {
        return NULL;
    }
    char *firstNumber, *secondNumber;
    firstNumber = strtok(s, DELIMETER);
    secondNumber = strtok(NULL, DELIMETER);
    myComplex->real_part = atof(firstNumber);
    myComplex->imagin_part = atof(secondNumber);
    return myComplex;
}

int toChar(char *str, int n, ConstComplexP num) {
    if((!str) || (!num) || (n < ZERO))
        return NEGAITVE;
    return snprintf(str, n, "%lf,%lfi", num->real_part, num->imagin_part);

}

void freeComplex(ComplexP num) {
    if (num != NULL)
        free(num);
    num = NULL;
}

ComplexP add(ConstComplexP num1, ConstComplexP num2) {
    if (num1 == NULL || num2 == NULL)
        return NULL;
    ComplexP newComplex = NULL;
    newComplex = (ComplexP) malloc(sizeof(Complex));
    if (newComplex == NULL) {
        return NULL;
    }
    newComplex->real_part = num1->real_part + num2->real_part;
    newComplex->imagin_part = num1->imagin_part + num2->imagin_part;
    return newComplex;
}

ComplexP multScalar(double scalar, ConstComplexP num) {
    if (num == NULL) {
        return NULL;
    }
    ComplexP newComplex = NULL;
    newComplex = (ComplexP) malloc(sizeof(Complex));
    if (newComplex == NULL) {
        return NULL;
    }
    newComplex->real_part = (num->real_part * scalar);
    newComplex->imagin_part = (num->imagin_part * scalar);
    return newComplex;
}

ComplexP mult(ConstComplexP num1, ConstComplexP num2) {
    if (num1 == NULL || num2 == NULL)
        return NULL;
    ComplexP newComplex = NULL;
    newComplex = (ComplexP) malloc(sizeof(Complex));
    if (newComplex == NULL) {
        return NULL;
    }
    newComplex->real_part = (num1->real_part * num2->real_part) - (num1->imagin_part * num2->imagin_part);
    newComplex->imagin_part = (num1->real_part * num2->imagin_part) + (num1->imagin_part * num2->real_part);
    return newComplex;
}


// helping method for validating the syntax send to fromCharFunc
static bool checkValidity(char *t) {

    char *s = (char *) malloc(sizeof(strlen(t)+ 1));
    memcpy(s, t, strlen(t) + 1);

    // CHECK1 i at the end
    if (s[strlen(s) - 1] != IMAGINARY_CHAR)
        return false;

    // CHECK2 only one "," and "i" (neccesary for using strtok)
    int commaCounter = 0, iCounter = 0;
    for (size_t i = 0; i < strlen(s); i++) {
        if (s[i] == COMMA_SIGN)
            commaCounter++;
        else if (s[i] == IMAGINARY_CHAR)
            iCounter++;
    }
    if (commaCounter != 1 || iCounter != 1)
        return false;

    // CHECK3 format is 2 strings and comma between
    char *firstNumber, *secondNumber;
    firstNumber = strtok(s, DELIMETER);
    if (firstNumber == NULL) {
        return false;
    }
    secondNumber = strtok(NULL, DELIMETER);
    if (secondNumber == NULL) {
        return false;
    }
    if (strtok(NULL, DELIMETER) != NULL) {
        return false;
    }

    //CHECK4 both digits
    if (checkDigit(firstNumber) == false || checkDigit(secondNumber) == false) {
        return false;
    }
    free(s);
    return true;
}

// helping method for check is a string is a legal double digit

static bool checkDigit(char *str) {
    int i = 0, dotCounter = 0;
    if (str[0] == MINUS_SIGN)
        i = 1;
    if (str[i] == DOT_SIGN)
        return false;
    if (str[strlen(str) - 1] == DOT_SIGN)
        return false;
    for (; i < (int) strlen(str); i++) {
        if (str[i] == DOT_SIGN)
            dotCounter++;
        else if (!isdigit(str[i]))
            return false;
    }
    if (dotCounter > 1)
        return false;
    return true;
}

static int compareFunc (ComplexP firstNum, ComplexP secondNum)
{
     if (fabs(firstNum->real_part - secondNum->real_part) < EPSILON)
    {
        if (fabs(firstNum->imagin_part - secondNum->imagin_part) < EPSILON)
            return EQUAL;
        else if (firstNum->imagin_part > secondNum->imagin_part)
            return FIRST_GREATER;
        else
            return SECOND_GREATER;
    }
    else if (firstNum->real_part > secondNum->real_part)
        return FIRST_GREATER;
    return SECOND_GREATER;
}

int (*getCompareFunc())(ComplexP, ComplexP)
{
    return compareFunc;
}



