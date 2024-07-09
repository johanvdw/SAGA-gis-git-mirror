
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
//                     Project.cpp                       //
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
#include <wx/string.h>
#include <wx/filename.h>

#include <saga_api/saga_api.h>

#include "helper.h"

#include "res_dialogs.h"

#include "saga_frame.h"

#include "wksp_base_control.h"

#include "wksp_data_manager.h"
#include "wksp_data_menu_files.h"

#include "wksp_table_manager.h"
#include "wksp_table.h"

#include "wksp_shapes_manager.h"
#include "wksp_shapes_type.h"
#include "wksp_shapes.h"

#include "wksp_tin_manager.h"
#include "wksp_tin.h"

#include "wksp_pointcloud_manager.h"
#include "wksp_pointcloud.h"

#include "wksp_grid_manager.h"
#include "wksp_grid_system.h"
#include "wksp_grid.h"

#include "wksp_map_manager.h"
#include "wksp_map.h"
#include "wksp_map_layer.h"
#include "wksp_map_graticule.h"
#include "wksp_map_basemap.h"
#include "wksp_layer.h"

#include "data_source_pgsql.h"

#include "project.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Project::CWKSP_Project(void)
{
	// nop
}

//---------------------------------------------------------
CWKSP_Project::~CWKSP_Project(void)
{
	// nop
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::Has_File_Name(void)
{
	return( wxFileExists(m_File_Name) );
}

//---------------------------------------------------------
bool CWKSP_Project::Clr_File_Name(void)
{
	m_File_Name.Clear();

	g_pSAGA_Frame->Set_Project_Name();

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Project::_Set_Project_Name(void)
{
	if( Has_File_Name() )
	{
		wxFileName fn(m_File_Name);

		if( fn.GetFullName().CmpNoCase("saga_gui.sg-project") && fn.GetFullName().CmpNoCase("saga_gui.cfg") )
		{
			g_pSAGA_Frame->Set_Project_Name(m_File_Name);

			return( true );
		}

		Clr_File_Name();
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::Load(bool bAdd)
{
	wxString File;

	return( DLG_Open(File, ID_DLG_PROJECT_OPEN) && _Load(File, bAdd, true) );
}

//---------------------------------------------------------
bool CWKSP_Project::Load(const wxString &File, bool bAdd, bool bUpdateMenu)
{
	return( _Load(File, bAdd, bUpdateMenu) );
}

//---------------------------------------------------------
bool CWKSP_Project::Save(void)
{
	wxString File;

	return( DLG_Save(File, ID_DLG_PROJECT_SAVE) && _Save(File, true, true) );
}

//---------------------------------------------------------
bool CWKSP_Project::Save(bool bSaveAsOnError)
{
	if( Has_File_Name() )
	{
		return( _Save(m_File_Name, true, true) );
	}

	return( bSaveAsOnError ? Save() : false );
}

//---------------------------------------------------------
bool CWKSP_Project::Save(const wxString &File, bool bSaveModified)
{
	return( _Save(File, bSaveModified, false) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::Copy(void)
{
	wxString File;

	if( !DLG_Save(File, ID_DLG_PROJECT_SAVE) )
	{
		return( false );
	}
 
 	if( SG_File_Cmp_Extension(&File, "sg-project") )
	{
		File = File.Left(File.Length() - 11);
	}

	wxFileName Directory(File);

	Directory.AppendDir(Directory.GetFullName());

	if( !Directory.DirExists() )
	{
		Directory.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}

	for(size_t i=0; i<SG_Get_Data_Manager().Table     ().Count(); i++)
	{
		_Copy_To_File(g_pData->Get(SG_Get_Data_Manager().Table     ().Get(i)), Directory.GetFullPath());
	}

	for(size_t i=0; i<SG_Get_Data_Manager().Shapes    ().Count(); i++)
	{
		_Copy_To_File(g_pData->Get(SG_Get_Data_Manager().Shapes    ().Get(i)), Directory.GetFullPath());
	}

	for(size_t i=0; i<SG_Get_Data_Manager().PointCloud().Count(); i++)
	{
		_Copy_To_File(g_pData->Get(SG_Get_Data_Manager().PointCloud().Get(i)), Directory.GetFullPath());
	}

	for(size_t i=0; i<SG_Get_Data_Manager().TIN       ().Count(); i++)
	{
		_Copy_To_File(g_pData->Get(SG_Get_Data_Manager().TIN       ().Get(i)), Directory.GetFullPath());
	}

	for(size_t i=0; i<SG_Get_Data_Manager().Grid      ().Count(); i++)
	{
		_Copy_To_File(g_pData->Get(SG_Get_Data_Manager().Grid      ().Get(i)), Directory.GetFullPath());
	}

	for(size_t i=0; i<SG_Get_Data_Manager().Grids     ().Count(); i++)
	{
		_Copy_To_File(g_pData->Get(SG_Get_Data_Manager().Grids     ().Get(i)), Directory.GetFullPath());
	}

	return( _Save(Directory.GetFullPath(), false, true) );
}

//---------------------------------------------------------
bool CWKSP_Project::_Copy_To_File(CWKSP_Data_Item *pItem, const wxString &Directory)
{
	wxFileName	Path(Directory);

	switch( pItem->Get_Type() )	// default extensions
	{
	case WKSP_ITEM_Table     : Path.SetExt("txt"     ); break;
	case WKSP_ITEM_Shapes    : Path.SetExt(SG_Shapes_Get_File_Extension_Default().c_str()); break;
	case WKSP_ITEM_TIN       : Path.SetExt(SG_Shapes_Get_File_Extension_Default().c_str()); break;
	case WKSP_ITEM_PointCloud: Path.SetExt("sg-pts-z"); break;
	case WKSP_ITEM_Grid      : Path.SetExt(SG_Grid_Get_File_Extension_Default().c_str()); break;
	case WKSP_ITEM_Grids     : Path.SetExt("sg-gds-z"); break;
	default:	return( false );
	}

	//-----------------------------------------------------
	wxString	Name(pItem->Get_Object()->Get_Name());

	Name.Replace(".", "-");
	Name.Replace(":", "-");

	Path.SetName(Name);

	for(int i=1; Path.FileExists(); i++)
	{
		Path.SetName(Name + wxString::Format("_%d", i));
	}

	Name	= Path.GetFullPath();

	return( pItem->Get_Object()->Save(&Name) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::CopyToDB(void)
{
	return( true );
}

//---------------------------------------------------------
bool CWKSP_Project::_Copy_To_Database(CWKSP_Data_Item *pItem, const wxString &Connection)
{
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::_Load(const wxString &File, bool bAdd, bool bUpdateMenu)
{
	if( wxFileExists(File) && !bAdd && g_pData->Get_Count() > 0 )
	{
		switch( DLG_Message_YesNoCancel(_TL("Close all data sets"), _TL("Load Project")) )
		{
		case 0:
			if( !g_pData->Close(true) )
				return( false );
			break;

		case 2:
			return( false );
		}
	}

	//-------------------------------------------------
	MSG_General_Add_Line();
	MSG_General_Add(wxString::Format("%s %s: %s...", _TL("Loading"), _TL("project"), File), true, true);

	//-------------------------------------------------
	bool bSuccess = false; CSG_MetaData Project;

	if( _Compatibility_Load_Data(File) )
	{
		bSuccess = true;
	}
	else if( !wxFileExists(File) )
	{
		MSG_Error_Add(_TL("file does not exist."            ), true, true, SG_UI_MSG_STYLE_FAILURE);
	}
	else if( !Project.Load(&File) )
	{
		MSG_Error_Add(_TL("could not read project file."    ), true, true, SG_UI_MSG_STYLE_FAILURE);
	}
	else if( Project.Get_Name().CmpNoCase("SAGA_PROJECT") )
	{
		MSG_Error_Add(_TL("invalid project file."           ), true, true, SG_UI_MSG_STYLE_FAILURE);
	}
	else if( Project("DATA") == NULL || Project["DATA"].Get_Children_Count() < 1 )
	{
		MSG_Error_Add(_TL("no data entries in project file."), true, true, SG_UI_MSG_STYLE_FAILURE);
	}
	else if( !_Load_DBConnections(Project["DATA"]) )
	{
		MSG_Error_Add(_TL("could not connect to database."  ), true, true, SG_UI_MSG_STYLE_FAILURE);
	}
	else
	{
		bSuccess = true; CSG_String Version(Project.Get_Property("VERSION"));

		//-------------------------------------------------
		g_pData->Get_Menu_Files()->Set_Update(false);

		for(int i=0; SG_UI_Process_Get_Okay() && i<Project["DATA"].Get_Children_Count(); i++)
		{
			_Load_Data(Project["DATA"][i], SG_File_Get_Path(&File).w_str(), true , Version);
		}

		for(int i=0; SG_UI_Process_Get_Okay() && i<Project["DATA"].Get_Children_Count(); i++)
		{
			_Load_Data(Project["DATA"][i], SG_File_Get_Path(&File).w_str(), false, Version);
		}

		g_pData->Get_Menu_Files()->Set_Update(true);

		//-------------------------------------------------
		g_pSAGA_Frame->Freeze();

		if( Project("MAPS") != NULL )
		{
			if( Project("MAPS.PARAMETERS") )
			{
				for(int i=0; i<Project["MAPS.PARAMETERS"].Get_Children_Count(); i++)
				{
					if( Project["MAPS.PARAMETERS"][i].Cmp_Name("OPTION") && Project["MAPS.PARAMETERS"][i].Cmp_Property("id", "THUMBNAILS") )
					{
						Project["MAPS.PARAMETERS"].Del_Child(i); break; // show thumbnails tab should only be changed through GUI (a change would prompt for restart)
					}
				}

				g_pMaps->Get_Parameters()->Serialize(Project["MAPS.PARAMETERS"], false);
			}

			if( Project["MAPS"].Get_Children_Count() > 0 )
			{
				if( g_pSAGA_Frame->GetActiveChild() && g_pSAGA_Frame->GetActiveChild()->IsMaximized() )
				{
					g_pSAGA_Frame->GetActiveChild()->Restore();
				}

				for(int i=0; SG_UI_Process_Get_Okay() && i<Project["MAPS"].Get_Children_Count(); i++)
				{
					_Load_Map(Project["MAPS"][i], SG_File_Get_Path(&File).w_str());
				}
			}
		}

		#ifndef MDI_TABBED
		if( g_pMaps->Get_Count() > 1 )
		{
			switch( g_pData->Get_Parameter("PROJECT_MAP_ARRANGE")->asInt() )
			{
			case 1:	g_pSAGA_Frame->Tile(wxHORIZONTAL); break;
			case 2:	g_pSAGA_Frame->Tile(wxVERTICAL  ); break;
			case 3: if( g_pSAGA_Frame->GetActiveChild() ) { g_pSAGA_Frame->GetActiveChild()->Maximize(); } break;
			}
		}
		else if( g_pSAGA_Frame->GetActiveChild() )
		{
			g_pSAGA_Frame->GetActiveChild()->Maximize();
		}
		#endif

		g_pSAGA_Frame->Thaw();
	}

	//-----------------------------------------------------
	if( bSuccess )
	{
		if( bUpdateMenu )
		{
			g_pData->Get_Menu_Files()->Recent_Add(SG_DATAOBJECT_TYPE_Undefined, File);
		}

		m_File_Name	= File;

		_Set_Project_Name();

		if( SG_UI_Process_Get_Okay() == false )
		{
			SG_UI_Process_Set_Okay();

			MSG_General_Add(_TL("Loading project has been interrupted."), true, true, SG_UI_MSG_STYLE_FAILURE);

			return( false );
		}

		MSG_General_Add(_TL("Project has been successfully loaded."), true, true, SG_UI_MSG_STYLE_SUCCESS);

		return( true );
	}

	if( bUpdateMenu )
	{
		g_pData->Get_Menu_Files()->Recent_Del(SG_DATAOBJECT_TYPE_Undefined, File);
	}

	CSG_String Name(SG_File_Get_Name(&File, true));

	if( Name.CmpNoCase("saga_gui.sg-project") || !SG_File_Cmp_Path(SG_UI_Get_Application_Path(true), SG_File_Get_Path(&File)) )
	{
		MSG_General_Add(_TL("failed"), false, false, SG_UI_MSG_STYLE_FAILURE);
	}
	else
	{
		MSG_General_Add(_TL("okay"  ), false, false, SG_UI_MSG_STYLE_SUCCESS);
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Project::_Save(const wxString &_File, bool bSaveModified, bool bUpdateMenu)
{
	wxString File(_File);

	if( !SG_File_Cmp_Extension(&File, "sg-project") && !SG_File_Cmp_Extension(&File, "sprj") )
	{
		File += ".sg-project";
	}

	wxString old_File_Name(m_File_Name);

	m_File_Name	= File;

	if( bSaveModified && !g_pData->Save_Modified() )
	{
		m_File_Name	= old_File_Name;

		return( false );
	}

	//-----------------------------------------------------
	CSG_MetaData Project, &Data = *Project.Add_Child("DATA");

	wxString ProjectDir(SG_File_Get_Path(&File).w_str());

	Project.Set_Name("SAGA_PROJECT"); Project.Add_Property("VERSION", SAGA_VERSION);

	//-----------------------------------------------------
	CWKSP_Table_Manager *pTables = g_pData->Get_Tables();

	for(int i=0; pTables && i<pTables->Get_Count(); i++)
	{
		_Save_Data(Data, ProjectDir, pTables->Get_Data(i));
	}

	CWKSP_TIN_Manager *pTINs = g_pData->Get_TINs();

	for(int i=0; pTINs && i<pTINs->Get_Count(); i++)
	{
		_Save_Data(Data, ProjectDir, pTINs->Get_Data(i));
	}

	CWKSP_PointCloud_Manager *pPointClouds = g_pData->Get_PointClouds();

	for(int i=0; pPointClouds && i<pPointClouds->Get_Count(); i++)
	{
		_Save_Data(Data, ProjectDir, pPointClouds->Get_Data(i));
	}

	CWKSP_Shapes_Manager *pShapes = g_pData->Get_Shapes();

	for(int j=0; pShapes && j<pShapes->Get_Count(); j++)
	{
		for(int i=0; i<pShapes->Get_Shapes_Type(j)->Get_Count(); i++)
		{
			_Save_Data(Data, ProjectDir, pShapes->Get_Shapes_Type(j)->Get_Data(i));
		}
	}

	CWKSP_Grid_Manager *pGrids = g_pData->Get_Grids();

	for(int j=0; pGrids && j<pGrids->Get_Count(); j++)
	{
		for(int i=0; i<pGrids->Get_System(j)->Get_Count(); i++)
		{
			_Save_Data(Data, ProjectDir, ((CWKSP_Data_Item *)pGrids->Get_System(j)->Get_Item(i)));
		}
	}

	//-----------------------------------------------------
	CSG_MetaData &Maps = *Project.Add_Child("MAPS");

	g_pMaps->Get_Parameters()->Serialize(*Maps.Add_Child("PARAMETERS"), true);

	if( g_pMaps->Get_Count() > 0 )
	{
		for(int i=0; i<g_pMaps->Get_Count(); i++)
		{
			_Save_Map(Maps, ProjectDir, g_pMaps->Get_Map(i));
		}
	}

	//-----------------------------------------------------
	if( Project.Save(&File) )
	{
		m_File_Name	= File;

		if( bUpdateMenu )
		{
			g_pData->Get_Menu_Files()->Recent_Add(SG_DATAOBJECT_TYPE_Undefined, m_File_Name);
		}

		MSG_General_Add(_TL("Project has been saved."), true, true, SG_UI_MSG_STYLE_SUCCESS);

		_Set_Project_Name();

		PROGRESSBAR_Set_Position(0);

		return( true );
	}

	m_File_Name.Clear();

	if( bUpdateMenu )
	{
		g_pData->Get_Menu_Files()->Recent_Del(SG_DATAOBJECT_TYPE_Undefined, File);
	}

	MSG_General_Add(_TL("Could not save project."), true, true, SG_UI_MSG_STYLE_FAILURE);

	PROGRESSBAR_Set_Position(0);

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::_Load_DBConnections(const CSG_MetaData &Data)
{
	CSG_Strings Connections;

	for(int i=0; i<Data.Get_Children_Count(); i++)
	{
		CSG_String Connection(Data[i].Get_Child("FILE") ? Data[i].Get_Child("FILE")->Get_Content() : "");

		if( !Connection.BeforeFirst(':').CmpNoCase("PGSQL") )
		{
			Connection = Connection.AfterFirst(':'); CSG_String Host  (Connection.BeforeFirst(':'));
			Connection = Connection.AfterFirst(':'); CSG_String Port  (Connection.BeforeFirst(':'));
			Connection = Connection.AfterFirst(':'); CSG_String DBName(Connection.BeforeFirst(':'));

			Connection = Host + ":" + Port + ":" + DBName;

			bool  bAdd = true;

			for(int j=0; j<Connections.Get_Count() && bAdd; j++)
			{
				if( !Connection.Cmp(Connections[j]) )
				{
					bAdd = false;
				}
			}

			if( bAdd )
			{
				Connections += Connection;
			}
		}
	}

	for(int i=0; i<Connections.Get_Count(); i++)
	{
		CSG_String Host  (Connections[i].BeforeFirst(':'));
		CSG_String Port  (Connections[i].AfterFirst (':').BeforeLast(':'));
		CSG_String DBName(Connections[i].AfterLast  (':'));

		if( !PGSQL_Connect(Host, Port, DBName) )
		{
			return( false );
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::_Load_Data(const CSG_MetaData &Entry, const wxString &ProjectDir, bool bLoad, const CSG_String &Version)
{
	if( !Entry.Cmp_Name("DATASET") || !Entry("FILE") || Entry["FILE"].Get_Content().is_Empty() )
	{
		return( false );
	}

	TSG_Data_Object_Type Type
	= Entry.Cmp_Property("type", "GRID"  ) ? SG_DATAOBJECT_TYPE_Grid
	: Entry.Cmp_Property("type", "GRIDS" ) ? SG_DATAOBJECT_TYPE_Grids
	: Entry.Cmp_Property("type", "TABLE" ) ? SG_DATAOBJECT_TYPE_Table
	: Entry.Cmp_Property("type", "SHAPES") ? SG_DATAOBJECT_TYPE_Shapes
	: Entry.Cmp_Property("type", "TIN"   ) ? SG_DATAOBJECT_TYPE_TIN
	: Entry.Cmp_Property("type", "POINTS") ? SG_DATAOBJECT_TYPE_PointCloud
	:                                        SG_DATAOBJECT_TYPE_Undefined;

	if( Type == SG_DATAOBJECT_TYPE_Undefined )
	{
		return( false );
	}

	//-----------------------------------------------------
	wxString File = Entry("FILE")->Get_Content().c_str();

	if( File.Find("PGSQL") != 0 && wxFileExists(Get_FilePath_Absolute(ProjectDir, File)) )
	{
		File = Get_FilePath_Absolute(ProjectDir, File);
	}

	CSG_String Unique_ID; Entry.Get_Property("dataset_id", Unique_ID);

	//-----------------------------------------------------
	CWKSP_Data_Item *pItem = NULL;

	if( bLoad )
	{
		if( Type == SG_DATAOBJECT_TYPE_Grid && Entry("PARAMETERS"))
		{
			for(int i=0; i<Entry["PARAMETERS"].Get_Children_Count() && !pItem; i++)
			{
				if( Entry["PARAMETERS"][i].Cmp_Property("id", "FILE_CACHE") )
				{
					bool bCached = Entry["PARAMETERS"][i].Cmp_Content("TRUE", true);

					pItem = g_pData->Add(SG_Create_Grid(&File, SG_DATATYPE_Undefined, bCached));

					if( pItem )
					{
						pItem->Set_Unique_ID(Unique_ID.c_str());
					}
				}
			}
		}
	}

	if( !pItem )
	{
		if( bLoad )
		{
			pItem = g_pData->Open(File, Type);

			if( pItem && !Unique_ID.is_Empty() )
			{
				pItem->Set_Unique_ID(Unique_ID.c_str());
			}
		}
		else
		{
			pItem = g_pData->Get_byID_or_File(Unique_ID, File, ProjectDir);
		}
	}

	//-----------------------------------------------------
	if( !pItem || !pItem->Get_Parameters() || !Entry.Get_Child("PARAMETERS") )
	{
		if( bLoad )
		{
			MSG_Error_Add(wxString::Format("%s [%s]", _TL("failed to load data"), File.c_str()));
		}

		return( false );
	}

	//-----------------------------------------------------
	CSG_MetaData *pEntry = Entry("PARAMETERS");

	for(int i=0; i<pEntry->Get_Children_Count(); i++)
	{
		if( !pEntry->Get_Child(i)->Get_Name().CmpNoCase("DATA") && !pEntry->Get_Child(i)->Get_Content().is_Empty() && pEntry->Get_Child(i)->Get_Content().BeforeFirst(':').CmpNoCase("PGSQL") )
		{
			wxString File(Get_FilePath_Absolute(ProjectDir, pEntry->Get_Child(i)->Get_Content().w_str()));

			pEntry->Get_Child(i)->Set_Content(&File);

		//	if( SG_Compare_SAGA_Version(Version) < 0 )
			{
				if( pEntry->Get_Child(i)->Cmp_Property("id", "OVERLAY_1") )
				{
					pEntry->Get_Child(i)->Set_Property("id", "OVERLAY_G");
				}

				if( pEntry->Get_Child(i)->Cmp_Property("id", "OVERLAY_2") )
				{
					pEntry->Get_Child(i)->Set_Property("id", "OVERLAY_B");
				}
			}
		}
	}

	pItem->Get_Parameters()->Serialize(*pEntry, false);

	//-----------------------------------------------------
	if( SG_Compare_Version(Version, "7.0.0") < 0 )	// inter-version-compatibility
	{
		CSG_Parameter *pParameter = pItem->Get_Parameter("COLORS_TYPE");

		if( pParameter && Type == SG_DATAOBJECT_TYPE_Grid )
		{
			if( pParameter->asInt() == 4 ) { pParameter->Set_Value(6); }	// Shade
			if( pParameter->asInt() == 5 ) { pParameter->Set_Value(4); }	// RGB Overlay
			if( pParameter->asInt() == 6 ) { pParameter->Set_Value(5); }	// RGB Composite
		}
	}

	//-----------------------------------------------------
	if( Type == SG_DATAOBJECT_TYPE_Grid && Type == pItem->Get_Object()->Get_ObjectType() )
	{
		pItem->Get_Parameter("FILE_CACHE")->Set_Value(((CWKSP_Grid *)pItem)->Get_Grid()->is_Cached());
	}

	pItem->Parameters_Changed();

	pItem->Get_Object()->Set_Modified(false);	// differing no-data values might have set the modified flag, ...ignore it here!

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Project::_Save_Data(CSG_MetaData &Entry, const wxString &ProjectDir, CWKSP_Data_Item *pItem)
{
	CSG_Data_Object *pDataObject = pItem ? pItem->Get_Object() : NULL;

	if( !pDataObject || !pDataObject->Get_File_Name(false) || SG_STR_LEN(pDataObject->Get_File_Name(false)) == 0 )
	{
		return( false );
	}

	CSG_MetaData *pEntry = Entry.Add_Child("DATASET");

	pEntry->Add_Property("dataset_id", pItem->Get_Unique_ID().wc_str());

	switch( pDataObject->Get_ObjectType() )
	{
	case SG_DATAOBJECT_TYPE_Grid      :	pEntry->Add_Property("type", "GRID"  );	break;
	case SG_DATAOBJECT_TYPE_Grids     :	pEntry->Add_Property("type", "GRIDS" );	break;
	case SG_DATAOBJECT_TYPE_Table     :	pEntry->Add_Property("type", "TABLE" );	break;
	case SG_DATAOBJECT_TYPE_Shapes    :	pEntry->Add_Property("type", "SHAPES");	break;
	case SG_DATAOBJECT_TYPE_TIN       :	pEntry->Add_Property("type", "TIN"   );	break;
	case SG_DATAOBJECT_TYPE_PointCloud:	pEntry->Add_Property("type", "POINTS");	break;
	default: return( false );
	}

	if( wxFileExists(pDataObject->Get_File_Name(false)) )
	{
		pEntry->Add_Child("FILE", SG_File_Get_Path_Relative(&ProjectDir, pDataObject->Get_File_Name(false)));
	}
	else if( pDataObject->Get_MetaData_DB().Get_Children_Count() > 0 )
	{
		pEntry->Add_Child("FILE", pDataObject->Get_File_Name(false));
	}
	else
	{
		return( false );
	}

	if( pItem->Get_Parameters() )
	{
		pItem->Get_Parameters()->DataObjects_Check(true);

		pItem->Get_Parameters()->Serialize(*pEntry->Add_Child("PARAMETERS"), true);

		//-------------------------------------------------
		pEntry = pEntry->Get_Child("PARAMETERS");

		for(int i=0; i<pEntry->Get_Children_Count(); i++)
		{
			if( !pEntry->Get_Child(i)->Get_Name().CmpNoCase("DATA") )
			{
				CSG_String File = pEntry->Get_Child(i)->Get_Content();

				if( File.BeforeFirst(':').CmpNoCase("PGSQL") && SG_File_Exists(File) )
				{
					pEntry->Get_Child(i)->Set_Content(SG_File_Get_Path_Relative(&ProjectDir, File.w_str()));
				}
			}
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Project::_Load_Map(const CSG_MetaData &Entry, const wxString &ProjectDir)
{
	if( Entry.Cmp_Name("MAP") )
	{
		CWKSP_Map *pMap = new CWKSP_Map;

		if( g_pMaps->Add(pMap) && pMap->Serialize(*((CSG_MetaData *)&Entry), ProjectDir, false) )
		{
			int Restore; Restore = Entry.Get_Property("SHOWN", Restore) ? Restore : 1;

			#ifndef MDI_TABBED
			if( Restore && g_pData->Get_Parameter("PROJECT_MAP_ARRANGE")->asInt() != 4 )
			#else
			if( Restore && g_pData->Get_Parameter("PROJECT_MAP_ARRANGE")->asInt() != 1 )
			#endif
			{
				pMap->View_Show(true);
			}

			return( true );
		}

		g_pMaps->Get_Control()->Del_Item(pMap, true);
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Project::_Save_Map(CSG_MetaData &Entry, const wxString &ProjectDir, CWKSP_Map *pMap)
{
	return( pMap && pMap->Serialize(Entry, ProjectDir, true) );
}


///////////////////////////////////////////////////////////
//														 //
//					v2.0 Compatibility					 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define DATA_ENTRIES_BEGIN	wxT("[DATA_ENTRIES_BEGIN]")
#define DATA_ENTRIES_END	wxT("[DATA_ENTRIES_END]")
#define DATA_ENTRY_BEGIN	wxT("[DATA_ENTRY_BEGIN]")
#define DATA_ENTRY_END		wxT("[DATA_ENTRY_END]")

//---------------------------------------------------------
#define MAP_ENTRIES_BEGIN	wxT("[MAP_ENTRIES_BEGIN]")
#define MAP_ENTRIES_END		wxT("[MAP_ENTRIES_END]")
#define MAP_ENTRY_BEGIN		wxT("[MAP_ENTRY_BEGIN]")
#define MAP_ENTRY_END		wxT("[MAP_ENTRY_END]")
#define MAP_ENTRY_NAME		wxT("[MAP_ENTRY_NAME]")

//---------------------------------------------------------
bool CWKSP_Project::_Compatibility_Load_Data(const wxString &File)
{
	CSG_String	sLine;
	CSG_File	Stream;

	if( !Stream.Open(&File, SG_FILE_R, true) )
	{
		return( false );
	}

	//-------------------------------------------------
	while( Stream.Read_Line(sLine) && sLine.Cmp(DATA_ENTRIES_BEGIN) );

	if( sLine.Cmp(DATA_ENTRIES_BEGIN) )
	{
		return( false );
	}

	g_pSAGA_Frame->Freeze();

	g_pData->Get_Menu_Files()->Set_Update(false);
	while( _Compatibility_Load_Data(Stream, SG_File_Get_Path(&File).w_str()) );
	g_pData->Get_Menu_Files()->Set_Update(true);

	//-------------------------------------------------
	while( Stream.Read_Line(sLine) && sLine.Cmp(MAP_ENTRIES_BEGIN) );

	if( !sLine.Cmp(MAP_ENTRIES_BEGIN) )
	{
		while( _Compatibility_Load_Map(Stream, SG_File_Get_Path(&File).w_str()) );
	}

	#ifndef MDI_TABBED
	switch( g_pData->Get_Parameter("PROJECT_MAP_ARRANGE")->asInt() )
	{
	case 1: g_pSAGA_Frame->Tile(wxHORIZONTAL); break;
	case 2: g_pSAGA_Frame->Tile(wxVERTICAL  ); break;
	}
	#endif

	g_pSAGA_Frame->Thaw();

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Project::_Compatibility_Load_Data(CSG_File &Stream, const wxString &ProjectDir)
{
	int				Type;
	CSG_String		sLine, sPath;
	CWKSP_Base_Item	*pItem;

	while( Stream.Read_Line(sLine) && sLine.Cmp(DATA_ENTRY_BEGIN) && sLine.Cmp(DATA_ENTRIES_END) );

	if( !sLine.Cmp(DATA_ENTRY_BEGIN) )
	{
		if(	Stream.Read_Line(sPath) && Stream.Read_Line(sLine) && sLine.asInt(Type) )
		{
			if( SG_File_Exists(sPath = Get_FilePath_Absolute(ProjectDir, sPath.w_str()).wc_str()) )
			{
				if(	(pItem = g_pData->Open(sPath.w_str(), Type)) != NULL )
				{
					if( pItem->Get_Parameters() )
					{
						pItem->Get_Parameters()->Serialize_Compatibility(Stream);
						pItem->Parameters_Changed();
					}
				}
			}
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Project::_Compatibility_Load_Map(CSG_File &Stream, const wxString &ProjectDir)
{
	TSG_Rect		r;
	CSG_String		sLine, sName;
	CWKSP_Base_Item	*pItem;
	CWKSP_Map		*pMap;

	while( Stream.Read_Line(sLine) && sLine.Cmp(MAP_ENTRY_BEGIN) && sLine.Cmp(MAP_ENTRIES_END) );

	if( !sLine.Cmp(MAP_ENTRY_BEGIN) && Stream.Read_Line(sLine) )
	{
		if(	SG_SSCANF(sLine, SG_T("%lf %lf %lf %lf"), &r.xMin, &r.xMax, &r.yMin, &r.yMax) == 4 )
		{
			pMap	= NULL;

			while( Stream.Read_Line(sLine) && sLine.Cmp(MAP_ENTRY_END) )
			{
				if( !sLine.Cmp(MAP_ENTRY_NAME) )
				{
					Stream.Read_Line(sName);
				}
				else
				{
					sLine	= Get_FilePath_Absolute(ProjectDir, sLine.w_str()).wc_str();

					if( (pItem = g_pData->Get_byFile(sLine.c_str())) != NULL
					&& ( pItem->Get_Type() == WKSP_ITEM_Grid
					  || pItem->Get_Type() == WKSP_ITEM_Grids
					  || pItem->Get_Type() == WKSP_ITEM_TIN
					  || pItem->Get_Type() == WKSP_ITEM_PointCloud
					  || pItem->Get_Type() == WKSP_ITEM_Shapes) )
					{
						if( pMap == NULL )
						{
							pMap	= new CWKSP_Map;
						}

						g_pMaps->Add((CWKSP_Layer *)pItem, pMap);
					}
				}
			}

			if( pMap )
			{
				if( sName.Length() > 0 )
				{
					pMap->Get_Parameter("NAME")->Set_Value(sName);
					pMap->Parameters_Changed();
				}

				pMap->Set_Extent(r, true);
				pMap->View_Show(true);
			}
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
