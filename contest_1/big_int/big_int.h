#pragma once

#include <iostream>
#include <cstdint>

class BigInteger {
public:
    BigInteger() = default;
    explicit BigInteger(const char*);
    BigInteger(long long); // NOLINT
    BigInteger(const BigInteger&);

    BigInteger& operator=(const BigInteger&) = default;
    friend BigInteger operator+(const BigInteger&);
    friend BigInteger operator-(const BigInteger&);
    friend BigInteger operator+(const BigInteger&, const BigInteger&);
    friend BigInteger operator-(const BigInteger&, const BigInteger&);

    friend std::istream& operator>>(std::istream& in, BigInteger&);
    friend std::ostream& operator<<(std::ostream& out, const BigInteger& num);

    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);

    [[nodiscard]]
    bool IsNegative() const {
        return is_negative;
    }

    [[deprecated]]
    static void Info(BigInteger&);

    void Invert() {
        is_negative = !is_negative;
    }

    template <typename T>
    class Buffer {
    public:
        explicit Buffer(size_t beg_size);
        Buffer(const Buffer&);

        ~Buffer() {
            delete[] array;
        };

        [[nodiscard]] size_t GetSize() const {
            return size;
        }
        [[nodiscard]] size_t GetContainerSize() const {
            return container_size;
        };
        void SetSize(size_t new_size) {
            size = new_size;
        }

        T* Begin() {
            return array;
        }
        T* End() {
            return array + size;
        }
        [[nodiscard]] const T* Begin() const {
            return array;
        }
        [[nodiscard]] const T* End() const {
            return array + size;
        }

        T& operator[](size_t idx) {
            return array[idx];
        };
        const T& operator[](size_t idx) const {
            return array[idx];
        };

        Buffer& operator=(const Buffer& src) {
            delete[] array;
            size = src.size;
            container_size = src.container_size;

            array = new T[container_size];

            for (int i = 0; i < size; ++i) {
                array[i] = src.array[i];
            }

            return *this;
        }

        void PushBack(T elem);

        void Resize(size_t new_size) {
            size_t upper_bound = std::min(new_size, size);
            T* new_array = new T[new_size];

            for (size_t i = 0; i < upper_bound; ++i) {
                new_array[i] = array[i];
            }

            delete[] array;
            array = new_array;
            size = new_size;
            container_size = new_size;
        }

    private:
        T* array;
        size_t size = 0;
        size_t container_size;
    };
private:
    const static int32_t BASE = 10000;
    const static size_t BASE_DEC_LEN = 4;

    bool is_negative = false;
    Buffer<int32_t> buffer = Buffer<int32_t>(2);

    void Normalize();
    void CheckSign();
public:
    static void RawSum(const BigInteger&, const BigInteger&, BigInteger&);
    static void RawSubtract(const BigInteger&, const BigInteger&, BigInteger&);
};