/*
 * hangman.c
 * 
 * Author: Namya Malik
 * 
 * September 2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

/**************** constants ****************/
#define MAX_LIVES 5


/**************** local functions ****************/
void hangman(int argc, char* argv[]);
void validate_arguments(int argc, char* argv[]);
char* choose_word(char* argv[]);
int* print_before_guess(char* word_chosen);
void player_guess(char* word_chosen, int* p, bool* solved, int* num_incorrect_guesses);
bool validate_user_guess(char character[], bool *solved);
void print_word_filled(char* word_chosen, int* p);
void check_solved(char* word_chosen, int* p, bool* solved);


/**************** main ****************/
/*
 * Validates command-line arguments and calls the primary hangman fxn
 *
 * Returns zero upon successful completion of program
 */
int main(int argc, char* argv[]) {
	validate_arguments(argc, argv); // check command-line arguments before game begins
	hangman(argc, argv);
	return 0;
}	


/**************** hangman ****************/
/*
 * Primary Hangman function that calls several other functions to play the game
 *
 * Void return
 */
void hangman(int argc, char* argv[]) {
	char* word_chosen = choose_word(argv); // randomly choose word from txt file
	#ifdef UNITTESTING
		printf("word chosen is %s\n", word_chosen);	
	#endif

	int* p = print_before_guess(word_chosen); // print dashes & return array of zeros corresponding to length word chosen
	
	bool solved = false;
	int num_incorrect_guesses = 0;
	
	// let user guess again while word remains unsolved and lives remain
	while (!solved && num_incorrect_guesses < MAX_LIVES) {
		player_guess(word_chosen, p, &solved, &num_incorrect_guesses);
	}

	#ifdef UNITTESTING
        printf("game finished\n");
    #endif
	
	free(word_chosen);
	free(p);
}


/**************** validate_arguments ****************/
/* 
 * Checks the number of arguments and if the file is readable
 *
 * Void return
 */
void validate_arguments(int argc, char* argv[]) {
	
	// if user provides incorrect number of  arguments
    if (argc != 2) {
        fprintf(stderr, "Error: incorrect number of arguments provided - please provide 1 argument for correct usage\n");
        exit(1);
    }
	
	// if file containing words is unreadable
	FILE* fp;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: filename provided is not a readable file\n");
        exit(2);
    }
    fclose(fp);
}


/**************** choose_word ****************/
/* 
 * Scans the words of a text file into an array and then randomly chooses one of those words
 *
 * Returns the word chosen
 */
char* choose_word(char* argv[]) {
	char word[200];
	char* word_array[1000];
	int num_words = 0;

	// open file and scan for each word
	FILE* fp = fopen(argv[1], "r");
	while (fscanf(fp, "%s", word) != EOF) {
		word_array[num_words] = malloc(strlen(word) + 1);
		if (word_array[num_words] == NULL) {
    		fprintf(stderr, "Error allocating memory");
    		exit(3);
    	}
		strcpy(word_array[num_words], word); // add each word to an array
		num_words++;
	}
	fclose(fp);
	
	// choose random word from array
	srand(time(NULL)); // so that different index generated each time
	int random_index = rand() % num_words; // index 0-11
	char* word_chosen = word_array[random_index];
	
	// copy word_chosen into a separate variable so it doesn't get lost when word_array slots are freed
	char* word_chosen_copy = malloc(strlen(word_chosen) + 1);
	if (word_chosen_copy == NULL) {
		fprintf(stderr, "Error allocating memory");
        exit(3);	
	}	
	strcpy(word_chosen_copy, word_chosen);

	// free slots of word_array
	for (int i = 0; i < num_words; i++) {
        if (word_array[i] != NULL) {
			free(word_array[i]);
		}	
    }

	return word_chosen_copy; // this will get freed at the end of program
}


/**************** print_before_guess ****************/
/*
 * Prints out a dash (in Hangman style) for each letter of the chosen word
 *
 * Returns an array of zeros. Length of array is the length of the word chosen (each zero corresponds to a letter of the chosen word)
 */
int* print_before_guess(char* word_chosen) {
	// print word but with all the letters replaced with dashes
	printf("\nWord: ");
	for (int i = 0; i < strlen(word_chosen); i++) {
		printf("_ "); 
	}
	printf("\n");

	// create array of zeros with a length to match the length of chosen word
	int* guess_array = malloc((strlen(word_chosen) + 1) * sizeof(int));
	for (int j = 0; j < strlen(word_chosen); j++) {
		guess_array[j] = 0;
	}

	return guess_array;
}


/**************** player_guess ****************/
/*
 * Lets user guess a letter and calls a fxn to validate the user's guess. Changes the corresponding element in the array of zeros to a one if user's guess was correct and prints out correct/incorrect/lives statements. Calls a fxn to print out the updated word (with letters and dashes). Calls a fxn to check if word has been solved.
 *
 * Void return
 */
