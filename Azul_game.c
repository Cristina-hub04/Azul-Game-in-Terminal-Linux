#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define NUM_TILES 100
#define GROUP_SIZE 4
#define NUM_COLORS 5
#define NUM_FACTORIES 5
#define MAX_NAME_LENGTH 50
#define BOARD_SIZE 5
#define PLAYER_TABLE_SIZE 20
#define MIDDLE_AREA_SIZE NUM_TILES
#define FLOOR_LINE_SIZE 7

// Define ANSI color escape sequences
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef enum {
    RED,
    YELLOW,
    BLUE,
    GREEN,
    PURPLE,
    NUM_TILE_COLORS
} TileColor;

typedef struct {
    TileColor color;
} Tile;

typedef struct {
    Tile tiles[GROUP_SIZE];
    int size;
} Factory;

typedef struct {
    Tile tiles[MIDDLE_AREA_SIZE];
    int size;
} MiddleArea;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
    TileColor board[BOARD_SIZE * BOARD_SIZE];
    TileColor player_table[NUM_TILE_COLORS][PLAYER_TABLE_SIZE];
    int table_counts[NUM_TILE_COLORS];
    TileColor half_matrix[BOARD_SIZE][BOARD_SIZE];
    Tile floor_line[FLOOR_LINE_SIZE];
    int floor_line_size;
} Player;

const char *tile_color_names[NUM_TILE_COLORS] = {
    "RED", "YELLOW", "BLUE", "GREEN", "PURPLE"
};

const int floor_line_penalties[FLOOR_LINE_SIZE] = {1, 1, 2, 2, 2, 3, 3};

void initialize_tile_bag(Tile bag[], int num_tiles) {
    for (int i = 0; i < num_tiles; i++) {
        bag[i].color = (TileColor)(i % NUM_TILE_COLORS);
    }
}

void shuffle_tile_bag(Tile bag[], int num_tiles) {
    for (int i = num_tiles - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Tile temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
}

void print_tile_color(TileColor color) {
    const char *color_code = ANSI_COLOR_RESET; // Default color code

    // Set ANSI color code based on tile color
    switch (color) {
        case RED:
            color_code = ANSI_COLOR_RED;
            break;
        case YELLOW:
            color_code = ANSI_COLOR_YELLOW;
            break;
        case BLUE:
            color_code = ANSI_COLOR_BLUE;
            break;
        case GREEN:
            color_code = ANSI_COLOR_GREEN;
            break;
        case PURPLE:
            color_code = ANSI_COLOR_MAGENTA;
            break;
        default:
            break;
    }

    // Print color name in the corresponding color
    printf("%s%s%s", color_code, tile_color_names[color], ANSI_COLOR_RESET);
}

void print_tile_bag(const Tile bag[], int num_tiles) {
    printf("Tile Bag Contents (remaining tiles: %d):\n", num_tiles);
    for (int i = 0; i < num_tiles; i++) {
        print_tile_color(bag[i].color);
        printf(" ");
    }
    printf("\n\n");
}

void print_factory(const Factory *factory) {
    printf("Factory Contents:\n");
    for (int i = 0; i < factory->size; i++) {
        print_tile_color(factory->tiles[i].color);
        printf(" ");
    }
    printf("\n\n");
}

void print_middle_area(const MiddleArea *middle_area) {
    printf("Middle Area Contents:\n");
    for (int i = 0; i < middle_area->size; i++) {
        print_tile_color(middle_area->tiles[i].color);
        printf(" ");
    }
    printf("\n\n");
}

void print_boards(const Player *player) {
    printf("%s's Board:\n", player->name);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i >= j) {
                if (player->half_matrix[i][j] != -1) {
                    print_tile_color(player->half_matrix[i][j]);
                } else {
                    printf(".");
                }
                printf(" ");
            } else {
                printf("  "); // Add spacing for non-usable spaces
            }
        }
        printf("\t");
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (player->board[i * BOARD_SIZE + j] != -1) {
                print_tile_color(player->board[i * BOARD_SIZE + j]);
            } else {
                printf(".");
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

void print_floor_line(const Player *player) {
    printf("%s's Floor Line:\n", player->name);
    for (int i = 0; i < player->floor_line_size; i++) {
        print_tile_color(player->floor_line[i].color);
        printf(" ");
    }
    printf("\n\n");
}

TileColor prompt_choose_tile_color(const Player *player) {
    char input[20];
    TileColor chosen_color = RED;

    printf("Select tile color for %s: ", player->name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    for (char *p = input; *p; ++p) {
        *p = toupper((unsigned char)*p);
    }

    for (int i = 0; i < NUM_TILE_COLORS; i++) {
        if (strcmp(input, tile_color_names[i]) == 0) {
            chosen_color = (TileColor)i;
            break;
        }
    }

    return chosen_color;
}

int prompt_choose_factory(const Player *player) {
    int chosen_factory = -1;

    while (chosen_factory < 1 || chosen_factory > NUM_FACTORIES) {
        printf("Select factory for %s (1-%d): ", player->name, NUM_FACTORIES);
        if (scanf("%d", &chosen_factory) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            chosen_factory = -1;
            printf("Invalid input. Please enter a number between 1 and %d.\n", NUM_FACTORIES);
        } else if (chosen_factory < 1 || chosen_factory > NUM_FACTORIES) {
            printf("Invalid choice. Please choose a factory number between 1 and %d.\n", NUM_FACTORIES);
        }
    }
    getchar(); // Clear newline character from input buffer

    return chosen_factory;
}

int prompt_choose_line(const Player *player) {
    int chosen_line = -1;

    while (chosen_line < 1 || chosen_line > BOARD_SIZE) {
        printf("Select line for %s (1-%d): ", player->name, BOARD_SIZE);
        if (scanf("%d", &chosen_line) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            chosen_line = -1;
            printf("Invalid input. Please enter a number between 1 and %d.\n", BOARD_SIZE);
        } else if (chosen_line < 1 || chosen_line > BOARD_SIZE) {
            printf("Invalid choice. Please choose a line number between 1 and %d.\n", BOARD_SIZE);
        }
    }
    getchar(); // Clear newline character from input buffer

    return chosen_line - 1; // Return 0-based index
}

int check_line_complete(TileColor line[]) {
    TileColor color = line[0]; // Get the color of the first tile
    for (int i = 1; i < BOARD_SIZE; i++) {
        if (line[i] != color) {
            return 0; // Line is not complete
        }
    }
    return 1; // Line is complete
}

int is_color_in_row(TileColor board[], int row, TileColor color) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[row * BOARD_SIZE + i] == color) {
            return 1; // Color is already in this row
        }
    }
    return 0;
}

