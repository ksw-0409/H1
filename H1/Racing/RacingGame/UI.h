#pragma once
#include <iostream>
#include <windows.h>
#include "conio.h"
#include "training.h"
#include "horse.h"
#define UP 72 // 위
#define DOWN 80 // 아래
const int MAX_MONTH = 54;
extern const int startStat;
using namespace std;


void init_console_size() {
    system("mode con:cols=124 lines=35");
}

void gotoxy(int x, int y) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 커서 핸들가져오기
    COORD pos;
    pos.X = x; // x좌표
    pos.Y = y; // y좌표	
    SetConsoleCursorPosition(consoleHandle, pos); // 커서 위치 설정
}



int keyControl() {
    char temp = _getch();

    if (temp == 72) // 위
        return UP;
    else if (temp == 80) // 아래
        return DOWN;
    else if (temp == 27) // ESC
        return 27;
    else if (temp == 13) // Enter
        return 13;
    else
        return -1; // 잘못된 입력
}

void drawArrow(int x, int y) {
    gotoxy(x, y);
    printf(">");
}

void clearArrow(int x, int y) {
    gotoxy(x, y);
    printf(" "); // 이전 위치의 '>'를 지움
}

int menuDraw() {
    int x = 52;
    int y = 16;
    int z = 40; // x는 좌우를 의미하며 x가 -의 숫자가 더커질수록 왼쪽 작을수록 오른쪽 z는 그걸 조정해주는 변수.
    int arrowX = x + 4; // '>' 기호 위치
    int currentY = y + 10; // 현재 선택된 항목의 y좌표

    cout << "\n\n\n ============================================================================================================================\n" << endl;
    gotoxy(x - z, y - 7);
    printf("                                         :::::::            :::::::            :::::::: \n");//1
    gotoxy(x - z, y - 6);
    printf("                                       :::::::            :::::::          :::::::::: \n");//1

    gotoxy(x - z, y - 5);
    printf("                                     :+:+:+:            :+:+:+:        +:+:+:+:+:+:  \n");//2
    gotoxy(x - z, y - 4);
    printf("                                   :+:+:+:            :+:+:+:             ::+:+::  \n");//2

    gotoxy(x - z, y - 3);
    printf("                                 +++:+++            +++:+++             ++:+:++   \n");//3
    gotoxy(x - z, y - 2);
    printf("                               +++:+++            +++:+++             ++:+:++   \n");//3

    gotoxy(x - z, y - 1);
    printf("                             +++#++:+++++++++++:++#+++              ++#+#++ \n");//4
    gotoxy(x - z, y);
    printf("                           +++#++:+++++++++++:++#+++              ++#+#++ \n");//4

    gotoxy(x - z, y + 1);
    printf("                         #++#++#            #++#++#             #+#+#+#     \n");//5
    gotoxy(x - z, y + 2);
    printf("                       #++#++#            #++#++#             #+#+#+#     \n");//5

    gotoxy(x - z, y + 3);
    printf("                     #++#++#            #++#++#             ##+#+##       \n");
    gotoxy(x - z, y + 4);
    printf("                   #++#++#            #++#++#             ##+#+##       \n");

    gotoxy(x - z, y + 5);
    //gotoxy(x-50, y );
    printf("                 #######            #######         ##################     ");
    gotoxy(x - z, y + 6);
    printf("               #######            #######         ##################     ");

    gotoxy(x + 4, y + 10);
    printf("  게임시작 ");

    gotoxy(x + 6, y + 12);
    printf("  종료	\n\n");
    cout << " ============================================================================================================================\n" << endl;

    // 처음에 '>' 표시를 바로 그리기
    drawArrow(arrowX, currentY);

    while (1) {
        int n = keyControl();
        switch (n) {
        case UP:
            if (currentY > y + 10) { // 최소 위치 제한
                clearArrow(arrowX, currentY);
                currentY -= 2; // 한 줄 위로 이동
                drawArrow(arrowX, currentY);
            }
            break;
        case DOWN:
            if (currentY < y + 12) { // 최대 위치 제한
                clearArrow(arrowX, currentY);
                currentY += 2; // 한 줄 아래로 이동
                drawArrow(arrowX, currentY);
            }
            break;
        case 13: // Enter
            system("cls");
            if (currentY == y + 10) return 1; // 게임시작
            //else if (currentY == y + 12) return 2; // 이어하기
            else if (currentY == y + 14) return 3; // 종료
            break;
        }
    }
}
void print_stat(int month, horse* player, training& trainer) {
    int display_year = (month % 18 == 0 && month != 0) ? month / 18 : month / 18 + 1;
    int d_day = (month % 6 == 0) ? 0 : 6 - (month % 6);

    string brred_str;
    switch (player->get_breed()) {
    case 0: brred_str = "도주마"; break;
    case 1: brred_str = "선행마"; break;
    case 2: brred_str = "선입마"; break;
    case 3: brred_str = "추입마"; break;
    default: brred_str = "알 수 없음"; break;
    }


    cout << "=============================================================================================================================\n"
        << "\t\t\t\t\t\t\t   " << display_year << "(년차)\n\n" //<< month << " / " << MAX_MONTH << "\n\n"
        << "\t\t\t\t\t" << player->get_name() << " | " << brred_str << " | " << month << "turn | " << "레이스 D-" << d_day+1 << "\n";
    cout << "=============================================================================================================================\n\n";
    cout << "\t-------------------------------------------------------------------------------------------------------------\n";
    cout << "\t          |     |                                    /\\---/\\                                     \n";
    cout << "\t          |     |                                   ((      \\                                    \n";
    cout << "\t          |     |                                 (()( 6'  6'\\                                  \n";
    cout << "\t          |     |                             ((((((          \\\n";
    cout << "\t          |     |                       ((((((          \\      \\    __________________\n";
    cout << "\t          |     |        $%__\"_______\"(((((              /↖(@__@)  /⢇⢝⠽⣾⡿⣞⣞//⣵⣵⢵⣵⣵⢇⢇/|\n";
    cout << "\t          |     ((((((($/         ______________________/______   /⡪⣎⢎⢎⢮⡪⣎//⣿⡿⣟⣿⡾⣿⣯/⣕|\n";
    cout << "\t|_________|((())/__________       |___________________________|  /=======//=======/⣜⣜|\n";
    cout << "\t|_________(((())/__________|      |___________________________| /=======//=======/⣜⣜⣜|\n";
    cout << "\t|_______((((((_____________|      |___    ____________    ____|/⣺⢪⡯⣗⢕⢯⣞//⡯⣿⣽⣟⣯⢿⢿|⣿⡺⡺⡺|\n";//
    cout << "\t|______(((())______________|         |    |          |    |    『===============|⣿⡺⣜⡺|\n";//
    cout << "\t|_______)))))______________|       __|____|          |____|___||⡽⣝⣞⡕⡵⣳||⢵⡳⣷⣶⣗⣝⢮⢯|⣫⢯/⢯|\n";
    cout << "\t|______((()()______________|      |_______| ._.-._., |________||⢽⣺⣺⡪⡪⣗||⢽⢵⣫⢟⣽⡺⣜⢽|⡳/|⢯|\n";
    cout << "\t|______(()(________________|._._._|_______| |      | |________||⢟⣞⢮⢯⢪⢯||⢽⠹⡺⡽⡮⣻⡪ |/⢯|⢯|\n";
    cout << "\t|_____(((((________________|         |    | |      | |    |    |=================⣻⡪⣻⡪|\n";
    cout << "\t|______(__)________________|       _ |____|_|______|_|____|___ |⡺⡵⡽⣝⣞⡕||⣳⡳⣳⢽⢵⡳⣷⣶|⢯⣶⣶/\n";
    cout << "\t|_______))_________________|      |___________________________||⢗⢽⣺⣺⡪⡪||⣺⢽⢵⣫⢟⣽⡺⣜|⣶⣶/\n";
    cout << "\t|__________________________|      |___    _____________    ___||⢗⢽⣺⣺⡪⡪||⣺⢽⢵⣫⢟⣽⡺⣜|⣶/\n";
    cout << "\t|__________________________|         |    | _\\    |___\\   |   ||⡪⡪⣗⢯⣺⢽||⢟⣽⡺⣜⢽⢵⣳⣳|/\n";
    cout << "\t|__________________________|--------------------------------------------------------------------------------\n";
    cout << "=============================================================================================================================\n";
    cout << "현능력치\n";
    cout << "스피드 : " << player->get_spd() << " / 1400" << "\t파워 : " << player->get_pow() << " / 1400" << "\t지구력 : " << player->get_sta() << " / 1400" << "\t근성 : " << player->get_guts() << " / 1400" << "\n";
    cout << "부상확률 : " << trainer.injury_percent(*player) << "%\t" << "체력: " << trainer.get_hp() << "\n";
    cout << "=============================================================================================================================\n";
    cout << "1) 스피드 훈련        2) 파워 훈련        3) 지구력 훈련        4) 근성 훈련        5) 휴식\n";

    while (true) {
        int n;
        cin >> n;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "숫자만 입력해주세요: ";
            continue;
        }
        cin.ignore(1000, '\n');

        switch (n) {
        case 1: {
            if (player->get_spd() == 1400) {
                std::cout << "최대치에 도달하였습니다. 다시 선택해주세요.\n";
                continue;
            }
            trainer.training_speed(*player);   return;
        }
        case 2: {
            if (player->get_pow() == 1400) {
                std::cout << "최대치에 도달하였습니다. 다시 선택해주세요.\n"; 
                continue;
            } trainer.training_power(*player);      return;
        }
        case 3: {
            if (player->get_sta() == 1400) {
                std::cout << "최대치에 도달하였습니다. 다시 선택해주세요.\n"; 
                continue;
            } trainer.training_endurance(*player); return;
        }
        case 4: {
            if (player->get_guts() == 1400) {
                std::cout << "최대치에 도달하였습니다. 다시 선택해주세요.\n"; 
                continue;
            } trainer.training_perseverance(*player);  return;
        }
        case 5: trainer.rest(*player);                return;
        default:
            cout << "잘못된 선택입니다. 다시 입력해주세요: ";
            break;
        }
    }
}

