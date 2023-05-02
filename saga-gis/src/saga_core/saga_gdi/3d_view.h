
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_GDI                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    3d_view_panel.h                    //
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
#ifndef HEADER_INCLUDED__3d_view_panel_H
#define HEADER_INCLUDED__3d_view_panel_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/panel.h>
#include <wx/image.h>
#include <wx/menu.h>

#include "saga_gdi.h"

#include "3d_view_tools.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	SG_3DVIEW_PLAY_STOP	= 0,
	SG_3DVIEW_PLAY_RUN_ONCE,
	SG_3DVIEW_PLAY_RUN_LOOP,
	SG_3DVIEW_PLAY_RUN_SAVE
};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SGDI_API_DLL_EXPORT CSG_3DView_Panel : public wxPanel, public CSG_3DView_Canvas
{
public:
	CSG_3DView_Panel(wxWindow *pParent, CSG_Grid *pDrape = NULL);

	CSG_Parameters				m_Parameters;

	virtual bool				Update_Parameters		(bool bSave);
	virtual bool				Update_View				(bool bStatistics = false);

	bool						Parameter_Value_Toggle	(const CSG_String &ID              , bool bUpdate = true);
	bool						Parameter_Value_Add		(const CSG_String &ID, double Value, bool bUpdate = true);

	static CSG_String			Get_Usage				(void);

	bool						Save_asImage			(const CSG_String &FileName);
	bool						Save_toClipboard		(void);

	void						Play_Pos_Add			(void);
	void						Play_Pos_Del			(void);
	void						Play_Pos_Clr			(void);
	CSG_Table &					Play_Pos_Table			(void)	{	return( *m_pPlay );	}

	void						Play_Once				(void);
	void						Play_Loop				(void);
	void						Play_Save				(void);
	void						Play_Stop				(void);
	int							Play_Get_State			(void)	{	return( m_Play_State );	}


protected:

	int							m_Play_State;

	TSG_Point					m_Down_Value;

	CSG_Table					*m_pPlay;

	wxPoint						m_Down_Screen;

	wxImage						m_Image;


	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);
	virtual int					On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual void				Update_Statistics		(void);
	virtual void				Update_Parent			(void);

	static CSG_Table			Get_Shortcuts			(void);
	static CSG_String			Get_Usage				(const CSG_Table &Shortcuts);

	virtual void				On_Size					(wxSizeEvent  &event);
	virtual void				On_Paint				(wxPaintEvent &event);
	virtual void				On_Key_Down				(wxKeyEvent   &event);
	virtual void				On_Mouse_LDown			(wxMouseEvent &event);
	virtual void				On_Mouse_LUp			(wxMouseEvent &event);
	virtual void				On_Mouse_RDown			(wxMouseEvent &event);
	virtual void				On_Mouse_RUp			(wxMouseEvent &event);
	virtual void				On_Mouse_MDown			(wxMouseEvent &event);
	virtual void				On_Mouse_MUp			(wxMouseEvent &event);
	virtual void				On_Mouse_Motion			(wxMouseEvent &event);
	virtual void				On_Mouse_Wheel			(wxMouseEvent &event);

	virtual bool				On_Before_Draw			(void);
	virtual bool				On_Draw					(void)	= 0;


private:

	static int					_On_Parameter_Changed	(CSG_Parameter *pParameter, int Flags);

	bool						_Play					(void);


	DECLARE_EVENT_TABLE()

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	MENU_USER_FIRST =   0,
	MENU_USER_LAST  = 100,
	MENU_FIRST      = MENU_USER_FIRST,
	MENU_PROPERTIES = MENU_USER_LAST + 1,
	MENU_USAGE,
	MENU_CLOSE,
	MENU_BOX,
	MENU_LABELS,
	MENU_NORTH,
	MENU_STEREO,
	MENU_CENTRAL,
	MENU_TO_CLIPBOARD,
	MENU_ROTATE_X_INC,
	MENU_ROTATE_X_DEC,
	MENU_ROTATE_Y_INC,
	MENU_ROTATE_Y_DEC,
	MENU_ROTATE_Z_INC,
	MENU_ROTATE_Z_DEC,
	MENU_SHIFT_X_INC,
	MENU_SHIFT_X_DEC,
	MENU_SHIFT_Y_INC,
	MENU_SHIFT_Y_DEC,
	MENU_SHIFT_Z_INC,
	MENU_SHIFT_Z_DEC,
	MENU_PLAY_POS_ADD,
	MENU_PLAY_POS_DEL,
	MENU_PLAY_POS_CLR,
	MENU_PLAY_RUN_ONCE,
	MENU_PLAY_RUN_LOOP,
	MENU_PLAY_RUN_SAVE,
	MENU_LAST
};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define CHECKBOX_UPDATE(pControl, id) if( event.GetEventObject() == pControl ) {\
	m_pPanel->m_Parameters(id)->Set_Value(pControl->GetValue() == 1 ? true : false);\
	m_pPanel->Update_View(); return;\
}\


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SGDI_API_DLL_EXPORT CSG_3DView_Dialog : public CSGDI_Dialog
{
public:
	CSG_3DView_Dialog(const CSG_String &Caption, int Style = SGDI_DLG_STYLE_START_MAXIMISED);

	virtual void				Update_Controls			(void);


protected:

	wxButton					*m_pCommands;

//	CSGDI_Slider				*m_pRotate;

	CSG_3DView_Panel			*m_pPanel;


	bool						Create					(CSG_3DView_Panel *pPanel);

	virtual void				On_Close				(wxCloseEvent   &event);
	virtual void				On_Update_Control		(wxCommandEvent &event);
	virtual void				On_Update_Choices		(wxCommandEvent &event);
	virtual void				On_Button				(wxCommandEvent &event);

	virtual void				Set_Menu				(wxMenu &Menu)	{}
	virtual void				On_Menu					(wxCommandEvent &event);
	virtual void				On_Menu_UI				(wxUpdateUIEvent &event);


private:

	DECLARE_EVENT_TABLE()

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__3d_view_panel_H
