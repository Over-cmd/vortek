#ifndef WINLATOR_H
#define WINLATOR_H

#include <unistd.h>
#include <syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vulkan/vulkan.h>

// 🚨 FIJADO: Corregida la macro matemática real dividiendo por el índice [0] para la vkDispatchTable
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
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
        char fmtBuf = {0}; \
        sprintf(fmtBuf, "%s\n", fmt); \
        fprintf(stderr, fmtBuf __VA_OPT__(,) __VA_ARGS__); \
    } \
    while (0)
#endif

/* 🚨 BLINDAJE DE COMPATIBILIDAD DEFINITIVO X11/WAYLAND BANNERLATOR 🚨 */

// Tipos opacos requeridos por la firma
typedef void* Display;
typedef unsigned long VisualID;
typedef void* VkAllocationCallbacks;

// 🚨 FIJADO MASTER: Forzamos la macro sobre la estructura para aplastar la definición void* del NDK 
// Esto obliga a Clang a ver a pCreateInfo->window como una estructura lícita en la línea 1900
#define VkXlibSurfaceCreateInfoKHR struct VkXlibSurfaceCreateInfoKHR
VkXlibSurfaceCreateInfoKHR {
    VkStructureType sType;
    const void* pNext;
    uint32_t flags;
    Display dpy;
    unsigned long window; 
};

// Estructura de memoria exigida por el serializador en la línea 16332
typedef struct ResourceMemory {
    VkDeviceMemory memory;
    VkDeviceSize size;
    void* pMappedData;
} ResourceMemory;

// Firma nativa externa para enlazar las llamadas de descriptores de archivos de Winlator
void recv_fds(int socket, int* fds, int* numFds, void* success, int count);

#endif // WINLATOR_H
