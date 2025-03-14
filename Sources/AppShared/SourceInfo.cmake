CollectSources(${PROJECT_MODULE_ROOT} MOD_SOURCES)

add_library(RunAsGPU-shared OBJECT ${MOD_SOURCES})

target_link_libraries(RunAsGPU-shared PUBLIC ${LIBPCI_LIBRARIES})
target_include_directories(RunAsGPU-shared PUBLIC ${LIBPCI_INCLUDE_DIRS})
target_compile_options(RunAsGPU-shared PUBLIC ${LIBPCI_CFLAGS_OTHER})