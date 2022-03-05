#include "big_int.h"

#include <cstring>
#include <cctype>
#include <iostream>

// ===========================================================
// Realization of BigInteger class
// ===========================================================

BigInteger::BigInteger(const char* src) {
    bool is_src_negative = (src[0] == '-');
    bool has_sign = !std::isdigit(src[0]);
    size_t dec_length = strlen(src);
    char* unit = new char[BASE_DEC_LEN];
    is_negative = is_src_negative;

    size_t i;
    size_t k;
    for (i = dec_length - 1; i >= BASE_DEC_LEN; i -= BASE_DEC_LEN) {
        for (size_t j = BASE_DEC_LEN; j != 0; --j) {
            unit[j - 1] = src[i + j - BASE_DEC_LEN];
        }

        buffer.PushBack(std::stoi(unit));
    }

    for (i += 1, k = BASE_DEC_LEN - 1 ; i > static_cast<size_t>(has_sign); --i, --k) {
        unit[k] = src[i - 1];
    }

    if (k < BASE_DEC_LEN) {
        for (++k;  k != 0; --k) {
            unit[k - 1] = '0';
        }
    }

    buffer.PushBack(std::stoi(unit));
    delete[] unit;
}

BigInteger::BigInteger(long long src) {
    if (src < 0) {
        is_negative = true;
        src = -src;
    }

    if (src == 0) {
        buffer.PushBack(0);
    } else {
        while (src > 0) {
            buffer.PushBack(static_cast<int32_t>(src % BASE));
            src /= BASE;
        }
    }
}

BigInteger::BigInteger(const BigInteger& src) {
    is_negative = src.is_negative;
    buffer = Buffer(src.buffer);
}

BigInteger operator+(const BigInteger& rhs) {
    return rhs;
}
BigInteger operator-(const BigInteger& rhs) {
    BigInteger copy = rhs;
    copy.Invert();

    return copy;
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger res;

    if (lhs.buffer.GetSize() < rhs.buffer.GetSize()) {
        res = rhs + lhs;
    } else {
        if (lhs.is_negative) {
            if (rhs.is_negative) {
                BigInteger::RawSum(lhs, rhs, res);
                res.is_negative = true;
            } else {
                BigInteger::RawSubtract(lhs, rhs, res);
                res.is_negative = !res.is_negative;
            }
        } else {
            if (rhs.is_negative) {
                BigInteger::RawSubtract(lhs, rhs, res);
            } else {
                BigInteger::RawSum(lhs, rhs, res);
            }
        }
    }

    res.CheckSign();
    return res;
}
BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs) {
    BigInteger res;

    if (lhs.buffer.GetSize() < rhs.buffer.GetSize()) {
        res = rhs - lhs;
        res.is_negative = !res.is_negative;
    } else {
        if (lhs.is_negative) {
            if (rhs.is_negative) {
                BigInteger::RawSubtract(lhs, rhs, res);
                res.is_negative = !res.is_negative;
            } else {
                BigInteger::RawSum(lhs, rhs, res);
                res.is_negative = true;
            }
        } else {
            if (rhs.is_negative) {
                BigInteger::RawSum(lhs, rhs, res);
            } else {
                BigInteger::RawSubtract(lhs, rhs, res);
            }
        }
    }

    res.CheckSign();
    return res;
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs) {
    bool equals = true;

    if ((lhs.buffer.GetSize() == rhs.buffer.GetSize()) && (lhs.is_negative == rhs.is_negative)) {
        size_t upper_bound = lhs.buffer.GetSize();

        for (size_t i = 0; (i < upper_bound) && equals; ++i) {
            equals = (lhs.buffer[i] == rhs.buffer[i]);
        }
    } else {
        equals = false;
    }

    return equals;
}

