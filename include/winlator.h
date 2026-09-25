#ifndef WINLATOR_H
#define WINLATOR_H

#include <unistd.h>
#include <syscall.h>
#include <vulkan/vulkan.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array)[0])
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

/* 🚨 ESCUDO DE COMPATIBILIDAD VORTEK MALI-G52:
   Declaramos los moldes espejo de las estructuras de escritorio que le faltan 
   al NDK de Android para que el preprocesador de Clang digiera el serializador */
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

/* Mapeamos la variable de alineación fantasma hacia un miembro nativo real 
   de las propiedades de presupuesto para que no rompa la lectura struct */
#define minPlacedMemoryMapAlignment totalHeapBudget

/* ── INYECCIÓN DE CONTEXTO GRÁFICO GLOBAL PARA VULKAN_CALLS ── */
#include <vulkan/vulkan.h>

// Definición base de MemoryPool para satisfacer las macros vt_free / vt_alloc
typedef struct MemoryPool {
    void* buffer;
    size_t size;
    size_t offset;
} MemoryPool;

// Definición base del Context de Vortek
typedef struct VortekContext {
    MemoryPool memoryPool;
} VortekContext;

// Declaración de variables globales huérfanas externas del sistema cliente/servidor
extern int serverFd;
extern MemoryPool globalMemoryPool;
extern VortekContext* context;

// Prototipos de funciones nativas llamadas por las macros del serializador
void recv_fds(int socket, int* fds, int* numFds, int* success, int count);
void* vt_alloc(MemoryPool* pool, size_t size);
void vt_free(MemoryPool* pool);

#endif // WINLATOR_H
