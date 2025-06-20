
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     image_tools                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//               sentinel_2_scene_import.cpp             //
//                                                       //
//                 Olaf Conrad (C) 2019                  //
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
#include "sentinel_2_scene_import.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum EBand_Head
{
	INFO_FIELD_ID = 0,
	INFO_FIELD_BAND,
	INFO_FIELD_NAME,
	INFO_FIELD_RES,
	INFO_FIELD_WAVE_MIN,
	INFO_FIELD_WAVE,
	INFO_FIELD_WAVE_MAX,
	INFO_FIELD_COUNT
};

//---------------------------------------------------------
CSG_Table CSentinel_2_Scene_Import::Get_Info_Bands(void)
{
	CSG_Table Info_Bands;

	Info_Bands.Add_Field("ID"        , SG_DATATYPE_Int   );
	Info_Bands.Add_Field("BAND"      , SG_DATATYPE_String);
	Info_Bands.Add_Field("NAME"      , SG_DATATYPE_String);
	Info_Bands.Add_Field("RESOLUTION", SG_DATATYPE_Int   );
	Info_Bands.Add_Field("WAVE_MIN"  , SG_DATATYPE_Double);
	Info_Bands.Add_Field("WAVE"      , SG_DATATYPE_Double);
	Info_Bands.Add_Field("WAVE_MAX"  , SG_DATATYPE_Double);

	#define ADD_INFO_BAND(band, name, res, wmin, wave, wmax) { CSG_Table_Record &Info = *Info_Bands.Add_Record();\
		Info.Set_Value(INFO_FIELD_ID  , 1 + (int)Info.Get_Index());\
		Info.Set_Value(INFO_FIELD_BAND, band);\
		Info.Set_Value(INFO_FIELD_NAME, CSG_String::Format("[%s] %s", SG_T(band), name));\
		Info.Set_Value(INFO_FIELD_RES , res);\
		Info.Set_Value(INFO_FIELD_WAVE_MIN, wmin);\
		Info.Set_Value(INFO_FIELD_WAVE    , wave);\
		Info.Set_Value(INFO_FIELD_WAVE_MAX, wmax);\
	}

	ADD_INFO_BAND("B01", _TL("Aerosols"                 ), 60,  412.0,  442.7,  456.0);	//  1, 60m
	ADD_INFO_BAND("B02", _TL("Blue"                     ), 10,  456.0,  492.7,  533.0);	//  2, 10m
	ADD_INFO_BAND("B03", _TL("Green"                    ), 10,  538.0,  559.8,  583.0);	//  3, 10m
	ADD_INFO_BAND("B04", _TL("Red"                      ), 10,  646.0,  664.6,  684.0);	//  4, 10m
	ADD_INFO_BAND("B05", _TL("Red Edge"                 ), 20,  695.0,  704.1,  714.0);	//  5, 20m
	ADD_INFO_BAND("B06", _TL("Red Edge"                 ), 20,  731.0,  740.5,  749.0);	//  6, 20m
	ADD_INFO_BAND("B07", _TL("Red Edge"                 ), 20,  769.0,  782.8,  797.0);	//  7, 20m
	ADD_INFO_BAND("B08", _TL("NIR"                      ), 10,  760.0,  832.8,  907.0);	//  8, 10m
	ADD_INFO_BAND("B8A", _TL("NIR"                      ), 20,  837.0,  864.7,  881.0);	//  9, 20m
	ADD_INFO_BAND("B09", _TL("Water Vapour"             ), 60,  932.0,  945.1,  958.0);	// 10, 60m
	ADD_INFO_BAND("B10", _TL("Cirrus"                   ), 60, 1337.0, 1373.5, 1412.0);	// 11, 60m
	ADD_INFO_BAND("B11", _TL("SWIR"                     ), 20, 1539.0, 1613.7, 1682.0);	// 12, 20m
	ADD_INFO_BAND("B12", _TL("SWIR"                     ), 20, 2078.0, 2202.4, 2320.0);	// 13, 20m
	ADD_INFO_BAND("TCI", _TL("True Color Image"         ), 10,    0.0,    0.0,    0.0);	// 14, 10m
	ADD_INFO_BAND("AOT", _TL("Aerosol Optical Thickness"), 10,    0.0,    0.0,    0.0);	// 15, 10m
	ADD_INFO_BAND("WVP", _TL("Water Vapour"             ), 10,    0.0,    0.0,    0.0);	// 16, 10m
	ADD_INFO_BAND("SCL", _TL("Scene Classification"     ), 20,    0.0,    0.0,    0.0);	// 17, 20m

	return( Info_Bands );
}

