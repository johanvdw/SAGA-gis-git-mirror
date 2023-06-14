
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
//                WKSP_Base_Control.cpp                  //
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
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/window.h>
#include <wx/image.h>
#include <wx/imaglist.h>

#include <saga_api/saga_api.h>

#include "helper.h"

#include "saga_frame.h"

#include "res_commands.h"
#include "res_controls.h"
#include "res_dialogs.h"
#include "res_images.h"

#include "active.h"

#include "wksp.h"

#include "wksp_base_control.h"
#include "wksp_base_manager.h"

#include "wksp_tool_control.h"
#include "wksp_tool_manager.h"
#include "wksp_tool.h"

#include "wksp_data_manager.h"
#include "wksp_data_layers.h"
#include "wksp_table.h"

#include "wksp_map_manager.h"
#include "wksp_map.h"
#include "wksp_map_buttons.h"
#include "wksp_layer.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	IMG_NO_ITEMS	= 0,
	IMG_ROOT
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CWKSP_Base_Control, wxTreeCtrl)

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CWKSP_Base_Control, wxTreeCtrl)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_WND_WKSP_TOOLS, CWKSP_Base_Control::On_Item_RClick    )
	EVT_TREE_DELETE_ITEM     (ID_WND_WKSP_TOOLS, CWKSP_Base_Control::On_Item_Delete    )
	EVT_TREE_SEL_CHANGED     (ID_WND_WKSP_TOOLS, CWKSP_Base_Control::On_Item_SelChanged)
	EVT_TREE_KEY_DOWN        (ID_WND_WKSP_TOOLS, CWKSP_Base_Control::On_Item_KeyDown   )

	EVT_TREE_ITEM_RIGHT_CLICK(ID_WND_WKSP_DATA , CWKSP_Base_Control::On_Item_RClick    )
	EVT_TREE_DELETE_ITEM     (ID_WND_WKSP_DATA , CWKSP_Base_Control::On_Item_Delete    )
	EVT_TREE_SEL_CHANGED     (ID_WND_WKSP_DATA , CWKSP_Base_Control::On_Item_SelChanged)
	EVT_TREE_KEY_DOWN        (ID_WND_WKSP_DATA , CWKSP_Base_Control::On_Item_KeyDown   )

	EVT_TREE_ITEM_RIGHT_CLICK(ID_WND_WKSP_MAPS , CWKSP_Base_Control::On_Item_RClick    )
	EVT_TREE_DELETE_ITEM     (ID_WND_WKSP_MAPS , CWKSP_Base_Control::On_Item_Delete    )
	EVT_TREE_SEL_CHANGED     (ID_WND_WKSP_MAPS , CWKSP_Base_Control::On_Item_SelChanged)
	EVT_TREE_KEY_DOWN        (ID_WND_WKSP_MAPS , CWKSP_Base_Control::On_Item_KeyDown   )

	EVT_LEFT_DOWN            (CWKSP_Base_Control::On_Item_LClick )
	EVT_LEFT_DCLICK          (CWKSP_Base_Control::On_Item_LDClick)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Base_Control::CWKSP_Base_Control(wxWindow *pParent, wxWindowID id)
	: wxTreeCtrl(pParent, id, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS)
{
	m_pManager	= NULL;

	AssignImageList(new wxImageList(IMG_SIZE_TREECTRL, IMG_SIZE_TREECTRL, true, 0));

	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_NOITEMS);
}

