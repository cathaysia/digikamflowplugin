#
# Copyright (c) 2021-2022, DragonBillow, <DragonBillow at outlook dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

cmake_minimum_required(VERSION 3.0)

project(
    digikamflowplugin
    VERSION 0.3.0
    DESCRIPTION "a plugin add a Flow View to digikam"
    HOMEPAGE_URL https://github.com/cathaysia/digikamflowplugin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use common cmake macro from cmake/modules/ to install unistall plugins.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

include(FeatureSummary)
include(MacroInstall)

#

add_subdirectory(extern/QtFlowLayout)
include_directories(extern/QtFlowLayout)
add_subdirectory(src)

#

# To plug install and uninstall plugin macro.

macro_add_uninstall_target()

#

include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "TGZ;RPM")
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}" "${CMAKE_PROJECT_NAME}"
                                 ALL .)
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/pack")

set(CPACK_PACKAGE_VENDOR "cathaysia")
set(CPCK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
set(CPACK_PROJECT_URL "${CMAKE_PROJECT_HOMEPAGE_URL}")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CMAKE_PACKAGE_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_STRIP_FILES ON)
# RPM
set(CPACK_RPM_PACKAGE_LICENSE "GPLv3")
set(CPACK_RPM_PACKAGE_URL ${CPACK_PROJECT_URL})
set(CPACK_RPM_PACKAGE_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_RPM_PACKAGE_REQUIRES "digikam")

include(CPack)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
