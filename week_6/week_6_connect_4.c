/* Play connect 4 between 2 people. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

//-----------------------------------------------------------------------------
// Setting up types and constants.

// Constants for Red player, Yellow player, and neither N.
enum player { Red, Yellow, N };
typedef enum player player;

// Constants to represent validity checking.
enum validity { Unchecked, BadFormat, BadLetter, BadCell, OK };
typedef enum validity validity;

// The game object contains all information on how many moves have been made,
// who's turn it is and information on which grid spaces contain coins.
struct game {
    int moves;
    player next;
    player grid[7][7];
};
typedef struct game game;

//-----------------------------------------------------------------------------
// Functions for game logic.

// Clearing the grid and initialising new game.
void new_game(game *g, player first) {
    g -> next = first;
    g -> moves = 0;
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        g -> grid[i][j] = N;
      }
    }
}

// Convert the letter in a valid move string such as "c" into a column index.
int column(char *text) {
    if (strchr(text, 'a') != NULL) return 0;
    if (strchr(text, 'b') != NULL) return 1;
    if (strchr(text, 'c') != NULL) return 2;
    if (strchr(text, 'd') != NULL) return 3;
    if (strchr(text, 'e') != NULL) return 4;
    if (strchr(text, 'f') != NULL) return 5;
    else return 6;
}

// Work out which row the coin will be dropped down to.
int row(game *g, char *text) {
    bool found = false;
    int row_placed = 0;
    for (int i = 0; i < 7; i++) {
      if (g -> grid[i][column(text)] != N && found == false) {
        found = true;
        row_placed = i - 1;
      }
    }
    if (found == false) {
        found = true;
        row_placed = 6;
    }
    return row_placed;
}

// Check the validity of the user input and return error codes based on input.
validity valid_input(game *g, char *text) {
    if (strlen(text) != 1) return BadFormat;
    if (text[0] != 'a' && text[0] != 'b' && text[0] != 'c') {
      if (text[0] != 'd' && text[0] != 'e' && text[0] != 'f') {
        if (text[0] != 'g') return BadLetter;
      }
    }
    return OK;
}

// Make a move and change the grid.
void move(game *g, char *c) {
    g -> moves++;
    if (g -> next == Red) {
      g -> grid[row(g, c)][column(c)] = Red;
      g -> next = Yellow;
    }
    else {
      g -> grid[row(g, c)][column(c)] = Yellow;
      g -> next = Red;
    }
}

// Check whether the given four_in_a_row is a four in a row.
player four_in_a_row(player p0, player p1, player p2, player p3) {
    if (p0 == p1 && p1 == p2 && p2 == p3 && p0 == Red) return Red;
    if (p0 == p1 && p1 == p2 && p2 == p3 && p0 == Yellow) return Yellow;
    else return N;
}

// Work out which player has won.
player win(game *g) {
    int winner = N;
    //test horizontal lines
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 4; j++) {
        if (winner == N) {
          winner = (four_in_a_row(g -> grid[i][j],
                                  g -> grid[i][j + 1],
                                  g -> grid[i][j + 2],
                                  g -> grid[i][j + 3]));
        }
      }
    }
    //test vertical lines
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 7; j++) {
        if (winner == N) {
          winner = (four_in_a_row(g -> grid[i][j],
                                  g -> grid[i + 1][j],
                                  g -> grid[i + 2][j],
                                  g -> grid[i + 3][j]));
        }
      }
    }
    //test diagonal lines going down and to the right
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (winner == N) {
          winner = (four_in_a_row(g -> grid[i][j],
                                  g -> grid[i + 1][j + 1],
                                  g -> grid[i + 2][j + 2],
                                  g -> grid[i + 3][j + 3]));
        }
      }
    }
    //test diagonal lines going down and to the left
    for (int i = 0; i < 4; i++) {
      for (int j = 3; j < 7; j++) {
        if (winner == N) {
          winner = (four_in_a_row(g -> grid[i][j],
                                  g -> grid[i + 1][j - 1],
                                  g -> grid[i + 2][j - 2],
                                  g -> grid[i + 3][j - 3]));
        }
      }
    }
    return winner;
}

// Check if the game cannot be won by anyone and must therefore be drawn.
bool draw(game *g) {
    int cells_occupied = 0;
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        if (g -> grid[i][j] == Red || g -> grid[i][j] == Yellow) cells_occupied++;
      }
    }
    if (cells_occupied == 49) return true;
    else return false;
}

//-----------------------------------------------------------------------------

// Plays the game with two players.
void play(char player) {
}

//-----------------------------------------------------------------------------
// Testing section + main & setup function.

void setup(game *g, char *grid) {
    int reds = 0, yellows = 0, moves = 0;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            char ch = grid[r * 8 + c];
            if (ch == 'R') { g -> grid[r][c] = Red; reds++; moves++; }
            else if (ch == 'Y') { g -> grid[r][c] = Yellow; yellows++; moves++; }
            else g -> grid[r][c] = N;
        }
    }
    g -> moves = moves;
    if (reds == yellows || reds == yellows - 1) g -> next = Red;
    else if (reds == yellows + 1) g -> next = Yellow;
}

// Check constants haven't been changed.
void check_constants() {
    assert(Red == 0 && Yellow == 1 && N == 2);
    assert(Unchecked == 0 && BadFormat == 1 && BadLetter == 2);
    assert(BadCell == 3 && OK == 4);
}

// Test that a new game has been correctly setup.
void test_new(game *g) {
    new_game(g, Red);
    assert(g -> grid[0][0] == N);
    assert(g -> grid[1][1] == N);
    assert(g -> grid[2][2] == N);
    assert(g -> grid[3][3] == N);
    assert(g -> grid[4][4] == N);
    assert(g -> grid[5][5] == N);
    assert(g -> grid[6][6] == N);
    assert(g -> grid[6][1] == N);
    assert(g -> grid[4][6] == N);
    assert(g -> grid[1][5] == N);
    assert(g -> next == Red);
    assert(g -> moves == 0);
}

// Tests the row function.
void test_row(game *g) {
    setup(g, "------- ------Y ------R ------R ---R--Y ---RY-R --RYYRY");
    assert(row(g, "c") == 5); //second row from bottom
    assert(row(g, "d") == 3); //fourth row from bottom
    assert(row(g, "g") == 0); //top row
}

// Tests the column function.
void test_column() {
    assert(column("a") == 0);
    assert(column("b") == 1);
    assert(column("c") == 2);
    assert(column("d") == 3);
    assert(column("e") == 4);
    assert(column("f") == 5);
    assert(column("g") == 6);
}

// Tests the valid_input function.
void test_valid_input(game *g) {
    new_game(g, Red);
    assert(valid_input(g, "h") == BadLetter);
    assert(valid_input(g, "k") == BadLetter);
    assert(valid_input(g, "z") == BadLetter);
    assert(valid_input(g, "A") == BadLetter);
    assert(valid_input(g, "2") == BadLetter);
    assert(valid_input(g, "b2") == BadFormat);
    assert(valid_input(g, "\0b") == BadFormat);
    assert(valid_input(g, "\0\0b") == BadFormat);
    assert(valid_input(g, "") == BadFormat);
}

// Tests the move function.
void test_move(game *g) {
    new_game(g, Red);
    move(g, "b");
    assert(g -> grid[6][1] == Red);
    assert(g -> next == Yellow);
    assert(g -> moves == 1);
    move(g, "b");
    assert(g -> grid[5][1] == Yellow);
    assert(g -> next == Red);
    assert(g -> moves == 2);
    move(g, "b");
    assert(g -> grid[4][1] == Red);
    assert(g -> next == Yellow);
    assert(g -> moves == 3);
}

// Tests the four in a row function.
void test_four_in_a_row() {
    assert(four_in_a_row(Red, Red, Red, Red) == Red);
    assert(four_in_a_row(Yellow, Yellow, Yellow, Yellow) == Yellow);
    assert(four_in_a_row(Red, Red, Red, Yellow) == N);
    assert(four_in_a_row(Red, Red, Yellow, Yellow) == N);
    assert(four_in_a_row(Red, Yellow, Yellow, Yellow) == N);
    assert(four_in_a_row(Red, Red, Yellow, Red) == N);
    assert(four_in_a_row(Red, Yellow, Red, Red) == N);
    assert(four_in_a_row(Yellow, Red, Red, Red) == N);
    assert(four_in_a_row(Yellow, Yellow, Red, Red) == N);
    assert(four_in_a_row(Yellow, Yellow, Yellow, Red) == N);
}

// Tests the win function.
void test_win(game *g) {
    // Testing horizontal lines
    setup(g, "YYYY--- ------- ------- ------- ------- ------- -------");
    assert(win(g) == Yellow);
    setup(g, "------- -RRRR-- ------- ------- ------- ------- -------");
    assert(win(g) == Red);
    setup(g, "------- ------- --YYYY- ------- ------- ------- -------");
    assert(win(g) == Yellow);
    setup(g, "------- ------- ------- ---RRRR ------- ------- -------");
    assert(win(g) == Red);
    setup(g, "------- ------- ------- ------- --YYYY- ------- -------");
    assert(win(g) == Yellow);
    setup(g, "------- ------- ------- ------- ------- -RRRR-- -------");
    assert(win(g) == Red);
    setup(g, "------- ------- ------- ------- YYYY--- ------- -------");
    assert(win(g) == Yellow);
    // Testing vertical lines
    setup(g, "Y------ Y------ Y------ Y------ ------- ------- -------");
    assert(win(g) == Yellow);
    setup(g, "------- -R----- -R----- -R----- -R----- ------- -------");
    assert(win(g) == Red);
    setup(g, "------- ------- --Y---- --Y---- --Y---- --Y---- -------");
    assert(win(g) == Yellow);
    setup(g, "------- ------- ------- ---R--- ---R--- ---R--- ---R---");
    assert(win(g) == Red);
    setup(g, "------- ------- ----Y-- ----Y-- ----Y-- ----Y-- -------");
    assert(win(g) == Yellow);
    setup(g, "------- -----R- -----R- -----R- -----R- ------- -------");
    assert(win(g) == Red);
    setup(g, "------Y ------Y ------Y ------Y ------- ------- -------");
    assert(win(g) == Yellow);
    // Testing diagonal lines down and to the right
    setup(g, "Y------ -Y----- --Y---- ---Y--- ------- ------- -------");
    assert(win(g) == Yellow);
    setup(g, "------- R------ -R----- --R---- ---R--- ------- -------");
    assert(win(g) == Red);
    setup(g, "------- ------- Y------ -Y----- --Y---- ---Y--- -------");
    assert(win(g) == Yellow);
    setup(g, "------- ------- ------- R------ -R----- --R---- ---R---");
    assert(win(g) == Red);
    // Testing diagonal lines down and to the left
    setup(g, "------- ------- ------- ------Y -----Y- ----Y-- ---Y---");
    assert(win(g) == Yellow);
    setup(g, "------- ------- ------R -----R- ----R-- ---R--- -------");
    assert(win(g) == Red);
    setup(g, "------- ------Y -----Y- ----Y-- ---Y--- ------- -------");
    assert(win(g) == Yellow);
    setup(g, "------R -----R- ----R-- ---R--- ------- ------- -------");
    assert(win(g) == Red);
}

// Tests no-one winning the game
void test_no_win(game *g) {
    setup(g, "------- ------- ------- ------- ------- ------- -------");
    assert(win(g) == N);
}

// Tests the draw function.
void test_draw(game *g) {
    setup(g, "------- ------Y ------R ------R ---R--Y ---RY-R --RYYRY");
    assert(draw(g) == false);
    setup(g, "RRYRRYR RYYYRYR RYRYYYR RRRYRYR YYRYRRR RYYRRYR YYRYRYR");
    assert(draw(g) == true);
}

// Testing functions.
void test() {
    game *g = malloc(sizeof(game));
    check_constants();
    test_new(g);
    test_row(g);
    test_column();
    test_valid_input(g);
    test_move(g);
    test_four_in_a_row();
    test_win(g);
    test_draw(g);
    test_no_win(g);
    free(g);
    printf("All tests passed.\n");
}

// Run the program with no args to carry out the tests, you can run with one
// arg to choose the first player to play the game.
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2 && strcmp(args[1],"Red") == 0) play(Red);
    else if (n == 2 && strcmp(args[1],"Yellow") == 0) play(Yellow);
    else {
        fprintf(stderr, "Use: ./connect_4  OR  ./connect_4 Red  OR ");
        fprintf(stderr, "./connect_4 Yellow\n");
        exit(1);
    }
    return 0;
}
