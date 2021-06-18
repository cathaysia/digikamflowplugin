set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 NO_MODULE COMPONENTS Core Widgets Gui REQUIRED)

find_package(DigikamCore CONFIG REQUIRED)

include(FindThreads)

if(NOT ${Threads_FOUND})
message(FATAL "线程库未发现")
endif()

set_package_properties(DigikamCore PROPERTIES
    URL "http://www.digikam.org"
    DESCRIPTION "digiKam core library"
)

add_library(
    PicFlowView MODULE
    PicFlowView.cpp
    PicFlowView.cpp
)

set_target_properties(PicFlowView
    PROPERTIES PREFIX ""
    LINK_WHAT_YOU_USE ON)
target_include_directories(
    PicFlowView PRIVATE
    Digikam::digikamcore
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Threads::Threads
)

target_link_libraries(
    PicFlowView PRIVATE
    Digikam::digikamcore
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Threads::Threads
    FlowLayout
)

include(GNUInstallDirs)
find_file(DIGIKAM_PLUGIN_CONFIG DigikamPluginConfig.cmake
    HINTS ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/DigikamPlugin/
    HINTS /usr/lib64/cmake/DigikamPlugin/
    HINTS /usr/lib/cmake/DigikamPlugin/
)
include(${DIGIKAM_PLUGIN_CONFIG})
cmake_print_variables(DPluginGenericInstallPath)
# 这里需要使用相对路径，否则 CPack 不识别
install(TARGETS PicFlowView
    LIBRARY DESTINATION "${DPluginGenericInstallPath}")
