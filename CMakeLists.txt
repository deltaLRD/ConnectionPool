project(ConnectionPool)
cmake_minimum_required(VERSION 3.20)
message("ConnectionPool")

find_package(unofficial-libmysql REQUIRED)
add_executable(ConnectionPool src/main.c)
target_include_directories(ConnectionPool PUBLIC include)
target_link_libraries(ConnectionPool PRIVATE unofficial::libmysql::libmysql)

add_executable(Test01 test/test.c)
enable_testing()
add_test(NAME Test01 COMMAND $<TARGET_FILE:Test01>)