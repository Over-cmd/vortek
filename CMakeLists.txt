cmake_minimum_required(VERSION 3.22.1)

project(Vortek C)

message("Building ${PROJECT_NAME}")

set(CMAKE_VERBOSE_MAKEFILE on)

# Usamos la bandera de advertencias compatible con el Clang moderno de Google/NDK
set(WARNS "-Wall -Wno-ignored-qualifiers")

# Aplicamos optimización total de velocidad e inyectamos la plataforma Android nativa
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ${WARNS} -DVK_USE_PLATFORM_ANDROID_KHR")

include_directories(include)

add_library(vulkan_vortek SHARED
    src/main.c
    src/vulkan_calls.c
    src/vk_object.c
    src/vk_object_pool.c
    src/arrays.c
    src/descriptor_update_template.c
    src/ring_buffer.c)
