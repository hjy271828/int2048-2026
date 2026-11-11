#include "int2048.h"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <ratio>
#include <string>

namespace sjtu {

int2048::int2048() {}

int2048::int2048(long long input_int) {
    int_data.clear();
    size_t input_abs;
    if (input_int < 0) {
        is_negative = 1;
        input_abs = -(size_t)(input_int);
    } else {
        is_negative = 0;
        input_abs = input_int;
    }
    while (input_abs) {
        int_data.push_back(input_abs % 10);
        input_abs /= 10;
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
    int_data.resize(length - is_negative, 0);
    for (int cur_idx = 0; cur_idx < length - is_negative; ++cur_idx) {
        int_data[cur_idx] = int(input_int[length - 1 - cur_idx] - '0');
    }
    int_reduction();
}

void int2048::int_reduction() {
    int length = int_data.size();
    if (!length) {
        is_negative = 0;
        return;
    }
    for (int cur_idx = length - 1; cur_idx >= 0; --cur_idx) {
        if (int_data[cur_idx]) {
            int_data.resize(cur_idx + 1);
            return;
        }
    }
    is_negative = 0;
    int_data.clear();
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
    int_data.resize(length - is_negative, 0);
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

bool int2048::compare_abs(const int2048& int1, const int2048& int2) {
    int int1_len = int1.int_data.size();
    int int2_len = int2.int_data.size();
    if (int1_len != int2_len) {
        return (int1_len > int2_len);
    }
    for (int cur_idx = int1_len - 1; cur_idx >= 0; --cur_idx) {
        if (int1.int_data[cur_idx] != int2.int_data[cur_idx]) {
            return (int1.int_data[cur_idx] > int2.int_data[cur_idx]);
        }
    }
    return 1;
}

int2048& int2048::minus(const int2048& another) {
    int int1_len = int_data.size();
    int int2_len = another.int_data.size();
    if (is_negative != another.is_negative) {
        add(opposite_num(another));
        return *this;
    } else {
        if (compare_abs(*this, another)) {
            bool borrow = 0;
            for (int cur_idx = 0; cur_idx < int1_len; ++cur_idx) {
                if (cur_idx < int2_len) {
                    int minus_calculated =
                        int_data[cur_idx] - another.int_data[cur_idx] - borrow;
                    if (minus_calculated < 0) {
                        int_data[cur_idx] = 10 + minus_calculated;
                        borrow = 1;
                    } else {
                        int_data[cur_idx] = minus_calculated;
                        borrow = 0;
                    }
                } else {
                    int minus_calculated = int_data[cur_idx] - borrow;
                    if (minus_calculated < 0) {
                        int_data[cur_idx] = 10 + minus_calculated;
                        borrow = 1;
                    } else {
                        int_data[cur_idx] = minus_calculated;
                        break;
                    }
                }
            }
            int_reduction();
            return *this;
        } else {
            is_negative = 1 - is_negative;
            int_data.resize(int2_len, 0);
            bool borrow = 0;
            for (int cur_idx = 0; cur_idx < int2_len; ++cur_idx) {
                if (cur_idx < int1_len) {
                    int minus_calculated =
                        another.int_data[cur_idx] - int_data[cur_idx] - borrow;
                    if (minus_calculated < 0) {
                        borrow = 1;
                        int_data[cur_idx] = minus_calculated + 10;
                    } else {
                        borrow = 0;
                        int_data[cur_idx] = minus_calculated;
                    }
                } else {
                    int minus_calculated = another.int_data[cur_idx] - borrow;
                    if (minus_calculated < 0) {
                        int_data[cur_idx] = 10 + minus_calculated;
                        borrow = 1;
                    } else {
                        int_data[cur_idx] = minus_calculated;
                        borrow = 0;
                    }
                }
            }
        }
        int_reduction();
        return *this;
    }
}

int2048 add(int2048 int1, const int2048& int2) {
    int1.add(int2);
    return std::move(int1);
}

int2048 minus(int2048 int1, const int2048& int2) {
    int1.minus(int2);
    return std::move(int1);
}

int2048 int2048::operator+() const { return *this; }

int2048 int2048::operator-() const {
    int2048 result;
    result.is_negative = 1 - is_negative;
    result.int_data = std::move(int_data);
    result.int_reduction();
    return result;
}

int2048& int2048::operator+=(const int2048& another) {
    add(another);
    return *this;
}

int2048 operator+(int2048 int1, const int2048& int2) {
    return std::move(add(int1, int2));
}

int2048& int2048::operator-=(const int2048& another) {
    minus(another);
    return *this;
}

int2048 operator-(int2048 int1, const int2048& int2) {
    return std::move(minus(int1, int2));
}

int2048& int2048::operator*=(const int2048& another) {
    int int1_len = int_data.size();
    int int2_len = another.int_data.size();
    is_negative = (is_negative ^ (another.is_negative));
    long max_possible_len = int2_len + int1_len;
    int2048 result;
    result.is_negative = (is_negative ^ (another.is_negative));
    result.int_data.resize(max_possible_len, 0);
    for (int ano_cur_idx = 0; ano_cur_idx < int2_len; ++ano_cur_idx) {
        int carry = 0;
        for (int cur_idx = 0; cur_idx < int1_len; ++cur_idx) {
            long cur_result_idx = cur_idx + ano_cur_idx;
            int calculated = int_data[cur_idx] * another.int_data[ano_cur_idx] +
                             carry + result.int_data[cur_result_idx];
            carry = calculated / 10;
            result.int_data[cur_result_idx] = calculated % 10;
        }
    }
    result.int_reduction();
    is_negative = result.is_negative;
    int_data = std::move(result.int_data);
    return *this;
}

int2048 operator*(int2048 int1, const int2048& int2) {
    int1 *= int2;
    return std::move(int1);
}

int find_quotient(std::vector<int>& dividend, const std::vector<int>& divisor,
                  int start) {
    int dividend_size = dividend.size();
    int l1 = dividend_size - start;
    int l2 = divisor.size();
    if (l1 < l2) {
        return 0;
    }
    // l1==l2 或 l1==l2+1

    std::vector<int> tmp;
    tmp.resize(l2, 0);
    int search_l, search_r;
    int ans = 0;

    if (l1 == l2) {
        search_l = dividend[dividend_size - 1] / (divisor[l2 - 1] + 1);
        search_r = (dividend[dividend_size] + 1) / divisor[l2 - 1];
        while (search_l < search_r) {
            int mid = search_l + (search_r - search_l) / 2;
            bool valid = 1;
            bool curr_is_bigger = 0;
            int carry = 0;
            for (int cur_idx = 0; cur_idx < l1; ++cur_idx) {
                int calculated = carry + divisor[cur_idx] * mid;
                carry = calculated / 10;
                tmp[cur_idx] = calculated % 10;
                if (calculated % 10 > dividend[cur_idx + start]) {
                    curr_is_bigger = 1;
                } else {
                    curr_is_bigger = 0;
                }
                if (cur_idx == l1 - 1) {
                    if (curr_is_bigger || carry) {
                        valid = 0;
                        break;
                    }
                }
            }
            if (valid) {
                ans = mid;
                search_l = mid + 1;
            } else {
                search_r = mid - 1;
            }
        }
    } else {
        assert(start > 0);
        search_l =
            (10 * dividend[dividend_size - 1] + dividend[dividend_size - 2]) /
            (divisor[l2 - 1] + 1);
        search_r = (10 * dividend[dividend_size - 1] +
                    dividend[dividend_size - 2] + 1) /
                   divisor[l2 - 1];
        int ans = 0;
        while (search_l < search_r) {
            int mid = search_l + (search_r - search_l) / 2;
            bool valid = 1;
            bool curr_is_bigger = 0;
            int carry = 0;
            for (int cur_idx = 0; cur_idx < l1; ++cur_idx) {
                if (cur_idx == l1 - 1) {
                    if (curr_is_bigger && carry == dividend[cur_idx + start] ||
                        carry > dividend[cur_idx + start]) {
                        valid = 0;
                        break;
                    }
                }
                int calculated = carry + divisor[cur_idx] * mid;
                carry = calculated / 10;
                tmp[cur_idx] = calculated % 10;
                if (calculated % 10 > dividend[cur_idx + start]) {
                    curr_is_bigger = 1;
                } else {
                    curr_is_bigger = 0;
                }
                if (cur_idx == l1 - 1) {
                    if (curr_is_bigger || carry) {
                        valid = 0;
                        break;
                    }
                }
            }
            if (valid) {
                ans = mid;
                search_l = mid + 1;
            } else {
                search_r = mid - 1;
            }
        }
    }

    if (ans) {
        int borrow = 0;
        for (int cur_idx = 0; cur_idx < l1; ++cur_idx) {
            int calculated;
            if (cur_idx < l2) {
                calculated = dividend[start + cur_idx] - tmp[cur_idx] - borrow;
            } else {
                calculated = dividend[start + cur_idx] - borrow;
            }
            if (calculated >= 0) {
                borrow = 0;
                dividend[start + cur_idx] = calculated;
            } else {
                borrow = 1;
                dividend[start + cur_idx] = calculated + 10;
            }
        }
    }

    return ans;
}

int2048& int2048::operator/=(const int2048& another) {
    int int1_len = int_data.size();
    int int2_len = another.int_data.size();
    is_negative = (is_negative ^ (another.is_negative));
    if (int2_len < 1) {
        std::cerr << "can't divided by 0\n";
        return *this;
    }
    if (int1_len < 1) {
        return *this;
    }
    if (!compare_abs(*this, another)) {
        if (is_negative) {
            int_data.clear();
            return *this;
        } else {
            int_data.clear();
            int_data.push_back(1);
            return *this;
        }
    }
    int start = int1_len - int2_len;
    std::vector<int> int_result;
    int_result.resize(start + 1, 0);
    while (start >= 0) {
        int finded_quotient = find_quotient(int_data, another.int_data, start);
        if (finded_quotient) {
            int_result[start] = finded_quotient;
            int_reduction();
            start = int_data.size() - int2_len;
        } else {
            if (start == 0) {
                break;
            }
            start--;
        }
    }


    if (int_data.size() && is_negative) {
        int_data = std::move(int_result);
        int_reduction();
        (*this) -= int2048(1);
    } else {
        int_data = std::move(int_result);
        int_reduction();
    }

    return *this;
}

int2048 operator/(int2048 int1, const int2048& int2) {
    int1 /= int2;
    return std::move(int1);
}


}  // namespace sjtu
