#ifndef HEADER_INCLUDED__cacyclical_H
#define HEADER_INCLUDED__cacyclical_H

#include "TLB_Interface.h"
#include <list>
#include <vector>

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
    std::vector<int> from;
    std::list<int> to;
    std::list<int> upstream;
    std::vector<bool> finished;
    int order;
    int strahler;
};

#endif
