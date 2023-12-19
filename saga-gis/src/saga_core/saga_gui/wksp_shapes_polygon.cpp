
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                WKSP_Shapes_Polygon.cpp                //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation, either version 2 of the     //
// License, or (at your option) any later version.       //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not, see   //
// <http://www.gnu.org/licenses/>.                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_gdi/sgdi_helper.h>

#include "helper.h"

#include "wksp_layer_classify.h"

#include "wksp_shapes_polygon.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Shapes_Polygon::CWKSP_Shapes_Polygon(CSG_Shapes *pShapes)
	: CWKSP_Shapes(pShapes)
{
	On_Create_Parameters();

	DataObject_Changed();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::On_Create_Parameters(void)
{
	CWKSP_Shapes::On_Create_Parameters();

	//-----------------------------------------------------
	// Display...

	BrushList_Add("NODE_DISPLAY",
		"DISPLAY_BRUSH"		, _TL("Fill Style"),
		_TL("")
	);

	m_Parameters.Add_Bool("NODE_DISPLAY",
		"OUTLINE"			, _TL("Outline"),
		_TL(""),
		true
	);

	m_Parameters.Add_Color("OUTLINE",
		"OUTLINE_COLOR"		, _TL("Color"),
		_TL(""),
		SG_GET_RGB(0, 0, 0)
	);

	m_Parameters.Add_Int("OUTLINE",
		"OUTLINE_SIZE"		, _TL("Size"),
		_TL(""),
		1, 1, true
	);

	m_Parameters.Add_Choice("NODE_DISPLAY",
		"DISPLAY_POINTS"	, _TL("Show Vertices"),
		_TL(""),
		CSG_String::Format("%s|%s|%s",
			_TL("no"),
			_TL("yes"),
			_TL("with label")
		), 0
	);

	m_Parameters.Add_Bool("NODE_DISPLAY",
		"DISPLAY_CENTROID"	, _TL("Show Centroid"),
		_TL(""),
		false
	);

	m_Parameters.Add_Color("NODE_SELECTION",
		"SEL_COLOR_FILL_0"	, _TL("Fill Color 1"),
		_TL(""),
		SG_GET_RGB(255, 225, 0)
	);

	m_Parameters.Add_Color("NODE_SELECTION",
		"SEL_COLOR_FILL_1"	, _TL("Fill Color 2"),
		_TL("if more than one feature is in selection, this colour is used for the non active features"),
		SG_GET_RGB(255, 255, 127)
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::On_DataObject_Changed(void)
{
	CWKSP_Shapes::On_DataObject_Changed();
}

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::On_Parameters_Changed(void)
{
	CWKSP_Shapes::On_Parameters_Changed();

	//-----------------------------------------------------
	m_bOutline  = m_Parameters("OUTLINE")->asBool();
	m_Pen       = wxPen(!m_bOutline ? m_pClassify->Get_Unique_Color() : Get_Color_asWX(m_Parameters("OUTLINE_COLOR")->asColor()), !m_bOutline ? 1 : m_Parameters("OUTLINE_SIZE")->asInt(), wxPENSTYLE_SOLID);
	m_Brush     = wxBrush(m_pClassify->Get_Unique_Color(), BrushList_Get_Style("DISPLAY_BRUSH"));

	m_bVertices = m_Parameters("DISPLAY_POINTS"  )->asInt ();
	m_bCentroid = m_Parameters("DISPLAY_CENTROID")->asBool();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Shapes_Polygon::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags)
{
	return( CWKSP_Shapes::On_Parameter_Changed(pParameters, pParameter, Flags) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::Draw_Initialize(CWKSP_Map_DC &dc_Map, int Flags)
{
	dc_Map.dc.SetBrush(m_Brush);
	dc_Map.dc.SetPen(m_Pen);

	m_Sel_Color_Fill[0]	= Get_Color_asWX(m_Parameters("SEL_COLOR_FILL_0")->asInt());
	m_Sel_Color_Fill[1]	= Get_Color_asWX(m_Parameters("SEL_COLOR_FILL_1")->asInt());
}

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::Draw_Shape(CWKSP_Map_DC &dc_Map, CSG_Shape *pShape, int Selection)
{
	//-----------------------------------------------------
	if( Selection )
	{
		dc_Map.dc.SetBrush(wxBrush(Selection == 1 ? m_Sel_Color_Fill[0] : m_Sel_Color_Fill[1], m_Brush.GetStyle()));
		dc_Map.dc.SetPen  (wxPen(m_Sel_Color, 0, wxPENSTYLE_SOLID));

		dc_Map.Draw_Polygon((CSG_Shape_Polygon *)pShape);

		dc_Map.dc.SetBrush(m_Brush);
		dc_Map.dc.SetPen  (m_Pen  );

		return;
	}

	//-----------------------------------------------------
	if( !m_Brush.IsTransparent() || m_bOutline )
	{
		int	Color;

		if( Get_Class_Color(pShape, Color) || m_bNoData )
		{
			m_Brush.SetColour(SG_GET_R(Color), SG_GET_G(Color), SG_GET_B(Color));
			dc_Map.dc.SetBrush(m_Brush);

			if( !m_bOutline )
			{
				m_Pen.SetColour(SG_GET_R(Color), SG_GET_G(Color), SG_GET_B(Color));
				dc_Map.dc.SetPen(m_Pen);
			}

			dc_Map.Draw_Polygon((CSG_Shape_Polygon *)pShape);
		}
	}

	//-----------------------------------------------------
	if( m_bCentroid )
	{
		TSG_Point	Point	= ((CSG_Shape_Polygon *)pShape)->Get_Centroid();

		dc_Map.dc.DrawCircle((int)dc_Map.xWorld2DC(Point.x), (int)dc_Map.yWorld2DC(Point.y), 2);
	}
}

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::Draw_Label(CWKSP_Map_DC &dc_Map, CSG_Shape *pShape, const wxString &Label)
{
	if( 0 )
	{
		TSG_Point_Int	p	= dc_Map.World2DC(((CSG_Shape_Polygon *)pShape)->Get_Centroid());

		Draw_Text(dc_Map.dc, TEXTALIGN_CENTER, p.x, p.y, Label, m_Label_Eff, m_Label_Eff_Color, m_Label_Eff_Size);
	}
	else
	{
		for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
		{
			if( !((CSG_Shape_Polygon *)pShape)->is_Lake(iPart) )
			{
				TSG_Point_Int	p	= dc_Map.World2DC(((CSG_Shape_Polygon *)pShape)->Get_Centroid());

				Draw_Text(dc_Map.dc, TEXTALIGN_CENTER, p.x, p.y, Label, m_Label_Eff, m_Label_Eff_Color, m_Label_Eff_Size);
			}
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Shapes_Polygon::Edit_Do_Mouse_Move_Draw(bool bMouseDown)
{
	return( CWKSP_Shapes::Edit_Do_Mouse_Move_Draw(bMouseDown) || (m_Edit_pShape && m_Edit_iPart >= 0 && (m_Edit_iPoint < 0 || bMouseDown)) );
}

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::Edit_Shape_Draw_Move(wxDC &dc, const CSG_Rect &rWorld, const wxPoint &Point)
{
	if( m_Edit_pShape && m_Edit_iPart >= 0 )
	{
		int	nPoints	= m_Edit_pShape->Get_Point_Count(m_Edit_iPart);

		if( m_Edit_iPoint < 0 )
		{
			if( nPoints > 0 )
			{
				CWKSP_Shapes::Edit_Shape_Draw_Move(dc, rWorld, Point,
					m_Edit_pShape->Get_Point(0, m_Edit_iPart)
				);

				if( nPoints > 1 )
				{
					CWKSP_Shapes::Edit_Shape_Draw_Move(dc, rWorld, Point,
						m_Edit_pShape->Get_Point(nPoints - 1, m_Edit_iPart)
					);
				}
			}
		}
		else
		{
			if( nPoints > 1 )
			{
				CWKSP_Shapes::Edit_Shape_Draw_Move(dc, rWorld, Point, m_Edit_pShape->Get_Point(
					m_Edit_iPoint > 0 ? m_Edit_iPoint - 1 : nPoints - 1, m_Edit_iPart, true)
				);

				if( nPoints > 2 )
				{
					CWKSP_Shapes::Edit_Shape_Draw_Move(dc, rWorld, Point, m_Edit_pShape->Get_Point(
						m_Edit_iPoint >= nPoints - 1 ? 0 : m_Edit_iPoint + 1, m_Edit_iPart, true)
					);
				}
			}

			CWKSP_Shapes::Edit_Shape_Draw_Move(dc, rWorld, Point);
		}
	}
}

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::_Edit_Shape_Draw(CWKSP_Map_DC &dc_Map)
{
	for(int iPart=0; m_Edit_pShape && iPart<m_Edit_pShape->Get_Part_Count(); iPart++)
	{
		if( m_Edit_pShape->Get_Point_Count(iPart) > 1 )
		{
			TSG_Point_Int A = dc_Map.World2DC(m_Edit_pShape->Get_Point(m_Edit_pShape->Get_Point_Count(iPart) - 1, iPart));

			for(int iPoint=0; iPoint<m_Edit_pShape->Get_Point_Count(iPart); iPoint++)
			{
				TSG_Point_Int B = A; A = dc_Map.World2DC(m_Edit_pShape->Get_Point(iPoint, iPart));

				dc_Map.dc.DrawLine(A.x, A.y, B.x, B.y);
			}
		}
		else if( m_Edit_pShape->Get_Point_Count(iPart) == 2 )
		{
			TSG_Point_Int	A	= dc_Map.World2DC(m_Edit_pShape->Get_Point(0, iPart));
			TSG_Point_Int	B	= dc_Map.World2DC(m_Edit_pShape->Get_Point(1, iPart));

			dc_Map.dc.DrawLine(A.x, A.y, B.x, B.y);
		}
	}
}

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::Edit_Shape_Draw(CWKSP_Map_DC &dc_Map)
{
	if( m_Edit_pShape )
	{
		if( m_Edit_bGleam )
		{
			dc_Map.dc.SetPen(wxPen(m_Edit_Color, 3));
			dc_Map.dc.SetLogicalFunction(wxINVERT);

			_Edit_Shape_Draw(dc_Map);

			dc_Map.dc.SetLogicalFunction(wxCOPY);
		}

		dc_Map.dc.SetPen(wxPen(m_Edit_Color));

		_Edit_Shape_Draw(dc_Map);

		CWKSP_Shapes::Edit_Shape_Draw(dc_Map);
	}
}

//---------------------------------------------------------
int CWKSP_Shapes_Polygon::Edit_Shape_HitTest(CSG_Point pos_Point, double max_Dist, int &pos_iPart, int &pos_iPoint)
{
	int			Result, iPart, iPoint;
	double		d;
	TSG_Point	A, B, Point, hit_Point;

	Result	= CWKSP_Shapes::Edit_Shape_HitTest(pos_Point, max_Dist, pos_iPart, pos_iPoint);

	if( Result == 0 && m_Edit_pShape )
	{
		for(iPart=0; iPart<m_Edit_pShape->Get_Part_Count(); iPart++)
		{
			B	= m_Edit_pShape->Get_Point(m_Edit_pShape->Get_Point_Count(iPart) - 1, iPart);

			for(iPoint=0; iPoint<m_Edit_pShape->Get_Point_Count(iPart); iPoint++)
			{
				A	= m_Edit_pShape->Get_Point(iPoint, iPart);
				d	= SG_Get_Nearest_Point_On_Line(pos_Point, A, B, Point, true);
				B	= A;

				if( d >= 0.0 && (0.0 > max_Dist || d < max_Dist) )
				{
					Result		= 2;
					max_Dist	= d;
					pos_iPoint	= iPoint;
					pos_iPart	= iPart;
					hit_Point	= Point;
				}
			}
		}

		if( Result )
		{
			pos_Point	= hit_Point;
		}
	}

	return( Result );
}

//---------------------------------------------------------
void CWKSP_Shapes_Polygon::Edit_Snap_Point_ToLine(CSG_Point pos_Point, CSG_Point &snap_Point, double &snap_Dist, CSG_Shape *pShape)
{
	int			iPart, iPoint;
	double		d;
	TSG_Point	A, B, Point;

	for(iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
	{
		B	= pShape->Get_Point(pShape->Get_Point_Count(iPart) - 1, iPart);

		for(iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
		{
			A	= pShape->Get_Point(iPoint, iPart);
			d	= SG_Get_Nearest_Point_On_Line(pos_Point, A, B, Point, true);
			B	= A;

			if( d >= 0.0 && d < snap_Dist )
			{
				snap_Dist	= d;
				snap_Point	= Point;
			}
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
