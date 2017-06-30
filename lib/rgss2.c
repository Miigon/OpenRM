#include "rgss2.h"
#include <ruby.h>
#include "openrmpriv.h"

#define NOT_IMPLEMENTED ormpriv_SendEvent(ORMRGSSNotImp,(void*)__FUNCTION__)

/* MARK: classes and modules */
VALUE RGSS2_Sprite,RGSS2_Window,RGSS2_Font,RGSS2_Table,RGSS2_Graphics,RGSS2_RPG;

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

/* MARK: Table */

struct RGSS2_Table_DATA
{
    uint8_t dimensions;
    unsigned long xsize;
    unsigned long ysize;
    unsigned long zsize;
    int16_t *block;
};

struct RGSS2_Table_DUMPED_METADATA
{
    uint32_t size : 32; // size of each member,it's always `2`(int16_t) in RGSS2
    uint32_t xsize : 32;
    uint32_t ysize : 32;
    uint32_t zsize : 32;
};

union RGSS2_Table_DUMPED_METADATA_CONV
{
    char *binary;
    struct RGSS2_Table_DUMPED_METADATA *metadata;
};

static void RGSS2_Table_FREE(struct RGSS2_Table_DATA *obj)
{
    free(obj->block);
    ruby_xfree(obj);
}

static VALUE RGSS2_Table_ALLOC(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, RGSS2_Table_FREE, ruby_xmalloc(sizeof(struct RGSS2_Table_DATA)));
}

static unsigned long RGSS2_Table_INDEX_FUNC(xsize,ysize,x,y,z)
    unsigned long xsize,ysize,x,y,z;
{
    return x + y * xsize + z * xsize * ysize;
}

static VALUE RGSS2_Table_initialize(int argc,VALUE *argv,VALUE self)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    if(argc > 3 || argc < 1) rb_raise(rb_eArgError,"wrong number of arguments (given %d, expected 1..3)",argc);
    uint8_t dimensions = obj->dimensions = argc;
    obj->xsize = obj->ysize = obj->zsize = 1;
    if(dimensions >= 1)
    {
        obj->xsize = FIX2ULONG(argv[0]);
    }
    if(dimensions >= 2)
    {
        obj->ysize = FIX2ULONG(argv[1]);
    }
    if(dimensions == 3)
    {
        obj->zsize = FIX2ULONG(argv[2]);
    }
    obj->block = calloc(obj->xsize * obj->ysize * obj->zsize,sizeof(int16_t));
    return Qnil;
}

static VALUE RGSS2_Table_INDEX(int argc,VALUE *argv,VALUE self)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    uint8_t dimensions = obj->dimensions;
    if(argc != dimensions) rb_raise(rb_eArgError,"wrong number of arguments (given %d, expected %d)",argc,obj->dimensions);
    unsigned long x,y,z; x = y = z = 0;
    if(dimensions >= 1) x = FIX2ULONG(argv[0]);
    if(dimensions >= 2) y = FIX2ULONG(argv[1]);
    if(dimensions == 3) z = FIX2ULONG(argv[2]);
    if(x < obj->xsize && y < obj->ysize && z < obj->zsize)
        return INT2FIX(obj->block[RGSS2_Table_INDEX_FUNC(obj->xsize,obj->ysize,x,y,z)]);
    else
        return Qnil;
}

static VALUE RGSS2_Table_SETINDEX(int argc,VALUE *argv,VALUE self)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    uint8_t dimensions = obj->dimensions;
    if(argc - 1 != dimensions) rb_raise(rb_eArgError,"wrong number of arguments (given %d, expected %d)",argc,dimensions + 1);
    unsigned long x,y,z; x = y = z = 0;
    if(dimensions >= 1) x = FIX2ULONG(argv[0]);
    if(dimensions >= 2) y = FIX2ULONG(argv[1]);
    if(dimensions == 3) z = FIX2ULONG(argv[2]);
    if(x < obj->xsize && y < obj->ysize && z < obj->zsize)
        obj->block[RGSS2_Table_INDEX_FUNC(obj->xsize,obj->ysize,x,y,z)] = FIX2SHORT(argv[argc - 1]);
    return argv[argc - 1];
}

static VALUE RGSS2_Table_resize(int argc,VALUE *argv,VALUE self)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    uint8_t dimensions = obj->dimensions;
    if(argc != dimensions) rb_raise(rb_eArgError,"wrong number of arguments (given %d, expected %d)",argc,obj->dimensions);
    unsigned long newx,newy,newz; newx = newy = newz = 1;
    if(dimensions >= 1) newx = FIX2ULONG(argv[0]);
    if(dimensions >= 2) newy = FIX2ULONG(argv[1]);
    if(dimensions == 3) newz = FIX2ULONG(argv[2]);
    int16_t *newBlock = malloc(newx * newy * newz * sizeof(int16_t));
    int16_t *block = obj->block;
    for(unsigned long z = 0;z < newz;z++)
    {
        if(obj->zsize <= z)
        {
            memset(newBlock + RGSS2_Table_INDEX_FUNC(newx,newy,z,0,0),'\0',RGSS2_Table_INDEX_FUNC(newx,newy,0,0,newz - z));
            break;
        }
        for(unsigned long y = 0;y < newy;y++)
        {
            if(obj->ysize <= y)
            {
                memset(newBlock + RGSS2_Table_INDEX_FUNC(newx,newy,0,y,z),'\0',RGSS2_Table_INDEX_FUNC(newx,newy,0,newy - y,0));
                break;
            }
            for(unsigned long x = 0;x < newz;x++)
            {
                if(obj->xsize <= x)
                {
                    memset(newBlock + RGSS2_Table_INDEX_FUNC(newx,newy,x,y,z),'\0',RGSS2_Table_INDEX_FUNC(newx - x,newy,0,0,0));
                    break;
                }
                newBlock[RGSS2_Table_INDEX_FUNC(newx,newy,x,y,z)] = block[RGSS2_Table_INDEX_FUNC(obj->xsize,obj->ysize,x,y,z)];
            }
        }
    }
    obj->block = newBlock;
    obj->xsize = newx;
    obj->ysize = newy;
    obj->zsize = newz;
    free(block);
    return self;
}

