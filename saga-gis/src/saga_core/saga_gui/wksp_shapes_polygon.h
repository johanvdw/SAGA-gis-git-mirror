
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
//                WKSP_Shapes_Polygon.h                  //
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
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Polygon_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Polygon_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "wksp_shapes.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Shapes_Polygon : public CWKSP_Shapes
{
public:
	CWKSP_Shapes_Polygon(CSG_Shapes *pShapes);

	bool						Get_Outline				(void)	{	return( m_bOutline );	}
	wxPen						Get_Def_Pen				(void)	{	return( m_Pen      );	}
	wxBrush						Get_Def_Brush			(void)	{	return( m_Brush    );	}

	virtual bool				Edit_Do_Mouse_Move_Draw	(bool bMouseDown);


protected:

	bool						m_bCentroid, m_bOutline;

	wxColour					m_Sel_Color_Fill[2];

	wxPen						m_Pen;

	wxBrush						m_Brush;


	virtual void				On_Create_Parameters	(void);
	virtual void				On_DataObject_Changed	(void);
	virtual void				On_Parameters_Changed	(void);

	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags);

	virtual void				Draw_Initialize			(CSG_Map_DC &dc_Map, int Flags);
	virtual void				Draw_Shape				(CSG_Map_DC &dc_Map, CSG_Shape *pShape, int Flags);
	virtual void				Draw_Label				(CSG_Map_DC &dc_Map, CSG_Shape *pShape, const wxString &Label);

	virtual void				Edit_Shape_Draw_Move	(wxDC &dc, const CSG_Rect &rWorld, const wxPoint &Point);
	virtual void				Edit_Shape_Draw			(CSG_Map_DC &dc_Map);
	virtual int					Edit_Shape_HitTest		(const CSG_Point &Point, double max_Dist, int &iPart, int &iPoint);
	virtual void				Edit_Snap_Point_ToLine	(const CSG_Point &Point, CSG_Point &snap_Point, double &snap_Dist, CSG_Shape *pShape);


private:

	void						_Edit_Shape_Draw		(CSG_Map_DC &dc_Map);

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Polygon_H
