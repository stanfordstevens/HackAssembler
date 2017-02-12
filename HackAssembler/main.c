//
//  main.c
//  HackAssembler
//
//  Created by Stanford Stevens on 2/7/17.
//  Copyright © 2017 Stanford Stevens. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, const char * argv[]) {
    char filename[200];
    printf("Enter filename> ");
    scanf("%s", filename);
    
    FILE *inputFile = fopen(filename, "r");
    
    if (inputFile == NULL) {
        fprintf(stderr, "Cant open input file\n");
        return 1;
    }
    
    FILE *outputFile = fopen("/Users/FireCrotch/Desktop/ouput.hack", "w");
    
    if (outputFile == NULL) {
        fprintf(stderr, "Cant open ouput file\n");
        return 1;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        if ((line[0] == '/' && line[1] == '/') || isspace(line[0])) {
            continue;
        }
        
        if (line[0] == '@') {
            int value = atoi(strtok(line, "@"));
            fprintf(outputFile, "0");
            
            int i, bit;
            for (i = 14; i >= 0; i--) {
                bit = value >> i;
                
                if (bit & 1) {
                    fprintf(outputFile, "1");
                } else {
                    fprintf(outputFile, "0");
                }
            }
            
            fprintf(outputFile, "\n");
            continue;
        }
        
        char *destination = NULL;
        char *component = NULL;
        char *jump = NULL;
        
        if (strchr(line, '=')) {
            destination = strtok(line, "=");
            component = strtok(strtok(NULL, "="), ";");
            jump = strtok(NULL, ";");
        } else if (strchr(line, ';')) {
            component = strtok(line, ";");
            jump = strtok(NULL, ";");
        } else {
            component = line;
        }
        
        if (destination == NULL) {
            destination = "000";
        } else if (strncmp(destination, "AMD", 3) == 0) {
            destination = "111";
        } else if (strncmp(destination, "AD", 2) == 0) {
            destination = "110";
        } else if (strncmp(destination, "AM", 2) == 0) {
            destination = "101";
        } else if (strncmp(destination, "MD", 2) == 0) {
            destination = "011";
        } else if (strncmp(destination, "M", 1) == 0) {
            destination = "001";
        } else if (strncmp(destination, "D", 1) == 0) {
            destination = "010";
        } else if (strncmp(destination, "A", 1) == 0) {
            destination = "100";
        }
        
        if (component == NULL) {
            printf("'component' should not be NULL!");
            return 1;
        } else if (strncmp(component, "0", 1) == 0) {
            component = "0101010";
        } else if (strncmp(component, "1", 1) == 0) {
            component = "0111111";
        } else if (strncmp(component, "-1", 2) == 0) {
            component = "0111010";
        } else if (strncmp(component, "!D", 2) == 0) {
            component = "0001101";
        } else if (strncmp(component, "!A", 2) == 0) {
            component = "0110001";
        } else if (strncmp(component, "-D", 2) == 0) {
            component = "0001101";
        } else if (strncmp(component, "-A", 2) == 0) {
            component = "0001101";
        } else if (strncmp(component, "D+1", 3) == 0 || strncmp(component, "1+D", 3) == 0) {
            component = "0011111";
        } else if (strncmp(component, "A+1", 3) == 0 || strncmp(component, "1+A", 3) == 0) {
            component = "0110111";
        } else if (strncmp(component, "D-1", 3) == 0) {
            component = "0001110";
        } else if (strncmp(component, "A-1", 3) == 0) {
            component = "0110010";
        } else if (strncmp(component, "D+A", 3) == 0 || strncmp(component, "A+D", 3) == 0) {
            component = "0000010";
        } else if (strncmp(component, "D-A", 3) == 0) {
            component = "0010011";
        } else if (strncmp(component, "A-D", 3) == 0) {
            component = "0000111";
        } else if (strncmp(component, "D&A", 3) == 0 || strncmp(component, "A&D", 3) == 0) {
            component = "0000000";
        } else if (strncmp(component, "D|A", 3) == 0 || strncmp(component, "A|D", 3) == 0) {
            component = "0010101";
        } else if (strncmp(component, "!M", 2) == 0) {
            component = "1110001";
        } else if (strncmp(component, "-M", 2) == 0) {
            component = "1110011";
        } else if (strncmp(component, "M+1", 3) == 0 || strncmp(component, "1+M", 3) == 0) {
            component = "1110111";
        } else if (strncmp(component, "M-1", 3) == 0) {
            component = "1110010";
        } else if (strncmp(component, "D+M", 3) == 0 || strncmp(component, "M+D", 3) == 0) {
            component = "1000010";
        } else if (strncmp(component, "D-M", 3) == 0) {
            component = "1010011";
        } else if (strncmp(component, "M-D", 3) == 0) {
            component = "1000111";
        } else if (strncmp(component, "D&M", 3) == 0 || strncmp(component, "M&D", 3) == 0) {
            component = "1000000";
        } else if (strncmp(component, "D|M", 3) == 0 || strncmp(component, "M|D", 3) == 0) {
            component = "1010101";
        } else if (strncmp(component, "D", 1) == 0) {
            component = "0001100";
        } else if (strncmp(component, "A", 1) == 0) {
            component = "0110000";
        } else if (strncmp(component, "M", 1) == 0) {
            component = "1110000";
        }
        
        if (jump == NULL) {
            jump = "000";
        } else if (strncmp(jump, "JGT", 3) == 0) {
            jump = "001";
        } else if (strncmp(jump, "JEQ", 3) == 0) {
            jump = "010";
        } else if (strncmp(jump, "JGE", 3) == 0) {
            jump = "011";
        } else if (strncmp(jump, "JLT", 3) == 0) {
            jump = "100";
        } else if (strncmp(jump, "JNE", 3) == 0) {
            jump = "101";
        } else if (strncmp(jump, "JLE", 3) == 0) {
            jump = "110";
        } else if (strncmp(jump, "JMP", 3) == 0) {
            jump = "111";
        }
        
        fprintf(outputFile, "111%s%s%s\n", component, destination, jump);
    }
    
    fclose(outputFile);
    
    return 0;
}
