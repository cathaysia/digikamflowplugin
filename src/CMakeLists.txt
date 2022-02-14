set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 NO_MODULE COMPONENTS Core Widgets Gui REQUIRED)
find_package(DigikamCore CONFIG REQUIRED)
include(FindThreads REQUIRED)

add_library(
  Generic_PicFlowView_Plugin
  MODULE
  flowplugin.cpp
  picdialog.cpp
  aspectratiopixmaplabel.cpp

  flowplugin.hpp
  picdialog.hpp
  aspectratiopixmaplabel.hpp
)

set_target_properties(
  Generic_PicFlowView_Plugin
  PROPERTIES PREFIX ""
  LINK_WHAT_YOU_USE ON
)

target_include_directories(
  Generic_PicFlowView_Plugin PRIVATE
  Digikam::digikamcore
  Qt5::Core
  Qt5::Widgets
  Qt5::Gui
  Threads::Threads
)

target_link_libraries(
  Generic_PicFlowView_Plugin PRIVATE
  Digikam::digikamcore
  Qt5::Core
  Qt5::Widgets
  Qt5::Gui
  Threads::Threads
  FlowLayout
)

# 这里需要使用相对路径，否则 CPack 不识别
install(
  # TARGETS Generic_PicFlowView_Plugin DESTINATION "${DPluginGenericInstallPath}"
  TARGETS Generic_PicFlowView_Plugin DESTINATION "lib64/qt5/plugins/digikam/generic/"
)
