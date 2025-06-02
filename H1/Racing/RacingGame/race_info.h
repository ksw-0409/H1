#pragma once
#include <string>
#include <sstream>
#include <iomanip>

class RaceInfo {
public:
    // 문자열의 출력 너비 계산 (UTF-8 한글은 2칸, 나머지는 1칸)
    static int width(const std::string& s) {
        int w = 0;
        for (size_t i = 0; i < s.size(); ) {
            unsigned char c = s[i];
            if (c < 0x80) {
                ++w;
                ++i;
            }
            else if ((c & 0xE0) == 0xC0) {
                w += 2;
                i += 2;
            }
            else if ((c & 0xF0) == 0xE0) {
                w += 2;
                i += 3;
            }
            else if ((c & 0xF8) == 0xF0) {
                w += 2;
                i += 4;
            }
            else {
                ++w;
                ++i;
            }
        }
        return w;
    }

    // 좌측 정렬 + 패딩
    static std::string pad(const std::string& s, int target_width) {
        int pad_size = target_width - width(s);
        std::ostringstream oss;
        oss << s;
        for (int i = 0; i < pad_size; ++i)
            oss << ' ';
        return oss.str();
    }
};
