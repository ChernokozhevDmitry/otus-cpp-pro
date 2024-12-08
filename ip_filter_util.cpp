#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <optional>
#include "ip_filter_util.h"

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

std::vector<ip_addr_type> filter(const std::vector<ip_addr_type>& vec, const std::array<std::optional<int>, 4>& ip_filter_data){
    std::vector<ip_addr_type> result;
    for (const auto& v :vec) {
        bool flag = true;
        for (size_t i = 0; i < v.size(); ++i){
            if ((v[i] != ip_filter_data[i])&&(ip_filter_data[i] != std::nullopt)){
                flag = false;
                break;
            }
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