std::istream& operator>>(std::istream& in, BigInteger& num) {
    char src[30005];
    in >> src;

    num = BigInteger(src);

    return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
    const int32_t* first_unit_ptr = num.buffer.Begin();

    out << (num.is_negative ? "-" : "") << *(num.buffer.End() - 1);
    for (const int32_t* i = num.buffer.End() - 2; i >= first_unit_ptr; --i) {
        int32_t unit = *i;
        out << (unit / 1000);
        unit %= 1000;
        out << (unit / 100);
        unit %= 100;
        out << (unit / 10);
        unit %= 10;
        out << unit;
    }

    return out;
}
void BigInteger::Info(BigInteger& num) {
    std::cout << "--------" << std::endl;
    std::cout << "Stored: " << num.buffer.GetSize() << std::endl;
    std::cout << "Container: " << num.buffer.GetContainerSize() << std::endl;
    std::cout << "Number: " << (num.is_negative ? "-" : "") << "|";

    for (size_t i = num.buffer.GetSize(); i > 0; --i) {
        std::cout << num.buffer[i - 1] << "|";
    }

    std::cout << std::endl << "--------" << std::endl;
}

void BigInteger::RawSum(const BigInteger& lhs, const BigInteger& rhs, BigInteger& res) {
    size_t sum_bound = std::min(lhs.buffer.GetSize(), rhs.buffer.GetSize());

    res.buffer.Resize(std::max(lhs.buffer.GetSize(), rhs.buffer.GetSize()));

    for (size_t i = 0; i < sum_bound; ++i) {
        res.buffer[i] = lhs.buffer[i] + rhs.buffer[i];
    }

    for (size_t i = sum_bound; i < lhs.buffer.GetSize(); ++i) {
        res.buffer[i] = lhs.buffer[i];
    }

    for (size_t i = sum_bound; i < rhs.buffer.GetSize(); ++i) {
        res.buffer[i] = rhs.buffer[i];
    }

    res.Normalize();
}

void BigInteger::RawSubtract(const BigInteger& lhs, const BigInteger& rhs, BigInteger& res) {
    // Assuming rhs is not longer than lhs
    size_t subtr_bound = rhs.buffer.GetSize();
    size_t upper_bound = lhs.buffer.GetSize();

    res.buffer.Resize(upper_bound);

    for (size_t i = 0; i < subtr_bound; ++i) {
        res.buffer[i] = lhs.buffer[i] - rhs.buffer[i];
    }

    for (size_t i = subtr_bound; i < upper_bound; ++i) {
        res.buffer[i] = lhs.buffer[i];
    }

    for (size_t i = 0; i < upper_bound - 1; ++i) {
        if (res.buffer[i] < 0) {
            res.buffer[i + 1]--;
            res.buffer[i] += BASE;
        }
    }

    if (res.buffer[upper_bound - 1] < 0) {
        for (size_t i = 0; i < upper_bound - 1; ++i) {
            if (res.buffer[i] != 0) {
                res.buffer[i] = BASE - res.buffer[i];
                res.buffer[i + 1]++;
            }
        }
        res.buffer[upper_bound - 1] = -res.buffer[upper_bound - 1];
        res.is_negative = true;
    }

    size_t end_size;
    for (end_size = upper_bound; (end_size > 1) && (res.buffer[end_size - 1] == 0); --end_size) {
    }

    res.buffer.SetSize(end_size);
}

void BigInteger::Normalize() {
    size_t size = buffer.GetSize();

    for (size_t i = 0; i < size - 1; ++i) {
        buffer[i + 1] += buffer[i] / BASE;
        buffer[i] %= BASE;
    }

    if (buffer[size - 1] > BASE) {
        buffer.PushBack(buffer[size - 1] / BASE);
        buffer[size - 1] %= BASE;
    }
}

void BigInteger::CheckSign() {
    if ((buffer.GetSize() == 1) && (buffer[0] == 0)) {
        is_negative = false;
    }
}

// ===========================================================
// Realization of Buffer class
// ===========================================================

#define Buff BigInteger::Buffer

template <typename T>
Buff<T>::Buffer(size_t beg_size) {
    array = new T[beg_size]{};
    container_size = beg_size;
}

template <typename T>
Buff<T>::Buffer(const Buffer<T>& src) {
    size = src.size;
    container_size = src.container_size;
    array = new T[container_size]{};

    for (int i = 0; i < size; ++i) {
        array[i] = src.array[i];
    }
}

template <typename T>
void Buff<T>::PushBack(T elem) {
    if (size == container_size) {
        container_size *= 2;
        T* new_array = new T[container_size];

        for (size_t i = 0; i < size; ++i) {
            new_array[i] = array[i];
        }

        delete[] array;
        array = new_array;
    }

    array[size++] = elem;
}