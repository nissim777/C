//
// Created by black_knight on 11/30/15.
// nissim levy


#ifndef GENGROUP_H
#define GENGROUP_H

#include  "GenGroup.h"

//********      types and functions types
/**
* A pointer to group number/member. 
*/

static bool checkIdentityElement(GroupMemberP IdentityElement, GroupMemberP mem, GroupMemberP (*oper)(GroupMemberP,
           GroupMemberP), int (*compare)(GroupMemberP, GroupMemberP), void (*freeMember)(GroupMemberP));
static bool checkInverseExist(GroupMemberP IdentityElement, GroupMemberP mem, GroupMemberP (*oper)(GroupMemberP,
                    GroupMemberP), int membersLen, int (*compare)(GroupMemberP, GroupMemberP), GroupMemberP members[],
                              void (*freeMember)(GroupMemberP));
static bool checkClosure(GroupMemberP mem1, GroupMemberP mem2, GroupMemberP (*oper)(GroupMemberP, GroupMemberP),
  int (*compare)(GroupMemberP, GroupMemberP), int membersLen, GroupMemberP members[], void (*freeMember)(GroupMemberP));
static bool checkComotative(GroupMemberP mem1, GroupMemberP mem2, GroupMemberP (*oper)(GroupMemberP, GroupMemberP),
                            int (*compare)(GroupMemberP, GroupMemberP),void (*freeMember)(GroupMemberP));
static bool checkAsotsyative(GroupMemberP mem1, GroupMemberP mem2, GroupMemberP mem3,
                    GroupMemberP (*oper)(GroupMemberP, GroupMemberP), int (*compare)(GroupMemberP, GroupMemberP),
                             void (*freeMember)(GroupMemberP));


GroupMemberP addFunctions(GroupMemberP  (*add)(GroupMemberP, GroupMemberP), GroupMemberP (*f)(GroupMemberP),
                          GroupMemberP (*g)(GroupMemberP), GroupMemberP n, void (*freeMember)(GroupMemberP))
{
    if (add == NULL || f == NULL || g == NULL || n == NULL || freeMember == NULL) {
        return NULL;
    }

    return add(f(n), g(n));
}

GroupMemberP composeFunctions(GroupMemberP (*f)(GroupMemberP), GroupMemberP (*g)(GroupMemberP), GroupMemberP n,
                              void (*freeMember)(GroupMemberP)) {
    if (f == NULL || g == NULL || n == NULL || freeMember == NULL)
        return NULL;
    return f(g(n));
}

bool isAbelianGroup(GroupMemberP IdentityElement, GroupMemberP members[], int membersLen,
                    GroupMemberP (*oper)(GroupMemberP, GroupMemberP),
                    void (*freeMember)(GroupMemberP), int (*compare)(GroupMemberP, GroupMemberP))
{
    if (IdentityElement == NULL || members == NULL || oper == NULL || freeMember == NULL || compare == NULL)
        return NULL;

    for (int i = 0; i < membersLen; i++) {
        // CHECK Identity Property for each Element
        if (!checkIdentityElement(IdentityElement, members[i], oper, compare, freeMember)) {
            return false;
        }
        // CHECK Existence of Inverte Element for each Element
        if (!checkInverseExist(IdentityElement, members[i], oper, membersLen, compare, members, freeMember)) {
            return false;
        }
        for (int k = i; k < membersLen; k++) {
            // CHECK Closure of each a,b Elements
            if (!checkClosure(members[i], members[k], oper, compare, membersLen, members, freeMember)) {
                return false;
            }
            if (!checkComotative(members[i], members[k], oper, compare, freeMember))
            {
                return false;
            }
            for (int m = k; m < membersLen; m++) {
                if (!checkAsotsyative(members[i], members[k], members[m], oper, compare, freeMember))
                {
                    return false;
                }
            }
        }
    }
    return true;

}


/**
 * @brief A function that gets a group of members and checks if the group satisfies the
 * identity element requirement.
 * @param IdentityElement the Identity Element defined for the group
 * @param members - An array holding all the members in the group.
 * @param membersLen - an int telling the size of members.
 * @param oper - A pointer to an operation function for the group.
 * @param freeMember - A pointer to function that frees a specified group member.
 * @param compare - A pointer to a comparator for the group.
 * @return A boolean value telling whether the given group satisfies the identity
 * element requirement.
 */
static bool checkIdentityElement(GroupMemberP IdentityElement, GroupMemberP mem, GroupMemberP (*oper)(GroupMemberP,
                           GroupMemberP), int (*compare)(GroupMemberP, GroupMemberP),  void (*freeMember)(GroupMemberP))
{
    bool returnValue = true;
    GroupMemberP operResult1, operResult2;
    operResult1 = oper(IdentityElement, mem);
    operResult2 = oper(mem, IdentityElement);
    if (compare(operResult1, operResult2) != 0)
        returnValue = false;
    else if (compare(operResult2, mem) != 0)
        returnValue = false;
    // COMMENT: third check is needed in because <EPS is NOT transitive
    else if (compare(operResult1, mem) != 0)
        returnValue = false;
    freeMember (operResult1);
    freeMember (operResult2);
    return returnValue;
}