//말생성 함수
inline horse select_horse() {
    int x = 45;

    gotoxy(x, 15);
    std::cout << "1. 도주마 사일런스 스즈카\n";
    gotoxy(x, 17);
    std::cout << "2. 선행마 마야노 탑건\n";
    gotoxy(x, 19);
    std::cout << "3. 선입마 오구리 캡\n";
    gotoxy(x, 21);
    std::cout << "4. 추입마 골드 쉽\n";
    gotoxy(x, 23);
    std::cout << "5. 커스텀 말 만들기\n";
    gotoxy(x, 25);
    std::cout << "시작마를 고르시오 (1 ~ 5): ";

    while (true) {
        int n;
        std::cin >> n;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "숫자만 입력해주세요: ";
            continue;
        }
        switch (n) {
        case 1: return horse("사일런스 스즈카", 0, startStat, startStat, startStat, startStat);
        case 2: return horse("마야노 탑건", 1, startStat, startStat, startStat, startStat);
        case 3: return horse("오구리 캡", 2, startStat, startStat, startStat, startStat);
        case 4: return horse("골드 쉽", 3, startStat, startStat, startStat, startStat);
        case 5: {
            std::string name;
            int breed;

            std::cout << "\n\t\t\t[커스텀 말 생성]\n";
            std::cout << "\t\t\t이름을 입력하세요: ";
            std::cin.ignore(); // 버퍼 비우기
            std::getline(std::cin, name);
            std::cout << "\t\t\t경주 스타일을 선택하세요 (1: 도주, 2: 선행, 3: 선입, 4: 추입): ";
            while (!(std::cin >> breed) || breed < 1 || breed > 4) {
                std::cin.clear();
                std::cin.ignore(100, '\n');
                std::cout << "\t\t\t잘못된 선택입니다. 다시 입력해주세요 (1~4): ";
            }

            return horse(name, breed - 1, startStat, startStat, startStat, startStat);
        }
        default:
            std::cout << "\t\t\t잘못된 선택입니다. 다시 입력해주세요: ";
        }
    }
}

/*
참고사이트
https://geundung.dev/14?category=699626 => 화면 메뉴란 구성
https://geundung.dev/15?category=699626 =>키보드 이벤트 처리
*/

//최종 등수 아스키아트 final_reward.h로 이동
