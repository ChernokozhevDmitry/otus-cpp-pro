cmake_minimum_required(VERSION 3.15)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED TRUE)

option(BUILD_WITH_TESTS "Build with tests" ON)

if(BUILD_WITH_TESTS)
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )

    # set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_executable(tests tests.cpp)
    target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR})
    set_target_properties(tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )    
    target_link_libraries(tests PUBLIC gtest gtest_main)    
endif()


configure_file(version.h.in version.h)

add_executable(ip_filter ip_filter.cpp)

# set_target_properties(helloworld_cli helloworld tests PROPERTIES
set_target_properties(ip_filter PROPERTIES
CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    if(BUILD_WITH_TESTS)
        target_compile_options(tests PRIVATE
            /W4
        )
    endif()

else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(BUILD_WITH_TESTS)
        target_compile_options(tests PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()



install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

if(BUILD_WITH_TESTS)
    enable_testing()
    add_test(tests tests)
endif()
