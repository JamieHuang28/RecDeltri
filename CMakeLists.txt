cmake_minimum_required(VERSION 2.8.3)
project(concavehull)
set(CMAKE_CXX_STANDARD 11)
# set build type to release
set(CMAKE_BUILD_TYPE Debug)

set(PYBIND11_PYTHON_VERSION 3.7)
find_package(pybind11 REQUIRED)

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()

# include helper file
include( ${CGAL_USE_FILE} )


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  
endif()

# include for local directory
include_directories( BEFORE include )

# include for local package

pybind11_add_module(${PROJECT_NAME} MODULE src/bindings.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE ${pybind11_LIBRARIES})

# Link the executable to CGAL and third-party libraries
target_link_libraries(${PROJECT_NAME}   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )

add_executable(example src/example.cpp)
add_executable(test_hier_skel src/test_hier_skel.cpp)