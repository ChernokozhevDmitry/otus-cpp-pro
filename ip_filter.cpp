#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <optional>
#include "ip_filter_util.h"


int main(int, char const *argv[]){
    try {
        std::string filename = argv[1];
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Could not open file for reading: " << filename << std::endl;
			return 0;
		}
        std::vector<ip_addr_type> ip_pool;
// не сплитуем всю строку, а сразу IP, остальное до \n пропускаем
        for(std::string line; std::getline(file, line, '\t');){
            ip_pool.push_back(split(line, '.'));
            std::getline(file, line, '\n');
        } 

        // TODO reverse lexicographically sort
        std::sort(ip_pool.begin(), ip_pool.end(),
                    [](const auto& lhs, const auto& rhs) -> bool {
                        if (lhs != rhs) {
                            return lhs > rhs;
                        }
                        return false;
                        });

        std::cout << ip_pool;

        // TODO filter by first byte and output
        std::cout << filter(ip_pool, {1, std::nullopt, std::nullopt, std::nullopt});

        // TODO filter by first and second bytes and output
        std::cout << filter(ip_pool, {46, 70, std::nullopt, std::nullopt});

        // TODO filter by any byte and output
        std::cout << filter_any(ip_pool, 46);

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}