static VALUE RGSS2_Table_xsize(VALUE self)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    return INT2FIX(obj->xsize);
}

static VALUE RGSS2_Table_ysize(VALUE self)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    return INT2FIX(obj->ysize);
}

static VALUE RGSS2_Table_zsize(VALUE self)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    return INT2FIX(obj->zsize);
}

static VALUE RGSS2_Table_LOAD(VALUE self_class,VALUE arg1)
{
    char *data = StringValuePtr(arg1);
    union RGSS2_Table_DUMPED_METADATA_CONV conv;
    conv.binary = data;
    VALUE newTbl = RGSS2_Table_ALLOC(RGSS2_Table);
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(newTbl,struct RGSS2_Table_DATA, obj);
    obj->xsize = conv.metadata->xsize;
    obj->ysize = conv.metadata->ysize;
    obj->zsize = conv.metadata->zsize;
    obj->block = (int16_t*)(data + sizeof(struct RGSS2_Table_DUMPED_METADATA) + sizeof(int32_t)/* data[16] and data[17] are always '\x58' and '\x02'*/);
    obj->dimensions = 0;
    if(obj->xsize > 1) obj->dimensions++;
    if(obj->ysize > 1) obj->dimensions++;
    if(obj->zsize > 1) obj->dimensions++;
    return newTbl;
}

static VALUE RGSS2_Table_DUMP(VALUE self,VALUE d)
{
    struct RGSS2_Table_DATA* obj;
    Data_Get_Struct(self, struct RGSS2_Table_DATA, obj);
    union RGSS2_Table_DUMPED_METADATA_CONV conv;
    conv.metadata->size = sizeof(int16_t);
    conv.metadata->xsize = obj->xsize;
    conv.metadata->ysize = obj->ysize;
    conv.metadata->zsize = obj->zsize;
    VALUE bin = rb_str_new(conv.binary, sizeof(struct RGSS2_Table_DUMPED_METADATA));
    bin = rb_str_cat(bin,"\x58\x02",2); // TODO:Less rb_str_cat
    return rb_str_cat(bin,obj->block,obj->xsize * obj->ysize * obj->zsize * sizeof(int16_t));
}

/* MARK: Registration */

void loadRGSS2()
{
    /* Defines all the RGSS2 classes and methods.
     * `undoc` means a method or a class exists in original RGSS2,but it's not documented.
     * */

    // Sprite
    RGSS2_Sprite = rb_define_class("Sprite",rb_cObject);
    rb_define_method(RGSS2_Sprite,"initialize",RGSS2_Sprite_initialize,-1);

    // Window
    RGSS2_Window = rb_define_class("Window",rb_cObject);
    rb_define_method(RGSS2_Window,"initialize",RGSS2_Window_initialize,-1);

    // Font
    RGSS2_Font = rb_define_class("Font",rb_cObject);
    rb_define_method(RGSS2_Font,"initialize",RGSS2_Font_initialize,-1);
    rb_define_singleton_method(RGSS2_Font,"default_name",RGSS2_Font_default_name,0);
    rb_define_singleton_method(RGSS2_Font,"default_name=",RGSS2_Font_default_name_EQ,1);

    // Table
    RGSS2_Table = rb_define_class("Table",rb_cObject);
    rb_define_alloc_func(RGSS2_Table,RGSS2_Table_ALLOC);
    rb_define_method(RGSS2_Table,"initialize",RGSS2_Table_initialize,-1);
    rb_define_method(RGSS2_Table,"[]",RGSS2_Table_INDEX,-1);
    rb_define_method(RGSS2_Table,"[]=",RGSS2_Table_SETINDEX,-1);
    rb_define_method(RGSS2_Table,"xsize",RGSS2_Table_xsize,0);
    rb_define_method(RGSS2_Table,"ysize",RGSS2_Table_ysize,0);
    rb_define_method(RGSS2_Table,"zsize",RGSS2_Table_zsize,0);
    rb_define_method(RGSS2_Table,"resize",RGSS2_Table_resize,-1);
    rb_define_singleton_method(RGSS2_Table,"_load",RGSS2_Table_LOAD,1); // undoc
    rb_define_method(RGSS2_Table,"_dump",RGSS2_Table_DUMP,1); // undoc

    // Graphics module
    RGSS2_Graphics = rb_define_module("Graphics");

}
