cmake_minimum_required(VERSION 3.22.1)

project(Vortek C)

message("Building ${PROJECT_NAME}")

set(CMAKE_VERBOSE_MAKEFILE on)

# Usamos banderas de advertencia compatibles con LLVM Clang
set(WARNS "-Wall -Wno-ignored-qualifiers -Wno-incompatible-pointer-types-discards-qualifiers")

# Inyectamos los identificadores numéricos de PC ausentes en las cabeceras móviles
set(VK_DEFS "-DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_FEATURES_EXT=1000272000 -DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_PROPERTIES_EXT=1000272001")

# Activamos optimización agresiva -O3 para tu GPU Mali e inyectamos la plataforma Android nativa
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ${WARNS} ${VK_DEFS} -DVK_USE_PLATFORM_ANDROID_KHR")

include_directories(include)

# 🚨 FIJADO MASTER: El objetivo se llama unificadamente vulkan_wrapper para Bannerlator
add_library(vulkan_wrapper SHARED
    src/main.c
    src/vulkan_calls.c
    src/vk_object.c
    src/vk_object_pool.c
    src/arrays.c
    src/descriptor_update_template.c
    src/ring_buffer.c)

# Vinculamos la librería de logs nativa de Android al nuevo target unificado
target_link_libraries(vulkan_wrapper PRIVATE log)
