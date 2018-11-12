/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 * Proudly modified by Tinsae Gizachew
 * 
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// custom variables and functions
int* tile_finder(int tile);
bool is_blocked(int tile_row, int tile_column);
int blank_row;
int blank_column;

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int dim;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    dim = atoi(argv[1]);
    if (dim < DIM_MIN || dim > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int row = 0; row < dim; row++)
        {
            for (int column = 0; column < dim; column++)
            {
                fprintf(file, "%i", board[row][column]);
                if (column < dim - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("You won the game!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // This help us to animate the board
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // TODO
    // The number of tiles required
    // We subtract one to leave one vacant space for movement
    int n_tiles = (dim * dim) - 1;
    int max_num = n_tiles;
    for (int row = 0; row < dim; row++)
    {
        for (int column = 0; column < dim; column++)
        {
            board[row][column] = max_num;
            max_num--;
        }
        printf("\n");
    }
    
    // swap if n_tiles is odd
    // the tiles to be swapped are  found in the last row 
    // and in the two columns to the left of the last column
    if (n_tiles % 2 != 0)
    {
        board[dim - 1][dim - 2] = 
        board[dim - 1][dim - 2] + 
        board[dim - 1][dim - 3];
        board[dim - 1][dim - 3] = 
        board[dim - 1][dim - 2] - 
        board[dim - 1][dim - 3];
        board[dim - 1][dim - 2] = 
        board[dim - 1][dim - 2] - 
        board[dim - 1][dim - 3];
    }
    // the bottom right is found at the last column and last row
    blank_row = dim - 1;
    blank_column = dim - 1;
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    // Shows the board on the screen, the bottom-right will be underscore
    for (int row = 0; row < dim; row++)
    {
        for (int column = 0; column < dim; column++)
        {
            // to display two digit numbers
            // since the maximum dimension is 9 x 9; we can't have more
            // than 2 digits;
            // so one space after is enough to align the two digit numbers 
            // with one digit numbers
            if (board[row][column] >= 10)
            {
                printf("[%d] ", board[row][column]);
            }    
            else
            {
                if (board[row][column] == 0) 
                {
                    // The underscore will not stored on the board
                    printf(" [_] ");
                }
                else
                {
                // put space before and after
                // for single digits to align them with 2 digit numbers
                    printf(" [%d] ", board[row][column]); 
                }

            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    // tile should be within range
    if (tile > (dim * dim) - 1 || tile < 1)
    {
        return false;
    }
    // find the row and column of the tile user input
    int *tile_loc = tile_finder(tile);
    
    // Once tile has been found see if blank tile is adjacent to it
    // Check if adjacent tile is a legit tile, then check if is blank
    
    if (is_blocked(tile_loc[0],tile_loc[1]) == 0)
    {
        // swap the blank space and the moved tile
        int tile_value = board[tile_loc[0]][tile_loc[1]];
        // The line below will always make a tile 0
        board[tile_loc[0]][tile_loc[1]] = board[blank_row][blank_column];
        // The tile which was blank will take the value of the tile moved
        board[blank_row][blank_column] = tile_value;
        // update location of blank tile
        blank_row = tile_loc[0];
        blank_column = tile_loc[1];
        return true;
    }
    else
    {
        return false;
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // In order a game to be won, the numbers should be arranged
    // in order starting from 1 up to d-1
    // from top row -> bottom row, left -> right
    int counter = 1;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] == counter)
            {
                counter++;
            }
        } 
    }
    // the last value of counter(after we exit the loop)
    // should be d*d (remeber )
    // and the bottom-right should be 0
    if (counter == dim * dim && board[dim - 1][dim - 1] == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int* tile_finder(int tile)
{
    static int tile_loc[2];

    for (int row = 0; row < dim; row++)
    {
        for (int column = 0; column < dim; column++)
        {
            if (board[row][column] == tile)
            {
                tile_loc[0] = row;
                tile_loc[1] = column;
            } 
        }
    }
    return tile_loc;
}

// check if the blank tile is next to the tile in every direction
bool is_blocked(int tile_row, int tile_column)
{
    bool top_free = board[tile_row - 1][tile_column] == 0;
    bool bottom_free = board[tile_row + 1][tile_column] == 0;
    bool left_free = board[tile_row][tile_column - 1] == 0;
    bool right_free = board[tile_row][tile_column + 1] == 0;

    if ((tile_row > 0 && top_free) || 
    (tile_row < dim - 1 && bottom_free) || 
    (tile_column > 0 && left_free) || 
    (tile_column < dim - 1 && right_free))
    {
        return false;
    }
    else
    {
        return true;
    }
}
