/**********************************************************
 * Version $Id: TLB_Interface.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                        ihacres                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   TLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2003 by                 //
//                        Author                         //
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
//    e-mail:     author@email.de                        //
//                                                       //
//    contact:    Author                                 //
//                Sesame Street. 7                       //
//                12345 Metropolis                       //
//                Nirvana                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//           The Tool Link Library Interface             //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// 1. Include the appropriate SAGA-API header...

#include "MLB_Interface.h"


//---------------------------------------------------------
// 2. Place general tool library information here...

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case TLB_INFO_Name:	default:
		return( _TL("Hydrology: IHACRES") );

	case TLB_INFO_Category:
		return( _TL("Simulation") );

	case TLB_INFO_Author:
		return( SG_T("Stefan Liersch (c) 2008") );

	case TLB_INFO_Description:
		return( _TW(
			"The metric conceptual rainfall-runoff model <b>IHACRES</b> "
			"(Identification of unit Hydrographs and Component flows "
			"from Rainfall, Evaporation and Streamflow data) has been "
			"implemented as a module library. The IHACRES model was "
			"developed by: Jakeman, A.J. and G.M. Hornberger (1993); Jakeman et al. (1990).\n"
			"\n"
			"<b>Purpose</b>\n"
			"The tool can be used to simulate streamflow (catchment runoff) "
			"on the basis of daily rainfall and temperature data. No spatial "
			"data, such as elevation models, soil or land use maps are required.\n"
			"\n"
			"IHACRES has been applied to catchments with a wide range of climatologies "
			"and sizes (Croke et al., 2004). It has been used to predict streamflow in "
			"ungauged catchments (Kokkonen et al., 2003; Post and Jakeman, 1999; "
			"Post et al., 1998), to study land cover effects on hydrologic processes "
			"(Croke et al., 2004; Kokkonen and Jakeman, 2002), and to investigate dynamic "
			"response characteristics and physical catchment descriptors (Kokkonen et al., "
			"2003; Sefton and Howarth, 1998).\n"
			"\n"
			"More recently, the model has been used to develop a "
			"<a target=\"_blank\" href=\"http://www.ufz.de/index.php?en=17175\">rainfall-runoff database</a> "
			"for flood risk assessment and forecasting by Liersch, S. and M. Volk (2008) "
			"(<a target=\"_blank\" href=\"http://www.iemss.org/iemss2008/uploads/Main/S05-12_Liersch_et_al-IEMSS2008.pdf\">pdf</a>).\n"
			"\n"
			"<b>References</b>\n<ul>"
			"<li>Croke, B.F.W., Merritt, W.S., Jakeman, A.J., 2004. A dynamic model for predicting hydrologic response to land cover changes in gauged and ungauged catchments. Journal Of Hydrology 291 (1), 115-31.</li>\n"
			"<li>Jakeman, A.J., Littlewood, I.G., Whitehead, P.G., 1990. Computation of the instantaneous unit hydrograph and identifiable component flows with application to two small upland catchments. Journal of Hydrology 117 (1-4), 275-300.</li>\n"
			"<li>Jakeman, A.J. and Hornberger, G.M., 1993. How Much Complexity Is Warranted in a Rainfall-Runoff Model?. Water Resources Research 29 (8), 2637-49.</li>\n"
			"<li>Kokkonen, T.S., Jakeman, A.J., Young, P.C., Koivusalo, H.J., 2003. Predicting daily flows in ungauged catchments: model regionalization from catchment descriptors at the Coweeta Hydrologic Laboratory. North Carolina Hydrological Processes 17 (11), 2219-38.</li>\n"
			"<li>Kokkonen, T.S. and Jakeman, A.J., 2002. Structural Effects of Landscape and Land Use on Streamflow Response. In: Environmental Foresight and Models: A Manifesto, 303-321.</li>\n"
			"<li>Liersch, S. and M. Volk, 2008. A rainfall-runoff database to support flood risk assessment. iEMSs 2008: International Congress on Environmental Modelling and Software. In: M. Sanchez-Marre, J. Bejar, J. Comas, A. Rizzoli and G. Guariso (Eds.): Proceedings of the iEMSs Fourth Biennial Meeting: International Congress on Environmental Modelling and Software (iEMSs 2008). International Environmental Modelling and Software Society, Barcelona, Catalonia, July 2008. Vol. 1: 494-502. ISBN: 978-84-7653-074-0. (PEER reviewed).</li>\n"
			"<li>Post, D.A. and Jakeman, A.J., 1999. Predicting the daily streamflow of ungauged catchments in S.E. Australia by regionalising the parameters of a lumped conceptual rainfall-runoff model. Ecological Modelling 123 (2-3), 91-104.</li>\n"
			"<li>Post, D.A., Jones, J.A. and Grant, G.E., 1998. An improved methodology for predicting the daily hydrologic response of ungauged catchments. Environmental Modelling & Software 13 (3-4), 395-403.</li>\n"
			"<li>Sefton, C.E.M. and Howarth, S.M., 1998. Relationships between dynamic response characteristics and physical descriptors of catchments in England and Wales. Journal of Hydrology 211 (1-4), 1-16.</li>\n"
			"</ul>"
		));

	case TLB_INFO_Version:
		return( SG_T("1.0") );

	case TLB_INFO_Menu_Path:
		return( _TL("Simulation|Hydrology|IHACRES") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your tools here...

//#include "ihacres_cal.h"
#include "ihacres_cal2.h"
#include "ihacres_v1.h"
#include "ihacres_elev.h"
#include "ihacres_elev_cal.h"
//#include "ihacres_climate_scen.h"
//#include "ihacres_climate_scen_db.h"
#include "ihacres_basin.h"
//#include "ihacres_auto_cal.h"

//---------------------------------------------------------
// 4. Allow your tools to be created here...

CSG_Tool *		Create_Tool(int i)
{
	// Don't forget to continuously enumerate the case switches
	// when adding new tools! Also bear in mind that the
	// enumeration always has to start with [case 0:] and
	// that [default:] must return NULL!...

	CSG_Tool	*pTool;

	switch( i )
	{
	//case 0:
	//	pTool = new Cihacres_cal;
	//	break;
	case 0:
		pTool = new Cihacres_cal2;
		break;
	case 1:
		pTool = new Cihacres_v1;
		break;
	case 2:
		pTool = new Cihacres_basin;
		break;
	case 3:
		pTool = new Cihacres_elev;
		break;
	case 4:
		pTool = new Cihacres_elev_cal;
		break;
	//case 6:
	//	pTool = new Cihacres_climate_scen;
	//	break;
	//case 7:
	//	pTool = new Cihacres_climate_scen_db;
	//	break;

	default:
		pTool	= NULL;
		break;
	}

	return( pTool );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
//{{AFX_SAGA

	TLB_INTERFACE

//}}AFX_SAGA
