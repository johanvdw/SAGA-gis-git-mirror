
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                      io_shapes                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  citygml_import.cpp                   //
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
#include "citygml_import.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CCityGML_Import::CCityGML_Import(void)
{
	Set_Name		(_TL("Import Building Sketches from CityGML"));

	Set_Author		("O.Conrad (c) 2014");

	Set_Description	(_TW(
		"This tool facilitates the import of building sketches using a CityGML based file format, "
		"that is commonly used by German land surveying offices and geoinformation distributors. "
	));

	//-----------------------------------------------------
	Parameters.Add_Shapes("",
		"BUILDINGS", _TL("Buildings"),
		_TL(""),
		PARAMETER_OUTPUT, SHAPE_TYPE_Polygon
	);

	Parameters.Add_FilePath("",
		"FILES"    , _TL("Files"),
		_TL(""),
		CSG_String::Format("%s|*.xml;*.gml|XML %s (*.xml)|*.xml|GML %s (*.gml)|*.xml|%s|*.*",
			_TL("Recognized Files"), _TL("Files"), _TL("Files"), _TL("All Files")
		), NULL, false, false, true
	);

	Parameters.Add_Bool("",
		"PARTS"    , _TL("Check for Building Parts"),
		_TL(""),
		false
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCityGML_Import::On_Execute(void)
{
	CSG_Strings	Files;

	if( !Parameters("FILES")->asFilePath()->Get_FilePaths(Files) )
	{
		return( false );
	}

	CSG_Shapes Buildings(SHAPE_TYPE_Polygon), *pBuildings = Parameters("BUILDINGS")->asShapes(); pBuildings->Destroy();

	for(int i=0; i<Files.Get_Count(); i++)
	{
		if( pBuildings->Get_Count() <= 0 )
		{
			Get_Buildings(Files[i], pBuildings, Parameters("PARTS")->asBool());
		}
		else if( Get_Buildings(Files[i], &Buildings, Parameters("PARTS")->asBool()) )
		{
			Add_Buildings(pBuildings, &Buildings);

			CSG_String Description(pBuildings->Get_Description());

			Description += "\n";
			Description += Buildings.Get_Name();

			pBuildings->Set_Description(Description);
		}
	}

	return( pBuildings->Get_Count() > 0 );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCityGML_Import::Load_Shapes(const CSG_String &File, CSG_Shapes &Shapes, int Type)
{
	CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Create_Tool("io_gdal", 3); // Import Shapes

	if( pTool )
	{
		CSG_Data_Manager Data; pTool->Set_Manager(&Data);

		pTool->Set_Parameter("FILES"    , File);
		pTool->Set_Parameter("GEOM_TYPE", Type);

		bool bResult = pTool->Execute() && pTool->Get_Parameter("SHAPES")->asList()->Get_Item_Count() > 0;

		if( bResult )
		{
			CSG_Shapes *pShapes = pTool->Get_Parameter("SHAPES")->asList()->Get_Item(0)->asShapes();

			Shapes.Create(*pShapes);
		}

		SG_Get_Tool_Library_Manager().Delete_Tool(pTool);

		return( bResult && Shapes.Get_Count() > 0 );
	}

	return( false );
}

//---------------------------------------------------------
bool CCityGML_Import::Get_Buildings(const CSG_String &File, CSG_Shapes *pPolygons)
{
	Process_Set_Text(_TL("importing gml"));

	CSG_Shapes Shapes;

	if( !Load_Shapes(File, Shapes, 0)
	&&  !Load_Shapes(File, Shapes, 9) && !Load_Shapes(File, Shapes, 11)
	&&  !Load_Shapes(File, Shapes, 5) && !Load_Shapes(File, Shapes, 7) )
	{
		Error_Set(CSG_String::Format("%s: %s", _TL("CityGML import failed"), File.c_str()));

		return( false );
	}

	//-----------------------------------------------------
	if( Shapes.Get_Type() == SHAPE_TYPE_Line )
	{
		Process_Set_Text(_TL("polygon conversion"));

		CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Create_Tool("shapes_polygons", 3);	// Convert Lines to Polygons

		if(	pTool )
		{
			pTool->Set_Manager(NULL);

			bool bResult
				=  pTool->Set_Parameter("POLYGONS", pPolygons)
				&& pTool->Set_Parameter("LINES"   , &Shapes)
				&& pTool->Set_Parameter("MERGE"   , true)
				&& pTool->Execute();

			SG_Get_Tool_Library_Manager().Delete_Tool(pTool);

			pPolygons->Set_Name(SG_File_Get_Name(File, false));

			return( bResult );
		}

		Error_Set(_TL("could not locate line string to polygon conversion tool"));

		return( false );
	}

	//-----------------------------------------------------
	if( Shapes.Get_Type() == SHAPE_TYPE_Polygon )
	{
		pPolygons->Create(Shapes);
		pPolygons->Set_Name(SG_File_Get_Name(File, false));

		for(int i=0; i<pPolygons->Get_Count(); i++)
		{
			CSG_Shape_Polygon *pPolygon = pPolygons->Get_Shape(i)->asPolygon();

			for(int j=pPolygon->Get_Part_Count()-1; j>0; j--)
			{
				pPolygon->Del_Part(j);
			}
		}

		return( true );
	}

	//-----------------------------------------------------
	Error_Set(CSG_String::Format("%s: %s", _TL("CityGML import failed"), File.c_str()));

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCityGML_Import::Get_Buildings(const CSG_String &File, CSG_Shapes *pBuildings, bool bParts)
{
	if( !Get_Buildings(File, pBuildings) )
	{
		Error_Set(_TL("CityGML file import failed"));

		return( false );
	}

	if( bParts == false )
	{
		return( true );
	}

	//-----------------------------------------------------
	// 1. check for building parts

	Process_Set_Text("%s: %s", _TL("loading building parts"), SG_File_Get_Name(File, true).c_str());

	CSG_MetaData	GML, GML_Parts;

	if( !GML.Create(File) )
	{
		Error_Set(_TL("loading failed"));

		return( false );
	}

	GML_Parts.Assign(GML, false);

	bParts	= false;

	for(int i=0; i<GML.Get_Children_Count(); i++)
	{
		if( GML[i].Get_Name().CmpNoCase("core:cityObjectMember") != 0 )
		{
			GML_Parts.Add_Child(GML[i]);
		}
		else if( Has_BuildingParts(GML[i]) && Add_BuildingParts(GML[i][0], GML_Parts) )
		{
			bParts	= true;
		}
	}

	if( bParts == false )
	{
		return( true );
	}

	//-----------------------------------------------------
	Process_Set_Text("%s: %s", _TL("saving building parts"), SG_File_Get_Name(File, true).c_str());

	CSG_String	tmpFile	= SG_File_Make_Path(SG_Dir_Get_Temp(), SG_File_Get_Name(File, true));

	if( !GML_Parts.Save(tmpFile) )
	{
		SG_File_Delete(tmpFile);

		Error_Set(_TL("check for building parts failed"));

		return( false );
	}

	//-----------------------------------------------------
	CSG_Shapes	Parts(SHAPE_TYPE_Polygon);

	if( Get_Buildings(tmpFile, &Parts) )
	{
		Add_Buildings(pBuildings, &Parts);
	}

	//-----------------------------------------------------
	SG_File_Delete(tmpFile);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCityGML_Import::Add_Buildings(CSG_Shapes *pBuildings, CSG_Shapes *pAdd)
{
	int *Index = (int *)SG_Malloc(pBuildings->Get_Field_Count() * sizeof(int));

	for(int i=0; i<pBuildings->Get_Field_Count(); i++)
	{
		CSG_String Name(pBuildings->Get_Field_Name(i));

		Index[i] = -1;

		for(int j=0; Index[i]<0 && j<pAdd->Get_Field_Count(); j++)
		{
			if( !Name.CmpNoCase(pAdd->Get_Field_Name(j)) )
			{
				Index[i] = j;
			}
		}
	}

	for(int i=0; i<pAdd->Get_Count(); i++)
	{
		CSG_Shape    *pPart = pAdd->Get_Shape(i);
		CSG_Shape *pPolygon = pBuildings->Add_Shape(pPart, SHAPE_COPY_GEOM);

		for(int j=0; j<pBuildings->Get_Field_Count(); j++)
		{
			if( Index[j] >= 0 )
			{
				*pPolygon->Get_Value(j) = *pPart->Get_Value(Index[j]);
			}
		}
	}

	SG_Free(Index);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCityGML_Import::Has_BuildingParts(const CSG_MetaData &GML)
{
	return( GML.Get_Name().CmpNoCase("core:cityObjectMember") == 0
		&&  GML.Get_Children_Count() == 1 && GML[0].Get_Name().CmpNoCase("bldg:Building") == 0
		&&  GML[0].Get_Child("bldg:consistsOfBuildingPart") != NULL
	);
}

//---------------------------------------------------------
bool CCityGML_Import::Add_BuildingParts(const CSG_MetaData &GML, CSG_MetaData &GML_Parts)
{
	if( GML.Get_Name().CmpNoCase("bldg:Building") )
	{
		return( false );
	}

	int				i;
	CSG_MetaData	head;

	for(i=0; i<GML.Get_Children_Count(); i++)
	{
		if( GML[i].Get_Name().CmpNoCase("core:creationDate")
		&&  GML[i].Get_Name().BeforeFirst(':').CmpNoCase("bldg") != 0 )
	//	&&  GML[i].Get_Name().BeforeFirst(':').CmpNoCase("core") != 0 )
	//	if( GML[i].Get_Name().CmpNoCase("bldg:consistsOfBuildingPart")
		{
			head.Add_Child(GML[i]);
		}
	}

	for(i=0; i<GML.Get_Children_Count(); i++)
	{
		if( GML[i]   .Get_Name().CmpNoCase("bldg:consistsOfBuildingPart") == 0 && GML[i].Get_Children_Count() == 1
		&&  GML[i][0].Get_Name().CmpNoCase("bldg:BuildingPart") == 0 )
		{
			CSG_MetaData	*pBuilding	= GML_Parts.Add_Child("core:cityObjectMember")->Add_Child(GML[i][0], false);

			pBuilding->Set_Name("bldg:Building");
			pBuilding->Add_Children(head);
			pBuilding->Add_Children(GML[i][0]);
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
