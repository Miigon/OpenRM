#include "openrmpriv.h"
#include "stringbuilder.h"

void ormpriv_HandleRubyError()
{
    ormutil_StringBuilder sb = ormutil_StringBuilderCreate();
    VALUE exception = rb_errinfo();
    VALUE inspect = rb_funcall(exception,rb_intern("inspect"),0);
    VALUE backtrace = rb_funcall(exception,rb_intern("backtrace"),0);
    SBCAT(sb,StringValuePtr(inspect));
    SBCAT(sb,"\n--- Backtrace ---\n");
    for(int i = 0;i < RARRAY_LEN(backtrace);i++)
    {
        VALUE backtraceItem = rb_ary_entry(backtrace,i);
        SBCAT(sb,StringValuePtr(backtraceItem));
        SBCAT(sb,"\n");
    }
    ormpriv_SendEvent(ORMRubyError,(void*)ormutil_StringBuilderGetString(sb));
    ormutil_StringBuilderFree(sb);
}


void ormpriv_SendEvent(ORMEvent type, void *data)
{
    if(eventCb != NULL) eventCb(type,data);
}

VALUE ormpriv_CallLoadGameProject(VALUE arg)
{
    return rb_funcall(Qnil,rb_intern("__OpenRM_LoadGameProject"),1,rb_str_new2(gameDir));
}
