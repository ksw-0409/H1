#ifndef TRAINING_H
#define TRAINING_H

#include<iostream>
#include "horse.h"

#define TRAINING_MHP 10 //훈련별 마이너스 체력
#define MIRACLE 10 //체력 미라클 수치 밑에서 훈련 성공시 추가
#define MAX_STAT 1400 //맥스 스탯 

class training {
    horse& h;
    int training_hp = 0; //100 최대 
    bool isInjury = 0; //참 부상, 거짓 노부상 
    int m_sta[4] = { 0 }; //부상시 마이너스 스탯 저장용

    void training_bass(bool& isInjury, int& m, int& s) { //트레이닝 베이스 M< 메인증가스탯, S<서브 증가스탯
        if (rand() % 101 < injury_percent(h)) { //부상시 모든능력치 감소
            isInjury = true;
            m_sta[0] = static_cast<int>(h.get_spd() * 0.1);
            m_sta[1] = static_cast<int>(h.get_sta() * 0.1);
            m_sta[2] = static_cast<int>(h.get_pow() * 0.1);
            m_sta[3] = static_cast<int>(h.get_guts() * 0.1);
            h.set_spd(-m_sta[0]);
            h.set_sta(-m_sta[1]);
            h.set_pow(-m_sta[2]);
            h.set_guts(-m_sta[3]);
            std::cout << "부상! \t전 능력치 - 10%" << std::endl;
            training_hp = 100;
            return;
        }
        else {
            if (training_hp <= MIRACLE) { //체력10밑 극적성공 
                m = (rand() % 36 + 15) * 2;
                s = (rand() % 16 + 10) * 2;
                std::cout << "극한의 상황에서 훈련을 성공했습니다!\n훈련 경험치 2배!\n";
            }
            else { //평범하게 훈련하는것 
                m = rand() % 41 + 20;
                s = rand() % 11 + 10;
                std::cout << "훈련 성공!\n";
            }
            training_hp -= TRAINING_MHP;
            if (training_hp < 0)training_hp = 0;
            isInjury = false;
        }
    }
public:
    training(horse& horse) : h(horse), training_hp(100), isInjury(false) {}

    void training_speed(horse& h) { //각 스탯 훈련 
        int m, s;
        training_bass(isInjury, m, s);
        int spd = h.get_spd();
        int pow = h.get_pow();
        if (spd + m + s > MAX_STAT) {
            m -= spd + m + s - MAX_STAT;
        }
        if (!isInjury) {
            std::cout << "스피드 + " << m + s << std::endl;
            h.set_spd(m + s - 10); // 스피드 능력치의 중요도를 고려 및 난이도 조절을 위하여 -10의 보정 
        }
    }

    void training_endurance(horse& h) {
        int m, s;
        training_bass(isInjury, m, s);
        if (!isInjury) {
            int sta = h.get_sta();
            int guts = h.get_guts();
            if (sta + m > MAX_STAT) {
                m -= sta + m - MAX_STAT;
            }
            if (guts + s > MAX_STAT) {
                s -= guts + s - MAX_STAT;
            }
            std::cout << "지구력 + " << m << " | 근성 + " << s << std::endl;
            h.set_sta(m);
            h.set_guts(s);
        }
    }

    void training_power(horse& h) {
        int m, s;
        training_bass(isInjury, m, s);
        if (!isInjury) {
            int pow = h.get_pow();
            int sta = h.get_sta();
            if (pow + m > MAX_STAT) {
                m -= pow + m - MAX_STAT;
            }
            if (sta + s > MAX_STAT) {
                s -= sta + s - MAX_STAT;
            }
            std::cout << "힘 + " << m << " | 지구력 + " << s << std::endl;
            h.set_pow(m);
            h.set_sta(s);
        }
    }

    void training_perseverance(horse& h) {
        int m, s;
        training_bass(isInjury, m, s);
        if (!isInjury) {
            int guts = h.get_guts();
            int pow = h.get_pow();
            if (guts + m > MAX_STAT) {
                m -= guts + m - MAX_STAT;
            }
            if (pow + s > MAX_STAT) {
                s -= pow + s - MAX_STAT;
            }
            std::cout << "근성 + " << m << " | 힘 + " << s << std::endl;
            h.set_guts(m);
            h.set_pow(s);
        }
    }

    int injury_percent(horse& h) { //훈련 hp별 부상 확률 
        if (training_hp <= 0) { return 100; }
        else if (training_hp <= 10) { return 90; }
        else if (training_hp <= 20) { return 60; }
        else if (training_hp <= 30) { return 30; }
        else if (training_hp <= 40) { return 10; }
        else { return 0; }
    }

    void rest(horse& h) {//훈련 hp 회복
        if (isInjury) { //부상후 회복 
            double rcv_pct = (static_cast<double>(rand() % 51 + 50)) / 100.0; // 50~100%
            isInjury = false;

            int rec_spd = static_cast<int>(m_sta[0] * rcv_pct);
            int rec_sta = static_cast<int>(m_sta[1] * rcv_pct);
            int rec_pow = static_cast<int>(m_sta[2] * rcv_pct);
            int rec_guts = static_cast<int>(m_sta[3] * rcv_pct);

            // MAX_STAT 초과 방지
            if (h.get_spd() + rec_spd > MAX_STAT) rec_spd = MAX_STAT - h.get_spd();
            if (h.get_sta() + rec_sta > MAX_STAT) rec_sta = MAX_STAT - h.get_sta();
            if (h.get_pow() + rec_pow > MAX_STAT) rec_pow = MAX_STAT - h.get_pow();
            if (h.get_guts() + rec_guts > MAX_STAT) rec_guts = MAX_STAT - h.get_guts();

            h.set_spd(rec_spd);
            h.set_sta(rec_sta);
            h.set_pow(rec_pow);
            h.set_guts(rec_guts);

            std::cout << "부상후 회복: 잃은 스탯 " << rcv_pct * 100 << "% 회복\n";
            return;
        }
        int rest_hp = rand() % 41 + 30; // 최소 30~70
        training_hp += rest_hp;
        if (training_hp > 100) {
            rest_hp -= training_hp - 100;
            training_hp = 100;
        }
        std::cout << "회복된 체력: " << rest_hp << std::endl;
    }
    //플레이어 hp 표시용 게터? 추가
    int get_hp() {
        return training_hp;
    }

};

#endif // !TRAINING_H
