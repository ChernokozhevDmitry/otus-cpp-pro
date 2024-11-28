#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm> 

// тип  октетов для удобства 
using ip_addr_type = std::vector<int>;

// не тронута с учебного примера, только октеты храним string на int 
ip_addr_type split(const std::string &str, char d){
    ip_addr_type r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos){
        r.push_back(std::stoi(str.substr(start, stop - start)));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(std::stoi(str.substr(start)));
    return r;
}

 
// TODO попробовать как-то так разобраться template <typename ... Args>, чтобы не тянуть массив, хотя через args будет только строгий порядок 
std::vector<ip_addr_type> filter(const std::vector<ip_addr_type>& vec, const std::array<int, 4>& ip_filter_data){
    std::vector<ip_addr_type> result;
    for (const auto& v :vec) {
        bool flag = true;
        for (size_t i = 0; i < v.size(); ++i){
            if ((v[i] != ip_filter_data[i])&&(ip_filter_data[i] >= 0)){
// TODO не собирался так for(int i; i < v.size(); ++i): ip_filter.cpp:34:28: error: comparison of integer expressions of different signedness:
// ‘int’ and ‘std::vector<int>::size_type’ {aka ‘long unsigned int’} [-Werror=sign-compare]
        // int hack = 0;
        // for (std::vector<int>::const_iterator c_it = v.cbegin(); c_it != v.cend(); ++c_it){
        //     if ((*c_it != ip_filter_data[hack])&&(ip_filter_data[hack] >= 0)){
                flag = false;
                break;
            }
            // ++hack;
        }
        if (flag) {
            result.push_back(v);
        }
    }    
    return result;         
}

std::vector<ip_addr_type> filter_any(const std::vector<ip_addr_type>& vec, int ip_filter_data){
    std::vector<ip_addr_type> result;
    for (const auto& v :vec) {
        if (std::any_of(v.begin(), v.end(),
                        [&ip_filter_data](const auto& ip_value ) {return ip_filter_data == ip_value;})){
            result.push_back(v);
        }
    }    
    return result;         
}

std::string print_ip_as_str(const ip_addr_type& v){
    std::string result = "";
    for (std::vector<int>::const_iterator c_it = v.cbegin(); c_it != v.cend(); ++c_it){
        if (c_it != v.cbegin()) {
            result+= '.';    
        }
        result+= std::to_string(*c_it);
    }
    return result;
}

std::ostream& operator << (std::ostream& os, const std::vector<ip_addr_type>& vec){
    for (const auto& v : vec){
        os << print_ip_as_str(v) << '\n';
    }
    return os;
}

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
        // std::sort(ip_pool.begin(), ip_pool.end(),
        //             [](const auto& lhs, const auto& rhs) -> bool {
        //                 for (int i = 0; i < lhs.size(); i++) {
        //                     if (lhs.at(i) != rhs.at(i)) {
        //                         return lhs.at(i) > rhs.at(i);
        //                     }
        //                 }
        //                 return false;
        //                 });

        std::cout << ip_pool;

        // TODO filter by first byte and output
        std::cout << filter(ip_pool, {1, -1, -1, -1});

        // TODO filter by first and second bytes and output
        std::cout << filter(ip_pool, {46, 70, -1, -1});

        // TODO filter by any byte and output
        std::cout << filter_any(ip_pool, 46);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}