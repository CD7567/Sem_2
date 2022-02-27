#pragma once

template <typename T>
bool BinarySearch(const T* begin, const T* end, const T& target) {
    bool is_found = false;
    const T* left = begin;
    const T* right = end - 1;

    while (left <= right) {
        const T* mid = left + (right - left) / 2;

        if (target < *mid) {
            right = mid - 1;
        } else if (*mid < target) {
            left = mid + 1;
        } else {
            is_found = true;
            break;
        }
    }

    return is_found;
}