file(GLOB RENDER_ROOT_SOURCE *.c)
file(GLOB RENDER_ROOT_HEADER *.h)

add_library(${PROJECT_RENDER_NAME} ${RENDER_ROOT_HEADER} ${RENDER_ROOT_SOURCE})

target_link_libraries(${PROJECT_RENDER_NAME} PUBLIC ${PROJECT_UTILS_NAME} glfw
                                                    cglm ${Vulkan_LIBRARIES})
target_include_directories(
  ${PROJECT_RENDER_NAME}
  PUBLIC ${PROJECT_BINARY_DIR}
         ${PROJECT_SOURCE_DIR}
         ${PROJECT_SOURCE_DIR_LOCATION}
         ${PROJECT_HEADERS}
         ${CMAKE_SOURCE_DIR}
         ${CMAKE_DEPENDENCIES_DIR}/GLFW/include
         ${CMAKE_DEPENDENCIES_DIR}/GLM/include
         ${Vulkan_INCLUDE_DIRS})
