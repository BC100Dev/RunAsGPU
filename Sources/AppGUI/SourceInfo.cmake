CollectSources(${PROJECT_MODULE_ROOT} ModSources)

include_directories("${PROJECT_MODULE_ROOT}/SharedData")

add_executable(RunAsGPU ${ModSources})

target_link_libraries(RunAsGPU PRIVATE RunAsGPU-shared Qt6::Widgets Qt6::Gui Qt6::Core)

qt_standard_project_setup()