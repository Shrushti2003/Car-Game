#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //This line gets a handle(console) to the standard output,which is the console window where text is displayed.
COORD CursorPosition; //(COORDINATE) use to specify cursor positions.Ex :- We set the X and Y coordinates of CursorPosition to 10(column position) and 5(row position),respectively.You can specify a specific location within the console.

int enemyY[3];      // Array to store the Y-coordinate (vertical position) of enemies.
int enemyx[3];     // Array to store the X-coordinate (horizontal position) of enemies.
int enemyFlag[3];  // Array to store flags indicating whether an enemy is active/visible (1) or not/not visible(0).

char car[4][4] = { ' ','+','+',' ',       // (green) Car design.
                    '+','+','+','+',
                    ' ','+','+',' ',
                    '+','+','+','+',};

int carPos = WIN_WIDTH/2; ////WIN_WIDTH:- (defined 70)represents the width of the game screen or console window. CarPos:- store the horizontal position(70/2 = 35) of the player's car,this would typically mean that the initial position of the player's car is centered horizontally on the screen at column 35, allowing it to move left or right.
int score = 0;
int lives = 3;  // Number of lives

void gotoxy(int x, int y) {  //gotoxy:-et the console cursor position to a specific coordinate(x,y).
    CursorPosition.X = x;   //sets the X-coordinate(column) of the CursorPosition structure to the provided x value.       (Ex :- gotoxy(10, 5); moves the cursor to column 10 and row 5,
    CursorPosition.Y = y;  //sets the Y-coordinate(row) of the CursorPosition structure to the provided y value.                  and then "Hello, World!" is printed at that location.)
    SetConsoleCursorPosition(console, CursorPosition); //moves the cursor to the desired position within the console window, used to set the console cursor position to the coordinates specified in CursorPosition.
}

void setcursor(bool visible, DWORD size) { //DWORD:- double word(32-bit unsigned integer);value specifying the cursor size,twice the size of something else.
    if (size == 0)
        size = 20;       // Default cursor size

    CONSOLE_CURSOR_INFO cursorInfo;  // declares a structure cursorInfo of type, CONSOLE_CURSOR_INFO:- used to store information about the console cursor.
    cursorInfo.bVisible = visible;  //sets the bVisible(boolean that controls the visibility of the cursor) member of cursorInfo to control the cursor's visibility based on the visible argument. true->show, false->hide.
    cursorInfo.dwSize = size;       //sets the dwSize member of cursorInfo to control the cursor's size based on the size argument.
    SetConsoleCursorInfo(console, &cursorInfo);  //apply the cursor settings stored in cursorInfo to the console, effectively changing the cursor's visibility and size.
}

void drawBorder(){
    for(int i=0; i<SCREEN_HEIGHT; i++){
        for(int j=0; j<17; j++){    //iterates to draw vertical lines on both the left and right sides of the game area
            gotoxy(0+j,i);        //move the cursor to the desired position for drawing the vertical line on the left side.
            cout<<"|";
            gotoxy(WIN_WIDTH-j,i);  //draw the vertical line on the right side.
            cout<<"|";
        }
    }
    for(int i=0; i<SCREEN_HEIGHT; i++){
        gotoxy(SCREEN_WIDTH,i);   //move the cursor to the far right edge (SCREEN_WIDTH) at the current row (i).
        cout<<"|";
    }
}

void updateScore() {
    gotoxy(WIN_WIDTH + 7, 5);  //moves the cursor horizontally to WIN_WIDTH + 7 and vertically to row 5.
    cout << "Score: " << score << endl;
}

void drawLives() {
    gotoxy(WIN_WIDTH + 7, 7); // This function moves the cursor to the specified position on the console window, where WIN_WIDTH + 7 is the x-coordinate and 7 is the y-coordinate. This positions the cursor to where the "Lives: " text will be displayed.
    cout << "Lives: ";
    for (int i = 1; i <= lives; i++) {   //loop iterates over the remaining lives
        SetConsoleTextAttribute(console, 0x0A); // Green color for remaining lives
        cout << i << " ";
    }
    for (int i = lives + 1; i <= 3; i++) {
        SetConsoleTextAttribute(console, 0x0C); // Red color for lost lives
        cout << i << " ";
    }
    SetConsoleTextAttribute(console, 0x07); // Reset to default color
}

void genEnemy(int ind){   // ind -> index
    enemyx[ind] = 17 + rand() % 33;   //generates a random number between 17 and 49 and assigns it to the enemyx position of the specified enemy.
}

void drawCar() {
    for (int i = 0; i < 4; i++) {   // i -> row
        for (int j = 0; j < 4; j++) {  // j -> column
            gotoxy(j + carPos, i + 22);  //move the console cursor to the specific position where the car character should be displayed, carPos:- horizontal position of the car, i + 22 represents the vertical position.
            SetConsoleTextAttribute(console, 0x0A);   // Green text
            cout << car[i][j];                       //draws the car's shape on the console screen.
        }
    }
}

