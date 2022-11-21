cmake_minimum_required(VERSION 3.23)
project(project_three)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

message(STATUS "Starting the build of Project 3 👍")
add_executable(project_three
		driver.cpp
		driver.h
		table.cpp
		table.h
		website.h
		website.cpp
		)

message(STATUS "Complete project built with no errors")
