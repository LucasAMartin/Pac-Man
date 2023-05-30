#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

// Map dimensions
const int mapWidth = 29;
const int mapHeight = 19;
char map[mapHeight][mapWidth] = {
        "############################",
        "##.####.#####.##.#####.###.#",
        "##........................ #",
        "##.####.##.########.##.###.#",
        "##.####.##.########.##.###.#",
        "##......##....##....##..o..#",
        "######.##### ## ##### ######",
        "######.##          ## ######",
        "######.## ###--### ## ######",
        "######.## #      # ## ######",
        "#     .   #      #   .     #",
        "######.## #      # ## ######",
        "######.## ######## ## ######",
        "######.##          ## ######",
        "######.## ######## ## ######",
        "##.......o....##..........# ",
        "##.###..#####..#####..######",
        "#.....# ####### #......##",
        "############################"
};

// Pacman data, including spawn tile
int pacmanX = 14;
int pacmanY = 13;
int score = 0;

// Ghost data, including spawn tile
int ghostX = 14;
int ghostY = 7;

void drawMap() {
    // Move cursor to top-left corner of console screen
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // Draw map
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (x == pacmanX && y == pacmanY) {
                std::cout << 'C';
            } else if (x == ghostX && y == ghostY) {
                std::cout << 'G';
            } else {
                std::cout << map[y][x];
            }
        }
        std::cout << std::endl;
    }

    // Draw score
    std::cout << "Score: " << score;
}


void movePacman() {
    // Get user input
    char input;
    // Using this library I can move on user input instead of needing to press enter everytime
    if (_kbhit()) {
        input = _getch();
        switch (input) {
            case 'w':
                if (map[pacmanY - 1][pacmanX] != '#' && map[pacmanY - 1][pacmanX] != '-') {
                    pacmanY--;
                }
                break;
            case 's':
                if (map[pacmanY + 1][pacmanX] != '#' && map[pacmanY + 1][pacmanX] != '-') {
                    pacmanY++;
                }
                break;
            case 'a':
                if (map[pacmanY][pacmanX - 1] != '#' && map[pacmanY][pacmanX - 1] != '-') {
                    pacmanX--;
                }
                break;
            case 'd':
                if (map[pacmanY][pacmanX + 1] != '#' && map[pacmanY][pacmanX + 1] != '-') {
                    pacmanX++;
                }
                break;
        }

        // Check for pellet collision
        if (map[pacmanY][pacmanX] == '.') {
            score++;
            map[pacmanY][pacmanX] = ' ';
        } else if (map[pacmanY][pacmanX] == 'o') {
            score += 10;
            map[pacmanY][pacmanX] = ' ';
        }
    }
}

void moveGhost() {
    // Counter variable to keep track of turns
    static int counter = 0;

    // Only move ghost every n turns
    int n = 5;
    if (counter % n == 0) {
        // Array to store valid directions
        int validDirections[4];
        int numValidDirections = 0;

        // Check for valid directions
        if (map[ghostY - 1][ghostX] != '#' && map[ghostY - 1][ghostX] != '-') {
            validDirections[numValidDirections] = 0;
            numValidDirections++;
        }
        if (map[ghostY + 1][ghostX] != '#' && map[ghostY + 1][ghostX] != '-') {
            validDirections[numValidDirections] = 1;
            numValidDirections++;
        }
        if (map[ghostY][ghostX - 1] != '#' && map[ghostY][ghostX - 1] != '-') {
            validDirections[numValidDirections] = 2;
            numValidDirections++;
        }
        if (map[ghostY][ghostX + 1] != '#' && map[ghostY][ghostX + 1] != '-') {
            validDirections[numValidDirections] = 3;
            numValidDirections++;
        }

        // Randomly choose a valid direction to move in
        if (numValidDirections > 0) {
            int direction = validDirections[rand() % numValidDirections];

            switch (direction) {
                case 0:
                    ghostY--;
                    break;
                case 1:
                    ghostY++;
                    break;
                case 2:
                    ghostX--;
                    break;
                case 3:
                    ghostX++;
                    break;
            }
        }
    }

    // Increment counter
    counter++;
}

bool checkWin() {
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            if (map[y][x] == '.' || map[y][x] == 'o') return false;
        }
    }
    return true;
}

// Print ascii art for "YOU WIN" using recursion
void printYouWin(int row) {
    string youWinArt[5] = {
            "*   * ***   *   *  *   * *** *   *",
            " * * *   *  *   *  *   *  *  **  *",
            "  *  *   *  *   *  ** **  *  * * *",
            "  *  *   *  *   *  ** **  *  *  **",
            "  *   ***    ***   *   * *** *   *"};

    if (row == 5) {
        return;
    } else {
        cout << youWinArt[row] << endl;
        printYouWin(row + 1);
    }
}

int main() {

    // Game loop
    while (true) {
        // Draw map
        drawMap();

        // Move Pacman
        movePacman();

        // Move ghost
        moveGhost();

        // Check for collision with ghost
        if (pacmanX == ghostX && pacmanY == ghostY) {
            cout << endl << endl << "Game Over!" << endl;
            cout << "Score: " << score << endl;
            break;
        }

        // Check for win condition
        if (checkWin()) {
            cout << endl << endl << "You Win!" << endl;
            cout << "Score: " << score << endl;
            printYouWin(0);
            break;
        }

        // Delay to make the game not speedrun itself every turn
        Sleep(50);
    }
    return 0;
}
