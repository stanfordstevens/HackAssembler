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

size_t numberOfSymbols = 0;
char **symbolKeys;
int *symbolAddresses;

typedef struct node {
    char *key;
    int value;
    struct node *next;
} node;

int hash(char *key) {
    return toupper(key[0]) - 'A';
}


//void addSymbol(node *symbols[26], char *key, int value) {
//    int hashedKey = hash(key);
//    
//    node *newPointer = malloc(sizeof(node));
//    if (newPointer == NULL) {
//        return;
//    }
//    
//    strcpy(newPointer->key, key);
//    newPointer->value = value;
//    newPointer->next = NULL;
//    
//    if (symbols[hashedKey] == NULL) {
//        symbols[hashedKey] = newPointer;
//    } else {
//        node *previousPointer = symbols[hashedKey];
//        while (1) {
//            if (previousPointer->next == NULL) {
//                previousPointer->next = newPointer;
//                break;
//            }
//            
//            previousPointer = previousPointer->next;
//        }
//    }
//}
//
//void getAddressFromSymbols(node *symbols[26], char *key, int *address) {
//    int hashedKey = hash(key);
//    
//    if (symbols[hashedKey] == NULL) {
//        return;
//    } else {
//        while (1) {
//            node *previousPointer = symbols[hashedKey];
//            if (strcmp(previousPointer->key, key) == 0) {
//                *address = previousPointer->value;
//                break;
//            } else if (previousPointer->next == NULL) {
//                break;
//            }
//        }
//    }
//}

void addSymbol(char *key, int address) {
    numberOfSymbols++;
    
    symbolKeys = realloc(symbolKeys, numberOfSymbols * sizeof(char *));
    symbolAddresses = realloc(symbolAddresses, numberOfSymbols * sizeof(int));
    
    size_t newIndex = numberOfSymbols - 1;
    symbolKeys[newIndex] = malloc(240 * sizeof(char));
    strcpy(symbolKeys[newIndex], key);
    symbolAddresses[newIndex] = address;
}

void addressForSymbolKey(char *key, int *address) {
    for (int i = 0; i < numberOfSymbols; i++) {
        if (strcmp(symbolKeys[i], key) == 0) {
            *address = symbolAddresses[i];
            return;
        }
    }
    
    return;
}

int shouldIgnoreLine(char *line) {
    if ((line[0] == '/' && line[1] == '/') || isspace(line[0]) || strcmp(line, "") == 0 || strcmp(line, "\r\n") == 0) { //TODO: this is awful
        return 1;
    }
    
    return 0;
}

char* trimLeadingWhitespaceFromString(char *string) {
    while(isspace((unsigned char)*string)) {
        string++;
    }
    
    return string;
}

int main(int argc, const char * argv[]) {
//    node *symbols[26] = {NULL};
    
    int variableAddress = 16;
    int initialSymbolAmount = 23;
    symbolKeys = malloc(initialSymbolAmount*sizeof(char *));
    for (int i = 0; i < initialSymbolAmount; i++) {
        symbolKeys[i] = malloc(240 * sizeof(char)); //TODO: dont know how big each string is
    }
    
    symbolAddresses = malloc(initialSymbolAmount*sizeof(int));
    
    addSymbol("SP", 0);
    addSymbol("LCL", 1);
    addSymbol("ARG", 2);
    addSymbol("THIS", 3);
    addSymbol("THAT", 4);
    addSymbol("R0", 0);
    addSymbol("R1", 1);
    addSymbol("R2", 2);
    addSymbol("R3", 3);
    addSymbol("R4", 4);
    addSymbol("R5", 5);
    addSymbol("R6", 6);
    addSymbol("R7", 7);
    addSymbol("R8", 8);
    addSymbol("R9", 9);
    addSymbol("R10", 10);
    addSymbol("R11", 11);
    addSymbol("R12", 12);
    addSymbol("R13", 13);
    addSymbol("R14", 14);
    addSymbol("R15", 15);
    addSymbol("SCREEN", 16384);
    addSymbol("KBD", 24576);
    
    char filepath[200];
    printf("Enter filepath> ");
    scanf("%s", filepath);
    
    FILE *inputFile = fopen(filepath, "r");
    
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
    int previousLineCount = -1;
    while (fgets(line, sizeof(line), inputFile)) {
        char *newLine = trimLeadingWhitespaceFromString(line);
        
        if (shouldIgnoreLine(newLine) == 1) {
            continue;
        }
        
        if (newLine[0] == '(') {
            char *beginning = strstr(newLine, "(");
            char *end = strstr(newLine, ")");
            
            size_t labelSize = end - beginning; //beginning - end
            char *label = malloc(labelSize * sizeof(char));
            
            strncpy(label, beginning + 1, labelSize - 1);
            label[labelSize - 1] = '\0';
            
            addSymbol(label, previousLineCount + 1);
            free(label); //TODO: should this be here?
            
            continue;
        } else {
            previousLineCount += 1;
        }
    }
    
    rewind(inputFile);
    
    while (fgets(line, sizeof(line), inputFile)) {
        char *newLine = trimLeadingWhitespaceFromString(line);
        
        if (shouldIgnoreLine(newLine) == 1 || line[0] == '(') {
            continue;
        }
        
        if (newLine[0] == '@') {
            fprintf(outputFile, "0");
            int address = -1;
            char *value = strtok(newLine, "@");
            
            if (isdigit(value[0])) {
                address = atoi(value);
            } else {
                value = strtok(value, "\r");
                addressForSymbolKey(value, &address);
                if (address == -1) {
                    address = variableAddress;
                    addSymbol(value, address);
                    variableAddress += 1;
                }
            }
            
            if (address == -1) {
                printf("No address for key: %s", value);
                return 1;
            }
            
            int i, bit;
            for (i = 14; i >= 0; i--) {
                bit = address >> i;
                
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
        
        if (strchr(newLine, '=')) {
            destination = strtok(newLine, "=");
            component = strtok(strtok(NULL, "="), ";");
            jump = strtok(NULL, ";");
        } else if (strchr(newLine, ';')) {
            component = strtok(newLine, ";");
            jump = strtok(NULL, ";");
        } else {
            component = newLine;
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