void place_tiles_on_half_matrix(Player *player, Factory *factory, TileColor chosen_color, int chosen_line, MiddleArea *middle_area) {
    if (is_color_in_row(player->board, chosen_line, chosen_color)) {
        printf("Cannot place color %s in row %d as it already exists.\n", tile_color_names[chosen_color], chosen_line + 1);
        for (int i = 0; i < factory->size; i++) {
            middle_area->tiles[middle_area->size++] = factory->tiles[i];
        }
        factory->size = 0;
        return;
    }

    for (int i = 0; i < factory->size; i++) {
        if (factory->tiles[i].color == chosen_color) {
            int placed = 0;
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (player->half_matrix[chosen_line][j] == -1) {
                    player->half_matrix[chosen_line][j] = chosen_color;
                    placed = 1;
                    break;
                }
            }
            if (!placed) {
                // Place the tile in the floor line if the chosen line is full
                if (player->floor_line_size < FLOOR_LINE_SIZE) {
                    player->floor_line[player->floor_line_size++] = factory->tiles[i];
                }
            }
        } else {
            middle_area->tiles[middle_area->size++] = factory->tiles[i];
        }
    }
    factory->size = 0;
}

void initialize_half_matrix(Player *player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i >= j) {
                player->half_matrix[i][j] = -1; // initialize to empty
            } else {
                player->half_matrix[i][j] = 0; // initialize non-usable spaces
            }
        }
    }
}

int calculate_score(Player *player) {
    int score = 0;
    // Add score for completed rows in the square board
    for (int i = 0; i < BOARD_SIZE; i++) {
        int row_complete = 1;
        TileColor color = player->board[i * BOARD_SIZE];
        for (int j = 1; j < BOARD_SIZE; j++) {
            if (player->board[i * BOARD_SIZE + j] != color) {
                row_complete = 0;
                break;
            }
        }
        if (row_complete) {
            score += BOARD_SIZE; // Each completed row gives BOARD_SIZE points
            // Remove tiles from completed row
            for (int j = 0; j < BOARD_SIZE; j++) {
                player->board[i * BOARD_SIZE + j] = -1;
            }
        }
    }

    // Add score for completed columns in the square board
    for (int j = 0; j < BOARD_SIZE; j++) {
        int col_complete = 1;
        TileColor color = player->board[j];
        for (int i = 1; i < BOARD_SIZE; i++) {
            if (player->board[i * BOARD_SIZE + j] != color) {
                col_complete = 0;
                break;
            }
        }
        if (col_complete) {
            score += BOARD_SIZE; // Each completed column gives BOARD_SIZE points
            // Remove tiles from completed column
            for (int i = 0; i < BOARD_SIZE; i++) {
                player->board[i * BOARD_SIZE + j] = -1;
            }
        }
    }

    // Subtract penalties from floor line
    for (int i = 0; i < player->floor_line_size; i++) {
        score -= floor_line_penalties[i];
    }

    player->score += score;
    return score;
}

