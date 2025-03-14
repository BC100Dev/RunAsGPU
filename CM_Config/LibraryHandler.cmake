find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBPCI REQUIRED libpci)
include_directories(${LIBPCI_INCLUDE_DIRS})
link_directories(${LIBPCI_LIBRARY_DIRS})