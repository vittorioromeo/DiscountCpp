cmake_minimum_required(VERSION 3.0)

# Include `vrm_cmake`.
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/../vrm_cmake/cmake/"
    "${CMAKE_SOURCE_DIR}/extlibs/vrm_cmake/cmake/")

include(vrm_cmake)

vrm_cmake_init_project(DiscountCpp)
vrm_cmake_add_common_compiler_flags()

find_library(LIB_MARKDOWN markdown)
find_path(INC_MARKDOWN mkdio.h)
include_directories(${INC_MARKDOWN})

set(DISCOUNTCPP_INC_DIR "${DISCOUNTCPP_SOURCE_DIR}/include/")
include_directories(${DISCOUNTCPP_INC_DIR})

vrm_cmake_header_only_install_glob("${DISCOUNTCPP_INC_DIR}" "include")
