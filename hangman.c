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

// function declarations
void validate_arguments(int argc, char* argv[]);
void print_prompt();
int fileno(FILE *stream);
char* scan_words(char* argv[]);
int* print_info(char* word_chosen);
bool player_guess(char* word_chosen, int* p, bool solved);

int main(int argc, char* argv[]) {
	validate_arguments(argc, argv);
	print_prompt();
	char* word_chosen = scan_words(argv);
	printf("word chosen is %s\n", word_chosen);

	int* p = print_info(word_chosen);
	bool solved = false;
	while (!solved) {
		solved = player_guess(word_chosen, p, solved);
	}
	
	return 0;
}


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


void print_prompt() {
	//if (isatty(fileno(stdin))) {
	//	printf("New Hangman Game!\n");
//	}
}


char* scan_words(char* argv[]) {
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
		word_array[num_words] = word_copy;
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

int* print_info(char* word_chosen) {
	int word_length = strlen(word_chosen);
	printf("\nWord: ");
	for (int i = 0; i < word_length; i++) {
		printf("_ "); 
	}
	printf("\n");

	int* guess_array = malloc(100*sizeof(int));
	for (int j = 0; j < word_length; j++) {
		guess_array[j] = 0;
		//printf("%d\n", guess_array[j]);
	}

	return guess_array;
}

bool player_guess(char* word_chosen, int* p, bool solved) {
	char character[100];
	printf("Enter your guess: ");
	fscanf(stdin, "%s", character);
	
	for (int i = 0; i < strlen(word_chosen); i++) {
		if (character[0] == word_chosen[i]) {
			p[i] = 1; // change from 0 to 1
		}
	}
	
	printf("\nWord: ");
	for (int b = 0; b < strlen(word_chosen); b++) {
		 if (p[b] == 0) {
			  printf("_ ");
		 }
		 else {
		 	printf("%c ", word_chosen[b]);
		 }
	}
	printf("\n");

	for (int j = 0; j < strlen(word_chosen); j++) {
		if (p[j] == 0) {
			solved = false;
			return solved;
		}
	}
	solved = true;
	return solved;
}
