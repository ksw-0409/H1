#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

class Horse_name {
private:
    std::vector<std::vector<std::string>> names;  // 품종별 이름들
    std::mt19937 gen;  // 난수 생성기

public:
    Horse_name() : gen(std::random_device{}()) {
        // 품종별 이름들 초기화
        names = {
            {"트윈 터보", "미호 노 부르봉", "사쿠라 바쿠신오", "메지로 파머", "스마트 팔콘", 
            "코파노 리키", "세이운 스카이", "아이네스 후진", "시크릿어리엇"},  // 도주마
            {"타이키 셔틀", "엘 콘도르 파사", "시리우스 심볼리", "토센 조던", "스페셜 위크", 
            "슈퍼 크릭", "마치카네 탄호이저", "파인 모션", "메지로 아르당"},  // 선행마
            {"토카이 테이오", "심볼리 루돌프", "다이와 스칼렛", "그래스 원더", "나이스 네이처", 
            "위닝 티켓", "에어 그루브", "나리타 브라이언", "사토노 다이아몬드"},  // 선입마
            {"히시 아마존", "나리타 타이신", "타마모 크로스", "어드마이어 베가", "이나리 원", 
            "메지로 브라이트", "오르페브르", "듀랜달", "드림 저니"}  // 추입마
        };

    }

    std::string get_name(int breed) {
        // 품종 번호 체크
        if (breed < 0 || breed >= names.size()) return "InvalidBreed";

        auto& pool = names[breed];
        if (pool.empty()) return "NoName";

        // 이름을 섞고 마지막 이름을 꺼냄
        std::shuffle(pool.begin(), pool.end(), gen);
        std::string name = pool.back();
        pool.pop_back();  // 중복 방지
        return name;
    }
};
