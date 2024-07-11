
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                                                       //
//                       io_gdal                         //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  ogr_export_kml.cpp                   //
//                                                       //
//            Copyright (C) 2012 O. Conrad               //
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
//    e-mail:     oconrad@saga-gis.de                    //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Bundesstr. 55                          //
//                D-20146 Hamburg                        //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "ogr_export_kml.h"

#include <cpl_string.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
COGR_Export_KML::COGR_Export_KML(void)
{
	Set_Name		(_TL("Export Shapes to KML"));

	Set_Author		("O.Conrad (c) 2012");

	Set_Description	(_TW(
		"This tool exports a vector layer to the Google Earth KML format using "
		"Frank Warmerdam's \"Geospatial Data Abstraction Library\" (GDAL/OGR). "
		"The output file is projected to geographic coordinates if necessary and possible. "
	));

	Add_Reference("GDAL/OGR contributors", "2019",
		"GDAL/OGR Geospatial Data Abstraction software Library",
		"A translator library for raster and vector geospatial data formats. Open Source Geospatial Foundation.",
		SG_T("https://gdal.org"), SG_T("Link")
	);

	//-----------------------------------------------------
	Parameters.Add_Shapes("",
		"SHAPES"	, _TL("Shapes"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_FilePath("",
		"FILE"		, _TL("File"),
		_TL(""),
		CSG_String::Format(
			"%s|*.kml;*.kml|%s|*.*",
			_TL("KML files (*.kml)"),
			_TL("All Files")
		), NULL, true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int COGR_Export_KML::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( has_GUI() && pParameter->Cmp_Identifier("SHAPES") && pParameter->asShapes() )
	{
		CSG_String	Path(SG_File_Get_Path((*pParameters)["FILE"].asString()));

		pParameters->Set_Parameter("FILE", SG_File_Make_Path(Path, pParameter->asShapes()->Get_Name(), "kml"));
	}

	return( CSG_Tool::On_Parameter_Changed(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool COGR_Export_KML::On_Execute(void)
{
	CSG_Shapes Shapes, *pShapes = Parameters("SHAPES")->asShapes();

	//-----------------------------------------------------
	if( pShapes->Get_Projection().Get_Type() == ESG_CRS_Type::Undefined )
	{
		Message_Add(_TL("layer uses undefined coordinate system, assuming geographic coordinates"));
	}
	else if( pShapes->Get_Projection().Get_Type() != ESG_CRS_Type::Geographic )
	{
		Message_Fmt("\n%s (%s: %s)\n", _TL("re-projection to geographic coordinates"), _TL("original"), pShapes->Get_Projection().Get_Name().c_str());

		bool bResult;

		SG_RUN_TOOL(bResult, "pj_proj4", 2,
			   SG_TOOL_PARAMETER_SET("SOURCE"    , pShapes)
			&& SG_TOOL_PARAMETER_SET("TARGET"    , &Shapes)
			&& SG_TOOL_PARAMETER_SET("CRS_STRING", CSG_Projection::Get_GCS_WGS84().Get_WKT())
		);

		if( bResult )
		{
			pShapes = &Shapes;

			Message_Fmt("\n%s: %s\n", _TL("re-projection"), _TL("success"));
		}
		else
		{
			Message_Fmt("\n%s: %s\n", _TL("re-projection"), _TL("failed" ));
		}
	}
	#if GDAL_VERSION_MAJOR == 3 && GDAL_VERSION_MINOR < 5
	else // if( pShapes->Get_Projection().Get_Type() == ESG_CRS_Type::Geographic )
	{
		#define WKT2_GCS_WGS84 "GEODCRS[\"WGS 84\","\
			"DATUM[\"World Geodetic System 1984\","\
				"ELLIPSOID[\"WGS 84\",6378137,298.257223563]],"\
			"CS[ellipsoidal,2],"\
				"AXIS[\"geodetic longitude (Lon)\",east],"\ // axis order 1st longitude, 2nd latitude
		"AXIS[\"geodetic latitude (Lat)\",north],"\
			"UNIT[\"degree\",0.0174532925199433],"\
			"ID[\"EPSG\",4326]]"

			pShapes->Get_Projection().Create(WKT2_GCS_WGS84);
	}
	#endif

	//-----------------------------------------------------
	CSG_OGR_DataSet DataSource;

	if( !DataSource.Create(Parameters("FILE")->asString(), "KML", "") )
	{
		Error_Set(_TL("KML file creation failed"));

		return( false );
	}

	if( !DataSource.Write(pShapes, "") )
	{
		Error_Set(_TL("failed to store data"));

		return( false );
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
