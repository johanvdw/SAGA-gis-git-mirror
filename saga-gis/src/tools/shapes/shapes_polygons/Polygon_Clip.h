
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                    shapes_polygons                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     Polygon_Clip.h                    //
//                                                       //
//                 Copyright (C) 2012 by                 //
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
#ifndef HEADER_INCLUDED__Polygon_Clip_H
#define HEADER_INCLUDED__Polygon_Clip_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_api/saga_api.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CPolygon_Clip : public CSG_Tool  
{
public:
	CPolygon_Clip(void);


protected:

	virtual bool			On_Execute				(void);

	virtual int				On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);


private:

	bool					Clip_Shapes				(CSG_Shapes *pClip, CSG_Shapes *pInput, CSG_Shapes *pOutput);

	void					Clip_Points				(CSG_Shapes *pClip, CSG_Shapes *pInput, CSG_Shapes *pOutput);
	void					Clip_Lines				(CSG_Shapes *pClip, CSG_Shapes *pInput, CSG_Shapes *pOutput);
	void					Clip_Polygons			(CSG_Shapes *pClip, CSG_Shapes *pInput, CSG_Shapes *pOutput);

	TSG_Point				Get_Crossing			(CSG_Shape_Polygon *pPolygon, const TSG_Point &a, const TSG_Point &b);

	bool					Dissolve				(CSG_Shapes *pPolygons, CSG_Shapes *pOutput);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Polygon_Clip_H
