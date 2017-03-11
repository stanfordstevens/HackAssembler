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

typedef struct node {
    char *key;
    int value;
    struct node *next;
} node;

int hash(char *key) {
    return toupper(key[0]) - 'A';
}

node *symbols[26] = {NULL};

void add_symbol(char *key, int value) {
    int hashedKey = hash(key);
    
    node *newPointer = malloc(sizeof(node));
    if (newPointer == NULL) {
        return;
    }
    
    newPointer->key = malloc((strlen(key) + 1) * sizeof(char));
    strcpy(newPointer->key, key);
    
    newPointer->value = value;
    newPointer->next = NULL;
    
    if (symbols[hashedKey] == NULL) {
        symbols[hashedKey] = newPointer;
    } else {
        node *previousPointer = symbols[hashedKey];
        while (1) {
            if (previousPointer->next == NULL) {
                previousPointer->next = newPointer;
                break;
            }
            
            previousPointer = previousPointer->next;
        }
    }
}

void address_for_symbol_key(char *key, int *address) {
    int hashedKey = hash(key);
    
    if (symbols[hashedKey] == NULL) {
        return;
    } else {
        node *previousPointer = symbols[hashedKey];
        while (1) {
            if (strcmp(previousPointer->key, key) == 0) {
                *address = previousPointer->value;
                break;
            } else if (previousPointer->next == NULL) {
                break;
            } else {
                previousPointer = previousPointer->next;
            }
        }
    }
}

//void add_symbol(char *key, int address) {
//    number_of_symbols++;
//    
//    if (number_of_symbols > length_of_symbols) {
//        length_of_symbols = length_of_symbols * 2;
//        symbol_keys = realloc(symbol_keys, length_of_symbols * sizeof(char *));
//        symbol_addresses = realloc(symbol_addresses, length_of_symbols * sizeof(int));
//    }
//    
//    size_t new_index = number_of_symbols - 1;
//    size_t key_length = strlen(key) + 1;
//    symbol_keys[new_index] = malloc(key_length * sizeof(char));
//    strcpy(symbol_keys[new_index], key);
//    symbol_addresses[new_index] = address;
//}
//
//void address_for_symbol_key(char *key, int *address) {
//    for (int i = 0; i < number_of_symbols; i++) {
//        if (strcmp(symbol_keys[i], key) == 0) {
//            *address = symbol_addresses[i];
//            return;
//        }
//    }
//    
//    return;
//}

int should_ignore_line(char *line) {
    if ((line[0] == '/' && line[1] == '/') || isspace(line[0]) || strcmp(line, "") == 0 || strcmp(line, "\r\n") == 0) { //TODO: this is awful
        return 1;
    }
    
    return 0;
}

char* trim_leading_whitespace(char *string) {
    while(isspace((unsigned char)*string)) {
        string++;
    }
    
    return string;
}

int main(int argc, const char * argv[]) {
    int variable_address = 16;
    
    add_symbol("SP", 0);
    add_symbol("LCL", 1);
    add_symbol("ARG", 2);
    add_symbol("THIS", 3);
    add_symbol("THAT", 4);
    add_symbol("R0", 0);
    add_symbol("R1", 1);
    add_symbol("R2", 2);
    add_symbol("R3", 3);
    add_symbol("R4", 4);
    add_symbol("R5", 5);
    add_symbol("R6", 6);
    add_symbol("R7", 7);
    add_symbol("R8", 8);
    add_symbol("R9", 9);
    add_symbol("R10", 10);
    add_symbol("R11", 11);
    add_symbol("R12", 12);
    add_symbol("R13", 13);
    add_symbol("R14", 14);
    add_symbol("R15", 15);
    add_symbol("SCREEN", 16384);
    add_symbol("KBD", 24576);
    
    char filepath[200];
    printf("Enter filepath> ");
    scanf("%s", filepath);
    
    FILE *input_file = fopen(filepath, "r");
    
    if (input_file == NULL) {
        fprintf(stderr, "Cant open input file\n");
        return 1;
    }
    
    FILE *output_file = fopen("/Users/FireCrotch/Desktop/ouput.hack", "w");
    
    if (output_file == NULL) {
        fprintf(stderr, "Cant open ouput file\n");
        return 1;
    }
    
    char line[256];
    int previous_line_count = -1;
    while (fgets(line, sizeof(line), input_file)) {
        char *new_line = trim_leading_whitespace(line);
        
        if (should_ignore_line(new_line) == 1) {
            continue;
        }
        
        if (new_line[0] == '(') {
            char *beginning = strstr(new_line, "(");
            char *end = strstr(new_line, ")");
            
            size_t label_size = end - beginning;
            char *label = malloc(label_size * sizeof(char));
            
            strncpy(label, beginning + 1, label_size - 1);
            label[label_size - 1] = '\0';
            
            add_symbol(label, previous_line_count + 1);
            free(label);
            
            continue;
        } else {
            previous_line_count += 1;
        }
    }
    
    rewind(input_file);
    
    while (fgets(line, sizeof(line), input_file)) {
        char *new_line = trim_leading_whitespace(line);
        
        if (should_ignore_line(new_line) == 1 || line[0] == '(') {
            continue;
        }
        
        if (new_line[0] == '@') {
            fprintf(output_file, "0");
            int address = -1;
            char *value = strtok(new_line, "@");
            
            if (isdigit(value[0])) {
                address = atoi(value);
            } else {
                value = strtok(value, "\r");
                address_for_symbol_key(value, &address);
                if (address == -1) {
                    address = variable_address;
                    add_symbol(value, address);
                    variable_address += 1;
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
                    fprintf(output_file, "1");
                } else {
                    fprintf(output_file, "0");
                }
            }
            
            fprintf(output_file, "\n");
            continue;
        }
        
        char *destination = NULL;
        char *component = NULL;
        char *jump = NULL;
        
        if (strchr(new_line, '=')) {
            destination = strtok(new_line, "=");
            component = strtok(strtok(NULL, "="), ";");
            jump = strtok(NULL, ";");
        } else if (strchr(new_line, ';')) {
            component = strtok(new_line, ";");
            jump = strtok(NULL, ";");
        } else {
            component = new_line;
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
        
        fprintf(output_file, "111%s%s%s\n", component, destination, jump);
    }
    
    fclose(output_file);
    
    return 0;
}
