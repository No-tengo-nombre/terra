file(GLOB VMA_ROOT_SOURCE *.cpp)

add_library(${PROJECT_VMA_NAME} ${VMA_ROOT_HEADER} ${VMA_ROOT_SOURCE})

target_link_libraries(${PROJECT_VMA_NAME} PUBLIC ${Vulkan_LIBRARIES})
target_include_directories(
  ${PROJECT_VMA_NAME}
  PUBLIC ${PROJECT_BINARY_DIR}
         ${PROJECT_SOURCE_DIR}
         ${PROJECT_SOURCE_DIR_LOCATION}
         ${PROJECT_HEADERS}
         ${CMAKE_SOURCE_DIR}
         ${CMAKE_DEPENDENCIES_DIR}/GLFW/include
         ${CMAKE_DEPENDENCIES_DIR}/GLM/include
         ${CMAKE_DEPENDENCIES_DIR}/VulkanMemoryAllocator/include
         ${Vulkan_INCLUDE_DIRS})
