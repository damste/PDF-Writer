/*
   Source File : SnakeGameTest.h


   Copyright 2024 PDF-Writer Snake Game

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   
*/
#pragma once
#include "ITestUnit.h"
#include <vector>
#include <utility>
#include <algorithm>
#include <ctime>

struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

class SnakeGame {
private:
    static const int GRID_WIDTH = 20;
    static const int GRID_HEIGHT = 15;
    
    std::vector<Position> snake;
    Position food;
    Direction currentDirection;
    int score;
    bool gameOver;
    time_t startTime;
    time_t endTime;
    int totalSteps; // For simulation duration calculation
    
    void generateFood();
    bool isValidPosition(const Position& pos);
    bool checkCollision(const Position& pos);
    void moveSnake();
    void drawGame();
    char getInput();
    
public:
    SnakeGame(bool seedRandom = true);
    void runInteractive();
    void runSimulation(int steps = 100);
    int getScore() const { return score; }
    double getGameDuration() const;
    int getSnakeLength() const { return static_cast<int>(snake.size()); }
    bool isGameOver() const { return gameOver; }
    void step(); // Single game step for simulation
    void setSeed(unsigned int seed); // For deterministic simulation
};

class SnakeGameTest : public ITestUnit
{
public:
    SnakeGameTest(void);
    ~SnakeGameTest(void);

    virtual PDFHummus::EStatusCode Run(const TestConfiguration& inTestConfiguration);
    
private:
    PDFHummus::EStatusCode generateGameReportPDF(const TestConfiguration& inTestConfiguration, 
                                                const SnakeGame& game);
};
