
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                        Grid_IO                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   ESRI_ArcInfo.cpp                    //
//                                                       //
//                 Copyright (C) 2007 by                 //
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
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "esri_arcinfo.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define HDR_NROWS			SG_T("NROWS")
#define HDR_NCOLS			SG_T("NCOLS")
#define HDR_X_CORNER		SG_T("XLLCORNER")
#define HDR_Y_CORNER		SG_T("YLLCORNER")
#define HDR_X_CENTER		SG_T("XLLCENTER")
#define HDR_Y_CENTER		SG_T("YLLCENTER")
#define HDR_CELLSIZE		SG_T("CELLSIZE")
#define HDR_NODATA			SG_T("NODATA_VALUE")
#define HDR_BYTEORDER		SG_T("BYTE_ORDER")
#define HDR_BYTEORDER_HI	SG_T("MSB_FIRST")
#define HDR_BYTEORDER_LO	SG_T("LSB_FIRST")


///////////////////////////////////////////////////////////
//														 //
//						Import							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CESRI_ArcInfo_Import::CESRI_ArcInfo_Import(void)
{
	Set_Name		(_TL("Import ESRI Arc/Info Grid"));

	Set_Author		("O.Conrad (c) 2007");

	Set_Description	(_TW(
		"Import grid from ESRI's Arc/Info grid format."
	));

	//-----------------------------------------------------
	Parameters.Add_Grid_Output("",
		"GRID"		, _TL("Grid"),
		_TL("")
	);

	Parameters.Add_FilePath("",
		"FILE"		, _TL("File"),
		_TL(""),
		CSG_String::Format("%s|*.asc;*.flt|%s|*.asc|%s|*.flt|%s|*.*",
			_TL("ESRI Arc/Info Grids"),
			_TL("ESRI Arc/Info ASCII Grids (*.asc)"),
			_TL("ESRI Arc/Info Binary Grids (*.flt)"),
			_TL("All Files")
		)
	);

	Parameters.Add_Node("",
		"NODE_ASCII", _TL("ASCII Grid Options"),
		_TL("")
	);

	Parameters.Add_Choice("NODE_ASCII",
		"GRID_TYPE"	, _TL("Target Grid Type"),
		_TL(""),
		CSG_String::Format("%s|%s|%s|%s",
			_TL("Integer (2 byte)"),
			_TL("Integer (4 byte)"),
			_TL("Floating Point (4 byte)"),
			_TL("Floating Point (8 byte)")
		), 2
	);

	Parameters.Add_Choice("NODE_ASCII",
		"NODATA"	, _TL("No-Data Value"),
		_TL("Choose whether the input file's NoData value or a user specified NoData value is written"),
		CSG_String::Format("%s|%s",
			_TL("Input File's NoData Value"),
			_TL("User Defined NoData Value")
		), 0
	);

	Parameters.Add_Double("NODE_ASCII",
		"NODATA_VAL", _TL("User Defined No-Data Value"),
		_TL(""),
		-99999.
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CESRI_ArcInfo_Import::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("NODATA") )
	{
		pParameters->Set_Enabled("NODATA_VAL", pParameter->asInt() == 1);
	}

	return( CSG_Tool::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CESRI_ArcInfo_Import::On_Execute(void)
{
	int		iNoData	= Parameters("NODATA"    )->asInt   ();
	double	dNoData	= Parameters("NODATA_VAL")->asDouble();

	TSG_Data_Type	Datatype;

	switch( Parameters("GRID_TYPE")->asInt() )
	{
	case  0: Datatype = SG_DATATYPE_Short ; break;
	case  1: Datatype = SG_DATATYPE_Int   ; break;
	default: Datatype = SG_DATATYPE_Float ; break;
	case  3: Datatype = SG_DATATYPE_Double; break;
	}

	CSG_Grid	*pGrid	= NULL;

	CSG_File	Stream;

	//-----------------------------------------------------
	// Binary...

	if( Stream.Open(SG_File_Make_Path("", Parameters("FILE")->asString(), "hdr"), SG_FILE_R, false) && (pGrid = Read_Header(Stream)) != NULL )
	{
		if( Stream.Open(SG_File_Make_Path("", Parameters("FILE")->asString(), "flt"), SG_FILE_R, true) )
		{
			float	*Line	= (float *)SG_Malloc(pGrid->Get_NX() * sizeof(float));

			for(int iy=0, y=pGrid->Get_NY()-1; iy<pGrid->Get_NY() && !Stream.is_EOF() && Set_Progress(iy, pGrid->Get_NY()); iy++, y--)
			{
				Stream.Read(Line, sizeof(float), pGrid->Get_NX());

				for(int x=0; x<pGrid->Get_NX(); x++)
				{
					pGrid->Set_Value(x, y, Line[x]);
				}
			}

			SG_Free(Line);
		}
	}

	//-----------------------------------------------------
	// ASCII...

	else if( Stream.Open(Parameters("FILE")->asString(), SG_FILE_R, false) && (pGrid = Read_Header(Stream, Datatype)) != NULL )
	{
		for(int iy=0, y=pGrid->Get_NY()-1; iy<pGrid->Get_NY() && !Stream.is_EOF() && Set_Progress(iy, pGrid->Get_NY()); iy++, y--)
		{
			for(int x=0; x<pGrid->Get_NX(); x++)
			{
				double	Value	= Read_Value(Stream);

				if( iNoData == 1 && Value == pGrid->Get_NoData_Value() )
				{
					Value	= dNoData;
				}

				pGrid->Set_Value(x, y, Value);
			}
		}

		if( iNoData == 1 )
		{
			pGrid->Set_NoData_Value(dNoData);
		}
	}

	//-----------------------------------------------------
	if( pGrid )
	{
		pGrid->Set_Name(SG_File_Get_Name(Parameters("FILE")->asString(), false));

		pGrid->Get_Projection().Load(SG_File_Make_Path("", Parameters("FILE")->asString(), "prj"));

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CESRI_ArcInfo_Import::Read_Value(CSG_File &Stream)
{
	CSG_String	s;	int	c;

	while( !Stream.is_EOF() && !SG_is_Character_Numeric(c = Stream.Read_Char()) );	// ignore leading white space...

	if( !Stream.is_EOF() && SG_is_Character_Numeric(c) )
	{
		do
		{
			if( c == ',' )
			{
				c	= '.';
			}

			s	+= (char)c;
		}
		while( !Stream.is_EOF() && SG_is_Character_Numeric(c = Stream.Read_Char()) );
	}

	return( s.asDouble() );
}

//---------------------------------------------------------
CSG_String CESRI_ArcInfo_Import::Read_Header_Line(CSG_File &Stream)
{
	CSG_String	s;	int	c;

	while( !Stream.is_EOF() && (c = Stream.Read_Char()) != 0x0A )
	{
		if( c != 0x0D )
		{
			s	+= (char)c;
		}
	}

	s.Make_Upper();	s.Replace(",", ".");

	return( s );
}

//---------------------------------------------------------
bool CESRI_ArcInfo_Import::Read_Header_Value(CSG_File &Stream, const CSG_String &sKey, int &Value)
{
	CSG_String	sLine(Read_Header_Line(Stream));

	if( sLine.Contains(sKey) )
	{
		CSG_String	sValue(sLine.c_str() + sKey.Length());

		return( sValue.asInt(Value) );
	}

	return( false );
}

//---------------------------------------------------------
bool CESRI_ArcInfo_Import::Read_Header_Value(CSG_File &Stream, const CSG_String &sKey, double &Value)
{
	CSG_String	sLine(Read_Header_Line(Stream));

	if( sLine.Contains(sKey) )
	{
		CSG_String	sValue(sLine.c_str() + sKey.Length());

		return( sValue.asDouble(Value) );
	}

	return( false );
}

//---------------------------------------------------------
CSG_Grid * CESRI_ArcInfo_Import::Read_Header(CSG_File &Stream, TSG_Data_Type Datatype)
{
	if( Stream.is_EOF() )
	{
		return( NULL );
	}

	//-----------------------------------------------------
	int NX; if( !Read_Header_Value(Stream, HDR_NCOLS, NX) ) { return( NULL ); }
	int NY; if( !Read_Header_Value(Stream, HDR_NROWS, NY) ) { return( NULL ); }

	//-------------------------------------------------
	double	xMin;	bool bCorner_X;

	if     ( Read_Header_Value(Stream, HDR_X_CORNER, xMin) ) { bCorner_X =  true; }
	else if( Read_Header_Value(Stream, HDR_X_CENTER, xMin) ) { bCorner_X = false; } else { return( NULL ); }

	double	yMin;	bool bCorner_Y;

	if     ( Read_Header_Value(Stream, HDR_Y_CORNER, yMin) ) { bCorner_Y =  true; }
	else if( Read_Header_Value(Stream, HDR_Y_CENTER, yMin) ) { bCorner_Y = false; } else { return( NULL ); }

	//-------------------------------------------------
	double Cellsize; if( !Read_Header_Value(Stream, HDR_CELLSIZE, Cellsize) ) { return( NULL ); }

	if( bCorner_X ) { xMin += Cellsize / 2.; }
	if( bCorner_Y ) { yMin += Cellsize / 2.; }

	//-------------------------------------------------
	double	NoData; Read_Header_Value(Stream, HDR_NODATA, NoData = -9999.);

	//-------------------------------------------------
	CSG_Grid *pGrid = Parameters("GRID")->asGrid();
	
	if( !pGrid )
	{
		Parameters("GRID")->Set_Value(pGrid = SG_Create_Grid());
	}

	pGrid->Create(Datatype, NX, NY, Cellsize, xMin, yMin);

	pGrid->Set_NoData_Value(NoData);

	return( pGrid );
}


///////////////////////////////////////////////////////////
//														 //
//						Export							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CESRI_ArcInfo_Export::CESRI_ArcInfo_Export(void)
{
	Set_Name		(_TL("Export ESRI Arc/Info Grid"));

	Set_Author		("O.Conrad (c) 2007");

	Set_Description	(_TW(
		"Export grid to ESRI's Arc/Info grid format."
	));

	//-----------------------------------------------------
	Parameters.Add_Grid("",
		"GRID"	, _TL("Grid"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_FilePath("",
		"FILE"	, _TL("File"),
		_TL(""),
		CSG_String::Format("%s|*.asc;*.flt|%s|*.asc|%s|*.flt|%s|*.*",
			_TL("ESRI Arc/Info Grids"),
			_TL("ESRI Arc/Info ASCII Grids (*.asc)"),
			_TL("ESRI Arc/Info Binary Grids (*.flt)"),
			_TL("All Files")
		), NULL, true
	);

	Parameters.Add_Choice("",
		"FORMAT"	, _TL("Format"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("binary"),
			_TL("ASCII")
		), 1
	);

	Parameters.Add_Choice("",
		"GEOREF"	, _TL("Geo-Reference"),
		_TL("The grids geo-reference must be related either to the center or the corner of its lower left grid cell."),
		CSG_String::Format("%s|%s",
			_TL("corner"),
			_TL("center")
		), 0
	);

	Parameters.Add_Int("",
		"PREC"		, _TL("ASCII Precision"),
		_TL("Number of decimals when writing floating point values in ASCII format."),
		4, -1, true
	);

	Parameters.Add_Choice("",
		"DECSEP"	, _TL("ASCII Decimal Separator"),
		_TL("Applies also to the binary format header file."),
		CSG_String::Format("%s|%s",
			_TL("point (.)"),
			_TL("comma (,)")
		), 0
	);

//	Parameters.Add_Choice("",
//		"BYTEORD"	, _TL("Binary Byte Order"),
//		_TL("Byte order when writing floating point values in binary format"),
//		CSG_String::Format("%s|%s",
//			_TL("most significant first"),
//			_TL("least significant first")
//		), 0
//	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CESRI_ArcInfo_Export::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("GRID") && pParameter->asGrid() )
	{
		CSG_String	Path(SG_File_Get_Path((*pParameters)["FILE"].asString()));

		pParameters->Set_Parameter("FILE", SG_File_Make_Path(Path, pParameter->asGrid()->Get_Name(),
			(*pParameters)["FORMAT"].asInt() == 0 ? "flt" : "asc"
		));
	}

	return( CSG_Tool_Grid::On_Parameter_Changed(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CESRI_ArcInfo_Export::On_Execute(void)
{
	CSG_File	Stream;

	CSG_Grid	*pGrid	= Parameters("GRID"  )->asGrid();
	CSG_String	File	= Parameters("FILE"  )->asString();
	int		Precision	= Parameters("PREC"  )->asInt();
	bool	bComma		= Parameters("DECSEP")->asInt() == 1;

	//-----------------------------------------------------
	// Binary...

	if( Parameters("FORMAT")->asInt() == 0 )
	{
		if( Stream.Open(SG_File_Make_Path("", File, "hdr"), SG_FILE_W, false) && Write_Header(Stream, pGrid, bComma)
		&&	Stream.Open(SG_File_Make_Path("", File, "flt"), SG_FILE_W, true) )
		{
			bool	bSwapBytes	= false;	//	bSwapBytes	= Parameters("BYTEORD")	->asInt() == 1;

			float	*Line	= (float *)SG_Malloc(pGrid->Get_NX() * sizeof(float));

			for(int iy=0, y=pGrid->Get_NY()-1; iy<pGrid->Get_NY() && Set_Progress(iy, pGrid->Get_NY()); iy++, y--)
			{
				for(int x=0; x<pGrid->Get_NX(); x++)
				{
					Line[x]	= pGrid->asFloat(x, y);

					if( bSwapBytes )
					{
						SG_Swap_Bytes(Line + x, sizeof(float));
					}
				}

				Stream.Write(Line, sizeof(float), pGrid->Get_NX());
			}

			SG_Free(Line);

			pGrid->Get_Projection().Save(SG_File_Make_Path("", File, "prj"));

			return( true );
		}
	}


	//-----------------------------------------------------
	// ASCII...

	else if( Stream.Open(File, SG_FILE_W, false) && Write_Header(Stream, pGrid, bComma) )
	{
		for(int iy=0, y=pGrid->Get_NY()-1; iy<pGrid->Get_NY() && Set_Progress(iy, pGrid->Get_NY()); iy++, y--)
		{
			for(int x=0; x<pGrid->Get_NX(); x++)
			{
				if( x > 0 )
				{
					Stream.Write(" ");
				}

				Stream.Write(Write_Value(pGrid->asDouble(x, y), Precision, bComma));
			}

			Stream.Write("\n");
		}

		pGrid->Get_Projection().Save(SG_File_Make_Path("", File, "prj"));

		return( true );
	}

	//-----------------------------------------------------
	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
inline CSG_String CESRI_ArcInfo_Export::Write_Value(double Value, int Precision, bool bComma)
{
	CSG_String	s;

	if( Precision < 0 )
	{
		s.Printf("%g"  , Value);
	}
	else if( Precision > 0 )
	{
		s.Printf("%.*f", Precision, Value);
	}
	else
	{
		s.Printf("%d"  , (int)(Value > 0. ? Value + 0.5 : Value - 0.5));
	}

	if( bComma )
	{
		s.Replace(".", ",");
	}
	else
	{
		s.Replace(",", ".");
	}

	return( s );
}

//---------------------------------------------------------
bool CESRI_ArcInfo_Export::Write_Header(CSG_File &Stream, CSG_Grid *pGrid, bool bComma)
{
	if( Stream.is_Open() && pGrid && pGrid->is_Valid() )
	{
		CSG_String	s;

		s	+= CSG_String::Format("%s %d\n", HDR_NCOLS, pGrid->Get_NX());
		s	+= CSG_String::Format("%s %d\n", HDR_NROWS, pGrid->Get_NY());

		if( Parameters("GEOREF")->asInt() == 0 )
		{
			s	+= CSG_String::Format("%s %s\n", HDR_X_CORNER, Write_Value(pGrid->Get_XMin() - 0.5 * pGrid->Get_Cellsize(), CSG_Grid_System::Get_Precision(), bComma).c_str());
			s	+= CSG_String::Format("%s %s\n", HDR_Y_CORNER, Write_Value(pGrid->Get_YMin() - 0.5 * pGrid->Get_Cellsize(), CSG_Grid_System::Get_Precision(), bComma).c_str());
		}
		else
		{
			s	+= CSG_String::Format("%s %s\n", HDR_X_CENTER, Write_Value(pGrid->Get_XMin(), CSG_Grid_System::Get_Precision(), bComma).c_str());
			s	+= CSG_String::Format("%s %s\n", HDR_Y_CENTER, Write_Value(pGrid->Get_YMin(), CSG_Grid_System::Get_Precision(), bComma).c_str());
		}

		s	+= CSG_String::Format("%s %s\n", HDR_CELLSIZE, Write_Value(pGrid->Get_Cellsize(), CSG_Grid_System::Get_Precision(), bComma).c_str());
		s	+= CSG_String::Format("%s %s\n", HDR_NODATA  , Write_Value(pGrid->Get_NoData_Value(), Parameters("PREC")->asInt(), bComma).c_str());

		if( Parameters("FORMAT")->asInt() == 0 )	// binary
		{	if( Parameters("BYTEORD") )
			s	+= CSG_String::Format("%s %s\n", HDR_BYTEORDER, Parameters("BYTEORD")->asInt() == 1 ? HDR_BYTEORDER_LO : HDR_BYTEORDER_HI);
		}

		Stream.Write(s);

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