void player_guess(char* word_chosen, int* p, bool* solved, int* num_incorrect_guesses) {
	char character[200] = " ";

	char letters_guessed_array[500];
	//char letters_guessed_array[*num_incorrect_guesses + 1];
	//char* letters_guessed_array = malloc(*num_incorrect_guesses * sizeof(char));

	// let user enter a guess and read from stdin
	printf("Enter your guess: ");
	if (fscanf(stdin, "%s", character) == EOF) {
		clearerr(stdin);
		printf("\n\n");
		return;
	}

	// if user's entry is valid then proceed
	if (validate_user_guess(character, solved)) {

		bool changed = false;
		bool already = false;
		bool incorrect_again = false;

		// check if user's guess matches a letter in the chosen word
		for (int i = 0; i < strlen(word_chosen); i++) {
			if (character[0] == word_chosen[i]) { // compare characters
				if (p[i] == 0) { // change corresponding array element from 0 to 1 to denote that letter has been guessed
					p[i] = 1;
					changed = true;
				}
				else if (p[i] == 1) {
					already = true;
				}
			}
		}
		
		// print correct/incorrect/lives statements
        if (changed) {
            printf("Correct guess\n");
            printf("Lives remaining: %d\n", MAX_LIVES - *num_incorrect_guesses);
        }
        else if (already) {
            printf("This letter has already been filled in\n");
            printf("Lives remaining: %d\n", MAX_LIVES - *num_incorrect_guesses);
        }
        else {
            for (int z = 0; z < strlen(letters_guessed_array); z++) {
                //printf("%ld\n", strlen(letters_guessed_array));
				if (letters_guessed_array[z] == character[0]) {
                    printf("You already guessed this incorrect letter\n");
                    printf("Lives remaining: %d\n", MAX_LIVES - *num_incorrect_guesses);
                    incorrect_again = true;
                    break;
                }
            }
            if (!incorrect_again) {
                printf("Incorrect guess\n");
				//printf("%c\n", letters_guessed_array[*num_incorrect_guesses]); 
                letters_guessed_array[*num_incorrect_guesses] = character[0];
                (*num_incorrect_guesses)++;
                printf("Lives remaining: %d\n", MAX_LIVES - *num_incorrect_guesses);
            }
        }
		
		// if lives remain then print updated word and check if it has been solved
		if (*num_incorrect_guesses < MAX_LIVES) {
			print_word_filled(word_chosen, p);
			check_solved(word_chosen, p, solved);
			return;
		}
		printf("\nGame Over. You ran out of lives. The word was '%s'.\n\n", word_chosen); // no lives remain
	}
}


/**************** print_word_filled ****************/
/*
 * Prints the chosen word with dashes/letters after user's guess (replaces a dash with a letter upon a successful guess)
 *
 * Void return
 */
void print_word_filled(char* word_chosen, int* p) {
		printf("\nWord: ");
		for (int b = 0; b < strlen(word_chosen); b++) {
			// print a dash if letter has not been guessed 
			if (p[b] == 0) {
				printf("_ ");
			}
			// print the letter if it has been guessed
			else {
				printf("%c ", word_chosen[b]);
			}
		}
		printf("\n");
}


/**************** check_solved ****************/
/*
 * Checks whether user has guessed all the letters in the chosen word
 *
 * Void return
 */
void check_solved(char* word_chosen, int* p, bool* solved) {
	for (int j = 0; j < strlen(word_chosen); j++) {
		// if array contains zero then word has not been solved
		if (p[j] == 0) {
			*solved = false;
			return;
		}
	}
	// if array contains no zeros then word has been solved
	*solved = true;
	printf("\nSuccess! You guessed the word!\n\n");
}


/**************** validate_user_guess ****************/
/*
 * Normalizes user's entry to all lowercase. Checks whether user provided a single alphabetic character as their guess. Also checks whether user typed 'quit' and ends the game accordingly.
 *
 * Returns a boolean whether user provided valid guess
 */
bool validate_user_guess(char character[], bool* solved) {
	
	// normalize the entry to lowercase (in case user provided uppercase letter)
    for (int i = 0; i < strlen(character); i++) {
		if (character[i] >= 65 && character[i] <= 90) {
            character[i] = character[i] + 32;
    	}
	}	

	// check length of user's guess
	if (strlen(character) > 1) {
		// end game if user typed quit
		if (strcmp(character, "quit") == 0) {
			*solved = true;
			printf("\nGame ended. You quit the game.\n\n");
			return false;
		}
		printf("Please provide a single alphabetic letter as your guess\n\n");
        return false;
    }

	// if length is 1 then check if it is alphabetic
    else {
        if (isalpha(character[0]) == 0) {
        	printf("Please provide a single alphabetic letter as your guess\n\n");
          	return false;
       	}
    }
	return true;
}
