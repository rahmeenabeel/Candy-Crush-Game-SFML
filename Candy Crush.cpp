#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

const int GRID_SIZE = 9;
const int TILE_SIZE = 60;
const int NUM_CANDY_TYPES = 5;
const int STRIPED = 5;
const int WRAPPED = 6;
const int COLOR_BOMB = 7;
const int TOTAL_CANDY_TYPES = 8;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int TARGET_SCORE = 1500;
const int MAX_MOVES = 20;

int grid[GRID_SIZE][GRID_SIZE];
int score = 0;
int movesLeft = MAX_MOVES;
bool isPaused = false;
bool gameOver = false;
bool wonGame = false;

sf::RectangleShape candies[TOTAL_CANDY_TYPES];
sf::Font font;
sf::Text scoreText, movesText, pauseText, endText;
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Candy Crush - SFML");

void initializeCandies() {
    // Regular candies (0-4)
    for (int i = 0; i < NUM_CANDY_TYPES; i++) {
        candies[i].setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        candies[i].setOutlineColor(sf::Color::Black);
        candies[i].setOutlineThickness(2);
    }
    candies[0].setFillColor(sf::Color::Blue);
    candies[1].setFillColor(sf::Color::Green);
    candies[2].setFillColor(sf::Color::Red);
    candies[3].setFillColor(sf::Color::Yellow);
    candies[4].setFillColor(sf::Color::Magenta);

    // Special candies (5-7)
    // Striped (5)
    candies[STRIPED].setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    candies[STRIPED].setFillColor(sf::Color::Cyan);
    candies[STRIPED].setOutlineColor(sf::Color::Black);
    candies[STRIPED].setOutlineThickness(2);

    // Wrapped (6)
    candies[WRAPPED].setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    candies[WRAPPED].setFillColor(sf::Color(255, 165, 0)); // Orange
    candies[WRAPPED].setOutlineColor(sf::Color::Black);
    candies[WRAPPED].setOutlineThickness(5);

    // Color Bomb (7)
    candies[COLOR_BOMB].setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    candies[COLOR_BOMB].setFillColor(sf::Color::White);
    candies[COLOR_BOMB].setOutlineColor(sf::Color::Black);
    candies[COLOR_BOMB].setOutlineThickness(3);
}

void initializeUI() {
    if (!font.loadFromFile("assets/arial.ttf")) {
        cout << "Failed to load font!\n";
        return;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(600, 50);

    movesText.setFont(font);
    movesText.setCharacterSize(20);
    movesText.setFillColor(sf::Color::White);
    movesText.setPosition(600, 100);

    pauseText.setFont(font);
    pauseText.setCharacterSize(40);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("PAUSED");
    pauseText.setPosition(300, 250);

    endText.setFont(font);
    endText.setCharacterSize(30);
    endText.setFillColor(sf::Color::Cyan);
    endText.setPosition(180, 250);
}

void updateUI() {
    scoreText.setString("Score: " + to_string(score));
    movesText.setString("Moves Left: " + to_string(movesLeft));
}

int getRandomCandy() {
    return rand() % NUM_CANDY_TYPES;
}

bool isInBounds(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

void crushCandies(bool toCrush[GRID_SIZE][GRID_SIZE]) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (toCrush[y][x]) {
                grid[y][x] = -1;
                score += 10;
            }
        }
    }

    for (int x = 0; x < GRID_SIZE; x++) {
        int writeRow = GRID_SIZE - 1;
        for (int y = GRID_SIZE - 1; y >= 0; y--) {
            if (grid[y][x] != -1) {
                grid[writeRow][x] = grid[y][x];
                writeRow--;
            }
        }
        while (writeRow >= 0) {
            grid[writeRow][x] = getRandomCandy();
            writeRow--;
        }
    }
}

bool markMatches(bool toCrush[GRID_SIZE][GRID_SIZE]) {
    bool found = false;

    // Clear toCrush array
    for (int y = 0; y < GRID_SIZE; y++)
        for (int x = 0; x < GRID_SIZE; x++)
            toCrush[y][x] = false;

    // Horizontal
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE - 2; x++) {
            int t = grid[y][x];
            if (t >= 0 && t == grid[y][x + 1] && t == grid[y][x + 2]) {
                found = true;
                int length = 3;
                if (x + 3 < GRID_SIZE && grid[y][x + 3] == t) length++;
                if (x + 4 < GRID_SIZE && grid[y][x + 4] == t) length++;

                for (int i = 0; i < length; i++) toCrush[y][x + i] = true;

                if (length == 4)
                    grid[y][x + 1] = STRIPED;
                else if (length >= 5)
                    grid[y][x + 2] = COLOR_BOMB;
            }
        }
    }

    // Vertical
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE - 2; y++) {
            int t = grid[y][x];
            if (t >= 0 && t == grid[y + 1][x] && t == grid[y + 2][x]) {
                found = true;
                int length = 3;
                if (y + 3 < GRID_SIZE && grid[y + 3][x] == t) length++;
                if (y + 4 < GRID_SIZE && grid[y + 4][x] == t) length++;

                for (int i = 0; i < length; i++) toCrush[y + i][x] = true;

                if (length == 4)
                    grid[y + 1][x] = STRIPED;
                else if (length >= 5)
                    grid[y + 2][x] = COLOR_BOMB;
            }
        }
    }
    // Check for T or L shapes
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            int t = grid[y][x];
            if (t < 0) continue;

            int hCount = 1, vCount = 1;

            // Count horizontal
            for (int i = 1; x + i < GRID_SIZE && grid[y][x + i] == t; i++) hCount++;
            for (int i = 1; x - i >= 0 && grid[y][x - i] == t; i++) hCount++;

            // Count vertical
            for (int i = 1; y + i < GRID_SIZE && grid[y + i][x] == t; i++) vCount++;
            for (int i = 1; y - i >= 0 && grid[y - i][x] == t; i++) vCount++;

            if ((hCount >= 3 && vCount >= 2) || (hCount >= 2 && vCount >= 3)) {
                found = true;
                // Mark full T/L-shape for crushing
                for (int i = -2; i <= 2; i++) {
                    if (x + i >= 0 && x + i < GRID_SIZE && grid[y][x + i] == t)
                        toCrush[y][x + i] = true;
                    if (y + i >= 0 && y + i < GRID_SIZE && grid[y + i][x] == t)
                        toCrush[y + i][x] = true;
                }
                grid[y][x] = WRAPPED;
            }
        }
    }

    return found;
}


