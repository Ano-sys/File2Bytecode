#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/**
*
* Creates a filepointer by the input char* and returns its content as char* 
* return needs to be freed later
*
*/
char *read_file(char *filename){
    // open file as read by name and create filepointer
    FILE *fp = fopen(filename, "r");
    
    // error check if fopen returns NULL which is an error
    if(fp == NULL){
        perror("Failed to open file!");
        return NULL;
    }

    // variable to hold current size of read data
    size_t contentSize = 1;
    // allocate a char* with initial length of 1 byte
    char *content = (char*) malloc(sizeof(char) * contentSize);

    // error check malloc
    if(content == NULL) {
        perror("Failed to allocate memory for content!");
        fclose(fp);
        return NULL;
    }

    // set the one byte in content to '\0'
    content[0] = '\0';

    // create character variable to get byte by byte from file
    char c;
    // get byte by byte from file until the end is met
    while ((c = fgetc(fp)) != EOF) {
        contentSize++;
        // realloc content with incremented contentSize to make space for the read byte in c
        content = (char*) realloc(content, contentSize);
        // error check realloc
        if(content == NULL) {
            perror("Failed to reallocate memory for content");
            fclose(fp);
            return NULL;
        }
        // hang the character at the end of the char* 
        content[contentSize-2] = c;
        // readd the '\0' at the end to terminate the char*
        content[contentSize-1] = '\0';
    }

    // close the filepointer to give it free
    fclose(fp);
    // return the read content
    return content;
}

int main(int argc, char **argv){
    // if the commandline args are less than 2 -> exit, the programm nee ds a file specified to read from, more arguments are unneccessary but aren't forbidden either, they are ignored 
    if(argc < 2){
        perror("Usage: file2bytecode.exe [filename]");
        exit(1);
    }

    // initial declaration of the "string" in which the content will be stored
    char *file_content;
    // set file_content to the return of read_file with argument 1 (filename) and simultaneously error check
    if((file_content = read_file(argv[1])) == NULL){
        perror("File could not be read, function terminated with errors!");
        exit(1);
    }

    // make space on the heap for output
    char *output = (char*) malloc(sizeof(char) * 1);
    if(!output){
        perror("Failed to allocate memory for output!");
        exit(1);
    }
    output[0] = '\0';

    // loop to convert readable text to their byte format
    for(int i = 0; i < strlen(file_content); i++){
        // if the loop is at the end of the file content only reallocate in the size of 5 more bytes
        if(i == strlen(file_content) - 1){
            // "0xAA\0"
            output = (char*)realloc(output, strlen(output) + 5);
            sprintf(output + i * 6, "0x%02X", file_content[i]);
        }
        else{
            // realloc 6 extra bytes to make space for a formated write "0xAA, ";
            output = (char*)realloc(output, strlen(output) + 6);
            sprintf(output + i * 6, "0x%02X, ", file_content[i]);
        }
    }

    // print the converted input with array brackets
    printf("Output:\n{ %s }", output);
    // free the allocated memory
    free(output);
    free(file_content);
    return 0;
}