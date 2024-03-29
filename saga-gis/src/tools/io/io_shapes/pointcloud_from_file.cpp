
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       io_shapes                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                pointcloud_from_file.cpp               //
//                                                       //
//                 Copyright (C) 2009 by                 //
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
#include "pointcloud_from_file.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CPointCloud_From_File::CPointCloud_From_File(void)
{
	Set_Name		(_TL("Import Point Cloud from Shape File"));

	Set_Author		("O. Conrad (c) 2009");

	Set_Description	(_TW(
		"Imports a point cloud from a point shapefile."
	));

	//-----------------------------------------------------
	Parameters.Add_PointCloud("",
		"POINTS", _TL("Point Cloud"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_FilePath("",
		"FILE"  , _TL("File"),
		_TL(""),
		CSG_String::Format("%s|*.shp|%s|*.*",
			_TL("ESRI Shapefiles"),
			_TL("All Files")
		)
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CPointCloud_From_File::On_Execute(void)
{
	CSG_String fName(Parameters("FILE")->asString());

	if( Read_Shapefile(fName) )
	{
		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CPointCloud_From_File::Read_Shapefile(const CSG_String &fName)
{
	CSG_Shapes Shapes;

	if( !Shapes.Create(fName) || Shapes.Get_Count() == 0 )
	{
		Error_Fmt("%s\n\"%s\"", _TL("failed to load file!"), fName.c_str());

		return( false );
	}

	if( Shapes.Get_Type() != SHAPE_TYPE_Point )
	{
		Error_Fmt("%s\n\"%s\"", _TL("unsupported geometry type! should be point!"), fName.c_str());

		return( false );
	}

	//-----------------------------------------------------
	CSG_PointCloud *pPoints = Parameters("POINTS")->asPointCloud();
	pPoints->Create();
	pPoints->Set_Name(SG_File_Get_Name(fName, false));

	for(int iField=0; iField<Shapes.Get_Field_Count(); iField++)
	{
		pPoints->Add_Field(Shapes.Get_Field_Name(iField), SG_DATATYPE_Double);
	}

	for(sLong iPoint=0; iPoint<Shapes.Get_Count() && Set_Progress(iPoint, Shapes.Get_Count()); iPoint++)
	{
		CSG_Shape *pPoint = Shapes.Get_Shape(iPoint);

		pPoints->Add_Point(pPoint->Get_Point().x, pPoint->Get_Point().y, 0.);

		for(int iField=0; iField<Shapes.Get_Field_Count(); iField++)
		{
			pPoints->Set_Value(3 + iField, pPoint->asDouble(iField));
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