/**
 * @brief A function that gets a group of members and checks if the group satisfies the
 * inverse element requirement.
 * @param IdentityElement the Identity Element defined for the group
 * @param members - An array holding all the members in the group.
 * @param membersLen - an int telling the size of members.
 * @param oper - A pointer to an operation function for the group.
 * @param freeMember - A pointer to function that frees a specified group member.
 * @param compare - A pointer to a comparator for the group.
 * @return A boolean value telling whether the given group satisfies the inverse
 * element requirement.
 */
static bool checkInverseExist(GroupMemberP IdentityElement, GroupMemberP mem, GroupMemberP (*oper)(GroupMemberP,
                 GroupMemberP), int membersLen, int (*compare)(GroupMemberP, GroupMemberP), GroupMemberP members[],
        void (*freeMember)(GroupMemberP))
{
    bool returnValue = false;

    for (int i = 0; i < membersLen; i++) {
        GroupMemberP operResult = (oper(members[i], mem));
        if (compare(operResult, IdentityElement) == 0)
        {
            returnValue = true;
        }
        freeMember (operResult);
    }
    return returnValue;
}

/**
 * @brief A function that gets a group of members and checks if the group satisfies the
 * closure requirement.
 * @param mem1,mem2 - 2 elements to be equaled
 * @param membersLen - an int telling the size of members.
 * @param oper - A pointer to an operation function for the group.
 * @param freeMember - A pointer to function that frees a specified group member.
 * @param compare - A pointer to a comparator for the group.
 * @return A boolean value telling whether the given group satisfies the closure requirement.
 */
static bool checkClosure(GroupMemberP mem1, GroupMemberP mem2, GroupMemberP (*oper)(GroupMemberP, GroupMemberP),
                         int (*compare)(GroupMemberP, GroupMemberP), int membersLen, GroupMemberP members[],
                         void (*freeMember)(GroupMemberP))
{
    bool returnValue = false;
    GroupMemberP operResult = oper(mem1, mem2);
    for (int i = 0; i < membersLen; i++) {
        if (compare( operResult, members[i]) == 0)
        {
            returnValue = true;
            break;
        }
    }
    freeMember (operResult);
    return returnValue;
}

/**
* @brief A function that gets a group of members and checks if the group satisfies the
* commutativity requirement.
* @param members - An array holding all the members in the group.
* @param membersLen - an int telling the size of members.
* @param oper - A pointer to an operation function for the group.
* @param freeMember - A pointer to function that frees a specified group member.
* @param compare - A pointer to a comparator for the group.
* @return A boolean value telling whether the given group satisfies the commutativity requirement.
*/
static bool checkComotative(GroupMemberP mem1, GroupMemberP mem2, GroupMemberP (*oper)(GroupMemberP, GroupMemberP),
                            int (*compare)(GroupMemberP, GroupMemberP),  void (*freeMember)(GroupMemberP))
{
    bool returnValue = false;
    GroupMemberP operResult1, operResult2;
    operResult1 = oper(mem1, mem2);
    operResult2 = oper(mem2, mem1);
    if (compare(operResult1, operResult2) == 0)
    {
        returnValue = true;
    }
    freeMember (operResult1);
    freeMember (operResult2);
    return returnValue;
}

/**
 * @brief A function that gets a group of members and checks if the group satisfies the
 * associativity requirement.
 * @param members - An array holding all the members in the group.
 * @param membersLen - an int telling the size of members.
 * @param oper - A pointer to an operation function for the group.
 * @param freeMember - A pointer to function that frees a specified group member.
 * @param compare - A pointer to a comparator for the group.
 * @return A boolean value telling whether the given group satisfies the associativity requirement.
 */
static bool checkAsotsyative(GroupMemberP mem1, GroupMemberP mem2, GroupMemberP mem3,
                  GroupMemberP (*oper)(GroupMemberP, GroupMemberP), int (*compare)(GroupMemberP, GroupMemberP),
        void (*freeMember)(GroupMemberP))
{
    bool returnValue = false;
    GroupMemberP leftFirst = oper(mem1, mem2);
    GroupMemberP rightFirst = oper(mem2, mem3);
    GroupMemberP leftSide = oper(leftFirst, mem3);
    GroupMemberP rightSide = oper(mem1, rightFirst);

    if (compare(leftSide, rightSide ) == 0)
    {
        returnValue = true;
    }
    freeMember (leftFirst);
    freeMember (rightFirst);
    freeMember(leftSide);
    freeMember(rightSide);
    return returnValue;
}

#endif // GENGROUP_H

