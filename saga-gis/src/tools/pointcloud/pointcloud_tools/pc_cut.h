
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
//                       pc_cut.h                        //
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
#ifndef HEADER_INCLUDED__PC_Cut_H
#define HEADER_INCLUDED__PC_Cut_H


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
class CPC_Cut : public CSG_Tool
{
public:
	CPC_Cut(void);

	virtual CSG_String				Get_MenuPath			(void)	{	return( _TL("Tools") );	}

	static bool						Get_Cut					(CSG_Parameter_PointCloud_List *pPointsList, CSG_Parameter_PointCloud_List *pCutList, const CSG_Rect &Extent, bool bInverse);
	static bool						Get_Cut					(CSG_Parameter_PointCloud_List *pPointsList, CSG_Parameter_PointCloud_List *pCutList, CSG_Shapes *pPolygons, bool bInverse);


protected:

	virtual int						On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);
	virtual int						On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual bool					On_Execute				(void);


private:

	static bool						Contains				(CSG_Shapes *pPolygons, double x, double y);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CPC_Cut_Interactive : public CSG_Tool_Interactive
{
public:
	CPC_Cut_Interactive(void);

	virtual CSG_String				Get_MenuPath			(void)	{	return( _TL("Tools") );	}


protected:

	virtual bool					On_Execute				(void);
	virtual bool					On_Execute_Position		(CSG_Point ptWorld, TSG_Tool_Interactive_Mode Mode);


private:

	CSG_Point						m_ptDown;

	CSG_Parameter_PointCloud_List	*m_pPointsList, *m_pCutList;

	CSG_Shapes						*m_pAOI;

	bool							m_bAOIBox, m_bAdd, m_bInverse;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__PC_Cut_H
