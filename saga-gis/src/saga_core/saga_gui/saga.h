
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
//                        SAGA.h                         //
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
#ifndef _SAGA_GUI__SAGA_H
#define _SAGA_GUI__SAGA_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/app.h>


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CSAGA : public wxApp
{
public:
	CSAGA(void);
	virtual ~CSAGA(void);

	virtual bool				OnInit					(void);
	virtual int					OnExit					(void);

	const wxString &			Get_App_Path			(void)	const	{	return( m_App_Path );	}

	bool						Process_Wait			(bool bEnforce = false);

	bool						Process_Set_Okay		(bool bOkay);
	bool						Process_Get_Okay		(void);

	bool						Process_Set_Frequency	(size_t Milliseconds);
	int							Process_Get_Frequency	(void)	const	{	return( m_Process_Frequency );		}

	bool						Set_Busy				(bool bOn, const CSG_String &Message = "");


private:

	bool						m_Process_bContinue;

	size_t						m_Process_Frequency;

	wxString					m_App_Path;
	
	wxLocale					m_wxLocale;


	void						_Init_Config			(void);

	void						On_Key_Down				(wxKeyEvent &event);


	//-----------------------------------------------------
	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------
extern CSAGA					*g_pSAGA;


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _SAGA_GUI__SAGA_H
