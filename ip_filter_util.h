
#pragma once
#include <iostream>
#include<optional>
#include <vector>
#include <string>

// тип  октетов для удобства 
using ip_addr_type = std::vector<int>;

// не тронута с учебного примера, только октеты храним string на int 
ip_addr_type split(const std::string &str, char d);
 
// TODO попробовать как-то так разобраться template <typename ... Args>, чтобы не тянуть массив, хотя через args будет только строгий порядок 
std::vector<ip_addr_type> filter(const std::vector<ip_addr_type>& vec, const std::array<std::optional<int>, 4>& ip_filter_data);

std::vector<ip_addr_type> filter_any(const std::vector<ip_addr_type>& vec, int ip_filter_data);

std::string print_ip_as_str(const ip_addr_type& v);

std::ostream& operator << (std::ostream& os, const std::vector<ip_addr_type>& vec);
