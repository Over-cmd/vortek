cmake_minimum_required(VERSION 3.22.1)

project(Vortek C)

message("Building ${PROJECT_NAME}")

set(CMAKE_VERBOSE_MAKEFILE on)

# Usamos banderas seguras e inyectamos compatibilidad para que ignore los warnings de punteros const
set(WARNS "-Wall -Wno-ignored-qualifiers -Wno-incompatible-pointer-types-discards-qualifiers")

# 🚨 EL TRUCO MAESTRO: Mapeamos los identificadores de PC que le faltan al NDK de Android hacia IDs neutros o sus valores reales de la API (1000272000+)
set(VK_DEFS "-DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_FEATURES_EXT=1000272000 -DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_PROPERTIES_EXT=1000272001")

# Aplicamos la optimización de velocidad e inyectamos las definiciones de Vulkan y Android nativo
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ${WARNS} ${VK_DEFS} -DVK_USE_PLATFORM_ANDROID_KHR")

include_directories(include)

add_library(vulkan_vortek SHARED
    src/main.c
    src/vulkan_calls.c
    src/vk_object.c
    src/vk_object_pool.c
    src/arrays.c
    src/descriptor_update_template.c
    src/ring_buffer.c)
