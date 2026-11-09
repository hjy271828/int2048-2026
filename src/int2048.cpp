#include "int2048.h"
#include <cstdio>
#include <ratio>
#include <string>

namespace sjtu {

int2048::int2048() {}

int2048::int2048(long long input_int) {
    if (input_int < 0) {
        is_negative = 1;
    } else {
        is_negative = 0;
    }
    int cur_idx = 0;
    while (input_int) {
        int_data[cur_idx] = input_int % 10;
        input_int /= 10;
        cur_idx++;
    }
}

int2048::int2048(const std::string& input_int) {
    int length = input_int.length();
    if (!length) {
        is_negative = 0;
        return;
    }
    if (input_int[0] == '-') {
        is_negative = 1;
    } else {
        is_negative = 0;
    }
    int_data.reserve(length - is_negative);
    for (int cur_idx = 0; cur_idx < length - is_negative; ++cur_idx) {
        int_data[cur_idx] = int(input_int[length - 1 - cur_idx] - '0');
    }
    int_reduction();
}

int2048::int2048(const int2048& another) {
    is_negative = another.is_negative;
    // int length = another.int_data.size();
    // int_data.reserve(length);
    // for (int cur_idx = 0; cur_idx < length; ++cur_idx) {
    //     int_data[cur_idx] = (another.int_data)[cur_idx];
    // }
    int_data = another.int_data;
    int_reduction();
}

void int2048::read(const std::string& input_int) {
    int_data.clear();
    int length = input_int.length();
    if (!length) {
        is_negative = 0;
        return;
    }
    if (input_int[0] == '-') {
        is_negative = 1;
    } else {
        is_negative = 0;
    }
    int_data.reserve(length - is_negative);
    for (int cur_idx = 0; cur_idx < length - is_negative; ++cur_idx) {
        char cur_char = input_int[length - 1 - cur_idx];
        int_data[cur_idx] = int(cur_char - '0');
    }
    int_reduction();
}

void int2048::print() {
    int_reduction();
    int length = int_data.size();
    if (!length) {
        std::cout << '0';
        return;
    }
    std::string output = "";
    if (is_negative) {
        output += "-";
    }
    for (int cur_idx = length - 1; cur_idx >= 0; --cur_idx) {
        int cur_int = int_data[cur_idx];
        output += char('0' + cur_int);
    }
    std::cout << output;
}

int2048 int2048::opposite_num(const int2048& another) {
    int length = another.int_data.size();
    int2048 result;
    if (!length) {
        result.is_negative = 0;
        return std::move(result);
    }
    result.is_negative = 1 - another.is_negative;
    result.int_data = another.int_data;
    return std::move(result);
}

int2048& int2048::add(const int2048& another) {
    int int1_len = int_data.size();
    int int2_len = another.int_data.size();
    int max_possible_len = std::max(int1_len, int2_len) + 1;
    if (is_negative == another.is_negative) {
        int_data.resize(max_possible_len, 0);
        bool carry = 0;
        for (int cur_idx = 0; cur_idx < max_possible_len; ++cur_idx) {
            if (cur_idx < int2_len) {
                int add_calculated =
                    int_data[cur_idx] + another.int_data[cur_idx] + carry;
                int_data[cur_idx] = add_calculated % 10;
                carry = add_calculated / 10;
            } else {
                int add_calculated = carry + int_data[cur_idx];
                carry = add_calculated / 10;
                int_data[cur_idx] = add_calculated % 10;
                if (!carry) {
                    break;
                }
            }
        }
        int_reduction();
        return *this;
    } else {
        minus(opposite_num(another));
        return *this;
    }
}

}  // namespace sjtu
