
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
//                   INFO_Messages.h                     //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__INFO_Messages_H
#define _HEADER_INCLUDED__SAGA_GUI__INFO_Messages_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/textctrl.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CINFO_Messages : public wxTextCtrl
{
public:
	CINFO_Messages(wxWindow *pParent);

	void						Add_Line			(void);
	void						Add_Time			(bool bNewLine = true);
	void						Add_String			(const wxString &Message, bool bNewLine = true, bool bTime = false, TSG_UI_MSG_STYLE Style = SG_UI_MSG_STYLE_NORMAL);


private:

	int							m_MaxLength;


	void						On_SysColourChanged	(wxSysColourChangedEvent &event);

	void						On_Context_Menu		(wxMouseEvent   &event);

	void						On_Copy				(wxCommandEvent &event);
	void						On_Clear			(wxCommandEvent &event);
	void						On_SelectAll		(wxCommandEvent &event);

	void						_Add_Text			(const wxString &Text);

	void						_Set_Style			(TSG_UI_MSG_STYLE Style);


	//-----------------------------------------------------
	wxDECLARE_EVENT_TABLE();
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__INFO_Messages_H
