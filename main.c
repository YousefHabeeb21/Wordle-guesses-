#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()
#include <stdbool.h>  // for boolean statements

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
#define WORDS_FILE_NAME "wordsLarge.txt"
// #define WORDS_FILE_NAME  "wordsTiny.txt"
#define MAX_NUMBER_OF_WORDS 12947   // Number of words in the full set of words file
#define true 1   // Make boolean logic easier to understand
#define false 0  // Make boolean logic easier to understand

typedef struct wordCount wordCountStruct;
struct wordCount{
    char word[ WORD_LENGTH + 1];   // The word length plus NULL
    int score;                     // Score for the word
};


//-----------------------------------------------------------------------------------------
// Read in words from file into array.  We've previously read the data file once to
// find out how many words are in the file.
void readWordsFromFile(
            char fileName[],        // Filename we'll read from
            wordCountStruct *words, // Array of words where we'll store words we read from file
            int *wordCount)          // How many words.  Gets updated here and returned
{
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    assert( inFilePtr != NULL);               // Ensure file open worked correctly

    // Read each word from file and store into array, initializing the score for that word to 0.
    char inputString[ 6];
    *wordCount = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        strcpy( words[ *wordCount].word, inputString);
//        printf("%s ",words[ *wordCount].word );
        words[ *wordCount].score = 0;
        (*wordCount)++;
    }

    // Close the file
    fclose( inFilePtr);
} // end readWordsFromFile(..)


//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordCountStruct *) a)->score;
    int secondScore = ((wordCountStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
        return strcmp( ((wordCountStruct *)a)->word,  ((wordCountStruct *)b)->word );
    }
} //end compareFunction(..)




int getIndex(char* secretWord, char i) {
    char *result = strchr(secretWord, i);
    if (result == NULL) {
        return -1;
    }
    return (int)(result - secretWord);
}

int getIndex2(char* secretWord, char i) {
    char *result = strrchr(secretWord, i);
    if (result == NULL) {
        return -1;
    }
    return (int)(result - secretWord);
}

bool getMiddle(int *mid, bool check3rd, char j, char computerGuess[], int first, int last){
    
    for(int m = 0; m < WORD_LENGTH; m++){
        if ((j == computerGuess[m]) && (m != first) && (m != last)){
            check3rd = true;
            *mid = m;
            break;
        }
    }
    return (check3rd);
}

void compare1(int j, int first, char result[], char temp[], char secretWordcopy[]){
    if(j == first){
        result[j] = temp[j];
        secretWordcopy[j] = '0';
    }
    else{
        result[j] = '*';
        if(temp[first] != secretWordcopy[first]){
            secretWordcopy[first] = '0';
        }
        else{
            result[j] = ' ';
        }
    }
}



void compare2(int j, int first,int last, bool check3rd, char result[], char temp[], char secretWordcopy[]){
    if(j == first || j == last){
        result[j] = temp[j];
        secretWordcopy[j] = '0';
    }
    else{
        result[j] = '*';
        if(temp[first] != secretWordcopy[first]){
            secretWordcopy[first] = '0';
        }
        else if(temp[last] != secretWordcopy[last]){
            secretWordcopy[last] = '0';
        }
        else{
            result[j] = ' ';
        }
    }
}


void compare3(int j, int first, int last, bool check3rd, int mid, char result[], char temp[], char secretWordcopy[]){
    if(j == first || j == last || j == mid){
        result[j] = temp[j];
        secretWordcopy[j] = '0';
    }
    else{
        result[j] = '*';
        if(temp[first] != secretWordcopy[first]){
            secretWordcopy[first] = '0';
        }
        else if(temp[last] != secretWordcopy[last]){
            secretWordcopy[last] = '0';
        }
        else if(temp[mid] != secretWordcopy[mid]){
            secretWordcopy[mid] = '0';
        }
        else{
            result[j] = ' ';
        }
    }
}


void elimination(char result[], char temp[], wordCountStruct allWords[], int wordCount){
    
    for (int i = 0; i < strlen(result); i++){
        if((result[i] != ' ') && (result[i] != '*')){
            for( int j = 0; j < wordCount; j++) {
                if(allWords[j].word[i] != result[i]) {
                    allWords[j].score = -1;

                }
            }
        }
    }

    for(int i = 0; i < strlen(result); i++){
      if(result[i] == ' '){

        bool checkdup = false;
        for (int g = 0; g < strlen(temp); g++){
          if(temp[i] == temp[g] && i != g){
            checkdup = true;
            break;
          }
        }

        if (checkdup == false){
          for(int j = 0; j < wordCount; j++){
            if(strchr(allWords[j].word, temp[i]) != NULL){
              allWords[j].score = -1;
            }
          }
        }
        else{
          for(int j = 0; j < wordCount; j++){
            if(allWords[j].word[i] == temp[i]){
                allWords[j].score = -1;
            }
          }
        }
      }
    }
    
    for(int i = 0; i < strlen(result); i++){
        if(result[i] == '*'){
            for(int j = 0; j < wordCount; j++){
                if(allWords[j].word[i] == temp[i]){
                    allWords[j].score = -1;
                }
            }
        }
    }
        
}



