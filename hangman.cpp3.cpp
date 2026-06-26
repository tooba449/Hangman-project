#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cctype>
#include <fstream>
#include <limits>

using namespace std;

// ============================================================
//   STRUCT — groups all player data in one place
// ============================================================
struct PlayerScore {
    char name[50];
    int wins;
    int losses;
    int totalGames;
};

// ============================================================
//   ANIMATION — typing effect (uses ctime for delay)
// ============================================================
void typePrint(const char* text, int delayMs = 40) {
    for (int i = 0; text[i] != '\0'; i++) {
        cout << text[i] << flush;
        // Busy-wait delay using clock() from <ctime>
        clock_t start = clock();
        while ((clock() - start) < (CLOCKS_PER_SEC * delayMs / 1000));
    }
    cout << "\n";
}

void showIntro() {
    cout << "\n\n";
    typePrint("  Welcome to Hangman Arcade...");
    typePrint("  Somewhere, a word is waiting to be found...");
    typePrint("  Can you guess it before the man hangs?");
    typePrint("  Every wrong letter brings him closer to the end...");
    typePrint("  6 lives. One word. No second chances.");
    typePrint("  The clock is ticking... the rope is ready...");
    typePrint("  Step forward if you are brave enough...");
    typePrint("  Let the game begin!");
    cout << "\n";
}

// ============================================================
//   VARIABLES — used throughout the program
// ============================================================
// int, char, bool, const char* — all demonstrated in functions below

// ============================================================
//   CLEAR SCREEN (cross-platform)
// ============================================================
void clearScreen() {
    cout << "\n\n----------------------------------\n\n";
}

// ============================================================
//   PAUSE SCREEN
// ============================================================
void pauseScreen() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ============================================================
//   PASS BY VALUE — demonstrates concept
//   Takes a copy of triesLeft — original not changed
// ============================================================
void showLivesStatus(int triesLeft) {   // pass by value
    // NESTED IF — checks lives status with multiple conditions
    if (triesLeft == 6) {
        cout << "[Status: SAFE — No wrong guesses yet!]\n";
    } else if (triesLeft >= 4) {
        // nested if inside else-if
        if (triesLeft == 5) {
            cout << "[Status: CAREFUL — 1 wrong guess!]\n";
        } else {
            cout << "[Status: WARNING — 2 wrong guesses!]\n";
        }
    } else if (triesLeft >= 2) {
        // nested if inside else-if
        if (triesLeft == 3) {
            cout << "[Status: DANGER — Halfway there!]\n";
        } else {
            cout << "[Status: CRITICAL — Only 2 lives left!]\n";
        }
    } else {
        // nested if inside else
        if (triesLeft == 1) {
            cout << "[Status: LAST CHANCE — 1 life remaining!]\n";
        } else {
            cout << "[Status: GAME OVER!]\n";
        }
    }
}

// ============================================================
//   FILE HANDLING — Save Score (ofstream)
//   PASS BY REFERENCE via pointer
// ============================================================
void saveScore(const PlayerScore* ps) {   // pass by pointer (reference)
    ofstream outFile("hangman_scores.txt", ios::app);
    if (outFile.is_open()) {
        outFile << ps->name << " " << ps->wins << " "
                << ps->losses << " " << ps->totalGames << "\n";
        outFile.close();
        cout << "Score saved successfully!\n";
    } else {
        cout << "Unable to open file for saving.\n";
    }
}

// ============================================================
//   FILE HANDLING — Display Scores (ifstream)
//   NON-PARAMETERIZED, NON-RETURNING function
// ============================================================
void displaySavedScores() {
    ifstream inFile("hangman_scores.txt");
    if (!inFile.is_open()) {
        cout << "No saved scores found.\n";
        return;
    }

    cout << "\n========== LEADERBOARD ==========\n";
    cout << "Name        Wins  Losses  Total\n";
    cout << "---------------------------------\n";

    char name[50];
    int wins, losses, total;
    bool hasData = false;

    while (inFile >> name >> wins >> losses >> total) {
        cout << name << "    " << wins << "     "
             << losses << "      " << total << "\n";
        hasData = true;
    }

    if (!hasData) {
        cout << "No records yet.\n";
    }

    cout << "=================================\n";
    inFile.close();
}

