#
# Copyright (c) 2021-2022, DragonBillow, <DragonBillow at outlook dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

set(CMAKE_AUTOMOC ON)

if(BUILD_WITH_QT6)

    find_package(Qt6 "6.5.0" REQUIRED
                 NO_MODULE COMPONENTS
                 Core
                 Widgets
                 Gui
                 Concurrent
    )

    set(QT_VERSION_MAJOR 6)

else()

    find_package(Qt5 "5.6.0" REQUIRED
                 NO_MODULE COMPONENTS
                 Core
                 Widgets
                 Gui
                 Concurrent
    )

    set(QT_VERSION_MAJOR 5)

endif()


find_package(DigikamCore CONFIG REQUIRED)

set_package_properties(
    DigikamCore PROPERTIES
    URL "http://www.digikam.org"
    DESCRIPTION "digiKam core library")

include(FindThreads REQUIRED)

# For i18n support...
# include(i18nUtils)

include_directories($<TARGET_PROPERTY:Digikam::digikamcore,INTERFACE_INCLUDE_DIRECTORIES>)

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
