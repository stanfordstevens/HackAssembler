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
            for (i = 15; i >= 0; i--) {
                bit = value >> i;
                
                if (bit & 1) {
                    fprintf(outputFile, "1");
                } else {
                    fprintf(outputFile, "0");
                }
            }
            
            fprintf(outputFile, "\n");
        }
    }
    
    fclose(outputFile);
    
    return 0;
}
