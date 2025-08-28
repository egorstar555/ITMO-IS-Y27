
#include "number.h"

#include <cstring>
#include <iomanip>

int2023_t from_int(int32_t i) {
    int2023_t res;
    int32_t cnt = 0;

    int32_t tmp = std::abs(i);
    do {
        res.digits[cnt++] = (tmp % kBase2023);
        tmp /= kBase2023;
    } while (tmp != 0);
    res = i < 0 ? -res : res;

    return res;
}

int2023_t from_string(const char* buff) {
    int2033_t tmp = string_to_int2033(buff);

    return pack(tmp);
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t res;
    int32_t carry = 0;
    int32_t sum;

    if (IsNegative(lhs) == IsNegative(rhs)) {
        for (int32_t i = 0; i < kNumElem; i++) {
            sum = carry + lhs.digits[i] + rhs.digits[i];
            carry = sum >= kBase2023;
            if (carry) {
                res.digits[i] = sum - kBase2023;
            } else {
                res.digits[i] = sum;
            }
        }
        res = IsNegative(lhs) ? -res : res;
    } else {
        if (IsNegative(lhs)) {
            return rhs - (-lhs);
        }
        if (IsNegative(rhs)) {
            return lhs - (-rhs);
        }
    }

    return res;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t res;
    bool is_res_negative = false;
    int32_t difference;

    if (!IsNegative(lhs) && IsNegative(rhs)) {
        return lhs + (-rhs);
    }
    if (IsNegative(lhs) && !IsNegative(rhs)) {
        return -(-lhs + rhs);
    }
    int32_t carry = 0;
    if (rhs > lhs) {
        res = rhs;
        for (int32_t i = 0; i < kNumElem; i++) {
            difference = res.digits[i] - lhs.digits[i] + carry;
            if (difference < 0) {
                res.digits[i] = difference + kBase;
                carry = -1;
            } else {
                res.digits[i] = difference;
                carry = 0;
            }
        }
        if (!IsNegative(lhs) && !IsNegative(rhs)) {
            is_res_negative = true;
        }
    } else {
        res = lhs;
        for (int32_t i = 0; i < kNumElem; i++) {
            difference = res.digits[i] - rhs.digits[i] + carry;
            if (difference < 0) {
                res.digits[i] = difference + kBase;
                carry = -1;
            } else {
                res.digits[i] = difference;
                carry = 0;
            }
        }
        if (IsNegative(lhs) && IsNegative(rhs)) {
            is_res_negative = true;
        }
    }
    bool is_zero = true;
    for (unsigned char digit: res.digits) {
        if (digit != 0) {
            is_zero = false;
            break;
        }
    }
    if (is_res_negative && !is_zero) {
        res = -res;
    }

    return res;
}

int2023_t operator-(const int2023_t& value) {
    int2023_t tmp = value;
    tmp.digits[kNumElem - 1] = tmp.digits[kNumElem - 1] ^ kSign;

    return tmp;
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t left = lhs;
    int2023_t right = rhs;
    if (IsNegative(left)) {
        left = -left;
    }
    if (IsNegative(right)) {
        right = -right;
    }
    for (int32_t i = kNumElem - 1; i >= 0; i--) {
        if (left.digits[i] != right.digits[i]) {
            return left.digits[i] > right.digits[i];
        }
    }

    return false;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int32_t carry;
    int2023_t res;
    int32_t prod;
    int2023_t left = lhs;
    int2023_t right = rhs;
    bool is_res_negative = false;

    if (IsNegative(lhs) != IsNegative(rhs)) {
        is_res_negative = true;
    }
    if (IsNegative(left)) {
        left = -left;
    }
    if (IsNegative(right)) {
        right = -right;
    }
    for (int32_t i = 0; i < kNumElem; i++) {
        carry = 0;
        for (int32_t j = 0; (j < kNumElem) || carry; j++) {
            prod = res.digits[i + j] + static_cast<int32_t>(left.digits[i]) * right.digits[j] + carry;
            carry = prod / kBase2023;
            res.digits[i + j] = static_cast<int8_t>(prod - carry * kBase2023);
        }
    }
    bool is_zero = true;
    for (unsigned char digit: res.digits) {
        if (digit != 0) {
            is_zero = false;
            break;
        }
    }
    if (is_res_negative && !is_zero) {
        res = -res;
    }

    return res;
}

