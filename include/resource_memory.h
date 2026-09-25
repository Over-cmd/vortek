#ifndef RESOURCE_MEMORY_H
#define RESOURCE_MEMORY_H

#include <vulkan/vulkan.h>

/* 🚨 MODELO DE ASIGNACIÓN VORTEK MALI-G52:
   Sella el tipo de datos del gestor de memoria para evitar el error 
   de identificador no declarado en el casteo del serializador. */
typedef struct ResourceMemory {
    VkDeviceMemory memory;
    VkDeviceSize size;
    void* pMappedData;
} ResourceMemory;

#endif // RESOURCE_MEMORY_H
