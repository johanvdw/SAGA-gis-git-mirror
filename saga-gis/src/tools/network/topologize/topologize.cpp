#include "topologize.h"
#include <map>

CTopologize::CTopologize(void)
{
    Set_Name(_TL("Topologize Polylines"));

    Set_Author(_TL("Copyrights (c) 2018 by Johan Van de Wauw"));

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
        NULL, "OUTPUTPOINTS"	, _TL("Output Nodes"),
        _TL(""),
        PARAMETER_OUTPUT_OPTIONAL, SHAPE_TYPE_Point
    );

    Parameters.Add_Value(
        NULL, "TOLERANCE"	, _TL("Tolerant Distance"),
        _TL(""),
        PARAMETER_TYPE_Double
    );

}

CTopologize::~CTopologize(void)
{}

bool CTopologize::On_Execute(void)
{
    CSG_Shapes *pInLines, *pOutLines, *pOutPoints;
	CSG_Shape *pInLine;
	int iPart, MaxNodeID=0;
    double tolerance;
	std::map<Vertex, int> vertices;

    pInLines	= Parameters("INPUTLINES")->asShapes();
    pOutLines = Parameters("OUTPUTLINES")->asShapes();
    pOutPoints = Parameters("OUTPUTPOINTS")->asShapes();
    tolerance = Parameters("TOLERANCE")->asDouble();

    pOutLines->Set_Name(CSG_String::Format(_TL("Topology of %s"),pInLines->Get_Name()));
    
	pOutLines->Add_Field("start_id", SG_DATATYPE_Int);
	pOutLines->Add_Field("end_id", SG_DATATYPE_Int);
	
	if (pOutPoints != 0) {
		pOutPoints->Set_Name(CSG_String::Format(_TL("Vertices of %s"), pInLines->Get_Name()));
		pOutPoints->Add_Field("ID", SG_DATATYPE_Int);
	}



	for (int iLine = 0; iLine < pInLines->Get_Count() && SG_UI_Process_Set_Progress(iLine, pInLines->Get_Count()); iLine++)
	{
        pInLine = pInLines->Get_Shape(iLine);

		for (iPart = 0; iPart < pInLine->Get_Part_Count(); iPart++)
		{
			// Copy the shape
			CSG_Shape *pOut = pOutLines->Add_Shape();

			for (int iPoint = 0; iPoint < pInLine->Get_Point_Count(iPart); iPoint++)
			{
				pOut->Add_Point(pInLine->Get_Point(iPoint, iPart));
			}

			// Starting point of the line
			Vertex start, end;
			start =  pInLine->Get_Point(0, iPart);
			end = pInLine->Get_Point(pInLine->Get_Point_Count(iPart)-1, iPart);
			if (tolerance != 0)
			{
                start.x = floor(start.x / tolerance + 0.5);
                start.y = floor(start.y / tolerance + 0.5);
                end.x = floor(end.x / tolerance + 0.5);
                end.y = floor(end.y / tolerance + 0.5);
			}
			
			auto start_it = vertices.emplace(std::map<Vertex, int>::value_type(start, -1));
			if (start_it.second)
				(*start_it.first).second = MaxNodeID++;
			pOut->Set_Value("start_id", (*start_it.first).second);

			auto it = vertices.emplace(std::map<Vertex, int>::value_type(end, -1));
			if (it.second)
				(*it.first).second = MaxNodeID++;
			pOut->Set_Value("end_id", (*it.first).second);
		}
	}

	if (pOutPoints != 0) {

		for (auto iVertex = vertices.begin(); iVertex != vertices.end(); iVertex++)
		{
			CSG_Shape * pOut = pOutPoints->Add_Shape();
			pOut->Set_Value("ID", iVertex->second);
            pOut->Add_Point(iVertex->first.x * tolerance + tolerance/2, iVertex->first.y * tolerance + tolerance/2);
		}
	}

	return true;
}
