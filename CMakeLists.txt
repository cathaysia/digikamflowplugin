cmake_minimum_required(VERSION 3.0)

project(digikamflowplugin
    VERSION 0.0.3
    DESCRIPTION "a plugin add a Flow View to digikam"
    HOMEPAGE_URL https://github.com/cathaysia/digikamflowplugin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(FeatureSummary)
include(CMakePrintHelpers)

add_subdirectory(extern/QtFlowLayout)
include_directories(extern/QtFlowLayout)
add_subdirectory(src)

set(CPACK_PACKAGE_VENDOR Z)
set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/pack)
include(CPack)
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME})
cmake_print_variables(CPACK_PACKAGE_FILE_NAME)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
