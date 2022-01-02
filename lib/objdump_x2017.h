#ifndef OBJDUMP_H
#define OBJDUMP_H

#include <stdio.h>
#include <stdlib.h>


typedef enum {
    FUNC,    // FUNCTION LABEL
    MOV,     // MOV OPERAMD
    CAL,     // CAL OPERAND
    RET,     // RET OPERAND
    REF,     // REF OPERAND
    ADD,     // ADD OPERAND
    PRINT,   // PRINT OPERAND
    NOT,     // NOT OPERAND
    EQU,     // EQU OPERAND
    REG,     // REG OPERAND
    STK,     // STK OPERAND
    VAL,     // VAL OPERAND
    PTR      // PTR OPERAND
} CODE;

/*
Store information of collection.
*/
struct collection {
    int collection; // Type of collection eg. val,ret,stk,ptr
    int value;      // Value of collection
    int total_bit;  // Total number of bit to store collection and value
};
typedef struct collection Collection;

/*
Store information of assembly code and its value.
*/
struct asm_item{
    CODE code;      // store type of assembly code
    int value;      // Store value or symbol of assembly code
};
typedef struct asm_item Asm;


//////////////////////////////////////////
//                                      //
//  FUNNCTION DECLARATIONS              //
//                                      //
//////////////////////////////////////////

/*
Binary parse Parser 
 -> parameter: addrss of Instructions array to store code, command-line argument
 -> do: parse instructions in file from command-line argument into instruction array
 -> return address of first index ofinstruction
 */
Asm*  parser(Asm assembly[], Asm* assembly_ptr , char** argv);

/*
get_collection(char* ptr)
-> parameter: address of code
-> return Collection of code (contains code, value, total number of bits)
*/
Collection get_collection(char* ptr);

/*
 string_binary_to_integer(char* str, int n)
 -> parameter : binary string, number of bits
 -> return : value in integer
*/
int string_binary_to_integer(char* str, int n);

/*
void add_binary_string(int n, char* string_address)
 -> parameter : hex value, address of buffer
 -> do: parse hex value to binary string and put in buffer
 -> return : nothing
*/
void add_binary_string(int n, char* string_address);

/*
const char* getCode(CODE c)
-> parameter : enum Code
-> return : code in string format
*/
const char* getCode(CODE c) ;

/*
void print_code(Asm* ptr, int offset)
-> parameter : pointer to the first index of instruction, number of instruction
-> do: print assembly code (for milestone)
-> return : nothing
*/
void print_code(Asm* ptr, int offset);


#endif