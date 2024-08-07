
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
//                   Triangulation.cpp                   //
//                                                       //
//                 Copyright (C) 2004 by                 //
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
#include "Interpolation_Triangulation.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CInterpolation_Triangulation::CInterpolation_Triangulation(void)
	: CInterpolation(false)
{
	Set_Name		(_TL("Triangulation"));

	Set_Author		("O.Conrad (c) 2004");

	Set_Description	(_TW(
		"Gridding of a shapes layer using Delaunay Triangulation."
	));

	//-----------------------------------------------------
	Parameters.Add_Bool("",
		"FRAME"				, _TL("Add Frame"),
		_TL(""),
		false
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CInterpolation_Triangulation::Interpolate(void)
{
	m_pGrid	= Get_Grid();

	CSG_TIN	TIN;

	if( !Get_TIN(TIN) )
	{
		Error_Set("failed to create TIN");

		return( false );
	}

	m_pGrid->Assign_NoData();

	//-----------------------------------------------------
	for(sLong iTriangle=0; iTriangle<TIN.Get_Triangle_Count() && Set_Progress(iTriangle, TIN.Get_Triangle_Count()); iTriangle++)
	{
		CSG_TIN_Triangle	*pTriangle	= TIN.Get_Triangle(iTriangle);

		if( m_pGrid->Get_Extent().Intersects(pTriangle->Get_Extent()) != INTERSECTION_None )
		{
			TSG_Point_3D	p[3];

			for(int iPoint=0; iPoint<3; iPoint++)
			{
				p[iPoint].x	= (pTriangle->Get_Node(iPoint)->Get_X() - m_pGrid->Get_XMin()) / m_pGrid->Get_Cellsize();
				p[iPoint].y	= (pTriangle->Get_Node(iPoint)->Get_Y() - m_pGrid->Get_YMin()) / m_pGrid->Get_Cellsize();
				p[iPoint].z	=  pTriangle->Get_Node(iPoint)->asDouble(0);
			}

			Set_Triangle(p);
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CInterpolation_Triangulation::Get_TIN(CSG_TIN &TIN)
{
	TIN.Destroy();

	bool	bFrame	= Parameters("FRAME")->asBool();

	double	x[4], y[4], z[4], dMin[4];

	x[0]	= m_pGrid->Get_Extent().Get_XMin();	y[0]	= m_pGrid->Get_Extent().Get_YMin();	dMin[0]	= -1.0;
	x[1]	= m_pGrid->Get_Extent().Get_XMin();	y[1]	= m_pGrid->Get_Extent().Get_YMax();	dMin[1]	= -1.0;
	x[2]	= m_pGrid->Get_Extent().Get_XMax();	y[2]	= m_pGrid->Get_Extent().Get_YMax();	dMin[2]	= -1.0;
	x[3]	= m_pGrid->Get_Extent().Get_XMax();	y[3]	= m_pGrid->Get_Extent().Get_YMin();	dMin[3]	= -1.0;

	TIN.Add_Field("Z", Get_Points()->Get_Field_Type(Get_Field()));

	for(sLong iShape=0; iShape<Get_Points()->Get_Count(); iShape++)
	{
		CSG_Shape	*pShape	= Get_Points()->Get_Shape(iShape);

		if( !pShape->is_NoData(Get_Field()) )
		{
			for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
			{
				for(int iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
				{
					TSG_Point	p = pShape->Get_Point(iPoint, iPart);

					TIN.Add_Node(p, NULL, false)->Set_Value(0, pShape->asDouble(Get_Field()));

					if( bFrame )
					{
						for(int iCorner=0; iCorner<4; iCorner++)
						{
							double d	= SG_Get_Distance(p.x, p.y, x[iCorner], y[iCorner]);

							if( dMin[iCorner] < 0.0 || d < dMin[iCorner] )
							{
								dMin[iCorner]	= d;
								z   [iCorner]	= pShape->asDouble(Get_Field());
							}
						}
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	if( bFrame )
	{
		for(int iCorner=0; iCorner<4; iCorner++)
		{
			if( dMin[iCorner] >= 0.0 )
			{
				CSG_Point	p(x[iCorner], y[iCorner]);

				TIN.Add_Node(p, NULL, false)->Set_Value(0, z[iCorner]);
			}
		}
	}

	TIN.Update();

	return( TIN.Get_Triangle_Count() > 0 );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CInterpolation_Triangulation::Set_Triangle(TSG_Point_3D p[3])
{
	//-----------------------------------------------------
	if( p[1].y < p[0].y ) {	TSG_Point_3D pp = p[1]; p[1] = p[0]; p[0] = pp;	}
	if( p[2].y < p[0].y ) {	TSG_Point_3D pp = p[2]; p[2] = p[0]; p[0] = pp;	}
	if( p[2].y < p[1].y ) {	TSG_Point_3D pp = p[2]; p[2] = p[1]; p[1] = pp;	}

	//-----------------------------------------------------
	TSG_Rect	r;

	r.yMin	= p[0].y;
	r.yMax	= p[2].y;
	r.xMin	= p[0].x < p[1].x ? (p[0].x < p[2].x ? p[0].x : p[2].x) : (p[1].x < p[2].x ? p[1].x : p[2].x);
	r.xMax	= p[0].x > p[1].x ? (p[0].x > p[2].x ? p[0].x : p[2].x) : (p[1].x > p[2].x ? p[1].x : p[2].x);

	if( r.yMin >= r.yMax || r.xMin >= r.xMax )
	{
		return;	// no area
	}

	if( (r.yMin < 0.0 && r.yMax < 0.0) || (r.yMin >= m_pGrid->Get_NY() && r.yMax >= m_pGrid->Get_NY())
	||	(r.xMin < 0.0 && r.xMax < 0.0) || (r.xMin >= m_pGrid->Get_NX() && r.xMax >= m_pGrid->Get_NX()) )
	{
		return;	// completely outside grid
	}

	//-----------------------------------------------------
	TSG_Point_3D	d[3];

	if( (d[0].y	= p[2].y - p[0].y) != 0.0 )
	{
		d[0].x	= (p[2].x - p[0].x) / d[0].y;
		d[0].z	= (p[2].z - p[0].z) / d[0].y;
	}

	if( (d[1].y	= p[1].y - p[0].y) != 0.0 )
	{
		d[1].x	= (p[1].x - p[0].x) / d[1].y;
		d[1].z	= (p[1].z - p[0].z) / d[1].y;
	}

	if( (d[2].y	= p[2].y - p[1].y) != 0.0 )
	{
		d[2].x	= (p[2].x - p[1].x) / d[2].y;
		d[2].z	= (p[2].z - p[1].z) / d[2].y;
	}

	//-----------------------------------------------------
	int	ay	= (int)r.yMin;	if( ay < 0 )	ay	= 0;	if( ay < r.yMin )	ay++;
	int	by	= (int)r.yMax;	if( by >= m_pGrid->Get_NY() )	by	= m_pGrid->Get_NY() - 1;

	for(int y=ay; y<=by; y++)
	{
		if( y <= p[1].y && d[1].y > 0.0 )
		{
			Set_Triangle_Line(y,
				p[0].x + (y - p[0].y) * d[0].x,
				p[0].z + (y - p[0].y) * d[0].z,
				p[0].x + (y - p[0].y) * d[1].x,
				p[0].z + (y - p[0].y) * d[1].z
			);
		}
		else if( d[2].y > 0.0 )
		{
			Set_Triangle_Line(y,
				p[0].x + (y - p[0].y) * d[0].x,
				p[0].z + (y - p[0].y) * d[0].z,
				p[1].x + (y - p[1].y) * d[2].x,
				p[1].z + (y - p[1].y) * d[2].z
			);
		}
	}
}

//---------------------------------------------------------
inline void CInterpolation_Triangulation::Set_Triangle_Line(int y, double xa, double za, double xb, double zb)
{
	if( xb < xa )
	{
		double	d;

		d	= xa;	xa	= xb;	xb	= d;
		d	= za;	za	= zb;	zb	= d;
	}

	if( xb > xa )
	{
		double	dz	= (zb - za) / (xb - xa);
		int		ax	= (int)xa;	if( ax < 0 )	ax	= 0;	if( ax < xa )	ax++;
		int		bx	= (int)xb;	if( bx >= m_pGrid->Get_NX() )	bx	= m_pGrid->Get_NX() - 1;

		for(int x=ax; x<=bx; x++)
		{
			double	z	= za + dz * (x - xa);

			if( m_pGrid->is_NoData(x, y) || m_pGrid->asDouble(x, y) < z )
			{
				m_pGrid->Set_Value(x, y, z);
			}
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
