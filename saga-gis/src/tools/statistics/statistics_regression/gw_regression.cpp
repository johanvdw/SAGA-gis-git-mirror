
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                 statistics_regression                 //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   gw_regression.cpp                   //
//                                                       //
//                 Copyright (C) 2010 by                 //
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
#include "gw_regression.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGW_Regression::CGW_Regression(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("GWR for Single Predictor (Gridded Model Output)"));

	Set_Author		("O.Conrad (c) 2010");

	Set_Description	(_TW(
		"This Geographically Weighted Regression tool for a single predictor creates gridded model output."
	));

	GWR_Add_References(true);

	//-----------------------------------------------------
	Parameters.Add_Shapes("",
		"POINTS"	, _TL("Points"),
		_TL(""),
		PARAMETER_INPUT, SHAPE_TYPE_Point
	);

	Parameters.Add_Table_Field("POINTS",
		"DEPENDENT"	, _TL("Dependent Variable"),
		_TL("")
	);

	Parameters.Add_Table_Field("POINTS",
		"PREDICTOR"	, _TL("Predictor"),
		_TL("")
	);

	Parameters.Add_Bool("",
		"LOGISTIC"	, _TL("Logistic Regression"),
		_TL(""),
		false
	);

	//-----------------------------------------------------
	m_Grid_Target.Create(&Parameters, true, "", "TARGET_");

	m_Grid_Target.Add_Grid("INTERCEPT", _TL("Intercept"), false);
	m_Grid_Target.Add_Grid("SLOPE"    , _TL("Slope"    ), false);
	m_Grid_Target.Add_Grid("QUALITY"  , _TL("Quality"  ), false);

	//-----------------------------------------------------
	m_Weighting.Set_Weighting(SG_DISTWGHT_GAUSS);
	m_Weighting.Create_Parameters(Parameters);

	//-----------------------------------------------------
	m_Search.Create(&Parameters, "NODE_SEARCH", 16);

	Parameters("SEARCH_RANGE"     )->Set_Value(1);
	Parameters("SEARCH_POINTS_ALL")->Set_Value(1);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CGW_Regression::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("POINTS") )
	{
		m_Grid_Target.Set_User_Defined(pParameters, pParameter->asShapes());

		m_Search.On_Parameter_Changed(pParameters, pParameter);

		pParameters->Set_Parameter("DW_BANDWIDTH", GWR_Fit_To_Density(pParameter->asShapes(), 4.0, 1));
	}

	m_Grid_Target.On_Parameter_Changed(pParameters, pParameter);

	return( CSG_Tool::On_Parameter_Changed(pParameters, pParameter) );
}

//---------------------------------------------------------
int CGW_Regression::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	m_Weighting.Enable_Parameters(*pParameters);

	m_Search     .On_Parameters_Enable(pParameters, pParameter);
	m_Grid_Target.On_Parameters_Enable(pParameters, pParameter);

	return( CSG_Tool::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGW_Regression::On_Execute(void)
{
	//-----------------------------------------------------
	m_pPoints		= Parameters("POINTS"   )->asShapes();
	m_iDependent	= Parameters("DEPENDENT")->asInt   ();
	m_iPredictor	= Parameters("PREDICTOR")->asInt   ();

	m_Weighting.Set_Parameters(Parameters);

	//-----------------------------------------------------
	if( !m_Search.Initialize(m_pPoints, -1) )
	{
		return( false );
	}

	//-----------------------------------------------------
	m_pQuality		= m_Grid_Target.Get_Grid("QUALITY"  );
	m_pSlope		= m_Grid_Target.Get_Grid("SLOPE"    );
	m_pIntercept	= m_Grid_Target.Get_Grid("INTERCEPT");

	if( !m_pIntercept || !m_pSlope || !m_pQuality )
	{
		m_Search.Finalize();

		return( false );
	}

	m_pIntercept->Fmt_Name("%s (%s)", Parameters("DEPENDENT")->asString(), _TL("GWR Intercept"));
	m_pSlope    ->Fmt_Name("%s (%s)", Parameters("DEPENDENT")->asString(), _TL("GWR Slope"    ));
	m_pQuality  ->Fmt_Name("%s (%s)", Parameters("DEPENDENT")->asString(), _TL("GWR Quality"  ));

	bool	bLogistic	= Parameters("LOGISTIC")->asBool();

	//-----------------------------------------------------
	for(int y=0; y<m_pIntercept->Get_NY() && Set_Progress(y, m_pIntercept->Get_NY()); y++)
	{
		#pragma omp parallel for
		for(int x=0; x<m_pIntercept->Get_NX(); x++)
		{
			CSG_Regression_Weighted	Model;

			if( Get_Model(x, y, Model, bLogistic) )
			{
				m_pIntercept->Set_Value(x, y, Model[0]);
				m_pSlope    ->Set_Value(x, y, Model[1]);
				m_pQuality  ->Set_Value(x, y, Model.Get_R2());
			}
			else
			{
				m_pIntercept->Set_NoData(x, y);
				m_pSlope    ->Set_NoData(x, y);
				m_pQuality  ->Set_NoData(x, y);
			}
		}
	}

	//-----------------------------------------------------
	m_Search.Finalize();

	DataObject_Update(m_pIntercept);
	DataObject_Update(m_pSlope    );
	DataObject_Update(m_pQuality  );

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGW_Regression::Get_Model(int x, int y, CSG_Regression_Weighted &Model, bool bLogistic)
{
	Model.Destroy(); TSG_Point Point = m_pIntercept->Get_System().Get_Grid_to_World(x, y);

	//-----------------------------------------------------
	if( m_Search.Do_Use_All() )
	{
		for(sLong iPoint=0; iPoint<m_pPoints->Get_Count(); iPoint++)
		{
			CSG_Shape *pPoint = m_pPoints->Get_Shape(iPoint); double Value;

			if( !pPoint->is_NoData(m_iDependent) && !pPoint->is_NoData(m_iPredictor) )
			{
				Model.Add_Sample(m_Weighting.Get_Weight(SG_Get_Distance(Point, pPoint->Get_Point())),
					pPoint->asDouble(m_iDependent), CSG_Vector(1, &(Value = pPoint->asDouble(m_iPredictor)))
				);
			}
		}
	}

	//-----------------------------------------------------
	else
	{
		CSG_Array_sLong Index; CSG_Vector Distance;

		if( !m_Search.Get_Points(Point, Index, Distance) )
		{
			return( false );
		}

		for(sLong iPoint=0; iPoint<Index.Get_Size(); iPoint++)
		{
			CSG_Shape *pPoint = m_pPoints->Get_Shape(Index[iPoint]); double Value;

			if( !pPoint->is_NoData(m_iDependent) && !pPoint->is_NoData(m_iPredictor) )
			{
				Model.Add_Sample(m_Weighting.Get_Weight(Distance[iPoint]),
					pPoint->asDouble(m_iDependent), CSG_Vector(1, &(Value = pPoint->asDouble(m_iPredictor)))
				);
			}
		}
	}

	//-----------------------------------------------------
	return( Model.Calculate(bLogistic) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
