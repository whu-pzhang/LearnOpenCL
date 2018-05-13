#include <stdio.h>
#include <stdlib.h>

char *getKernelSource(const char *filename)
{
    FILE *pFile = fopen(filename, "r");
    fseek(pFile, 0L, SEEK_END);
    long len = ftell(pFile) + 1;
    rewind(pFile);

    char *source = (char *)malloc(sizeof(char) * len);
    fread(source, sizeof(char), len, pFile);
    fclose(pFile);
    return source;
}
