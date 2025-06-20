
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                      Project.h                        //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
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
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_GUI__Project_H
#define _HEADER_INCLUDED__SAGA_GUI__Project_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Project
{
public:
	CWKSP_Project(void);
	virtual ~CWKSP_Project(void);

	const wxString &		Get_File_Name		(void)	{	return( m_File_Name );	}
	bool					Has_File_Name		(void);
	bool					Clr_File_Name		(void);

	bool					Load				(bool bAdd);
	bool					Load				(const wxString &FileName, bool bAdd, bool bUpdateMenu);

	bool					Save				(void);
	bool					Save				(bool bSaveAsOnError);
	bool					Save				(const wxString &FileName, bool bSaveModified);

	bool					Copy				(void);
	bool					CopyToDB			(void);


private:

	wxString				m_File_Name;

	bool					_Set_Project_Name	(void);

	bool					_Load				(const wxString &FileName, bool bAdd, bool bUpdateMenu);
	bool					_Save				(const wxString &FileName, bool bSaveModified, bool bUpdateMenu);

	bool					_Load_DBConnections	(const CSG_MetaData &Data);

	bool					_Load_Data			(const CSG_MetaData &Entry, const wxString &ProjectDir, bool bLoad, const CSG_String &Version);
	bool					_Save_Data			(      CSG_MetaData &Entry, const wxString &ProjectDir, class CWKSP_Data_Item *pItem);

	bool					_Load_Map			(const CSG_MetaData &Entry, const wxString &ProjectDir);
	bool					_Save_Map			(      CSG_MetaData &Entry, const wxString &ProjectDir, class CWKSP_Map *pMap);

	bool					_Copy_To_File		(class CWKSP_Data_Item *pData, const wxString &Directory);
	bool					_Copy_To_Database	(class CWKSP_Data_Item *pData, const wxString &Connection);

	//-----------------------------------------------------
	bool	_Compatibility_Load_Data	(const wxString &FileName);
	bool	_Compatibility_Load_Data	(CSG_File &Stream, const wxString &ProjectDir);
	bool	_Compatibility_Load_Map		(CSG_File &Stream, const wxString &ProjectDir);
	//-----------------------------------------------------

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__Project_H