void drawEnemy(int ind) {
    if (enemyFlag[ind] == true) {    //function proceeds to render the enemy's shape
        gotoxy(enemyx[ind], enemyY[ind]);  // moves the console cursor to the specific position where the enemy character should be displayed. The enemyx and enemyY arrays determine the horizontal and vertical positions of the enemy.
        SetConsoleTextAttribute(console, 0x0C);  // Red text
        cout << " **** ";
        gotoxy(enemyx[ind], enemyY[ind] + 1);    //moves the cursor,which is one row down from the current position of a game element specified by ind.
        SetConsoleTextAttribute(console, 0x0C);
        cout << "  **  ";
        gotoxy(enemyx[ind], enemyY[ind] + 2);     // Red text
        SetConsoleTextAttribute(console, 0x0C);
        cout << " **** ";
        gotoxy(enemyx[ind], enemyY[ind] + 3);      // Red text
        SetConsoleTextAttribute(console, 0x0C);
        cout << "  **  ";
    }
}

int collision() {
    for (int i = 0; i < 2; i++) {
        if (enemyFlag[i] == 1 && enemyY[i] + 4 >= 23) {  //checks whether the Y position of the first enemy (index 0) plus the height of the car (4) is greater than or equal to 23. Tests if the enemy is at or below row 23, which is typically the player's car position.
            if (enemyx[i] + 4 - carPos >= 0 && enemyx[i] - carPos <= 4) {   // hecks whether the rightmost X coordinate of the first enemy (plus its width, 4) minus the X coordinate of the car (carPos) is greater than or equal to 0.checks if the enemy is to the right of the car.
                return 1;                                               // or  if the enemy is to the left of the car within a certain range (4 units).
            }
        }
    }
    return 0;
}

void gameover() {
    system("cls");
    SetConsoleTextAttribute(console, 0x07); // White color for game over display
    cout << endl;
    cout << "\t\t----------------------------------------" << endl;
    cout << "\t\t------------  Game Over ----------------" << endl;
    cout << "\t\t----------------------------------------" << endl << endl;
    cout << "\t\tTotal Score: " << score << endl; // Display current game score
    Sleep(3000); // Wait for 3 seconds
    cout << "\t\t Press any key to go back to the menu.";
    getch();
    score = 0; // Reset score after game over
    lives = 3; // Reset lives after game over
}

void eraseCar(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            gotoxy(j+carPos, i+22);    // used to move the cursor to the position where the car was originally rendered. It calculates the correct position based on the carPos variable, which determines the horizontal position of the car, and i + 22, which determines the vertical position of the car. This ensures that the cursor is placed at the correct location for erasing the car.
            SetConsoleTextAttribute(console, 0x0F);
            cout<<" ";
        }
    }
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyx[ind], enemyY[ind]);           // Erase the first row
        cout << "       ";
        gotoxy(enemyx[ind], enemyY[ind] + 1);       // Erase the second row
        cout << "       ";
        gotoxy(enemyx[ind], enemyY[ind] + 2);      // Erase the third row
        cout << "       ";
        gotoxy(enemyx[ind], enemyY[ind] + 3);      // Erase the fourth row
        cout << "       ";
    }
}

void resetEnemy(int ind){
    eraseEnemy(ind);       // Erase the current enemy
    enemyY[ind] = 1;      // Reset the enemy's vertical position (typically at the top)
    genEnemy(ind);       // Generate a new horizontal position for the enemy
}

