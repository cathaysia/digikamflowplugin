#
# Copyright (c) 2021-2022, DragonBillow, <DragonBillow at outlook dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

# set QT_VERSION_MAJOR
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)

find_package(
    Qt${QT_VERSION_MAJOR} NO_MODULE
    COMPONENTS Core Widgets Gui
    REQUIRED)
find_package(DigikamCore CONFIG REQUIRED)

set_package_properties(
    DigikamCore PROPERTIES
    URL "http://www.digikam.org"
    DESCRIPTION "digiKam core library")

include(FindThreads REQUIRED)

# For i18n support...
# include(i18nUtils)

include_directories(
    $<TARGET_PROPERTY:Digikam::digikamcore,INTERFACE_INCLUDE_DIRECTORIES>/digikam
)

set(PicFlowView_generic_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/plugflow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/picdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/aspectratiopixmaplabel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugsettings.cpp
    # For i18n Support...
    # ${i18n_QRC_SRCS}
    # ${i18n_QM}
)

add_library(Generic_PicFlowView_Plugin MODULE ${PicFlowView_generic_SRCS})

set_target_properties(Generic_PicFlowView_Plugin PROPERTIES PREFIX "")

target_link_libraries(
    Generic_PicFlowView_Plugin
    PRIVATE Digikam::digikamcore Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui
            Threads::Threads FlowLayout)

macro_add_plugin_install_target(Generic_PicFlowView_Plugin generic)
