#include <stdio.h>
#include <stdlib.h>

#define MAX_ROWS 20
#define MAX_COLS 20
#define WALL '#'
#define EMPTY ' '
#define PLAYER '^'
#define PLAYERUP '^'
#define PLAYERDOWN 'v'
#define PLAYERRIGHT '>'
#define PLAYERLEFT '<'
#define SNAKE '~'
#define GOAL 'X'
#define OBSTACLE 'O'

int moves = 0;

struct Location
{
    int row;
    int col;
};

typedef struct Location Location;

struct Node
{
    Location data;
    struct Node* next;
};

typedef struct Node Node;

struct LinkedList
{
    Node* head;
};

typedef struct LinkedList LinkedList;

void push(LinkedList* list, Location value)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = list->head;
    list->head = newNode;
}

Location pop(LinkedList* list)
{
    Location value;
    if (list->head)
    {
        value = list->head->data;
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    else
    {
        value.row = -1; // You can use a sentinel value to indicate an empty list
        value.col = -1;
    }
    return value;
}

int isEmpty(LinkedList* list)
{
    return list->head == NULL;
}

void destroy(LinkedList* list)
{
    while (!isEmpty(list))
    {
        pop(list);
    }
}

typedef struct MazeGame
{
    char gameMap[MAX_ROWS][MAX_COLS];
    int numRows;
    int numCols;
    int numObj;

    Location player;
    Location snake;
    Location goal;
    LinkedList moveHistory;
    LinkedList moveSnakeHistory;
} MazeGame;

MazeGame* createMazeGame(const char* mapFileName)
{
    MazeGame* game = (MazeGame*)malloc(sizeof(MazeGame));
    FILE* mapFile = fopen(mapFileName, "r");
    if (!mapFile)
    {
        fprintf(stderr, "Error: File not found.\n");
        exit(1);
    }

    fscanf(mapFile, "%d %d %d", &game->numObj, &game->numRows, &game->numCols);

    for (int i = 0; i < MAX_ROWS; i++)
    {
        for (int j = 0; j < MAX_COLS; j++)
        {
            game->gameMap[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < game->numObj; i++)
    {
        int r, c, code;
        fscanf(mapFile, "%d %d %d", &r, &c, &code);
        if (code == 0)
        {
            // player location
            game->player.row = r;
            game->player.col = c;
            game->gameMap[r][c] = PLAYER;
        }
        else if (code == 1)
        {
            // snake
            game->snake.row = r;
            game->snake.col = c;
            game->gameMap[r][c] = SNAKE;
        }
        else if (code == 2)
        {
            // goal
            game->goal.row = r;
            game->goal.col = c;
            game->gameMap[r][c] = GOAL;
        }
        else if (code == 3)
        {
            game->gameMap[r][c] = OBSTACLE;
        }
    }

    for (int i = 0; i < game->numRows; i++)
    {
        game->gameMap[i][0] = WALL;
        game->gameMap[i][game->numCols - 1] = WALL;
    }
    for (int i = 0; i < game->numCols; i++)
    {
        game->gameMap[0][i] = WALL;
        game->gameMap[game->numRows - 1][i] = WALL;
    }

    fclose(mapFile);
    return game;
}

void printMap(const MazeGame* game)
{
    system("cls");
    for (int row = 0; row < game->numRows; ++row)
    {
        for (int col = 0; col < game->numCols; ++col)
        {
            printf("%c ", game->gameMap[row][col]);
        }
        printf("\n");
    }
}

int isValidMove(const MazeGame* game, int newRow, int newCol)
{
    return (newRow >= 0 && newRow < game->numRows && newCol >= 0 && newCol < game->numCols &&
            game->gameMap[newRow][newCol] != OBSTACLE && game->gameMap[newRow][newCol] != WALL);
}

int movePlayer(MazeGame* game, char move)
{
    int newRow = game->player.row;
    int newCol = game->player.col;

    if (move == 'w') newRow--;
    else if (move == 's') newRow++;
    else if (move == 'a') newCol--;
    else if (move == 'd') newCol++;

    if (isValidMove(game, newRow, newCol))
    {
        push(&game->moveHistory, game->player);
        game->gameMap[game->player.row][game->player.col] = EMPTY;
        game->player.row = newRow;
        game->player.col = newCol;
        if (move == 'w') game->gameMap[game->player.row][game->player.col] = PLAYERUP;
        else if (move == 's') game->gameMap[game->player.row][game->player.col] = PLAYERDOWN;
        else if (move == 'a') game->gameMap[game->player.row][game->player.col] = PLAYERLEFT;
        else if (move == 'd') game->gameMap[game->player.row][game->player.col] = PLAYERRIGHT;
        return 1;
    }

    return 0;
}

int moveSnake(MazeGame* game)
{
    // Calculate the new snake position
    int newRow = game->snake.row;
    int newCol = game->snake.col;

    // If the snake is moving towards the player, update its position
    if (game->player.row < newRow && game->gameMap[newRow - 1][newCol] != OBSTACLE) newRow--;
    else if (game->player.row > newRow && game->gameMap[newRow + 1][newCol] != OBSTACLE) newRow++;
    else if (game->player.col < newCol && game->gameMap[newRow][newCol - 1] != OBSTACLE) newCol--;
    else if (game->player.col > newCol && game->gameMap[newRow][newCol + 1] != OBSTACLE) newCol++;

    // Update the snake's position
    if (isValidMove(game, newRow, newCol))
    {
        push(&game->moveSnakeHistory, game->snake);
        game->gameMap[game->snake.row][game->snake.col] = EMPTY;
        game->snake.row = newRow;
        game->snake.col = newCol;
        game->gameMap[game->snake.row][game->snake.col] = SNAKE;
    }

    // Check if the snake has bitten the player
    if (game->player.row == game->snake.row && game->player.col == game->snake.col)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void undo(MazeGame* game)
{
    if (!isEmpty(&game->moveHistory))
    {
        Location previous = pop(&game->moveHistory);
        game->gameMap[game->player.row][game->player.col] = EMPTY;
        game->player = previous;
        game->gameMap[game->player.row][game->player.col] = PLAYER;
    }
    if (!isEmpty(&game->moveSnakeHistory))
    {
        Location prev = pop(&game->moveSnakeHistory);
        game->gameMap[game->snake.row][game->snake.col] = EMPTY;
        game->snake = prev;
        game->gameMap[game->snake.row][game->snake.col] = SNAKE;
    }
}

int isWin(const MazeGame* game)
{
    return (game->player.row == game->goal.row && game->player.col == game->goal.col);
}

int isGameOver(const MazeGame* game)
{
    return (game->player.row == game->snake.row && game->player.col == game->snake.col);
}

void play(MazeGame* game)
{
    while (1)
    {
        printMap(game);

        if (isWin(game))
        {
            printf("Congratulations! You Won!\n");
            break;
        }

        char move;
        printf("Enter Move (w(Up)/a(Left)/s(Down)/d(Right)/u(Undo)): ");
        scanf(" %c", &move);

        if (move == 'u')
        {
            undo(game);
        }
        else if (move == 'w' || move == 'a' || move == 's' || move == 'd')
        {
            if (movePlayer(game, move) && moveSnake(game))
            {
                printf("Game Over! Snake bit you.\n");
                break;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        const char* mapFileName = argv[1];
        MazeGame* game = createMazeGame(mapFileName);
        play(game);
        free(game);
    }
    else
    {
        printf("Please provide a file name as an argument.\n");
    }

    return 0;
}