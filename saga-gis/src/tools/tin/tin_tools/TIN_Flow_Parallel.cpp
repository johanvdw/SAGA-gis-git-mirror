
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       TIN_Tools                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 TIN_Flow_Parallel.cpp                 //
//                                                       //
//                 Copyright (C) 2003 by                 //
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
#include "TIN_Flow_Parallel.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CTIN_Flow_Parallel::CTIN_Flow_Parallel(void)
{
	Set_Name		(_TL("Flow Accumulation (Parallel)"));

	Set_Author		("O.Conrad (c) 2004");

	Set_Description	(_TW(
		"Calculates the catchment area based on the selected elevation values.\n\n"
	));

	//-----------------------------------------------------
	Parameters.Add_TIN("",
		"DEM"		, _TL("TIN"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Table_Field("DEM",
		"ZFIELD"	, _TL("Z Values"),
		_TL("")
	);

	Parameters.Add_TIN("",
		"FLOW"		, _TL("Flow Accumulation"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Choice("",
		"METHOD"	, _TL("Method"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("Single Flow Direction"),
			_TL("Multiple Flow Direction")
		)
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CTIN_Flow_Parallel::On_Execute(void)
{
	CSG_TIN *pDEM = Parameters("DEM")->asTIN();

	m_iHeight = Parameters("ZFIELD")->asInt();

	m_pFlow = Parameters("FLOW")->asTIN();
	m_pFlow->Create(*pDEM);

	m_iArea     = m_pFlow->Get_Field_Count(); m_pFlow->Add_Field("AREA"    , SG_DATATYPE_Double);
	m_iFlow     = m_pFlow->Get_Field_Count(); m_pFlow->Add_Field("FLOW"    , SG_DATATYPE_Double);
	m_iSpecific = m_pFlow->Get_Field_Count(); m_pFlow->Add_Field("SPECIFIC", SG_DATATYPE_Double);

	m_pFlow->Set_Index(m_iHeight, TABLE_INDEX_Descending);

	//-----------------------------------------------------
	for(sLong iPoint=0; iPoint<m_pFlow->Get_Node_Count() && Set_Progress(iPoint, m_pFlow->Get_Node_Count()); iPoint++)
	{
		switch( Parameters("METHOD")->asInt() )
		{
		default: Let_it_flow_single  (m_pFlow->Get_Node(m_pFlow->Get_Record_byIndex(iPoint)->Get_Index())); break;
		case  1: Let_it_flow_multiple(m_pFlow->Get_Node(m_pFlow->Get_Record_byIndex(iPoint)->Get_Index())); break;
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CTIN_Flow_Parallel::Let_it_flow_single(CSG_TIN_Node *pPoint)
{
	int iMin = -1; double dzMin = 0., Area = pPoint->Get_Polygon_Area();

	pPoint->Set_Value(m_iArea, Area);
	pPoint->Add_Value(m_iFlow, Area);

	for(int i=0; i<pPoint->Get_Neighbor_Count(); i++)
	{
		double dz = pPoint->Get_Gradient(i, m_iHeight);

		if( dz > dzMin )
		{
			iMin = i; dzMin = dz;
		}
	}

	if( iMin >= 0 )
	{
		pPoint->Get_Neighbor(iMin)->Add_Value(m_iFlow, pPoint->asDouble(m_iFlow));
	}

	pPoint->Set_Value(m_iSpecific, Area > 0. ? 1. / Area : -1.);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CTIN_Flow_Parallel::Let_it_flow_multiple(CSG_TIN_Node *pPoint)
{
	double Area = pPoint->Get_Polygon_Area();

	pPoint->Set_Value(m_iArea, Area);
	pPoint->Add_Value(m_iFlow, Area);

	if( pPoint->Get_Neighbor_Count() > 0 )
	{
		double dzSum = 0.; CSG_Vector dz(pPoint->Get_Neighbor_Count());

		for(int i=0; i<pPoint->Get_Neighbor_Count(); i++)
		{
			if( (dz[i] = pPoint->Get_Gradient(i, m_iHeight)) > 0. )
			{
				dzSum += dz[i];
			}
		}

		if( dzSum > 0. )
		{
			double Flow = pPoint->asDouble(m_iFlow);

			for(int i=0; i<pPoint->Get_Neighbor_Count(); i++)
			{
				if( dz[i] > 0. )
				{
					pPoint->Get_Neighbor(i)->Add_Value(	m_iFlow, Flow * dz[i] / dzSum);
				}
			}
		}
	}

	pPoint->Set_Value(m_iSpecific, Area > 0. ? 1. / Area : -1.);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
