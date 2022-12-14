#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>
#include<time.h>
#pragma warning (disable:4996)


//x y 좌표 결정해주기 위해 gotoxy함수 선언
void gotoxy(int x, int y);


//게임 화면 프레임 잡기
int frameArr[21][12] = { 0, };   //화면 프레임 배열     0:공백, 1:틀, 2:내려오는 블록, 3:굳어버린 블록
int score = 0;      //현재 스코어
int level = 1;      //현재 레벨
void scoreUpdate(int);


//내려오는 블록 구현
int blockArr[4][4] = { 0, };   //빈칸은 0을 넣고, 블록 모양대로 2를 넣어주기
int blockX, blockY;      //블록의 (0,0) 위치를 기록할건데
int blockChoice;
int blockVersion;
int blockFlag = 1;      //블록을 멈출건지 내릴건지 확인하는 플래그       // 0 : 스탑 / 1 :고
void blockShape(int, int);
void block1(int);
void block2(int);
void block3(int);
void block4(int);
void block5(int);
void block6(int);
void block7(int);
void blockEnter(int, int);
int right = 0;


//스크린 출력 함수 (블록 및 프레임 모두 포함)
void printScreen();
//스크린 움직임 나타내는 함수
void moveScreen();
//스크린 지워주는 함수
void clearScreen();
//한줄 완성시, 해당 라인 지워주는 함수
void lineComplete(int);


//게임오버 플래그
int gameOverFlag = 0;


int main() {


    //랜덤함수 준비
    srand((unsigned)time(NULL));



    //게임 틀 구성 (벽은 1, 공간은 0)
    for (int i = 0; i < 21; i++) {
        frameArr[i][0] = 1;
        frameArr[i][11] = 1;
    }
    for (int j = 0; j < 12; j++) {
        frameArr[20][j] = 1;
    }


    //블록 시작되는 처음 위치 및 블록 모양 랜덤으로 결정
    blockY = 0, blockX = rand() % 7 + 1;
    blockChoice = rand() % 7 + 1;
    blockVersion = rand() % 4 + 1;

    //천장에 닿지 않을 때까지 반복
    while (gameOverFlag == 0) {


        //내리던 것 계속 내리기
        int nowTime = GetTickCount64();
        while (blockFlag == 1) {
            if (GetTickCount64() > nowTime + 1000) {
                nowTime = GetTickCount64();

                clearScreen();   //지우고
                blockY++;
                printScreen();   //그리고
            }
            moveScreen();   //움직이고


        }

        //내리던 것 멈추기, 그리고 새로운 것 출력 준비
        if (blockFlag == 0) {

            //새로운 것 출력 준비
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    blockArr[i][j] = 0;
            blockFlag = 1;
            blockY = 0, blockX = rand() % 7 + 1;
            blockChoice = rand() % 7 + 1;
            blockVersion = rand() % 4 + 1;
        }
    }


    printf("Game Over!!");


    return 0;
}


//gotoxy함수 정의
void gotoxy(int x, int y) {
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}



//블록 모양 알려주는 함수
void blockShape(int choice, int version) {

    int p = version;

    switch (choice) {
    case 1: block1(p); break;
    case 2: block2(p); break;
    case 3: block3(p); break;
    case 4: block4(p); break;
    case 5: block5(p); break;
    case 6: block6(p); break;
    case 7: block7(p); break;
    }

    blockVersion = p;
}


//키 조작 함수
void moveScreen() {
    clearScreen();   //지우고
    char pressKey;

    if (_kbhit()) {
        pressKey = _getch();


        switch (pressKey) {

        case 75:   //왼쪽방향
            if (frameArr[blockY][blockX - 1] != 1 && frameArr[blockY][blockX - 1] != 3)
                blockX--;
            break;


        case 77:   //오른쪽방향
            if (frameArr[blockY][blockX + right + 1] != 1 && frameArr[blockY][blockX + right + 1] != 3)
                blockX++;
            break;


        case 80:   //아래쪽방향
            if (frameArr[blockY + 1][blockX] != 1 && frameArr[blockY + 1][blockX] != 3)
                blockY++;
            break;


        case 32:   //스페이스바 모양변경!
            blockEnter(blockChoice, blockVersion);
            break;
        }


        printScreen();   //그리고

    }


}


