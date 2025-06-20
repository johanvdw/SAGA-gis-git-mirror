
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
//                  VIEW_ScatterPlot.h                   //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__VIEW_ScatterPlot_H
#define _HEADER_INCLUDED__SAGA_GUI__VIEW_ScatterPlot_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_api/saga_api.h>

#include "view_base.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CVIEW_ScatterPlot : public CVIEW_Base
{
public:
	CVIEW_ScatterPlot(CWKSP_Data_Item *pItem);

	static class wxToolBarBase *	_Create_ToolBar				(void);
	static class wxMenu *			_Create_Menu				(void);

	virtual void					Do_Update					(void);


private:

	double							m_xRange[3], m_yRange[3];

	class CWKSP_Data_Item			*m_pItem;

	CSG_Grid						*m_pGrid, m_Count;

	CSG_Grids						*m_pGrids;

	CSG_Table						*m_pTable;

	CSG_Trend						m_Trend;

	CSG_Parameters					m_Parameters;

	CSG_String						m_sTitle, m_sX, m_sY;


	void							On_Size						(wxSizeEvent    &event);
	void							On_Paint					(wxPaintEvent   &event);

	void							On_Parameters				(wxCommandEvent &event);
	void							On_Update					(wxCommandEvent &event);
	void							On_AsTable					(wxCommandEvent &event);
	void							On_ToClipboard				(wxCommandEvent &event);

	void							_On_Construction			(void);

	bool							_Update_Data				(void);

	bool							_Initialize_Count			(void);
	bool							_Initialize_Grids			(CSG_Grid *pGrid_X, CSG_Grid *pGrid_Y);
	bool							_Initialize_Shapes			(void);
	bool							_Initialize_Table			(void);

	CSG_Colors						_Get_Colors					(int nColors = 255);

	void							_Draw						(wxDC &dc, wxRect r);
	wxRect							_Draw_Get_rDiagram			(wxRect r);
	void							_Draw_Legend				(wxDC &dc, wxRect r);
	void							_Draw_Image					(wxDC &dc, wxRect r);
	void							_Draw_Points				(wxDC &dc, wxRect r);
	void							_Draw_Regression			(wxDC &dc, wxRect r);
	void							_Draw_Frame					(wxDC &dc, wxRect r);


	//-----------------------------------------------------
	DECLARE_CLASS(CVIEW_ScatterPlot)
	DECLARE_EVENT_TABLE()

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__VIEW_ScatterPlot_H
