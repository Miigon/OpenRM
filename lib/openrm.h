#ifndef OPENRM_H
#define OPENRM_H

// Currently C language is not supported!
// This header can be included by C++ only

typedef enum {ORMError,ORMRubyError,ORMPrint} ORMEvent;
typedef void(*ORMEventCallbackFunc)(ORMEvent type,void* data);

extern "C"
{

/* MARK: OpenRM Context API */

unsigned int orm_Version();

void orm_Initialize();

void orm_SetEventCallback(ORMEventCallbackFunc cb);

void orm_LoadGameProject();

void orm_SetGameDir(const char *gameDir);
const char *orm_GetGameDir();

/* MARK:Ruby Operations */

bool orm_RbEval(const char *str);

};

#endif