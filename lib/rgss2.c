#include "rgss2.h"
#include <ruby.h>
#include "openrmpriv.h"

#define NOT_IMPLEMENTED ormpriv_SendEvent(ORMRGSSNotImp,(void*)__FUNCTION__)

/* MARK: class Sprite */

static VALUE RGSS2_Sprite_initialize(int argc,VALUE *argv,VALUE self)
{
    NOT_IMPLEMENTED;
    return Qnil;
}

/* MARK: class Window */

static VALUE RGSS2_Window_initialize(int argc,VALUE *argv,VALUE self)
{
    NOT_IMPLEMENTED;
    return Qnil;
}

/* MARK: class Font */

static VALUE RGSS2_Font_PROPERTY_default_name;

static VALUE RGSS2_Font_initialize(int argc,VALUE *argv,VALUE self)
{
    NOT_IMPLEMENTED;
    return Qnil;
}

static VALUE RGSS2_Font_default_name()
{
    return RGSS2_Font_PROPERTY_default_name;
}

static VALUE RGSS2_Font_default_name_EQ(VALUE value)
{
    RGSS2_Font_PROPERTY_default_name = value;
    return value;
}

/* MARK: Registration */

void loadRGSS2()
{
    VALUE RGSS2_Sprite = rb_define_class("Sprite",rb_cObject);
    rb_define_method(RGSS2_Sprite,"initialize",RGSS2_Sprite_initialize,-1);

    VALUE RGSS2_Window = rb_define_class("Window",rb_cObject);
    rb_define_method(RGSS2_Window,"initialize",RGSS2_Window_initialize,-1);

    VALUE RGSS2_Font = rb_define_class("Font",rb_cObject);
    rb_define_method(RGSS2_Font,"initialize",RGSS2_Font_initialize,-1);
    rb_define_singleton_method(RGSS2_Font,"default_name",RGSS2_Font_default_name,0);
    rb_define_singleton_method(RGSS2_Font,"default_name=",RGSS2_Font_default_name_EQ,1);

    VALUE RGSS2_Graphics = rb_define_module("Graphics");

    VALUE RGSS2_RPG = rb_define_module("RPG");
    

}