void printStatements(int guessNumber, char result[], char temp[]){
    
    printf("%5d. ", guessNumber);
    //print the guess word
    for(int k = 0; k < WORD_LENGTH; k++){
        if(result[k] == temp[k]){
            printf("%c ", toupper(temp[k]));
        }
        else{
            printf("%c ", temp[k]);
        }
    }
    printf("\n       ");
    
    
    // print the error under the guess word
    for(int h = 0; h < WORD_LENGTH; h++){
        if(result[h] == '*'){
            printf("* ");
        }
        else{
            printf("  ");
        }
    }
}



// -----------------------------------------------------------------------------------------
// Find a secret word
void findSecretWord(wordCountStruct allWords[], int wordCount, char secretWord[]){
    
    printf("Trying to find secret word: \n");
    // Display secret word with a space between letters, to match the guess words below.
    printf("       ");
    for( int i = 0; i < WORD_LENGTH; i++) {
        printf("%c ", secretWord[ i]);
    }
    printf("\n\n");
    
    char starters [5][6];
    strcpy(starters[0],"brick");
    strcpy(starters[1],"jumpy");
    strcpy(starters[2],"glent");
    strcpy(starters[3],"vozhd");
    strcpy(starters[4],"waqfs");
    
    int o = 0, l = 0, guessNumber = 1;
    bool win = false;
    
    while(!win){
        
        char temp[6] = {}; // guess word
        
        // if(o < 5){ // use starter words
        //     strcpy(temp, starters[o]);
        //     l--;
        // }
        // else{  //copy a word from the file
          strcpy(temp,allWords[0].word);
        // }

        char secretWordcopy[6] = {}; // used to prevent extra '*'
        
        strcpy(secretWordcopy,secretWord);
        char result[6] = {};

        for(int j = 0; j < WORD_LENGTH; j++){
            
            if(strchr(secretWordcopy, temp[j]) != NULL){
                
                int first = getIndex(secretWordcopy, temp[j]); // retrieve first index
                int last = getIndex2(secretWordcopy, temp[j]); // retrieve last index
                
                int mid = 0;
                bool check3rd = false;  // retrieve middle index
                check3rd = getMiddle(&mid,check3rd,temp[j],secretWordcopy,first,last);
                
                // if there is only one occurence
                if(first == last){
                    compare1(j, first, result, temp, secretWordcopy);
                }
                // if there are two occurences
                else if (first != last && check3rd == false){
                    compare2(j, first, last, check3rd, result, temp, secretWordcopy);
                }
                //if there are 3 occurences
                else if (first != last && check3rd == true){
                    compare3(j, first, last, check3rd, mid, result, temp, secretWordcopy);
                }
            }
            //if there is no letter that matches
            else{
                result[j] = ' ';
            }
        }
        
        elimination(result, temp, allWords, wordCount);
        
        qsort(allWords, wordCount, sizeof(wordCountStruct), compareFunction);
        
        int count = 0;
    
        for(int k = 0; k < wordCount; k++){
            if( allWords[k].score >= 0){
                count++;
            }
        }
        wordCount = count;

        printStatements(guessNumber, result,temp);
        
        // //check to see if you won
        if(strcmp(secretWord, result) == 0){
            printf("\n\n got it! \n");
            win = true;
        }
        printf("\n");
        guessNumber++;
        l++;
        o++;
    }
}



// -----------------------------------------------------------------------------------------
int main() {
    char wordsFileName[81];                   // Stores the answers file name
    strcpy(wordsFileName, WORDS_FILE_NAME);   // Set the filename, defined at top of program.
    srand( (unsigned) time( NULL));           // Seed the random number generator to be current time
    // Declare space for all the words, of a maximum known size.
    wordCountStruct allWords[ MAX_NUMBER_OF_WORDS];
    // Start out the wordCount to be the full number of words.  This will decrease as
    //    play progresses each time through the game.
    int wordCount = 0;
    // The secret word that the computer will try to find, plus the return character from fgets.
    char secretWord[ WORD_LENGTH + 1];
    char userInput[ 81];                // Used for menu input of secret word

    // Read in words from file, update wordCount and display information
    readWordsFromFile( wordsFileName, allWords, &wordCount);
    printf("Using file %s with %d words. \n", wordsFileName, wordCount);

    // Run the word-guessing game three times
    for( int i = 0; i < 3; i++) {
        // Reset secret Word
        strcpy( secretWord, "");
        // Display prompt
        printf("-----------------------------------------------------------\n");
        printf("\n");
        printf("Enter a secret word or just r to choose one at random: ");
        scanf(" %s", userInput);
        // Eliminate the return character at end or userInput if it is there
        int length = (int) strlen( userInput);
        if( userInput[ length] == '\n') {
            userInput[ length] = '\0';
        }
        strcpy( secretWord, userInput);   // Store the secret word from user input
                
        // If input was 'r' then choose a word at random.
        if( strlen( secretWord) <= 1) {
            // Randomly select a secret word to be guessed.
            int randomIndex = rand() % wordCount;
            strcpy( secretWord, allWords[ randomIndex].word);
        }
        
        // Run the game once with the current secret word
        findSecretWord( allWords, wordCount, secretWord);

        for (int i = 0; i < wordCount; i++){
          allWords[i].score = 0;
        }
      
    }

    printf("Done\n");
    printf("\n");
    return 0;
} // end main()
