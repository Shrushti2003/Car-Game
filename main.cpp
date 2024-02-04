#include <iostream>
#include<conio.h>
#include<windows.h>
#include<cstdlib>
#include<time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyx[3];
int enemyFlag[3];

char car[4][4] = { ' ','+','+',' ',
                    '+','+','+','+',
                    ' ','+','+',' ',
                    '+','+','+','+',};

int carPos = WIN_WIDTH/2;
int score = 0;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size) {
    if (size == 0)
        size = 20;  // Default cursor size

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = visible;
    cursorInfo.dwSize = size;
    SetConsoleCursorInfo(console, &cursorInfo);
}

void drawBorder(){
for(int i=0; i<SCREEN_HEIGHT; i++){
    for(int j=0; j<17; j++){
            gotoxy(0+j,i);
            cout<<"|";
            gotoxy(WIN_WIDTH-j,i);
            cout<<"|";
    }
    }
    for(int i=0; i<SCREEN_HEIGHT; i++){
        gotoxy(SCREEN_WIDTH,i);
        cout<<"|";
    }
}

void updateScore(){
      gotoxy(WIN_WIDTH + 7, 5);
      cout<<"Score: "<<score<<endl;
}

void genEnemy(int ind){
      enemyx[ind] = 17 + rand()%(33);
}

void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + 22);
            SetConsoleTextAttribute(console, 0x0A); // Green text
            cout << car[i][j];
        }
    }
}


void drawEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyx[ind], enemyY[ind]);
        SetConsoleTextAttribute(console, 0x0C); // Red text
        cout << " **** ";
        gotoxy(enemyx[ind], enemyY[ind] + 1);
        SetConsoleTextAttribute(console, 0x0C); // Red text
        cout << "  **  ";
        gotoxy(enemyx[ind], enemyY[ind] + 2);
        SetConsoleTextAttribute(console, 0x0C); // Red text
        cout << " **** ";
        gotoxy(enemyx[ind], enemyY[ind] + 3);
        SetConsoleTextAttribute(console, 0x0C); // Red text
        cout << "  **  ";
    }
}


int collision() {
    if (enemyY[0] + 4 >= 23) {
        if (enemyx[0] + 4 - carPos >= 0 && enemyx[0] - carPos <= 4) {
            return 1;
        }
    }
    return 0;
}


void gameover() {
    system("cls");
    SetConsoleTextAttribute(console, 0x0F); // White text on default black background
    cout << endl;
    cout << "\t\t----------------------------------------" << endl;
    cout << "\t\t------------  Game Over ----------------" << endl;
    cout << "\t\t----------------------------------------" << endl << endl;
    cout << "\t\t Press any key to go back to the menu.";
    getch();
    SetConsoleTextAttribute(console, 0x0F); // White text on default black background

}


void eraseCar(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            gotoxy(j+carPos, i+22);
            SetConsoleTextAttribute(console, 0x0F); // White text on default black background
            cout<<" ";
        }
    }
}
void eraseEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyx[ind], enemyY[ind]);      cout << "       ";
        gotoxy(enemyx[ind], enemyY[ind] + 1); cout << "       ";
        gotoxy(enemyx[ind], enemyY[ind] + 2); cout << "       ";
        gotoxy(enemyx[ind], enemyY[ind] + 3); cout << "       ";
    }
}


void resetEnemy(int ind){
       eraseEnemy(ind);
       enemyY[ind] = 1;
       genEnemy(ind);
}
void play(){
carPos = -1 + WIN_WIDTH/2;
score = 0;
enemyFlag[0] = 1;
enemyFlag[1] = 0;
enemyY[0] = enemyY[1] = 1;

system("cls");
drawBorder();
updateScore();
genEnemy(0);
genEnemy(1);

gotoxy(WIN_WIDTH + 7, 2);
SetConsoleTextAttribute(console, 0x0A); // White text on green background
 cout<<" Car Game ";
SetConsoleTextAttribute(console, 0x07); // White text on default black background
gotoxy(WIN_WIDTH + 6, 4); cout<<"-----------";
gotoxy(WIN_WIDTH + 6, 6); cout<<"------------";
gotoxy(WIN_WIDTH + 7, 12); cout<<" Control ";
gotoxy(WIN_WIDTH + 7, 13); cout<<"-----------";
gotoxy(WIN_WIDTH + 2, 14); cout<<" A Key - Left ";
gotoxy(WIN_WIDTH + 2, 15); cout<<" D Key - Right ";

gotoxy(18, 5);
cout<<"Press any key to start";
getch();
gotoxy(18, 5);
cout<<"                     ";

while(true){
    if(kbhit()){
        char ch = getch();
        if(ch=='a' || ch=='A'){
            if(carPos > 18)
                carPos -= 4;
        }
        if(ch=='d' || ch=='D'){
            if(carPos < 50)
                carPos += 4;
        }
        if(ch == 27){
            break;
        }
    }

    drawCar();
    drawEnemy(0);
    drawEnemy(1);

    if(collision()==1){
        gameover();
        return;
    }

Sleep(50);
eraseCar();
eraseEnemy(0);
eraseEnemy(1);

if (enemyY[0] == 10 && enemyFlag[1] == 0) {
    enemyFlag[1] = 1;
}

if (enemyFlag[0] == 1) {
    enemyY[0] += 1;
}

if (enemyFlag[1] == 1) {
    enemyY[1] += 1;
}

if (enemyY[0] > SCREEN_HEIGHT - 4) {
    resetEnemy(0);
    score++;
    updateScore();
}

if (enemyY[1] > SCREEN_HEIGHT - 4) {
    resetEnemy(1);
    score++;
    updateScore();
}

}
}
/*void setcursor(bool visible, DWORD size) {
    if (size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = visible;
    cursorInfo.dwSize = size;
    SetConsoleCursorInfo(console, &cursorInfo);
}*/

void instructions() {
    // Implement the instructions for your game
    cout << "Instructions of the game." << endl;
    // ... additional code ...
}



int main() {
    srand((unsigned)time(NULL));
    do {
        system("cls");
        gotoxy(10, 5); cout << " ------------------------------------------- ";
        gotoxy(10, 6); cout << " |                 Car Game                 |";
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
            exit(0);

    } while (true);

    return 0;
}
