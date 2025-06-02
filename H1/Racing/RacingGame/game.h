#pragma once
#include "race.h"
#include "training.h"
#include "UI.h"
#include <windows.h>

inline void play_game(horse& player) {
    training trainer(player);

    const string names[] = {
    "아카데미 지역리그", "아카데미 플레이오프", "아카데미 파이널",
    "컨퍼런스 그룹 스테이지", "컨퍼런스 녹아웃 스테이지", "컨퍼런스 챔피언십",
    "챔피언스 디비전", "챔피언스 세미파이널", "챔피언스 결승전"
    };

    Sleep(500);
    system("cls");
    //첫 레이스 전 훈련BGM 실행
    PlaySound(TEXT("BGM2.wav"), NULL, SND_ASYNC | SND_LOOP);

    for (int i = 0; i < MAX_MONTH; i++) {
        // 티어 계산
        if (i != 0 && i % 6 == 0) {
            //매경기마다 티어 1한단계씩 올라감
            int tier = 10 - (i / 6);
            // 최소 티어는 1로 보장
            if (tier < 1) tier = 1;
             
            int race_index = 9 - tier;

            Race race(player, tier);
            if (race_index >= 0 && race_index < 9) {
                cout << "============================================\n";
                cout << "  " << names[race_index] << "\n";
                cout << "============================================\n\n";
            }

            race.start(tier);
            std::cout << "-----------------------레이스 종료-----------------------\n";
            race.reward();
            //레이스끝나고 다시 훈련BGM 실행
            PlaySound(TEXT("BGM2.wav"), NULL, SND_ASYNC | SND_LOOP);
        }

        print_stat(i + 1, &player, trainer);
        Sleep(1000);
        system("cls");
    }


    Race race(player, 1);
    cout << "============================================\n";
    cout << "  " << names[8] << "\n";
    cout << "============================================\n\n";
    race.start(1);
    race.final_reward();

}