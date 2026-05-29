cmake_minimum_required(VERSION 4.3.0)
project("SONIIC4Ep1")

set(SOURCE_FILES
  game/main.cpp
  game/
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
