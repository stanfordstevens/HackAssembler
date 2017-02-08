//
//  main.c
//  HackAssembler
//
//  Created by Stanford Stevens on 2/7/17.
//  Copyright © 2017 Stanford Stevens. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    FILE *inputFile = fopen("/Users/FireCrotch/Documents/Programming/Learning/nand2tetris/projects/06/add/Add.asm", "r");
    
    if (inputFile == NULL) {
        fprintf(stderr, "Cant open input file\n");
        return 1;
    }
    
    FILE *outputFile = fopen("/Users/FireCrotch/Desktop/ouput.hack", "w");
    
    if (outputFile == NULL) {
        fprintf(stderr, "Cant open ouput file\n");
        return 1;
    }
    
    int c;
    
    while ((c = fgetc(inputFile)) != EOF) {
        printf("%c", c);
    }
    
    return 0;
}