void play(){
    carPos = -1 + WIN_WIDTH/2;  //sets the initial horizontal position of the player's car (carPos) to be just to the left of the center of the game screen. WIN_WIDTH:- defined as 70,so this sets carPos to 34.
    score = 0; // Reset score at the start of each game
    lives = 3; // Reset lives at the start of each game
    enemyFlag[0] = 1;   //array used to indicate whether an enemy is active/visible (1) or not/not visible (0), first enemy (index 0) is active, and the second enemy (index 1) is not active at the beginning of the game.
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;  // sets the initial vertical positions of the two enemies (enemyY). Both enemies are set to be at row 1.

    system("cls");
    drawBorder();
    updateScore();
    drawLives();
    genEnemy(0);
    genEnemy(1);

    gotoxy(WIN_WIDTH + 7, 2);   //Moves the console cursor to the position defined by WIN_WIDTH + 7 columns to the right and 2 rows down from the top-left corner of the console screen.
    SetConsoleTextAttribute(console, 0x1E); // Dark blue color for heading
    cout<<" Car Game ";
    SetConsoleTextAttribute(console, 0x07);  // White text on green background
    gotoxy(WIN_WIDTH + 6, 4); cout<<"-----------";  //Displays a horizontal line of dashes under the title, creating a separator effect.
    gotoxy(WIN_WIDTH + 6, 6); cout<<"------------";
    gotoxy(WIN_WIDTH + 7, 12); cout<<" Control ";
    gotoxy(WIN_WIDTH + 7, 13); cout<<"-----------";
    gotoxy(WIN_WIDTH + 2, 14); cout<<" F Key - Left ";
    gotoxy(WIN_WIDTH + 2, 15); cout<<" J Key - Right ";

    gotoxy(18, 5);     //moves the cursor to the position (18, 5) on the console screen and displays the message
    cout<<"Press any key to start";
    getch();
    gotoxy(18, 5);   //After the user presses a key, it moves the cursor back to the same position (18, 5) and clears the message by overwriting it with spaces, effectively making it disappear from the screen.
    cout<<"                     ";

    while(true){   //loop is an infinite loop that represents the main game loop. The game continues until the player presses the "Esc" key (ASCII code 27).
        if(kbhit()){   //checks if a key has been pressed without blocking. If a key is pressed, it reads the key using getch() and stores it in the ch variable.
            char ch = getch();  //waits for the user to press a key. Once a key is pressed, it continues to the next part of the code.
            if(ch=='f' || ch=='F'){
                if(carPos > 18)  //After the user presses a key, it moves the cursor back to the same position (18, 5) and clears the message by overwriting it with spaces, effectively making it disappear from the screen.
                    carPos -= 4;
            }
            if(ch=='j' || ch=='J'){
                if(carPos < 50)   //if the current position of the car is to the left of the limit defined by 50)
                    carPos += 4;  //increments the carPos variable by 4.
            }
            if(ch == 27){
                break;
            }
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);

        if(collision()==1){   //checks if there is a collision between the player's car and any of the enemy cars.
            lives--;         //If a collision occurs, this decrements the number of lives by one.
            if (lives == 0) { //number of lives becomes zero,gameover() function to end the game and display the game over screen.
                gameover();
                return;
            } else {
                drawLives();
                resetEnemy(0);
                resetEnemy(1);
                drawEnemy(0);
                drawEnemy(1);
            }
        }

        Sleep(50);      //50-millisecond delay in the game loop. It adds a small pause between frames to control the game's speed.
        eraseCar();     //These lines clear the previous positions of
        eraseEnemy(0);  //the player's car and the two enemies from the screen. This prevents old positions from leaving
        eraseEnemy(1);  //traces on the screen.


        if (enemyY[0] == 10 && enemyFlag[1] == 0) {  //checks if the first enemy (enemyY[0]) is at a specific vertical position (row 10) and if the second enemy (enemyFlag[1]) is not active (equal to 0).If both conditions are true, it means that the first enemy has reached a certain point in the game (row 10) and the second enemy is not yet active.
            enemyFlag[1] = 1;       //sets enemyFlag[1] to 1, making the second enemy active. This likely introduces new challenges for the player.
        }

        if (enemyFlag[0] == 1) {   //checks if the first enemy is active.If the condition is true (i.e., the first enemy is active), it increments the vertical position of the first enemy (enemyY[0]) by 1. This effectively makes the enemy move down one row on the screen.
            enemyY[0] += 1;
        }

        if (enemyFlag[1] == 1) {   //checks if the second enemy is active.

            enemyY[1] += 1;
        }

        if (enemyY[0] > SCREEN_HEIGHT - 4) {   //checks if the first enemy has reached or passed the bottom of the game screen, and it's time to reset it.
            resetEnemy(0);  //first enemy has reached the bottom of the screen,to reset the first enemy's position to the top of the screen and generate a new horizontal position for it.
            score++;        //After resetting the first enemy, the player's score is incremented by 1,
            updateScore();  // display the updated score on the screen.
        }

        if (enemyY[1] > SCREEN_HEIGHT - 4) {
            resetEnemy(1);
            score++;
            updateScore();
        }
    }
}

void instructions() {
    cout << "Instructions of the game." << endl;
}

int main() {
    srand((unsigned)time(NULL));
    do {
        system("cls");
        gotoxy(10, 5); cout << " ------------------------------------------- ";
        gotoxy(10, 6);
        SetConsoleTextAttribute(console, 0x6E); // Dark yellow color for heading
        cout << " |                 Car Game                 |";
        SetConsoleTextAttribute(console, 0x07); // Reset to default color
        gotoxy(10, 7); cout << " --------------------------------------------";

        gotoxy(10, 9);  cout << " 1. Start Game ";
        gotoxy(10, 10); cout << " 2. Instructions ";
        gotoxy(10, 11); cout << " 3. Quit ";

        gotoxy(10, 13); cout << " Select option: ";
        char op = _getche();

        if (op == '1')
            play();
        else if (op == '2')
            instructions();
        else if (op == '3')
            break;

    } while (true);

    system("cls");

    return 0;
}
