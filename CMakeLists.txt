cmake_minimum_required (VERSION 3.30)
set(CMAKE_VERBOSE_MAKEFILEON ON)

project (http_ssl_example)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR arm)
#message(STATUS "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
#message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_NAME}")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-DDEBUG) #debug
endif()
#remove_definitions(-DDEBUG)

add_definitions(-DUSING_BOOST_LOCALE) #设置使用locale库转换gbk to utf8

unset(DEBUG)
set(Boost_ROOT E:/vcpkg/installed/x64-mingw-static/share)

#set(OPENSSL_ROOT  E:/vcpkg/installed/x64-mingw-dynamic/share)
find_package(Boost CONFIG COMPONENTS asio program_options locale date_time)
#find_package(Boost CONFIG COMPONENTS asio program_options PATHS E:/vcpkg/installed/x64-mingw-static/share)
find_package(OpenSSL CONFIG COMPONENTS ssl crypto PATHS E:/vcpkg/installed/x64-mingw-dynamic/share)
find_package(unofficial-sqlite3 CONFIG REQUIRED PATHS E:/vcpkg/installed/x64-mingw-dynamic)

add_subdirectory(D:/mvnworkspace/example/GBKtoUTF8 GBKtoUTF8.out)
add_subdirectory(D:/mvnworkspace/example/debug_macro debug_macro.out)

set(post_option_target "get_post_option") #获取配置

set(WHMP 1) #智慧家庭
set (ENERGYSAVING 0) #基站节能
IF (ENERGYSAVING)
set("${post_option_target}_FILES" 

				src/get_post_option1.cpp

)
elseif(WHMP)
set("${post_option_target}_FILES" 

			
				src/get_post_option.cpp
				)

endif()
#add_executable(${post_option_target})
add_library (${post_option_target})
set_target_properties(${post_option_target}  PROPERTIES SOURCES "${${post_option_target}_FILES}")
set_target_properties(${post_option_target}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(${post_option_target}  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
#设置目标引用的包含文件目录
#set_target_properties(${post_option_target}  PROPERTIES INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
target_link_libraries(${post_option_target}  Boost::program_options Boost::locale gbktoutf8)



set("${PROJECT_NAME}_SERVER_FILES" 
#				src/http_mark.h
#				src/HttpBase.h
#				src/httpBase.cpp
#				src/httpBoost.cpp
#				src/httpBoost.h
#				src/sync_client.cpp
			src/server.cpp
#				src/example.cpp
)
#message(STATUS "${${PROJECT_NAME}_FILES}")
#set(Boost_ROOT e:/Boost-1.86.0)

#set(Boost_DIR E:/vcpkg/packages)
#set(Boost_config_DIR e:/Boost-1.86.0/lib/cmake/boost_config-1.86.0)
add_library(ws2_32lib SHARED IMPORTED)
set_target_properties(ws2_32lib PROPERTIES IMPORTED_IMPLIB "E:/w64devkit-1.19.0/x86_64-w64-mingw32/lib/libws2_32.a")
add_library(wsock32lib SHARED IMPORTED)
set_target_properties(wsock32lib PROPERTIES IMPORTED_IMPLIB "E:/w64devkit-1.19.0/x86_64-w64-mingw32/lib/libwsock32.a")

#find_package(OpenSSL CONFIG COMPONENTS ssl crypto)

include(D:/mvnworkspace/find_boost.cmake)
include(D:/mvnworkspace/find_openssl.cmake)

#aux_source_directory("${PROJECT_SOURCE_DIR}/src" SRC_FILES)
#message(STATUS "SRC_FILES=${SRC_FILES}")
set(client_target "http_post_client")

IF (ENERGYSAVING)
set("${client_target}_FILES" 
#				src/http_mark.h
#				src/HttpBase.h
#				src/httpBase.cpp
#				src/httpBoost.cpp
#				src/httpBoost.h
#				src/sync_client.cpp
			src/http_post_client.cpp
#				src/example.cpp
)
elseif(WHMP)
set("${client_target}_FILES" 
			
				src/http_post_client_whmp.cpp
				)

endif()
add_executable(${client_target})
#set_target_properties(${PROJECT_NAME}  PROPERTIES SOURCES "${SRC_FILES}")
set_target_properties(${client_target}  PROPERTIES SOURCES "${${client_target}_FILES}")
set_target_properties(${client_target}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set_target_properties(${client_target}  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
#设置目标引用的包含文件目录
set_target_properties(${client_target}  PROPERTIES INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
target_link_libraries(${client_target}   ws2_32lib wsock32lib ${post_option_target} debug_macro Boost::date_time Boost::program_options OpenSSL::Crypto OpenSSL::SSL )

set(server_target "${PROJECT_NAME}_SERVER")

#add_executable(${server_target} "${${server_target}_FILES}" )
add_executable(${server_target})
set_target_properties(${server_target}  PROPERTIES SOURCES "${${server_target}_FILES}")
set_target_properties(${server_target}  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
#设置目标引用的包含文件目录
set_target_properties(${server_target}  PROPERTIES INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
target_link_libraries(${server_target}   ws2_32lib wsock32lib Boost::program_options OpenSSL::Crypto OpenSSL::SSL )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++ -std=c++17 -g -Wall  -O2 -Wdeprecated-declarations")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -D_GLIBCXX_USE_CXX11_ABI=0 -g -Wall  -O2")
