//
// Created by black_knight on 11/30/15.
// nissim levy

#include <stdio.h>
#include <stdlib.h>
#include "Complex.h"
#include "GenGroup.h"
#include "Integer.h"

#define COMPLEX_STRING_LENGTH 50
#define S1 "1,0i"
#define S2 "-1,0i"
#define S3 "2,0i"
#define EXIT_CODE 1

typedef GroupMemberP (*groupOper)(GroupMemberP, GroupMemberP);
typedef void (*groupFree)(GroupMemberP);
typedef int (*groupComp)(GroupMemberP, GroupMemberP);

// DATA FOR EXAMPLES:
const char* COMPLEX_MESSAGE = "Complex Group with complex multipication operation\n";
const char* INTEGER_MESSAGE = "Integer Group with regular multipication operation\n";
const int IDENTITY_LOCATION = 0, GROUP1_SIZE = 2, GROUP2_SIZE = 2, GROUP3_SIZE = 2, GROUP4_SIZE = 2 ;
const int FIRST_NUM = 1, SECOND_NUM = -1, THIRD_NUM = 17;

static void printCopmlexMessage (ComplexP members[], int memberslen, bool isAbel)
{

    char stringToprint[COMPLEX_STRING_LENGTH];
    printf("////////////////\n");
    printf (COMPLEX_MESSAGE);
    toChar(stringToprint, COMPLEX_STRING_LENGTH, members[IDENTITY_LOCATION]);
    printf("Identity number: %s\n", stringToprint);
    printf("Group members: " );
    for (int i = 0; i < memberslen; i++)
    {
        toChar(stringToprint, COMPLEX_STRING_LENGTH, members[i]);
        printf("%s ;", stringToprint);
    }
    printf ("\nThis is ");
    if (!isAbel)
    {
        printf("NOT ");
    }
    printf ("an abelian group");
    printf("\n////////////////\n\n");
}

static void printIntegerMessage (IntegerP members[], int memberslen, bool isAbel)
{

    printf("////////////////\n");
    printf (INTEGER_MESSAGE);
    printf("Identity number: %d\n", members[IDENTITY_LOCATION]->num);
    printf("Group members: " );
    for (int i = 0; i < memberslen; i++)
    {
        printf(" %d ;", members[i]->num);
    }
    printf ("\nThis is ");
    if (!isAbel)
    {
        printf("NOT ");
    }
    printf ("an abelian group");
    printf("\n////////////////\n\n");
}

int main()
{
    // EXAMPLE1 - COMPLEX_NUM
    char s1[COMPLEX_STRING_LENGTH] = S1, s2[COMPLEX_STRING_LENGTH] = S2;
    bool isAbel;

    // turn chars into Complex numbers and enter to group
    ComplexP *group1 = (ComplexP *) malloc(sizeof(ComplexP) * GROUP1_SIZE);
    if (group1 == NULL)
    {
        exit(EXIT_CODE);
    }
    group1[IDENTITY_LOCATION] = fromCharFunc(s1);
    group1[1] = fromCharFunc(s2);

    // check if ablelain group
    isAbel = isAbelianGroup( group1[IDENTITY_LOCATION], (GroupMemberP*)group1, GROUP1_SIZE, (groupOper)mult,
                        (groupFree)freeComplex, (groupComp)getCompareFunc());
    printCopmlexMessage ( group1, GROUP1_SIZE, isAbel);

    // EXAMPLE2 - COMPLEX_NUM
    char s3[COMPLEX_STRING_LENGTH] = S1, s4[COMPLEX_STRING_LENGTH] = S3;

    ComplexP *group2 = (ComplexP *) malloc(sizeof(ComplexP) * GROUP1_SIZE);
    if (group2 == NULL)
    {
        exit(EXIT_CODE);
    }
    group2[IDENTITY_LOCATION] = fromCharFunc(s3);
    group2[1] = fromCharFunc(s4);

    isAbel = isAbelianGroup(group2[IDENTITY_LOCATION], (GroupMemberP*)group2, GROUP1_SIZE, (groupOper)mult,
                             (groupFree)freeComplex, (groupComp)getCompareFunc());
    printCopmlexMessage (group2, GROUP2_SIZE, isAbel);

    // EXAMPLE 3 - int nums

    IntegerP* group3 = (IntegerP*) malloc (sizeof(IntegerP)* GROUP3_SIZE);
    if (group3 == NULL) {
        exit(EXIT_CODE);
    }
    group3[IDENTITY_LOCATION] = PointerFromInt(FIRST_NUM);
    group3[1] = PointerFromInt(SECOND_NUM);

    isAbel = isAbelianGroup( group3[IDENTITY_LOCATION], (GroupMemberP*)group3, GROUP2_SIZE, (groupOper)multInteger,
                             (groupFree)freeInteger, (groupComp)getIntCompareFunc());

    printIntegerMessage ( group3, GROUP3_SIZE, isAbel);

    // EXAMPLE 4 - int nums

    IntegerP* group4 = (IntegerP*) malloc (sizeof(IntegerP)* GROUP4_SIZE);
    if (group4 == NULL)
    {
        exit(EXIT_CODE);
    }
    group4[IDENTITY_LOCATION] = PointerFromInt(FIRST_NUM);
    group4[1] = PointerFromInt(THIRD_NUM);

    isAbel = isAbelianGroup( group4[IDENTITY_LOCATION], (GroupMemberP*)group4, GROUP2_SIZE, (groupOper)multInteger,
                             (groupFree)freeInteger, (groupComp)getIntCompareFunc());

    printIntegerMessage ( group4, GROUP4_SIZE, isAbel);

    for (int i = 0; i<GROUP1_SIZE; i++)
    {
        freeComplex(group1[i]);
    }
    for (int i = 0; i<GROUP2_SIZE; i++)
    {
        freeComplex(group2[i]);
    }
    for (int i = 0; i<GROUP3_SIZE; i++)
    {
        freeInteger(group3[i]);
    }
    for (int i = 0; i<GROUP4_SIZE; i++)
    {
        freeInteger(group4[i]);
    }
    free(group1);
    free(group2);
    free(group3);
    free(group4);
}
