cmake_minimum_required(VERSION 3.5)
project(CommLib)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()



# set(BUILD_SHARED_LIBS OFF)
# include(FetchContent)

# FetchContent_Declare(magic_enum
#   GIT_REPOSITORY    https://github.com/Neargye/magic_enum.git
#   SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/magic_enum
# )
# set(BUILD_SHARED_LIBS ON)
# FetchContent_GetProperties(magic_enum)
# if(NOT magic_enum_POPULATED)
#   FetchContent_Populate(magic_enum)
#   add_subdirectory(${magic_enum_SOURCE_DIR} ${magic_enum_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
# set(BUILD_SHARED_LIBS ON)

find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)

include_directories(
  include
  ${SPDLOG_INCLUDE_DIR}
)

set(library_name ${PROJECT_NAME})

add_library(${library_name}
  src/comm_lib/comm.cpp
  src/comm_lib/methods/uart.cpp
)
add_library(${PROJECT_NAME}::${library_name} ALIAS ${library_name})
target_link_libraries(${library_name} PRIVATE spdlog::spdlog_header_only Threads::Threads)

add_executable(test src/main.cpp)
add_dependencies(test ${library_name})
target_link_libraries(test ${library_name})

include(GNUInstallDirs)

install(TARGETS ${library_name} 
  EXPORT  ${PROJECT_NAME}Targets
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}                     
  RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}                       
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

export(PACKAGE ${PROJECT_NAME})

install(DIRECTORY include/CommLib
  DESTINATION include
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)