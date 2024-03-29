
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     image_tools                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  spot_scene_import.h                  //
//                                                       //
//                 Olaf Conrad (C) 2024                  //
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
#ifndef HEADER_INCLUDED__spot_scene_import_H
#define HEADER_INCLUDED__spot_scene_import_H


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
class CSPOT_Scene_Import : public CSG_Tool
{
public:
	CSPOT_Scene_Import(void);

	virtual CSG_String		Get_MenuPath			(void)	{	return( _TL("A:File|Satellite Imagery") );	}


protected:

	virtual int				On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual bool			On_Execute				(void);


private:

	bool					Load_Metadata			(CSG_MetaData &Metadata, const CSG_String &File, bool bVerbose = true);

	CSG_String				Get_File_Path			(const CSG_MetaData &Metadata, const CSG_String &Root);

	bool					Set_Band_Info			(CSG_Grids *pBands, int Band, int Mission);

	static bool				Get_Reference_Frame		(const CSG_MetaData &Metadata, CSG_Shapes &Frame);
	static bool				Set_Reference_UTM		(CSG_Shapes &Frame, int UTM_Zone, bool UTM_South);

	bool					Georeference			(const CSG_MetaData &Metadata, CSG_Grids &Bands);

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__spot_scene_import_H
