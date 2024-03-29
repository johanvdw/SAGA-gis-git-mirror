
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                      io_shapes                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    citygml_import.h                   //
//                                                       //
//                 Copyright (C) 2014 by                 //
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
#ifndef HEADER_INCLUDED__citygml_import_H
#define HEADER_INCLUDED__citygml_import_H


///////////////////////////////////////////////////////////
//                                                       //												
//                                                       //												
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_api/saga_api.h>


///////////////////////////////////////////////////////////
//                                                       //												
//                                                       //												
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CCityGML_Import : public CSG_Tool
{
public:
	CCityGML_Import(void);

	virtual CSG_String	Get_MenuPath			(void)	{	return( _TL("Import") );	}


protected:

	virtual bool		On_Execute				(void);


private:

	bool				Load_Shapes				(const CSG_String &File, CSG_Shapes &Shapes, int Type);

	bool				Get_Buildings			(const CSG_String &File, CSG_Shapes *pPolygons, bool bParts);
	bool				Get_Buildings			(const CSG_String &File, CSG_Shapes *pPolygons);

	bool				Add_Buildings			(CSG_Shapes *pBuildings, CSG_Shapes *pAdd);

	bool				Has_BuildingParts		(const CSG_MetaData &GML);
	bool				Add_BuildingParts		(const CSG_MetaData &GML, CSG_MetaData &GML_Parts);

};


///////////////////////////////////////////////////////////
//                                                       //												
//                                                       //												
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__citygml_import_H
