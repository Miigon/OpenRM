#ifndef OPENRM_H
#define OPENRM_H

#include <stdbool.h>

typedef enum {ORMError,ORMRubyError,ORMRGSSNotImp,ORMPrint} ORMEvent;
typedef void(*ORMEventCallbackFunc)(ORMEvent type,void* data);

#ifdef __cplusplus
extern "C"
{
#endif

/* MARK: OpenRM Context API */

unsigned int orm_Version();

void orm_Initialize();

void orm_SetEventCallback(ORMEventCallbackFunc cb);

void orm_LoadGameProject();

void orm_SetGameDir(const char *gameDir);
const char *orm_GetGameDir();

/* MARK:Ruby Operations */

bool orm_RbEval(const char *str);

#ifdef __cplusplus
};
#endif

#endif
