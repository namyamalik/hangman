## Hangman README.md
## Author: Namya Malik

### To-Do
* valgrind warning
* defensive coding: parameter null checking, use assertp?
* comment and clean up code

### Compilation
`make` compiles the `hangman` executable

### Usage
* Executes from a command line with usage syntax: `./hangman filename`
* `make test` runs this above command and starts the game
* `make clean` removes object files and the executable
* `make valgrind` tests for memory leaks

### Game Rules
* Single player game
* Goal is for the player to guess a word that has been randomly chosen from a list of words
* Player must guess a single letter on each turn
* Player has 5 "lives" (this is the number of incorrect guesses allowed)
* Player can type "quit" to exit the game at any point

### Assumptions
* `filename` contains words with alphabetic characters only (since user can only guess alphabetic letters)
* Each word in `filename` is less than 200 characters long
* There are less than 1000 total words to scan in `filename`
* User inputs a guess that is less than 200 characters long

### Non-Assumptions
* `filename` may be formatted in different ways as long as each word is separated by a whitespace character
* `filename` words may be lowercase or uppercase alphabets, the program will treat them as identical
* User may type in lowercase or uppercase alphabets, the program will treat them as identical

### Exit Status
0 - success  
1 - incorrect number of command-line arguments provided  
2 - `filename` is not readable  
3 -  error allocating memory  

### Potential Next Steps
* Graphics
