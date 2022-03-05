#pragma once

#include <algorithm>
#include <cstdint>

class LightNumBuffer {
public:
    explicit LightNumBuffer(size_t beg_size) {
        array = new int32_t[beg_size]{};
        container_size = beg_size;
    }
    LightNumBuffer(const LightNumBuffer& src) {
        size = src.size;
        container_size = src.container_size;
        array = new int32_t[container_size]{};

        for (int i = 0; i < size; ++i) {
            array[i] = src.array[i];
        }
    }

    LightNumBuffer(const char* cstr) {

    }

    ~LightNumBuffer() {
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

    int32_t* Begin() {
        return array;
    }
    int32_t * End() {
        return array + size;
    }
    [[nodiscard]] const int32_t* Begin() const {
        return array;
    }
    [[nodiscard]] const int32_t* End() const {
        return array + size;
    }

    int32_t& operator[](size_t idx) {
        return array[idx];
    };
    const int32_t& operator[](size_t idx) const {
        return array[idx];
    };

    LightNumBuffer& operator=(const LightNumBuffer& src) {
        delete[] array;
        size = src.size;
        container_size = src.container_size;

        array = new int32_t[container_size];

        for (int i = 0; i < size; ++i) {
            array[i] = src.array[i];
        }

        return *this;
    }

    void PushBack(int32_t elem);

    void Resize(size_t new_size) {
        size_t upper_bound = std::min(new_size, size);
        int32_t * new_array = new int32_t[new_size];

        for (size_t i = 0; i < upper_bound; ++i) {
            new_array[i] = array[i];
        }

        delete[] array;
        array = new_array;
        size = new_size;
        container_size = new_size;
    }

private:
    int32_t* array;
    bool is_negative = false;
    size_t size = 0;
    size_t container_size;
};

int main() {

}