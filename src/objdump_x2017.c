#include "objdump_x2017.h"

/*
const char* getCode(CODE c)
-> parameter : enum Code
-> return : code in string format
*/
const char* getCode(CODE c) {
   switch (c) 
   {
      case FUNC: return "FUNC LABEL";
      case MOV: return "MOV";
      case CAL: return "CAL";
      case RET: return "RET";
      case REF: return "REF";
      case ADD: return "ADD";
      case PRINT: return "PRINT";
      case NOT: return "NOT";
      case EQU: return "EQU";
      case REG: return "REG";
      case STK: return "STK";
      case VAL: return "VAL";
      case PTR: return "PTR";
   }
   return "";
   
}

/*
void add_binary_string(int n, char* string_address)
 -> parameter : hex value, address of buffer
 -> do: parse hex value to binary string and put in buffer
 -> return : nothing
*/
void add_binary_string(int n, char* string_address){
    
    int value, index;

    for (index = 7; index >= 0; index--)
    {
        value = n >> index;
        if (value & 1){
          *string_address = '1'; 
        }
        else{
           *string_address = '0';
        }   
        string_address++;
    }

    return ;
}

/*
 string_binary_to_integer(char* str, int n)
 -> parameter : binary string, number of bits
 -> return : value in integer
*/
int string_binary_to_integer(char* str, int number_of_bits){
    int ret = 0;
    
    while(number_of_bits){

        if(*str == '1'){
            
            ret+= 1<<(number_of_bits-1);
        }
        str++;
        number_of_bits--;
    }
    return ret;
}

/*
get_collection(char* ptr)
-> parameter: address of code
-> return Collection of code (contains code, value, total number of bits)
*/
Collection get_collection(char* ptr){
    Collection ret;
    ptr-=2;
    
    int c =  string_binary_to_integer(ptr, 2);
    if(c == 0){
        // 00 - value: 1 byte long
        ptr -=8;
        ret.value=string_binary_to_integer(ptr, 8);
        ret.total_bit =10;
    } 
    else if(c == 1){
        // 01 - register address: 3 bits long
        ptr -=3;
        ret.value=string_binary_to_integer(ptr, 3);
        ret.total_bit =5;
    }
    else if(c == 2){
        // 10 - stack symbol: 5 bits long.
        ptr -=5;
        ret.value=string_binary_to_integer(ptr, 5);
        ret.total_bit =7;
    }
    else if(c == 3){
        // 11 - pointer valued: 5 bits long
        ptr -=5;
        ret.value=string_binary_to_integer(ptr, 5);
        ret.total_bit =7;
    }
    ret.collection = c;
    
    return ret;
}

/*
void print_code(Asm* ptr, int offset)
-> parameter : pointer to the first index of instruction, number of instruction
-> do: print assembly code (for milestone)
-> return : nothing
*/
void print_code(Asm* ptr, int offset){
    for (int i = 0; i < offset; i++){
        if((*ptr).code == FUNC){
            printf("%s",getCode((*ptr).code));
            printf(" %d",(*ptr).value);
        }
        else if((*ptr).code == MOV ||
        (*ptr).code == CAL ||
        (*ptr).code == REF ||
        (*ptr).code == ADD ||
        (*ptr).code == PRINT ||
        (*ptr).code == NOT ||
        (*ptr).code == EQU ){
            printf("\n    %s",getCode((*ptr).code));//d
        }
        else if ((*ptr).code == RET){
            printf("\n    %s",getCode((*ptr).code));  
            if((*(ptr+1)).code == MOV ||
            (*(ptr+1)).code ==CAL ||
            (*(ptr+1)).code ==REF ||
            (*(ptr+1)).code ==ADD ||
            (*(ptr+1)).code ==PRINT ||
            (*(ptr+1)).code ==NOT ||
            (*(ptr+1)).code ==EQU){
                ;
            }
            else{
                printf("\n");
            }
        }
        else {
            printf(" %s",getCode((*ptr).code));
            if((*ptr).code == VAL){
                printf(" %d",(*ptr).value);
            }
            else if((*ptr).code == REG){
                printf(" %d",(*ptr).value);
            }
            else if((*ptr).code == STK || (*ptr).code == PTR){
                if ((*ptr).value < 27){
                    printf(" %c",(*ptr).value + 'A');
                }
                else{
                    printf(" %c",(*ptr).value - 27 + 'a');
                }
            }  
        } 
        ptr++;
    }

}

