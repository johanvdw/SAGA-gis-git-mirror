
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                        db_odbc                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   Get_Connection.h                    //
//                                                       //
//                 Copyright (C) 2008 by                 //
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
#ifndef HEADER_INCLUDED__Get_Connection_H
#define HEADER_INCLUDED__Get_Connection_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CGet_Servers : public CSG_Tool
{
public:
	CGet_Servers(void);

	virtual CSG_String			Get_MenuPath			(void)	{	return( _TL("Tools") );	}


protected:

	virtual bool				On_Execute				(void);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CGet_Connection : public CSG_Tool
{
public:
	CGet_Connection(void);


protected:

	virtual bool				On_Before_Execution		(void);

	virtual bool				On_Execute				(void);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CDel_Connection : public CSG_ODBC_Tool
{
public:
	CDel_Connection(void);


protected:

	virtual bool				On_Execute				(void);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CDel_Connections : public CSG_Tool
{
public:
	CDel_Connections(void);

	virtual CSG_String			Get_MenuPath			(void)	{	return( _TL("Tools") );	}


protected:

	virtual bool				On_Before_Execution		(void);

	virtual bool				On_Execute				(void);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CTransaction : public CSG_ODBC_Tool
{
public:
	CTransaction(void);

	virtual CSG_String			Get_MenuPath			(void)	{	return( _TL("Tools") );	}


protected:

	virtual bool				On_Before_Execution		(void);

	virtual bool				On_Execute				(void);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CExecute_SQL : public CSG_ODBC_Tool
{
public:
	CExecute_SQL(void);

	virtual CSG_String			Get_MenuPath			(void)	{	return( _TL("Tools") );	}


protected:

	virtual bool				On_Execute				(void);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Get_Connection_H
