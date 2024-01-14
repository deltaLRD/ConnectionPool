project(ConnectionPool)
cmake_minimum_required(VERSION 3.20)
message("ConnectionPool")

find_package(unofficial-libmysql REQUIRED)

set(ConnectionPool_SRC 
    src/connection_pool.c
    src/csv_reader.c
    src/vector.c
)

IF (CMAKE_BUILD_TYPE STREQUAL Debug) 
    ADD_DEFINITIONS(-DDEBUG)
ENDIF()

add_library(ConnectionPool_Static ${ConnectionPool_SRC})
target_include_directories(ConnectionPool_Static PUBLIC include)
target_link_libraries(ConnectionPool_Static PRIVATE unofficial::libmysql::libmysql)

# if(CMAKE_COMPILER_IS_GNUCC)
#     target_compile_options(ConnectionPool_Static PRIVATE -Wno-pointer-to-int-cast)
# endif()
add_executable(ConnectionPool src/main.c)
target_compile_definitions(ConnectionPool PUBLIC _XOPEN_SOURCE)
target_include_directories(ConnectionPool PUBLIC include)
target_link_libraries(ConnectionPool PRIVATE ConnectionPool_Static)

add_executable(Test01 test/test.c )
target_include_directories(Test01 PUBLIC include)
target_link_libraries(Test01 PRIVATE ConnectionPool_Static)
enable_testing()
add_test(NAME Test01 COMMAND $<TARGET_FILE:Test01> WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

add_executable(Test02 test/testVector.c)
target_include_directories(Test02 PUBLIC include)
target_link_libraries(Test02 PRIVATE ConnectionPool_Static)
add_test(NAME Test02 COMMAND $<TARGET_FILE:Test02> WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

add_executable(Test03 test/testCSVReader.c)
target_include_directories(Test03 PUBLIC include)
target_link_libraries(Test03 PRIVATE ConnectionPool_Static)
add_test(NAME Test03 COMMAND $<TARGET_FILE:Test03> WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

add_executable(Test04 test/testConnectionPool.c)
target_include_directories(Test04 PUBLIC include)
target_link_libraries(Test04 PRIVATE ConnectionPool_Static)
add_test(NAME Test04 COMMAND $<TARGET_FILE:Test04> WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)