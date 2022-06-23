cmake_minimum_required(VERSION 3.0.0)
project(mcl_test VERSION 0.1.0)


#set(CMAKE_BUILD_TYPE "RELEASE")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 ")
include_directories (./include)

add_executable(prove prove.cpp circom.h function.h threadpool.h sm2_create_key_pair.c sm2_sign_and_verify.c sm3_with_preprocess.c)
add_executable(verify verify.cpp  function.h threadpool.h)

find_library(MCL_LIB mcl HINTS ${PROJECT_SOURCE_DIR})
find_library(SSL_LIB ssl HINTS ${PROJECT_SOURCE_DIR})
find_library(CRYPTO_LIB crypto HINTS ${PROJECT_SOURCE_DIR})
find_library(NTL_LIB ntl HINTS ${PROJECT_SOURCE_DIR})
find_library(NTL_LIB ntl HINTS ${PROJECT_SOURCE_DIR})
 




target_link_libraries(prove gmp m)
target_link_libraries(prove dl)
target_link_libraries(prove jsoncpp)
target_link_libraries(prove ${NTL_LIB} m pthread)
target_link_libraries(prove ${MCL_LIB})
target_link_libraries(prove ${SSL_LIB}) 
target_link_libraries(prove ${CRYPTO_LIB} )

target_link_libraries(verify gmp m)
target_link_libraries(verify jsoncpp)
target_link_libraries(verify ${NTL_LIB} m pthread)
target_link_libraries(verify ${MCL_LIB})
target_link_libraries(verify ${SSL_LIB})
target_link_libraries(verify ${CRYPTO_LIB} )
