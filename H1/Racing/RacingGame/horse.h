#ifndef HORSE_H
#define HORSE_H
#include<iostream>
#include<string>
#include<cmath>
#include<cstdlib>

#define BASELINE 400 // 스탯 기준치 (이동로직 계산 사용)

enum HorseType { PLAYER, CPU };

class horse {
private:
    HorseType type;
    // 말 능력치
    int spd = 0;   // 속도
    int pow = 0;   // 힘
    int sta = 0;   // 스태미나
    int guts = 0;  // 근성

    // 말 기본 정보
    std::string name;   // 말 이름
    int breed;          // 품종 번호 (도주 - 0 / 선행 - 1 / 선입 - 2 / 추입 - 3)

    // 위치 관련 변수
    int prev_pos = 0;        // 이전 위치
    int curr_pos = 0;        // 현재 위치
    double decimal_point = 0.0; // 소수점 누적 (정수 이동 제외 분)
    int rank = 0; // 레이스 등수 저장

protected:
    // 스텟별 가중치
    double breed_mod[3];      // 구간 가중도 0: 초반 / 1: 중반 / 2: 후반
    double stat_mod[3];       // 능력치 가중도 0: 초반 / 1: 중반 / 2: 후반

public:
    // 임시 생성용 기본생성자
    horse() : type(CPU), name("임시생성자"), breed(0), spd(0), pow(0), sta(0), guts(0) {}

    // CPU용 생성자
    horse(std::string name, int breed, int tier)
        : type(CPU), name(name), breed(breed) {
        set_ai_stats(breed, tier);
        set_modifiers(breed);
       // std::cout << "Horse Created: " << name << ", Breed: " << breed << ", Tier: " << tier << std::endl;
       // std::cout << "Stats - Speed: " << spd << ", Stamina: " << sta << ", pow: " << pow << ", Guts: " << guts << std::endl;
    }

    // 플레이어용 생성자
    horse(std::string name, int breed, int spd, int pow, int sta, int guts)
        : type(PLAYER), name(name), breed(breed),
        spd(spd), pow(pow), sta(sta), guts(guts) {
        set_modifiers(breed);
    }

    // AI 스탯 설정
    void set_ai_stats(int breed, int tier) {
        int t = (10 - tier);
        switch (breed) {
            // 도주
        case 0:
            spd = (1100 + rand() % 101) / 9 * t;
            pow = (1150 + rand() % 101) / 9 * t;
            sta = (900 + rand() % 101) / 9 * t;
            guts = (650 + rand() % 101) / 9 * t;
            break;

            // 선행
        case 1:
            spd = (1100 + rand() % 101) / 9 * t;
            pow = (1000 + rand() % 101) / 9 * t;
            sta = (1100 + rand() % 101) / 9 * t;
            guts = (600 + rand() % 101) / 9 * t;
            break;

            // 선입
        case 2:
            spd = (1100 + rand() % 101) / 9 * t;
            pow = (850 + rand() % 101) / 9 * t;
            sta = (1050 + rand() % 101) / 9 * t;
            guts = (1100 + rand() % 101) / 9 * t;
            break;

            // 추입
        case 3:
            spd = (1100 + rand() % 101) / 9 * t;
            pow = (800 + rand() % 101) / 9 * t;
            sta = (950 + rand() % 101) / 9 * t;
            guts = (1200 + rand() % 101) / 9 * t;
            break;
        }
    }

    // 품종별 주행 특성 설정
    void set_modifiers(int breed) {
        static double s_mod[4][3] = { // 능력치 가중도
            {1.0, 0.8, 0.6}, // 도주
            {0.9, 1.0, 0.8}, // 선행
            {0.8, 0.9, 1.0}, // 선입
            {0.7, 0.8, 1.0}  // 추입
        };

        static double b_mod[4][3] = { // 구간 가중도
            {1.5, 1.2, 1.0}, // 도주
            {1.2, 1.3, 1.1}, // 선행
            {1.1, 1.2, 1.4}, // 선입
            {1.0, 1.2, 1.6}  // 추입
        };

        for (int i = 0; i < 3; ++i) {
            stat_mod[i] = s_mod[breed][i]; // 능력치 가중도
            breed_mod[i] = b_mod[breed][i]; // 구간 가중도
        }
    }

    // 스탯 조회용 getter
    std::string get_name() { return name; }     // 말 이름 리턴
    int get_spd() { return spd; }               // 스피드 리턴
    int get_pow() { return pow; }               // 파워 리턴
    int get_sta() { return sta; }               // 스태미나 리턴
    int get_guts() { return guts; }             // 근성 리턴
    int get_breed() { return breed; }           // 품종 리턴
    int get_rank() { return rank; }             // 랭크 리턴


    // 스탯 추가용 setter
    void set_spd(int n) { this->spd += n; }     // 스피드 증가
    void set_pow(int n) { this->pow += n; }     // 파워 증가
    void set_sta(int n) { this->sta += n; }     // 스태미나 증가 (오타 수정)
    void set_guts(int n) { this->guts += n; }   // 근성 증가
    void set_breed(int n) { this->breed = n; }  // 품종 리턴
    void set_rank(int n) { this->rank = n; }    // 랭크 리턴

    // 현재 위치가 어느 구간인지 반환 (0: 초반 / 1: 중반 / 2: 후반)
    int position() const {
        if (curr_pos <= 19) return 0;
        else if (curr_pos <= 39) return 1;
        else return 2;
    }

    // 말 이동 로직
    void move(int tier) {
        // 구간별 대응 스탯 선택: [0]:pow, [1]:sta, [2]:guts
        int stat_by_section[3] = { pow, sta, guts };
        double correction = 1.0; // 최종 이동 거리 보정치 (너무 빨리 끝나는 것을 방지)

        if (tier == 7) { correction = 0.8; } // 7티어 경기 보정치
        else if (tier < 7) { correction = 0.7; } // 1~6티어 경기 보정치

        int seg = position(); // 현재 위치한 구간

        // 속도 기반 이동 거리 + 능력치  + 누적 소수점
        double base_speed = (static_cast<double>(spd) / BASELINE) + 2.0 + (rand() % 11 / 10.0);

        double stat_ratio = (static_cast<double>(stat_by_section[seg]) / BASELINE) * stat_mod[seg]; // 능력치 가중도 계산

        double move_distance = ( (base_speed + stat_ratio) * breed_mod[seg] + decimal_point ) * correction; // 최종 이동 거리

        // 소수점 부분은 다음 이동에 반영되도록 저장
        decimal_point = fmod(move_distance, 1.0);

        // 정수 부분만 현재 위치에 반영
        prev_pos = curr_pos;
        curr_pos += static_cast<int>(move_distance);
    }

    // 현재 위치 반환 (외부 조회용)
    int get_position() const { return curr_pos; }

    // 이전 위치 반환 (외부 조회용)
    int get_prev_pos() const { return prev_pos; }

    // 동석차 방지
    double get_decimal_point() { return decimal_point; }
    void add_position(int n) { this->curr_pos += n; }

    // 리셋
    void reset() {
        prev_pos = 0;        // 이전 위치
        curr_pos = 0;        // 현재 위치
        decimal_point = 0.0; // 소수점 누적
        rank = 0; // 레이스 등수
    }
};


#endif // !PHORSE_H
