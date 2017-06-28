#include "stringbuilder.h"
#include <stdlib.h>

struct sbObject
{
    char *buf;
    size_t size;
    size_t len;
};

ormutil_StringBuilder ormutil_StringBuilderCreate()
{
    struct sbObject *obj = malloc(sizeof(struct sbObject));
    obj->size = 16;
    obj->buf = malloc(obj->size);
    obj->len = 0;
    return obj;
}

void ormutil_StringBuilderFree(ormutil_StringBuilder sb)
{
    struct sbObject *obj = sb;
    free(obj->buf);
    free(obj);
}

void ormutil_StringBuilderAppend(ormutil_StringBuilder sb,const char *str)
{
    struct sbObject *obj = sb;
    const char *p = str;
    while(*p != '\0')
    {
        if(obj->len == obj->size - 1)
        {
            obj->size *= 2;
            obj->buf = realloc(obj->buf,obj->size);
        }
        obj->buf[obj->len] = *p;
        p++;obj->len++;
    }
    obj->buf[obj->len] = '\0';
}

char *ormutil_StringBuilderGetString(ormutil_StringBuilder sb)
{
    return ((struct sbObject*)sb)->buf;
}