/*
   Source File : SnakeGameTest.cpp


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
#include "SnakeGameTest.h"
#include "PDFWriter.h"
#include "PDFPage.h"
#include "PDFRectangle.h"
#include "PageContentContext.h"
#include "TestsRunner.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace PDFHummus;

// SnakeGame Implementation
SnakeGame::SnakeGame() : currentDirection(RIGHT), score(0), gameOver(false) {
    // Initialize snake in the center
    snake.push_back(Position(GRID_WIDTH/2, GRID_HEIGHT/2));
    snake.push_back(Position(GRID_WIDTH/2-1, GRID_HEIGHT/2));
    snake.push_back(Position(GRID_WIDTH/2-2, GRID_HEIGHT/2));
    
    srand(static_cast<unsigned int>(time(nullptr)));
    generateFood();
    startTime = chrono::steady_clock::now();
}

void SnakeGame::generateFood() {
    do {
        food.x = rand() % GRID_WIDTH;
        food.y = rand() % GRID_HEIGHT;
    } while (checkCollision(food));
}

bool SnakeGame::isValidPosition(const Position& pos) {
    return pos.x >= 0 && pos.x < GRID_WIDTH && pos.y >= 0 && pos.y < GRID_HEIGHT;
}

bool SnakeGame::checkCollision(const Position& pos) {
    for (const auto& segment : snake) {
        if (segment == pos) {
            return true;
        }
    }
    return false;
}

void SnakeGame::moveSnake() {
    Position newHead = snake[0];
    
    switch (currentDirection) {
        case UP: newHead.y--; break;
        case DOWN: newHead.y++; break;
        case LEFT: newHead.x--; break;
        case RIGHT: newHead.x++; break;
    }
    
    // Check wall collision
    if (!isValidPosition(newHead)) {
        gameOver = true;
        endTime = chrono::steady_clock::now();
        return;
    }
    
    // Check self collision
    if (checkCollision(newHead)) {
        gameOver = true;
        endTime = chrono::steady_clock::now();
        return;
    }
    
    snake.insert(snake.begin(), newHead);
    
    // Check if food eaten
    if (newHead == food) {
        score += 10;
        generateFood();
    } else {
        snake.pop_back();
    }
}

void SnakeGame::drawGame() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    
    cout << "=== SNAKE GAME ===" << endl;
    cout << "Score: " << score << " | Length: " << snake.size() << endl;
    cout << "Controls: W(up) A(left) S(down) D(right) Q(quit)" << endl;
    cout << endl;
    
    // Draw top border
    for (int i = 0; i < GRID_WIDTH + 2; i++) cout << "#";
    cout << endl;
    
    // Draw game area
    for (int y = 0; y < GRID_HEIGHT; y++) {
        cout << "#";
        for (int x = 0; x < GRID_WIDTH; x++) {
            Position current(x, y);
            if (current == snake[0]) {
                cout << "O"; // Head
            } else if (checkCollision(current)) {
                cout << "o"; // Body
            } else if (current == food) {
                cout << "*"; // Food
            } else {
                cout << " ";
            }
        }
        cout << "#" << endl;
    }
    
    // Draw bottom border
    for (int i = 0; i < GRID_WIDTH + 2; i++) cout << "#";
    cout << endl;
}

char SnakeGame::getInput() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) > 0) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        return ch;
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
#endif
    return 0;
}

void SnakeGame::run() {
    cout << "Benvenuto al gioco Snake!" << endl;
    cout << "Premi INVIO per iniziare..." << endl;
    cin.get();
    
    while (!gameOver) {
        drawGame();
        
        char input = getInput();
        switch (input) {
            case 'w': case 'W':
                if (currentDirection != DOWN) currentDirection = UP;
                break;
            case 's': case 'S':
                if (currentDirection != UP) currentDirection = DOWN;
                break;
            case 'a': case 'A':
                if (currentDirection != RIGHT) currentDirection = LEFT;
                break;
            case 'd': case 'D':
                if (currentDirection != LEFT) currentDirection = RIGHT;
                break;
            case 'q': case 'Q':
                gameOver = true;
                endTime = chrono::steady_clock::now();
                break;
        }
        
        moveSnake();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    
    drawGame();
    cout << endl << "GAME OVER!" << endl;
    cout << "Punteggio finale: " << score << endl;
    cout << "Lunghezza finale: " << snake.size() << endl;
    cout << "Tempo di gioco: " << fixed << setprecision(1) << getGameDuration() << " secondi" << endl;
    cout << endl << "Generazione del report PDF..." << endl;
}

double SnakeGame::getGameDuration() const {
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    return duration.count() / 1000.0;
}

// SnakeGameTest Implementation
SnakeGameTest::SnakeGameTest(void) {
}

SnakeGameTest::~SnakeGameTest(void) {
}

EStatusCode SnakeGameTest::Run(const TestConfiguration& inTestConfiguration) {
    cout << "=== SNAKE GAME TEST ===" << endl;
    cout << "Questo test esegue un gioco Snake e genera un report PDF." << endl;
    cout << endl;
    
    SnakeGame game;
    game.run();
    
    return generateGameReportPDF(inTestConfiguration, game);
}

EStatusCode SnakeGameTest::generateGameReportPDF(const TestConfiguration& inTestConfiguration, 
                                                const SnakeGame& game) {
    PDFWriter pdfWriter;
    EStatusCode status;
    
    do {
        status = pdfWriter.StartPDF(RelativeURLToLocalPath(inTestConfiguration.mSampleFileBase, "SnakeGameReport.pdf"), ePDFVersion13);
        if (status != eSuccess) {
            cout << "Errore nell'avvio del PDF" << endl;
            break;
        }
        
        PDFPage* page = new PDFPage();
        page->SetMediaBox(PDFRectangle(0, 0, 595, 842));
        
        PageContentContext* contentContext = pdfWriter.StartPageContentContext(page);
        if (NULL == contentContext) {
            status = eFailure;
            cout << "Errore nella creazione del contesto della pagina" << endl;
            break;
        }
        
        // Title
        contentContext->BT();
        contentContext->k(0, 0, 0, 100); // Black
        contentContext->Tf(pdfWriter.GetFontForFile("Helvetica"), 24);
        contentContext->Td(50, 750);
        contentContext->Tj("SNAKE GAME - REPORT DI GIOCO");
        contentContext->ET();
        
        // Game statistics
        contentContext->BT();
        contentContext->Tf(pdfWriter.GetFontForFile("Helvetica"), 14);
        contentContext->Td(50, 700);
        
        stringstream stats;
        stats << "Punteggio Finale: " << game.getScore();
        contentContext->Tj(stats.str());
        contentContext->ET();
        
        contentContext->BT();
        contentContext->Td(50, 680);
        stats.str("");
        stats << "Lunghezza Finale del Serpente: " << game.getSnakeLength();
        contentContext->Tj(stats.str());
        contentContext->ET();
        
        contentContext->BT();
        contentContext->Td(50, 660);
        stats.str("");
        stats << "Durata del Gioco: " << fixed << setprecision(1) << game.getGameDuration() << " secondi";
        contentContext->Tj(stats.str());
        contentContext->ET();
        
        // Performance evaluation
        contentContext->BT();
        contentContext->Tf(pdfWriter.GetFontForFile("Helvetica"), 16);
        contentContext->Td(50, 620);
        contentContext->Tj("VALUTAZIONE PERFORMANCE:");
        contentContext->ET();
        
        contentContext->BT();
        contentContext->Tf(pdfWriter.GetFontForFile("Helvetica"), 12);
        contentContext->Td(50, 590);
        
        string performance;
        if (game.getScore() >= 100) {
            performance = "ECCELLENTE! Sei un maestro del Snake!";
        } else if (game.getScore() >= 50) {
            performance = "BUONO! Continua cosi!";
        } else if (game.getScore() >= 20) {
            performance = "DISCRETO. Puoi migliorare!";
        } else {
            performance = "PRINCIPIANTE. Continua a praticare!";
        }
        
        contentContext->Tj(performance);
        contentContext->ET();
        
        // Draw a simple snake representation
        contentContext->q();
        contentContext->k(0, 100, 0, 0); // Green
        contentContext->w(5);
        
        // Snake body
        for (int i = 0; i < min(10, game.getSnakeLength()); i++) {
            contentContext->re(100 + i * 15, 400, 10, 10);
            contentContext->f();
        }
        
        // Food
        contentContext->k(100, 0, 0, 0); // Red
        contentContext->re(300, 400, 10, 10);
        contentContext->f();
        contentContext->Q();
        
        // Footer
        contentContext->BT();
        contentContext->k(0, 0, 0, 50); // Gray
        contentContext->Tf(pdfWriter.GetFontForFile("Helvetica"), 10);
        contentContext->Td(50, 50);
        contentContext->Tj("Generato da PDF-Writer Snake Game Test");
        contentContext->ET();
        
        status = pdfWriter.EndPageContentContext(contentContext);
        if (status != eSuccess) {
            cout << "Errore nella chiusura del contesto della pagina" << endl;
            break;
        }
        
        status = pdfWriter.WritePageAndRelease(page);
        if (status != eSuccess) {
            cout << "Errore nella scrittura della pagina" << endl;
            break;
        }
        
        status = pdfWriter.EndPDF();
        if (status != eSuccess) {
            cout << "Errore nella chiusura del PDF" << endl;
            break;
        }
        
        cout << "Report PDF generato con successo: SnakeGameReport.pdf" << endl;
        
    } while (false);
    
    return status;
}

ADD_CATEGORIZED_TEST(SnakeGameTest, "Games")
