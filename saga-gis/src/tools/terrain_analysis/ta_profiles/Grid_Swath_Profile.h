
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                     ta_profiles                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  Grid_Swath_Profile.h                 //
//                                                       //
//                 Copyright (C) 2005 by                 //
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
#ifndef HEADER_INCLUDED__Grid_Swath_Profile_H
#define HEADER_INCLUDED__Grid_Swath_Profile_H


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
class CGrid_Swath_Profile : public CSG_Tool_Grid_Interactive
{
public:
	CGrid_Swath_Profile(void);


protected:

	virtual bool				On_Execute			(void);
	virtual bool				On_Execute_Finish	(void);
	virtual bool				On_Execute_Position	(CSG_Point ptWorld, TSG_Tool_Interactive_Mode Mode);


private:

	bool						m_bAdd{false};

	double						m_Width{100.};

	CSG_Shapes					*m_pPoints{NULL}, *m_pLine{NULL};

	CSG_Grid					*m_pDEM{NULL};

	CSG_Parameter_Grid_List		*m_pValues{NULL};


	bool						Set_Profile	(void);
	bool						Set_Profile	(CSG_Point A, CSG_Point B, CSG_Point Left, CSG_Point Right);

	bool						Add_Point	(CSG_Point Point, CSG_Point Left, CSG_Point Right, CSG_Point Step);
	bool						Add_Swath	(CSG_Shape *pPoint, int iEntry, CSG_Grid *pGrid, CSG_Point Left, CSG_Point Right, CSG_Point Step);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Grid_Swath_Profile_H
