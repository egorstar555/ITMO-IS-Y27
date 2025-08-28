#pragma once

#include <cinttypes>
#include <iostream>

const int32_t kNumElem = 253;
const int16_t kBase2023 = 256;
const uint8_t kSign = 0b10000000;

struct int2023_t {
    uint8_t digits[kNumElem] = {0};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& value);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int32_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);

bool operator>(const int2023_t& lhs, const int2023_t& rhs);

bool IsNegative(const int2023_t& value);

const int32_t kBase = 1000 * 1000 * 1000; // описание типа хранящего в 10^9 системе счисления
const int32_t kCharNumber = 9;

struct int2033_t { // log2((1000**3)**68 - 1) == 2033
    static const int8_t kMaxLen = 68;

    bool is_negative = false;
    int32_t elem_count = 0;
    int32_t digits[kMaxLen] = {0};
};

int2023_t pack(const int2033_t& value);

int2033_t unpack(const int2023_t& value);

int2033_t string_to_int2033(const char* buff);

int2033_t int_to_int2033(const int32_t i);

int2033_t operator+(const int2033_t& lhs, const int2033_t& rhs);

int2033_t operator-(const int2033_t& lhs, const int2033_t& rhs);

int2033_t operator*(const int2033_t& lhs, const int2033_t& rhs);

int2033_t operator*(const int2033_t& lhs, const int32_t& rhs);

int2033_t operator/(const int2033_t& lhs, const int2033_t& rhs);

int operator%(const int2033_t& lhs, const int32_t& rhs);

bool operator==(const int2033_t& lhs, const int2033_t& rhs);

bool operator!=(const int2033_t& lhs, const int2033_t& rhs);

bool operator>(const int2033_t& lhs, const int2033_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2033_t& value);