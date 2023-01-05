#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * cypher(const char *originalText, const char *word, const char *cyphered)
{
    
    int wordSize = strlen(word);
    int cypheredSize = strlen(cyphered);


    

    //strstr(const char *haystack, const char *needle)

    //find number of  word ocurrences
    size_t ocurrences = 0;
    for (const char *originalText2 = originalText; (originalText2 = strstr(originalText2, word)) != NULL; originalText2 += wordSize)
        ocurrences++;


    //calculate new string size to accomodate new word
    char *result = malloc(strlen(originalText) + ocurrences * (wordSize - cypheredSize) + 1);

    const char *originalText2 = originalText;
    char *t = result;

    //replace
    if (ocurrences != 0)
    {
        for (const char *text; (text = strstr(originalText2, word)) != NULL; originalText2 = text + wordSize)
        {   
            //memcpy(void *dest, const void * src, size_t n)
            memcpy(t, originalText2, text - originalText2);
            t += text - originalText2;
            memcpy(t, cyphered, cypheredSize);
            t += cypheredSize;
        }
    }

    strcpy(t, originalText2);

    return result;
}

char * readFile(char* filename, int* textLines, long* textLengthOut) {
    FILE *file;

    if(filename == NULL) {
        file =stdin;
    }
    else {
        file = fopen(filename, "r");
        if(file == NULL){
            printf("file could not be opened\n");
            return NULL;
        }
    }
    
    //set end of file
    fseek (file, 0, SEEK_END);
    //get size of file
    long textLength = ftell(file);
    if(textLengthOut != NULL){
        *textLengthOut = textLength;
    } 
    //set start of file
    fseek(file, 0, SEEK_SET);
    //set size
    char * text = malloc(textLength);
    text[textLength] = '\0';    //null terminate

    if(text) {
        fread(text, 1, textLength, file);
        if(textLines != NULL) {
            *textLines = 0;
            for(size_t i = 0; i < textLength; i++){
                if(text[i] == '\n'){
                    (*textLines)++;
                }
                    
            }
        }
        return text;
    }
    
    return NULL;
}
