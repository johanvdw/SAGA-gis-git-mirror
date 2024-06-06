
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
//                VIEW_Table_Control.cpp                 //
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

#include <saga_api/saga_api.h>

#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"

#include "wksp_data_manager.h"

#include "view_table_data.h"
#include "view_table_control.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CVIEW_Table_Control, wxGrid)

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVIEW_Table_Control, wxGrid)
#if (wxMAJOR_VERSION == 3 && wxMINOR_VERSION <= 1)
	EVT_GRID_RANGE_SELECT     (CVIEW_Table_Control::On_Selected     ) // RANGE_SELECT was split in RANGE_SELECTING and SELECTED in 3.2
#else
	EVT_GRID_RANGE_SELECTING  (CVIEW_Table_Control::On_Selecting    )
	EVT_GRID_RANGE_SELECTED   (CVIEW_Table_Control::On_Selected     )
#endif

	EVT_GRID_CELL_CHANGED     (CVIEW_Table_Control::On_Changed      )

	EVT_GRID_CELL_LEFT_CLICK  (CVIEW_Table_Control::On_LClick       )
	EVT_GRID_CELL_LEFT_DCLICK (CVIEW_Table_Control::On_LDClick      )
	EVT_GRID_LABEL_LEFT_CLICK (CVIEW_Table_Control::On_LClick_Label )
	EVT_GRID_LABEL_LEFT_DCLICK(CVIEW_Table_Control::On_LDClick_Label)
	EVT_GRID_CELL_RIGHT_CLICK (CVIEW_Table_Control::On_RClick       )
	EVT_GRID_LABEL_RIGHT_CLICK(CVIEW_Table_Control::On_RClick_Label )

	//-----------------------------------------------------
	EVT_MENU     (ID_CMD_TABLE_SELECTION_ONLY , CVIEW_Table_Control::On_Sel_Only       )
	EVT_UPDATE_UI(ID_CMD_TABLE_SELECTION_ONLY , CVIEW_Table_Control::On_Sel_Only_UI    )
	EVT_MENU     (ID_CMD_TABLE_SELECTION_CLEAR, CVIEW_Table_Control::On_Sel_Clear      )
	EVT_UPDATE_UI(ID_CMD_TABLE_SELECTION_CLEAR, CVIEW_Table_Control::On_Sel_Clear_UI   )

	EVT_MENU     (ID_CMD_TABLE_AUTOSIZE_COLS  , CVIEW_Table_Control::On_Autosize_Cols  )
	EVT_MENU     (ID_CMD_TABLE_AUTOSIZE_ROWS  , CVIEW_Table_Control::On_Autosize_Rows  )

	EVT_MENU     (ID_CMD_TABLE_FIELD_OPEN_APP , CVIEW_Table_Control::On_Cell_Open      )
	EVT_MENU     (ID_CMD_TABLE_FIELD_OPEN_DATA, CVIEW_Table_Control::On_Cell_Open      )

	EVT_MENU     (ID_CMD_TABLE_TO_CLIPBOARD   , CVIEW_Table_Control::On_ToClipboard    )

	EVT_MENU     (ID_CMD_TABLE_FIELD_ADD      , CVIEW_Table_Control::On_Field_Add      )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_ADD      , CVIEW_Table_Control::On_Field_Add_UI   )
	EVT_MENU     (ID_CMD_TABLE_FIELD_MOVE     , CVIEW_Table_Control::On_Field_Move     )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_MOVE     , CVIEW_Table_Control::On_Field_Move_UI  )
	EVT_MENU     (ID_CMD_TABLE_FIELD_DEL      , CVIEW_Table_Control::On_Field_Del      )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_DEL      , CVIEW_Table_Control::On_Field_Del_UI   )
	EVT_MENU     (ID_CMD_TABLE_FIELD_HIDE     , CVIEW_Table_Control::On_Field_Hide     )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_HIDE     , CVIEW_Table_Control::On_Field_Hide_UI  )
	EVT_MENU     (ID_CMD_TABLE_FIELD_RENAME   , CVIEW_Table_Control::On_Field_Rename   )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_RENAME   , CVIEW_Table_Control::On_Field_Rename_UI)
	EVT_MENU     (ID_CMD_TABLE_FIELD_TYPE     , CVIEW_Table_Control::On_Field_Type     )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_TYPE     , CVIEW_Table_Control::On_Field_Type_UI  )
	EVT_MENU     (ID_CMD_TABLE_FIELD_SORT     , CVIEW_Table_Control::On_Field_Sort     )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_SORT     , CVIEW_Table_Control::On_Field_Sort_UI  )
	EVT_MENU     (ID_CMD_TABLE_FIELD_CALC     , CVIEW_Table_Control::On_Field_Calc     )
	EVT_UPDATE_UI(ID_CMD_TABLE_FIELD_CALC     , CVIEW_Table_Control::On_Field_Calc_UI  )

	EVT_MENU     (ID_CMD_TABLE_RECORD_ADD     , CVIEW_Table_Control::On_Record_Add     )
	EVT_UPDATE_UI(ID_CMD_TABLE_RECORD_ADD     , CVIEW_Table_Control::On_Record_Add_UI  )
	EVT_MENU     (ID_CMD_TABLE_RECORD_INS     , CVIEW_Table_Control::On_Record_Ins     )
	EVT_UPDATE_UI(ID_CMD_TABLE_RECORD_INS     , CVIEW_Table_Control::On_Record_Ins_UI  )
	EVT_MENU     (ID_CMD_TABLE_RECORD_DEL     , CVIEW_Table_Control::On_Record_Del     )
	EVT_UPDATE_UI(ID_CMD_TABLE_RECORD_DEL     , CVIEW_Table_Control::On_Record_Del_UI  )
	EVT_MENU     (ID_CMD_TABLE_RECORD_DEL_ALL , CVIEW_Table_Control::On_Record_Clr     )
	EVT_UPDATE_UI(ID_CMD_TABLE_RECORD_DEL_ALL , CVIEW_Table_Control::On_Record_Clr_UI  )
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CVIEW_Table_Control::CVIEW_Table_Control(wxWindow *pParent, CSG_Table *pTable, int Constraint)
	: wxGrid(pParent, -1, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
	m_pData  = new CVIEW_Table_Data(m_pTable = pTable);

	m_Cursor = -1;

	EnableDragGridSize(false);
	DisableDragRowSize();
	SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
	SetCellHighlightColour(SYS_Get_Color(wxSYS_COLOUR_HIGHLIGHT));

	{
		wxGridCellEditor   *pEditor   = GetDefaultEditorForType  (wxGRID_VALUE_FLOAT);
		pEditor  ->SetParameters("-1,-1,g"); // Use the shorter of e or f (g)
		pEditor  ->DecRef();
	}

	{
		wxGridCellRenderer *pRenderer = GetDefaultRendererForType(wxGRID_VALUE_FLOAT);
		pRenderer->SetParameters("-1,-1,g"); // Use the shorter of e or f (g)
		pRenderer->DecRef();
	}

#if !(wxMAJOR_VERSION == 3 && wxMINOR_VERSION <= 1)
	{
		wxGridCellRenderer *pRenderer = GetDefaultRendererForType(wxGRID_VALUE_DATE );
		pRenderer->SetParameters("%Y-%m-%d");
		pRenderer->DecRef();
	}
#endif

	SetTable(m_pData, true, wxGrid::wxGridSelectRows);

	Update_Table();
}

