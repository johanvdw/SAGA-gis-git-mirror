
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                  statistics_kriging                   //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  variogram_dialog.h                   //
//                                                       //
//                 Olaf Conrad (C) 2008                  //
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
//    contact:    SAGA User Group Association            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Variogram_Dialog_H
#define HEADER_INCLUDED__Variogram_Dialog_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_api/saga_api.h>
#include <saga_gdi/saga_gdi.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CSG_Variogram
{
public:
	enum ESG_Variogram_Field
	{
		FIELD_CLASS	= 0,
		FIELD_DISTANCE,
		FIELD_COUNT,
		FIELD_VAR_EXP,
		FIELD_VAR_CUM,
		FIELD_VAR_MODEL
	};

	CSG_Variogram(void)	{}

	static bool		Calculate			(const CSG_Matrix &Points, CSG_Table *pVariogram, int nClasses = 25, double maxDistance = 0.0, int nSkip = 1);

	static bool		Get_Extent			(const CSG_Matrix &Points, CSG_Matrix &Extent);

	static double	Get_Diagonal		(const CSG_Matrix &Points);

	static double	Get_Lag_Distance	(const CSG_Matrix &Points, int Method, int nSkip = 1);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifdef WITH_GUI
class CVariogram_Dialog : public CSGDI_Dialog
{
public:
	CVariogram_Dialog(void);

	bool						Execute		(const CSG_Matrix &Points, CSG_Table *pVariogram, CSG_Trend *pModel);


private:

	wxButton					*m_pSettings;

	wxCheckBox					*m_pPairs;

	wxChoice					*m_pFormulas;

	wxTextCtrl					*m_pFormula, *m_pSummary;

	CSGDI_Slider				*m_pDistance;

	class CVariogram_Diagram	*m_pDiagram;

	CSG_Parameters				m_Settings;

	CSG_Trend					*m_pModel;

	CSG_Table					*m_pVariogram;

	const CSG_Matrix			*m_pPoints;

	CSG_Rect					m_Extent;

	bool						m_bLog;

	int							m_Field, m_zField, m_nPoints;

	double						m_Distance, m_Diagonal;


	void						On_Update_Control		(wxCommandEvent &event);
	void						On_Update_Choices		(wxCommandEvent &event);
	void						On_Button				(wxCommandEvent &event);

	void						Set_Variogram			(void);
	void						Set_Model				(void);

	const char *				Get_Formula				(int Index);
	CSG_String					Get_Formula				(void);


	DECLARE_EVENT_TABLE()

};
#else
class CVariogram_Dialog
{
};
#endif

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Variogram_Dialog_H
