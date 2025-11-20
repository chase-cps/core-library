include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(chase_tests
    main.cc
    SystemTest.cc
)

target_link_libraries(chase_tests
    PRIVATE
    chase
    GTest::gtest_main
)

target_include_directories(chase_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/representation
    ${CMAKE_SOURCE_DIR}/include/utilities
)

include(GoogleTest)
gtest_discover_tests(chase_tests)
