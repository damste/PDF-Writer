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
#include <sstream>
#include <iomanip>
#include <cstring>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace PDFHummus;

// Cross-platform sleep function
void sleepMs(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

// SnakeGame Implementation
SnakeGame::SnakeGame(bool seedRandom) : currentDirection(RIGHT), score(0), gameOver(false), isSimulation(false), totalSteps(0) {
    // Initialize snake in the center
    snake.push_back(Position(GRID_WIDTH/2, GRID_HEIGHT/2));
    snake.push_back(Position(GRID_WIDTH/2-1, GRID_HEIGHT/2));
    snake.push_back(Position(GRID_WIDTH/2-2, GRID_HEIGHT/2));
    
    if (seedRandom) {
        srand(static_cast<unsigned int>(time(NULL)));
    }
    generateFood();
    startTime = time(NULL);
}

void SnakeGame::setSeed(unsigned int seed) {
    srand(seed);
    // Regenerate food with new seed
    generateFood();
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
    for (size_t i = 0; i < snake.size(); ++i) {
        if (snake[i] == pos) {
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
        endTime = time(NULL);
        return;
    }
    
    // Check self collision
    if (checkCollision(newHead)) {
        gameOver = true;
        endTime = time(NULL);
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
    // Only clear screen in interactive mode
    const char* interactive = getenv("SNAKE_INTERACTIVE");
    if (interactive && strcmp(interactive, "1") == 0) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    
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

void SnakeGame::step() {
    if (!gameOver) {
        moveSnake();
        if (isSimulation) {
            totalSteps++;
        }
    }
}

void SnakeGame::runInteractive() {
    isSimulation = false; // This is interactive mode
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
                endTime = time(NULL);
                break;
        }
        
        step();
        sleepMs(200);
    }
    
    drawGame();
    cout << endl << "GAME OVER!" << endl;
    cout << "Punteggio finale: " << score << endl;
    cout << "Lunghezza finale: " << snake.size() << endl;
    cout << "Tempo di gioco: " << fixed << setprecision(1) << getGameDuration() << " secondi" << endl;
    cout << endl << "Generazione del report PDF..." << endl;
}

void SnakeGame::runSimulation(int steps) {
    isSimulation = true; // This is simulation mode
    cout << "Esecuzione simulazione Snake (" << steps << " passi)..." << endl;
    
    // Simple AI: try to move towards food
    for (int i = 0; i < steps && !gameOver; i++) {
        // Simple AI logic: move towards food
        Position head = snake[0];
        Direction newDirection = currentDirection;
        
        // Try to move towards food
        if (food.x > head.x && currentDirection != LEFT) {
            newDirection = RIGHT;
        } else if (food.x < head.x && currentDirection != RIGHT) {
            newDirection = LEFT;
        } else if (food.y > head.y && currentDirection != UP) {
            newDirection = DOWN;
        } else if (food.y < head.y && currentDirection != DOWN) {
            newDirection = UP;
        }
        
        currentDirection = newDirection;
        step();
        
        // Occasionally change direction to make it more interesting
        if (i % 10 == 0 && rand() % 4 == 0) {
            Direction directions[] = {UP, DOWN, LEFT, RIGHT};
            Direction randomDir = directions[rand() % 4];
            // Only change if it won't immediately reverse
            if ((randomDir == UP && currentDirection != DOWN) ||
                (randomDir == DOWN && currentDirection != UP) ||
                (randomDir == LEFT && currentDirection != RIGHT) ||
                (randomDir == RIGHT && currentDirection != LEFT)) {
                currentDirection = randomDir;
            }
        }
    }
    
    endTime = time(NULL);
    
    cout << "Simulazione completata!" << endl;
    cout << "Punteggio finale: " << score << endl;
    cout << "Lunghezza finale: " << snake.size() << endl;
    if (gameOver) {
        cout << "Passi eseguiti: Gioco terminato prima" << endl;
    } else {
        stringstream ss;
        ss << steps;
        cout << "Passi eseguiti: " << ss.str() << endl;
    }
}

double SnakeGame::getGameDuration() const {
    if (isSimulation) {
        // For simulation: assume 200ms per step
        return totalSteps * 0.2;
    } else {
        // For interactive: use actual time
        return difftime(endTime, startTime);
    }
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
    
    // Check if interactive mode is requested
    const char* interactive = getenv("SNAKE_INTERACTIVE");
    if (interactive && strcmp(interactive, "1") == 0) {
        cout << "Modalità interattiva abilitata (SNAKE_INTERACTIVE=1)" << endl;
        SnakeGame game(true); // Seed with current time
        game.runInteractive();
        return generateGameReportPDF(inTestConfiguration, game);
    } else {
        cout << "Modalità simulazione (usa SNAKE_INTERACTIVE=1 per modalità interattiva)" << endl;
        SnakeGame game(false); // Don't seed randomly
        game.setSeed(42); // Fixed seed for reproducible results
        game.runSimulation(150); // Run simulation for 150 steps
        return generateGameReportPDF(inTestConfiguration, game);
    }
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
        
        // Try to get a font - use actual font files if available
        PDFUsedFont* titleFont = NULL;
        PDFUsedFont* textFont = NULL;
        
        // Try to load arial.ttf from TestMaterials (like other tests do)
        textFont = pdfWriter.GetFontForFile(
            RelativeURLToLocalPath(inTestConfiguration.mSampleFileBase, "TestMaterials/fonts/arial.ttf"));
        titleFont = textFont; // Use same font for title, just different size
        
        // Title
        contentContext->BT();
        contentContext->k(0, 0, 0, 100); // Black
        if (titleFont) {
            contentContext->Tf(titleFont, 24);
            contentContext->Td(50, 750);
            EStatusCode titleStatus = contentContext->Tj("SNAKE GAME - REPORT DI GIOCO");
            if (titleStatus != eSuccess) {
                cout << "Warning: Could not render title text" << endl;
            }
        } else {
            // Fallback: draw title as a rectangle if no font available
            contentContext->q();
            contentContext->k(0, 0, 0, 100);
            contentContext->re(50, 740, 400, 20);
            contentContext->f();
            contentContext->Q();
        }
        contentContext->ET();
        
        // Game statistics
        if (textFont) {
            contentContext->BT();
            contentContext->Tf(textFont, 14);
            contentContext->Td(50, 700);
            
            stringstream stats;
            stats << "Punteggio Finale: " << game.getScore();
            EStatusCode statStatus = contentContext->Tj(stats.str());
            if (statStatus != eSuccess) {
                cout << "Warning: Could not render statistics text" << endl;
            }
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
        } else {
            // Fallback: draw statistics as colored bars if no font
            contentContext->q();
            contentContext->k(0, 100, 0, 0); // Green for score
            int scoreWidth = std::min(300, game.getScore() * 3);
            contentContext->re(50, 700, scoreWidth, 10);
            contentContext->f();
            
            contentContext->k(0, 0, 100, 0); // Blue for length
            int lengthWidth = std::min(300, game.getSnakeLength() * 10);
            contentContext->re(50, 680, lengthWidth, 10);
            contentContext->f();
            contentContext->Q();
        }
        
        // Performance evaluation
        if (textFont) {
            contentContext->BT();
            contentContext->Tf(textFont, 16);
            contentContext->Td(50, 620);
            contentContext->Tj("VALUTAZIONE PERFORMANCE:");
            contentContext->ET();
            
            contentContext->BT();
            contentContext->Tf(textFont, 12);
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
        }
        
        // Draw a simple snake representation
        contentContext->q();
        contentContext->k(0, 100, 0, 0); // Green
        contentContext->w(5);
        
        // Snake body
        for (int i = 0; i < std::min(10, game.getSnakeLength()); i++) {
            contentContext->re(100 + i * 15, 400, 10, 10);
            contentContext->f();
        }
        
        // Food
        contentContext->k(100, 0, 0, 0); // Red
        contentContext->re(300, 400, 10, 10);
        contentContext->f();
        contentContext->Q();
        
        // Footer
        if (textFont) {
            contentContext->BT();
            contentContext->k(0, 0, 0, 50); // Gray
            contentContext->Tf(textFont, 10);
            contentContext->Td(50, 50);
            contentContext->Tj("Generato da PDF-Writer Snake Game Test");
            contentContext->ET();
        }
        
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
