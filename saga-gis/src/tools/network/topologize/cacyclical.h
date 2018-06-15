#ifndef HEADER_INCLUDED__cacyclical_H
#define HEADER_INCLUDED__cacyclical_H

#include "TLB_Interface.h"
#include <list>

class CAcyclical : public CSG_Tool
{
public:
    CAcyclical(void);
    virtual ~CAcyclical(void);

protected:
    virtual bool			On_Execute(void);
};

struct node
{
    std::list<int> from;
    std::list<int> to;
    std::list<int> upstream;
    bool finished;
    int order;
    int strahler;
};

#endif
