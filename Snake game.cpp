#include<bits/stdc++.h>
#include<conio.h> 
#include<windows.h>

using namespace std;

#define MAX_LENGTH 1000

// Directions
const char MOVE_UP = 'U';
const char MOVE_DOWN = 'D';
const char MOVE_LEFT = 'L';
const char MOVE_RIGHT = 'R'; 

int windowWidth, windowHeight;

void initConsole()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    GetConsoleScreenBufferInfo(consoleHandle, &screenBufferInfo);
    windowHeight = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;
    windowWidth = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
}

struct Position{
    int xPos;
    int yPos;
    Position(){
    }
    Position(int x, int y)
    {
        xPos = x;
        yPos = y;
    }
};

class SnakeGame{ 
    int snakeLength;
    char currentDirection;
public:
    Position snakeBody[MAX_LENGTH];
    SnakeGame(int x, int y)
    {
       snakeLength = 1;
       snakeBody[0] = Position(x,y);
       currentDirection = MOVE_RIGHT;
    }

    int getSnakeLength(){
        return snakeLength;
    }

    void setDirection(char newDirection){
        if(newDirection == MOVE_UP && currentDirection != MOVE_DOWN)
        {
            currentDirection = newDirection;
        }
        else if(newDirection == MOVE_DOWN && currentDirection != MOVE_UP)
        {
            currentDirection = newDirection;
        }
        else if(newDirection == MOVE_LEFT && currentDirection != MOVE_RIGHT)
        {
            currentDirection = newDirection;
        }
        else if(newDirection == MOVE_RIGHT && currentDirection != MOVE_LEFT)
        {
            currentDirection = newDirection;
        }
    }

    bool moveSnake(Position foodItem){
        for(int i= snakeLength-1; i>0; i--)
        {
            snakeBody[i] = snakeBody[i-1];
        }

        switch(currentDirection)
        {
            int value;
            case MOVE_UP:
                value = snakeBody[0].yPos;
                snakeBody[0].yPos = value-1;
                break;
            case MOVE_DOWN:
                value = snakeBody[0].yPos;
                snakeBody[0].yPos = value+1;
                break;
            case MOVE_RIGHT:
                value = snakeBody[0].xPos;
                snakeBody[0].xPos = value+1;
                break;
            case MOVE_LEFT:
                value = snakeBody[0].xPos;
                snakeBody[0].xPos = value-1;
                break;
        }

        // Self-collision check
        for(int i=1; i<snakeLength; i++)
        {
            if(snakeBody[0].xPos == snakeBody[i].xPos && snakeBody[0].yPos == snakeBody[i].yPos)
            {
                return false;
            }
        }

        // Check if snake eats food
        if(foodItem.xPos == snakeBody[0].xPos && foodItem.yPos == snakeBody[0].yPos)
        {
            snakeBody[snakeLength] = Position(snakeBody[snakeLength-1].xPos, snakeBody[snakeLength-1].yPos);
            snakeLength++;
        }

        return true;
    }
};

class GameBoard{
    SnakeGame *snake;
    const char SNAKE_PART = 'O';
    Position foodItem;
    const char FOOD_ITEM = 'o';
    int playerScore;
public:
    GameBoard(){
        spawnFoodItem();
        snake = new SnakeGame(10,10);
        playerScore = 0;
    }

    ~GameBoard(){
        delete snake;
    }

    int getPlayerScore(){
        return playerScore;
    }

    void spawnFoodItem(){
        int x = rand() % windowWidth;
        int y = rand() % windowHeight;
        foodItem = Position(x, y);
    }

    void showScore(){
        moveToPosition(windowWidth/2, 0);
        cout << "Current Score : " << playerScore;
    }

    void moveToPosition(int x, int y)
    {
        COORD cursorPos;
        cursorPos.X = x;
        cursorPos.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
    }

    void renderBoard(){
        system("cls");
        for(int i=0; i<snake->getSnakeLength(); i++)
        {
            moveToPosition(snake->snakeBody[i].xPos, snake->snakeBody[i].yPos);
            cout << SNAKE_PART;
        }

        moveToPosition(foodItem.xPos, foodItem.yPos);
        cout << FOOD_ITEM;

        showScore();
    }

    bool updateBoard(){
        bool alive = snake->moveSnake(foodItem);
        if(!alive)
        {
            return false;
        }

        if(foodItem.xPos == snake->snakeBody[0].xPos && foodItem.yPos == snake->snakeBody[0].yPos)
        {
            playerScore++;
            spawnFoodItem();
        }
        return true;
    }

    void getPlayerInput(){
        if(kbhit())
        {
            int key = getch();
            if(key == 'w' || key == 'W')
            {
                snake->setDirection(MOVE_UP);
            }
            else if(key == 'a' || key == 'A')
            {
                snake->setDirection(MOVE_LEFT);
            }
            else if(key == 's' || key == 'S')
            {
                snake->setDirection(MOVE_DOWN);
            }
            else if(key == 'd' || key == 'D')
            {
                snake->setDirection(MOVE_RIGHT);
            }
        }
    }
};

int main(){
    srand(time(0));
    initConsole();
    GameBoard *gameBoard = new GameBoard();
    while(gameBoard->updateBoard())
    {
        gameBoard->getPlayerInput();
        gameBoard->renderBoard();
        Sleep(100);
    }

    cout << "Game over" << endl;
    cout << "Final score is :" << gameBoard->getPlayerScore();
    return 0;
}
