#ifndef OPENRM_STRINGBUILDER_H
#define OPENRM_STRINGBUILDER_H

typedef void* ormutil_StringBuilder;
ormutil_StringBuilder ormutil_StringBuilderCreate();
void ormutil_StringBuilderFree(ormutil_StringBuilder sb);
void ormutil_StringBuilderAppend(ormutil_StringBuilder sb,const char *str);
char *ormutil_StringBuilderGetString(ormutil_StringBuilder sb);

#define SBCAT ormutil_StringBuilderAppend

#endif
