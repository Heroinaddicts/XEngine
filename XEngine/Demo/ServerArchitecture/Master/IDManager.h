#ifndef __idmanager_h__
#define __idmanager_h__

#include "header.h"

class idmanager {
public:
    virtual ~idmanager() {}

    static idmanager & getInstance();
    virtual int allocaID(const std::string & name, const std::string & ip, const int32 port);

private:
    idmanager() {}

private:
    std::map<std::string, int> _id_map;
};

#endif //__idmanager_h__
