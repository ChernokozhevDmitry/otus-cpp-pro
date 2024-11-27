cmake_minimum_required(VERSION 3.5)

project(02HW-ip_filter CXX)

add_executable(ip_filter
               ip_filter.cpp
)

set_target_properties(ip_filter
                      PROPERTIES CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED ON
)

target_compile_features(ip_filter PUBLIC cxx_std_17)

if (MSVC)
  target_compile_options(ip_filter PUBLIC /W3 /WX)
else()
  target_compile_options(ip_filter PUBLIC -Wall -Wextra -Werror -pedantic)
endif()