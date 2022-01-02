#include "objdump_x2017.h"

/*
Main for milestone submission
-> print program assembly code to stdout
*/
int main(int argc, char** argv){

   if(argc <2){
       fprintf(stderr,"Missing argument.\n");
       return 1;
   }
    Asm program[256];
    Asm* assembly_ptr_end = & program[255];
    Asm* assembly_ptr_current;
    assembly_ptr_current = parser(program,assembly_ptr_end , argv);
    int count= assembly_ptr_end - assembly_ptr_current;
    print_code(assembly_ptr_current,count);
    return 0;
}
