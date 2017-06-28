#include "openrm.h"
#include <ruby.h>
#include <sstream>

#define ORMAPI extern "C"
#define ORM_VERSION 0u

static void ormpriv_HandleRubyError();
static inline VALUE ormpriv_CallLoadGameProject(VALUE);
static inline void ormpriv_SendEvent(ORMEvent type, void *data);

namespace ORMContext
{
    const char *gameDir;
    bool gameLoaded;
    bool rubyInited;
    ORMEventCallbackFunc cb;
    VALUE rubyObject;
};

/* MARK: OpenRM Context API */

ORMAPI unsigned int orm_Version()
{
    return ORM_VERSION;
}

ORMAPI void orm_Initialize()
{
    if(ORMContext::rubyInited) ruby_finalize();
    ruby_init();// TODO: use `ruby_setup()` instead
    ruby_init_loadpath();
    rb_set_safe_level(0);
    ORMContext::rubyInited = true;
    ORMContext::gameLoaded = false;
    ORMContext::cb = nullptr;
    ORMContext::gameDir = nullptr;
}

ORMAPI void orm_SetEventCallback(ORMEventCallbackFunc cb)
{
    ORMContext::cb = cb;
}

static inline void ormpriv_SendEvent(ORMEvent type, void *data)
{
    if(ORMContext::cb != nullptr) ORMContext::cb(type,data);
}

static inline VALUE ormpriv_CallLoadGameProject(VALUE)
{
    rb_funcall(Qnil,rb_intern("__OpenRM_LoadGameProject"),1,rb_str_new2(ORMContext::gameDir));
}

ORMAPI void orm_LoadGameProject()
{
    if(orm_GetGameDir() == nullptr)
    {
        ormpriv_SendEvent(ORMError, (void *) "orm_LoadGameProject: GameDir is null! use `orm_SetGameDir` first!");
        return;
    }
    orm_RbEval("require_relative \"../liborm.rb\"");
    int status;
    rb_protect(ormpriv_CallLoadGameProject,Qnil,&status);
    if(status != 0) ormpriv_HandleRubyError();
}

ORMAPI void orm_SetGameDir(const char *gameDir)
{
    ORMContext::gameDir = gameDir;
}

ORMAPI const char *orm_GetGameDir()
{
    return ORMContext::gameDir;
}

/* MARK:Ruby Operations */

static void ormpriv_HandleRubyError()
{
    std::stringstream ss;
    VALUE exception = rb_errinfo();
    static const ID inspect_id = rb_intern("inspect");
    VALUE inspect = rb_funcall(exception,inspect_id,0);
    static const ID backtrace_id = rb_intern("backtrace");
    VALUE backtrace = rb_funcall(exception,backtrace_id,0);
    ss << StringValuePtr(inspect) << std::endl << "--- Backtrace ---" << std::endl;
    for(int i = 0;i < RARRAY_LEN(backtrace);i++)
    {
        VALUE backtraceItem = rb_ary_entry(backtrace,i);
        ss << StringValuePtr(backtraceItem) << std::endl;
    }

    ormpriv_SendEvent(ORMRubyError,(void*)ss.str().c_str());
}

ORMAPI bool orm_RbEval(const char *str)
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