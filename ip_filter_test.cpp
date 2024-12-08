#include "gtest/gtest.h"
#include "ip_filter_util.h"
#include <algorithm>
#include <array>
#include <optional>
#include <vector>
#include <iostream>


const std::vector<ip_addr_type> ip_pool  {{0, 1, 2, 3}, {1, 2, 3, 4}, {2, 3, 4, 5}, {3, 4, 5, 6}, {4, 5, 6, 7},
                                          {5, 6, 7, 8}, {6, 7, 8, 9}, {7, 8, 9, 0}, {8, 9, 0, 1}, {9, 0, 1, 2}};

TEST(SPLITDATA, test_split) {
    const ip_addr_type expected = {1, 2, 3, 4};
    EXPECT_EQ(expected, split("1.2.3.4", '.'));
}
 

TEST(FILTRATION_TEST, test_filter_Y_n_n_n) {
    const std::vector<ip_addr_type> expected {{7, 8, 9, 0}};
    EXPECT_EQ(expected, filter(ip_pool, {7, std::nullopt, std::nullopt, std::nullopt}));
}

TEST(FILTRATION_TEST, test_filter_n_Y_n_n) {
    const std::vector<ip_addr_type> expected {{7, 8, 9, 0}};
    EXPECT_EQ(expected, filter(ip_pool, {std::nullopt, 8, std::nullopt, std::nullopt}));
}

TEST(FILTRATION_TEST, test_filter_n_n_Y_n) {
    const std::vector<ip_addr_type> expected {{7, 8, 9, 0}};
    EXPECT_EQ(expected, filter(ip_pool, {std::nullopt, std::nullopt, 9, std::nullopt}));
}

TEST(FILTRATION_TEST, test_filter_n_n_n_Y) {
    const std::vector<ip_addr_type> expected {{7, 8, 9, 0}};
    EXPECT_EQ(expected, filter(ip_pool, {std::nullopt, std::nullopt, std::nullopt, 0}));
}

TEST(FILTRATION_TEST, test_filter_any) {
    const std::vector<ip_addr_type> expected {{0, 1, 2, 3}, {1, 2, 3, 4}, {8, 9, 0, 1}, {9, 0, 1, 2}};
    EXPECT_EQ(expected, filter_any(ip_pool, 1));
}

TEST(OUTPUT, test_print_ip_as_str) {
   const std::string expected = "1.2.3.4";
   EXPECT_EQ(expected, print_ip_as_str({1, 2, 3, 4}));
}