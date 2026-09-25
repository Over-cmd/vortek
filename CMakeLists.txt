cmake_minimum_required(VERSION 3.22.1)

project(VortekLayer C)

message("Building ${PROJECT_NAME} as a Vulkan Layer")

set(CMAKE_VERBOSE_MAKEFILE on)

# Banderas de advertencia LLVM Clang con visibilidad pública obligatoria para capas
set(WARNS "-Wall -Wno-ignored-qualifiers -Wno-incompatible-pointer-types-discards-qualifiers -fvisibility=default")

# Identificadores numéricos de compatibilidad para tu GPU
set(VK_DEFS "-DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_FEATURES_EXT=1000272000 -DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_PROPERTIES_EXT=1000272001")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ${WARNS} ${VK_DEFS} -DVK_USE_PLATFORM_ANDROID_KHR")

include_directories(include)

# El objetivo se llama unificadamente vulkan_wrapper para que la APK de Bannerlator lo asimile
add_library(vulkan_wrapper SHARED
    src/main.c
    src/vulkan_calls.c
    src/vk_object.c
    src/vk_object_pool.c
    src/arrays.c
    src/descriptor_update_template.c
    src/ring_buffer.c)

# Forzamos la exportación pública de la función de negociación de capas e inyectamos los alias ICD
set(LINK_ALIASES
    "-Wl,--export-dynamic"
    "-Wl,--defsym=vkNegotiateLoaderLayerInterfaceVersion=vt_vkNegotiateLoaderLayerInterfaceVersion"
    "-Wl,--defsym=vkGetInstanceProcAddr=vt_call_vkGetInstanceProcAddr"
    "-Wl,--defsym=vkGetDeviceProcAddr=vt_call_vkGetDeviceProcAddr"
)

string(REPLACE ";" " " LINK_ALIASES_STR "${LINK_ALIASES}")
set_target_properties(vulkan_wrapper PROPERTIES LINK_FLAGS "${LINK_ALIASES_STR}")

# Vinculamos la librería de logs nativa de Android al target
target_link_libraries(vulkan_wrapper PRIVATE log)
