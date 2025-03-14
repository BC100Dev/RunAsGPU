CollectSources(${PROJECT_MODULE_ROOT} ModSources)

add_executable(RunAsGPU-CLI ${ModSources})
target_link_libraries(RunAsGPU-CLI PRIVATE ${LIBPCI_LIBRARIES} RunAsGPU-shared)