//화면 출력 함수
void printScreen() {

    //테트리스!
    gotoxy(44, 4);
    printf("Tetris!");

    //게임방법
    gotoxy(44, 8);
    printf("   <<<<<<<< Game Rule >>>>>>>>");
    gotoxy(44, 10);
    printf("← :left    ↓: down    → : right\n");
    gotoxy(44, 12);
    printf("     space bar : shape change");


    //score 출력
    gotoxy(44, 16);
    printf("socre : %d", score);

    /*
    //level 출력
    gotoxy(44, 19);
    printf("level : %d", level);
    */

    //블록 모양 결정해서 프레임에 블록 모양 넣기
    blockShape(blockChoice, blockVersion);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockArr[i][j] == 2 && frameArr[blockY + i][blockX + j] != 1)
                frameArr[blockY + i][blockX + j] = 2;
        }
    }


    //게임 화면 프레임 출력 (콘솔창에서 x축은 10만큼, y축은 3만큼 띄어줌)
    int x = 10, y = 3;

    int flag = 0;   //만약 접한다면 flag를 1로 바꾸어 줄 것임!!
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 12; j++) {
            gotoxy(x + j * 2, y + i);
            if (frameArr[i][j] == 1) printf("▩");
            else if (frameArr[i][j] == 2 || frameArr[i][j] == 3) printf("■");
            else printf("  ");

            if (i < 20 && ((frameArr[i][j] == 2 && frameArr[i + 1][j] == 1) || frameArr[i][j] == 2 && frameArr[i + 1][j] == 3)) {
                flag = 1;
            }
        }
        x = 10;
    }



    //콤보 점수 부여를 위해 콤보 개수 셈
    int cnt = 0;

    //접하는 상황!
    if (flag == 1) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {

                //굳힐 것들 굳혀버리기
                if (frameArr[blockY + i][blockX + j] == 2) {
                    frameArr[blockY + i][blockX + j] = 3;

                }
            }

            //라인 완성 여부 확인
            int flg = 0, line = blockY + i;
            for (int k = 1; k < 11; k++) {
                if (frameArr[blockY + i][k] != 3) { flg = 1; break; }
            }
            if (flg == 0) {//라인완성!
                lineComplete(line);
                cnt++;
            }

            //게임 오버일 경우도 함께 고려하기
            if (blockY + i == 0) {
                gameOverFlag = 1;
            }

        }
        blockFlag = 0;
    }
    scoreUpdate(cnt);


}


void lineComplete(int line) {

    for (int j = 1; j < 11; j++) {
        frameArr[line][j] = 0;
    }


    for (int i = 0; i < line; i++) {
        for (int j = 1; j < 11; j++)
            frameArr[line - i][j] = frameArr[line - i - 1][j];
    }

}

void scoreUpdate(int combo) {
    score += 100 * combo;
    if (combo != 1) {
        score += 25 * combo;
    }
}

void clearScreen() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (frameArr[blockY + i][blockX + j] != 1 && frameArr[blockY + i][blockX + j] != 3)
                frameArr[blockY + i][blockX + j] = 0;
        }
    }
}





void blockEnter(int choice, int version) {

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            blockArr[i][j] = 0;

    version++;
    if (version > 4) version -= 4;

    blockShape(choice, version);
}

