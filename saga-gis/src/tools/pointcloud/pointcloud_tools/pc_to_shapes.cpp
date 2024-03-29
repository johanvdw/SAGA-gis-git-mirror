/**********************************************************
 * Version $Id$
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                   pointcloud_tools                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    pc_to_shapes.cpp                   //
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


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "pc_to_shapes.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CPC_To_Shapes::CPC_To_Shapes(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("Point Cloud to Shapes"));

	Set_Author		(SG_T("O.Conrad (c) 2009"));

	Set_Description	(_TW(
		""
	));


	//-----------------------------------------------------
	Parameters.Add_PointCloud(
		NULL	, "POINTS"		, _TL("Points"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Shapes(
		NULL	, "SHAPES"		, _TL("Shapes"),
		_TL(""),
		PARAMETER_OUTPUT, SHAPE_TYPE_Point
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CPC_To_Shapes::On_Execute(void)
{
	int				iField;
	CSG_PointCloud	*pPoints;
	CSG_Shapes		*pShapes;

	pPoints	= Parameters("POINTS")	->asPointCloud();
	pShapes	= Parameters("SHAPES")	->asShapes();

	//-----------------------------------------------------
	pShapes->Create(SHAPE_TYPE_Point, pPoints->Get_Name(), NULL, SG_VERTEX_TYPE_XYZ);

	for(iField=2; iField<pPoints->Get_Field_Count(); iField++)
	{
		pShapes->Add_Field(pPoints->Get_Field_Name(iField), pPoints->Get_Field_Type(iField));
	}

	//-----------------------------------------------------
	for(sLong iPoint=0; iPoint<pPoints->Get_Count() && Set_Progress(iPoint, pPoints->Get_Count()); iPoint++)
	{
		pPoints->Set_Cursor(iPoint);

		CSG_Shape	*pShape	= pShapes->Add_Shape();

		pShape->Add_Point(pPoints->Get_X(), pPoints->Get_Y());
		pShape->Set_Z(pPoints->Get_Z(), 0);

		for(iField=2; iField<pPoints->Get_Field_Count(); iField++)
		{
			switch (pPoints->Get_Field_Type(iField))
			{
			default:					pShape->Set_Value(iField - 2, pPoints->Get_Value(iField));		break;
			case SG_DATATYPE_Date:
			case SG_DATATYPE_String:	CSG_String sAttr; pPoints->Get_Value(iField, sAttr); pShape->Set_Value(iField - 2, sAttr);		break;
			}
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
