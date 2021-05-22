
set(AUTOMOC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_library(FlowLayout STATIC flowlayout.cpp)

target_link_libraries(FlowLayout PRIVATE Qt5::Widgets)
target_include_directories(FlowLayout PUBLIC INTERFACE flowlayout.h)
