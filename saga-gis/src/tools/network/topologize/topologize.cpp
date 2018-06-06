#include "topologize.h"
#include "stp.h"

CTopologize::CTopologize(void)
{
    Set_Name(_TL("Topologize Polylines"));

    Set_Author(_TL("Copyrights (c) 2007 by Sun Zhuo"));

    Set_Description(_TL(
        "Topologize Polylines.")
    );

    //----------------------------------------------------

    Parameters.Add_Shapes(
        NULL, "INPUTLINES"	, _TL("Input Lines"),
        _TL(""),
        PARAMETER_INPUT,SHAPE_TYPE_Line
    );
    Parameters.Add_Shapes(
        NULL, "OUTPUTLINES"	, _TL("Output Lines"),
        _TL(""),
        PARAMETER_OUTPUT, SHAPE_TYPE_Line
    );
    Parameters.Add_Shapes(
        NULL, "OUTPUTPOINTS"	, _TL("Output Points"),
        _TL(""),
        PARAMETER_OUTPUT, SHAPE_TYPE_Point
    );

    Parameters.Add_Value(
        NULL, "TOLERANCE"	, _TL("Tolerant Distance"),
        _TL(""),
        PARAMETER_TYPE_Double
    );

    Parameters.Add_Value(
        NULL	, "SPHERICAL"	, _TL("Earth CoorSys"),
        _TL("Is it the Earth Coordinate System or not?"),
        PARAMETER_TYPE_Bool		, 0
    );
}

CTopologize::~CTopologize(void)
{}

bool CTopologize::On_Execute(void)
{
    CNetwork Network;
    CSG_Shapes *pIntLines, *pOutLines, *pOutPoints;
    double tolerance;

    pIntLines	= Parameters("INPUTLINES")->asShapes();
    pOutLines = Parameters("OUTPUTLINES")->asShapes();
    pOutPoints = Parameters("OUTPUTPOINTS")->asShapes();
    tolerance = Parameters("TOLERANCE")->asDouble();
    Network.IsSpherical = Parameters("SPHERICAL")->asBool();

    pOutLines->Set_Name(CSG_String::Format(_TL("Topology of %s"),pIntLines->Get_Name()));
    pOutPoints->Set_Name(CSG_String::Format(_TL("Vertices of %s"),pIntLines->Get_Name()));

    return Network.Topologize(pIntLines, pOutPoints, pOutLines, tolerance);
}
