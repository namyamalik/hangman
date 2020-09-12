## Hangman README.md
## Author: Namya Malik

### To-Do
* quit game, normalize "quit"
* memory leaks
* comment and clean up code
* testing and readme and implementation/design

### Compilation
`make` will compile the `hangman` executable

### Playing the Game
`make test` will start the game

### Rules
* Single player game
* Goal is for the player to guess a word that has been randomly chosen from a list of words
* Player must guess a single letter on each turn
* Player has 5 "lives" (number of incorrect guesses allowed)
* Player can type "quit" to exit the game at any point

### Exit Status
0 - success
1 -
2 - 
3 -

### Assumptions
* Text file has one word per line
* Text file contains words with alphabetic characters only
* Each word in the text file is less than 200 characters long
* There are less than 1000 words to scan in the text file (aka 1000 lines since there is 1 word per line)

* User inputs a guess that is less than 200 characters long

### Non-Assumptions
* User may type in lowercase or uppercase alphabets, the program will treat them as identical

### Potential Next Steps
* Graphics
