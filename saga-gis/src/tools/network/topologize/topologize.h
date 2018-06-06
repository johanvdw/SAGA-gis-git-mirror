#include "TLB_Interface.h"

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
