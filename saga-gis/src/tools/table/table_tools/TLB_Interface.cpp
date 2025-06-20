
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                     table_tools                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2023 by                 //
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
// 1. Include the appropriate SAGA-API header...

#include <saga_api/saga_api.h>


//---------------------------------------------------------
// 2. Place general tool library information here...

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case TLB_INFO_Name:	default:
		return( _TL("Tools") );

	case TLB_INFO_Category:
		return( _TL("Table") );

	case TLB_INFO_Author:
		return( "SAGA User Group Associaton (c) 2002-2023" );

	case TLB_INFO_Description:
		return( _TL("Tools for the creation and manipulation of tables.") );

	case TLB_INFO_Version:
		return( "1.0" );

	case TLB_INFO_Menu_Path:
		return( _TL("Table|Tools") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

#include "Table_Create_Empty.h"
#include "Table_Rotate.h"
#include "Table_Enumerate.h"
#include "Join_Tables.h"
#include "table_change_date_format.h"
#include "table_change_field_type.h"
#include "table_change_color_format.h"
#include "table_text_replacer.h"
#include "table_field_deletion.h"
#include "table_selection.h"
#include "table_categories_to_indicators.h"
#include "field_formatted_string.h"
#include "table_classify_supervised.h"
#include "table_cluster_analysis.h"


//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	switch( i )
	{
	case  0: return( new CTable_Create_Empty );
	case 22: return( new CTable_Create_Copy );
	case  1: return( new CTable_Rotate );
	case  2: return( new CTable_Enumerate );
	case  3: return( new CJoin_Table );
	case  5: return( new CTable_Change_Date_Format );
	case  6: return( new CTable_Change_Time_Format );
	case  7: return( new CTable_Change_Field_Type );
	case 23: return( new CTable_Change_Field_Name );
	case  8: return( new CTable_Append_Cols );
	case  9: return( new CTable_Change_Color_Format );
	case 10: return( new CTable_Text_Replacer );
	case 11: return( new CTable_Field_Deletion );

	case 15: return( new CSelection_Copy );
	case 16: return( new CSelection_Delete );
	case 17: return( new CSelection_Invert );
	case 18: return( new CSelect_Numeric );
	case 19: return( new CSelect_String );

	case 20: return( new CTable_Categories_to_Indicators );

	case 24: return( new CField_Formatted_String );

	case 26: return( new CTable_Classify_Supervised );

	case 28: return( new CTable_Cluster_Analysis );

	//-----------------------------------------------------
	case 30: return( NULL );
	default: return( TLB_INTERFACE_SKIP_TOOL );
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
//{{AFX_SAGA

	TLB_INTERFACE

//}}AFX_SAGA
