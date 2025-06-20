
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
//                    DLG_Colors.h                       //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__DLG_Colors_H
#define _HEADER_INCLUDED__SAGA_GUI__DLG_Colors_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "dlg_base.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CDLG_Colors : public CDLG_Base
{
	DECLARE_CLASS(CDLG_Colors)

public:
	CDLG_Colors(class CSG_Colors *pColors);
	virtual ~CDLG_Colors(void);

	void						On_Load			(wxCommandEvent &event);
	void						On_Save			(wxCommandEvent &event);


protected:

	class CSG_Colors			*m_pColors, *m_pOriginal;

	class CColorPresets			*m_pPresets;

	class CDLG_Colors_Control	*m_pControl;


	virtual void				Set_Position	(wxRect r);

	virtual void				Save_Changes	(void);

	void						On_Count		(wxCommandEvent &event);
	void						On_Mirror		(wxCommandEvent &event);
	void						On_Invert		(wxCommandEvent &event);
	void						On_Greyscale	(wxCommandEvent &event);
	void						On_Random		(wxCommandEvent &event);
	void						On_Preset		(wxCommandEvent &event);
	void						On_ComboBox		(wxCommandEvent &event);
	void						On_CheckBox		(wxCommandEvent &event);


	//-----------------------------------------------------
	DECLARE_EVENT_TABLE()
};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__DLG_Colors_H