int2023_t operator*(const int2023_t& lhs, const int32_t& rhs) {
    int2023_t res;
    int64_t prod;
    int32_t carry = 0;

    bool is_res_negative = false;

    if (IsNegative(lhs) != (rhs < 0)) {
        is_res_negative = true;
    }

    for (int32_t i = 0; (i < kNumElem) | carry; i++) {
        prod = static_cast<int64_t>(lhs.digits[i]) * std::abs(rhs) + carry;
        carry = static_cast<int32_t>(prod / kBase2023);
        res.digits[i] = static_cast<int8_t>(prod - carry * kBase2023);
    }
    bool is_zero = true;
    for (unsigned char digit: res.digits) {
        if (digit != 0) {
            is_zero = false;
            break;
        }
    }
    if (is_res_negative && !is_zero) {
        res = -res;
    }
    return res;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t left = lhs;
    int2023_t right = rhs;
    int2023_t res;
    int2023_t cur_number; // часть делимого
    int2023_t cur; // делитель умноженный на подбираемый множитель
    int32_t mid;
    int32_t r;
    int32_t l;
    int32_t part_res;
    bool is_res_negative = false;

    if (IsNegative(lhs) != IsNegative(rhs)) {
        is_res_negative = true;
    }
    if (IsNegative(left)) {
        left = -left;
    }
    if (IsNegative(right)) {
        right = -right;
    }

    for (int32_t i = kNumElem - 1; i >= 0; i--) {
        for (int32_t j = kNumElem - 1; j >= 1; j--) {
            cur_number.digits[j] = cur_number.digits[j - 1];
        }
        cur_number.digits[0] = left.digits[i];
        part_res = 0;
        l = 0;
        r = kBase;
        while (l <= r) {
            mid = (l + r) >> 1;
            cur = right * mid;
            if (cur_number > cur || cur_number == cur) {
                part_res = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        res.digits[i] = part_res;
        cur_number = cur_number - right * part_res;
    }

    bool is_zero = true;
    for (unsigned char digit: res.digits) {
        if (digit != 0) {
            is_zero = false;
            break;
        }
    }
    if (is_res_negative && !is_zero) {
        res = -res;
    }

    return res;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (size_t i = 0; i < kNumElem; i++) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    int2033_t tmp = unpack(value);
    stream << tmp;

    return stream;
}

bool IsNegative(const int2023_t& value) {
    return value.digits[kNumElem - 1] & kSign;
}

int2023_t pack(const int2033_t& value) {
    int2023_t res;
    int2033_t zero = int_to_int2033(0);
    int2033_t n256 = int_to_int2033(256);
    int2033_t tmp = value;

    int32_t i = 0;

    while (tmp != zero) {
        res.digits[i] = tmp % kBase2023;
        tmp = tmp / n256;
        i++;
    }
    if (value.is_negative) {
        res = -res;
    }

    return res;
}

int2033_t unpack(const int2023_t& value) {
    int2033_t res;
    int2023_t tmp = value;
    bool res_is_negative = false;

    if (tmp.digits[kNumElem - 1] & kSign) {
        res_is_negative = true;
        tmp = -tmp;
    }
    for (int32_t i = kNumElem - 1; i >= 0; i--) {
        res = res * kBase2023 + int_to_int2033(tmp.digits[i]);
    }
    res.is_negative = res_is_negative;

    return res;
}

// функции и операторы для int2033
int2033_t int_to_int2033(const int32_t i) {
    int2033_t res;
    int32_t cnt = 0;

    res.is_negative = i < 0;
    int32_t tmp = std::abs(i);
    do {
        res.digits[cnt++] = (tmp % kBase);
        tmp /= kBase;
    } while (tmp != 0);
    res.elem_count = cnt;

    return res;
}

int2033_t string_to_int2033(const char* buff) {
    int2033_t res;
    int32_t cnt = 0;
    auto len_buff = static_cast<std::size_t>(std::strlen(buff));
    char my_buff[len_buff + 1];

    if (buff[0] == '-') {
        res.is_negative = true;
        strcpy(my_buff, buff + 1);
        len_buff--;
    } else {
        strcpy(my_buff, buff);
    }
    for (int64_t i = len_buff; i > 0; i -= kCharNumber) {
        my_buff[i] = '\0';
        res.digits[cnt++] = strtol(i >= kCharNumber ? my_buff + i - kCharNumber : my_buff, nullptr, 10);
    }
    res.elem_count = cnt;
    if (res.digits[0] == 0 && res.elem_count == 1) {
        res.is_negative = false;
    }

    return res;
}

int2033_t operator+(const int2033_t& lhs, const int2033_t& rhs) {
    int2033_t res;
    int2033_t left = lhs;
    int2033_t right = rhs;
    int32_t carry = 0;

    if (lhs.is_negative == rhs.is_negative) {
        res.elem_count = std::max(lhs.elem_count, rhs.elem_count);
        for (int32_t i = 0; i < res.elem_count || carry; i++) {
            res.digits[i] = carry + lhs.digits[i] + rhs.digits[i];
            carry = res.digits[i] >= kBase;
            if (carry) res.digits[i] -= kBase;
        }
        if (res.digits[res.elem_count]) {
            res.elem_count++;
        }
        res.is_negative = lhs.is_negative;
    } else {
        if (lhs.is_negative) {
            left.is_negative = false;
            return right - left;
        }
        if (rhs.is_negative) {
            right.is_negative = false;
            return left - right;
        }
    }

    return res;
}

int2033_t operator-(const int2033_t& lhs, const int2033_t& rhs) {
    int32_t res_len;
    int32_t carry = 0;
    int2033_t left = lhs;
    int2033_t right = rhs;
    int2033_t res;

    if (!lhs.is_negative && rhs.is_negative) {
        right.is_negative = false;
        return left + right;
    }
    if (lhs.is_negative && !rhs.is_negative) {
        left.is_negative = false;
        res = left + right;
        res.is_negative = true;
        return res;
    }
    if (right > left) {
        res = right;
        res_len = rhs.elem_count;
        right = left;
        if (!lhs.is_negative && !rhs.is_negative) {
            res.is_negative = true;
        } else {
            res.is_negative = false;
        }
    } else {
        res = left;
        res_len = lhs.elem_count;
        if (lhs.is_negative && rhs.is_negative) {
            res.is_negative = true;
        } else {
            res.is_negative = false;
        }

    }
    for (int32_t i = 0; i < res_len; i++) {
        res.digits[i] -= right.digits[i] + carry;
        if (res.digits[i] < 0) {
            res.digits[i] += kBase;
            res.digits[i + 1]--;
        }
    }
    while (res_len && !res.digits[res_len]) {
        res_len--;
    }
    res.elem_count = res_len + 1;
    if (res.digits[0] == 0 && res.elem_count == 1) {
        res.is_negative = false;
    }

    return res;
}

int2033_t operator*(const int2033_t& lhs, const int32_t& rhs) {
    int2033_t res;
    int64_t prod;
    bool is_rhs_negative = rhs < 0;
    int32_t carry = 0;

    res.elem_count = lhs.elem_count;
    for (int32_t i = 0; (i < res.elem_count) | carry; i++) {
        prod = static_cast<int64_t>(lhs.digits[i]) * std::abs(rhs) + carry;
        carry = static_cast<int32_t>(prod / kBase);
        res.digits[i] = static_cast<int32_t>(prod - carry * kBase);
    }
    if (res.digits[res.elem_count]) {
        ++res.elem_count;
    }

    int32_t res_len = res.elem_count;
    while (res_len > 0 && !res.digits[res_len]) {
        res_len--;
    }
    res.elem_count = res_len + 1;

    if (lhs.is_negative != is_rhs_negative) {
        res.is_negative = true;
    }
    if (res.digits[0] == 0 && res.elem_count == 1) {
        res.is_negative = false;
    }
    return res;
}

int2033_t operator*(const int2033_t& lhs, const int2033_t& rhs) {
    int32_t carry;
    int2033_t res;
    int64_t prod;

    for (int32_t i = 0; i < lhs.elem_count; i++) {
        carry = 0;
        for (int32_t j = 0; (j < rhs.elem_count) || carry; j++) {
            prod = res.digits[i + j] + static_cast<int64_t>(lhs.digits[i]) * rhs.digits[j] + carry;
            carry = static_cast<int32_t>(prod / kBase);
            res.digits[i + j] = static_cast<int32_t>(prod - carry * kBase);
        }
    }
    int32_t res_len = lhs.elem_count + rhs.elem_count;
    while (res_len > 0 && !res.digits[res_len]) {
        res_len--;
    }
    res.elem_count = res_len + 1;
    if (lhs.is_negative != rhs.is_negative) {
        res.is_negative = true;
    }
    if (res.digits[0] == 0 && res.elem_count == 1) {
        res.is_negative = false;
    }
    return res;
}

int2033_t operator/(const int2033_t& lhs, const int2033_t& rhs) {
    int2033_t res;
    int2033_t cur_number;
    int2033_t cur;
    cur_number.elem_count = 1;
    int32_t mid;
    int32_t r;
    int32_t l;
    int32_t part_res;

    for (int32_t i = lhs.elem_count - 1; i >= 0; i--) {
        for (int32_t j = cur_number.elem_count; j >= 1; j--) {
            cur_number.digits[j] = cur_number.digits[j - 1];
        }
        if (cur_number.digits[cur_number.elem_count]) {
            cur_number.elem_count++;
        }
        cur_number.digits[0] = lhs.digits[i];
        part_res = 0;
        l = 0;
        r = kBase;
        while (l <= r) {
            mid = (l + r) >> 1;
            cur = rhs * mid;
            if (cur_number > cur || cur_number == cur) {
                part_res = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        res.digits[i] = part_res;
        cur_number = cur_number - rhs * part_res;
    }
    int32_t res_len = lhs.elem_count;
    while (res_len > 0 && !res.digits[res_len]) {
        res_len--;
    }
    res.elem_count = res_len + 1;
    if ((lhs.is_negative != rhs.is_negative)) {
        res.is_negative = true;
    }
    if (res.digits[0] == 0 && res.elem_count == 1) {
        res.is_negative = false;
    }

    return res;
}

int32_t operator%(const int2033_t& lhs, const int32_t& rhs) {
    int64_t cur;
    int64_t ost = 0;

    for (int32_t i = lhs.elem_count - 1; i >= 0; i--) {
        cur = ost * kBase + lhs.digits[i];
        ost = cur % rhs;
    }
    return static_cast<int32_t>(ost);
}

bool operator==(const int2033_t& lhs, const int2033_t& rhs) {
    if (lhs.is_negative != rhs.is_negative || lhs.elem_count != rhs.elem_count) {
        return false;
    }
    for (size_t i = 0; i < lhs.elem_count; i++) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const int2033_t& lhs, const int2033_t& rhs) {
    return !(lhs == rhs);
}

bool operator>(const int2033_t& lhs, const int2033_t& rhs) {
    if (lhs.elem_count != rhs.elem_count) {
        return lhs.elem_count > rhs.elem_count;
    }
    for (int32_t i = lhs.elem_count - 1; i >= 0; i--) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return lhs.digits[i] > rhs.digits[i];
        }
    }

    return false;
}

std::ostream& operator<<(std::ostream& stream, const int2033_t& value) {
    if (value.is_negative) {
        stream << '-';
    }
    stream << (value.elem_count > 0 ? value.digits[value.elem_count - 1] : 0);
    for (int32_t i = value.elem_count - 2; i >= 0; i--) {
        stream << std::setfill('0') << std::setw(kCharNumber) << value.digits[i];
    }

    return stream;
}


