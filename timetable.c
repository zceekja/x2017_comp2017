#include <stdio.h>
#include <stdlib.h>

/*  function to check number of double colon
    - if more than 2 ajacent double colon return -1
    - if number of double colon == 2 return 0
    - else return -1
*/
int check_double_colon(char* str){
    int count = 0;
    char current;
    char previous;
    char previous_previous;

    current = *str;
    while (*str++){
        previous_previous = previous;
        previous = current;
        current = *str;

        if(previous == ':' && current == ':'){
            count++;
        }
        if (previous_previous == ':' && previous == ':' && current == ':'){
            return -1;
        }
    }
    if (count == 2){
        return 0;
    }
    else {
        return -1;
    }
}

/* function to check for match source and destination
    - if match return 0
    - else return -1
*/
int check_match(char* str, char* source, char* destination){

    while(*source == *str){
        if(*source == '\0')
            break;
        str++;
        source++;
    }
    if(*source == '\0' && *str == ':'){
        str++;
        if(*str != ':')
            return -1;
        else
            str++;
    }
    else 
        return -1;
    while(*destination == *str){
        if(*destination == '\0')
            break;
        str++;
        destination++;
    }
    if(*destination == '\0' && *str == ':'){
        str++;
        if(*str != ':'){
            return -1;
        }
        else 
            return 0;
        
    }
    else{

        return -1;
    }

}


/* funtion to parse from string format to int
    - return time in second
*/
int s_to_i(char* str){
    int ret = 0;
    int hour = 0;
    if (*str >= '0' && *str <= '2'){
        hour = (*str -'0');
        ret +=  (*str - '0')*60*60*10;
    }
    else
        return  -1;
    str++;
    if (hour == 2) {
        if (*str >= '0' && *str <= '3')
            ret +=  (*str - '0')*60*60;
        else
            return -1;
    }
    else {
        if (*str >= '0' && *str <= '9')
            ret +=  (*str - '0')*60*60;
        else
            return -1;
    }
    str++;
    if (*str != ':')
        return -1;
    str++;
    if (*str >= '0' && *str <= '5')
        ret +=  (*str - '0')*60*10;
    else
        return -1;
    str++;
    if (*str >= '0' && *str <= '9')
        ret +=  (*str - '0')*60;
    else
        return -1;
    str++;
    if (*str != ':')
        return -1;
    str++;
    if (*str >= '0' && *str <= '5')
        ret +=  (*str - '0')*10;
    else
        return -1;
    str++;
    if (*str >= '0' && *str <= '9')
        ret +=  (*str - '0');
    else
        return -1;

    return ret;
}


/*
    Funtion to get optimal time.
    - return time in second
*/
int get_time(char* str , int time , int answer){

    int count =0;
    int ret_old = answer;
    while(count < 4) {
        if (*str == ':'){
            count++;
        }
        str++;
        ///
    }
    int ret_new = s_to_i(str);

    if(ret_new == -1)
        return ret_old;

    else if(answer == -1){
        return ret_new;
    }

    else if (answer - time > 0){
        if(ret_new - time >= 0){
            if (ret_new - time < answer - time)
                return ret_new;
             else
                return ret_old;
        }
        else 
            return ret_old;
    }
    else{
        if (ret_new - time > 0)

            return ret_new;
        else{
            if(ret_new - time < answer -time)
                return ret_new;
            
            else
                return ret_old;
        }
    }
}

//check for valid and match
int valid_check(char* data,char* source,char*  destination){
    if (check_double_colon(data) != 0){
        return -1;
    }
    if (check_match( data, source, destination) == 0){
        return 0;
    }
	return -1;
}
int valid_check2(char* data){
    if (check_double_colon(data) != 0){
        return -1;
    }
    int count =0;
    while(count < 4) {
         
        if (*data == ':'){
            count++;
        }
        data++;
        ///
    }
    int check = s_to_i(data);
    if(check != -1){
        return 0;
    }
    else{
        return -1;
    }

}

//check for valid length of entry
int overload_check(char* entry , int is_overload){
    for (int i = 0; i <4096;i++){
        if( entry[i] == '\0'){
            //printf("newline found!\n");
                return -1;
        }
        if( entry[i] == '\n'){
            if(is_overload == 0)
                return 0;
            else 
                return -2;
        }
    }
    return 0;
}


int main(int argc, char **argv) {
    //check for valid argument
    if(argc < 4){
        printf("Not enough arguments\n");
        exit(0);
    }

    char* source = argv[1];
    char* destination = argv[2];
    char* time = argv[3];

    char entry[4096];
    char* entry_ptr =  entry;
    int answer = -1;

    int is_overload = 0;
    int data_counter =0;
    while( NULL != fgets(entry, 4096, stdin)) {

        //encounters an entry greater than the maximum string length
        //skip this entry.
        is_overload = overload_check(entry, is_overload);
        if(is_overload == -2){
            is_overload = 0;
            continue;
        }
        else if( is_overload == -1){
            continue;
        }


    	if(valid_check(entry_ptr, source, destination) == 0){
            answer = get_time(entry_ptr, s_to_i(time), answer);
    	}
        if(!valid_check2(entry_ptr)){
            data_counter++;
        }
    }
    if(!data_counter){
        fprintf(stderr,"No data to process.\n");
        return -1;
    }
    //Print output
    if(answer == -1)
        printf("No suitable trains can be found\n");
    else{
        printf("The next train to %s from %s departs at ",destination,source);
        if(answer/60/60 <10)
            printf("0");
        printf("%d:", answer/60/60);
        if((answer - (answer/60/60)*60*60)/60 <10)
            printf("0");
        printf("%d:",(answer - (answer/60/60)*60*60)/60);
        if((answer - answer/60/60*60*60 - (answer - (answer/60/60)*60*60)/60*60) <10)
            printf("0");
        printf("%d\n",(answer - answer/60/60*60*60 - (answer - (answer/60/60)*60*60)/60*60));
    }
    return 0;
}