//---------------------------------------------------------
CVIEW_Table_Control::~CVIEW_Table_Control(void)
{}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_Table_Control::Load(const wxString &File)
{
	CSG_Table	Table(&File);

	if( Table.Get_Count() > 0
	&&  Table.Get_Field_Count() == m_pTable->Get_Field_Count()
	&&  m_pTable->Assign_Values(&Table) && Update_Table() )
	{
		PROCESS_Set_Okay();
		return( true );
	}

	PROCESS_Set_Okay();
	return( false );
}

//---------------------------------------------------------
bool CVIEW_Table_Control::Save(const wxString &File, int Format)
{
	if( m_pTable->Save(&File) )
	{
		PROCESS_Set_Okay();
		return( true );
	}

	PROCESS_Set_Okay();
	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_Table_Control::Update_Float_Format(void)
{
	CWKSP_Base_Item *pItem = (CWKSP_Base_Item *)g_pData->Get(m_pTable);

	if( pItem && pItem->Get_Parameter("TABLE_FLT_STYLE") && pItem->Get_Parameter("TABLE_FLT_DECIMALS") )
	{
		wxGridCellFloatRenderer *pRenderer = (wxGridCellFloatRenderer *)GetDefaultRendererForType(wxGRID_VALUE_FLOAT);
		wxGridCellFloatEditor   *pEditor   = (wxGridCellFloatEditor   *)GetDefaultEditorForType  (wxGRID_VALUE_FLOAT);

		switch( pItem->Get_Parameter("TABLE_FLT_STYLE")->asInt() )
		{
		default:	// system default
			pRenderer->SetParameters(wxString::Format("-1,-1,f"));
			pEditor  ->SetParameters(wxString::Format("-1,-1,f"));
			break;

		case  1:	// compact (use the shorter of e or f (g))
			pRenderer->SetParameters(wxString::Format("-1,-1,g"));
			pEditor  ->SetParameters(wxString::Format("-1,-1,g"));
			break;

		case  2:	// fix number of decimals
			pRenderer->SetParameters(wxString::Format("-1,%d,f", pItem->Get_Parameter("TABLE_FLT_DECIMALS")->asInt()));
			pEditor  ->SetParameters(wxString::Format("-1,%d,f", pItem->Get_Parameter("TABLE_FLT_DECIMALS")->asInt()));
			break;
		}

		pRenderer->DecRef();
		pEditor  ->DecRef();
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_Table_Control::Update_Table(void)
{
	return( GetBatchCount() == 0 && _Update_Records() );
}

//---------------------------------------------------------
bool CVIEW_Table_Control::Update_Selection(void)
{
	return( _Update_Selection() );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_Table_Control::_Update_Records(bool bViews)
{
	Update_Float_Format();

	//-----------------------------------------------------
	int	dCols = m_pData->GetNumberCols() - GetNumberCols();

	if( dCols > 0 )
	{
		AppendCols(dCols);
	}
	else if( dCols < 0 )
	{
		DeleteCols(0, -dCols);
	}

	//-----------------------------------------------------
	int	dRows = m_pData->GetNumberRows() - GetNumberRows();

	if( dRows > 0 )
	{
		AppendRows(dRows);
	}
	else if( dRows < 0 )
	{
		DeleteRows(0, -dRows);
	}

	//-----------------------------------------------------
	for(int iField=0; iField<m_pTable->Get_Field_Count(); iField++)
	{
		if( m_pTable->Get_Field_Type(iField) == SG_DATATYPE_Color )
		{
			BeginBatch();

			for(int i=0; i<m_pData->GetNumberRows(); i++)
			{
				wxColour Colour(Get_Color_asWX(m_pData->Get_Record(i)->asInt(iField)));

				SetCellBackgroundColour(i, iField, Colour);
				SetCellTextColour      (i, iField, Colour);
			}

			EndBatch();
		}
	}

	//-----------------------------------------------------
	_Update_Selection(bViews);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_Table_Control::_Update_Selection(bool bViews)
{
	if( GetBatchCount() > 0 )
	{
		return( false );
	}

	//-----------------------------------------------------
	BeginBatch();

	if( m_pData->m_bSelection )
	{
		int	dRows = m_pData->GetNumberRows() - GetNumberRows();

		if( dRows > 0 )
		{
			AppendRows(dRows);
		}
		else if( dRows < 0 )
		{
			DeleteRows(0, -dRows);
		}

		SelectAll();
	}
	else
	{
		ClearSelection();

		if( m_pTable->Get_Selection_Count() > 0 )
		{
			for(int i=0; i<m_pData->GetRowsCount(); i++)
			{
				if( m_pData->Get_Record(i)->is_Selected() )
				{
					SelectRow(i, true);
				}
			}
		}
	}

	if( bViews )
	{
		g_pData->Update_Views(m_pTable);
	}

	EndBatch();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Selecting(wxGridRangeSelectEvent &event)
{
	if( !m_pData->m_bSelection && event.Selecting() )
	{
		if( m_pTable->Get_Selection_Count() > 0 )
		{
			m_pTable->Select();	// clear current selection

			ClearSelection();
		}
	}
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Selected(wxGridRangeSelectEvent &event)
{
	if( GetBatchCount() < 1 && !m_pData->m_bSelection && event.Selecting() )
	{
		int	iFirst	= event.GetBottomRow() <= event.GetTopRow() ? event.GetBottomRow() : event.GetTopRow();
		int	iLast	= event.GetBottomRow() >  event.GetTopRow() ? event.GetBottomRow() : event.GetTopRow();

		if( iFirst <= iLast )
		{
			m_pTable->Select();

			for(int i=iFirst; i<=iLast; i++)
			{
				m_pTable->Select(m_pData->Get_Record(i), true);
			}

			_Update_Selection(true);
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Changed(wxGridEvent &event)
{
	m_pData->On_Changed(event.GetRow(), event.GetCol());
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_LClick(wxGridEvent &event)
{
	SetGridCursor(event.GetRow(), event.GetCol());

	//-----------------------------------------------------
	if( event.AltDown    () && m_pTable->Get_Field_Type(event.GetCol()) == SG_DATATYPE_String )
	{
		Open_Application(GetCellValue(event.GetRow(), event.GetCol()));
	}

	//-----------------------------------------------------
	if( event.ControlDown() && m_pTable->Get_Field_Type(event.GetCol()) == SG_DATATYPE_String )
	{
		g_pData->Open   (GetCellValue(event.GetRow(), event.GetCol()));
	}
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_LDClick(wxGridEvent &event)
{
	if( m_pData->Get_Field_Type(event.GetCol()) == SG_DATATYPE_Color )
	{
		CSG_Table_Record *pRecord = m_pData->Get_Record(event.GetRow());

		if( pRecord )
		{
			long Value = pRecord->asInt(event.GetCol());

			if( DLG_Color_From_Text(Value) )
			{
				pRecord->Set_Value(event.GetCol(), Value);

				wxColour Colour(Get_Color_asWX(Value));

				SetCellBackgroundColour(event.GetRow(), event.GetCol(), Colour);
				SetCellTextColour      (event.GetRow(), event.GetCol(), Colour);

				ForceRefresh();
			}
		}
	}
	else
	{
		event.Skip();
	}
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_LClick_Label(wxGridEvent &event)
{
	if( !m_pData->m_bSelection )
	{
		if( event.GetRow() >= 0 )	// select records
		{
			if( event.ControlDown() )
			{
				m_pTable->Select(m_pData->Get_Record(event.GetRow()), true);
			}
			else if( !event.ShiftDown() )
			{
				m_pTable->Select();	// clear current selection

				m_pTable->Select(m_pData->Get_Record(event.GetRow()), false);
			}
			else if( m_Cursor >= 0 && m_Cursor < GetNumberRows() )
			{
				int	iFirst	= event.GetRow() <= m_Cursor ? event.GetRow() : m_Cursor + 1;
				int	iLast	= event.GetRow() >  m_Cursor ? event.GetRow() : m_Cursor - 1;

				for(int iRecord=iFirst; iRecord<=iLast; iRecord++)
				{
					m_pTable->Select(m_pData->Get_Record(iRecord), true);
				}
			}

			_Update_Selection(true);
		}
		else if( event.GetCol() < 0 )
		{
			m_pTable->Select(); // clear current selection

			_Update_Selection(true);
		}
	}

	m_Cursor = event.GetRow();
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_LDClick_Label(wxGridEvent &event)
{
	if( m_pData->Sort(event.GetCol(), -1) )
	{
		m_Cursor	= -1;

		_Update_Records();
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_RClick(wxGridEvent &event)
{
	SetGridCursor(event.GetRow(), event.GetCol());

	//-----------------------------------------------------
	switch( m_pTable->Get_Field_Type(event.GetCol()) )
	{
	default:
		{
			// nop
		}
		break;

	case SG_DATATYPE_String:
		{
			wxMenu	Menu;

			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_OPEN_APP);

			wxString	Value	= GetCellValue(event.GetRow(), event.GetCol());

			if( m_pData->is_DataSource(Value) )
			{
				CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_OPEN_DATA);
			}

			PopupMenu(&Menu, event.GetPosition());
		}
		break;

	case SG_DATATYPE_Color:
		{
			CSG_Table_Record *pRecord = m_pData->Get_Record(event.GetRow()); long lValue;

			if( pRecord && DLG_Color(lValue = pRecord->asInt(event.GetCol())) )
			{
				pRecord->Set_Value(event.GetCol(), lValue);

				wxColour Colour(Get_Color_asWX(pRecord->asInt(event.GetCol())));

				SetCellBackgroundColour(event.GetRow(), event.GetCol(), Colour);
				SetCellTextColour      (event.GetRow(), event.GetCol(), Colour);

				ForceRefresh();
			}
		}
		break;
	}
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_RClick_Label(wxGridEvent &event)
{
	if( event.GetCol() != -1 )
	{
		wxMenu	Menu(_TL("Columns"));

		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_ADD    );
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_MOVE   );
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_DEL    );
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_RENAME );
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_TYPE   );
		Menu.AppendSeparator();
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_AUTOSIZE_COLS);
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_HIDE   );
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_SORT   );
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_FIELD_CALC   );
		Menu.AppendSeparator();
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_TO_CLIPBOARD );

		PopupMenu(&Menu, event.GetPosition().x, event.GetPosition().y - GetColLabelSize());
	}

	//-----------------------------------------------------
	else if( event.GetRow() != -1 )
	{
		wxMenu	Menu(_TL("Rows"));

		if( m_pData->m_bSelection )
		{
			CMD_Menu_Add_Item(&Menu,  true, ID_CMD_TABLE_SELECTION_ONLY );
			Menu.AppendSeparator();
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_TO_CLIPBOARD   );
		}
		else
		{
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_RECORD_ADD     );
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_RECORD_INS     );
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_RECORD_DEL     );
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_RECORD_DEL_ALL );
		//	Menu.AppendSeparator();
		//	CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_AUTOSIZE_ROWS  );
			Menu.AppendSeparator();
			CMD_Menu_Add_Item(&Menu,  true, ID_CMD_TABLE_SELECTION_ONLY );
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_SELECTION_CLEAR);
			Menu.AppendSeparator();
			CMD_Menu_Add_Item(&Menu, false, ID_CMD_TABLE_TO_CLIPBOARD   );
		}

		PopupMenu(&Menu, event.GetPosition().x - GetRowLabelSize(), event.GetPosition().y);
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Sel_Only(wxCommandEvent  &event)
{
	if( m_pData->m_bSelection == true )
	{
		m_pData->m_bSelection	= false;

		_Update_Records();
	}
	else if( m_pTable->Get_Selection_Count() > 0 )
	{
		m_pData->m_bSelection	= true;

		_Update_Records();
	}
}

void CVIEW_Table_Control::On_Sel_Only_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pData->m_bSelection || m_pTable->Get_Selection_Count() > 0);

	event.Check(m_pData->m_bSelection);
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Sel_Clear(wxCommandEvent  &event)
{
	if( m_pTable->Get_Selection_Count() > 0 )
	{
		m_pTable->Select();	// clear current selection

		_Update_Selection(true);
	}
}

