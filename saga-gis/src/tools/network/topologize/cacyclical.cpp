#include "cacyclical.h"
#include <map>

CAcyclical::CAcyclical(void)
{
    Set_Name(_TL("DAG Polylines"));

    Set_Author(_TL("Copyrights (c) 2018 by Johan Van de Wauw"));

    Set_Description(_TL(
        "DAG Polylines.")
    );

    //----------------------------------------------------

    Parameters.Add_Shapes(
        NULL, "INPUTLINES"	, _TL("Input Topology"),
        _TL(""),
        PARAMETER_INPUT,SHAPE_TYPE_Line
    );

}


bool CAcyclical::On_Execute(void)
{
    /*std::map<int, node> nodes;
    auto pInLines	= Parameters("INPUTLINES")->asShapes();

    // loop over segments --> create nodes + link these to segments
    for (int iLine = 0; iLine < pInLines->Get_Count() && SG_UI_Process_Set_Progress(iLine, pInLines->Get_Count()); iLine++)
    {
        CSG_Shape * pLine = pInLines->Get_Shape(iLine);
        nodes[iLine].to.push_back(iLine);
    }
    */
    return true;
}
