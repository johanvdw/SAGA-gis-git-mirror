
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
//                    data_source.h                      //
//                                                       //
//          Copyright (C) 2011 by Olaf Conrad            //
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
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_GUI__Data_Source_H
#define _HEADER_INCLUDED__SAGA_GUI__Data_Source_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/notebook.h>


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CData_Source : public wxNotebook
{
public:
	CData_Source(wxWindow *pParent);
	virtual ~CData_Source(void);

	void							Add_Pages				(void);

	void							Autoconnect_DB			(void);

	class CData_Source_Files *		Get_Files				(void)	{	return( m_pFiles );	}
	class CData_Source_ODBC *		Get_ODBC				(void)	{	return( m_pODBC  );	}
	class CData_Source_PgSQL *		Get_PgSQL				(void)	{	return( m_pPgSQL );	}

	bool							Set_Data_Source			(class CWKSP_Base_Item *pItem);
	bool							Update_Database			(const wxString &Server);


private:

	class CData_Source_Files		*m_pFiles;

	class CData_Source_ODBC			*m_pODBC;

	class CData_Source_PgSQL		*m_pPgSQL;


	bool							_Show_Page				(wxWindow *pPage);
	bool							_Hide_Page				(wxWindow *pPage);


//---------------------------------------------------------
DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------
extern CData_Source					*g_pData_Source;


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__Data_Source_H
