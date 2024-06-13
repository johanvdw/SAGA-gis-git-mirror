
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                imagery_classification                 //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//               Grid_Cluster_Analysis.cpp               //
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
#include "classify_cluster_analysis.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_Cluster_Analysis::CGrid_Cluster_Analysis(void)
{
	Set_Name		(_TL("K-Means Clustering for Grids"));

	Set_Author		("O.Conrad (c) 2001");

	Set_Description	(_TW(		
		"This tool implements the K-Means cluster analysis for grids "
		"in two variants, iterative minimum distance (Forgy 1965) "
		"and hill climbing (Rubin 1967). "
	));
	
	Add_Reference("Forgy, E.", "1965",
		"Cluster analysis of multivariate data: efficiency vs. interpretability of classifications",
		"Biometrics 21:768."
	);

	Add_Reference("Rubin, J.", "1967",
		"Optimal classification into groups: an approach for solving the taxonomy problem",
		"J. Theoretical Biology, 15:103-144."
	);

	//-----------------------------------------------------
	Parameters.Add_Grid_List("",
		"GRIDS"		, _TL("Grids"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid("",
		"CLUSTER"		, _TL("Clusters"),
		_TL(""),
		PARAMETER_OUTPUT, true, SG_DATATYPE_Byte
	);

	Parameters.Add_Table("",
		"STATISTICS"	, _TL("Statistics"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Choice("",
		"METHOD"		, _TL("Method"),
		_TL(""),
		CSG_String::Format("%s|%s|%s",
			_TL("Iterative Minimum Distance (Forgy 1965)"),
			_TL("Hill-Climbing (Rubin 1967)"),
			_TL("Combined Minimum Distance / Hillclimbing") 
		), 1
	);

	Parameters.Add_Int("",
		"NCLUSTER"		, _TL("Clusters"),
		_TL("Number of clusters"),
		10, 2, true
	);

	Parameters.Add_Int("",
		"MAXITER"		, _TL("Maximum Iterations"),
		_TL("Maximum number of iterations, ignored if set to zero."),
		10, 0, true
	);

	Parameters.Add_Bool("",
		"NORMALISE"		, _TL("Normalise"),
		_TL("Automatically normalise grids by standard deviation before clustering."),
		false
	);

	Parameters.Add_Bool("",
		"RGB_COLORS"	, _TL("Update Colors from Features"),
		_TL("Use the first three features in list to obtain blue, green, red components for class colour in look-up table."),
		false
	)->Set_UseInCMD(false);

	Parameters.Add_Choice("",
		"INITIALIZE"	, _TL("Start Partition"),
		_TL(""),
		CSG_String::Format("%s|%s|%s",
			_TL("random"),
			_TL("periodical"),
			_TL("keep values") 
		), 0
	);

	//-----------------------------------------------------
	Parameters.Add_Bool(""          , "OLDVERSION", _TL("Old Version"), _TL("slower but memory saving"), false);
	Parameters.Add_Bool("OLDVERSION", "UPDATEVIEW", _TL("Update View"), _TL(""), true)->Set_UseInCMD(false);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CGrid_Cluster_Analysis::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( pParameter->Cmp_Identifier("OLDVERSION") )
	{
		pParameters->Set_Enabled("INITIALIZE", pParameter->asBool() == false);
		pParameters->Set_Enabled("UPDATEVIEW", pParameter->asBool() == true );
	}

	if( pParameter->Cmp_Identifier("GRIDS") )
	{
		pParameters->Set_Enabled("RGB_COLORS", pParameter->asGridList()->Get_Grid_Count() >= 3);
	}

	return( CSG_Tool_Grid::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_Cluster_Analysis::On_Execute(void)
{
	if( Parameters("OLDVERSION")->asBool() ) {	return( _On_Execute() );	}

	//-----------------------------------------------------
	CSG_Parameter_Grid_List	*pGrids	= Parameters("GRIDS")->asGridList();

	CSG_Grid *pCluster = Parameters("CLUSTER")->asGrid();

	bool bNormalize = Parameters("NORMALISE")->asBool();

	//-----------------------------------------------------
	CSG_Cluster_Analysis Analysis;

	if( !Analysis.Create(pGrids->Get_Grid_Count()) )
	{
		return( false );
	}

	pCluster->Set_NoData_Value(0);

	//-----------------------------------------------------
	for(sLong iElement=0, nElements=0; iElement<Get_NCells() && Set_Progress_Cells(iElement); iElement++)
	{
		bool bNoData = false;

		for(int iFeature=0; iFeature<pGrids->Get_Grid_Count() && !bNoData; iFeature++)
		{
			if( pGrids->Get_Grid(iFeature)->is_NoData(iElement) )
			{
				bNoData = true;
			}
		}

		if( bNoData || !Analysis.Add_Element() )
		{
			pCluster->Set_Value(iElement, 0);
		}
		else
		{
			pCluster->Set_Value(iElement, 1);

			for(int iFeature=0; iFeature<pGrids->Get_Grid_Count(); iFeature++)
			{
				double d = pGrids->Get_Grid(iFeature)->asDouble(iElement);

				if( bNormalize )
				{
					d = (d - pGrids->Get_Grid(iFeature)->Get_Mean()) / pGrids->Get_Grid(iFeature)->Get_StdDev();
				}

				Analysis.Set_Feature(nElements, iFeature, d);
			}

			nElements++;
		}
	}

	if( Analysis.Get_nElements() < 2 )
	{
		return( false );
	}

	//-----------------------------------------------------
	bool bResult = Analysis.Execute(
		Parameters("METHOD"    )->asInt(),
		Parameters("NCLUSTER"  )->asInt(),
		Parameters("MAXITER"   )->asInt(),
		Parameters("INITIALIZE")->asInt()
	);

	for(sLong iElement=0, nElements=0; iElement<Get_NCells(); iElement++)
	{
		Set_Progress_Cells(iElement);

		if( !pCluster->is_NoData(iElement) )
		{
			pCluster->Set_Value(iElement, 1 + (int)Analysis.Get_Cluster(nElements++));
		}
	}

	Save_Statistics(pGrids, bNormalize, Analysis);

	Save_LUT(pCluster);

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CGrid_Cluster_Analysis::Save_Statistics(CSG_Parameter_Grid_List *pGrids, bool bNormalize, const CSG_Cluster_Analysis &Analysis)
{
	CSG_Table &Statistics = *Parameters("STATISTICS")->asTable();

	Statistics.Destroy();
	Statistics.Set_Name(_TL("Cluster Analysis"));
	Statistics.Add_Field(_TL("ClusterID"), SG_DATATYPE_Int   );
	Statistics.Add_Field(_TL("Elements" ), SG_DATATYPE_Int   );
	Statistics.Add_Field(_TL("Std.Dev." ), SG_DATATYPE_Double);

	CSG_String s; s.Printf("\n%s:\t%d \n%s:\t%d \n%s:\t%d \n%s:\t%d \n%s:\t%f\n\n%s\t%s\t%s",
		_TL("Number of Iterations"), Analysis.Get_Iteration(),
		_TL("Number of Elements"  ), Analysis.Get_nElements(),
		_TL("Number of Variables" ), Analysis.Get_nFeatures(),
		_TL("Number of Clusters"  ), Analysis.Get_nClusters(),
		_TL("Standard Deviation"  ), sqrt(Analysis.Get_SP()),
		_TL("Cluster"), _TL("Elements"), _TL("Std.Dev.")
	);

	for(int iFeature=0; iFeature<Analysis.Get_nFeatures(); iFeature++)
	{
		s += CSG_String::Format("\t%s", pGrids->Get_Grid(iFeature)->Get_Name());

		Statistics.Add_Field(pGrids->Get_Grid(iFeature)->Get_Name(), SG_DATATYPE_Double);
	}

	Message_Add(s);

	for(int iCluster=0; iCluster<Analysis.Get_nClusters(); iCluster++)
	{
		s.Printf("\n%d\t%d\t%f", iCluster, Analysis.Get_nMembers(iCluster), sqrt(Analysis.Get_Variance(iCluster)));

		CSG_Table_Record &Record = *Statistics.Add_Record();

		Record.Set_Value(0, iCluster);
		Record.Set_Value(1, Analysis.Get_nMembers(iCluster));
		Record.Set_Value(2, sqrt(Analysis.Get_Variance(iCluster)));

		for(int iFeature=0; iFeature<Analysis.Get_nFeatures(); iFeature++)
		{
			double Centroid = Analysis.Get_Centroid(iCluster, iFeature);

			if( bNormalize )
			{
				Centroid = pGrids->Get_Grid(iFeature)->Get_Mean() + Centroid * pGrids->Get_Grid(iFeature)->Get_StdDev();
			}

			s += CSG_String::Format("\t%f", Centroid);

			Record.Set_Value(iFeature + 3, Centroid);
		}

		Message_Add(s, false);
	}
}

//---------------------------------------------------------
void CGrid_Cluster_Analysis::Save_LUT(CSG_Grid *pCluster)
{
	CSG_Parameter *pLUT = DataObject_Get_Parameter(pCluster, "LUT");

	if( pLUT && pLUT->asTable() )
	{
		CSG_Parameter_Grid_List *pGrids = Parameters("GRIDS")->asGridList();

		CSG_Table &Statistics = *Parameters("STATISTICS")->asTable();

		bool bRGB = pGrids->Get_Grid_Count() >= 3 && Parameters("RGB_COLORS")->asBool();

		for(int iCluster=0; iCluster<Statistics.Get_Count(); iCluster++)
		{
			CSG_Table_Record *pClass = pLUT->asTable()->Get_Record(iCluster);

			if( !pClass )
			{
				(pClass = pLUT->asTable()->Add_Record())->Set_Value(0, SG_Color_Get_Random());
			}

			pClass->Set_Value(1, CSG_String::Format("%s %d", _TL("Cluster"), iCluster + 1));
			pClass->Set_Value(2, "");
			pClass->Set_Value(3, iCluster + (Parameters("OLDVERSION")->asBool() ? 0 : 1));
			pClass->Set_Value(4, iCluster + (Parameters("OLDVERSION")->asBool() ? 0 : 1));

			if( bRGB )
			{
				#define SET_COLOR_COMPONENT(c, i) c = (int)(127 + (Statistics[iCluster].asDouble(3 + i) - pGrids->Get_Grid(i)->Get_Mean()) * 127 / pGrids->Get_Grid(i)->Get_StdDev()); if( c < 0 ) c = 0; else if( c > 255 ) c = 255;

				int	r; SET_COLOR_COMPONENT(r, 2);
				int	g; SET_COLOR_COMPONENT(g, 1);
				int	b; SET_COLOR_COMPONENT(b, 0);

				pClass->Set_Value(0, SG_GET_RGB(r, g, b));
			}
		}

		pLUT->asTable()->Set_Count(Statistics.Get_Count());

		DataObject_Set_Parameter(pCluster, pLUT);
		DataObject_Set_Parameter(pCluster, "COLORS_TYPE", 1);	// Color Classification Type: Lookup Table

		DataObject_Update(pCluster);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////
//														 //
//				Deprecated Old Version					 //
//														 //
///////////////////////////////////////////////////////////
//														 //
//				slow, but saves memory !				 //
//														 //
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_Cluster_Analysis::_On_Execute(void)
{
	int						i, j, *nMembers, nCluster;
	sLong					nElements;
	double					*Variances, **Centroids, SP;
	CSG_Grid				**Grids, *pCluster;
	CSG_Parameter_Grid_List	*pGrids;

	//-----------------------------------------------------
	pGrids		= Parameters("GRIDS")	->asGridList();
	pCluster	= Parameters("CLUSTER")	->asGrid();
	nCluster	= Parameters("NCLUSTER")->asInt();

	if( pGrids->Get_Grid_Count() < 1 )
	{
		return( false );
	}

	//-----------------------------------------------------
	Grids		= (CSG_Grid **)SG_Malloc(pGrids->Get_Grid_Count() * sizeof(CSG_Grid *));

	if( Parameters("NORMALISE")->asBool() )
	{
		for(i=0; i<pGrids->Get_Grid_Count(); i++)
		{
			Grids[i]	= SG_Create_Grid(pGrids->Get_Grid(i), SG_DATATYPE_Float);
			Grids[i]	->Assign(pGrids->Get_Grid(i));
			Grids[i]	->Standardise();
		}
	}
	else
	{
		for(i=0; i<pGrids->Get_Grid_Count(); i++)
		{
			Grids[i]	= pGrids->Get_Grid(i);
		}
	}

	pCluster->Set_NoData_Value(-1.);
	pCluster->Assign_NoData();

	nMembers	= (int     *)SG_Malloc(nCluster * sizeof(int));
	Variances	= (double  *)SG_Malloc(nCluster * sizeof(double));
	Centroids	= (double **)SG_Malloc(nCluster * sizeof(double *));

	for(i=0; i<nCluster; i++)
	{
		Centroids[i] = (double  *)SG_Malloc(pGrids->Get_Grid_Count() * sizeof(double));
	}

	//-------------------------------------------------
	switch( Parameters("METHOD")->asInt() )
	{
	case 0: SP = _MinimumDistance(Grids, pGrids->Get_Grid_Count(), pCluster, nCluster, nMembers, Variances, Centroids, nElements = Get_NCells()); break;
	case 1: SP = _HillClimbing   (Grids, pGrids->Get_Grid_Count(), pCluster, nCluster, nMembers, Variances, Centroids, nElements = Get_NCells()); break;
	case 2: SP = _MinimumDistance(Grids, pGrids->Get_Grid_Count(), pCluster, nCluster, nMembers, Variances, Centroids, nElements = Get_NCells());
	        SP = _HillClimbing   (Grids, pGrids->Get_Grid_Count(), pCluster, nCluster, nMembers, Variances, Centroids, nElements = Get_NCells()); break;
	}

	//-------------------------------------------------
	if( Parameters("NORMALISE")->asBool() )
	{
		for(i=0; i<pGrids->Get_Grid_Count(); i++)
		{
			delete(Grids[i]);

			for(j=0; j<nCluster; j++)
			{
				Centroids[j][i]	= pGrids->Get_Grid(i)->Get_StdDev() * Centroids[j][i] + pGrids->Get_Grid(i)->Get_Mean();
			}
		}
	}

	//-------------------------------------------------
	int					iCluster, iFeature;
	CSG_String			s;
	CSG_Table_Record	*pRecord;
	CSG_Table			*pTable;

	pTable	= Parameters("STATISTICS")->asTable();

	pTable->Destroy();
	pTable->Set_Name(_TL("Cluster Analysis"));

	pTable->Add_Field(_TL("ClusterID"), SG_DATATYPE_Int   );
	pTable->Add_Field(_TL("Elements" ), SG_DATATYPE_Int   );
	pTable->Add_Field(_TL("Std.Dev." ), SG_DATATYPE_Double);

	s.Printf("\n%s:\t%lld \n%s:\t%d \n%s:\t%d \n%s:\t%f\n\n%s\t%s\t%s",
		_TL("Number of Elements" ), nElements,
		_TL("Number of Variables"), pGrids->Get_Grid_Count(),
		_TL("Number of Clusters" ), nCluster,
		_TL("Standard Deviation" ), sqrt(SP),
		_TL("Cluster"), _TL("Elements"), _TL("Std.Dev.")
	);

	for(iFeature=0; iFeature<pGrids->Get_Grid_Count(); iFeature++)
	{
		s	+= CSG_String::Format("\t%s", pGrids->Get_Grid(iFeature)->Get_Name());

		pTable->Add_Field(pGrids->Get_Grid(iFeature)->Get_Name(), SG_DATATYPE_Double);
	}

	Message_Add(s);

	for(iCluster=0; iCluster<nCluster; iCluster++)
	{
		Variances[iCluster]	= nMembers[iCluster] ? Variances[iCluster] / nMembers[iCluster] : 0.;

		s.Printf("\n%d\t%d\t%f", iCluster, nMembers[iCluster], sqrt(Variances[iCluster]));

		pRecord	= pTable->Add_Record();
		pRecord->Set_Value(0, iCluster);
		pRecord->Set_Value(1, nMembers[iCluster]);
		pRecord->Set_Value(2, sqrt(Variances[iCluster]));

		for(iFeature=0; iFeature<pGrids->Get_Grid_Count(); iFeature++)
		{
			double	Centroid	= Centroids[iCluster][iFeature];

			if( Parameters("NORMALISE")->asBool() )
			{
				Centroid	= pGrids->Get_Grid(iFeature)->Get_Mean() + Centroid * pGrids->Get_Grid(iFeature)->Get_StdDev();
			}

			s	+= CSG_String::Format("\t%f", Centroid);

			pRecord->Set_Value(iFeature + 3, Centroid);
		}

		Message_Add(s, false);
	}

	//-------------------------------------------------
	Save_LUT(pCluster);

	//-------------------------------------------------
	for(i=0; i<nCluster; i++)
	{
		SG_Free(Centroids[i]);
	}

	SG_Free(Centroids);
	SG_Free(Variances);
	SG_Free(nMembers);
	SG_Free(Grids);

	return( true );
}

//---------------------------------------------------------
double CGrid_Cluster_Analysis::_MinimumDistance(CSG_Grid **Grids, int nGrids, CSG_Grid *pCluster, int nCluster, int *nMembers, double *Variances, double **Centroids, sLong &nElements)
{
	bool	bContinue;
	sLong	iElement;
	int		iGrid, iCluster, nClusterElements, nShifts, minCluster, nPasses;
	double	d, Variance, minVariance, SP, SP_Last	= -1;

	//-----------------------------------------------------
	for(iElement=0, nClusterElements=0; iElement<nElements; iElement++)
	{
		for(iGrid=0, bContinue=true; iGrid<nGrids && bContinue; iGrid++)
		{
			if( Grids[iGrid]->is_NoData(iElement) )
			{
				bContinue	= false;
			}
		}

		if( bContinue )
		{
			if( pCluster->asInt(iElement) < 0 || pCluster->asInt(iElement) >= nCluster )
			{
				pCluster->Set_Value(iElement, (int)(iElement % nCluster));
			}

			nClusterElements++;
		}
		else
		{
			pCluster->Set_Value(iElement, -1);
		}
	}

	if( Parameters("UPDATEVIEW")->asBool() )
	{
		DataObject_Update(pCluster, 0, nCluster, 1);
	}

	//-----------------------------------------------------
	int	maxIter	= Parameters("MAXITER")->asInt();

	for(nPasses=1, bContinue=true; bContinue && (maxIter==0 || nPasses<=maxIter) && Process_Get_Okay(false); nPasses++)
	{
		for(iCluster=0; iCluster<nCluster; iCluster++)
		{
			Variances[iCluster]	= 0;
			nMembers [iCluster]	= 0;

			for(iGrid=0; iGrid<nGrids; iGrid++)
			{
				Centroids[iCluster][iGrid]	= 0;
			}
		}

		//-------------------------------------------------
		for(iElement=0; iElement<nElements; iElement++)
		{
			if( pCluster->asInt(iElement) >= 0 )
			{
				iCluster	= pCluster->asInt(iElement);
				nMembers[iCluster]++;

				for(iGrid=0; iGrid<nGrids; iGrid++)
				{
					Centroids[iCluster][iGrid]	+= Grids[iGrid]->asDouble(iElement);
				}
			}
		}

		//-------------------------------------------------
		for(iCluster=0; iCluster<nCluster; iCluster++)
		{
			d		= nMembers[iCluster] > 0 ? 1. / (double)nMembers[iCluster] : 0;

			for(iGrid=0; iGrid<nGrids; iGrid++)
			{
				Centroids[iCluster][iGrid]	*= d;
			}
		}

		//-------------------------------------------------
		SP		= 0;
		nShifts	= 0;

		for(iElement=0; iElement<nElements && bContinue; iElement++)
		{
			if( pCluster->asInt(iElement) >= 0 )
			{
				minVariance	= -1;

				for(iCluster=0; iCluster<nCluster; iCluster++)
				{
					Variance	= 0;

					for(iGrid=0; iGrid<nGrids; iGrid++)
					{
						d			= Centroids[iCluster][iGrid] - Grids[iGrid]->asDouble(iElement);
						Variance	+= d * d;
					}

					if( minVariance<0 || Variance<minVariance )
					{
						minVariance	= Variance;
						minCluster	= iCluster;
					}
				}

				if( pCluster->asInt(iElement) != minCluster )
				{
					pCluster->Set_Value(iElement, minCluster);
					nShifts++;
				}

				SP						+= minVariance;
				Variances[minCluster]	+= minVariance;
			}
		}

		//-------------------------------------------------
		if( nShifts == 0 )//|| (SP_Last >= 0 && SP >= SP_Last) )
		{
			bContinue	= false;
		}

		SP	/= nElements;

		Process_Set_Text("%s: %d >> %s %f", _TL("pass"), nPasses, _TL("change"), SP_Last < 0. ? SP : SP_Last - SP);

		SP_Last		= SP;

		if( Parameters("UPDATEVIEW")->asBool() )
		{
			DataObject_Update(pCluster);
		}
	}

	nElements	= nClusterElements;

	return( SP );
}

//---------------------------------------------------------
double CGrid_Cluster_Analysis::_HillClimbing(CSG_Grid **Grids, int nGrids, CSG_Grid *pCluster, int nCluster, int *nMembers, double *Variances, double **Centroids, sLong &nElements)
{
	bool	bContinue;
	sLong	iElement;
	int		iGrid, iCluster, jCluster, kCluster, nClusterElements, noShift, nPasses;
	double	d, e, n_iK, n_jK, V, VMin, V1, V2, SP, SP_Last	= -1;

	//-----------------------------------------------------
	for(iCluster=0; iCluster<nCluster; iCluster++)
	{
		Variances[iCluster]	= 0;
		nMembers [iCluster]	= 0;

		for(iGrid=0; iGrid<nGrids; iGrid++)
		{
			Centroids[iCluster][iGrid]	= 0;
		}
	}

	//-----------------------------------------------------
	for(iElement=0, nClusterElements=0; iElement<nElements; iElement++)
	{
		for(iGrid=0, bContinue=true; iGrid<nGrids && bContinue; iGrid++)
		{
			if( Grids[iGrid]->is_NoData(iElement) )
			{
				bContinue	= false;
			}
		}

		if( bContinue )
		{
			if( pCluster->asInt(iElement) < 0 || pCluster->asInt(iElement) >= nCluster )
			{
				pCluster->Set_Value(iElement, (int)(iElement % nCluster));
			}

			nClusterElements++;

			iCluster	= pCluster->asInt(iElement);

			nMembers[iCluster]++;

			V			= 0.;

			for(iGrid=0; iGrid<nGrids; iGrid++)
			{
				d							 = Grids[iGrid]->asDouble(iElement);
				Centroids[iCluster][iGrid]	+= d;
				V							+= d * d;
			}

			Variances[iCluster]	+= V;
		}
		else
		{
			pCluster->Set_Value(iElement, -1);
		}
	}

	//-----------------------------------------------------
	for(iCluster=0; iCluster<nCluster; iCluster++)
	{
		d	= nMembers[iCluster] != 0 ? 1. / (double)nMembers[iCluster] : 0;
		V	= 0.;

		for(iGrid=0; iGrid<nGrids; iGrid++)
		{
			Centroids[iCluster][iGrid]	*= d;
			e							 = Centroids[iCluster][iGrid];
			V							+= e * e;
		}

		Variances[iCluster]	-= nMembers [iCluster] * V;
	}

	if( Parameters("UPDATEVIEW")->asBool() )
	{
		DataObject_Update(pCluster, 0, nCluster, 1);
	}

	//-----------------------------------------------------
	noShift		= 0;

	int	maxIter	= Parameters("MAXITER")->asInt();

	for(nPasses=1, bContinue=true; bContinue && (maxIter==0 || nPasses<=maxIter) && Process_Get_Okay(false); nPasses++)
	{
		for(iElement=0; iElement<nElements && bContinue; iElement++)
		{
			if( pCluster->asInt(iElement) >= 0 )
			{
				if( noShift++ >= nElements )
				{
					bContinue	= false;
				}
				else
				{

					//-------------------------------------
					iCluster	= pCluster->asInt(iElement);

					if( nMembers[iCluster] > 1 )
					{
						V	= 0.;

						for(iGrid=0; iGrid<nGrids; iGrid++)
						{
							d	= Centroids[iCluster][iGrid] - Grids[iGrid]->asDouble(iElement);
							V	+= d * d;
						}

						n_iK	= nMembers[iCluster];
						V1		= V * n_iK / (n_iK - 1.);
						VMin	= -1.;

						//---------------------------------
						for(jCluster=0; jCluster<nCluster; jCluster++)
						{
							if( jCluster != iCluster )
							{
								V	= 0.;

								for(iGrid=0; iGrid<nGrids; iGrid++)
								{
									d	= Centroids[jCluster][iGrid] - Grids[iGrid]->asDouble(iElement);
									V	+= d * d;
								}

								n_jK	= nMembers[jCluster];
								V2		= V * n_jK / (n_jK + 1.);

								if( VMin < 0 || V2 < VMin )
								{
									VMin		= V2;
									kCluster	= jCluster;
								}
							}
						}

						//---------------------------------
						if( VMin >= 0 && VMin < V1 )
						{
							noShift				= 0;
							Variances[iCluster]	-= V1;
							Variances[kCluster]	+= VMin;
							V1					= 1. / (n_iK - 1.);
							n_jK				= nMembers[kCluster];
							V2					= 1. / (n_jK + 1.);

							for(iGrid=0; iGrid<nGrids; iGrid++)
							{
								d							= Grids[iGrid]->asDouble(iElement);
								Centroids[iCluster][iGrid]	= (n_iK * Centroids[iCluster][iGrid] - d) * V1;
								Centroids[kCluster][iGrid]	= (n_jK * Centroids[kCluster][iGrid] + d) * V2;
							}

							pCluster->Set_Value(iElement, kCluster);

							nMembers[iCluster]--;
							nMembers[kCluster]++;
						}
					}
				}
			}
		}

		//-------------------------------------------------
		for(iCluster=0, SP=0.; iCluster<nCluster; iCluster++)
		{
			SP	+= Variances[iCluster];
		}

		SP	/= nElements;

		Process_Set_Text("%s: %d >> %s %f", _TL("pass"), nPasses, _TL("change"), SP_Last < 0. ? SP : SP_Last - SP);

		SP_Last		= SP;

		if( Parameters("UPDATEVIEW")->asBool() )
		{
			DataObject_Update(pCluster);
		}
	}

	nElements	= nClusterElements;

	return( SP );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