/*
Binary parse Parser 
 -> parameter: addrss of Instructions array to store code, command-line argument
 -> do: parse instructions in file from command-line argument into instruction array
 -> return address of first index ofinstruction
 */
 Asm*  parser(Asm assembly[], Asm* assembly_ptr , char** argv){
 
    unsigned char buffer[1];
    char binary_string[33] = {'\0'}; 
    //char* string_to_print = &binary_string[0];
    char* current_ptr = &binary_string[32];
    char* instruction_ptr;
    FILE *ptr;

    int count_asm =0;
    Asm current;


    ptr = fopen(argv[1],"rb"); 
    if(!ptr){
        fprintf(stderr, "No file.\n");
        exit(1);
    }

    fseek(ptr, 0L, SEEK_END);
    int size_of_file = ftell(ptr);
    // if program not yet get n_of_instruction 0 else 1
    int check1 = 0;
    int number_of_instructions = 0;
    int index = -1;
    int ptr_offset = 0;
    int opercode = 0;
    while(1){
        
        while (ptr_offset <24 ){
            if(size_of_file){
                fseek(ptr,index--,SEEK_END);
                if (fread(buffer,sizeof(buffer),1,ptr)!=0){
                    current_ptr -=8;
                    ptr_offset += 8;
                    add_binary_string(buffer[0], current_ptr);
                    size_of_file--;
                }
            }
            else{
                break;
            }
        }  

        if (check1 == 0){
            instruction_ptr = &binary_string[27];
            number_of_instructions = string_binary_to_integer(instruction_ptr, 5);
            ptr_offset -=  5;
            current_ptr += 5;
            for(int i = 26; i>=0; i--){
                binary_string[i+5] = binary_string[i];
            }
            check1 =1;
        }
        else if(number_of_instructions){
            
            instruction_ptr = &binary_string[29];
            opercode = string_binary_to_integer(instruction_ptr, 3);

            if (opercode == 0){
                // 000 - [MOV A B]
            
                Collection collection1 = get_collection(&binary_string[32 - 3]);       
                Collection collection2 = get_collection(&binary_string[32 - 3 - collection1.total_bit]);
    
                if(collection2.collection == 0){
                    current.code = VAL;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection2.collection ==1){
                    current.code = REG;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection2.collection== 2){
                    current.code = STK;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection2.collection ==3){
                    current.code = PTR;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                if(collection1.collection == 0){
                    current.code = VAL;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection1.collection ==1){
                    current.code = REG;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection1.collection== 2){
                    current.code = STK;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;

                }
                else if(collection1.collection ==3){
                    current.code = PTR;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                current.code = MOV;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                ptr_offset -= ( 3 + collection1.total_bit + collection2.total_bit);
                current_ptr += ( 3 + collection1.total_bit + collection2.total_bit); 

                for(int i = 31-3 - collection1.total_bit - collection2.total_bit; i>=0; i--){
                    binary_string[i+3+collection1.total_bit+collection2.total_bit] = binary_string[i];
                }
                number_of_instructions--;

            }
            else if(opercode == 1){
                // 001 - [CAL A]
                Collection collection1 = get_collection(&binary_string[32 - 3]);
                if(collection1.collection == 0){
                    current.code = VAL;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else {
                    fprintf(stderr,"error!, add operation only take value\n"); 
                    exit(1);
                }
                current.code = CAL;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                ptr_offset -= ( 3 + collection1.total_bit );
                current_ptr += ( 3 + collection1.total_bit ); 

                for(int i = 31-3 - collection1.total_bit; i>=0; i--){
                    binary_string[i+ 3 + collection1.total_bit] = binary_string[i];
                }
                number_of_instructions--;
            }
            else if(opercode == 2){
                // 010 - [RET]
                current.code = RET;
                current.value = 0;
                assembly_ptr--;

                *assembly_ptr = current;
                count_asm++;

                number_of_instructions--;
                ptr_offset -=3;
                current_ptr +=3;
                for(int i = 28; i>=0; i--){
                    binary_string[i+3] = binary_string[i];
                }
            }
            else if(opercode == 3){
                // 011 - [REF A B] 
                Collection collection1 = get_collection(&binary_string[32 - 3]);       
                Collection collection2 = get_collection(&binary_string[32 - 3 - collection1.total_bit]);
    
                if(collection2.collection == 0){
                    current.code = VAL;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection2.collection ==1){
                    current.code = REG;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection2.collection== 2){
                    current.code = STK;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection2.collection ==3){
                    current.code = PTR;
                    current.value = collection2.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                if(collection1.collection == 0){
                    current.code = VAL;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection1.collection ==1){
                    current.code = REG;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection1.collection== 2){
                    current.code = STK;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;

                }
                else if(collection1.collection ==3){
                    current.code = PTR;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                current.code = REF;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                ptr_offset -= ( 3 + collection1.total_bit + collection2.total_bit);
                current_ptr += ( 3 + collection1.total_bit + collection2.total_bit); 

                for(int i = 31-3 - collection1.total_bit - collection2.total_bit; i>=0; i--){
                    binary_string[i+3+collection1.total_bit+collection2.total_bit] = binary_string[i];
                }
                number_of_instructions--;
            }
            else if(opercode == 4){
                // 100 -[ADD AB]
                Collection collection1 = get_collection(&binary_string[32 - 3]);       
                Collection collection2 = get_collection(&binary_string[32 - 3 - collection1.total_bit]);
                if(collection1.collection != 1 || collection2.collection != 1) {
                    fprintf(stderr,"error!, add operation only except register\n");  
                    exit(1);
                }
                current.code = REG;
                current.value = collection2.value;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                current.code = REG;
                current.value = collection1.value;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                current.code = ADD;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;

                ptr_offset -= ( 3 + 2+ 3+2+3 );
                current_ptr += (3 + 2+ 3+2+3);
                for(int i = 31-13; i>=0; i--){
                    binary_string[i+13] = binary_string[i];
                }
                number_of_instructions--;

            }
            else if(opercode == 5){
                // 101 - [PRINT A]
                Collection collection1 = get_collection(&binary_string[32 - 3]);
                if(collection1.collection == 0){
                    current.code = VAL;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection1.collection ==1){
                    current.code = REG;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else if(collection1.collection== 2){
                    current.code = STK;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;

                }
                else if(collection1.collection ==3){
                    current.code = PTR;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                current.code = PRINT;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                ptr_offset -= ( 3 + collection1.total_bit );
                current_ptr += ( 3 + collection1.total_bit ); 

                for(int i = 31-3 - collection1.total_bit; i>=0; i--){
                    binary_string[i+ 3 + collection1.total_bit] = binary_string[i];
                }
                number_of_instructions--;

            }
            else if (opercode == 6){
                // 110 - [NOT A]
                Collection collection1 = get_collection(&binary_string[32 - 3]);
                if(collection1.collection == 1){
                    current.code = REG;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else {
                    fprintf( stderr,"error!, add operation only take Register address\n"); 
                    exit(1);
                }
                current.code = NOT;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                ptr_offset -= ( 3 + collection1.total_bit );
                current_ptr += ( 3 + collection1.total_bit ); 

                for(int i = 31-3 - collection1.total_bit; i>=0; i--){
                    binary_string[i+ 3 + collection1.total_bit] = binary_string[i];
                }
                number_of_instructions--;
            }
            else if (opercode == 7){
                // 111 - [EQU A]
                Collection collection1 = get_collection(&binary_string[32 - 3]);
                if(collection1.collection == 1){
                    current.code = REG;
                    current.value = collection1.value;
                    assembly_ptr--;
                    *assembly_ptr = current;
                    count_asm++;
                }
                else {
                    fprintf(stderr,"error!, add operation only take Register address\n"); 
                    exit(1);
                }
                current.code = EQU;
                assembly_ptr--;
                *assembly_ptr = current;
                count_asm++;
                ptr_offset -= ( 3 + collection1.total_bit );
                current_ptr += ( 3 + collection1.total_bit ); 

                for(int i = 31-3 - collection1.total_bit; i>=0; i--){
                    binary_string[i+ 3 + collection1.total_bit] = binary_string[i];
                }
                number_of_instructions--;
            }
        }
        if (number_of_instructions == 0 && check1 ==1){
            current.code = FUNC;
            current.value = string_binary_to_integer(&binary_string[32 - 3], 3);
            assembly_ptr--;
            *assembly_ptr = current;
            count_asm++;

            ptr_offset -=  3;
            current_ptr += 3;
            for(int i = 28; i>=0; i--){
                binary_string[i+3] = binary_string[i];
            }
            check1 =0;         
        }
        if(check1==0 && size_of_file == 0 && ptr_offset<8){
            break;
        }
    }
    int array32[32] ={35};
    int count_32 =0;
    int is_found = 0;

    for(int i = 255- count_asm ; i < 255 ; i++){
        if(assembly[i].code == FUNC){
            for(int i =0; i<count_32;i++){
                array32[i] = 35;
            }
            count_32 =0;
            
        }
        else if (assembly[i].code == STK || assembly[i].code == PTR){
            for(int j = 0; j <count_32; j++){
                if(array32[j] ==assembly[i].value ){
                    assembly[i].value = j;
                    is_found =1;
                    break;
                }
            }
            if(!is_found){
                array32[count_32] = assembly[i].value;  
                assembly[i].value = count_32;
                count_32++;
            }
            is_found =0;
        } 
    }
    return assembly_ptr;
}