// ============================================================
//   HELPER — Display Word
//   PARAMETERIZED, NON-RETURNING function
//   Uses 1D ARRAY (bool guessed[])
// ============================================================
void displayWord(const char word[], const bool guessed[], int length) {
    cout << "\nWord: ";
    for (int i = 0; i < length; i++) {
        if (guessed[i]) {
            cout << word[i] << " ";
        } else {
            cout << "_ ";
        }
    }
    cout << "\n";
}

// ============================================================
//   HELPER — Check if Word Fully Guessed
//   PARAMETERIZED, RETURNING function (returns bool)
// ============================================================
bool isWordGuessed(const bool guessed[], int length) {
    for (int i = 0; i < length; i++) {
        if (!guessed[i]) return false;
    }
    return true;
}

// ============================================================
//   HELPER — Draw Hangman
//   Uses SWITCH STATEMENT (conditional)
// ============================================================
void drawHangman(int triesLeft) {
    cout << "\nLives left: " << triesLeft << "\n";
    switch (triesLeft) {
        case 6: cout << "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========\n"; break;
        case 5: cout << "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========\n"; break;
        case 4: cout << "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========\n"; break;
        case 3: cout << "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========\n"; break;
        case 2: cout << "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========\n"; break;
        case 1: cout << "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========\n"; break;
        case 0: cout << "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========\n"; break;
    }
}

// ============================================================
//   HELPER — Display Guessed Letters
//   Uses 1D ARRAY (char guessedLetters[])
// ============================================================
void displayGuessedLetters(const char guessedLetters[], int count) {
    cout << "Guessed letters: ";
    for (int i = 0; i < count; i++) {
        cout << guessedLetters[i] << " ";
    }
    cout << "\n";
}

// ============================================================
//   CORE GAME LOGIC — One Round
//   Uses DO-WHILE LOOP, 1D ARRAYS, IF/ELSE, VARIABLES
// ============================================================
bool playHangmanWithWord(const char word[], const char* category) {
    int length = (int)strlen(word);
    bool guessed[100] = {false};      // 1D bool array
    char guessedLetters[26];          // 1D char array
    int guessCount = 0;
    int triesLeft = 6;

    cout << "\n=============================\n";
    cout << "   Welcome to Hangman!\n";
    cout << "=============================\n";
    cout << "Category: " << category << "\n";
    cout << "The word has " << length << " letters.\n";

    // DO-WHILE LOOP — ensures at least one iteration
    do {
        drawHangman(triesLeft);
        showLivesStatus(triesLeft);    // pass by value — copy of triesLeft
        displayWord(word, guessed, length);
        if (guessCount > 0) displayGuessedLetters(guessedLetters, guessCount);

        char guess;
        cout << "\nEnter your guess (a letter): ";
        cin >> guess;
        guess = (char)tolower(guess);

        // IF — input validation
        if (!isalpha(guess)) {
            cout << "Please enter a valid letter!\n";
            continue;
        }

        bool alreadyGuessed = false;
        for (int i = 0; i < guessCount; i++) {
            if (guessedLetters[i] == guess) {
                alreadyGuessed = true;
                break;
            }
        }

        // IF-ELSE — duplicate check
        if (alreadyGuessed) {
            cout << "You already guessed '" << guess << "'! Try another letter.\n";
            continue;
        }

        guessedLetters[guessCount++] = guess;

        bool correctGuess = false;
        for (int i = 0; i < length; i++) {
            if (word[i] == guess && !guessed[i]) {
                guessed[i] = true;
                correctGuess = true;
            }
        }

        // IF-ELSE — correct or wrong guess
        if (correctGuess) {
            cout << "Correct! '" << guess << "' is in the word!\n";
        } else {
            cout << "Wrong! '" << guess << "' is not in the word.\n";
            triesLeft--;
        }

    } while (triesLeft > 0 && !isWordGuessed(guessed, length));

    // RETURNING function — returns true/false
    if (isWordGuessed(guessed, length)) {
        cout << "\nCongratulations! You guessed the word: " << word << "!\n";
        return true;
    } else {
        drawHangman(0);
        cout << "\nGame Over! The word was: " << word << "\n";
        return false;
    }
}