int main() {
    srand(time(NULL));

    Player player1, player2;
    strncpy(player1.name, "Player 1", MAX_NAME_LENGTH - 1);
    player1.name[MAX_NAME_LENGTH - 1] = '\0';
    player1.score = 0;
    memset(player1.board, -1, sizeof(player1.board));
    memset(player1.player_table, -1, sizeof(player1.player_table));
    memset(player1.table_counts, 0, sizeof(player1.table_counts));
    initialize_half_matrix(&player1);
    player1.floor_line_size = 0;

    strncpy(player2.name, "Player 2", MAX_NAME_LENGTH - 1);
    player2.name[MAX_NAME_LENGTH - 1] = '\0';
    player2.score = 0;
    memset(player2.board, -1, sizeof(player2.board));
    memset(player2.player_table, -1, sizeof(player2.player_table));
    memset(player2.table_counts, 0, sizeof(player2.table_counts));
    initialize_half_matrix(&player2);
    player2.floor_line_size = 0;

    Tile tile_bag[NUM_TILES];
    initialize_tile_bag(tile_bag, NUM_TILES);
    shuffle_tile_bag(tile_bag, NUM_TILES);

    Factory factories[NUM_FACTORIES];
    int num_tiles_in_bag = NUM_TILES;
    for (int i = 0; i < NUM_FACTORIES; i++) {
        factories[i].size = 0;
        for (int j = 0; j < GROUP_SIZE; j++) {
            if (num_tiles_in_bag > 0) {
                factories[i].tiles[j] = tile_bag[--num_tiles_in_bag];
                factories[i].size++;
            }
        }
    }

    MiddleArea middle_area;
    middle_area.size = 0;

    int num_rounds = 5;
    for (int round = 1; round <= num_rounds; round++) {
        printf("Round %d\n", round);

        // Player 1's turn
        printf("%s's turn:\n", player1.name);

        print_tile_bag(tile_bag, num_tiles_in_bag);

        for (int i = 0; i < NUM_FACTORIES; i++) {
            printf("Factory %d:\n", i + 1);
            print_factory(&factories[i]);
        }
        printf("Middle Area:\n");
        print_middle_area(&middle_area);

        TileColor chosen_color_player1 = prompt_choose_tile_color(&player1);
        int chosen_factory_player1 = prompt_choose_factory(&player1);
        int chosen_line_player1 = prompt_choose_line(&player1);

        if (chosen_factory_player1 != -1) {
            place_tiles_on_half_matrix(&player1, &factories[chosen_factory_player1 - 1], chosen_color_player1, chosen_line_player1, &middle_area);
        }

        print_boards(&player1);
        print_floor_line(&player1);

        // Player 2's turn
        printf("%s's turn:\n", player2.name);

        print_tile_bag(tile_bag, num_tiles_in_bag);

        for (int i = 0; i < NUM_FACTORIES; i++) {
            printf("Factory %d:\n", i + 1);
            print_factory(&factories[i]);
        }
        printf("Middle Area:\n");
        print_middle_area(&middle_area);

        TileColor chosen_color_player2 = prompt_choose_tile_color(&player2);
        int chosen_factory_player2 = prompt_choose_factory(&player2);
        int chosen_line_player2 = prompt_choose_line(&player2);

        if (chosen_factory_player2 != -1) {
            place_tiles_on_half_matrix(&player2, &factories[chosen_factory_player2 - 1], chosen_color_player2, chosen_line_player2, &middle_area);
        }

        print_boards(&player2);
        print_floor_line(&player2);
    }

    // Game ends, calculate final scores
    int score_player1 = calculate_score(&player1);
    int score_player2 = calculate_score(&player2);

    printf("Final Scores:\n");
    printf("%s: %d\n", player1.name, player1.score);
    printf("%s: %d\n", player2.name, player2.score);

    // Determine the winner
    if (score_player1 > score_player2) {
        printf("%s wins!\n", player1.name);
    } else if (score_player1 < score_player2) {
        printf("%s wins!\n", player2.name);
    } else {
        printf("It's a tie!\n");
    }

    return 0;
}
