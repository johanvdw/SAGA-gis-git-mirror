
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                      Grid_Shapes                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//            Grid_Statistics_AddTo_Polygon.cpp          //
//                                                       //
//                 Copyright (C) 2003 by                 //
//                      Olaf Conrad                      //
//                                                       //
//                 quantile calculation:                 //
//                 Copyright (C) 2007 by                 //
//                   Johan Van de Wauw                   //
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
#include "Grid_Statistics_AddTo_Polygon.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_Statistics_AddTo_Polygon::CGrid_Statistics_AddTo_Polygon(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("Grid Statistics for Polygons"));

	Set_Author		("O.Conrad (c) 2003, Quantile Calculation (c) 2007 by Johan Van de Wauw");

	Set_Description	(_TW(
		"Zonal grid statistics. For each polygon statistics based on all covered grid cells will be calculated."
	));

	//-----------------------------------------------------
	Parameters.Add_Grid_List(
		"", "GRIDS"			, _TL("Grids"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Shapes(
		"", "POLYGONS"		, _TL("Polygons"),
		_TL(""),
		PARAMETER_INPUT, SHAPE_TYPE_Polygon
	);

	Parameters.Add_Choice(
		"", "NAMING"		, _TL("Field Naming"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("grid number"),
			_TL("grid name")
		), 1
	);

	Parameters.Add_Choice(
		"", "METHOD"		, _TL("Method"),
		_TL(""),
		CSG_String::Format("%s|%s|%s|%s",
			_TL("simple and fast"),
			_TL("polygon wise (cell centers)"),
			_TL("polygon wise (cell area)"),
			_TL("polygon wise (cell area weighted)")
		), 0
	);

	Parameters.Add_Bool(
		"", "PARALLELIZED"	, _TL("Use Multiple Cores"),
		_TL(""),
		false
	);

	//-----------------------------------------------------
	Parameters.Add_Shapes(
		"", "RESULT"		, _TL("Statistics"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL, SHAPE_TYPE_Polygon
	);

	Parameters.Add_Bool  ("RESULT", "COUNT"    , _TL("Number of Cells"   ), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "MIN"      , _TL("Minimum"           ), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "MAX"      , _TL("Maximum"           ), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "RANGE"    , _TL("Range"             ), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "SUM"      , _TL("Sum"               ), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "MEAN"     , _TL("Mean"              ), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "VAR"      , _TL("Variance"          ), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "STDDEV"   , _TL("Standard Deviation"), _TL(""),  true);
	Parameters.Add_Bool  ("RESULT", "GINI"     , _TL("Gini"              ), _TL(""), false);
	Parameters.Add_String("RESULT", "QUANTILES", _TL("Percentiles"       ),
		_TL("Separate the desired percentiles by semicolon, e.g. \"5; 25; 50; 75; 95\""),
		""
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CGrid_Statistics_AddTo_Polygon::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("METHOD") )
	{
		pParameters->Set_Enabled("PARALLELIZED", pParameter->asInt() != 0 && SG_OMP_Get_Max_Num_Threads() > 1);
	}

	return( CSG_Tool_Grid::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GET_FIELD_NAME(VAR)	Naming == 0 ? CSG_String::Format("G%02d_%s", iGrid + 1, VAR) : CSG_String::Format("%s (%s)", pGrids->Get_Grid(iGrid)->Get_Name(), VAR)

//---------------------------------------------------------
bool CGrid_Statistics_AddTo_Polygon::On_Execute(void)
{
	//-----------------------------------------------------
	CSG_Shapes	*pPolygons	= Parameters("POLYGONS")->asShapes();

	if( pPolygons->Get_Count() <= 0 )
	{
		Error_Set(_TL("no polygons in input layer"));

		return( false );
	}

	//-----------------------------------------------------
	CSG_Parameter_Grid_List	*pGrids	= Parameters("GRIDS")->asGridList();

	if( pGrids->Get_Grid_Count() < 1 )
	{
		Error_Set(_TL("no grids in selection"));

		return( false );
	}

	if( !Get_System().Get_Extent().Intersects(pPolygons->Get_Extent()) )
	{
		Error_Set(_TL("no spatial intersection between grid(s) and polygon layer"));

		return( false );
	}

	//-----------------------------------------------------
	int	nFields		= 0;

	int	fCOUNT		= Parameters("COUNT" )->asBool() ? nFields++ : -1;
	int	fMIN		= Parameters("MIN"   )->asBool() ? nFields++ : -1;
	int	fMAX		= Parameters("MAX"   )->asBool() ? nFields++ : -1;
	int	fRANGE		= Parameters("RANGE" )->asBool() ? nFields++ : -1;
	int	fSUM		= Parameters("SUM"   )->asBool() ? nFields++ : -1;
	int	fMEAN		= Parameters("MEAN"  )->asBool() ? nFields++ : -1;
	int	fVAR		= Parameters("VAR"   )->asBool() ? nFields++ : -1;
	int	fSTDDEV		= Parameters("STDDEV")->asBool() ? nFields++ : -1;
	int	fGINI		= Parameters("GINI"  )->asBool() ? nFields++ : -1;

	//-----------------------------------------------------
	CSG_Vector	Percentiles;

	{
		CSG_Strings	Values	= SG_String_Tokenize(Parameters("QUANTILES")->asString(), ";");

		for(int i=0; i<Values.Get_Count(); i++)
		{
			double	Value;

			if( Values[i].asDouble(Value) && Value >= 0. && Value <= 100. )
			{
				Percentiles.Add_Row(Value);
			}
		}
	}

	int	fQUANTILE	= Percentiles.Get_N() > 0 ? nFields : -1;

	nFields	+= Percentiles.Get_N();

	//-----------------------------------------------------
	if( nFields == 0 )
	{
		Error_Set(_TL("no output parameter in selection"));

		return( false );
	}

	//-----------------------------------------------------
	bool	bParallelized	= Parameters("PARALLELIZED")->is_Enabled() && Parameters("PARALLELIZED")->asBool();

	int		Naming			= Parameters("NAMING")->asInt();
	int		Method			= Parameters("METHOD")->asInt();

	//-----------------------------------------------------
	CSG_Grid	Index;

	if( Method == 0 && !Get_Simple_Index(pPolygons, Index) )
	{
		Error_Set(_TL("no grids in selection"));

		return( false );
	}

	//-----------------------------------------------------
	if( Parameters("RESULT")->asShapes() != NULL && Parameters("RESULT")->asShapes() != pPolygons )
	{
		pPolygons	= Parameters("RESULT")->asShapes();
		pPolygons	->Assign(Parameters("POLYGONS")->asShapes());
		pPolygons	->Fmt_Name("%s [%s]", Parameters("POLYGONS")->asShapes()->Get_Name(), _TL("Grid Statistics"));
	}

	CSG_Simple_Statistics	*Statistics	= new CSG_Simple_Statistics[pPolygons->Get_Count()];

	//-----------------------------------------------------
	for(int iGrid=0; iGrid<pGrids->Get_Grid_Count() && Process_Get_Okay(); iGrid++)
	{
		Process_Set_Text("[%d/%d] %s", 1 + iGrid, pGrids->Get_Grid_Count(), pGrids->Get_Grid(iGrid)->Get_Name());

		if( (Method == 0 && Get_Simple (pGrids->Get_Grid(iGrid), pPolygons, Statistics, Percentiles.Get_N() > 0 || fGINI > 0, Index        ))
		||  (Method != 0 && Get_Precise(pGrids->Get_Grid(iGrid), pPolygons, Statistics, Percentiles.Get_N() > 0 || fGINI > 0, bParallelized)) )
		{
			nFields	= pPolygons->Get_Field_Count();

			if( fCOUNT    >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("CELLS"   )), SG_DATATYPE_Int   );
			if( fMIN      >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("MIN"     )), SG_DATATYPE_Double);
			if( fMAX      >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("MAX"     )), SG_DATATYPE_Double);
			if( fRANGE    >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("RANGE"   )), SG_DATATYPE_Double);
			if( fSUM      >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("SUM"     )), SG_DATATYPE_Double);
			if( fMEAN     >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("MEAN"    )), SG_DATATYPE_Double);
			if( fVAR      >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("VARIANCE")), SG_DATATYPE_Double);
			if( fSTDDEV   >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("STDDEV"  )), SG_DATATYPE_Double);
			if (fGINI     >= 0 )	pPolygons->Add_Field(GET_FIELD_NAME(_TL("GINI"    )), SG_DATATYPE_Double);
			if( fQUANTILE >= 0 )
			{
				for(int iPercentile=0; iPercentile<Percentiles.Get_N(); iPercentile++)
				{
					pPolygons->Add_Field(GET_FIELD_NAME(CSG_String::Format("Q%02d", (int)Percentiles[iPercentile]).c_str()), SG_DATATYPE_Double);
				}
			}

			//---------------------------------------------
			for(sLong i=0; i<pPolygons->Get_Count() && Set_Progress(i, pPolygons->Get_Count()); i++)
			{
				CSG_Shape	*pPolygon	= pPolygons->Get_Shape(i);

				if( Statistics[i].Get_Count() == 0 )
				{
					if( fCOUNT    >= 0 )	pPolygon->Set_NoData(nFields + fCOUNT );
					if( fMIN      >= 0 )	pPolygon->Set_NoData(nFields + fMIN   );
					if( fMAX      >= 0 )	pPolygon->Set_NoData(nFields + fMAX   );
					if( fRANGE    >= 0 )	pPolygon->Set_NoData(nFields + fRANGE );
					if( fSUM      >= 0 )	pPolygon->Set_NoData(nFields + fSUM   );
					if( fMEAN     >= 0 )	pPolygon->Set_NoData(nFields + fMEAN  );
					if( fVAR      >= 0 )	pPolygon->Set_NoData(nFields + fVAR   );
					if( fSTDDEV   >= 0 )	pPolygon->Set_NoData(nFields + fSTDDEV);
					if( fGINI     >= 0 )	pPolygon->Set_NoData(nFields + fGINI  );
					if( fQUANTILE >= 0 )
					{
						for(int iPercentile=0, iField=nFields + fQUANTILE; iPercentile<Percentiles.Get_N(); iPercentile++, iField++)
						{
							pPolygon->Set_NoData(iField);
						}
					}
				}
				else
				{
					if( fCOUNT    >= 0 )	pPolygon->Set_Value(nFields + fCOUNT , Statistics[i].Get_Count   ());
					if( fMIN      >= 0 )	pPolygon->Set_Value(nFields + fMIN   , Statistics[i].Get_Minimum ());
					if( fMAX      >= 0 )	pPolygon->Set_Value(nFields + fMAX   , Statistics[i].Get_Maximum ());
					if( fRANGE    >= 0 )	pPolygon->Set_Value(nFields + fRANGE , Statistics[i].Get_Range   ());
					if( fSUM      >= 0 )	pPolygon->Set_Value(nFields + fSUM   , Statistics[i].Get_Sum     ());
					if( fMEAN     >= 0 )	pPolygon->Set_Value(nFields + fMEAN  , Statistics[i].Get_Mean    ());
					if( fVAR      >= 0 )	pPolygon->Set_Value(nFields + fVAR   , Statistics[i].Get_Variance());
					if( fSTDDEV   >= 0 )	pPolygon->Set_Value(nFields + fSTDDEV, Statistics[i].Get_StdDev  ());
					if( fGINI     >= 0 )	pPolygon->Set_Value(nFields + fGINI  , Statistics[i].Get_Gini    ());
					if( fQUANTILE >= 0 )
					{
						for(int iPercentile=0, iField=nFields + fQUANTILE; iPercentile<Percentiles.Get_N(); iPercentile++, iField++)
						{
							pPolygon->Set_Value(iField, Statistics[i].Get_Percentile(Percentiles[iPercentile]));
						}
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	delete[](Statistics);

	DataObject_Update(pPolygons);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_Statistics_AddTo_Polygon::Get_Precise(CSG_Grid *pGrid, CSG_Shapes *pPolygons, CSG_Simple_Statistics *Statistics, bool bHoldValues, bool bParallelized)
{
	int	Method	= Parameters("METHOD")->asInt();

	if( bParallelized )
	{
		#pragma omp parallel for
		for(sLong i=0; i<pPolygons->Get_Count(); i++)
		{
			Get_Precise(pGrid, (CSG_Shape_Polygon *)pPolygons->Get_Shape(i), Statistics[i], bHoldValues, Method);
		}
	}
	else
	{
		for(sLong i=0; i<pPolygons->Get_Count() && Set_Progress(i, pPolygons->Get_Count()); i++)
		{
			Get_Precise(pGrid, (CSG_Shape_Polygon *)pPolygons->Get_Shape(i), Statistics[i], bHoldValues, Method);
		}
	}

	return( true );
}

//---------------------------------------------------------
bool CGrid_Statistics_AddTo_Polygon::Get_Precise(CSG_Grid *pGrid, CSG_Shape_Polygon *pPolygon, CSG_Simple_Statistics &Statistics, bool bHoldValues, int Method)
{
	//-----------------------------------------------------
	CSG_Shapes	Intersect(SHAPE_TYPE_Polygon);

	CSG_Shape_Polygon	*pCell, *pArea;

	if( Method == 3 )	// polygon wise (cell area weighted)
	{
		pCell	= (CSG_Shape_Polygon *)Intersect.Add_Shape();
		pArea	= (CSG_Shape_Polygon *)Intersect.Add_Shape();
	}

	//-----------------------------------------------------
	Statistics.Create(bHoldValues);

	int	ax	= Get_System().Get_xWorld_to_Grid(pPolygon->Get_Extent().Get_XMin()) - 1;	if( ax < 0         )	ax	= 0;
	int	bx	= Get_System().Get_xWorld_to_Grid(pPolygon->Get_Extent().Get_XMax()) + 1;	if( bx >= Get_NX() )	bx	= Get_NX() - 1;
	int	ay	= Get_System().Get_yWorld_to_Grid(pPolygon->Get_Extent().Get_YMin()) - 1;	if( ay < 0         )	ay	= 0;
	int	by	= Get_System().Get_yWorld_to_Grid(pPolygon->Get_Extent().Get_YMax()) + 1;	if( by >= Get_NY() )	by	= Get_NY() - 1;

	TSG_Point	Center;
	TSG_Rect	Cell;

	//-----------------------------------------------------
	Center.y	= Get_System().Get_yGrid_to_World(ay);
	Cell.yMin	= Center.y - 0.5 * Get_Cellsize();
	Cell.yMax	= Cell.yMin + Get_Cellsize();
		
	for(int y=ay; y<=by; y++, Center.y+=Get_Cellsize(), Cell.yMin+=Get_Cellsize(), Cell.yMax+=Get_Cellsize())
	{
		Center.x	= Get_System().Get_xGrid_to_World(ax);
		Cell.xMin	= Center.x - 0.5 * Get_Cellsize();
		Cell.xMax	= Cell.xMin + Get_Cellsize();

		for(int x=ax; x<=bx; x++, Center.x+=Get_Cellsize(), Cell.xMin+=Get_Cellsize(), Cell.xMax+=Get_Cellsize())
		{
			if( !pGrid->is_NoData(x, y) )
			{
				switch( Method )
				{
				//-----------------------------------------
				default:	// polygon wise (cell centers)
					if( pPolygon->Contains(Center) )
					{
						Statistics	+= pGrid->asDouble(x, y);
					}
					break;

				//-----------------------------------------
				case  2:	// polygon wise (cell area)
					if( pPolygon->Intersects(Cell) )
					{
						Statistics	+= pGrid->asDouble(x, y);
					}
					break;

				//-----------------------------------------
				case  3:	// polygon wise (cell area weighted)
					switch( pPolygon->Intersects(Cell) )
					{
					case INTERSECTION_None     :	break;
					case INTERSECTION_Identical:
					case INTERSECTION_Contains :	Statistics.Add_Value(pGrid->asDouble(x, y),       Get_Cellarea());	break;
					case INTERSECTION_Contained:	Statistics.Add_Value(pGrid->asDouble(x, y), pPolygon->Get_Area());	break;
					case INTERSECTION_Overlaps :
						pCell->Del_Parts();

						pCell->Add_Point(Cell.xMin, Cell.yMin);	pCell->Add_Point(Cell.xMin, Cell.yMax);
						pCell->Add_Point(Cell.xMax, Cell.yMax);	pCell->Add_Point(Cell.xMax, Cell.yMin);

						if( SG_Shape_Get_Intersection(pPolygon, pCell, pArea) )
						{
							Statistics.Add_Value(pGrid->asDouble(x, y), pArea->Get_Area());
						}
						break;
					}
					break;
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
bool CGrid_Statistics_AddTo_Polygon::Get_Simple(CSG_Grid *pGrid, CSG_Shapes *pPolygons, CSG_Simple_Statistics *Statistics, bool bHoldValues, CSG_Grid &Index)
{
	for(sLong i=0; i<pPolygons->Get_Count(); i++)
	{
		Statistics[i].Create(bHoldValues);
	}

	for(int y=0; y<Get_NY() && Set_Progress_Rows(y); y++)
	{
		for(int x=0, i; x<Get_NX(); x++)
		{
			if( !pGrid->is_NoData(x, y) && (i = Index.asInt(x, y)) >= 0 && i < pPolygons->Get_Count() )
			{
				Statistics[i] += pGrid->asDouble(x, y);
			}
		}
	}

	return( true );
}

//---------------------------------------------------------
bool CGrid_Statistics_AddTo_Polygon::Get_Simple_Index(CSG_Shapes *pPolygons, CSG_Grid &Index)
{
	Index.Create(Get_System(), pPolygons->Get_Count() < 32767 ? SG_DATATYPE_Short : SG_DATATYPE_Int);
	Index.Assign(-1.);

	bool *bCrossing = (bool *)SG_Malloc(Get_NX() * sizeof(bool));

	//-----------------------------------------------------
	for(sLong iShape=0; iShape<pPolygons->Get_Count() && Set_Progress(iShape, pPolygons->Get_Count()); iShape++)
	{
		CSG_Shape *pPolygon = pPolygons->Get_Shape(iShape); TSG_Rect Extent = pPolygon->Get_Extent();

		int xStart = Get_System().Get_xWorld_to_Grid(Extent.xMin) - 1;	if( xStart < 0 )		xStart	= 0;
		int xStop  = Get_System().Get_xWorld_to_Grid(Extent.xMax) + 1;	if( xStop >= Get_NX() )	xStop	= Get_NX() - 1;

		CSG_Point Left (Get_XMin() - 1., 0.);
		CSG_Point Right(Get_XMax() + 1., 0.);

		//-------------------------------------------------
		for(int y=0; y<Get_NY(); y++)
		{
			double yPos = Get_YMin() + y * Get_Cellsize();

			if( yPos >= Extent.yMin && yPos <= Extent.yMax )
			{
				memset(bCrossing, 0, Get_NX() * sizeof(bool));

				Left.y = Right.y = yPos;

				//-----------------------------------------
				for(int iPart=0; iPart<pPolygon->Get_Part_Count(); iPart++)
				{
					CSG_Point B = pPolygon->Get_Point(pPolygon->Get_Point_Count(iPart) - 1, iPart);

					for(int iPoint=0; iPoint<pPolygon->Get_Point_Count(iPart); iPoint++)
					{
						CSG_Point A = B; B = pPolygon->Get_Point(iPoint, iPart);

						if( ((A.y <= yPos && yPos <  B.y)
						||   (A.y >  yPos && yPos >= B.y)) )
						{
							TSG_Point C; SG_Get_Crossing(C, A, B, Left, Right, false);

							int ix = (int)((C.x - Get_XMin()) / Get_Cellsize() + 1.);

							if( ix < 0 )
							{
								ix = 0;
							}
							else if( ix >= Get_NX() )
							{
								continue;
							}

							bCrossing[ix] = !bCrossing[ix];
						}
					}
				}

				//-----------------------------------------
				for(int x=xStart, bFill=0; x<=xStop; x++)
				{
					if( bCrossing[x] )
					{
						bFill = bFill ? 0 : 1;
					}

					if( bFill )
					{
						Index.Set_Value(x, y, (double)iShape);
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	SG_Free(bCrossing);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
