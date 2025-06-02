#pragma once
//최종등수 아스키아트 불러올때 필요해서 추가
#include "final_reward.h"
#include "race_info.h"
#include "canvas.h"
#include "horse.h"
#include "horse_name.h"
#include <array>
#include <algorithm>
#include <cmath>
#include <random>
#include <iomanip>
#include <vector>
#include <Windows.h>

using namespace std;

class Race {
private:
	static const int HORSE_COUNT = 7;

	horse& player;    // 실제 플레이어 참조
	Canvas canvas;    // canvas 생성
	Horse_name name;  // Horse_name 생성

	int lane = rand() % HORSE_COUNT; // 플레이어 라인 추첨;
	array<int, 6> cpu_type = { 0, 1, 1, 2, 2, 3 }; //cpu 특성 배열

	horse horses[HORSE_COUNT];       // 빈 말 배열 생성
	bool finished[HORSE_COUNT] = {}; // 말이 결승선에 도착여부 확인
	
	int h_turn[7] = { }; // 도착 턴 저장 배열
	string lap_time[7][2] = {}; // 랩타임 저장을 위한 배열 ([분][초])
	string h_breed = ""; 

public:
	Race(horse& player, int tier) : player(player) { // 플레이어 말 입력, 현재 레이스 티어 입력
		horses[lane] = player; // 화면 표시용 복사본
		horses[lane].reset();  // 복사본 초기화

		canvas.set_player(lane); // 플레이어 마크 ★ 지정

		// CPU 타입 셔플
		mt19937 g(std::random_device{}());
		shuffle(cpu_type.begin(), cpu_type.end(), g);

		for (int i = 0, j = 0; i < HORSE_COUNT; i++) { // 말 생성
			if (i != lane) {
				int type = cpu_type[j];
				horses[i] = horse(name.get_name(type), type, tier); // cpu이름 생성로직 만들기
				j++;
			}
		}
	}

	void lap_time_set(double total, int num) {
		double time = (h_turn[num] * 1.5) + (10 - total / 10) + 60;
		int min = time / 60;
		double sec = fmod(time, 60.0);

		// 소수점 셋째 자리에서 자르기 (반올림 없이)
		double truncated_sec = floor(sec * 1000) / 1000.0;

		// 문자열로 저장 (to_string은 기본적으로 6자리 출력)
		string sec_str = to_string(truncated_sec);

		// 소수점 셋째 자리까지만 남기기
		size_t dot_pos = sec_str.find('.');
		if (dot_pos != string::npos && dot_pos + 4 < sec_str.size()) {
			sec_str = sec_str.substr(0, dot_pos + 4); // 소수점 이하 3자리까지
		}

		lap_time[num][0] = to_string(min); // 분 저장
		lap_time[num][1] = sec_str;        // 초 저장
	}


	void tie_breaker(int rank) { // 타이 브레이커 및 랩타임 계산
		vector<pair<double, int>> list; // pair인 vector list(거리, 말번호)

		for (int i = 0; i < HORSE_COUNT; i++) {
			if (horses[i].get_rank() == 0 && finished[i]) {
				double total = horses[i].get_position() + horses[i].get_decimal_point();
				lap_time_set(total, i);
				list.emplace_back(total, i);  // 거리, 말 번호
			}
		}

		sort(list.rbegin(), list.rend()); // 거리 내림차순

		for (int i = 0; i < list.size(); i++) {
			int num = list[i].second;
			int total_rank = rank + i + 1;
			horses[num].set_rank(total_rank);
		}
	}