//---------------------------------------------------------
#define BAND_IS_10m(b)	(b+1 ==  2 || b+1 ==  3 || b+1 ==  4 || b+1 ==  8)
#define BAND_IS_20m(b)	(b+1 ==  5 || b+1 ==  6 || b+1 ==  7 || b+1 ==  9 || b+1 == 12 || b+1 == 13)
#define BAND_IS_60m(b)	(b+1 ==  1 || b+1 == 10 || b+1 == 11)
#define BAND_IS_TCI(b)	(b+1 == 14)
#define BAND_IS_AOT(b)	(b+1 == 15)
#define BAND_IS_WVP(b)	(b+1 == 16)
#define BAND_IS_SCL(b)	(b+1 == 17)


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSentinel_2_Scene_Import::CSentinel_2_Scene_Import(void)
{
	Set_Name		(_TL("Import Sentinel-2 Scene"));

	Set_Author		("O.Conrad (c) 2019");

	Set_Description	(_TW(
		"Import Sentinel-2 scenes from a folder structure as provided "
		"by the original ESA download. "
	));

	Add_Reference("https://sentinel.esa.int/web/sentinel/missions/sentinel-2",
		SG_T("Sentinel-2 at ESA's Sentinel Online")
	);

	Add_Reference("https://sentinel.esa.int/web/sentinel/document-library/content/-/article/sentinel-2-user-handbook",
		SG_T("Sentinel-2 User Handbook")
	);

	//-----------------------------------------------------
	Parameters.Add_FilePath("",
		"METAFILE"		, _TL("Metadata File"),
		_TL(""),
		CSG_String::Format("%s|MTD*.xml|%s|*.*",
			_TL("Sentinel-2 Metadata Files"),
			_TL("All Files")
		)
	);

	Parameters.Add_Grid_List("", "BANDS"    , _TL("Spectral Bands" ), _TL(""), PARAMETER_OUTPUT);
	Parameters.Add_Grid_List("", "BANDS_AUX", _TL("Auxiliary Bands"), _TL(""), PARAMETER_OUTPUT);

	Parameters.Add_Bool("",
		"MULTI2GRIDS"	, _TL("Spectral Bands as Grid Collection"),
		_TL(""),
		true
	);

	Parameters.Add_Bool("", "LOAD_60M", _TL("Aerosol, Vapour, Cirrus"  ), _TL(""), false);
//	Parameters.Add_Bool("", "LOAD_TCI", _TL("True Color Image"         ), _TL(""), false);
	Parameters.Add_Bool("", "LOAD_AOT", _TL("Aerosol Optical Thickness"), _TL(""), false);
	Parameters.Add_Bool("", "LOAD_WVP", _TL("Water Vapour"             ), _TL(""), false);
	Parameters.Add_Bool("", "LOAD_SCL", _TL("Scene Classification"     ), _TL(""), false);

	Parameters.Add_Choice("",
		"REFLECTANCE"	, _TL("Reflectance Values"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("digital number"),
			_TL("fraction")
		), 1
	);

	Parameters.Add_Choice("",
		"RESOLUTION"	, _TL("Resolution"),
		_TL("Allows to resample all spectral bands to the same target resolution, either 10 or 20m."),
		CSG_String::Format("%s|%s|%s",
			_TL("original"), SG_T("10m"), SG_T("20m")
		), 0
	);

	Parameters.Add_Choice("",
		"PROJECTION"	, _TL("Coordinate System"),
		_TL("If using the extent option in combination with \'Different UTM Zone\' or \'Geographic Coordinates\' extent is expected to be defined with UTM North coordinates."),
		CSG_String::Format("%s|%s|%s|%s|%s",
			_TL("original"),
			_TL("UTM North"),
			_TL("UTM South"),
			_TL("Different UTM Zone"),
			_TL("Geographic Coordinates")
		), 0
	);

	Parameters.Add_Choice("PROJECTION",
		"RESAMPLING"	, _TL("Resampling"),
		_TL(""),
		CSG_String::Format("%s|%s|%s|%s",
			_TL("Nearest Neighbour"),
			_TL("Bilinear Interpolation"),
			_TL("Bicubic Spline Interpolation"),
			_TL("B-Spline Interpolation")
		), 3
	);

	Parameters.Add_Int("PROJECTION",
		"UTM_ZONE"		, _TL("Zone"),
		_TL(""),
		32, 1, true, 60, true
	);

	Parameters.Add_Bool("PROJECTION",
		"UTM_SOUTH"		, _TL("South"),
		_TL(""),
		false
	);

	//-----------------------------------------------------
	Parameters.Add_Choice("",
		"EXTENT"		, _TL("Extent"),
		_TL(""),
		CSG_String::Format("%s|%s|%s|%s",
			_TL("original"),
			_TL("user defined"),
			_TL("grid system"),
			_TL("shapes extent")
		), 0
	);

	Parameters.Add_Double("EXTENT", "EXTENT_XMIN", _TL("Left"  ), _TL(""));
	Parameters.Add_Double("EXTENT", "EXTENT_XMAX", _TL("Right" ), _TL(""));
	Parameters.Add_Double("EXTENT", "EXTENT_YMIN", _TL("Bottom"), _TL(""));
	Parameters.Add_Double("EXTENT", "EXTENT_YMAX", _TL("Top"   ), _TL(""));

	Parameters.Add_Grid_System("EXTENT",
		"EXTENT_GRID"	, _TL("Grid System"),
		_TL("")
	);

	Parameters.Add_Shapes("EXTENT",
		"EXTENT_SHAPES"	, _TL("Shapes Extent"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Double("EXTENT",
		"EXTENT_BUFFER"	, _TL("Buffer"),
		_TL(""),
		0., 0., true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSentinel_2_Scene_Import::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("METAFILE") )
	{
		bool bLevel2 = SG_File_Exists  (pParameter->asString())
		            && SG_File_Get_Name(pParameter->asString(), false).Find("MTD_MSIL2") == 0;

		pParameters->Set_Enabled("LOAD_AOT", bLevel2);
		pParameters->Set_Enabled("LOAD_WVP", bLevel2);
		pParameters->Set_Enabled("LOAD_SCL", bLevel2);
	}

	if( pParameter->Cmp_Identifier("PROJECTION") )
	{
		pParameters->Set_Enabled("RESAMPLING", pParameter->asInt() == 3 || pParameter->asInt() == 4);
		pParameters->Set_Enabled("UTM_ZONE"  , pParameter->asInt() == 3);
		pParameters->Set_Enabled("UTM_SOUTH" , pParameter->asInt() == 3);
		pParameters->Set_Enabled("RESOLUTION", pParameter->asInt() != 3 && pParameter->asInt() != 4);
	}

	if(	pParameter->Cmp_Identifier("EXTENT") )
	{
		pParameters->Set_Enabled("EXTENT_XMIN"  , pParameter->asInt() == 1);
		pParameters->Set_Enabled("EXTENT_XMAX"  , pParameter->asInt() == 1);
		pParameters->Set_Enabled("EXTENT_YMIN"  , pParameter->asInt() == 1);
		pParameters->Set_Enabled("EXTENT_YMAX"  , pParameter->asInt() == 1);
		pParameters->Set_Enabled("EXTENT_GRID"  , pParameter->asInt() == 2);
		pParameters->Set_Enabled("EXTENT_SHAPES", pParameter->asInt() == 3);
		pParameters->Set_Enabled("EXTENT_BUFFER", pParameter->asInt() >= 2);
	}

	return( CSG_Tool::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSentinel_2_Scene_Import::On_Execute(void)
{
	CSG_MetaData Info_General, Info_Image, Info_Granule;

	if( !Load_Metadata(Parameters("METAFILE")->asString(), Info_General, Info_Granule, Info_Image) )
	{
		Error_Fmt("%s [%s]", _TL("failed to load metadata"), Parameters("METAFILE")->asString());

		return( false );
	}

	CSG_String Date = Info_General["PRODUCT_START_TIME"].Get_Content().BeforeFirst('T');

	//-----------------------------------------------------
	CSG_String Path = SG_File_Get_Path(Parameters("METAFILE")->asString());

	bool bLevel2  = SG_File_Get_Name(Parameters("METAFILE")->asString(), false).Find("MTD_MSIL2") == 0;

	bool bLoadTCI = Parameters("LOAD_TCI") && Parameters("LOAD_TCI")->asBool();
	bool bLoadAOT = Parameters("LOAD_AOT") && Parameters("LOAD_AOT")->asBool() && bLevel2;
	bool bLoadWVP = Parameters("LOAD_WVP") && Parameters("LOAD_WVP")->asBool() && bLevel2;
	bool bLoadSCL = Parameters("LOAD_SCL") && Parameters("LOAD_SCL")->asBool() && bLevel2;
	bool bLoad60m = Parameters("LOAD_60M") && Parameters("LOAD_60M")->asBool();

	int Resolution = Parameters("PROJECTION")->asInt() != 3 && Parameters("PROJECTION")->asInt() != 4 ? Parameters("RESOLUTION")->asInt() : 0;

	bool bMultiGrids = Parameters("MULTI2GRIDS")->asBool();

	CSG_Table Info_Bands(Get_Info_Bands());

	//-----------------------------------------------------
	Parameters("BANDS"    )->asGridList()->Del_Items();
	Parameters("BANDS_AUX")->asGridList()->Del_Items();

	CSG_Grids *pBands[2]; pBands[0] = pBands[1] = NULL; CSG_Grid *pSCL = NULL;

	for(int Band=0; Band<Info_Bands.Get_Count() && Process_Get_Okay(); Band++)
	{
		if( (!bLoadTCI && BAND_IS_TCI(Band))
		||  (!bLoadAOT && BAND_IS_AOT(Band))
		||  (!bLoadWVP && BAND_IS_WVP(Band))
		||  (!bLoadSCL && BAND_IS_SCL(Band))
		||  (!bLoad60m && BAND_IS_60m(Band)) )
		{
			continue;
		}

		CSG_Grid *pBand = Load_Band(Path, Find_Band(Info_Bands[Band], Info_Granule));

		if( !pBand )
		{
			continue;
		}

		if( Resolution && (BAND_IS_10m(Band) || BAND_IS_20m(Band)) )
		{
			Parameters("BANDS")->asGridList()->Update_Data(); CSG_Grid *pGrid = Parameters("BANDS")->asGridList()->Get_Grid(0);

			CSG_Grid_System System; if( pGrid ) System = pGrid->Get_System(); else System.Create(Resolution == 1 ? 10. : 20., pBand->Get_Extent());

			if( !System.is_Equal(pBand->Get_System()) )
			{
				SG_UI_Msg_Lock(true);
				pGrid = pBand; pBand = SG_Create_Grid(System, pBand->Get_Type()); pBand->Assign(pGrid); delete(pGrid);
				SG_UI_Msg_Lock(false);
			}
		}

		pBand->Get_MetaData().Add_Child(Info_General)->Set_Name("SENTINEL-2");
		pBand->Set_Description(Info_General.asText());

		if( bMultiGrids && (BAND_IS_10m(Band) || BAND_IS_20m(Band)) )
		{
			int b = pBand->Get_Cellsize() == 10. ? 0 : 1;

			if( pBands[b] == NULL )
			{
				if( (pBands[b] = SG_Create_Grids(pBand->Get_System(), Info_Bands)) == NULL )
				{
					Error_Set(_TL("memory allocation failed"));

					return( false );
				}

				Parameters("BANDS")->asGridList()->Add_Item(pBands[b]);
			}

			pBands[b]->Add_Grid(Info_Bands[Band], pBand, true);
		}
		else
		{
			pBand->Fmt_Name("S2_%s_%s", Date.c_str(), Info_Bands[Band].asString(INFO_FIELD_BAND));

			if( BAND_IS_10m(Band) || BAND_IS_20m(Band) || BAND_IS_60m(Band) )
			{
				double Scaling, Offset; Get_Scaling(Info_Image, Band, Scaling, Offset);

				pBand->Set_Scaling(Scaling, Offset);
			}

			if( BAND_IS_SCL(Band) )
			{
				pSCL = pBand;
			}

			if( BAND_IS_10m(Band) || BAND_IS_20m(Band) )
			{
				Parameters("BANDS"    )->asGridList()->Add_Item(pBand);
			}
			else
			{
				Parameters("BANDS_AUX")->asGridList()->Add_Item(pBand);
			}
		}
	}

	//-----------------------------------------------------
	for(int i=0; i<2; i++)
	{
		double Scaling, Offset; Get_Scaling(Info_Image, 0, Scaling, Offset);

		if( pBands[i] )
		{
			pBands[i]->Fmt_Name("S2_%s_%dm", Date.c_str(), i == 0 ? 10 : 20);
			pBands[i]->Get_MetaData().Add_Child(Info_General)->Set_Name("SENTINEL-2");
			pBands[i]->Get_MetaData().Add_Child(Info_Image  )->Set_Name("BANDS");
			pBands[i]->Set_Description(Info_General.asText());
			pBands[i]->Set_Z_Attribute (INFO_FIELD_WAVE, true);
			pBands[i]->Set_Scaling(Scaling, Offset);

			DataObject_Add(pBands[i]);
		}
	}

	if( pSCL )
	{
		Load_Classification(pSCL, Parameters("METAFILE")->asString());
	}

	return( Parameters("BANDS")->asGridList()->Get_Grid_Count() > 0 );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSentinel_2_Scene_Import::Load_Metadata(const CSG_String &File, CSG_MetaData &General, CSG_MetaData &Granule, CSG_MetaData &Image)
{
	CSG_MetaData Metadata;

	if( !Metadata.Load(File)
	||  !Metadata("n1:General_Info")
	||  !Metadata["n1:General_Info"]("Product_Info")
	||  !Metadata["n1:General_Info"]["Product_Info"]("Product_Organisation")
	||  !Metadata["n1:General_Info"]["Product_Info"]["Product_Organisation"]("Granule_List")
	||  !Metadata["n1:General_Info"]["Product_Info"]["Product_Organisation"]["Granule_List"]("Granule") )
	{
		return( false );
	}

	Granule = Metadata["n1:General_Info"]["Product_Info"]["Product_Organisation"]["Granule_List"]["Granule"];
	General = Metadata["n1:General_Info"]["Product_Info"];

	General.Del_Child("Product_Organisation");

	if( Metadata["n1:General_Info"]("Product_Image_Characteristics") )
	{
		Image = Metadata["n1:General_Info"]["Product_Image_Characteristics"];
	}

	CSG_String _File(Granule.Get_Content(0)); _File = SG_File_Get_Path(File) + "/GRANULE/" + _File.AfterFirst('/').BeforeFirst('/') + "/MTD_TL.xml";

	if( Metadata.Load(_File)
	&&  Metadata("n1:Geometric_Info")
	&&  Metadata["n1:Geometric_Info"]("Tile_Angles")
	&&  Metadata["n1:Geometric_Info"]["Tile_Angles"]("Mean_Sun_Angle") )
	{
		double Azimuth, Zenith;

		if( Metadata["n1:Geometric_Info"]["Tile_Angles"]["Mean_Sun_Angle"].Get_Content("AZIMUTH_ANGLE", Azimuth)
		&&  Metadata["n1:Geometric_Info"]["Tile_Angles"]["Mean_Sun_Angle"].Get_Content( "ZENITH_ANGLE", Zenith ) )
		{
			General.Add_Child("SUN_AZIMUTH",      Azimuth);
			General.Add_Child("SUN_HEIGHT" , 90. - Zenith);
		}
	}

	return( true );
}

//---------------------------------------------------------
bool CSentinel_2_Scene_Import::Get_Scaling(CSG_MetaData &Image, int Band, double &Scaling, double &Offset)
{
	Scaling = 1.; Offset = 0.;

	if( Parameters("REFLECTANCE")->asInt() == 1 )
	{
		if( Image.Get_Content("QUANTIFICATION_VALUE", Scaling) && Scaling != 0. )
		{
			Scaling = 1. / Scaling;
		}
		else if( Image("QUANTIFICATION_VALUES_LIST") && Image["QUANTIFICATION_VALUES_LIST"]("BOA_QUANTIFICATION_VALUE")
		&&       Image["QUANTIFICATION_VALUES_LIST"].Get_Content("BOA_QUANTIFICATION_VALUE", Scaling) && Scaling != 0. )
		{
			Scaling = 1. / Scaling;
		}
		else
		{
			Scaling = 1. / 10000.;
		}
	}

	if( Image("Radiometric_Offset_List") && Image["Radiometric_Offset_List"].Get_Children_Count() == 13
	&&  Image["Radiometric_Offset_List"][Band].Get_Content().asDouble(Offset) )
	{
		Offset *= Scaling;
	}
	else if( Image("BOA_ADD_OFFSET_VALUES_LIST") && Image["BOA_ADD_OFFSET_VALUES_LIST"].Get_Children_Count() == 13
	&&       Image["BOA_ADD_OFFSET_VALUES_LIST"][Band].Get_Content().asDouble(Offset) )
	{
		Offset *= Scaling;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSentinel_2_Scene_Import::Find_Band(const CSG_Table_Record &Band, const CSG_MetaData &Granule)
{
	CSG_String IDold(Band.asString(1)), IDnew(CSG_String::Format("%s_%dm", Band.asString(1), Band.asInt(3)));

	for(int i=0; i<Granule.Get_Children_Count(); i++)
	{
		if(  Granule[i].Cmp_Name("IMAGE_FILE")
		&& (!Granule[i].Get_Content().Right(IDnew.Length()).Cmp(IDnew)
		||  !Granule[i].Get_Content().Right(IDold.Length()).Cmp(IDold)) )
		{
			return( Granule[i].Get_Content() );
		}
	}

	return( "" );
}

//---------------------------------------------------------
CSG_Grid * CSentinel_2_Scene_Import::Load_Band(const CSG_String &Path, const CSG_String &File)
{
	if( File.is_Empty() )
	{
		return( NULL ); // metadata provides no file name for requested band!
	}

	Process_Set_Text("%s: %s", _TL("loading"), File.AfterLast('/').c_str());

	//-----------------------------------------------------
	CSG_String _File(Path + "/" + File + ".jp2");

	#ifdef _SAGA_MSW
	_File.Replace("/", "\\");
	#endif

	SG_UI_Msg_Lock(true);
	CSG_Grid *pBand = Load_Grid(_File);
	SG_UI_Msg_Lock(false);

	if( !pBand )
	{
		CSG_String Message(CSG_String::Format("%s: \"%s\"", _TL("failed to load band"), File.c_str()));

		Message_Add("\n" + Message, false); SG_UI_Msg_Add_Error(Message);

		return( NULL );
	}

	pBand->Set_NoData_Value(0);

	if( !pBand->Get_Projection().is_Okay() )
	{
		// undefined coordinate system, nothing that further can be done...
	}

	//-----------------------------------------------------
	else if( Parameters("PROJECTION")->asInt() == 1 || Parameters("PROJECTION")->asInt() == 2 ) // UTM North/South
	{
		CSG_Projection Target, Source = pBand->Get_Projection();

		if( Source.Get_PROJ().Find("+proj=utm") >= 0 && Source.Get_PROJ().Find("+zone") >= 0 )
		{
			CSG_String Zone = Source.Get_PROJ().Right(Source.Get_PROJ().Length() - Source.Get_PROJ().Find("+zone")).AfterFirst('=');

			if( Target.Set_UTM_WGS84(Zone.asInt(), Parameters("PROJECTION")->asInt() == 2) && Target != Source
			&& ((Parameters("PROJECTION")->asInt() == 1 && Source.Get_PROJ().Find("+south") >= 0)
			 || (Parameters("PROJECTION")->asInt() == 2 && Source.Get_PROJ().Find("+south") <  0) ) )
			{
				CSG_Grid *pTmp = pBand; pBand = SG_Create_Grid(pTmp->Get_Type(), pTmp->Get_NX(), pTmp->Get_NY(), pTmp->Get_Cellsize(), pTmp->Get_XMin(),
					pTmp->Get_YMin() + (Parameters("PROJECTION")->asInt() == 1 ? -10000000 : 10000000)
				);

				if( pBand )
				{
					pBand->Get_Projection().Create(Target);
					pBand->Set_Name              (pTmp->Get_Name());
					pBand->Set_Description       (pTmp->Get_Description());
					pBand->Set_NoData_Value_Range(pTmp->Get_NoData_Value(), pTmp->Get_NoData_Value(true));
					pBand->Set_Scaling           (pTmp->Get_Scaling(), pTmp->Get_Offset());

					#pragma omp parallel for
					for(int y=0; y<pBand->Get_NY(); y++) for(int x=0; x<pBand->Get_NX(); x++)
					{
						pBand->Set_Value(x, y, pTmp->asDouble(x, y));
					}

					delete(pTmp);
				}
			}
		}
	}

	//-----------------------------------------------------
	else if( Parameters("PROJECTION")->asInt() == 3 )	// Different UTM Zone
	{
		CSG_Projection Projection = CSG_Projection::Get_UTM_WGS84(
			Parameters("UTM_ZONE" )->asInt (),
			Parameters("UTM_SOUTH")->asBool()
		);

		if( !Projection.is_Equal(pBand->Get_Projection()) )
		{
			CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Create_Tool("pj_proj4", 4);	// Coordinate Transformation (Grid)

			if(	pTool )
			{
				Message_Fmt("\n%s (%s: %s >> %s)\n", _TL("re-projection to different UTM Zone"), _TL("original"), pBand->Get_Projection().Get_PROJ().c_str(), Projection.Get_PROJ().c_str());

				pTool->Set_Manager(NULL);

				if( pTool->Set_Parameter("CRS_WKT"         , Projection.Get_WKT2())
				&&  pTool->Set_Parameter("CRS_PROJ"        , Projection.Get_PROJ())
				&&  pTool->Set_Parameter("SOURCE"          , pBand)
				&&  pTool->Set_Parameter("RESAMPLING"      , Parameters("RESAMPLING"))
			//	&&  pTool->Set_Parameter("DATA_TYPE"       , 10) // "Preserve" => is already default!
				&&  pTool->Set_Parameter("TARGET_USER_SIZE", pBand->Get_Cellsize())
				&&  pTool->Execute() )
				{
					delete(pBand);

					pBand = pTool->Get_Parameters()->Get_Parameter("GRID")->asGrid();
				}

				SG_Get_Tool_Library_Manager().Delete_Tool(pTool);
			}
		}
	}

	//-----------------------------------------------------
	else if( Parameters("PROJECTION")->asInt() == 4 )	// Geographic Coordinates
	{
		CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Create_Tool("pj_proj4", 4);	// Coordinate Transformation (Grid)

		if(	pTool )
		{
			Message_Fmt("\n%s (%s: %s)\n", _TL("re-projection to geographic coordinates"), _TL("original"), pBand->Get_Projection().Get_Name().c_str());

			pTool->Set_Manager(NULL);

			if( pTool->Set_Parameter("CRS_WKT"   , CSG_Projection::Get_GCS_WGS84().Get_WKT2())
			&&  pTool->Set_Parameter("CRS_PROJ"  , CSG_Projection::Get_GCS_WGS84().Get_PROJ())
			&&  pTool->Set_Parameter("SOURCE"    , pBand)
			&&  pTool->Set_Parameter("RESAMPLING", Parameters("RESAMPLING"))
		//	&&  pTool->Set_Parameter("DATA_TYPE" , 10) // "Preserve" => is already default!
			&&  pTool->Execute() )
			{
				delete(pBand);

				pBand = pTool->Get_Parameters()->Get_Parameter("GRID")->asGrid();
			}

			SG_Get_Tool_Library_Manager().Delete_Tool(pTool);
		}
	}

	//-----------------------------------------------------
	pBand->Set_File_Name("");

	return( pBand );
}

//---------------------------------------------------------
CSG_Grid * CSentinel_2_Scene_Import::Load_Grid(const CSG_String &File)
{
	CSG_Rect Extent;

	switch( Parameters("EXTENT")->asInt() )
	{
	default: // original
		return( SG_Create_Grid(File) );

	case  1: // user defined
		Extent.Assign(
			Parameters("EXTENT_XMIN")->asDouble(),
			Parameters("EXTENT_YMIN")->asDouble(),
			Parameters("EXTENT_XMAX")->asDouble(),
			Parameters("EXTENT_YMAX")->asDouble()
		);
		break;

	case  2: // grid system
		Extent = Parameters("EXTENT_GRID"  )->asGrid_System()->Get_Extent();
		Extent.Inflate(Parameters("EXTENT_BUFFER")->asDouble(), false);
		break;

	case  3: // shapes extent
		Extent = Parameters("EXTENT_SHAPES")->asShapes     ()->Get_Extent();
		Extent.Inflate(Parameters("EXTENT_BUFFER")->asDouble(), false);
		break;
	}

	//-----------------------------------------------------
	CSG_Grid *pGrid = NULL; CSG_Tool *pTool = SG_Get_Tool_Library_Manager().Create_Tool("io_gdal", 0);	// Import Raster

	if( pTool && pTool->Set_Manager(NULL)
	&&  pTool->Set_Parameter("FILES"      , File)
	&&	pTool->Set_Parameter("EXTENT"     , 1)
	&&	pTool->Set_Parameter("EXTENT_XMIN", Extent.Get_XMin())
	&&	pTool->Set_Parameter("EXTENT_XMAX", Extent.Get_XMax())
	&&	pTool->Set_Parameter("EXTENT_YMIN", Extent.Get_YMin())
	&&	pTool->Set_Parameter("EXTENT_YMAX", Extent.Get_YMax())
	&&  pTool->Execute() )
	{
		pGrid = pTool->Get_Parameter("GRIDS")->asGridList()->Get_Grid(0);
	}

	SG_Get_Tool_Library_Manager().Delete_Tool(pTool);

	return( pGrid );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSentinel_2_Scene_Import::Load_Classification(CSG_Grid *pGrid, const CSG_String &File)
{
	const int Colors[] = {
		SG_GET_RGB(255, 255, 255), //  0 NODATA
		SG_GET_RGB(255,   0,   0), //  1 SATURATED DEFECTIVE
		SG_GET_RGB(128,   0,   0), //  2 DARK FEATURE SHADOW
		SG_GET_RGB(162,   0, 162), //  3 CLOUD SHADOW
		SG_GET_RGB(  0, 128,   0), //  4 VEGETATION
		SG_GET_RGB(255, 255,   0), //  5 NOT VEGETATED
		SG_GET_RGB(  0,   0, 255), //  6 WATER
		SG_GET_RGB(222, 222, 222), //  7 UNCLASSIFIED
		SG_GET_RGB(255,   0, 255), //  8 CLOUD MEDIUM PROBA
		SG_GET_RGB(228,   0, 192), //  9 CLOUD HIGH PROBA
		SG_GET_RGB(255, 192, 255), // 10 THIN CIRRUS
		SG_GET_RGB(128, 255, 255)  // 11 SNOW ICE
	};

	//-----------------------------------------------------
	CSG_MetaData Metadata;

	if( !Metadata.Load(File)
	||  !Metadata("n1:General_Info")
	||  !Metadata["n1:General_Info"]("Product_Image_Characteristics")
	||  !Metadata["n1:General_Info"]["Product_Image_Characteristics"]("Scene_Classification_List") )
	{
		return( false );
	}

	CSG_MetaData ClassList = Metadata["n1:General_Info"]["Product_Image_Characteristics"]["Scene_Classification_List"];

	//-----------------------------------------------------
	CSG_Table LUT;

	LUT.Add_Field("Color"      , SG_DATATYPE_Color );
	LUT.Add_Field("Name"       , SG_DATATYPE_String);
	LUT.Add_Field("Description", SG_DATATYPE_String);
	LUT.Add_Field("Minimum"    , SG_DATATYPE_Double);
	LUT.Add_Field("Maximum"    , SG_DATATYPE_Double);

	for(int i=0, Index; i<ClassList.Get_Children_Count(); i++)
	{
		if( ClassList[i].Cmp_Name("Scene_Classification_ID")
		&&  ClassList[i]("SCENE_CLASSIFICATION_TEXT" )
		&&  ClassList[i].Get_Content("SCENE_CLASSIFICATION_INDEX", Index) )
		{
			CSG_String Name(ClassList[i]["SCENE_CLASSIFICATION_TEXT"].Get_Content().AfterFirst('_')); Name.Replace("_", " ");

			CSG_Table_Record &Class = *LUT.Add_Record();

			Class.Set_Value(0, Colors[i % 12]);
			Class.Set_Value(1, Name);
			Class.Set_Value(3, Index);
			Class.Set_Value(4, Index);
		}
	}

	if( LUT.Get_Count() > 0 )
	{
		DataObject_Add(pGrid);

		CSG_Parameter *pLUT = DataObject_Get_Parameter(pGrid, "LUT");

		if( pLUT && pLUT->asTable() && pLUT->asTable()->Assign_Values(&LUT) )
		{
			DataObject_Set_Parameter(pGrid, pLUT);
			DataObject_Set_Parameter(pGrid, "COLORS_TYPE", 1);	// Color Classification Type: Lookup Table

			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
