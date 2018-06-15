#ifndef HEADER_INCLUDED__topologize_H
#define HEADER_INCLUDED__topologize_H
#include "TLB_Interface.h"
#include <list>
///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CTopologize : public CSG_Tool
{
public:
    CTopologize(void);
    virtual ~CTopologize(void);
    static bool Topologize(CSG_Shapes *pInLines, CSG_Shapes *pOutPoints, CSG_Shapes *pOutLines, double tolerance);

protected:
    virtual bool			On_Execute(void);

};

struct Vertex : TSG_Point
{
	friend bool operator < (const Vertex& v1, const Vertex& v2)
	{
		if (v1.x<v2.x)
			return true;
		else if (v1.x == v2.x)
			return v1.y<v2.y;
		else
			return false;
	}
	void operator = (const TSG_Point& v)
	{
		x = v.x;
		y = v.y;
	}

};
#endif
