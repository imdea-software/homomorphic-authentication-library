#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("Please give the function file as argument\n");
		return 1;
	}

	char * function_file = (char *) argv[1];
	FILE * f_file = fopen(function_file, "r");
    if (f_file == NULL)
    {
        printf("Error: no such file..\n");
        return 1;
    }

	// Reading from the file

	FILE * c_file = fopen("func/function.c","w+");

	char * word = malloc(sizeof(char));

	fputs("/*****Automatically generated file *****/\n\n", c_file);
    fputs("#include \"function.h\"\n", c_file);

    // Loop with fgp_tag
    while (fscanf(f_file, "%c", word) != EOF)
    {
        if (*word != 'T')
        {
        	fputc((int) *word, c_file);
        }
        else
        {
        	fscanf(f_file, "%c", word);
        	if (*word != 'Y')
        	{
        		fputc((int) 'T', c_file);
        		fputc((int) *word, c_file);
        		continue;
        	}
        	else
        	{
        		while (*word != 'E')
	        	{
	        		fscanf(f_file, "%c", word);
	        	}
	        	fputs("fgp_tag", c_file);
        	}
        }
    }

    fputc((int) '\n', c_file);
    // Go back to the begining of the f_file
    rewind(f_file);


    // Loop with fgp_vkf
    while (fscanf(f_file, "%c", word) != EOF)
    {
        if (*word != 'T')
        {
        	fputc((int) *word, c_file);
        }
        else
        {
        	fscanf(f_file, "%c", word);
        	if (*word != 'Y')
        	{
        		fputc((int) 'T', c_file);
        		fputc((int) *word, c_file);
        		continue;
        	}
        	else
        	{
        		while (*word != 'E')
	        	{
	        		fscanf(f_file, "%c", word);
	        	}
	        	fputs("fgp_vkf", c_file);
        	}
        }
    }
    
    // Go back to the begining of the f_file
    rewind(f_file);


    // Loop with fgp_poly
     while (fscanf(f_file, "%c", word) != EOF)
    {
        if (*word != 'T' )
        {
            fputc((int) *word, c_file);
        }
        else
        {
            fscanf(f_file, "%c", word);
            if (*word != 'Y')
            {
                fputc((int) 'T', c_file);
                fputc((int) *word, c_file);
                continue;
            }
            else
            {
                while (*word != 'E')
                {
                    fscanf(f_file, "%c", word);
                }
                fputs("fgp_msg", c_file);
            }
        }
    }
    
    fclose(c_file);
    fclose(f_file);

	return 0;
}