	void show_race_summary() { // 레이스 사전 정보 출력
		using RI = RaceInfo;

		const int COL_NUM_WIDTH = 6;
		const int COL_NAME_WIDTH = 20;
		const int COL_BREED_WIDTH = 12;
		const int COL_STAT_WIDTH = 8;

		cout
			<< RI::pad("번호", COL_NUM_WIDTH)
			<< RI::pad("이름", COL_NAME_WIDTH)
			<< RI::pad("주행 특성", COL_BREED_WIDTH)
			<< RI::pad("스피드", COL_STAT_WIDTH)
			<< RI::pad("파워", COL_STAT_WIDTH)
			<< RI::pad("지구력", COL_STAT_WIDTH)
			<< RI::pad("근성", COL_STAT_WIDTH)
			<< "\n";

		for (int i = 0; i < HORSE_COUNT; ++i) {
			switch (horses[i].get_breed())
			{
			case 0:
				h_breed = "도주마";
				break;
			case 1:
				h_breed = "선행마";
				break;
			case 2:
				h_breed = "선입마";
				break;
			case 3:
				h_breed = "추입마";
				break;
			}

			cout
				<< RI::pad(to_string(i + 1) + "번마", COL_NUM_WIDTH)
				<< RI::pad(horses[i].get_name(), COL_NAME_WIDTH)
				<< RI::pad(h_breed, COL_BREED_WIDTH)
				<< RI::pad(to_string(horses[i].get_spd()), COL_STAT_WIDTH)
				<< RI::pad(to_string(horses[i].get_pow()), COL_STAT_WIDTH)
				<< RI::pad(to_string(horses[i].get_sta()), COL_STAT_WIDTH)
				<< RI::pad(to_string(horses[i].get_guts()), COL_STAT_WIDTH)
				<< "\n";
		}
	}

	void show_race_rank() { // 레이스 등수 ui 출력
		using RI = RaceInfo;

		const int COL_NUM_WIDTH = 6;
		const int COL_NAME_WIDTH = 20;
		const int COL_BREED_WIDTH = 12;
		const int COL_RANK_WIDTH = 8;
		const int COL_TIME_WIDTH = 10;

		cout
			<< RI::pad("번호", COL_NUM_WIDTH)
			<< RI::pad("이름", COL_NAME_WIDTH)
			<< RI::pad("주행 특성", COL_BREED_WIDTH)
			<< RI::pad("등수", COL_RANK_WIDTH)
			<< RI::pad("기록", COL_TIME_WIDTH)
			<< "\n";

		for (int i = 0; i < HORSE_COUNT; ++i) {
			string h_num = to_string(i + 1) + "번마";
			string h_name = horses[i].get_name();

			switch (horses[i].get_breed())
			{
			case 0:
				h_breed = "도주마";
				break;
			case 1:
				h_breed = "선행마";
				break;
			case 2:
				h_breed = "선입마";
				break;
			case 3:
				h_breed = "추입마";
				break;
			}

			string h_rank = horses[i].get_rank() > 0
				? to_string(horses[i].get_rank()) + "등"
				: "";

			string h_time = horses[i].get_rank() > 0
				? lap_time[i][0] +":"+ lap_time[i][1]
				: "";

			cout
				<< RI::pad(h_num, COL_NUM_WIDTH)
				<< RI::pad(h_name, COL_NAME_WIDTH)
				<< RI::pad(h_breed, COL_BREED_WIDTH)
				<< RI::pad(h_rank, COL_RANK_WIDTH)
				<< RI::pad(h_time, COL_TIME_WIDTH)
				<< "\n";
		}
	}

	void cpu_check() { // 사전 정보 출력
		canvas.printMap();   // 맵 출력
		show_race_summary(); // 레인별 능력치 보여주기
		cout << "\n레이스를 시작하려면 엔터를 눌러주세요.\n";
		getchar();
	}

	void restart_game() { // 4등이하면 재시작하게 예외처리
		final_reward();
		cout << "\n목표 등수에 도달하지 못했습니다...\n";
		cout << "게임을 다시 시작합니다...\n";
		getchar();
		throw std::runtime_error("RESTART");  // 예외로 재시작을 알림
	}

