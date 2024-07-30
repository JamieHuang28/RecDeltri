cmake_minimum_required(VERSION 2.8.3)
project(concavehull)
set(CMAKE_CXX_STANDARD 11)

set(PYBIND11_PYTHON_VERSION 3.7)
find_package(pybind11 REQUIRED)

pybind11_add_module(${PROJECT_NAME} MODULE src/bindings.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${pybind11_LIBRARIES})