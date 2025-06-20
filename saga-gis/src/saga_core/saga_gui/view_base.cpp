
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
//                    VIEW_Base.cpp                      //
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
#include <wx/window.h>
#include <wx/icon.h>

#include <saga_api/saga_api.h>

#include "res_commands.h"
#include "res_controls.h"
#include "res_images.h"

#include "saga_frame.h"

#include "helper.h"

#include "view_base.h"

#include "wksp_data_item.h"
#include "wksp_map.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CVIEW_Base, MDI_ChildFrame);

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVIEW_Base, MDI_ChildFrame)
	EVT_SYS_COLOUR_CHANGED(CVIEW_Base::On_SysColourChanged)
	EVT_SIZE              (CVIEW_Base::On_Size            )
	EVT_ACTIVATE          (CVIEW_Base::On_Activate        )
END_EVENT_TABLE()

//---------------------------------------------------------
bool CVIEW_Base::m_bUpdates = true;


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CVIEW_Base::CVIEW_Base(class CWKSP_Base_Item *pOwner, int View_ID, wxString Caption, int Icon_ID, bool bShow)
	: MDI_ChildFrame(g_pSAGA_Frame, -1, Caption, MDI_Get_Def_Position(), MDI_Get_Def_Size(), wxDEFAULT_FRAME_STYLE|wxNO_FULL_REPAINT_ON_RESIZE)
{
	SetBackgroundColour(SYS_Get_Color(wxSYS_COLOUR_WINDOW));

	m_pOwner     = pOwner;
	m_View_ID    = View_ID;

	m_Size_Min.x = 0;
	m_Size_Min.y = 0;

#if !defined(MDI_TABBED)
	SetStatusBarPane(-1);
#endif

#if !defined(WITH_WXBMPBUNDLE)
	SetIcon (IMG_Get_Icon (Icon_ID));
#elif !defined(MDI_TABBED)
	SetIcons(IMG_Get_Icons(Icon_ID));
#else // workaround: hopefully works in future too just using wxAuiMDIChildFrame::SetIcons(IMG_Get_Icons(Icon_ID));
	wxAuiNotebook *pNotebook = GetMDIParentFrame()->GetNotebook();
	int Page = pNotebook->FindPage(this);
	if( Page != wxNOT_FOUND )
	{
		int Size(FromDIP(16));
		pNotebook->SetPageBitmap(Page, IMG_Get_Bitmaps(Icon_ID, wxSize(Size, Size)));
	}
#endif

	g_pSAGA_Frame->On_Child_Created(this);

	if( bShow )
	{
		Do_Show();
	}
}

//---------------------------------------------------------
CVIEW_Base::~CVIEW_Base(void)
{
	if( m_pOwner )
	{
		switch( m_pOwner->Get_Type() )
		{
		case WKSP_ITEM_Map:
			((CWKSP_Map       *)m_pOwner)->View_Closes(this);
			break;

		default:
			((CWKSP_Data_Item *)m_pOwner)->View_Closes(this);
			break;
		}
	}

	if( g_pSAGA_Frame ) // might already be NULL on app's closing!
	{
		g_pSAGA_Frame->On_Child_Deleted(this);
		g_pSAGA_Frame->On_Child_Activates(-1);
	}
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::Do_Show(void)
{
	switch( m_pOwner->Get_Type() )
	{
	case WKSP_ITEM_Map:
		((CWKSP_Map       *)m_pOwner)->View_Opened(this);
		break;

	default:
		((CWKSP_Data_Item *)m_pOwner)->View_Opened(this);
		break;
	}

#ifndef MDI_TABBED
	if( !g_pSAGA_Frame->GetActiveChild() && !g_pSAGA_Frame->IsFrozen() )
	{
		Maximize();
	}

	m_bUpdates = false;
	Show();
	m_bUpdates = true;
#endif

	g_pSAGA_Frame->On_Child_Activates(m_View_ID);
}

//---------------------------------------------------------
void CVIEW_Base::Do_Destroy(void)
{
	m_pOwner	= NULL;

	Destroy();
}

//---------------------------------------------------------
void CVIEW_Base::Do_Update(void)
{
	// NOP
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::Set_Size_Min(int x, int y)
{
	m_Size_Min.x	= x;
	m_Size_Min.y	= y;
}

//---------------------------------------------------------
void CVIEW_Base::On_Size(wxSizeEvent &event)
{
	if( event.GetSize().x < m_Size_Min.x || event.GetSize().y < m_Size_Min.y )
	{
		SetSize(wxSize(
			event.GetSize().x < m_Size_Min.x ? m_Size_Min.x : event.GetSize().x,
			event.GetSize().y < m_Size_Min.y ? m_Size_Min.y : event.GetSize().y)
		);
	}
	else
	{
		event.Skip();
	}
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::On_SysColourChanged(wxSysColourChangedEvent &event)
{
	SetBackgroundColour(SYS_Get_Color(wxSYS_COLOUR_WINDOW));

	event.Skip();
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::On_Activate(wxActivateEvent &event)
{
	if( g_pSAGA_Frame && event.GetActive() )
	{
		g_pSAGA_Frame->On_Child_Activates(m_View_ID);
	}
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::On_Quit(wxCommandEvent &WXUNUSED(event))
{
	Close(true);
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::On_Command(wxCommandEvent &event)
{
}

//---------------------------------------------------------
void CVIEW_Base::On_Command_UI(wxUpdateUIEvent &event)
{
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
