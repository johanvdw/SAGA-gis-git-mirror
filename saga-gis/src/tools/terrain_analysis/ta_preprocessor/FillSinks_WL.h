
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                    ta_preprocessor                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    FillSinks_WL.h                     //
//                                                       //
//                 Copyright (C) 2007 by                 //
//                    Volker Wichmann                    //
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
//    e-mail:     reklovw@web.de					     //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__FillSinks_WL_H
#define HEADER_INCLUDED__FillSinks_WL_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_api/saga_api.h>

#include <iostream>
#include <queue>
#include <vector>

using namespace std;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CFillSinks_WL_Node
{
public:
	CFillSinks_WL_Node() : spill( 0 ) {}
	virtual ~CFillSinks_WL_Node() {}

	int		x;
	int		y;
	double	spill;
};

class CompareGreater
{
public:
	bool operator()(CFillSinks_WL_Node n1, CFillSinks_WL_Node n2)
	{
		return n1.spill > n2.spill; 
	}
};


//---------------------------------------------------------
class CFillSinks_WL : public CSG_Tool_Grid
{
public:
	CFillSinks_WL(void);
	virtual ~CFillSinks_WL(void);


protected:

	virtual bool		On_Execute(void);


private:

	typedef				vector< CFillSinks_WL_Node > nodeVector;
	typedef				priority_queue< CFillSinks_WL_Node, nodeVector, CompareGreater > PriorityQ;

	CSG_Grid			*pFilled;

	int					Get_Dir(int x, int y, double z);

};

//---------------------------------------------------------
class CFillSinks_WL_XXL : public CSG_Tool_Grid
{
public:
	CFillSinks_WL_XXL(void);
	virtual ~CFillSinks_WL_XXL(void);


protected:

	virtual bool		On_Execute(void);


private:

	typedef		vector< CFillSinks_WL_Node > nodeVector;
	typedef		priority_queue< CFillSinks_WL_Node, nodeVector, CompareGreater > PriorityQ;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__FillSinks_WL_H