//블록 종류 나열!!
void block1(int x) {

    switch (x) {

    case 1:
        for (int i = 0; i < 4; i++)
            blockArr[i][0] = 2;
        right = 0;
        break;
    case 2:
        for (int j = 0; j < 4; j++)
            blockArr[0][j] = 2;
        right = 3;
        break;
    case 3:
        for (int i = 0; i < 4; i++)
            blockArr[i][0] = 2;
        right = 0;
        break;
    case 4:
        for (int j = 0; j < 4; j++)
            blockArr[0][j] = 2;
        right = 3;
        break;
    }

}
void block2(int x) {

    switch (x) {
    case 1:
        for (int i = 0; i < 3; i++)
            blockArr[i][1] = 2;
        blockArr[1][0] = 2;
        right = 1;
        break;

    case 2:
        for (int j = 0; j < 3; j++)
            blockArr[0][j] = 2;
        blockArr[1][1] = 2;
        right = 2;
        break;

    case 3:
        for (int i = 0; i < 3; i++)
            blockArr[i][0] = 2;
        blockArr[1][1] = 2;
        right = 1;
        break;

    case 4:
        for (int j = 0; j < 3; j++)
            blockArr[1][j] = 2;
        blockArr[0][1] = 2;
        right = 2;
        break;
    }


}
void block3(int x) {
    switch (x) {
    case 1:
        for (int i = 0; i < 3; i++)
            blockArr[i][0] = 2;
        blockArr[2][1] = 2;
        right = 1;
        break;

    case 2:
        for (int j = 0; j < 3; j++)
            blockArr[1][j] = 2;
        blockArr[0][2] = 2;
        right = 2;
        break;

    case 3:
        for (int i = 0; i < 3; i++)
            blockArr[i][1] = 2;
        blockArr[0][0] = 2;
        right = 1;
        break;

    case 4:
        for (int j = 0; j < 3; j++)
            blockArr[0][j] = 2;
        blockArr[1][0] = 2;
        right = 2;
        break;
    }
}
void block4(int x) {
    switch (x) {
    case 1:
    case 2:
    case 3:
    case 4:
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                blockArr[i][j] = 2;
        right = 1;
        break;

    }
}
void block5(int x) {

    switch (x) {
    case 1:
        for (int i = 0; i < 3; i++)
            blockArr[i][1] = 2;
        blockArr[2][0] = 2;
        right = 1;
        break;
    case 2:
        for (int j = 0; j < 3; j++)
            blockArr[0][j] = 2;
        blockArr[1][2] = 2;
        right = 2;
        break;
    case 3:
        for (int i = 0; i < 3; i++)
            blockArr[i][0] = 2;
        blockArr[0][1] = 2;
        right = 1;
        break;
    case 4:
        for (int j = 0; j < 3; j++)
            blockArr[1][j] = 2;
        blockArr[0][0] = 2;
        right = 2;
        break;
    }

}
void block6(int x) {

    switch (x) {
    case 1:
        for (int j = 1; j < 3; j++)
            blockArr[0][j] = 2;
        for (int j = 0; j < 2; j++)
            blockArr[1][j] = 2;
        right = 2;
        break;
    case 2:
        for (int i = 0; i < 2; i++)
            blockArr[i][0] = 2;
        for (int i = 1; i < 3; i++)
            blockArr[i][1] = 2;
        right = 1;
        break;
    case 3:
        for (int j = 1; j < 3; j++)
            blockArr[0][j] = 2;
        for (int j = 0; j < 2; j++)
            blockArr[1][j] = 2;
        right = 2;
        break;
    case 4:
        for (int i = 0; i < 2; i++)
            blockArr[i][0] = 2;
        for (int i = 1; i < 3; i++)
            blockArr[i][1] = 2;
        right = 1;
        break;
    }

}
void block7(int x) {

    switch (x) {
    case 1:
        for (int j = 0; j < 2; j++)
            blockArr[0][j] = 2;
        for (int j = 1; j < 3; j++)
            blockArr[1][j] = 2;
        right = 2;
        break;
    case 2:
        for (int i = 0; i < 2; i++)
            blockArr[i][1] = 2;
        for (int i = 1; i < 3; i++)
            blockArr[i][0] = 2;
        right = 1;
        break;
    case 3:
        for (int j = 0; j < 2; j++)
            blockArr[0][j] = 2;
        for (int j = 1; j < 3; j++)
            blockArr[1][j] = 2;
        right = 2;
        break;
    case 4:
        for (int i = 0; i < 2; i++)
            blockArr[i][1] = 2;
        for (int i = 1; i < 3; i++)
            blockArr[i][0] = 2;
        right = 1;
        break;
    }
}