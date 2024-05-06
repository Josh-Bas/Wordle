#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Each of our text files contains 1000 words
#define LISTSIZE 1000

// Values for color and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET "\e[0;39m"


// User-defined function prototypes
char *get_guess(int wordsize);
int check_word(const char *guess, int wordsize, int status[], const char *choice);
void print_word(const char *guess, int wordsize, const int status[]);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }

    int wordsize = atoi(argv[1]);

    // Ensure argv[1] is either 5, 6, 7, or 8 and store that value in wordsize instead
    if (wordsize < 5 || wordsize > 8)
    {
        printf("Error: wordsize must be 5, 6, 7, or 8\n");
        return 1;
    }

    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    char options[LISTSIZE][wordsize + 1];

    // Load word file into an array of size LISTSIZE
    for (int i = 0; i < LISTSIZE; i++)
    {
        if (fscanf(wordlist, "%s", options[i]) != 1)
        {
            printf("Error reading file %s.\n", wl_filename);
            fclose(wordlist);
            return 1;
        }
    }
    fclose(wordlist);

    srand(time(NULL));
    const char *choice = options[rand() % LISTSIZE];

    int guesses = wordsize + 1;
    int won = 0;

    printf(GREEN "This is WORDLE" RESET "\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // Main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // Obtain user's guess
        char *guess = get_guess(wordsize);

        // Array to hold guess status, initially set to zero
        int status[wordsize];
        memset(status, WRONG, sizeof(status));

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        printf("Guess %i: ", i + 1);
        // Print the guess
        print_word(guess, wordsize, status);

        // If they guessed it exactly right, terminate loop
        if (score == EXACT * wordsize)
        {
            won = 1;
            free(guess);
            break;
        }
        free(guess);
    }

    // Print the game's result
    if (won == 1)
    {
        printf("You won!\n");
    }
    else
    {
        printf("The word was: %s\n", choice);
    }
    return 0;
}

// Function to get user's guess
char *get_guess(int wordsize)
{
    char *guess = malloc(wordsize + 1);

    // Ensure users actually provide a guess that is the correct length
    if (guess == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    do
    {
        printf("Input a %i-letter word: ", wordsize);
        scanf("%s", guess);
    } while (strlen(guess) != wordsize);

    return guess;
}

// Function to check the user's guess against the chosen word
int check_word(const char *guess, int wordsize, int status[], const char *choice)
{
    int score = 0;

    // Compare guess to choice and score points as appropriate, storing points in status
    for (int i = 0; i < wordsize; i++)
    {
        for (int j = 0; j < wordsize; j++)
        {
            if (guess[i] == choice[j] && i == j)
            {
                score += EXACT;
                status[i] = EXACT;
                break;
            }
            else if (guess[i] == choice[j])
            {
                score += CLOSE;
                status[i] = CLOSE;
            }
        }
    }
    return score;
}

// Function to print the user's guess with correct color coding
void print_word(const char *guess, int wordsize, const int status[])
{
    // Print word character-for-character with correct color coding, then reset terminal font to normal
    for (int i = 0; i < wordsize; i++)
    {
        if (status[i] == EXACT)
        {
            printf(GREEN "%c" RESET, guess[i]);
        }
        else if (status[i] == CLOSE)
        {
            printf(YELLOW "%c" RESET, guess[i]);
        }
        else if (status[i] == WRONG)
        {
            printf(RED "%c" RESET, guess[i]);
        }
    }

    printf("\n");
}
