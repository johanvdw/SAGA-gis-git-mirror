
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
//                  WKSP_PointCloud.h                    //
//                                                       //
//          Copyright (C) 2009 by Olaf Conrad            //
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
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_PointCloud_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_PointCloud_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "wksp_layer.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_PointCloud : public CWKSP_Layer
{
public:
	CWKSP_PointCloud(CSG_PointCloud *pPointCloud);
	virtual ~CWKSP_PointCloud(void);

	virtual TWKSP_Item			Get_Type				(void)	{	return( WKSP_ITEM_PointCloud );	}

	CSG_PointCloud *			Get_PointCloud			(void)	{	return( (CSG_PointCloud *)m_pObject );	}
	class CWKSP_Table *			Get_Table				(void)	{	return( m_pTable );		}

	virtual wxString			Get_Description			(void);

	virtual wxToolBarBase *		Get_ToolBar				(void);
	virtual wxMenu *			Get_Menu				(void);

	virtual bool				On_Command				(int Cmd_ID);
	virtual bool				On_Command_UI			(wxUpdateUIEvent &event);

	virtual wxString			Get_Value				(CSG_Point ptWorld, double Epsilon);

	int							Get_Field_Value			(void) { return( m_Stretch.Value  ); }
	int							Get_Field_Normal		(void) { return( m_Stretch.Normal ); }
	double						Get_Scale_Normal		(void) { return( m_Stretch.Scale  ); }

	bool						asImage					(CSG_Grid *pImage);

	virtual wxMenu *			Edit_Get_Menu			(void);
	virtual TSG_Rect			Edit_Get_Extent			(void);
	virtual bool				Edit_On_Mouse_Up		(const CSG_Point &Point, double ClientToWorld, int Key);
	virtual bool				Edit_Set_Index			(int Index);
	virtual int					Edit_Get_Index			(void);
	virtual bool				Edit_Set_Attributes		(void);


protected:

	virtual void				On_Create_Parameters	(void);
	virtual void				On_DataObject_Changed	(void);
	virtual void				On_Parameters_Changed	(void);
	virtual void				On_Update_Views			(void);

	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags);

	virtual void				On_Draw					(CSG_Map_DC &dc_Map, int Flags);


private:

	int							m_PointSize = 1, m_Aggregation = 0, m_Edit_Index = -1;

	wxColour					m_Color_Pen;

	CSG_Grid					m_Z, m_N;

	class CWKSP_Table			*m_pTable = NULL;

	class CStretch				{ public: int Value = -1, Normal = -1; double Scale = 1.; } m_Stretch;


	void						_Draw_Point				(CSG_Map_DC &dc_Map, int x, int y, double z, int Color);
	void						_Draw_Point				(CSG_Map_DC &dc_Map, int x, int y, double z, int Color, int Radius);
	void						_Draw_Points			(CSG_Map_DC &dc_Map);
	void						_Draw_Thumbnail			(CSG_Map_DC &dc_Map);

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_PointCloud_H
