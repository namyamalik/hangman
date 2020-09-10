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

/**************** local functions ****************/
void validate_arguments(int argc, char* argv[]);
char* choose_word(char* argv[]);
int* length_info(char* word_chosen);
bool player_guess(char* word_chosen, int* p, bool solved);
bool validate_user_guess(char character[]);

/**************** main ****************/
int main(int argc, char* argv[]) {
	
	validate_arguments(argc, argv); // check arguments
	char* word_chosen = choose_word(argv); // randomly choose word from txt file
	printf("word chosen is %s\n", word_chosen);

	int* p = length_info(word_chosen); // print dashes & return array of zeros corresponding to length word chosen
	bool solved = false;
	while (!solved) { // while word remains unsolved, let player guess again
		solved = player_guess(word_chosen, p, solved);
	}
	
	return 0;
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
 * Scans the words of a text file into an array and then randomly chooses one of
 * those words
 *
 * Returns the word chosen
 */
char* choose_word(char* argv[]) {
	char* word = malloc(200 * sizeof(char));
	if (word == NULL) {
		fprintf(stderr, "Error allocating memory");
		exit(3);
	}
	char** word_array = malloc(100 * sizeof(char*));
	if (word_array == NULL) {
        fprintf(stderr, "Error allocating memory");
        free(word);
		exit(3);
    }
	int num_words = 0;
	char* word_copy = NULL;	

	// open file and scan for each word
	FILE* fp = fopen(argv[1], "r");
	while (fscanf(fp, "%s", word) == 1) {
		word_copy = malloc(strlen(word) * sizeof(char));
		if (word_copy == NULL) {
            fprintf(stderr, "Error allocating memory");
            free(word);
            free(word_array);
            exit(3);
        }

		strcpy(word_copy, word);
		word_array[num_words] = word_copy; // add each word to an array
		//printf("%d %s\n", num_words, word_array[num_words]);
		num_words++;
		//if (word_copy != NULL) {
        //	free(word_copy);
    	//}
		//printf("%d %s\n", num_words, word_copy);
		
		//free(word_copy);
	}
	fclose(fp);

	//for (int i = 0; i < num_words; i++) {
    //        printf("%d %s\n", i, word_array[i]);
   // }

	// choose random word from array
	srand(time(NULL)); // so that different index generated each time
	int random_index = rand() % num_words; // index 0-11
	
	char* word_chosen = word_array[random_index];
	if (word != NULL) {
		free(word);
	}
	if (word_array != NULL) {
		free(word_array);
	}
	    // free(word_copy);
	return word_chosen;
}


/**************** length_info ****************/
/*
 * Prints out a dash (in Hangman style) for each letter of the chosen word
 *
 * Returns an array of zeros. Length of array is the length of the word chosen ( * each zero corresponds to a letter of the chosen word)
 */
int* length_info(char* word_chosen) {
	// print word but with all the letters replaced with dashes
	printf("\nWord: ");
	for (int i = 0; i < strlen(word_chosen); i++) {
		printf("_ "); 
	}
	printf("\n");

	// create array of zeros with a length to match the length of chosen word
	int* guess_array = malloc(100*sizeof(int));
	for (int j = 0; j < strlen(word_chosen); j++) {
		guess_array[j] = 0;
		//printf("%d\n", guess_array[j]);
	}

	return guess_array;
}


/**************** player_guess ****************/
/*
 * Lets user guess a letter. Changes the corresponding element in the array of zeros to a one if user's guess was correct. Then prints out the dashes again but fills in a letter if the user's guess was correct.
 *
 * Returns a boolean whether word has been solved (if array is filled with all ones and no zeros)
 */
bool player_guess(char* word_chosen, int* p, bool solved) {
	char character[100];
	
	// let user enter a guess and read from stdin
	printf("Enter your guess: ");
	fscanf(stdin, "%s", character);

	// validate user's guess
	if (validate_user_guess(character)) {
	
		bool changed = false;
		bool already = false;

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

		// print correct/incorrect statments
		if (changed) {
			printf("Correct guess\n");
		}
		else if (already) {
			printf("This letter has already been filled in\n");
		}
		else {
			printf("Incorrect guess\n");
		}

	}	
	
	// print the word with dashes/letters after the user's guess
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

	// check if the word has been solved or not
	for (int j = 0; j < strlen(word_chosen); j++) {
		// if array contains zero then word has not been solved
		if (p[j] == 0) {
			solved = false;
			return solved;
		}
	}
	solved = true;
	return solved;
}

/**************** validate_user_guess ****************/
/*
 * Checks whether user provided a single alphabetic character as their guess
 *
 * Returns a boolean whether user provided valid guess
 */
bool validate_user_guess(char character[]) {
	// check length of user's guess
	if (strlen(character) > 1) {
        printf("Please provide a single alphabetic letter as your guess\n");
        return false;
    }
	// if length is 1 then check if it is alphabetic
    else {
        if (isalpha(character[0]) == 0) {
        	printf("Please provide a single alphabetic letter as your guess\n");
          	return false;
       	}
    }
    return true;
}
