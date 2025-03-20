set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

qt_standard_project_setup()

pkg_check_modules(LIBPCI REQUIRED libpci)
include_directories(${LIBPCI_INCLUDE_DIRS})
link_directories(${LIBPCI_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")