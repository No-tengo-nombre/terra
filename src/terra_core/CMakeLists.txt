file(GLOB CORE_ROOT_SOURCE *.c)
file(GLOB CORE_ROOT_HEADER *.h)
file(GLOB CORE_VK_SOURCE vk/*.c)
file(GLOB CORE_VK_HEADER vk/*.h)

add_library(${PROJECT_CORE_NAME} ${CORE_ROOT_HEADER} ${CORE_ROOT_SOURCE} ${CORE_VK_HEADER} ${CORE_VK_SOURCE})

target_link_libraries(${PROJECT_CORE_NAME} PUBLIC ${PROJECT_UTILS_NAME} glfw
                                                    cglm ${Vulkan_LIBRARIES})
target_include_directories(
  ${PROJECT_CORE_NAME}
  PUBLIC ${PROJECT_BINARY_DIR}
         ${PROJECT_SOURCE_DIR}
         ${PROJECT_SOURCE_DIR_LOCATION}
         ${PROJECT_HEADERS}
         ${CMAKE_SOURCE_DIR}
         ${CMAKE_DEPENDENCIES_DIR}/GLFW/include
         ${CMAKE_DEPENDENCIES_DIR}/GLM/include
         ${Vulkan_INCLUDE_DIRS})