void CVIEW_Table_Control::On_Sel_Clear_UI(wxUpdateUIEvent &event)
{
	event.Enable(!m_pData->m_bSelection && m_pTable->Get_Selection_Count() > 0);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Autosize_Cols(wxCommandEvent &event)
{
	AutoSizeColumns(false);
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Autosize_Rows(wxCommandEvent &event)
{
	AutoSizeRows(false);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Cell_Open(wxCommandEvent &event)
{
	wxString	Value	= GetCellValue(GetGridCursorRow(), GetGridCursorCol());

	if( event.GetId() == ID_CMD_TABLE_FIELD_OPEN_APP )
	{
		if( !Open_Application(Value) )
		{
			DLG_Message_Show_Error(_TL("failed"), CMD_Get_Name(ID_CMD_TABLE_FIELD_OPEN_APP));
		}
	}

	if( event.GetId() == ID_CMD_TABLE_FIELD_OPEN_DATA )
	{
		if( !m_pData->is_DataSource(Value) || !g_pData->Open(Value) )
		{
			DLG_Message_Show_Error(_TL("failed"), CMD_Get_Name(ID_CMD_TABLE_FIELD_OPEN_DATA));
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_ToClipboard(wxCommandEvent &event)
{
	m_pData->To_Clipboard();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Add(wxCommandEvent &event)
{
	CSG_String	Fields;

	for(int i=0; i<m_pTable->Get_Field_Count(); i++)
	{
		Fields	+= m_pTable->Get_Field_Name(i) + CSG_String('|');
	}

	//-----------------------------------------------------
	CSG_Parameters	P(_TL("Add Field"));

	P.Add_String(
		"", "NAME"	, _TL("Name"),
		_TL(""),
		_TL("Field")
	);

	P.Add_Choice(
		"", "TYPE"	, _TL("Field Type"),
		_TL(""),
		CSG_String::Format("%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s",
			SG_Data_Type_Get_Name(SG_DATATYPE_String).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Date  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Color ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Byte  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Char  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Word  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Short ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_DWord ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Int   ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_ULong ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Long  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Float ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Double).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Binary).c_str()
		), 0
	);

	P.Add_Choice(
		"", "POSITION"	, _TL("Insert at Position"),
		_TL(""),
		Fields, m_pTable->Get_Field_Count() - 1
	);

	P.Add_Choice(
		"", "AFTER"		, _TL("Insert..."),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("before"),
			_TL("after")
		), 1
	);

	//-----------------------------------------------------
	if( DLG_Parameters(&P) )
	{
		TSG_Data_Type Type;

		switch( P("TYPE")->asInt() )
		{
		default: Type = SG_DATATYPE_String; break;
		case  1: Type = SG_DATATYPE_Date  ; break;
		case  2: Type = SG_DATATYPE_Color ; break;
		case  3: Type = SG_DATATYPE_Byte  ; break;
		case  4: Type = SG_DATATYPE_Char  ; break;
		case  5: Type = SG_DATATYPE_Word  ; break;
		case  6: Type = SG_DATATYPE_Short ; break;
		case  7: Type = SG_DATATYPE_DWord ; break;
		case  8: Type = SG_DATATYPE_Int   ; break;
		case  9: Type = SG_DATATYPE_ULong ; break;
		case 10: Type = SG_DATATYPE_Long  ; break;
		case 11: Type = SG_DATATYPE_Float ; break;
		case 12: Type = SG_DATATYPE_Double; break;
		case 13: Type = SG_DATATYPE_Binary; break;
		}

		int Position = P("POSITION")->asInt() + P("AFTER")->asInt();

		m_pTable->Add_Field(P("NAME")->asString(), Type, Position);

		g_pData->Update(m_pTable, NULL);
	}
}

void CVIEW_Table_Control::On_Field_Add_UI(wxUpdateUIEvent &event)
{
	event.Enable(true);
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Move(wxCommandEvent &event)
{
	CSG_String	Fields; int Offset = m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_PointCloud ? 3 : 0;

	for(int i=Offset; i<m_pTable->Get_Field_Count(); i++)
	{
		Fields	+= m_pTable->Get_Field_Name(i) + CSG_String('|');
	}

	//-----------------------------------------------------
	CSG_Parameters	P(_TL("Move Field"));

	P.Add_Choice(
		"", "FIELD"		, _TL("Field"),
		_TL(""),
		Fields
	);

	P.Add_Choice(
		"", "POSITION"	, _TL("Move to Position"),
		_TL(""),
		Fields
	);

	//-----------------------------------------------------
	if( DLG_Parameters(&P) )
	{
		int	Field = Offset + P("FIELD")->asInt(), Position = Offset + P("POSITION")->asInt();

		if( m_pTable->Mov_Field(Field, Position) )
		{
			g_pData->Update(m_pTable, NULL);
		}
	}
}

void CVIEW_Table_Control::On_Field_Move_UI(wxUpdateUIEvent &event)
{
	int	Offset = m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_PointCloud ? 3 : 0;

	event.Enable(m_pTable->Get_Field_Count() > Offset + 1);
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Del(wxCommandEvent &event)
{
	CSG_Parameters	P(_TL("Delete Fields")); int Offset = m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_PointCloud ? 3 : 0;

	for(int i=Offset; i<m_pTable->Get_Field_Count(); i++)
	{
		P.Add_Bool("", SG_Get_String(i), m_pTable->Get_Field_Name(i), _TL(""), false);
	}

	if( DLG_Parameters(&P) )
	{
		bool bChanged = false;

		for(int i=m_pTable->Get_Field_Count()-1; i>=Offset; i--)
		{
			if( P(SG_Get_String(i))->asBool() && m_pTable->Del_Field(i) )
			{
				DeleteCols(i);

				bChanged = true;
			}
		}

		if( bChanged )
		{
			m_pData->m_Fields.Destroy();

			g_pData->Update(m_pTable, NULL);
		}
	}
}

void CVIEW_Table_Control::On_Field_Del_UI(wxUpdateUIEvent &event)
{
	int	Offset	= m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_PointCloud ? 3 : 0;

	event.Enable(m_pTable->Get_Field_Count() > Offset + 0);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Hide(wxCommandEvent &event)
{
	CSG_Parameters P(_TL("Hide Fields"));

	for(int i=0; i<m_pTable->Get_Field_Count(); i++)
	{
		P.Add_Bool("", SG_Get_String(i), m_pTable->Get_Field_Name(i), _TL(""), m_pData->m_Fields.Get_Size());
	}

	for(int i=0; i<(int)m_pData->m_Fields.Get_Size(); i++)
	{
		if( P(m_pData->m_Fields[i]) )
		{
			P[m_pData->m_Fields[i]].Set_Value(false);
		}
	}

	//-----------------------------------------------------
	if( DLG_Parameters(&P) )
	{
		m_pData->m_Fields.Destroy();

		for(int i=0; i<m_pTable->Get_Field_Count(); i++)
		{
			if( !P[i].asBool() )
			{
				m_pData->m_Fields += i;
			}
		}

		Update_Table();
	}
}

void CVIEW_Table_Control::On_Field_Hide_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_Field_Count() > 1);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Rename(wxCommandEvent &event)
{
	CSG_Parameters P(_TL("Rename Fields"));

	for(int i=0; i<m_pTable->Get_Field_Count(); i++)
	{
		P.Add_String("", SG_Get_String(i), m_pTable->Get_Field_Name(i), _TL(""), m_pTable->Get_Field_Name(i));
	}

	//-----------------------------------------------------
	if( DLG_Parameters(&P) )
	{
		for(int i=0; i<m_pTable->Get_Field_Count(); i++)
		{
			CSG_String	s(P(i)->asString());

			if( s.Length() > 0 && s.Cmp(m_pTable->Get_Field_Name(i)) )
			{
				m_pTable->Set_Field_Name(i, s);

				SetColLabelValue(i, s.c_str());
			}
		}

		g_pData->Update(m_pTable, NULL);
	}
}

void CVIEW_Table_Control::On_Field_Rename_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_Field_Count() > 0);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Type(wxCommandEvent &event)
{
	CSG_Parameters P(_TL("Change Field Type"));

	for(int i=0; i<m_pTable->Get_Field_Count(); i++)
	{
		P.Add_Data_Type("", SG_Get_String(i), m_pTable->Get_Field_Name(i), _TL(""), SG_DATATYPES_Table, m_pTable->Get_Field_Type(i));
	}

	if( DLG_Parameters(&P) )
	{
		bool bChanged = false, bResetColors = false;

		for(int i=0; i<m_pTable->Get_Field_Count(); i++)
		{
			TSG_Data_Type Type = P(i)->asDataType()->Get_Data_Type();

			if( Type != m_pTable->Get_Field_Type(i) )
			{
				bChanged = true;

				if( m_pTable->Get_Field_Type(i) == SG_DATATYPE_Color )
				{
					bResetColors = true;
				}

				m_pTable->Set_Field_Type(i, Type);
			}
		}

		if( bChanged )
		{
			if( bResetColors )
			{
				DeleteRows(0, GetNumberRows(), false);
			}

			Update_Table();

			g_pData->Update(m_pTable, NULL);
		}
	}
}

void CVIEW_Table_Control::On_Field_Type_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_Field_Count() > 0);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Sort(wxCommandEvent &event)
{
	if( m_pData->On_Sort() )
	{
		m_Cursor	= -1;

		_Update_Records();
	}
}

void CVIEW_Table_Control::On_Field_Sort_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_Field_Count() > 0 && m_pTable->Get_Count() > 1);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CVIEW_Table_Control::_Parameter_Callback(CSG_Parameter *pParameter, int Flags)
{
	CSG_Parameters	*pParameters	= pParameter ? pParameter->Get_Parameters() : NULL;

	if( pParameters && pParameters->Cmp_Identifier("FIELD_CALCULATOR") )
	{
		if( Flags & PARAMETER_CHECK_ENABLE )
		{
			if( pParameter->Cmp_Identifier("FIELD") )
			{
				pParameters->Set_Enabled("NAME", pParameter->asInt() >= pParameter->asChoice()->Get_Count() - 1);
			}
		}

		return( 1 );
	}

	return( 0 );
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Field_Calc(wxCommandEvent &event)
{
	CSG_String	Fields;

	for(int i=0; i<m_pTable->Get_Field_Count(); i++)
	{
		Fields	+= m_pTable->Get_Field_Name(i) + CSG_String("|");
	}

	Fields	+= _TL("<new>") + CSG_String("|");

	//-----------------------------------------------------
	if( m_Field_Calc.Get_Count() == 0 )
	{
		m_Field_Calc.Create(this, _TL("Table Field Calculator"), _TL(""), SG_T("FIELD_CALCULATOR"));

		m_Field_Calc.Set_Callback_On_Parameter_Changed(_Parameter_Callback);

		m_Field_Calc.Add_Choice(""     , "FIELD"    , _TL("Target Field"), _TL(""), Fields, m_pTable->Get_Field_Count());
		m_Field_Calc.Add_String("FIELD", "NAME"     , _TL("Field Name"  ), _TL(""), _TL("Result"));
		m_Field_Calc.Add_Bool  (""     , "SELECTION", _TL("Selection"   ), _TL(""), true);
		m_Field_Calc.Add_String(""     , "FORMULA"  , _TL("Formula"     ), _TL(""), "f1 + f2");
	}

	m_Field_Calc("FIELD")->asChoice()->Set_Items(Fields);

	m_Field_Calc.Set_Enabled("SELECTION", m_pTable->Get_Selection_Count() > 0);

	//-----------------------------------------------------
	if( DLG_Parameters(&m_Field_Calc) )
	{
		bool	bResult;

		SG_RUN_TOOL(bResult, "table_calculus", 1,	// table field calculator
				SG_TOOL_PARAMETER_SET("TABLE"    , m_pTable)
			&&	SG_TOOL_PARAMETER_SET("FIELD"    , m_Field_Calc("FIELD"    )->asInt   ())
			&&	SG_TOOL_PARAMETER_SET("NAME"     , m_Field_Calc("NAME"     )->asString())
			&&	SG_TOOL_PARAMETER_SET("SELECTION", m_Field_Calc("SELECTION")->asBool  ())
			&&	SG_TOOL_PARAMETER_SET("FORMULA"  , m_Field_Calc("FORMULA"  )->asString())
		);
	}
}

void CVIEW_Table_Control::On_Field_Calc_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_Field_Count() > 0);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Table_Control::On_Record_Add(wxCommandEvent &event)
{
	if( m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_Table && m_pTable->Add_Record() )
	{
		_Update_Records();
	}
}

void CVIEW_Table_Control::On_Record_Add_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_Table && !m_pTable->is_Indexed());
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Record_Ins(wxCommandEvent &event)
{
	if( m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_Table && m_pTable->Ins_Record(GetGridCursorRow()) )
	{
		_Update_Records();
	}
}

void CVIEW_Table_Control::On_Record_Ins_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_ObjectType() == SG_DATAOBJECT_TYPE_Table && !m_pTable->is_Indexed());
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Record_Del(wxCommandEvent &event)
{
	if( m_pTable->Del_Selection() )
	{
		_Update_Records(true);
	}
}

void CVIEW_Table_Control::On_Record_Del_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_Selection_Count() > 0);
}

//---------------------------------------------------------
void CVIEW_Table_Control::On_Record_Clr(wxCommandEvent &event)
{
	if( DLG_Message_Confirm(CMD_Get_Name(ID_CMD_TABLE_RECORD_DEL_ALL), _TL("Table")) && m_pTable->Del_Records() )
	{
		_Update_Records(true);
	}
}

void CVIEW_Table_Control::On_Record_Clr_UI(wxUpdateUIEvent &event)
{
	event.Enable(m_pTable->Get_Count() > 0);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
