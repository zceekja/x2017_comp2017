#ifndef VM_X2017_H
#define VM_X2017_H

#include "objdump_x2017.h"

#define BYTE unsigned char

struct x2017{
/*

    **************** The Architecture *****************

    Register                        Ram
    [Genral purpose ] 0         [  index first     ] 0 
    [Genral purpose ] 1         [ instruction size ] 1
    [Genral purpose ] 2         [  personel use    ] 2
    [Genral purpose ] 3         [  personel use    ] 3
    [Stack pointer  ] 4     SP->[ Return address   ] 4     
    [No. symbol     ] 5         [ Symbol A         ] 
    [collection 2   ] 6         [ Symbol B         ]          
    [Program counter] 7         [ ...              ] 
                                [ last symbol      ]
                                [ number of symbol ]
                                [                  ]
                                [                  ] 
                                [                  ] 254
                                [                  ] 255

    ****************************************************
   
*/                           
    BYTE reg[8] ;
    BYTE ram[256] ;
};
typedef struct x2017 VM_x2017;


/*
-> parameter: size of current stack
-> do: If stack is full, exit program with status 1 and print error message to stderr
-> return: nothing
*/
void is_stack_full(int v);

#endif