	void reward() { // 보상 함수
		int rank = horses[lane].get_rank();
		player.set_rank(rank);  // 등수 저장

		int bonus = 0;

		switch (rank) {
		case 1:
			cout << "1등을 달성했습니다! (모든 능력치 +50)\n";
			bonus = 50;
			break;
		case 2:
			cout << "2등을 달성했습니다! (모든 능력치 +30)\n";
			bonus = 30;
			break;
		case 3:
			cout << "3등을 달성했습니다! (모든 능력치 +10)\n";
			bonus = 10;
			break;
		default:
			restart_game();
			return;
		}

		player.set_spd(bonus);
		player.set_pow(bonus);
		player.set_sta(bonus);
		player.set_guts(bonus);

		// 능력치 상한 제한 (1400 초과 시 1400으로 고정)
		if (player.get_spd() > 1400) player.set_spd(1400 - player.get_spd());
		if (player.get_pow() > 1400) player.set_pow(1400 - player.get_pow());
		if (player.get_sta() > 1400) player.set_sta(1400 - player.get_sta());
		if (player.get_guts() > 1400) player.set_guts(1400 - player.get_guts());

		cout << "시상식이 종료되었습니다.\n";
		cout << "훈련장으로 복귀합니다.\n";
		getchar();
		system("cls");
	}

	void start(int tier) { // tier을 인자로 받음
		if (tier >= 7) { PlaySound(TEXT("BGM3-1.wav"), NULL, SND_ASYNC | SND_LOOP); }
		else if (tier >= 4) { PlaySound(TEXT("BGM3-2.wav"), NULL, SND_ASYNC | SND_LOOP); }
		else if (tier >= 2) { PlaySound(TEXT("BGM3-3.wav"), NULL, SND_ASYNC | SND_LOOP); }
		else if (tier == 1) { PlaySound(TEXT("BGM3-4.wav"), NULL, SND_ASYNC | SND_LOOP); }
		cpu_check();

		int finished_count = 0;
		int finished_turn = 0;
		
		while (finished_count < HORSE_COUNT) {
			finished_turn++;
			int rank = finished_count;

			system("cls");

			for (int i = 0; i < HORSE_COUNT; i++) {
				if (finished[i]) { continue; }

				horses[i].move(tier); // 말 이동

				int prev_pos = horses[i].get_prev_pos(); // 이전 위치
				int curr_pos = horses[i].get_position(); // 이동한 위치

				if (curr_pos >= 60) {
					canvas.set_tile(i, 60, prev_pos); // 결승선에 도달한 말 위치 고정
					finished[i] = true;
					h_turn[i] = finished_turn; // 도착 턴수 저장
					++finished_count;
				}

				else { canvas.set_tile(i, curr_pos, prev_pos); } // 일반 이동

				//cout << i + 1 << "이전 위치: " << horses[i].get_prev_pos() << " | 이동한 위치 " << horses[i].get_position() << " | 소수점 " << horses[i].get_decimal_point() << std::endl;

			}

			if (finished_count > rank) { tie_breaker(rank); }; //결승선을 넘은 말이 있을때만 실행.

			canvas.printMap();
			show_race_rank();
			//getchar(); // sleep or getchar로 진행
			Sleep(1000);
		}
	}

	void final_reward() {//최종 등수 아스키아트 불러오기
		int rank = horses[lane].get_rank();
		player.set_rank(rank);
		std::cout << "-----------------------레이스 종료-----------------------\n";
		switch (rank) {
		case 1:
			cout << "최종 우승을 달성했습니다!\n시상식으로 이동하려면 엔터를 눌러주세요.\n";
			getchar();
			show_win();
			break;
		case 2:
			cout << "2등을 달성했습니다!\n시상식으로 이동하려면 엔터를 눌러주세요.\n";
			getchar();
			show_se();
			break;
		case 3:
			cout << "3등을 달성했습니다!\n시상식으로 이동하려면 엔터를 눌러주세요.\n";
			getchar();
			show_th();
			break;
		default:
			show_go();
			getchar();
			break;
		}
		throw std::runtime_error("RESTART");
	}
};