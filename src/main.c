#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>

#include "algorithms/maximal_matching/maximal_matching.h"
#include "algorithms/pitts/pitts.h"


////////////////////////////////
// ENSURE 64 BIT ARCHITECTURE //
////////////////////////////////
#if INTPTR_MAX == INT64_MAX
    // System is 64-bit
#elif INTPTR_MAX == INT32_MAX
    #error "This is a 32-bit system! 64-bit is required."
#else
    #error "Unknown architecture size."
#endif

//////////////////////////////////////////////
// Constants (only accessible by this file) //
//////////////////////////////////////////////
#define max_num_characters 100
#define maximal_matching_id 1 
#define pitts_id 100
#define incorrect_alg_id 0


//////////////////////
// Helper functions //
//////////////////////
char* ask(char* buffer, size_t buffer_size, char* message);
int get_algorithm_id(char buffer[], size_t buffer_size);
int prompt_for_alg();
bool file_exists(char *path);
void prompt_for_edges_filepath(char* filepath, size_t filepath_size);
long parseLongBase10(const char *str);
long prompt_num_nodes(void);

//////////
// MAIN //
//////////
int main(int argc, char *argv[]){

    // Get algorithm and edges filepath
    int algorithm_id;
    char edges_filepath[max_num_characters];
    long num_nodes;
    switch(argc){
        case 1:
            //Manual Entry Format
            algorithm_id = prompt_for_alg(); //Prompt for Algorithm
            prompt_for_edges_filepath(edges_filepath,sizeof(edges_filepath)); // prompt for edges filepath
            num_nodes = prompt_num_nodes();
            break;
        case 4:
            algorithm_id = get_algorithm_id(argv[1],sizeof(argv[1])); // get algorithm
            strcpy(edges_filepath, argv[2]);
            num_nodes = parseLongBase10(argv[3]);
            if(algorithm_id != incorrect_alg_id && file_exists(edges_filepath) && num_nodes > 0){
                break;
            }
            
        default:
            fprintf(stderr, "You are running the program incorrectly. Please run the program as: ./approxmvc alg_name edges_filepath number_of_nodes\n");
            fprintf(stderr, "or without arguments for manual entry \n");
            return EXIT_FAILURE;
    }

    //
    // Load Edges file into memory
    //
    //open file
    int fd = open(edges_filepath,O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error opening file");
        return EXIT_FAILURE;
    }
    //get file size
    struct stat file_status;
    if (fstat(fd, &file_status) == -1) {
        fprintf(stderr, "Error getting file stats");
        close(fd);
        return 1;
    }
    off_t file_size = file_status.st_size;
    if ((uint64_t)file_size > SIZE_MAX) { //check if conversion to size_t is possible (yes in 64 bit systems, no otherwise)
        fprintf(stderr, "File is too large to fit into memory address space\n");
        return EXIT_FAILURE;
    }
    size_t map_len = (size_t)file_size; //convert to size_t, needed for mmap call
    //  create a new mapping in the virtual address space of this calling process to access the edges file
    void* edges_ptr = mmap(NULL,map_len,PROT_READ,MAP_SHARED,fd,0);
    close(fd);// clean up file descriptor
    if(edges_ptr == MAP_FAILED){
        fprintf(stderr, "Could create a new mapping in virtual address space for edges file ");
        return EXIT_FAILURE;
    }

    //TEST
    // printf("---------\n");
    // printf("TEST\n");
    // printf("Number of nodes: %lu\n", num_nodes);
    // printf("Edges filepath: %s\n", edges_filepath);
    // printf("Algorithm ID: %d\n", algorithm_id);
    //// Read all edges
    // char* edges_txt_ptr = (char*) edges_ptr;
    // printf("%s",edges_txt_ptr);

    

    // Run appropriate algorithm
    switch(algorithm_id){
        case maximal_matching_id:
            maximal_matching(edges_ptr, num_nodes);
            break;
        case pitts_id:
            pitts(edges_ptr, num_nodes);
            break;
        default:
            break;
    }

    return 0;
}


/////////////////////////////
// ASK USER AND GET ANSWER //
/////////////////////////////

char* ask(char* buffer, size_t buffer_size, char* message){
    printf("%s",message);
    char* result = fgets(buffer, buffer_size, stdin); //Get user input
    //strip result of the ending newline
    if(result !=NULL){
        // Find \n and replace it with \0
        buffer[strcspn(buffer, "\r\n")] = 0;
    }
    return result;
}



/////////////////////////////////
/// GETTING ALGORITHM NAME/ID ///
/////////////////////////////////

// returns integer id of algorithm to be used. 
int get_algorithm_id(char buffer[], size_t buffer_size){
    if (strcmp(buffer, "maximal_matching") == 0){
        return maximal_matching_id;
    }else if(strcmp(buffer, "pitts") == 0){
        return pitts_id;
    }
    
    printf("Incorrect Algorithm name. Possible values are:\n");
    printf("maximal_matching\n");
    printf("pitts\n");
    return incorrect_alg_id; //incorrect / could not find
}


int prompt_for_alg(){
    int algorithm_id = incorrect_alg_id;
    char alg_name[max_num_characters+1];
    bool is_first_loop = true;
    while(algorithm_id == incorrect_alg_id){
        if(!is_first_loop){
            printf("\n");
        }
        ask(alg_name,sizeof(alg_name),"Enter The name of algorithm: ");
        algorithm_id = get_algorithm_id(alg_name,sizeof(alg_name));
        is_first_loop = false;
    }
    return algorithm_id;
}




////////////////////////////
// GETTING EDGES FILEPATH //
////////////////////////////

bool file_exists(char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        fclose(file);
        return true;
    }
    printf("File does not exist\n");
    return false;
}

void prompt_for_edges_filepath(char* filepath, size_t filepath_size){
    bool is_file_valid = false;
    while(!is_file_valid){
        ask(filepath,filepath_size,"Enter Edges Filepath: ");
        is_file_valid = file_exists(filepath);
    }
    return;
}


///////////////////////
// GETTING NUM NODES //
///////////////////////
long parseLongBase10(const char *str){
    if(str==NULL){
        fprintf(stderr, "parseLong: pointer to string to parse is NULL\n");
        return 0;
    }else if(*str == '\0'){
        fprintf(stderr, "parseLong: string to parse is NULL (empty)\n");
        exit(EXIT_FAILURE);
    }
    
    char* temp;
    char** endptr = &temp;
    long result = strtol(str,endptr,10);
    
    if(str==*endptr){
        fprintf(stderr,"parseLongBase10: string to parse has no digits at all\n");
        return 0;
    }else if(**endptr!='\0'){
        fprintf(stderr, "parseLongBase10: string to parse contains some non-digit characters starting with: %c\n", **endptr);
        return 0;
    }else if(errno == ERANGE){
        if(result == LONG_MIN){
            fprintf(stderr, "parseLongBase10: UNDERFLOW\n");
        }else if(errno == LONG_MAX){
           fprintf(stderr, "parseLongBase10: OVERFLOW\n");
        }
        return 0;
    }
    return result;
}

long prompt_num_nodes(void){
    char buffer[max_num_characters+1];
    long num_nodes = 0;
    while(num_nodes == 0){
        ask(buffer,sizeof(buffer),"Enter number of nodes (must be greater than zero): ");
        num_nodes = parseLongBase10(buffer);
    }
    return num_nodes;
}