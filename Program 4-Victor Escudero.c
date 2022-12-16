//  wordleGuessSecretWord.c
//  Have the program do the guessing to discover the secret wordle word.
/* ---------------------------------------------
Program 4: Guess Wordle Word

Course: CS 211, Fall 2022. Tues 8am lab
System: Mac using Replit
Author: Victor Escudero
---------------------------------------------
*/
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
/*  Running the program looks like:
        Using file wordsLarge.txt with 12947 words. 
        -----------------------------------------------------------
        
        Enter a secret word or just r to choose one at random: dream
        Trying to find secret word: 
               d r e a m 
        
            1. s o r e s 
                   * *   
            2. s e r e r 
                 * *     
            3. r a r e r 
               * *   *   
            4. r a r e e 
               * *   *   
            5. s a r e e 
                 * * *   
            6. p a r e r 
                 * * *   
            7. c a r e r 
                 * * *   
            8. b a r e r 
                 * * *   
            9. f a r e r 
                 * * *   
           10. D a r e r 
                 * * *   
           11. D e a r e 
                 * * *   
           12. D e b A r 
                 *     * 
           13. D R E A r 
                       * 
           14. D R E A d 
                         
           15. D R E A M 
                         
        Got it!

     ... and then it runs two more times ...
 */
#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
#define WORDS_FILE_NAME "wordsLarge.txt"
//#define WORDS_FILE_NAME  "wordsTiny.txt"
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
  char inputString[6];
  *wordCount = 0;
  while( fscanf( inFilePtr, "%s", inputString) != EOF) {
    strcpy( words[ *wordCount].word, inputString);
    words[ *wordCount].score = 0;
    (*wordCount)++;
  }

  // Close the file
  fclose( inFilePtr);
} // end readWordsFromFile(..)
//--------------------------------------------------------------------
//This function display guess word with capital letters on the letters that matched the same position with hidden word and displays an (*) under the letters that are in the secret word but not on the right position
void displayWords(char computerGuess[], char secretWord[])
{
  //we make copies of the words we want to compare
  char * guessWord = malloc(6*sizeof(char));
  char * hiddenWord = malloc(6*sizeof(char));
  char displayCharacter = ' ';

  strcpy(guessWord, computerGuess);//copy guess word
  strcpy(hiddenWord, secretWord);//copy secret word

  //First pass to check same position and same letter.
  for(int i = 0; i < 5; i++)
  {
    if(guessWord[i] != hiddenWord[i])
    {
      printf(" %c ", guessWord[i]);
    }
    else 
    {
      //If it matches then make that letter a capital letter and printf it 
      guessWord[i] = toupper(guessWord[i]);
      printf(" %c ", guessWord[i]);
      guessWord[i] = '-';
      hiddenWord[i] = '+';
    }
  }

  printf("\n");
  printf("%5c ", ' ');

  //For the next line print an asterisk(*) under the letters that are in the word.
  for(int i = 0; i< WORD_LENGTH; i++)
  {
    displayCharacter = ' ';//deafult display character.
    for(int j = 0; j< WORD_LENGTH; j++)
    {
      if(guessWord[i] == hiddenWord[j])
      {
        displayCharacter = '*';//if letter is present in hidden word.
      }
    }
    printf(" %c ", displayCharacter);//display character.
  }
  printf("\n");

  //Free dynamically allocated c strings
  free(guessWord);
  free(hiddenWord);
}//end of display function
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
// -----------------------------------------------------------------------------------------
//scoreCalculator takes 2 words, one from answer words and one from all words. It compares the guess word with the asnwer word and calculates score. 3 points for same letter same position and 1 point for same letter in word but wrong position
int scoreCalculator(char answer[], char wordToCompare[])
{
  int score = 0;// score of word

  //we make copies of the words we want to compare
  char * ansWord = malloc(6*sizeof(char));
  char * compareWord = malloc(6*sizeof(char));

  strcpy(ansWord, answer);//copy answer word
  strcpy(compareWord, wordToCompare);//copy guess word

  //First pass to check same position and same letter.
  for(int i = 0; i < WORD_LENGTH; i++)
  {
    if(ansWord[i] == compareWord[i])
    {
      score = score + 3;//3 points for same character in the same position.
      //blank out both letters from both words so we don't give extra points.
      ansWord[i] = '+';
      compareWord[i] = '-';
    }
  }
  //second pass to score 1 point if letter is in the word but not in the correct spot
  for(int i = 0; i< WORD_LENGTH; i++)
  {
    for(int j = 0; j< WORD_LENGTH; j++)
    {
      if(ansWord[i] == compareWord[j])
      {
        score = score + 1;//1 point for same letter but wrong position
        //blank out both letters from both words so we don't give extra points.
        ansWord[i] = '+';
        compareWord[j] = '-';
      }
    }
  }

  //Free dynamically allocated c strings
  free(ansWord);
  free(compareWord);
  
  return score;//return total score for one word
}
//---------------------------------------------------------
//scoreFunc takes the total word array of type struct wordCountStruct, and the size of the all words array. The function calculates the score for each word
void bestFirstWordCalculator(wordCountStruct * pOriginalArray, int sizeOfTotalWords)
{
  int score = 0;//score
  
  wordCountStruct * pArrayCopy = (wordCountStruct*)malloc(sizeOfTotalWords*sizeof(wordCountStruct));//copy of all words array
  
  //copying array
  for(int i = 0; i < sizeOfTotalWords; i++)
  {
    strcpy(pArrayCopy[i].word, pOriginalArray[i].word);
    pArrayCopy[i].score = pOriginalArray[i].score;
  }
 
  //Compare all words with themselves and get score for each word to see which one contains the best common letters
  for(int i = 0; i < sizeOfTotalWords; i++)
  {
    for(int j = 0; j< sizeOfTotalWords; j++)
    {
      score = score + scoreCalculator(pOriginalArray[i].word, pArrayCopy[j].word);//Let score calculator function deal with scoring
      pOriginalArray[i].score = score;// assign total score for word
    }
    score = 0;//reset score after it has compared one word with all answer words
  }

  //free array copy
  free(pArrayCopy);
}
// -----------------------------------------------------------------------------------------
//It compares secrete word and guess word. Gets and stores a char array containing letters that are in the word and also in another aray letters that are not in the word. Finally it updates the final guess word wich contains letters that have macthed in the right position. 
void wordCompare(char secretWord[] , char * computerGuess, char * lettersNotInWord, char * lettersInWord, char * finalGuess, int * InWordCount, int * NotInWordCount)
{
   //we make copies of the words we want to compare
  char * guessWord = malloc(6*sizeof(char));
  char * hiddenWord = malloc(6*sizeof(char));

  strcpy(guessWord, computerGuess);//copy guess word
  strcpy(hiddenWord, secretWord);//copy secret word

  int letterFound = false;//boolean variable to check if letters were found 
  int repeatedLetter = false;//boolean variable to check if this letter we already used

  //First pass to check same position and same letter.
  for(int i = 0; i < WORD_LENGTH; i++)
  {
    if(guessWord[i] == hiddenWord[i])//Same letters and same position
    {
      //If letters on word are in the right position then add those letter to our special array for comparing later
      finalGuess[i] = guessWord[i];

      //blank out both letters from both words so we don't compare twice
      guessWord[i] = ' ';
      hiddenWord[i] = ' ';
    }
  }

  //Second pass compares one letter at time to all other letters on answer to see if it is there 
  for(int i = 0; i< WORD_LENGTH; i++)
  {
    char c = guessWord[i];
    
    //We compare letter to all the letters in array of letters that are present in hidden word to check if it is repeated. If it is assign true to repeatedLetter
    for(int k = 0; k < *InWordCount;k++)
    {
      //Compare the letters to the found array letters so we dont put repeated letters
      if(c == lettersInWord[k] )
      {
        repeatedLetter = true;
      }
    }

    //We compare letter to all the letters in array of letters that are not present in hidden word to check if it is repeated. If it is assign true to repeatedLetter
    for(int k = 0; k < *NotInWordCount;k++)
    {
      //Compare the letter to the already not found array letters so we dont put repeated letters
      if(c == lettersNotInWord[k] )
      {
        repeatedLetter = true;
      }
    }

    //We compare letter to all the letters in array of finalguess which contains letters that matched perfectly with hidden words. That way we do not store repeated words.
    for(int k = 0; k < WORD_LENGTH;k++)
    {
      //Compare the letter to the already not found array letters so we dont put repeated letters.
      if(c == finalGuess[k] )
      {
        repeatedLetter = true;
      }
    }

    //After checking if the letters are repeated then check if letter is present in the word ans store the letter in their respective array.
    // Only consider non-blanks and non repeated letters.
    if(c != ' ' && repeatedLetter == false) 
    {
      for(int j = 0; j< WORD_LENGTH; j++)
      {
        if(c == hiddenWord[j])//if letter in guess word is found in hidden word then.
        {
          lettersInWord[*InWordCount] = c;
          (*InWordCount)++;
          
          //blank out both letters from both words so we don't compare twice.
          guessWord[i] = ' ';
          hiddenWord[j] = ' ';
          letterFound = true;//make boolean variable true if we found any letter that is in hidden word.
        }
      }
    } 
    
    //If letter is not repeated, letter is not blank, and letter is not found then store in the respective array.
    if(letterFound == false && repeatedLetter == false && c != ' ')
    {
      lettersNotInWord[*NotInWordCount] = guessWord[i];//store letter in array if it was not found on word.
      (*NotInWordCount)++;
    }
 
    letterFound = false;//Reset boolean variable to their default.
    repeatedLetter = false;//Reset boolean variable ot their default.
  }

  //Free dynamically allocated c strings
  free(guessWord);
  free(hiddenWord);
}
// -----------------------------------------------------------------------------------------
//wordScore function takes guess words and 3 char array to compar. These char arrays contains letter not in word, letter in word, and letters with perfect match.It gives 3 points for same letter same position and 1 point for same letter in word but wrong position. It also uses letters not in word to get rid of words containing these.
int wordScore(char wordToCompare[], char finalGuess[], char lettersInWord[], int InWordCount)//FIX ME SCORING NOT WORKING PROPERLY
{
  int score = 0;// score of word

  //we make copies of the words we want to compare
  char * compareWord = malloc(6*sizeof(char));
  char * sameLetters = malloc(6*sizeof(char));

  strcpy(compareWord, wordToCompare);//copy word we want to compare
  strcpy(sameLetters, lettersInWord);//copy of letters in word array.

  //First pass to check same position and same letter.
  for(int i = 0; i< WORD_LENGTH; i++)
  {
    if(compareWord[i] == finalGuess[i])
    {
      score = score + 3;//3 points for same character in the same position.
      //blank out letters from compare word so we don't give extra points.
      compareWord[i] = ' ';
    }
  }
  
  //Second pass to score 1 point if letters are present in guess word but not in right position. Use lettersInWord array
  for(int i = 0; i< WORD_LENGTH; i++)
  {
    char c = compareWord[i];
    if(c != ' ')//Only consider non blank letters
    {
      for(int j = 0; j< InWordCount; j++)
      {
        if(c == sameLetters[j])
        {
          score = score + 1;//1 point for letter present in word
          //blank out letters from compare word and same letter word so we don't give extra points.
          compareWord[i] = ' ';
          sameLetters[i] = ' ';
        }
      }
    }
  } 

  //Free dynamically allocated c strings/ c arrays
  free(compareWord);
  free(sameLetters);

  return score;//return total score for one word
}
// -----------------------------------------------------------------------------------------
//This function takes all word array along with it size and our three char arrays and their counts to pass it to wordScore function so we can score all words and pick the best possible next guess. 
char * findNewGuessWord(wordCountStruct * pWordArray, int sizeOfTotalWords, char lettersNotInWord[], char lettersInWord[], char finalGuess[], int InWordCount, int NotInWordCount )
{
 
  int score = 0;//score
  
  //Allocate space for our best possible next guess.
  char * newGuess = malloc(6*sizeof(char));

  int matchedLetterFound = false;//boolean variable
  
  //Check if word contains letters that are not in the hidden word and automatically make the score -1 for these.
  for(int i = 0; i < sizeOfTotalWords; i++)// size of total word array
  {
    //only proceed if final guess contains any matched letters  
    if(strcmp(finalGuess,"     ") != 0)
    {
      //compare letter by letter of word and if there is a least one matched letter turn boolean variable to true
      for(int j = 0; j< 5; j++)
      {
        if(pWordArray[i].word[j] == finalGuess[j])
        {
          matchedLetterFound = true;
        }
      }
      
      //If matched letter not found on word then score is -1 since we only want to compare words that have matched same letters in same position
      if(matchedLetterFound == false)
      {
        pWordArray[i].score = -1;
      }

      matchedLetterFound = false;//reset boolean variable for next comparison
    }
    
    for(int j = 0; j< WORD_LENGTH; j++)//compare letter by letter of word
    {
      for(int k = 0; k< NotInWordCount; k++)//compare each letter to array of letter not in word
      {
        //if any letters that are not in the hidden word show up then make score for that word -1 so we do not use it for comparisons
        if(pWordArray[i].word[j] == lettersNotInWord[k])
        {
          pWordArray[i].score = -1;
        }
        
      }//end of looping through lettersNotOnWord array
    }//end of looping through comparison word
  }//end of looping through all words in array

  //We sort scores in descending order so all -1 values are at the bottom
  qsort(pWordArray, sizeOfTotalWords, sizeof(wordCountStruct), compareFunction);
 
  //get new size array
  int newArraySize = 0;
  for(int i = 0; i < sizeOfTotalWords; i++)
  {
    //At the first instance of score -1 we cut the amount of times we loop through array by assigning a new size with only potential good guesses
    if(pWordArray[i].score == -1)
    {
      newArraySize = i;
      break;
    }
  }//end of looping through all important words in array

  printf("\n\n");
  //If the score of word is -1 it means it contains letters that are not in the hidden word so disregard them and dont even compare them. Then, proceed to get a score for only important words
  for(int i = 0; i < newArraySize; i++)//loop through only important words with newArraySize
  {
    score = wordScore(pWordArray[i].word, finalGuess, lettersInWord, InWordCount);//Get score for word
    pWordArray[i].score = score;//get score from wordScore function and assign it to word in array
    
  }//end of looping through all important words in array

  //We sort the new scores in descending order leaving the best possible next guess at the top for next check. 
  qsort(pWordArray, sizeOfTotalWords, sizeof(wordCountStruct), compareFunction);

  //Get best possible next guess;
  strcpy(newGuess,pWordArray[0].word);

  //After we guess this word we set score to -1 so we do not guess it again.
  pWordArray[0].score = -1;
  
  return newGuess;//return new best possible guess
}//end of findnewGuess function.
// -----------------------------------------------------------------------------------------
// Find a secret word
void findSecretWord(
        wordCountStruct allWords[],    // Array of all the words
        int wordCount,                  // How many words there are in allWords
        char secretWord[])              // The word to be guessed
{
  char computerGuess[6];  // Allocate space for the computer guess
  
  char lettersNotInWord[27];//Allocate array of letters that are not in the secret word, size 27 to possibly cover all letters in the alphabet.
  lettersNotInWord[26] = '\0';//initialize last value with the terminated character;
  char lettersInWord[6] = "     ";//Allocate array of letters that are in the secret word
  char finalGuess[6] = "     ";//Initialize the final guess to blank and add letters on the right spot until it matches the answer

  //counters for letters on word and not on word arrays.
  int NotInWordCount = 0;
  int InWordCount = 0;
  
  printf("Trying to find secret word: \n");
  // Display secret word with a space between letters, to match the guess words below.
  printf("       ");
  for( int i=0; i<WORD_LENGTH; i++) {
      printf("%c ", secretWord[ i]);
  }
  printf("\n");
  printf("\n");

  //Find best starting words by calculating best first word from array with the one with the most common letters
  bestFirstWordCalculator(allWords, wordCount);//Calculate scores for all words 
  qsort(allWords, wordCount, sizeof(wordCountStruct), compareFunction);//6. We sort scores in descending order.

  //Assign best first word to start guessing by picking the word with the most common letters.
  strcpy(computerGuess,allWords[0].word);

  //Reset scoring for further comparisons
  for(int i = 0; i < wordCount; i++)
  {
    allWords[i].score = 0;
  }
  
  // Loop until the word is found
  int guessNumber = 1;
  
  int wordFound = false;// Boolean variable to stay in loop until we find the hidden word
  char * nextGuessWord = NULL;//Next guess word

  //Start the loop to find hidden word and loop through until we find it.
  while( wordFound == false ) 
  {
    //If first guess equals hidden word then print it and end loop
    if(strcmp(computerGuess,secretWord) == 0)
    {
      //Display guess word with apropriate interface
      printf("%5d.", guessNumber);
      displayWords(computerGuess,secretWord);
      break;
    }

    //Display guess word with apropriate interface
    printf("%5d.", guessNumber);
    displayWords(computerGuess,secretWord);
    
    //compare both words and get letters on word, letters not on word, and final guess which contains letters that matched perfectly to find next guess
    wordCompare(secretWord, computerGuess, lettersNotInWord, lettersInWord, finalGuess, &InWordCount, &NotInWordCount);

    //Use arrays created by wordCompare function to pick the best next computer guess to get answer eventually
    nextGuessWord = findNewGuessWord(allWords, wordCount, lettersNotInWord, lettersInWord, finalGuess, InWordCount, NotInWordCount);

    //Assign new computer guess.
    strcpy(computerGuess, nextGuessWord);

    //free allocated c-string guess
    free(nextGuessWord);
    nextGuessWord = NULL;//reset pointer for next word
    
    // Update guess number.
    guessNumber++;

    if(strcmp(computerGuess,secretWord) == 0)//If guess word equals hidden word, make boolean variable true to exit loop
    {
      wordFound = true;
      //Display guess word with apropriate interface
      printf("%5d.", guessNumber);
      displayWords(computerGuess,secretWord);
    }
    
    //reset c-Strings and their count
    strcpy(lettersInWord,"     "); 
    InWordCount = 0;
  } //end for( int i...)
} //end findSecretWord
// Find a secret word
// -----------------------------------------------------------------------------------------
int main(){
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
  char userInput[81];                // Used for menu input of secret word
  
  // Read in words from file, update wordCount and display information
  readWordsFromFile( wordsFileName, allWords, &wordCount);
  printf("Using file %s with %d words. \n", wordsFileName, wordCount);

  // Run the word-guessing game three times
  for( int i=0; i<3; i++) 
  {
    // Reset secret Word
    strcpy( secretWord, "");
    // Display prompt
    printf("-----------------------------------------------------------\n");
    printf("\n");
    printf("Enter a secret word or just r to choose one at random: ");
    scanf(" %s", userInput);
    // Eliminate the return character at end or userInput if it is there
    int length = (int) strlen( userInput);
    if( userInput[ length] == '\n')
    {
      userInput[ length] = '\0';
    }
    strcpy( secretWord, userInput);   // Store the secret word from user input
            
    // If input was 'r' then choose a word at random.
    if( strlen( secretWord) <= 1) {
      // Randomly select a secret word to be guessed.
      int randomIndex = rand() % wordCount;
      strcpy( secretWord, allWords[randomIndex].word);
    }
    
    // Run the game once with the current secret word
    findSecretWord(allWords, wordCount, secretWord);//UNCOMMENT ME WHEN DONE

    printf("Got it!\n");
  }//end of for loop 3 tries
  
  printf("Done\n");
  printf("\n");
  return 0;
} // end main()