// stb_image.h - public domain image loader
// https://github.com/nothings/stb

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#ifndef STB_IMAGE_H
#define STB_IMAGE_H

// Disable warnings for MSVC
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#define STBI_ASSERT(x)
#define STBI_MALLOC(sz)           malloc(sz)
#define STBI_REALLOC(p,newsz)     realloc(p,newsz)
#define STBI_FREE(p)              free(p)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
extern unsigned char *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);
extern void stbi_image_free(void *retval_from_stbi_load);
extern const char *stbi_failure_reason(void);

#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // STB_IMAGE_H

// --- Implementation Section ---
#ifdef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stdint.h>

#define STBI_NO_PSD
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

// Include full stb_image implementation
#define STBI_NO_THREAD_LOCALS
#define STBI_NO_LINEAR
#define STBI_NO_JPEG
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#endif
