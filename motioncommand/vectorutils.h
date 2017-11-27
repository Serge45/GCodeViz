#ifndef VECTORUTILS_H
#define VECTORUTILS_H
#include <string>
#include <vector>
#include <cassert>

template<typename T>
std::string stdVectorToString(const std::vector<T> &arr) {
    std::stringstream ss;

    assert(arr.size() >= 0);

    if (arr.size() == 1) {

        ss << arr[0];
    } else {

        ss << "{";

        for (size_t i = 0; i < arr.size() - 1; ++i) {
            ss << arr[i]
               << ",";
        }
        ss << arr.back()
           << "}";
    }
    return ss.str();
}

#endif // VECTORUTILS_H

