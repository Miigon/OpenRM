#ifndef OPENRM_PRIV_H
#define OPENRM_PRIV_H
#include "openrm.h"
#include <ruby.h>

extern const char *gameDir;
extern bool gameLoaded;
extern bool rubyInited;
extern ORMEventCallbackFunc eventCb;

void ormpriv_HandleRubyError();
VALUE ormpriv_CallLoadGameProject(VALUE);
void ormpriv_SendEvent(ORMEvent type, void *data);
VALUE ormpriv_ChangeDirectory(VALUE);

#endif
