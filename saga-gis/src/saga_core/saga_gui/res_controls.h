
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
//                    RES_Controls.h                     //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__RES_Controls_H
#define _HEADER_INCLUDED__SAGA_GUI__RES_Controls_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/wx.h>
#include <wx/event.h>

//---------------------------------------------------------
#include <saga_api/saga_api.h>


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum ID_CONTROLS
{
	ID_WND_START				= wxID_HIGHEST,

	ID_WND_MAIN,

	ID_WND_INFO,
	ID_WND_INFO_MESSAGES,
	ID_WND_INFO_DESCRIPTION,
	ID_WND_INFO_ATTRIBUTES,

	ID_WND_ACTIVE,
	ID_WND_ACTIVE_PARAMETERS,
	ID_WND_ACTIVE_DESCRIPTION,
	ID_WND_ACTIVE_HISTORY,
	ID_WND_ACTIVE_ATTRIBUTES,
	ID_WND_ACTIVE_LEGEND,
	ID_WND_ACTIVE_INFO,

	ID_WND_DATA_SOURCE,
	ID_WND_DATA_SOURCE_FILES,
	ID_WND_DATA_SOURCE_DATABASE,
	ID_WND_DATA_SOURCE_WEBSERVICE,

	ID_WND_PARM,

	ID_WND_WKSP,
	ID_WND_WKSP_TOOLS,
	ID_WND_WKSP_DATA,
	ID_WND_WKSP_MAPS,

	ID_WND_PROGRESSBAR,

	//-----------------------------------------------------
	ID_VIEW_TABLE,
	ID_VIEW_TABLE_DIAGRAM,
	ID_VIEW_MAP,
	ID_VIEW_MAP_3D,
	ID_VIEW_HISTOGRAM,
	ID_VIEW_SCATTERPLOT,
	ID_VIEW_LAYOUT,

	//-----------------------------------------------------
	ID_TB_MAIN,

	ID_TB_DATA_TABLE,
	ID_TB_DATA_SHAPES,
	ID_TB_DATA_POINTCLOUD,
	ID_TB_DATA_TIN,
	ID_TB_DATA_GRID,
	ID_TB_DATA_GRIDS,

	ID_TB_VIEW_TABLE,
	ID_TB_VIEW_TABLE_DIAGRAM,
	ID_TB_VIEW_MAP,
	ID_TB_VIEW_MAP_3D,
	ID_TB_VIEW_HISTOGRAM,
	ID_TB_VIEW_SCATTERPLOT,
	ID_TB_VIEW_LAYOUT,

	//-----------------------------------------------------
	ID_LISTBOX_SELECT,
	ID_LISTBOX_ADD,

	ID_COMBOBOX_SELECT,

	//-----------------------------------------------------
	ID_BTN_OK,
	ID_BTN_CANCEL,

	ID_BTN_APPLY,
	ID_BTN_RESTORE,

	ID_BTN_ADD,
	ID_BTN_ADD_ALL,
	ID_BTN_INSERT,
	ID_BTN_DELETE,
	ID_BTN_DELETE_ALL,

	ID_BTN_UP,
	ID_BTN_DOWN,

	ID_BTN_EXECUTE,

	ID_BTN_LOAD,
	ID_BTN_SAVE,

	ID_BTN_DEFAULTS,

	ID_BTN_DESCRIPTION,

	ID_BTN_TABLE_TO_WKSP,
	ID_BTN_TABLE_FROM_WKSP,

	ID_BTN_COLORS,
	ID_BTN_COLORS_MIRROR,
	ID_BTN_COLORS_INVERT,
	ID_BTN_COLORS_GREYSCALE,
	ID_BTN_COLORS_RANDOM,
	ID_BTN_COLORS_PRESET,
	ID_BTN_COLORS_COUNT
};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const wxString	CTRL_Get_Name		(int ID_CTRL);


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__RES_Controls_H
