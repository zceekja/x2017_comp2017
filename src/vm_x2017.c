#include "vm_x2017.h"


int main(int argc, char** argv){

    /*
    Parse binary program code to Programe code
    */
   if(argc <2){
       fprintf(stderr,"Missing argument.\n");
       return 1;
   }
    Asm program[256];
    Asm* assembly_ptr_end = & program[255];
    Asm* assembly_ptr_current;
    assembly_ptr_current = parser(program,assembly_ptr_end , argv);
    int count= assembly_ptr_end - assembly_ptr_current;
    int index_first_function = 255- count;
    /*
    Program Execution

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
   
    VM_x2017 vm = {0};
    vm.ram[0] = index_first_function;
    vm.ram[1] = count;

    for(vm.ram[2]= vm.ram[0]; vm.ram[2] < 255; vm.ram[2]++){
        if (program[vm.ram[2]].code ==FUNC){
            if(program[vm.ram[2]].value == 0){

                vm.reg[7] = vm.ram[2];
                break;
            }
        }
    }
    //set stack pointer to address 4 in RAM
    vm.reg[4] =4; 
    //push 0 to stack, so we know when to exit program.
    vm.ram[vm.reg[4]] = 0;

    //Execute Program
    while(1){
        switch(program[vm.reg[7]].code){
            // FUNC -> Function label
            case 0:
                    
                    vm.reg[7]++;
                    break;
                    
            // MOV -> Copies the value at some point B in memory to another point A in memory
            case 1:
                vm.reg[7]++;   
                vm.reg[7]++;
                if (program[vm.reg[7]].code == VAL){
                    vm.reg[6] = program[vm.reg[7]].value;
                } 
                else if (program[vm.reg[7]].code == REG) {
                    vm.reg[6]= vm.reg[program[vm.reg[7]].value];
                }
                else if (program[vm.reg[7]].code == STK) {
                    vm.reg[6]= vm.ram[vm.reg[4]+program[vm.reg[7]].value +1];
                    if (vm.reg[5] <= program[vm.reg[7]].value){
                        vm.reg[5]++;
                    }
                }
                
                else if (program[vm.reg[7]].code == PTR) {
                    vm.reg[6] = vm.ram[vm.ram[vm.reg[4]+program[vm.reg[7]].value + 1]];
                }

                if(program[vm.reg[7]-1].code == REG){
                    vm.reg[program[vm.reg[7]-1].value] = vm.reg[6];
                }
                else if (program[vm.reg[7]-1].code == STK){
                    is_stack_full(vm.reg[4]+program[vm.reg[7]-1].value+1);
                    vm.ram[vm.reg[4]+program[vm.reg[7]-1].value + 1] = vm.reg[6];
                    if (vm.reg[5] <= program[vm.reg[7]-1].value){
                        vm.reg[5]++;
                    }
                }

                else if (program[vm.reg[7]-1].code == PTR){  // probably dont need
                    vm.ram[vm.ram[vm.reg[4]+program[vm.reg[7]-1].value+1]] = vm.reg[6];
                }
                vm.reg[7]++;
                break;
            // CAL ->  Calls another function
            case 2:
                vm.reg[7]++;
                for(vm.ram[2]= vm.ram[0]; vm.ram[2]< vm.ram[0]+vm.ram[1]; vm.ram[2]++){
                    if (program[vm.ram[2]].code == FUNC){
                        if (program[vm.ram[2]].value == program[vm.reg[7]].value){
                            is_stack_full(vm.reg[4]+vm.reg[5]+2);
                            vm.ram[vm.reg[4]+vm.reg[5]+1] = vm.reg[5]; // push number of symbol in current stack frame to stack
                            vm.ram[vm.reg[4]+vm.reg[5]+2] = vm.reg[7]+1; //push return address to stack 
                            vm.reg[4]+= vm.reg[5]+2; // increment stack pointer
                            vm.reg[7] = vm.ram[2]; // set PC to call functioon
                            vm.reg[5] = 0; // reset number of symbol in register
                        }
                    }
                }
                break;
            // RET -> Terminates the current function
            case 3:
            
                if(vm.ram[vm.reg[4]] == 0){
                    //printf("Exit succesfully!\n");
                    return 0;
                }
                else{
                    // set pc to previous address
                    vm.reg[7] = vm.ram[vm.reg[4]]; //set PC to  return address
                    vm.reg[5] = vm.ram[vm.reg[4]-1]; // set number of symbol in register 
                    vm.reg[4] -= (vm.reg[5] +2); // decrement stack pointer...
                }
                break;
             // REF ->  Takes a stack symbol B and stores its corresponding stack address in A
            case 4:
 
                vm.reg[7]++;
                vm.reg[7]++;
                if(program[vm.reg[7]-1].code == REG){
                    if (program[vm.reg[7]].code == STK){
                        vm.reg[program[vm.reg[7]-1].value] = 1+vm.reg[4]+program[vm.reg[7]].value;
                    }
                    else if(program[vm.reg[7]].code == PTR) {
                        vm.reg[program[vm.reg[7]-1].value] = vm.ram[1+vm.reg[4]+program[vm.reg[7]].value];
                    }
                    if (vm.reg[5] <= program[vm.reg[7]].value){
                        vm.reg[5]++;
                    }
                }
                else if (program[vm.reg[7]-1].code == STK){
                    is_stack_full(vm.reg[4]+program[vm.reg[7]-1].value+1);
                    if (program[vm.reg[7]].code == STK){
                        vm.ram[vm.reg[4]+program[vm.reg[7]-1].value+1] =  vm.reg[4]+program[vm.reg[7]].value +1;
                    }
                    else if(program[vm.reg[7]].code == PTR) {
                        vm.ram[vm.reg[4]+program[vm.reg[7]-1].value+1] = vm.ram[ vm.reg[4]+program[vm.reg[7]].value +1];

                    }
                    if (vm.reg[5] <= program[vm.reg[7]-1].value){
                        vm.reg[5]++;
                    }
                    if (vm.reg[5] <= program[vm.reg[7]].value){
                        vm.reg[5]++;
                    }
                }
                else if (program[vm.reg[7]-1].code == PTR){
                    if (program[vm.reg[7]].code == STK){
                        vm.ram[vm.ram[vm.reg[4]+program[vm.reg[7]-1].value+1]] =  vm.reg[4]+program[vm.reg[7]].value +1;
                    }
                    else if(program[vm.reg[7]].code == PTR) {
                        vm.ram[vm.ram[vm.reg[4]+program[vm.reg[7]-1].value+1]] =  vm.ram[vm.reg[4]+program[vm.reg[7]].value +1];
                    }
                    if (vm.reg[5] <= program[vm.reg[7]-1].value){
                        vm.reg[5]++;
                    }
                    if (vm.reg[5] <= program[vm.reg[7]].value){
                        vm.reg[5]++;
                    }
                }
                vm.reg[7]++;

                break;
            // ADD -> Takes two register addresses and ADDs their values, storing the result in the first listed register.
            case 5:
                  
                vm.reg[7]++;   
                vm.reg[7]++;
                if (program[vm.reg[7]].code == VAL){
                    vm.reg[6] = program[vm.reg[7]].value;
                } 
                else if (program[vm.reg[7]].code == REG) {
                    vm.reg[6]= vm.reg[program[vm.reg[7]].value];
                }
                else if (program[vm.reg[7]].code == STK) {// probably dont need
                    
                    vm.reg[6]= vm.ram[vm.reg[4]+program[vm.reg[7]].value];
                    if (vm.reg[5] <= program[vm.reg[7]].value){
                        vm.reg[5]++;
                    }
                }
                else if (program[vm.reg[7]].code == PTR) {
                    vm.reg[6] = vm.ram[vm.reg[4]+program[vm.reg[7]].value];
                }

                if(program[vm.reg[7]-1].code == REG){
                    if(program[vm.reg[7]-1].value == 7){
                        vm.ram[3] = 0;
                        for(vm.ram[2]= vm.reg[7]+1; vm.ram[2]< vm.ram[0]+vm.ram[1]; vm.ram[2]++){
                            if(program[vm.ram[2]].code == MOV ||
                            program[vm.ram[2]].code == CAL || 
                            program[vm.ram[2]].code == RET ||
                            program[vm.ram[2]].code == REF ||
                            program[vm.ram[2]].code == ADD ||
                            program[vm.ram[2]].code == PRINT ||
                            program[vm.ram[2]].code == NOT ||
                            program[vm.ram[2]].code == EQU ){
                                vm.ram[3]++;
                            }
                            if(vm.ram[3] ==  vm.reg[6]){
                                vm.reg[7] = vm.ram[2];
                                break;
                            }
                        }
                        for(vm.ram[2]= vm.reg[7]+1; vm.ram[2]< vm.ram[0]+vm.ram[1]; vm.ram[2]++){
                            if(program[vm.ram[2]].code == MOV ||
                            program[vm.ram[2]].code == CAL || 
                            program[vm.ram[2]].code == RET ||
                            program[vm.ram[2]].code == REF ||
                            program[vm.ram[2]].code == ADD ||
                            program[vm.ram[2]].code == PRINT ||
                            program[vm.ram[2]].code == NOT ||
                            program[vm.ram[2]].code == EQU ){            
                                vm.reg[7] = vm.ram[2]-1;
                                break;
                            }
                        }
                    }
                    else{
                        vm.reg[program[vm.reg[7]-1].value] += vm.reg[6];
                    }
                }
                else if (program[vm.reg[7]-1].code == STK){
                    is_stack_full(vm.reg[4]+program[vm.reg[7]-1].value+1);
                    vm.ram[vm.reg[4]+program[vm.reg[7]-1].value+1] += vm.reg[6];
                    if (vm.reg[5] < program[vm.reg[7]-1].value){
                        vm.reg[5] = program[vm.reg[7]-1].value;
                    }
                }
                else if (program[vm.reg[7]-1].code == PTR){  // probably dont needd
                    vm.ram[vm.reg[4]+program[vm.reg[7]-1].value+1] += vm.reg[6];
                }
                vm.reg[7]++;

                break;
            // PRINT -> Takes any address type and prints the contents to a new line of standard output
            case 6:
                    
                vm.reg[7]++;
                if(program[vm.reg[7]].code == VAL){
                   printf("%d\n",program[vm.reg[7]].value);
                }
                else if(program[vm.reg[7]].code == REG){
                   printf("%d\n",vm.reg[program[vm.reg[7]].value]); 
                }
                else if (program[vm.reg[7]].code == STK){
                   printf("%d\n",vm.ram[1+vm.reg[4]+program[vm.reg[7]].value]);
                }
                else if (program[vm.reg[7]].code == PTR){
                  printf("%d\n",vm.ram[vm.ram[1+vm.reg[4]+program[vm.reg[7]].value]]);
                }
                vm.reg[7]++;

                break;
            // NOT -> takes a register address and performs a bitwise not operation on the value at that address. 
            case 7:
           
                vm.reg[7]++;
                if(program[vm.reg[7]].code == REG){
                   vm.reg[program[vm.reg[7]].value] = ~vm.reg[program[vm.reg[7]].value];
                }
                vm.reg[7]++;
                break;
            // EQU -> Takes a register address and tests if it equals zero. 
            //     -> The value in the register will be set to 1 if it is 0, or 0 if it is not.
            case 8:
                    
                vm.reg[7]++;
                if(program[vm.reg[7]].code == REG){
                    if( vm.reg[program[vm.reg[7]].value] == 0){
                        vm.reg[program[vm.reg[7]].value] = 1;
                    }
                    else{
                        vm.reg[program[vm.reg[7]].value] = 0;
                    }
                }
                vm.reg[7]++;
                break;
            default:
                break;
        }
    }
     return 0;
}

/*
-> parameter: size of current stack
-> do: If stack is full, exit program with status 1 and print error message to stderr
-> return: nothing
*/
void is_stack_full(int v){
    if (v >= 255){
        fprintf(stderr,"STACK OVERFLOW.\n");
        exit(1);
    }
}