#pragma once
#include <glad/glad.h>

struct glExt
{
    inline static void (*NamedStringARB)(int type, int namelen, const char* name,
        int stringlen, const char* string);

    inline  static void (*DeleteNamedStringARB) (int namelen, const char* name);

    inline static void (*CompileShaderIncludeARB)(GLuint shader, GLsizei count,
        const char* const* path,
        const int* length);

    inline static bool (*IsNamedStringARB)(int namelen, const char* name);

    inline static void (*GetNamedStringARB)(int namelen, const char* name,
        GLsizei bufSize, int* stringlen,
        char* string);

    inline static void (*GetNamedStringivARB)(int namelen, const char* name,
        enum pname, int* params);
};