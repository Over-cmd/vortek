cmake_minimum_required(VERSION 3.22.1)

project(Vortek C)

message("Building ${PROJECT_NAME}")

set(CMAKE_VERBOSE_MAKEFILE on)

# Usamos banderas de advertencia compatibles con LLVM Clang y visibilidad por defecto
set(WARNS "-Wall -Wno-ignored-qualifiers -Wno-incompatible-pointer-types-discards-qualifiers -fvisibility=default")

# Inyectamos los identificadores numéricos de PC ausentes en las cabeceras móviles
set(VK_DEFS "-DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_FEATURES_EXT=1000272000 -DVK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAP_MEMORY_PLACED_PROPERTIES_EXT=1000272001")

# Activamos optimización de rendimiento y la plataforma Android nativa
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ${WARNS} ${VK_DEFS} -DVK_USE_PLATFORM_ANDROID_KHR")

include_directories(include)

# El objetivo se llama unificadamente vulkan_wrapper para Bannerlator
add_library(vulkan_wrapper SHARED
    src/main.c
    src/vulkan_calls.c
    src/vk_object.c
    src/vk_object_pool.c
    src/arrays.c
    src/descriptor_update_template.c
    src/ring_buffer.c)

# 🚨 JUGADA MAESTRA DEFINITIVA: Creamos alias directos en el linker para exportar los nombres ICD limpios 
# Esto hace que vt_call_vkCreateInstance se exponga al sistema biónico como vkCreateInstance de forma legítima
set(LINK_ALIASES
    "-Wl,--defsym=vkGetInstanceProcAddr=vt_call_vkGetInstanceProcAddr"
    "-Wl,--defsym=vkGetDeviceProcAddr=vt_call_vkGetDeviceProcAddr"
    "-Wl,--defsym=vkCreateInstance=vt_call_vkCreateInstance"
    "-Wl,--defsym=vkDestroyInstance=vt_call_vkDestroyInstance"
    "-Wl,--defsym=vkEnumeratePhysicalDevices=vt_call_vkEnumeratePhysicalDevices"
    "-Wl,--defsym=vkCreateDevice=vt_call_vkCreateDevice"
    "-Wl,--defsym=vkDestroyDevice=vt_call_vkDestroyDevice"
    "-Wl,--defsym=vkEnumerateInstanceVersion=vt_call_vkEnumerateInstanceVersion"
    "-Wl,--defsym=vkEnumerateInstanceExtensionProperties=vt_call_vkEnumerateInstanceExtensionProperties"
    "-Wl,--defsym=vkEnumerateDeviceExtensionProperties=vt_call_vkEnumerateDeviceExtensionProperties"
)

# Unificamos los alias en las propiedades de enlace del binario compartido
string(REPLACE ";" " " LINK_ALIASES_STR "${LINK_ALIASES}")
set_target_properties(vulkan_wrapper PROPERTIES LINK_FLAGS "${LINK_ALIASES_STR}")

# Vinculamos la librería de logs nativa de Android al target
target_link_libraries(vulkan_wrapper PRIVATE log)