// ============================================================
//   MAIN HANGMAN FUNCTION
//   Uses 2D ARRAY for word lists, PASS BY POINTER
//   Uses CTIME for random seed
// ============================================================
void playHangman(PlayerScore* ps) {   // pass by pointer (reference)

    // 2D ARRAY — rows = categories, cols = words
    const char* wordList[3][6] = {
        {"apple",    "banana",   "grape",    "mango",    "lemon",   "peach"},     // Fruits
        {"tiger",    "elephant", "zebra",    "giraffe",  "rabbit",  "markhor"},   // Animals
        {"canada",   "brazil",   "germany",  "pakistan", "china",   "palestine"}  // Countries
    };

    const char* categoryNames[3] = {"Fruits", "Animals", "Countries"};

    // CTIME — seed random with current time
    srand((unsigned int)time(nullptr));

    char playAgain;

    // DO-WHILE LOOP — game session loop
    do {
        clearScreen();

        // VARIABLES — int for index
        int catIndex  = rand() % 3;
        int wordIndex = rand() % 6;

        // 2D ARRAY ACCESS
        const char* secretWord = wordList[catIndex][wordIndex];

        bool won = playHangmanWithWord(secretWord, categoryNames[catIndex]);

        // POINTER — update struct through pointer using ->
        ps->totalGames++;
        if (won) ps->wins++;
        else     ps->losses++;

        cout << "\n--- Session Stats for " << ps->name << " ---\n";
        cout << "Wins: "   << ps->wins
             << "  Losses: " << ps->losses
             << "  Total: "  << ps->totalGames << "\n";

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
        playAgain = (char)tolower(playAgain);

    } while (playAgain == 'y');
}

// ============================================================
//   FORWARD DECLARATION
// ============================================================
void showMenu();

// ============================================================
//   MAIN MENU — showMenu()
//   NON-RETURNING function, uses DO-WHILE LOOP, SWITCH
// ============================================================
void showMenu() {
    int choice = 0;

    // DO-WHILE — menu always shows at least once
    do {
        clearScreen();
        cout << "================================\n";
        cout << "   HANGMAN - ARCADE EDITION\n";
        cout << "================================\n";
        cout << "1. Play Hangman\n";
        cout << "2. View Leaderboard\n";
        cout << "3. Exit\n";
        cout << "================================\n";
        cout << "Select (1-3): ";

        // IF — invalid input handling
        if (!(cin >> choice) || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
            choice = 0;
        }

    } while (choice < 1 || choice > 3);

    // IF-ELSE for menu options
    if (choice == 1) {
        // STRUCT — create PlayerScore object
        PlayerScore ps;
        ps.wins       = 0;
        ps.losses     = 0;
        ps.totalGames = 0;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter your name: ";
        cin.getline(ps.name, 50);

        playHangman(&ps);   // pass address (pointer)

        char save;
        cout << "\nSave your score to leaderboard? (y/n): ";
        cin >> save;
        if (tolower(save) == 'y') {
            saveScore(&ps);
        }

        pauseScreen();
        showMenu();

    } else if (choice == 2) {
        clearScreen();
        displaySavedScores();
        pauseScreen();
        showMenu();

    } else {
        typePrint("  Thanks for playing Hangman Arcade! See you next time... ;)");
    }
}

// ============================================================
//   MAIN FUNCTION — entry point
// ============================================================
int main() {
    showIntro();     // animation intro
    showMenu();
    return 0;
}
