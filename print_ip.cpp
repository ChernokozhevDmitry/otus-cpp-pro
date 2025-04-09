#include <iostream>
#include <type_traits>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <tuple>

// main template SFINAE
template<typename T>
// for integer type (work since cpp11++)
typename std::enable_if<std::is_integral<T>::value>::type print_ip_impl(T value) {
    constexpr std::size_t byteCount = sizeof(T);
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);

    for (std::size_t i = 0; i < byteCount; ++i) {
        std::cout << static_cast<unsigned int>(bytes[byteCount - 1 - i]);
        if (i < byteCount - 1) {
            std::cout << ".";
        }
    }
    std::cout << std::endl;
}

// for string type (work since cpp11++)
void print_ip_impl(const std::string& value) {
    std::cout << value << std::endl;
}

// for std::vector (work since cpp11++)
template<typename T>
void print_ip_impl(const std::vector<T>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ".";
        }
    }
    std::cout << std::endl;
}

// for std::list (work since cpp11++)
template<typename T>
void print_ip_impl(const std::list<T>& lst) {
    for (auto it = lst.begin(); it != lst.end(); ) {
        std::cout << *it;
        ++it;
        if (it != lst.end()) {
            std::cout << ".";
        }
    }
    std::cout << std::endl;
}

// for tuple (work only since cpp17++)
template <typename Tuple, std::size_t... I>
void print_tuple_impl(const Tuple& tup, std::index_sequence<I...>) {
    ((std::cout << (I == 0 ? "" : ".") << std::get<I>(tup)), ...);
}
template<typename... Args>
void print_ip_impl(const std::tuple<Args...>& tup) {
    print_tuple_impl(tup, std::index_sequence_for<Args...>{});
}

// main function print_ip (call print_ip_impl for different implementations)
template<typename T>
void print_ip(T value) {
    print_ip_impl(value);
}

int main() {
    print_ip(int8_t{-1});                          // 255
    print_ip(int16_t{0});                          // 0.0
    print_ip(int32_t{2130706433});                 // 127.0.0.1
    print_ip(int64_t{8875824491850138409});        // 123.45.67.89.101.112.131.41

    print_ip(std::string{"Hello, World!"});       // Hello, World!
    
    print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    print_ip(std::list<int>{400, 300, 200, 100});   // 400.300.200.100
    
// (work only since cpp17++)
    print_ip(std::make_tuple(123, 456, 789, 0));   // 123.456.789.0

    return 0;
}