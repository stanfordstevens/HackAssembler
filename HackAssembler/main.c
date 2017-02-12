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
        if (line[0] == '/' && line[1] == '/') {
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
        
        char *destination;
        char *component;
        char *jump;
        
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
        } else if (strcmp(destination, "M")) {
            destination = "001";
        } else if (strcmp(destination, "D")) {
            destination = "010";
        } else if (strcmp(destination, "A")) {
            destination = "100";
        } else if (strcmp(destination, "AM")) {
            destination = "101";
        } else if (strcmp(destination, "AD")) {
            destination = "110";
        } else if (strcmp(destination, "AMD")) {
            destination = "111";
        }
        
        if (component == NULL) {
            printf("'component' should not be NULL!");
            return 1;
        } else if (strcmp(component, "0")) {
            component = "0101010";
        } else if (strcmp(component, "1")) {
            component = "0111111";
        } else if (strcmp(component, "-1")) {
            component = "0111010";
        } else if (strcmp(component, "D")) {
            component = "0001100";
        } else if (strcmp(component, "A")) {
            component = "0110000";
        } else if (strcmp(component, "!D")) {
            component = "0001101";
        } else if (strcmp(component, "!A")) {
            component = "0110001";
        } else if (strcmp(component, "-D")) {
            component = "0001101";
        } else if (strcmp(component, "-A")) {
            component = "0001101";
        } else if (strcmp(component, "D+1") || strcmp(component, "1+D")) {
            component = "0011111";
        } else if (strcmp(component, "A+1") || strcmp(component, "1+A")) {
            component = "0110111";
        } else if (strcmp(component, "D-1")) {
            component = "0001110";
        } else if (strcmp(component, "A-1")) {
            component = "0110010";
        } else if (strcmp(component, "D+A") || strcmp(component, "A+D")) {
            component = "0000010";
        } else if (strcmp(component, "D-A")) {
            component = "0010011";
        } else if (strcmp(component, "A-D")) {
            component = "0000111";
        } else if (strcmp(component, "D&A") || strcmp(component, "A&D")) {
            component = "0000000";
        } else if (strcmp(component, "D|A") || strcmp(component, "A|D")) {
            component = "0010101";
        } else if (strcmp(component, "M")) {
            component = "1110000";
        } else if (strcmp(component, "!M")) {
            component = "1110001";
        } else if (strcmp(component, "-M")) {
            component = "1110011";
        } else if (strcmp(component, "M+1") || strcmp(component, "1+M")) {
            component = "1110111";
        } else if (strcmp(component, "M-1")) {
            component = "1110010";
        } else if (strcmp(component, "D+M") || strcmp(component, "M+D")) {
            component = "1000010";
        } else if (strcmp(component, "D-M")) {
            component = "1010011";
        } else if (strcmp(component, "M-D")) {
            component = "1000111";
        } else if (strcmp(component, "D&M") || strcmp(component, "M&D")) {
            component = "1000000";
        } else if (strcmp(component, "D|M") || strcmp(component, "M|D")) {
            component = "1010101";
        }
        
        if (jump == NULL) {
            jump = "000";
        } else if (strcmp(jump, "JGT")) {
            jump = "001";
        } else if (strcmp(jump, "JEQ")) {
            jump = "010";
        } else if (strcmp(jump, "JGE")) {
            jump = "011";
        } else if (strcmp(jump, "JLT")) {
            jump = "100";
        } else if (strcmp(jump, "JNE")) {
            jump = "101";
        } else if (strcmp(jump, "JLE")) {
            jump = "110";
        } else if (strcmp(jump, "JMP")) {
            jump = "111";
        }
        
        fprintf(outputFile, "111%s%s%s\n", component, destination, jump);
    }
    
    fclose(outputFile);
    
    return 0;
}
