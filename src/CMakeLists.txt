set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(FlowLayout)
include_directories(FlowLayout)
find_package(Qt5 NO_MODULE COMPONENTS Core Widgets Gui REQUIRED)

find_package(DigikamCore CONFIG REQUIRED)

set_package_properties(DigikamCore PROPERTIES
    URL "http://www.digikam.org"
    DESCRIPTION "digiKam core library"
)

add_library(
    PicFlowView MODULE
    PicFlowView.cpp
    PicFlowView.cpp
)

set_target_properties(PicFlowView PROPERTIES PREFIX "")

target_include_directories(
    PicFlowView PRIVATE
    Digikam::digikamcore
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

target_link_libraries(
    PicFlowView PRIVATE
    Digikam::digikamcore
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    FlowLayout
)

include(GNUInstallDirs)
find_file(DIGIKAM_PLUGIN_CONFIG DigikamPluginConfig.cmake
    HINTS ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/DigikamPlugin/
    HINTS /usr/lib/cmake/DigikamPlugin/
)
include(${DIGIKAM_PLUGIN_CONFIG})
message(STATUS "PicFlowView will be installed to /usr/${DPluginGenericInstallPath}")
install(TARGETS PicFlowView DESTINATION /usr/${DPluginGenericInstallPath})


