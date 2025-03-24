CollectSources(${PROJECT_MODULE_ROOT} ModSources)

include_directories("${PROJECT_MODULE_ROOT}/SharedData")

add_executable(RunAsGPU ${ModSources} ${PROJECT_MODULE_ROOT}/Resources/resources.qrc)

target_link_libraries(RunAsGPU PRIVATE RunAsGPU-shared Qt6::Widgets Qt6::Gui Qt6::Core)