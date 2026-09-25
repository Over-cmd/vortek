#ifndef WINLATOR_H
#define WINLATOR_H

#include <unistd.h>
#include <syscall.h>
#include <vulkan/vulkan.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))
#define CLAMP(x, low, high) (((x)>(high))?(high):(((x)<(low))?(low):(x)))
#define PACK16(a, b) (a << 16) | (b & 0xffff)
#define PACK32(a, b) ((int64_t)a << 32) | (b & 0xffffffffl)
#define UNPACK32(packed, a, b) int a = (int)(packed >> 32); int b = (int)(packed & 0xffffffffl)
#define IS_POWER_OF_TWO(n) ((n > 0) && ((n & (n - 1)) == 0))
#define SWAP(a, b, type) do { type tmp = a; a = b; b = tmp; } while (0)
#define BITMASK_SET(bits, flag) bits |= flag
#define BITMASK_UNSET(bits, flag) bits &= ~flag
#define GETEXP(x) (31 - __builtin_clz(x))

#define APP_CACHE_DIR "/data/data/com.winlator/cache"
#define LIBVULKAN_PATH "/system/lib64/libvulkan.so"

#define CLOSEFD(x) \
    do { \
        if (x > 0) { \
            close(x); \
            x = -1; \
        } \
    } \
    while(0)

#define MEMFREE(x) \
    do { \
        if (x != NULL) { \
            free(x); \
            x = NULL; \
        } \
    } \
    while(0)

static inline pid_t currentThreadId() {
#ifdef __ANDROID__
    return gettid();
#else
    return syscall(SYS_gettid);
#endif
}

#ifdef __ANDROID__
#include <android/log.h>
#define println(...) __android_log_print(ANDROID_LOG_DEBUG, "System.out", __VA_ARGS__)
#else
#define println(fmt, ...) \
    do { \
        char fmtBuf[256] = {0}; \
        sprintf(fmtBuf, "%s\n", fmt); \
        fprintf(stderr, fmtBuf __VA_OPT__(,) __VA_ARGS__); \
    } \
    while (0)
#endif

/* ── INYECCIÓN DE CONTEXTO GRÁFICO GLOBAL PARA VULKAN_CALLS ── */

// 1. Estructuras de listas genéricas reales utilizadas por Vortek
typedef struct ArrayList {
    void** elements;
    int size;
    int capacity;
} ArrayList;

// 2. Estructuras base necesarias para el serializador y gestor de memoria
typedef struct MemoryPool {
    void* data;
    size_t size;
    size_t offset;
    ArrayList allocationList;
    int allocationList_elements; 
} MemoryPool;

typedef struct VortekContext {
    MemoryPool memoryPool;
} VortekContext;

// 3. Definición estructural del búfer circular (Anillo de comunicación de Vortek)
typedef struct RingBuffer {
    void* data;
    uint32_t head;
    uint32_t tail;
    uint32_t size;
} RingBuffer;

/* 🚨 MOLDES ESPEJO DE ESCRITORIO DEFINITIVOS:
   Declaramos las variables con sus nombres virtuales exactos para satisfacer 
   la asignación directa val->miembro en vortek_serializer.h sin macros de remapeo */
typedef struct VkPhysicalDeviceMapMemoryPlacedFeaturesEXT {
    VkStructureType sType;
    void* pNext;
    VkBool32 memoryMapPlaced;
    VkBool32 memoryMapRangePlaced;
    VkBool32 memoryUnmapReserve;
} VkPhysicalDeviceMapMemoryPlacedFeaturesEXT;

typedef struct VkPhysicalDeviceMapMemoryPlacedPropertiesEXT {
    VkStructureType sType;
    void* pNext;
    VkDeviceSize minPlacedMemoryMapAlignment;
} VkPhysicalDeviceMapMemoryPlacedPropertiesEXT;

/* 🚨 FIJADO: Agregado el struct MappedMemory huérfano exigido por vulkan_calls.c:331 */
typedef struct MappedMemory {
    VkDeviceSize allocationSize;
    void* pMemory;
} MappedMemory;

// 4. Declaración de variables globales huérfanas exigidas por las macros vt_send / vt_recv
extern int serverFd;
extern MemoryPool globalMemoryPool;
extern VortekContext* context;
extern RingBuffer* serverRing;
extern RingBuffer* clientRing;

// 5. Prototipos de funciones nativas llamadas por las macros del serializador
void recv_fds(int socket, int* fds, int* numFds, char* success, int count);

#endif // WINLATOR_H
