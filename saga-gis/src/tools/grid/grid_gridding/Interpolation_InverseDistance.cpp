
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                     Grid_Gridding                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//           Interpolation_InverseDistance.cpp           //
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
#include "Interpolation_InverseDistance.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CInterpolation_InverseDistance::CInterpolation_InverseDistance(void)
	: CInterpolation(true, true)
{
	Set_Name		(_TL("Inverse Distance Weighted"));

	Set_Author		("O.Conrad (c) 2003");

	Set_Description	(_TW(
		"Inverse distance grid interpolation from irregular distributed points."
	));

	//-----------------------------------------------------
	m_Search_Options.Create(&Parameters, "NODE_SEARCH", 1);

	m_Weighting.Set_Weighting (SG_DISTWGHT_IDW);
	m_Weighting.Set_IDW_Offset(false);
	m_Weighting.Set_IDW_Power (2.);

	m_Weighting.Create_Parameters(Parameters);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CInterpolation_InverseDistance::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("POINTS") )
	{
		m_Search_Options.On_Parameter_Changed(pParameters, pParameter);

		if( pParameter->asShapes() && pParameter->asShapes()->Get_Count() > 1 )
		{	// get a rough estimation of point density for band width suggestion
			pParameters->Set_Parameter("DW_BANDWIDTH", SG_Get_Rounded_To_SignificantFigures(
				0.5 * sqrt(pParameter->asShapes()->Get_Extent().Get_Area() / pParameter->asShapes()->Get_Count()), 1
			));
		}
	}

	return( CInterpolation::On_Parameter_Changed(pParameters, pParameter) );
}

//---------------------------------------------------------
int CInterpolation_InverseDistance::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	m_Search_Options.On_Parameters_Enable(pParameters, pParameter);

	m_Weighting.Enable_Parameters(*pParameters);

	return( CInterpolation::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CInterpolation_InverseDistance::On_Initialize(void)
{
	if( !m_Search_Options.Do_Use_All(true) && !m_Search.Create(Get_Points(), Get_Field()) )
	{
		Error_Set(_TL("failed to initialize search engine"));

		return( false );
	}

	return(	m_Weighting.Set_Parameters(Parameters) );
}

//---------------------------------------------------------
bool CInterpolation_InverseDistance::On_Finalize(void)
{
	m_Search.Destroy();

	return(	true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CInterpolation_InverseDistance::Get_Value(double x, double y, double &Value)
{
	CSG_Simple_Statistics s;

	//-----------------------------------------------------
	if( m_Search.is_Okay() )	// local
	{
		CSG_Array_sLong Index; CSG_Vector Distance;

		if( m_Search.Get_Nearest_Points(x, y,
			m_Search_Options.Get_Max_Points(),
			m_Search_Options.Get_Radius(), Index, Distance
		) < m_Search_Options.Get_Min_Points() )
		{
			return( false );
		}

		for(sLong i=0; i<Index.Get_Size(); i++)
		{
			if( Distance[i] > 0. )
			{
				s.Add_Value(m_Search.Get_Point_Value(Index[i]), m_Weighting.Get_Weight(Distance[i]));
			}
			else
			{
				s.Create(); s += m_Search.Get_Point_Value(Index[i]);

				for(++i; i<Index.Get_Size(); i++)
				{
					if( Distance[i] <= 0. )
					{
						s += m_Search.Get_Point_Value(Index[i]);
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	else						// global
	{
		CSG_Shapes *pPoints = Get_Points(); int Field = Get_Field();

		for(sLong i=0; i<pPoints->Get_Count(); i++)
		{
			CSG_Shape *pPoint = pPoints->Get_Shape(i);

			if( !pPoint->is_NoData(Field) )
			{
				double d = Get_Distance(x, y, pPoint->Get_Point());

				if( d > 0. )
				{
					s.Add_Value(pPoint->asDouble(Field), m_Weighting.Get_Weight(d));
				}
				else
				{
					s.Create(); s += pPoint->asDouble(Field);

					for(++i; i<pPoints->Get_Count(); i++)
					{
						pPoint = pPoints->Get_Shape(i);

						if( !pPoint->is_NoData(Field) && is_Identical(x, y, pPoint->Get_Point()) )
						{
							s += pPoint->asDouble(Field);
						}
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	Value = s.Get_Mean();

	return( true );
}

//---------------------------------------------------------
inline double CInterpolation_InverseDistance::Get_Distance(double x, double y, const TSG_Point &Point)
{
	double dx = x - Point.x;
	double dy = y - Point.y;

	return( sqrt(dx*dx + dy*dy) );
}

//---------------------------------------------------------
inline bool CInterpolation_InverseDistance::is_Identical(double x, double y, const TSG_Point &Point)
{
	return( x == Point.x && y == Point.y );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