void initializeGrid() {
    bool crushArray[GRID_SIZE][GRID_SIZE];
    do {
        for (int y = 0; y < GRID_SIZE; y++)
            for (int x = 0; x < GRID_SIZE; x++)
                grid[y][x] = getRandomCandy();
    } while (markMatches(crushArray)); // Repeat until no initial matches are found
}

void processMatches() {
    bool found;
    do {
        bool crushArray[GRID_SIZE][GRID_SIZE];
        found = markMatches(crushArray);
        if (found) crushCandies(crushArray);
    } while (found);
}

void drawGrid() {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            int type = grid[y][x];
            if (type >= 0 && type <= COLOR_BOMB) {  // Changed to <= COLOR_BOMB
                candies[type].setPosition(x * TILE_SIZE, y * TILE_SIZE);
                window.draw(candies[type]);

                // Additional decorations for special candies
                if (type == STRIPED) {
                    sf::RectangleShape line(sf::Vector2f(TILE_SIZE - 10, 3));
                    line.setFillColor(sf::Color::White);
                    line.setPosition(x * TILE_SIZE + 5, y * TILE_SIZE + TILE_SIZE / 2 - 1);
                    window.draw(line);
                }
                else if (type == WRAPPED) {
                    sf::RectangleShape inner(sf::Vector2f(TILE_SIZE - 20, TILE_SIZE - 20));
                    inner.setFillColor(sf::Color::Black);
                    inner.setPosition(x * TILE_SIZE + 10, y * TILE_SIZE + 10);
                    window.draw(inner);
                }
                else if (type == COLOR_BOMB) {
                    // Draw colorful dots
                    sf::CircleShape dot(3);
                    for (int i = 0; i < 5; i++) {
                        dot.setFillColor(candies[i].getFillColor());
                        dot.setPosition(x * TILE_SIZE + 10 + i * 8, y * TILE_SIZE + 10 + (i % 2) * 10);
                        window.draw(dot);
                    }
                }
            }
        }
    }
}

bool swapCandies(sf::Vector2i a, sf::Vector2i b) {
    int& ca = grid[a.y][a.x];
    int& cb = grid[b.y][b.x];
    swap(ca, cb);

    bool crushArray[GRID_SIZE][GRID_SIZE];
    if (markMatches(crushArray)) {
        crushCandies(crushArray);
        processMatches();
        return true;
    }
    else {
        swap(ca, cb);
        return false;
    }
}

void checkGameStatus() {
    if (score >= TARGET_SCORE) {
        gameOver = true;
        wonGame = true;
        endText.setString("YOU WIN! Final score: " + to_string(score));
    }
    else if (movesLeft <= 0) {
        gameOver = true;
        endText.setString("YOU LOSE! Score: " + to_string(score) + " (Target: " + to_string(TARGET_SCORE) + ")");
    }
}

void saveGame(const string& filename) {
    ofstream file(filename);
    if (!file) return;
    file << movesLeft << " " << score << "\n";
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            file << grid[y][x] << " ";
        }
        file << "\n";
    }
}

bool loadGame(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    file >> movesLeft >> score;
    for (int y = 0; y < GRID_SIZE; y++)
        for (int x = 0; x < GRID_SIZE; x++)
            file >> grid[y][x];
    return true;
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    window.setFramerateLimit(60);

    initializeCandies();
    initializeUI();

    // Skip loading completely to test fresh game start
    initializeGrid();
    score = 0;
    movesLeft = MAX_MOVES;
    gameOver = false;
    wonGame = false;

    processMatches(); // Crush initial matches if any

    sf::Vector2i firstClick(-1, -1);
    bool firstSelected = false;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (!gameOver && e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::S)
                    saveGame("save.txt");
                else if (e.key.code == sf::Keyboard::P)
                    isPaused = true;
                else if (e.key.code == sf::Keyboard::R)
                    isPaused = false;
            }

            if (!isPaused && !gameOver && e.type == sf::Event::MouseButtonPressed) {
                int x = e.mouseButton.x / TILE_SIZE;
                int y = e.mouseButton.y / TILE_SIZE;
                if (isInBounds(x, y)) {
                    if (!firstSelected) {
                        firstClick = { x, y };
                        firstSelected = true;
                    }
                    else {
                        sf::Vector2i secondClick(x, y);
                        if ((abs(firstClick.x - x) == 1 && firstClick.y == y) ||
                            (abs(firstClick.y - y) == 1 && firstClick.x == x)) {
                            if (swapCandies(firstClick, secondClick)) {
                                movesLeft--;
                                checkGameStatus();
                            }
                        }
                        firstSelected = false;
                    }
                }
            }
        }

        updateUI();
        window.clear(sf::Color(50, 50, 50));
        drawGrid();
        window.draw(scoreText);
        window.draw(movesText);
        if (isPaused) window.draw(pauseText);
        if (gameOver) window.draw(endText);
        window.display();
    }

    return 0;
}

