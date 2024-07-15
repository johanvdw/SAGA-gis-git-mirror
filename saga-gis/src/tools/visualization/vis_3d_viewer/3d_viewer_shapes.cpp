
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       3d_viewer                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 3d_viewer_shapes.cpp                  //
//                                                       //
//                 Copyright (C) 2014 by                 //
//                      Olaf Conrad                      //
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
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "3d_viewer_shapes.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class C3D_Viewer_Shapes_Panel : public CSG_3DView_Panel
{
public:
	C3D_Viewer_Shapes_Panel(wxWindow *pParent, CSG_Shapes *pShapes, int Field_Color);


protected:

	virtual int					On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual void				Update_Statistics		(void);
	virtual void				Update_Parent			(void);

	virtual void				On_Key_Down				(wxKeyEvent   &event);

	virtual bool				On_Draw					(void);

	virtual int					Get_Color				(double Value);


private:

	bool						m_Color_bGrad;

	double						m_Color_Min, m_Color_Scale;

	CSG_Colors					m_Colors;

	CSG_Shapes					*m_pShapes;


	void						Draw_Shape				(CSG_Shape *pShape, int Field_Color);

	//-----------------------------------------------------
	#ifdef __WXMAC__
	virtual void				On_Menu					(wxCommandEvent &event)
	{
		switch( event.GetId() )
		{
		default: CSG_3DView_Dialog::On_Menu(event); return;
		case MENU_PROPERTIES: { CSG_Parameters P(m_pPanel->Get_Parameters()); if( SG_UI_Dlg_Parameters(&P, P.Get_Name()) ) { m_pPanel->Set_Parameters(P); } return; }
		}
	}
	#endif


	//-----------------------------------------------------
	DECLARE_EVENT_TABLE()

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
BEGIN_EVENT_TABLE(C3D_Viewer_Shapes_Panel, CSG_3DView_Panel)
	EVT_KEY_DOWN	(C3D_Viewer_Shapes_Panel::On_Key_Down)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
C3D_Viewer_Shapes_Panel::C3D_Viewer_Shapes_Panel(wxWindow *pParent, CSG_Shapes *pShapes, int cField)
	: CSG_3DView_Panel(pParent)
{
	m_pShapes = pShapes;

	CSG_String Attributes;

	for(int i=0; i<m_pShapes->Get_Field_Count(); i++)
	{
		Attributes += m_pShapes->Get_Field_Name(i); Attributes	+= "|";
	}

	m_Parameters.Add_Choice("GENERAL"    , "COLORS_ATTR" , _TL("Color Attribute"), _TL(""), Attributes, cField);
	m_Parameters.Add_Colors("COLORS_ATTR", "COLORS"      , _TL("Colors"         ), _TL(""));
	m_Parameters.Add_Bool  ("COLORS_ATTR", "COLORS_GRAD" , _TL("Graduated"      ), _TL(""), true);
	m_Parameters.Add_Range ("COLORS_ATTR", "COLORS_RANGE", _TL("Value Range"    ), _TL(""));

	Update_Statistics();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int C3D_Viewer_Shapes_Panel::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	return( CSG_3DView_Panel::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void C3D_Viewer_Shapes_Panel::Update_Statistics(void)
{
	int Field = m_Parameters("COLORS_ATTR")->asInt();

	m_Parameters("COLORS_RANGE")->asRange()->Set_Range(
		m_pShapes->Get_Mean(Field) - 1.5 * m_pShapes->Get_StdDev(Field),
		m_pShapes->Get_Mean(Field) + 1.5 * m_pShapes->Get_StdDev(Field)
	);

	//-----------------------------------------------------
	if( m_pShapes->Get_Selection_Count() > 0 )
	{
		CSG_Simple_Statistics z;

		for(sLong i=0; i<m_pShapes->Get_Selection_Count(); i++)
		{
			CSG_Shape *pShape = m_pShapes->Get_Selection(i);

			for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
			{
				for(int iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
				{
					z += pShape->Get_Z(iPoint, iPart);
				}
			}
		}

		CSG_Rect r   = m_pShapes->Get_Selection_Extent();
		m_Data_Min.x = r.Get_XMin();
		m_Data_Max.x = r.Get_XMax();
		m_Data_Min.y = r.Get_YMin();
		m_Data_Max.y = r.Get_YMax();
		m_Data_Min.z = z.Get_Minimum();
		m_Data_Max.z = z.Get_Maximum();
	}
	else
	{
		CSG_Rect r   = m_pShapes->Get_Extent();
		m_Data_Min.x = r.Get_XMin();
		m_Data_Max.x = r.Get_XMax();
		m_Data_Min.y = r.Get_YMin();
		m_Data_Max.y = r.Get_YMax();
		m_Data_Min.z = m_pShapes->Get_ZMin();
		m_Data_Max.z = m_pShapes->Get_ZMax();
	}

	//-----------------------------------------------------
	Update_View();
}

//---------------------------------------------------------
void C3D_Viewer_Shapes_Panel::Update_Parent(void)
{
	((CSG_3DView_Dialog *)GetParent())->Update_Controls();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void C3D_Viewer_Shapes_Panel::On_Key_Down(wxKeyEvent &event)
{
	CSG_3DView_Panel::On_Key_Down(event);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int C3D_Viewer_Shapes_Panel::Get_Color(double Value)
{
	if( m_Color_Scale <= 0. )
	{
		return( (int)Value );
	}

	double c = m_Color_Scale * (Value - m_Color_Min);

	return( m_Color_bGrad ? m_Colors.Get_Interpolated(c) : m_Colors[(int)c] );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool C3D_Viewer_Shapes_Panel::On_Draw(void)
{
	//-----------------------------------------------------
	int Field     = m_Parameters("COLORS_ATTR")->asInt();

	m_Colors      = *m_Parameters("COLORS")->asColors();
	m_Color_bGrad = m_Parameters("COLORS_GRAD")->asBool();
	m_Color_Min   = m_Parameters("COLORS_RANGE")->asRange()->Get_Min();
	double  Range = m_Parameters("COLORS_RANGE")->asRange()->Get_Max() - m_Color_Min;
	m_Color_Scale = Range > 0. ? m_Colors.Get_Count() / Range : 1.;

	//-----------------------------------------------------
	if( m_pShapes->Get_Selection_Count() > 0 )
	{
		for(sLong iShape=0; iShape<m_pShapes->Get_Selection_Count(); iShape++)
		{
			Draw_Shape(m_pShapes->Get_Selection(iShape), Field);
		}
	}
	else
	{
		for(sLong iShape=0; iShape<m_pShapes->Get_Count(); iShape++)
		{
			Draw_Shape(m_pShapes->Get_Shape(iShape), Field);
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void C3D_Viewer_Shapes_Panel::Draw_Shape(CSG_Shape *pShape, int Field_Color)
{
	int Color = Get_Color(pShape->asDouble(Field_Color));

	switch( pShape->Get_Type() )
	{
	//-----------------------------------------------------
	case SHAPE_TYPE_Point: case SHAPE_TYPE_Points:
		for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
		{
			for(int iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
			{
				CSG_Point_3D a = pShape->Get_Point_Z(iPoint, iPart); m_Projector.Get_Projection(a);

				Draw_Point(a.x, a.y, a.z, Color, 2);
			}
		}
		break;

	//-----------------------------------------------------
	case SHAPE_TYPE_Line:
		for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
		{
			CSG_Point_3D a = pShape->Get_Point_Z(0, iPart);
			m_Projector.Get_Projection(a);

			for(int iPoint=1; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
			{
				CSG_Point_3D b = a; a = pShape->Get_Point_Z(iPoint, iPart); m_Projector.Get_Projection(a);

				Draw_Line(a, b, Color);
			}
		}
		break;

	//-----------------------------------------------------
	case SHAPE_TYPE_Polygon:
		{
			if( 1 )
			{
				for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
				{
					CSG_Point_3D a = pShape->Get_Point_Z(0, iPart, false); m_Projector.Get_Projection(a);

					for(int iPoint=1; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
					{
						CSG_Point_3D b = a; a = pShape->Get_Point_Z(iPoint, iPart); m_Projector.Get_Projection(a);

						Draw_Line(a, b, Color);
						// Draw_Line(a, b, SG_GET_RGB(0, 0, 0));
					}
				}
			}

			if( 0 )
			{
				CSG_Vector LightSource;

				//	if( m_Parameters("SHADING")->asInt() && LightSource.Create(3) )
				if( LightSource.Create(3) )
				{
					//	double decline = m_Parameters("SHADE_DEC")->asDouble() * -M_DEG_TO_RAD;
					//	double azimuth = m_Parameters("SHADE_AZI")->asDouble() *  M_DEG_TO_RAD;
					double decline = 45. * -M_DEG_TO_RAD;
					double azimuth = 90. *  M_DEG_TO_RAD;

					LightSource[0] = sin(decline) * cos(azimuth);
					LightSource[1] = sin(decline) * sin(azimuth);
					LightSource[2] = cos(decline);
				}

				for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
				{
					CSG_Shapes Polygons(SHAPE_TYPE_Polygon, NULL, NULL, SG_VERTEX_TYPE_XYZ); CSG_Shape_Polygon &Polygon = *Polygons.Add_Shape()->asPolygon();

					for(int iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
					{
						CSG_Point_3D a = pShape->Get_Point_Z(iPoint, iPart); m_Projector.Get_Projection(a);

						Polygon.Add_Point(a);
					}

					Draw_Polygon(Polygon, Color, LightSource);
				}
			}
		}
		break;

	//-----------------------------------------------------
	default:
		break;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class C3D_Viewer_Shapes_Dialog : public CSG_3DView_Dialog
{
public:
	C3D_Viewer_Shapes_Dialog(CSG_Shapes *pShapes, int Field_Color)
		: CSG_3DView_Dialog(_TL("3D Shapes Viewer"))
	{
		Create(new C3D_Viewer_Shapes_Panel(this, pShapes, Field_Color));
	}

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
C3D_Viewer_Shapes::C3D_Viewer_Shapes(void)
{
	Set_Name		(_TL("3D Shapes Viewer"));

	Set_Author		("O. Conrad (c) 2014");

	Set_Description	(_TW(
		"3D viewer for 3D Shapes."
	));

	Set_Description(Get_Description() + C3D_Viewer_Shapes_Panel::Get_Usage());

	//-----------------------------------------------------
	Parameters.Add_Shapes("",
		"SHAPES"	, _TL("Shapes"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Table_Field("SHAPES",
		"COLOR"		, _TL("Colour"),
		_TL("")
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool C3D_Viewer_Shapes::On_Execute(void)
{
	CSG_Shapes *pShapes = Parameters("SHAPES")->asShapes();

	if( !pShapes->is_Valid() )
	{
		Error_Set(_TL("invalid input"));

		return( false );
	}

	C3D_Viewer_Shapes_Dialog dlg(pShapes, Parameters("COLOR")->asInt());

	dlg.ShowModal();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
