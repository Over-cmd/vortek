cmake_minimum_required(VERSION 3.22.1)

project(Vortek C)
message("Building ${PROJECT_NAME}")

set(CMAKE_VERBOSE_MAKEFILE on)

# Configuración de banderas limpia para Android
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -w -DVK_USE_PLATFORM_ANDROID_KHR")

include_directories(include)

add_library(vulkan_vortek SHARED 
            src/main.c
            src/vulkan_calls.c
            src/vk_object.c
            src/vk_object_pool.c
            src/arrays.c
            src/descriptor_update_template.c
            src/ring_buffer.c)

# 🚀 LA LÍNEA CRÍTICA: Enlazamos explícitamente la librería gráfica de logs de Android (-llog)
target_link_libraries(vulkan_vortek log)