//---------------------------------------------------------
CWKSP_Base_Control::~CWKSP_Base_Control(void)
{}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Base_Control::_Set_Manager(CWKSP_Base_Manager *pManager)
{
	if( m_pManager == NULL )
	{
		m_pManager	= pManager;

		AddRoot   (m_pManager->Get_Name(), IMG_ROOT, IMG_ROOT, m_pManager);
		AppendItem(m_pManager->GetId(), _TL("<no items>"), IMG_NO_ITEMS, IMG_NO_ITEMS, NULL);
		Expand    (m_pManager->GetId());

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Base_Control::On_Command(wxCommandEvent &event)
{
	switch( event.GetId() )
	{
	case ID_CMD_WKSP_ITEM_CLOSE:
		_Del_Active(false);
		break;

	case ID_CMD_WKSP_ITEM_SHOW:
		_Show_Active();
		break;

	case ID_CMD_WKSP_ITEM_SEARCH:
		_Search_Item();
		break;

	//-----------------------------------------------------
	default:
		if( !m_pManager->On_Command(event.GetId()) )
		{
			CWKSP_Base_Item	*pItem	= Get_Item_Selected();

			if( pItem )
			{
				pItem->On_Command(event.GetId());
			}
		}
	}
}

//---------------------------------------------------------
void CWKSP_Base_Control::On_Command_UI(wxUpdateUIEvent &event)
{
	if( !m_pManager->On_Command_UI(event) )
	{
		CWKSP_Base_Item	*pItem	= Get_Item_Selected();

		if( pItem )
		{
			pItem->On_Command_UI(event);
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Base_Control::_Add_Item(CWKSP_Base_Item *pItem, int Image, int selImage, bool bSort)
{
	if( pItem != NULL && pItem->Get_Manager() == m_pManager )
	{
		if( m_pManager->Get_Count() == 1 )
		{
			DeleteChildren(m_pManager->GetId());
		}

		AppendItem(m_pManager->GetId(), pItem->Get_Name(), Image, selImage, pItem);

		if( bSort )
		{
			SortChildren(m_pManager->GetId());
		}

		Expand(m_pManager->GetId());

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Base_Control::Del_Item(CWKSP_Base_Item *pItem, bool bSilent)
{
	return( _Del_Item(pItem, bSilent) );
}

bool CWKSP_Base_Control::_Del_Item(CWKSP_Base_Item *pItem, bool bSilent)
{
	//-----------------------------------------------------
	if( pItem == NULL || pItem->GetId().IsOk() == false || pItem->Get_Control() != this )
	{
		return( false );
	}

	if( pItem->Get_Type() == WKSP_ITEM_Tool && ((CWKSP_Tool *)pItem)->Get_Tool()->Get_Type() != TOOL_TYPE_Chain )
	{	// tool libraries can be unloaded, tools not! (except of tool chain tools)
		return( false );
	}

	if( pItem->Get_Type() == WKSP_ITEM_Table &&	((CWKSP_Table *)pItem)->Get_Object()->Get_ObjectType() != SG_DATAOBJECT_TYPE_Table )
	{
		return( false );
	}

	if( !bSilent && !_Del_Item_Confirm(pItem) )
	{
		return( false );
	}

	//-----------------------------------------------------
	if( m_pManager == g_pActive->Get_Active() && Get_Selection_Count() > 1 )
	{
		g_pActive->Set_Active(NULL);
	}

	//-----------------------------------------------------
	if( pItem == m_pManager )
	{
		if( m_pManager->Get_Count() > 0 )
		{
			Freeze();

			if( m_pManager == g_pData || m_pManager == g_pMaps )
			{
				if( g_pData_Buttons ) { g_pData_Buttons->Freeze(); }
				if( g_pMap_Buttons  ) { g_pMap_Buttons ->Freeze(); }

				g_pSAGA_Frame->Close_Children();
			}

			//---------------------------------------------
			DeleteChildren(m_pManager->GetId());
			AppendItem    (m_pManager->GetId(), _TL("<no items>"), 0, 0, NULL);
			Expand        (m_pManager->GetId());

			if( g_pTools == m_pManager )
			{
				g_pTools->Update();
			}

			if( g_pData == m_pManager )
			{
				g_pData->MultiSelect_Check();
			}

			if( g_pMaps == m_pManager )
			{
				g_pMaps->Reset_Numbering();
			}

			//---------------------------------------------
			Thaw();

			if( m_pManager == g_pData || m_pManager == g_pMaps )
			{
				if( g_pData_Buttons ) { g_pData_Buttons->Thaw(); g_pData_Buttons->Update_Buttons(); }
				if( g_pMap_Buttons  ) { g_pMap_Buttons ->Thaw(); g_pMap_Buttons ->Update_Buttons(); }
			}
		}
	}

	//-----------------------------------------------------
	else
	{
		CWKSP_Base_Manager	*pItem_Manager	= pItem->Get_Manager();

		Freeze();

		Delete(pItem->GetId());

		if( pItem_Manager != NULL && pItem_Manager->Get_Count() == 0 )
		{
			Thaw();

			if( m_pManager->Get_Type() == WKSP_ITEM_Data_Manager )
			{
				if( g_pData_Buttons ) { g_pData_Buttons->Update_Buttons(); }
			}

			if( m_pManager->Get_Type() == WKSP_ITEM_Map_Manager )
			{
				if( g_pMap_Buttons  ) { g_pMap_Buttons ->Update_Buttons(); }
			}

			return( _Del_Item(pItem_Manager, true) );
		}

		if( m_pManager->Get_Type() == WKSP_ITEM_Tool_Manager )
		{
			g_pTools->Update();
		}

		if( pItem_Manager != NULL && pItem_Manager->Get_Type() == WKSP_ITEM_Map )
		{
			((CWKSP_Map *)pItem_Manager)->View_Refresh(false);
		}

		Thaw();

		Refresh();

		if( m_pManager->Get_Type() == WKSP_ITEM_Data_Manager && g_pData_Buttons )
		{
			g_pData_Buttons->Update_Buttons();
		}

		if( m_pManager->Get_Type() == WKSP_ITEM_Map_Manager  && g_pMap_Buttons  )
		{
			g_pMap_Buttons->Update_Buttons();
		}
	}

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CWKSP_Base_Control::_Del_Item_Confirm(CWKSP_Base_Item *pItem)
{
	if( DLG_Message_Confirm(ID_DLG_DELETE) )
	{
		if( m_pManager->Get_Type() == WKSP_ITEM_Data_Manager )
		{
			return( g_pData->Save_Modified(pItem) );
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Base_Control::Get_Selection_Count(void)
{
	return( GetSelection().IsOk() ? 1 : 0 );
}

//---------------------------------------------------------
CWKSP_Base_Item * CWKSP_Base_Control::Get_Item_Selected(bool bUpdate)
{
	wxTreeItemId	ID	= GetSelection();

	return( ID.IsOk() ? (CWKSP_Base_Item *)GetItemData(ID) : NULL );
}

//---------------------------------------------------------
bool CWKSP_Base_Control::Set_Item_Selected(CWKSP_Base_Item *pItem, bool bKeepMultipleSelection)
{
	if( pItem && pItem->GetId().IsOk() && pItem->Get_Control() == this )
	{
		SelectItem(pItem->GetId());

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxMenu * CWKSP_Base_Control::Get_Menu(void)
{
	CWKSP_Base_Item	*pItem	= Get_Item_Selected();

	return( pItem ? pItem->Get_Menu() : NULL );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Base_Control::_Del_Active(bool bSilent)
{
	if( PROCESS_is_Executing() && m_pManager->Get_Type() != WKSP_ITEM_Map_Manager )	// never allow tool or data deletion while a tool is running!
	{
		return( false );
	}

	wxTreeItemId	ID	= GetSelection();

	if( ID.IsOk() )
	{
		_Del_Item((CWKSP_Base_Item *)GetItemData(ID), bSilent);
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Base_Control::_Show_Active(void)
{
	wxArrayTreeItemIds	IDs;

	if( (GetWindowStyle() & wxTR_MULTIPLE) != 0 && GetSelections(IDs) > 0 && ((CWKSP_Base_Item *)GetItemData(IDs[0]))->Get_Control() == this )
	{
		int	iMap = 0;

		for(size_t iItem=0; iItem<IDs.GetCount(); iItem++)
		{
			CWKSP_Base_Item *pItem = IDs[iItem].IsOk() ? (CWKSP_Base_Item *)GetItemData(IDs[iItem]) : NULL;

			if( pItem )
			{
				switch( pItem->Get_Type() )
				{
				case WKSP_ITEM_Table     :
					((CWKSP_Table *)pItem)->Set_View(true);
					break;

				case WKSP_ITEM_Grid      :
				case WKSP_ITEM_Grids     :
				case WKSP_ITEM_Shapes    :
				case WKSP_ITEM_TIN       :
				case WKSP_ITEM_PointCloud:
					iMap	= 1;
					break;

				default:
					break;
				}
			}
		}

		if( iMap && (iMap = DLG_Maps_Add()) >= 0 )
		{
			for(size_t iItem=0; iItem<IDs.GetCount(); iItem++)
			{
				CWKSP_Base_Item *pItem = IDs[iItem].IsOk() ? (CWKSP_Base_Item *)GetItemData(IDs[iItem]) : NULL;

				if( pItem )
				{
					switch( pItem->Get_Type() )
					{
					case WKSP_ITEM_Grid      :
					case WKSP_ITEM_Grids     :
					case WKSP_ITEM_Shapes    :
					case WKSP_ITEM_TIN       :
					case WKSP_ITEM_PointCloud:
						g_pMaps->Add((CWKSP_Layer *)pItem, g_pMaps->Get_Map(iMap));
						break;

					default:
						break;
					}
				}
			}

			g_pMaps->Get_Map(iMap)->View_Show(true);

			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Base_Control::_Search_Compare(wxString A, wxString B, bool bCase)
{
	return( bCase ? B.Find(A) != wxNOT_FOUND : B.MakeUpper().Find(A.MakeUpper().c_str()) != wxNOT_FOUND );
}

//---------------------------------------------------------
bool CWKSP_Base_Control::_Search_Get_List(CSG_Table *pList, CWKSP_Base_Item *pItem, const wxString &String, bool bName, bool bDesc, bool bCase, TWKSP_Item Type)
{
	if( pItem == NULL )
	{
		return( false );
	}

	if(	Type == WKSP_ITEM_Undefined || Type == pItem->Get_Type() )
	{
		if( (bName && _Search_Compare(String, pItem->Get_Name       (), bCase))
		||  (bDesc && _Search_Compare(String, pItem->Get_Description(), bCase)) )
		{
			CSG_Table_Record	*pRecord	= pList->Add_Record();

			pRecord->Set_Value(0, pItem->Get_Name().wx_str());
			pRecord->Set_Value(1, pItem->Get_Type_Name(pItem->Get_Type()).wx_str());
			pRecord->Set_Value(2, CSG_String::Format("%p", pItem));
		}
	}

	if( pItem->is_Manager() )
	{
		for(int i=0; i<((CWKSP_Base_Manager *)pItem)->Get_Count(); i++)
		{
			_Search_Get_List(pList, ((CWKSP_Base_Manager *)pItem)->Get_Item(i), String, bName, bDesc, bCase, Type);
		}
	}

	return( true );
}

//---------------------------------------------------------
CWKSP_Base_Item * CWKSP_Base_Control::Search_Item(const wxString &Caption, TWKSP_Item Type)
{
	static CSG_Parameters	Search(Caption);

	if( Search.Get_Count() == 0 )
	{
		Search.Add_String("", "STRING", _TL("Search for..." ), _TL(""), ""   );
		Search.Add_Bool  ("", "NAME"  , _TL("Name"          ), _TL(""), true );
		Search.Add_Bool  ("", "DESC"  , _TL("Description"   ), _TL(""), false);
		Search.Add_Bool  ("", "CASE"  , _TL("Case Sensitive"), _TL(""), false);
	}

	if( !DLG_Parameters(&Search) )
	{
		return( NULL );
	}

	//-----------------------------------------------------
	CSG_Table	List;

	List.Add_Field(_TL("NAME"), SG_DATATYPE_String);
	List.Add_Field(_TL("TYPE"), SG_DATATYPE_String);
	List.Add_Field(_TL("ADDR"), SG_DATATYPE_String);

	_Search_Get_List(&List, m_pManager, Search("STRING")->asString(), Search("NAME")->asBool(), Search("DESC")->asBool(), Search("CASE")->asBool(), Type);

	if( List.Get_Count() <= 0 )
	{
		wxMessageBox(_TL("Search text not found"), _TL("Search for..."), wxOK|wxICON_EXCLAMATION);

		return( NULL );
	}

	//-----------------------------------------------------
	List.Set_Index(1, TABLE_INDEX_Ascending, 0, TABLE_INDEX_Ascending);

	wxArrayString	Items;

	for(int i=0; i<List.Get_Count(); i++)
	{
		if( Type == WKSP_ITEM_Undefined )
		{
			Items.Add(wxString::Format("[%s] %s", List[i].asString(1), List[i].asString(0)));
		}
		else
		{
			Items.Add(List[i].asString(0));
		}
	}

	//-----------------------------------------------------
	wxSingleChoiceDialog	dlg(MDI_Get_Top_Window(),
		wxString::Format("%s: '%s'", _TL("Search Result"), Search("STRING")->asString()),
		Caption, Items
	);

	void	*pItem;

	if( dlg.ShowModal() != wxID_OK || SG_SSCANF(List[dlg.GetSelection()].asString(2), SG_T("%p"), &pItem) != 1 )
	{
		return( NULL );
	}

	return( (CWKSP_Base_Item *)pItem );
}

//---------------------------------------------------------
bool CWKSP_Base_Control::_Search_Item(void)
{
	CWKSP_Base_Item	*pItem	= Search_Item(_TL("Locate..."));

	if( pItem && pItem->GetId().IsOk() )
	{
		EnsureVisible	(pItem->GetId());
		SelectItem		(pItem->GetId());
		ScrollTo		(pItem->GetId());

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Base_Control::On_Item_LClick(wxMouseEvent &event)
{
	g_pActive->Set_Active(Get_Item_Selected());

	event.Skip();
}

//---------------------------------------------------------
void CWKSP_Base_Control::On_Item_LDClick(wxMouseEvent &event)
{
	CWKSP_Base_Item	*pItem;

	if( (pItem = Get_Item_Selected()) != NULL )
	{
		pItem->On_Command(ID_CMD_WKSP_ITEM_RETURN);
	}
}

//---------------------------------------------------------
void CWKSP_Base_Control::On_Item_RClick(wxTreeEvent &event)
{
	if( m_pManager->Get_Type() != WKSP_ITEM_Data_Manager )
	{
		SelectItem(event.GetItem());
	}

	if( Get_Selection_Count() <= 1 )
	{
		g_pActive->Set_Active(Get_Item_Selected());
	}

	wxMenu	*pMenu	= Get_Menu();

	if( pMenu )
	{
		PopupMenu(pMenu);

		delete(pMenu);
	}

	event.Skip();
}

//---------------------------------------------------------
void CWKSP_Base_Control::On_Item_KeyDown(wxTreeEvent &event)
{
	switch( event.GetKeyCode() )
	{
	case WXK_DELETE:
		_Del_Active(false);
		break;

	default: {
		CWKSP_Base_Item	*pItem = Get_Item_Selected();

		if( pItem )
		{
			switch( event.GetKeyCode() )
			{
			default:
				break;

			case WXK_RETURN:
				pItem->On_Command(ID_CMD_WKSP_ITEM_RETURN);
				break;
			}
		}
		break; }
	}
}

//---------------------------------------------------------
void CWKSP_Base_Control::On_Item_SelChanged(wxTreeEvent &event)
{
	if( g_pActive )
	{
		CWKSP_Base_Item	*pItem	= Get_Item_Selected(true);

		if( pItem )
		{
			g_pActive->Set_Active(pItem);
		}
	}

	event.Skip();
}

//---------------------------------------------------------
void CWKSP_Base_Control::On_Item_Delete(wxTreeEvent &event)
{
	event.Skip();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
