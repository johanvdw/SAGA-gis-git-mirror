
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                    ta_morphometry                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 Convergence_Radius.cpp                //
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
#include "Convergence_Radius.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CConvergence_Radius::CConvergence_Radius(void)
{
	Set_Name		(_TL("Convergence Index (Search Radius)"));

	Set_Author		("O.Conrad (c) 2003");

	Set_Description	(_TW(
		"Convergence Index (Search Radius)"
	));

	Add_Reference("Koethe, R. & Lehmeier, F.", "1996",
		"SARA - System zur Automatischen Relief-Analyse",
		"User Manual, 2. Edition [Dept. of Geography, University of Goettingen, unpublished]."
	);

	Parameters.Add_Grid("",
		"ELEVATION"		, _TL("Elevation"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid("",
		"CONVERGENCE"	, _TL("Convergence Index"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Bool("",
		"SLOPE"			, _TL("Gradient"),
		_TL(""),
		false
	);

	Parameters.Add_Choice("",
		"DIFFERENCE"	, _TL("Difference"),
		_TL(""),
		CSG_String::Format("%s|%s",
			_TL("direction to the center cell"),
			_TL("center cell's aspect direction")
		), 0
	);

	Parameters.Add_Double("",
		"RADIUS"		, _TL("Radius [Cells]"),
		_TL(""),
		10., 1., true
	);

	m_Cells.Get_Weighting().Create_Parameters(Parameters);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CConvergence_Radius::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	m_Cells.Get_Weighting().Enable_Parameters(*pParameters);

	return( CSG_Tool_Grid::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CConvergence_Radius::On_Execute(void)
{
	CSG_Grid	*pConvergence;

	m_pDTM			= Parameters("ELEVATION"  )->asGrid();
	pConvergence	= Parameters("CONVERGENCE")->asGrid();
	m_bSlope		= Parameters("SLOPE"      )->asBool();
	m_bDifference	= Parameters("DIFFERENCE" )->asInt() == 0;

	//-----------------------------------------------------
	m_Cells.Get_Weighting().Set_Parameters(Parameters);

	if( !m_Cells.Set_Radius(Parameters("RADIUS")->asDouble()) )
	{
		return( false );
	}

	//-----------------------------------------------------
	DataObject_Set_Colors(pConvergence, 11, SG_COLORS_RED_GREY_BLUE, true);

	//-----------------------------------------------------
	m_Direction.Create(m_Cells.Get_Count());

	for(int i=0; i<m_Cells.Get_Count(); i++)
	{
		int	y, x;	double	d, w;

		if( m_Cells.Get_Values(i, x, y, d, w, false) )
		{
			m_Direction[i]	= SG_Get_Angle_Of_Direction(0., 0., x, y);
			m_Direction[i]	= y != 0 ? (M_PI_180 + atan2((double)x, (double)y)) : (x > 0 ? M_PI_270 : M_PI_090);
		}
	}

	//-----------------------------------------------------
	m_Slope	.Create(Get_System(), SG_DATATYPE_Float);
	m_Aspect.Create(Get_System(), SG_DATATYPE_Float);

	for(int y=0; y<Get_NY() && Set_Progress_Rows(y); y++)
	{
		#pragma omp parallel for
		for(int x=0; x<Get_NX(); x++)
		{
			double	Slope, Aspect;

			if( m_pDTM->is_InGrid(x, y) && m_pDTM->Get_Gradient(x, y, Slope, Aspect) && Aspect >= 0. )
			{
				m_Slope	.Set_Value(x, y, Slope);
				m_Aspect.Set_Value(x, y, Aspect);
			}
			else
			{
				m_Slope	.Set_NoData(x, y);
				m_Aspect.Set_NoData(x, y);
			}
		}
	}

	//-----------------------------------------------------
	for(int y=0; y<Get_NY() && Set_Progress_Rows(y); y++)
	{
		#pragma omp parallel for
		for(int x=0; x<Get_NX(); x++)
		{
			double	d;

			if( Get_Convergence(x, y, d) )
			{
				pConvergence->Set_Value(x, y, d);
			}
			else
			{
				pConvergence->Set_NoData(x, y);
			}
		}
	}

	//-----------------------------------------------------
	m_Cells    .Destroy();
	m_Direction.Destroy();
	m_Slope    .Destroy();
	m_Aspect   .Destroy();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CConvergence_Radius::Get_Convergence(int x, int y, double &Convergence)
{
	if( !m_pDTM->is_InGrid(x, y) )
	{
		return( false );
	}

	int		i, ix, iy;	double	id, iw, z	= m_pDTM->asDouble(x, y);

	CSG_Simple_Statistics	s;

	//-----------------------------------------------------
	for(i=0; i<m_Cells.Get_Count(); i++)
	{
		if( m_Cells.Get_Values(i, ix = x, iy = y, id, iw, true) && id > 0. && m_Aspect.is_InGrid(ix, iy) )
		{
			double	d, Direction	= m_bDifference ? m_Direction[i] : m_Aspect.asDouble(x, y);

			d		= fmod(m_Aspect.asDouble(ix, iy) - Direction, M_PI_360);

			if( d < -M_PI_180 )
			{
				d	+= M_PI_360;
			}
			else if( d > M_PI_180 )
			{
				d	-= M_PI_360;
			}

		//	s.Add_Value(m_bSlope ? fabs(d) * tan(m_Slope(ix, iy)) : fabs(d), iWeight);
			s.Add_Value(fabs(d), m_bSlope ? iw * tan(m_Slope(ix, iy)) : iw);
		}
	}

	//-----------------------------------------------------
	if( s.Get_Count() > 0 )
	{
		Convergence	= (s.Get_Mean() - M_PI_090) * 100. / M_PI_090;

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
