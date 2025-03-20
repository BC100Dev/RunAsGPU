set(CMAKE_AUTOMOC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

pkg_check_modules(LIBPCI REQUIRED libpci)
include_directories(${LIBPCI_INCLUDE_DIRS})
link_directories(${LIBPCI_LIBRARY_DIRS})