cmake_minimum_required(VERSION 2.8)

project(TCP/UDP_Server)			# Название проекта

SET(CMAKE_CXX_FLAGS -pthread)

set(SOURCE_EXE main.cpp)		# Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB Server.cpp)			# Тоже самое, но для библиотеки

add_library(Server STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем foo

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

target_link_libraries(main Server)		# Линковка программы с библиотекой

add_executable(client client.cpp)	# Создает исполняемый файл с именем main