#include <iostream>
#include "openrm.h"

void callback(ORMEvent type,void* data)
{
    switch(type)
    {
        case ORMError:
            std::cerr << (char *)data << std::endl;
            break;
        case ORMRubyError:
            std::cerr << "Ruby exception: " << (char *)data << std::endl;
            break;
        case ORMPrint:
            std::cout << (char *)data << std::endl;
            break;
        case ORMRGSSNotImp:
            std::cerr << "RGSS feature not implemented: " << (char *)data << std::endl;
        default:
            // do nothing
            break;
    }
}

int main(int argc,char **argv)
{
    std::cout << "OpenRM build" << orm_Version() << std::endl;
    orm_Initialize();
    orm_SetEventCallback(callback);

    orm_SetGameDir("/Users/miigon/Shared Folder/Projects/Project1");
    orm_LoadGameProject();
}
