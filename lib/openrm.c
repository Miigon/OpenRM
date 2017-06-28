#include "openrm.h"
#include <ruby.h>
#include "openrmpriv.h"
#include "rgss2.h"

#define ORM_VERSION 0u

const char *gameDir;
bool gameLoaded;
bool rubyInited;
ORMEventCallbackFunc eventCb;

/* MARK: OpenRM Context API */

unsigned int orm_Version()
{
    return ORM_VERSION;
}

void orm_Initialize()
{
    if(rubyInited) ruby_finalize();
    ruby_init();// TODO: use `ruby_setup()` instead
    ruby_init_loadpath();
    rb_set_safe_level(0);
    rubyInited = true;
    gameLoaded = false;
    eventCb = NULL;
    gameDir = NULL;
}

void orm_SetEventCallback(ORMEventCallbackFunc cb)
{
    eventCb = cb;
}

void orm_LoadGameProject()
{
    if(orm_GetGameDir() == NULL)
    {
        ormpriv_SendEvent(ORMError, (void *) "orm_LoadGameProject: GameDir is null! use `orm_SetGameDir` first!");
        return;
    }
    loadRGSS2(); // TODO: call by liborm.rb
    rb_define_singleton_method(Qnil,"__OpenRM_ChangeDirectory",ormpriv_ChangeDirectory,1);
    orm_RbEval("require_relative \"../liborm.rb\"");
    int status;
    rb_protect(ormpriv_CallLoadGameProject,Qnil,&status);
    if(status != 0) ormpriv_HandleRubyError();
}

void orm_SetGameDir(const char *dir)
{
    gameDir = dir;
}

const char *orm_GetGameDir()
{
    return gameDir;
}

/* MARK:Ruby Operations */

bool orm_RbEval(const char *str)
{
    int status = -1;
    rb_eval_string_protect(str,&status);
    if(status != 0) // Unhandled exception
    {
        ormpriv_HandleRubyError();
        return false;
    }
    return true;
}
