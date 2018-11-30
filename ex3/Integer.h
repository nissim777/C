
#ifndef INTEGER_H
#define INTEGER_H

#include "Epsilon.h"

//********      types and functions types
/**
* A pointer to an Integer number.
*/
typedef struct Integer {
    int num;
}Integer;

typedef struct Integer* IntegerP;

/**
* A const pointer to an Integer Number.
*/
typedef const struct Integer* ConstIntegerP;

/**
 * @brief create a new Integer number.
 * @param intNum - The value of the Integer.
 * @return IntegerP - A pointer to an Integer struct with the value of the integer num
 */
IntegerP PointerFromInt(int intNum);

/**
 * @brief free the Integer object
 * @param intNum - The Integer to free.
 */
void freeInteger(IntegerP integerNum);

/**
 * @brief multyplying the two numbers, according to regular mult defined to the int nums
 * @param num1 first Integer.
 * @param num2 second Integer.
 * @return IntegerP
 */
IntegerP multInteger(ConstIntegerP num1, ConstIntegerP num2);


/**
 * @brief Create a pointer to a function that compare integers with EPSILON
 * @return A pointer to a function that gets 2 Integer pointers (num1,num2) and return
 * int as following:
 * 0 - iff |num1-num2| <EPSILON
 * otherwize:
 * >0 if num1>num2
 * <0 if num2<num1
 */
int (*getIntCompareFunc())(IntegerP num1, IntegerP num2);

#endif //EX3_INTEGER_H
