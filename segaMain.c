#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    
    uint8_t *rom;
    if(argc < 2){
        puts("Didn't supply a file");
        return 0;
    }

    FILE *fp = fopen(argv[1], "rb");

    fseek(fp, 0L, SEEK_END);
    uint32_t lSize = ftell( fp);
    rewind(fp);

    rom = (uint8_t *)malloc(lSize);

    fread(rom, lSize, 1, fp);
    
    return 0;
}
