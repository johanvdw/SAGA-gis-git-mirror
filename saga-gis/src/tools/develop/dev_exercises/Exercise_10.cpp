
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       Lectures                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   Exercise_10.cpp                     //
//                                                       //
//                 Copyright (C) 2003 by                 //
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
#include "Exercise_10.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define ALIVE 0


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CExercise_10::CExercise_10(void)
{
	Set_Name		(_TL("10: Dynamic Simulation - Life"));

	Set_Author		("O.Conrad (c) 2006");

	Set_Description	(_TW(
		"Conway's game of life. A cellular automaton."
	));

	Add_Reference("Conrad, O.", "2007",
		"SAGA - Entwurf, Funktionsumfang und Anwendung eines Systems für Automatisierte Geowissenschaftliche Analysen",
		"ediss.uni-goettingen.de.",
		SG_T("https://ediss.uni-goettingen.de/handle/11858/00-1735-0000-0006-B26C-6"), SG_T("online")
	);

	Add_Reference("O. Conrad, B. Bechtel, M. Bock, H. Dietrich, E. Fischer, L. Gerlitz, J. Wehberg, V. Wichmann, and J. Böhner", "2015",
		"System for Automated Geoscientific Analyses (SAGA) v. 2.1.4",
		"Geoscientific Model Development, 8, 1991-2007.",
		SG_T("https://doi.org/10.5194/gmd-8-1991-2015"), SG_T("doi:10.5194/gmd-8-1991-2015")
	);


	//-----------------------------------------------------
	Parameters.Add_Grid("",
		"RESULT" , _TL("Life"),
		_TL(""),
		PARAMETER_OUTPUT, true, SG_DATATYPE_Byte
	);

	Parameters.Add_Bool("",
		"REFRESH", _TL("Refresh"),
		_TL(""),
		true
	);

	Parameters.Add_Int("",
		"COLORS" , _TL("Fade Color Count"),
		_TL(""),
		m_nColors, 3, true, 255, true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CExercise_10::On_Execute(void)
{
	m_pLife = Parameters("RESULT")->asGrid();
	m_pLife->Set_NoData_Value(-1);

	CSG_Colors Colors;
	Colors.Set_Count(1 + (m_nColors = Parameters("COLORS")->asInt()));
	Colors.Set_Ramp(SG_GET_RGB(127, 127, 127), SG_GET_RGB(0, 0, 0));
	Colors.Set_Color(0, SG_GET_RGB(255, 255, 255));
	DataObject_Set_Colors(m_pLife, Colors);

	//-----------------------------------------------------
	if( Parameters("REFRESH")->asBool() ) // Initialise life's world...
	{
		for(int y=0; y<Get_NY(); y++) for(int x=0; x<Get_NX(); x++)
		{
			m_pLife->Set_Value(x, y, CSG_Random::Get_Uniform(0, 100) < 25 ? ALIVE : 1);
		}
	}

	//-----------------------------------------------------
	m_Life.Create(m_pLife, SG_DATATYPE_Byte); // temporary

	for(int i=1; Process_Get_Okay(); i++)
	{
		Process_Set_Text("%d %s", i, _TL("Life Cycle"));

		if( Next_Step() == false )
		{
			Message_Fmt("\n%s %d %s\n", _TL("Dead after"), i, _TL("Life Cycles"));

			break;
		}
	}

	m_Life.Destroy();

	SG_UI_Process_Set_Okay();

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CExercise_10::Next_Step(void)
{
	bool bAlive = false;

	//-----------------------------------------------------
	#pragma omp parallel for
	for(int y=0; y<Get_NY(); y++) for(int x=0; x<Get_NX(); x++)
	{
		//-------------------------------------------------
		// Count neighbours...

		int	n = 0;

		for(int i=0; i<8; i++)
		{
			int ix = Get_xTo(i, x); if( ix < 0 ) { ix = Get_NX() - 1; } else if( ix >= Get_NX() ) { ix = 0; }
			int iy = Get_yTo(i, y); if( iy < 0 ) { iy = Get_NY() - 1; } else if( iy >= Get_NY() ) { iy = 0; }

			if( m_pLife->asByte(ix, iy) == ALIVE )
			{
				n++;
			}
		}

		//-------------------------------------------------
		// Dead or alive...

		int c = m_pLife->asByte(x, y); // current status

		switch( n )
		{
		case  3: // birth if not yet alive...
			c = ALIVE;
			break;

		case  2: // keep status...
			if( c != ALIVE )
			{
				c++; // color fading...
			}
			break;

		default: // dead...
			if( c < m_nColors )
			{
				c++; // color fading...
			}
		}

		m_Life.Set_Value(x, y, c);

		if( bAlive == false && c > 1 && c < m_nColors )
		{
			#pragma omp critical
			{
				bAlive = true;
			}
		}
	}

	//-----------------------------------------------------
	m_pLife->Assign(&m_Life);

	DataObject_Update(m_pLife, 0, m_nColors, 1);

	return( bAlive );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
