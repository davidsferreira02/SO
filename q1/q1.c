#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int printPhrases = 0;
    char* fileName;

    //only print number of phrases
    if(argc == 2) {
        fileName = malloc(strlen(argv[1])+ 1);
        strcpy(fileName, argv[1]);
    }
    //print phrases
    else if(argc == 3 && strcmp(argv[2], "-l")) {
        printPhrases = 1;
        fileName = malloc(strlen(argv[1])+ 1);
        strcpy(fileName, argv[2]);
    }
    //wrong input
    else{
        printf("usage: phrases [-l] file\n");
        return -1;
    }

    //terminating chars
    char* delimiters = ".!?";

    //File
    FILE* file;
    file = fopen(fileName, "r");
    if(file == NULL){
        printf("file could not be opened\n");
        return -1;
    }

        char *phrase;
        int phraseCounter = 0;

        //set end of file
        fseek (file, 0, SEEK_END);
        //get size of file
        long textLength = ftell(file);
        //set start of file
        fseek(file, 0, SEEK_SET);
        //set size
        char * text = malloc(textLength + 1);
        text[textLength] = '\0';    //null terminate

        //read contents of file to string
        char* textCopy;
        if(text){
            fread(text, 1, textLength, file);
            //copy content
            textCopy = strdup(text);
        }
        else {
            printf("file is empty\n");
            return -1;
        }

        //remove newline from string
        for(size_t i = 0; i < textLength; i++) {
            if(text[i] == '\n') text[i] = ' ';
        }

        phrase = strtok(text, delimiters);
        phraseCounter++;

        while(phrase != NULL) {
            if(printPhrases == 1)
                printf("[%i] %s%c\n", phraseCounter, phrase, textCopy[phrase-text+ strlen(phrase)]);
            phrase = strtok(NULL, delimiters);
            phraseCounter++;
        }

        if(printPhrases == 0){
            phraseCounter--; //decrement because fo while
            printf("%i\n", phraseCounter);
        }


    fclose(file);
    return 0;
}