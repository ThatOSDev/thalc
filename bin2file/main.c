#include <stdio.h>

int main(int argc, char* argv[])
{
	FILE *source = fopen("ElaineSans-SemiBold.ttf", "rb");
	if (source == NULL)
    {
       printf("File not found.\n");
       return -1;
	}
	fseek(source, 0, SEEK_END);
	unsigned long totalsize = ftell(source);
	rewind(source);
	unsigned char buffer[totalsize];
	int i;
    for (i = 0; i < totalsize; i++)
    {
        buffer[i] = fgetc(source);
    }
	fclose(source);

	int jj = 0;
    FILE *fPtr;
    fPtr = fopen("ElaineSans_FONT.h", "w");
    fprintf(fPtr, "\n#ifndef ELAINSANS_FONT_H\n#define ELAINSANS_FONT_H\n\nunsigned char asciifont[%lu] = {", totalsize);
	for (i = 0; i < (totalsize - 1); i++)
    {
       fprintf(fPtr, "%d,", buffer[i]);
       jj++;
       if(jj == 15)
       {
           jj = 0;
           fprintf(fPtr, "\n");
       }
	}
	fprintf(fPtr, "%d", buffer[i]);
	fprintf(fPtr, "};\n\n#endif // ELAINSANS_FONT_H\n\n");

    fclose(fPtr);

    return 0;
}

