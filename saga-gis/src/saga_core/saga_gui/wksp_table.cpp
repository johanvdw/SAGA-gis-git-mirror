
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
//                    WKSP_Table.cpp                     //
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
#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"

#include "wksp_base_control.h"
#include "wksp_data_manager.h"
#include "wksp_table.h"

#include "view_table.h"
#include "view_table_diagram.h"

#include "data_source_pgsql.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Table::CWKSP_Table(CSG_Table *pTable)
	: CWKSP_Data_Item(pTable)
{
	m_pView		= NULL;
	m_pDiagram	= NULL;

	//-----------------------------------------------------
	On_Create_Parameters();

	DataObject_Changed();
}

//---------------------------------------------------------
CWKSP_Table::~CWKSP_Table(void)
{
	if( m_pObject->Get_ObjectType() != SG_DATAOBJECT_TYPE_Table )
	{
		m_pObject	= NULL;
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Table::Get_Description(void)
{
	wxString s;

	//-----------------------------------------------------
	s += wxString::Format("<h4>%s</h4>", _TL("Table"));

	s += "<table border=\"0\">";

	DESC_ADD_STR(_TL("Name"              ), m_pObject->Get_Name());
	DESC_ADD_STR(_TL("Description"       ), m_pObject->Get_Description());

	if( SG_File_Exists(m_pObject->Get_File_Name(false)) )
	{
		DESC_ADD_STR(_TL("Data Source"   ), SG_File_Get_Path(m_pObject->Get_File_Name(false)      ).c_str());
		DESC_ADD_STR(_TL("File"          ), SG_File_Get_Name(m_pObject->Get_File_Name(false), true).c_str());
	}
	else if( m_pObject->Get_MetaData_DB().Get_Children_Count() )
	{
		DESC_ADD_STR(_TL("Data Source"   ), m_pObject->Get_File_Name(false));
	}
	else
	{
		DESC_ADD_STR(_TL("Data Source"   ), _TL("memory"));
	}

	DESC_ADD_STR (_TL("Modified"         ), m_pObject->is_Modified() ? _TL("yes") : _TL("no"));
	DESC_ADD_LONG(_TL("Number of Records"), Get_Table()->Get_Count());
	DESC_ADD_LONG(_TL("Selected"         ), Get_Table()->Get_Selection_Count());
	DESC_ADD_STR (_TL("File Encoding"    ), Get_Table()->Get_File_Encoding() ? SG_T("UTF-8") : SG_T("ANSI"));

	s += "</table>";

	s += Get_TableInfo_asHTML(Get_Table());

	//-----------------------------------------------------
//	s += wxString::Format(wxT("<hr><b>%s</b><font size=\"-1\">"), _TL("Data History"));
//	s += m_pTable->Get_History().Get_HTML();
//	s += wxString::Format(wxT("</font"));

	//-----------------------------------------------------
	return( s );
}

//---------------------------------------------------------
wxMenu * CWKSP_Table::Get_Menu(void)
{
	wxMenu	*pMenu	= new wxMenu(m_pObject->Get_Name());

	if( m_pObject->Get_ObjectType() == SG_DATAOBJECT_TYPE_Table )
	{
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_CLOSE);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_SAVE);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_SAVEAS);

		if( PGSQL_has_Connections() )
			CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_SAVETODB);

		if( m_pObject->is_Modified() && SG_File_Exists(m_pObject->Get_File_Name(false)) )
			CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_RELOAD);

		if( m_pObject->is_File_Native() )
			CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_DEL_FILES);

		if( m_pObject->Get_MetaData().Get_Children_Count() > 0 )
			CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_METADATA);
	}

	CMD_Menu_Add_Item(pMenu,  true, ID_CMD_TABLE_SHOW);
	CMD_Menu_Add_Item(pMenu,  true, ID_CMD_TABLE_DIAGRAM);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_TABLE_SCATTERPLOT);

	return( pMenu );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Table::On_Command(int Cmd_ID)
{
	switch( Cmd_ID )
	{
	default:
		return( CWKSP_Data_Item::On_Command(Cmd_ID) );

	case ID_CMD_WKSP_ITEM_RETURN:
		Set_View(true);
		break;

	case ID_CMD_TABLE_SHOW:
		Toggle_View();
		break;

	case ID_CMD_TABLE_DIAGRAM:
		Toggle_Diagram();
		break;

	case ID_CMD_TABLE_SCATTERPLOT:
		Add_ScatterPlot();
		break;
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Table::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	default:
		return( CWKSP_Data_Item::On_Command_UI(event) );

	case ID_CMD_TABLE_SHOW:
		event.Check(m_pView != NULL);
		break;

	case ID_CMD_TABLE_DIAGRAM:
		event.Check(m_pDiagram != NULL);
		break;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Table::On_Create_Parameters(void)
{
	CWKSP_Data_Item::On_Create_Parameters();

	//-----------------------------------------------------
	m_Parameters.Add_Node("", "NODE_TABLE", _TL("Display"), _TL(""));

	m_Parameters.Add_Parameter(g_pData->Get_Parameter("TABLE_FLT_STYLE"   ));
	m_Parameters.Add_Parameter(g_pData->Get_Parameter("TABLE_FLT_DECIMALS"));
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Table::On_DataObject_Changed(void)
{
	CWKSP_Data_Item::On_DataObject_Changed();

	//-----------------------------------------------------
	// ...
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Table::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags)
{
	if( Flags & PARAMETER_CHECK_ENABLE )
	{
		if( pParameter->Cmp_Identifier("TABLE_FLT_STYLE") )
		{
			pParameters->Set_Enabled("TABLE_FLT_DECIMALS", pParameter->asInt() == 2); // fix number of decimals
		}
	}

	return( CWKSP_Data_Item::On_Parameter_Changed(pParameters, pParameter, Flags) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Table::Set_View(bool bShow)
{
	if( bShow )
	{
		if( !m_pView )
		{
			m_pView	= new CVIEW_Table(this);
		}
		else
		{
			m_pView->Activate();
		}
	}
	else if( m_pView )
	{
		m_pView->Destroy();
	}
}

//---------------------------------------------------------
void CWKSP_Table::Toggle_View(void)
{
	Set_View(m_pView == NULL);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Table::Set_Diagram(bool bShow, CSG_Parameters *pParameters)
{
	if( bShow )
	{
		if( m_pDiagram == NULL )
		{
			m_pDiagram = new CVIEW_Table_Diagram(this, pParameters);
		}
		else if( pParameters )
		{
			m_pDiagram->Update_Parameters(pParameters);
		}
	}
	else if( m_pDiagram )
	{
		m_pDiagram->Destroy();
	}

	return( true );
}

//---------------------------------------------------------
void CWKSP_Table::Toggle_Diagram(void)
{
	Set_Diagram(m_pDiagram == NULL);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Table::Show(int Flags)
{
	Set_View(true);

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Table::View_Closes(MDI_ChildFrame *pView)
{
	if( pView == m_pView    ) m_pView    = NULL;
	if( pView == m_pDiagram ) m_pDiagram = NULL;

	return( CWKSP_Data_Item::View_Closes(pView) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
