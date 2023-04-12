
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_API                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                       shapes.h                        //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'.                              //
//                                                       //
// This library is free software; you can redistribute   //
// it and/or modify it under the terms of the GNU Lesser //
// General Public License as published by the Free       //
// Software Foundation, either version 2.1 of the        //
// License, or (at your option) any later version.       //
//                                                       //
// This library is distributed in the hope that it will  //
// be useful, but WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU Lesser General Public //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU Lesser     //
// General Public License along with this program; if    //
// not, see <http://www.gnu.org/licenses/>.              //
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
#ifndef HEADER_INCLUDED__SAGA_API__shapes_H
#define HEADER_INCLUDED__SAGA_API__shapes_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
/** \file shapes.h
* The vector data container, a specialized CSG_Table class
* following a geometry object based approach.
* @see CSG_Shapes
* @see CSG_Table
* @see CSG_Data_Object
*/


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "table.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	SG_VERTEX_TYPE_XY		= 0,
	SG_VERTEX_TYPE_XYZ,
	SG_VERTEX_TYPE_XYZM
}
TSG_Vertex_Type;

//---------------------------------------------------------
typedef enum
{
	SHAPE_TYPE_Undefined	= 0,
	SHAPE_TYPE_Point,
	SHAPE_TYPE_Points,
	SHAPE_TYPE_Line,
	SHAPE_TYPE_Polygon
}
TSG_Shape_Type;

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_String	SG_Get_ShapeType_Name	(TSG_Shape_Type Type);

//---------------------------------------------------------
typedef enum
{
	SHAPE_FILE_FORMAT_Undefined	= 0,
	SHAPE_FILE_FORMAT_ESRI,
	SHAPE_FILE_FORMAT_GeoPackage,
	SHAPE_FILE_FORMAT_GeoJSON
}
TSG_Shape_File_Format;

//---------------------------------------------------------
typedef enum
{
	SHAPE_NO_COPY	= 0,
	SHAPE_COPY_GEOM,
	SHAPE_COPY_ATTR,
	SHAPE_COPY
}
TSG_ADD_Shape_Copy_Mode;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape : public CSG_Table_Record
{
	friend class CSG_Shapes;

public:

	//-----------------------------------------------------
	virtual void				Destroy				(void);

	virtual bool				Assign				(CSG_Table_Record *pRecord);
	virtual bool				Assign				(CSG_Table_Record *pRecord, bool bAssign_Attributes);
	virtual bool				Assign				(CSG_Table_Record *pRecord, TSG_ADD_Shape_Copy_Mode mCopy);

	virtual bool				is_Valid			(void)	const = 0;

	TSG_Shape_Type				Get_Type			(void)	const;
	TSG_Vertex_Type				Get_Vertex_Type		(void)	const;

	class CSG_Shape_Point   *	asPoint				(void)	const	{	return( Get_Type() == SHAPE_TYPE_Point   ? (CSG_Shape_Point   *)this : NULL );	}
	class CSG_Shape_Points  *	asPoints			(void)	const	{	return( Get_Type() == SHAPE_TYPE_Points  ? (CSG_Shape_Points  *)this : NULL );	}
	class CSG_Shape_Line    *	asLine				(void)	const	{	return( Get_Type() == SHAPE_TYPE_Line    ? (CSG_Shape_Line    *)this : NULL );	}
	class CSG_Shape_Polygon *	asPolygon			(void)	const	{	return( Get_Type() == SHAPE_TYPE_Polygon ? (CSG_Shape_Polygon *)this : NULL );	}

	//-----------------------------------------------------
	virtual int					Get_Point_Count		(void)      const = 0;
	virtual int					Get_Point_Count		(int iPart) const = 0;
	virtual int					Get_Part_Count		(void)      const = 0;

	virtual int					Add_Part			(class CSG_Shape_Part *pPart) {	return( -1   );	}
	virtual CSG_Shape_Part *	Get_Part			(int iPart) const             {	return( NULL );	}
	virtual int					Del_Part			(int iPart)                   {	return( -1   );	}
	virtual int					Del_Parts			(void)                        {	return( -1   );	}

	virtual int					Add_Point			(double x, double y,                    int iPart = 0) = 0;
	virtual int					Add_Point			(const CSG_Point    &p,                 int iPart = 0) = 0;
	virtual int					Add_Point			(const CSG_Point_3D &p,                 int iPart = 0) = 0;
	virtual int					Add_Point			(const CSG_Point_4D &p,                 int iPart = 0) = 0;

	virtual int					Ins_Point			(double x, double y,    int iPoint    , int iPart = 0) = 0;
	virtual int					Ins_Point			(const CSG_Point    &p, int iPoint    , int iPart = 0) = 0;
	virtual int					Ins_Point			(const CSG_Point_3D &p, int iPoint    , int iPart = 0) = 0;
	virtual int					Ins_Point			(const CSG_Point_4D &p, int iPoint    , int iPart = 0) = 0;

	virtual int					Set_Point			(double x, double y,    int iPoint = 0, int iPart = 0) = 0;
	virtual int					Set_Point			(const CSG_Point    &p, int iPoint = 0, int iPart = 0) = 0;
	virtual int					Set_Point			(const CSG_Point_3D &p, int iPoint = 0, int iPart = 0) = 0;
	virtual int					Set_Point			(const CSG_Point_4D &p, int iPoint = 0, int iPart = 0) = 0;

	virtual int					Del_Point			(                       int iPoint    , int iPart = 0) = 0;

	virtual TSG_Point			Get_Point			(int iPoint = 0)                                const = 0;
	virtual TSG_Point			Get_Point			(int iPoint, int iPart, bool bAscending = true) const = 0;

	//-----------------------------------------------------
	virtual void				Set_Z				(double z, int iPoint = 0, int iPart = 0)                     {               }
	virtual double				Get_Z				(int iPoint = 0, int iPart = 0, bool bAscending = true) const { return( 0. ); }
	virtual double				Get_ZMin			(void)                                                        { return( 0. ); }
	virtual double				Get_ZMax			(void)                                                        { return( 0. ); }
	virtual TSG_Point_3D		Get_Point_Z			(int iPoint = 0, int iPart = 0, bool bAscending = true) const
	{
		TSG_Point_3D Point; TSG_Point p = Get_Point(iPoint, iPart, bAscending);

		Point.x = p.x;
		Point.y = p.y;
		Point.z = Get_Z(iPoint, iPart, bAscending);

		return( Point );
	}

	//-----------------------------------------------------
	virtual void				Set_M				(double m, int iPoint = 0, int iPart = 0)                     {               }
	virtual double				Get_M				(int iPoint = 0, int iPart = 0, bool bAscending = true) const { return( 0. ); }
	virtual double				Get_MMin			(void)                                                        { return( 0. ); }
	virtual double				Get_MMax			(void)                                                        { return( 0. ); }
	virtual TSG_Point_4D		Get_Point_ZM		(int iPoint = 0, int iPart = 0, bool bAscending = true) const
	{
		TSG_Point_4D Point; TSG_Point p = Get_Point(iPoint, iPart, bAscending);

		Point.x = p.x;
		Point.y = p.y;
		Point.z = Get_Z(iPoint, iPart, bAscending);
		Point.m = Get_M(iPoint, iPart, bAscending);

		return( Point );
	}

	//-----------------------------------------------------
	virtual const CSG_Rect &	Get_Extent			(void) = 0;
	virtual const CSG_Rect &	Get_Extent			(int iPart)	{	return( Get_Extent() );	}

	virtual TSG_Point			Get_Centroid		(void) = 0;

	TSG_Intersection			Intersects			(CSG_Shape *pShape);
	TSG_Intersection			Intersects			(const TSG_Rect &Extent);

	virtual double				Get_Distance		(TSG_Point Point                            ) const = 0;
	virtual double				Get_Distance		(TSG_Point Point                 , int iPart) const = 0;
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next           ) const = 0;
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart) const = 0;

	virtual bool				Revert_Points		(int iPart)	{	return( true );	}


protected:

	CSG_Shape(class CSG_Shapes *pOwner, sLong Index);
	virtual ~CSG_Shape(void);

	virtual bool				On_Assign			(CSG_Shape *pShape)								= 0;
	virtual TSG_Intersection	On_Intersects		(CSG_Shape *pShape)								= 0;
	virtual TSG_Intersection	On_Intersects		(TSG_Rect Extent)								= 0;

	virtual void				_Invalidate			(void);

};


///////////////////////////////////////////////////////////
//														 //
//						Point							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Point : public CSG_Shape
{
	friend class CSG_Shapes;

public:

	virtual bool				is_Valid			(void)		const	{	return( true );	}

	virtual int					Get_Point_Count		(void)		const	{	return(  1 );	}
	virtual int					Get_Point_Count		(int iPart)	const	{	return(  1 );	}
	virtual int					Get_Part_Count		(void)		const	{	return(  1 );	}

	virtual int					Add_Point			(double x, double y,                    int iPart = 0) { return( Set_Point(x, y, 0) ); }
	virtual int					Add_Point			(const CSG_Point    &p,                 int iPart = 0) { return( Set_Point(p   , 0) ); }
	virtual int					Add_Point			(const CSG_Point_3D &p,                 int iPart = 0) { return( Set_Point(p   , 0) ); }
	virtual int					Add_Point			(const CSG_Point_4D &p,                 int iPart = 0) { return( Set_Point(p   , 0) ); }

	virtual int					Ins_Point			(double x, double y,    int iPoint    , int iPart = 0) { return( Set_Point(x, y, 0) ); }
	virtual int					Ins_Point			(const CSG_Point    &p, int iPoint    , int iPart = 0) { return( Set_Point(p   , 0) ); }
	virtual int					Ins_Point			(const CSG_Point_3D &p, int iPoint    , int iPart = 0) { return( Set_Point(p   , 0) ); }
	virtual int					Ins_Point			(const CSG_Point_4D &p, int iPoint    , int iPart = 0) { return( Set_Point(p   , 0) ); }

	virtual int					Set_Point			(double x, double y,    int iPoint = 0, int iPart = 0) { m_Point.x =   x; m_Point.y =   y;                               _Invalidate(); return( 1 ); }
	virtual int					Set_Point			(const CSG_Point    &p, int iPoint = 0, int iPart = 0) { m_Point.x = p.x; m_Point.y = p.y;                               _Invalidate(); return( 1 ); }
	virtual int					Set_Point			(const CSG_Point_3D &p, int iPoint = 0, int iPart = 0) { m_Point.x = p.x; m_Point.y = p.y; Set_Z(p.z, 0);                _Invalidate(); return( 1 ); }
	virtual int					Set_Point			(const CSG_Point_4D &p, int iPoint = 0, int iPart = 0) { m_Point.x = p.x; m_Point.y = p.y; Set_Z(p.z, 0); Set_M(p.m, 0); _Invalidate(); return( 1 ); }

	virtual int					Del_Point			(                       int iPoint    , int iPart = 0) { return( -1 ); }

	virtual TSG_Point			Get_Point			(int iPoint = 0)                                const  { return( m_Point ); }
	virtual TSG_Point			Get_Point			(int iPoint, int iPart, bool bAscending = true) const  { return( m_Point ); }

	virtual TSG_Point			Get_Centroid		(void)                                                 { return( m_Point ); }

	virtual const CSG_Rect &	Get_Extent			(void);

	virtual double				Get_Distance		(TSG_Point Point                            )	const  {                 return( SG_Get_Distance(Point, m_Point) ); }
	virtual double				Get_Distance		(TSG_Point Point                 , int iPart)	const  {                 return( SG_Get_Distance(Point, m_Point) ); }
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next           )	const  { Next = m_Point; return( SG_Get_Distance(Point, m_Point) ); }
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart)	const  { Next = m_Point; return( SG_Get_Distance(Point, m_Point) ); }


protected:

	CSG_Shape_Point(class CSG_Shapes *pOwner, sLong Index);
	virtual ~CSG_Shape_Point(void);


	TSG_Point					m_Point;


	virtual bool				On_Assign			(CSG_Shape *pShape);
	virtual TSG_Intersection	On_Intersects		(CSG_Shape *pShape);
	virtual TSG_Intersection	On_Intersects		(TSG_Rect Region);

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Point_Z : public CSG_Shape_Point
{
	friend class CSG_Shapes;

public:

	CSG_Shape_Point_Z(class CSG_Shapes *pOwner, sLong Index) : CSG_Shape_Point(pOwner, Index)                     { m_Z = 0.; }

	virtual void				Set_Z				(double z, int iPoint = 0, int iPart = 0)                     { m_Z = z; _Invalidate(); }
	virtual double				Get_Z				(int iPoint = 0, int iPart = 0, bool bAscending = true) const { return( m_Z ); }
	virtual double				Get_ZMin			(void)                                                        { return( m_Z ); }
	virtual double				Get_ZMax			(void)                                                        { return( m_Z ); }


private:

	double						m_Z;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Point_ZM : public CSG_Shape_Point_Z
{
	friend class CSG_Shapes;

public:

	CSG_Shape_Point_ZM(class CSG_Shapes *pOwner, sLong Index) : CSG_Shape_Point_Z(pOwner, Index)                  { m_M = 0.; }

	virtual void				Set_M				(double m, int iPoint = 0, int iPart = 0)                     { m_M = m; _Invalidate(); }
	virtual double				Get_M				(int iPoint = 0, int iPart = 0, bool bAscending = true) const {	return( m_M ); }
	virtual double				Get_MMin			(void)                                                        { return( m_M ); }
	virtual double				Get_MMax			(void)                                                        { return( m_M ); }


private:

	double						m_M;

};


///////////////////////////////////////////////////////////
//														 //
//						Points							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Part
{
	friend class CSG_Shape_Points;
	friend class CSG_Shape_Line;
	friend class CSG_Shape_Polygon;

public:

	class CSG_Shape_Points *	Get_Owner			(void)	{	return( m_pOwner );	}

	virtual bool				Destroy				(void);
	virtual bool				Assign				(CSG_Shape_Part *pPart);

	const CSG_Rect &			Get_Extent			(void)	{	_Update_Extent();	return( m_Extent );	}

	int							Get_Count			(void)	const {	return( m_nPoints );	}

	TSG_Point					Get_Point			(int iPoint, bool bAscending = true)	const
	{
		if( iPoint >= 0 && iPoint < m_nPoints )
		{
			return( m_Points[bAscending ? iPoint : m_nPoints - 1 - iPoint] );
		}

		return( (TSG_Point)CSG_Point(0., 0.) );
	}


	int							Add_Point			(double x, double y   ) { return( Ins_Point(x, y, m_nPoints) ); }
	int							Add_Point			(const CSG_Point    &p) { return( Ins_Point(p   , m_nPoints) ); }
	int							Add_Point			(const CSG_Point_3D &p) { return( Ins_Point(p   , m_nPoints) ); }
	int							Add_Point			(const CSG_Point_4D &p) { return( Ins_Point(p   , m_nPoints) ); }

	int							Ins_Point			(double x, double y   , int iPoint);
	int							Ins_Point			(const CSG_Point    &p, int iPoint) { return( Ins_Point(p.x, p.y, iPoint) ); }
	int							Ins_Point			(const CSG_Point_3D &p, int iPoint);
	int							Ins_Point			(const CSG_Point_4D &p, int iPoint);

	int							Set_Point			(double x, double y   , int iPoint);
	int							Set_Point			(const CSG_Point    &p, int iPoint) { return( Set_Point(p.x, p.y, iPoint) ); }
	int							Set_Point			(const CSG_Point_3D &p, int iPoint);
	int							Set_Point			(const CSG_Point_4D &p, int iPoint);

	int							Del_Point			(                    int iPoint);

	void						Set_Z				(double z, int iPoint)                     { if    ( m_Z && iPoint >= 0 && iPoint < m_nPoints ) { m_Z[iPoint] = z; _Invalidate(); } }
	double						Get_Z				(int iPoint, bool bAscending = true) const { return( m_Z && iPoint >= 0 && iPoint < m_nPoints ?   m_Z[bAscending ? iPoint : m_nPoints - 1 - iPoint] : 0. ); }
	double						Get_ZMin			(void)                                     { _Update_Extent(); return( m_ZMin ); }
	double						Get_ZMax			(void)                                     { _Update_Extent(); return( m_ZMax ); }
	TSG_Point_3D				Get_Point_Z			(int iPoint, bool bAscending = true) const
	{
		TSG_Point_3D Point; TSG_Point p = Get_Point(iPoint, bAscending);

		Point.x = p.x;
		Point.y = p.y;
		Point.z = Get_Z(iPoint, bAscending);

		return( Point );
	}

	void						Set_M				(double m, int iPoint)                     { if    ( m_M && iPoint >= 0 && iPoint < m_nPoints ) { m_M[iPoint] = m; _Invalidate(); } }
	double						Get_M				(int iPoint, bool bAscending = true) const { return( m_M && iPoint >= 0 && iPoint < m_nPoints ?	 m_M[bAscending ? iPoint : m_nPoints - 1 - iPoint] : 0. ); }
	double						Get_MMin			(void)                                     { _Update_Extent(); return( m_MMin ); }
	double						Get_MMax			(void)                                     { _Update_Extent(); return( m_MMax ); }
	TSG_Point_4D				Get_Point_ZM		(int iPoint, bool bAscending = true) const
	{
		TSG_Point_4D Point; TSG_Point p = Get_Point(iPoint, bAscending);

		Point.x = p.x;
		Point.y = p.y;
		Point.z = Get_Z(iPoint, bAscending);
		Point.m = Get_M(iPoint, bAscending);

		return( Point );
	}

	bool						Revert_Points		(void);


protected:

	CSG_Shape_Part(class CSG_Shape_Points *pOwner);
	virtual ~CSG_Shape_Part(void);


	bool						m_bUpdate;

	int							m_nPoints, m_nBuffer;

	double						*m_Z, m_ZMin, m_ZMax, *m_M, m_MMin, m_MMax;

	TSG_Point					*m_Points;

	CSG_Rect					m_Extent;

	class CSG_Shape_Points		*m_pOwner;


	virtual bool				_Alloc_Memory		(int nPoints);
	virtual void				_Invalidate			(void);
	virtual void				_Update_Extent		(void);

};


///////////////////////////////////////////////////////////
//														 //
//						Points							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Points : public CSG_Shape
{
	friend class CSG_Shapes;
	friend class CSG_Shape_Part;

public:

	virtual void				Destroy				(void);

	virtual bool				is_Valid			(void)		const	{	return( m_nParts > 0 && m_pParts[0]->Get_Count() > 0 );	}

	virtual int					Get_Point_Count		(void)		const	{	return( m_nPoints );	}
	virtual int					Get_Point_Count		(int iPart)	const	{	return( iPart >= 0 && iPart < m_nParts ? m_pParts[iPart]->Get_Count() : 0 );	}
	virtual int					Get_Part_Count		(void)		const	{	return( m_nParts );		}

	virtual int					Add_Part			(class CSG_Shape_Part *pPart);
	virtual CSG_Shape_Part *	Get_Part			(int iPart)	const	{	return( iPart >= 0 && iPart < m_nParts ? m_pParts[iPart] : NULL );	}
	virtual int					Del_Part			(int iPart);
	virtual int					Del_Parts			(void);

	virtual int					Add_Point			(double x, double y,                    int iPart = 0) { return( Ins_Point(x, y, Get_Point_Count(iPart), iPart) ); }
	virtual int					Add_Point			(const CSG_Point    &p,                 int iPart = 0) { return( Ins_Point(p   , Get_Point_Count(iPart), iPart) ); }
	virtual int					Add_Point			(const CSG_Point_3D &p,                 int iPart = 0) { return( Ins_Point(p   , Get_Point_Count(iPart), iPart) ); }
	virtual int					Add_Point			(const CSG_Point_4D &p,                 int iPart = 0) { return( Ins_Point(p   , Get_Point_Count(iPart), iPart) ); }

	virtual int					Ins_Point			(double x, double y,    int iPoint    , int iPart = 0);
	virtual int					Ins_Point			(const CSG_Point    &p, int iPoint    , int iPart = 0) { return( Ins_Point(p.x, p.y, iPoint, iPart) ); }
	virtual int					Ins_Point			(const CSG_Point_3D &p, int iPoint    , int iPart = 0);
	virtual int					Ins_Point			(const CSG_Point_4D &p, int iPoint    , int iPart = 0);

	virtual int					Set_Point			(double x, double y,    int iPoint = 0, int iPart = 0);
	virtual int					Set_Point			(const CSG_Point    &p, int iPoint = 0, int iPart = 0) { return( Set_Point(p.x, p.y, iPoint, iPart) ); }
	virtual int					Set_Point			(const CSG_Point_3D &p, int iPoint = 0, int iPart = 0);
	virtual int					Set_Point			(const CSG_Point_4D &p, int iPoint = 0, int iPart = 0);

	virtual int					Del_Point			(                       int iPoint    , int iPart = 0);

	virtual TSG_Point			Get_Point			(int iPoint = 0)                                const;
	virtual TSG_Point			Get_Point			(int iPoint, int iPart, bool bAscending = true) const
	{
		if( iPart >= 0 && iPart < m_nParts )
		{
			return( m_pParts[iPart]->Get_Point(iPoint, bAscending) );
		}

		return( (TSG_Point)CSG_Point(0., 0.) );
	}

	virtual void				Set_Z				(double z, int iPoint = 0, int iPart = 0)                               { if    ( iPart >= 0 && iPart < m_nParts ) m_pParts[iPart]->Set_Z(z, iPoint); }
	virtual double				Get_Z				(          int iPoint = 0, int iPart = 0, bool bAscending = true) const { return( iPart >= 0 && iPart < m_nParts ? m_pParts[iPart]->Get_Z(   iPoint, bAscending) : 0. ); }
	virtual double				Get_ZMin			(void)                                                                  { _Update_Extent();	return( m_ZMin ); }
	virtual double				Get_ZMax			(void)                                                                  { _Update_Extent();	return( m_ZMax ); }

	virtual void				Set_M				(double m, int iPoint = 0, int iPart = 0)                               { if    ( iPart >= 0 && iPart < m_nParts ) m_pParts[iPart]->Set_M(m, iPoint); }
	virtual double				Get_M				(          int iPoint = 0, int iPart = 0, bool bAscending = true) const { return( iPart >= 0 && iPart < m_nParts ? m_pParts[iPart]->Get_M(   iPoint, bAscending) : 0. ); }
	virtual double				Get_MMin			(void)                                                                  { _Update_Extent();	return( m_MMin ); }
	virtual double				Get_MMax			(void)                                                                  { _Update_Extent();	return( m_MMax ); }

	virtual const CSG_Rect &	Get_Extent			(void)	{ _Update_Extent(); return( m_Extent ); }

	virtual TSG_Point			Get_Centroid		(void);

	virtual double				Get_Distance		(TSG_Point Point                            ) const;
	virtual double				Get_Distance		(TSG_Point Point                 , int iPart) const;
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next           ) const;
	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart) const;

	virtual bool				Revert_Points		(int iPart)	{ return( iPart >= 0 && iPart < m_nParts ? m_pParts[iPart]->Revert_Points() : false ); }


protected:

	CSG_Shape_Points(class CSG_Shapes *pOwner, sLong Index);
	virtual ~CSG_Shape_Points(void);


	bool						m_bUpdate;

	int							m_nParts, m_nPoints;

	double						m_ZMin, m_ZMax, m_MMin, m_MMax;

	CSG_Rect					m_Extent;

	CSG_Shape_Part				**m_pParts;


	int							_Add_Part			(void);

	virtual CSG_Shape_Part *	_Get_Part			(void)	{	return( new CSG_Shape_Part(this) );	}

	virtual void				_Invalidate			(void)
	{
		if( !m_bUpdate )
		{
			m_bUpdate	= true;

			CSG_Shape::_Invalidate();
		}
	}

	void						_Update_Extent		(void);

	virtual bool				On_Assign			(CSG_Shape *pShape);
	virtual TSG_Intersection	On_Intersects		(CSG_Shape *pShape);
	virtual TSG_Intersection	On_Intersects		(TSG_Rect Region);

};


///////////////////////////////////////////////////////////
//														 //
//						Line							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Line : public CSG_Shape_Points
{
	friend class CSG_Shapes;

public:

	virtual bool				is_Valid			(void)	const	{	return( m_nParts > 0 && m_pParts[0]->Get_Count() > 1 );	}

	virtual TSG_Point			Get_Centroid		(void);

	double						Get_Length			(void);
	double						Get_Length			(int iPart);

	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart)	const;


protected:

	CSG_Shape_Line(class CSG_Shapes *pOwner, sLong Index);
	virtual ~CSG_Shape_Line(void);

	virtual TSG_Intersection	On_Intersects		(CSG_Shape *pShape);
	virtual TSG_Intersection	On_Intersects		(TSG_Rect Region);

};


///////////////////////////////////////////////////////////
//														 //
//						Polygon							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	SG_POLYGON_POINT_Outside,
	SG_POLYGON_POINT_Vertex,
	SG_POLYGON_POINT_Edge,
	SG_POLYGON_POINT_Interior
}
TSG_Polygon_Point_Relation;

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Polygon_Part : public CSG_Shape_Part
{
	friend class CSG_Shape_Polygon;

public:

	bool						is_Clockwise		(void)	{	_Update_Area();	return( m_bClockwise == 1 );	}

	double						Get_Perimeter		(void)	{	_Update_Area();	return( m_Perimeter );	}

	double						Get_Area			(void)	{	_Update_Area();	return( m_Area );	}

	const TSG_Point &			Get_Centroid		(void)	{	_Update_Area();	return( m_Centroid );	}

	TSG_Polygon_Point_Relation	Get_Point_Relation	(const CSG_Point &p);
	TSG_Polygon_Point_Relation	Get_Point_Relation	(double x, double y);

	bool						is_OnEdge			(const CSG_Point &p);
	bool						is_OnEdge			(double x, double y);

	bool						Contains			(const CSG_Point &p);
	bool						Contains			(double x, double y);

	bool						is_Neighbour		(CSG_Shape_Polygon_Part *pPart, bool bSimpleCheck = true);

	double						Get_Distance		(TSG_Point Point, TSG_Point &Next);


protected:

	CSG_Shape_Polygon_Part(class CSG_Shape_Points *pOwner);
	virtual ~CSG_Shape_Polygon_Part(void);


	int							m_bClockwise, m_bLake;

	double						m_Area, m_Perimeter;

	TSG_Point					m_Centroid;


	virtual void				_Invalidate			(void);

	void						_Update_Area		(void);

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shape_Polygon : public CSG_Shape_Points
{
	friend class CSG_Shapes;

public:

	virtual bool				is_Valid			(void)	const	{	return( m_nParts > 0 && m_pParts[0]->Get_Count() > 2 );	}


	CSG_Shape_Polygon_Part *	Get_Polygon_Part	(int iPart)	const	{	return( (CSG_Shape_Polygon_Part *)Get_Part(iPart) );	}

	bool						is_Lake				(int iPart);

	bool						is_Clockwise		(int iPart);

	double						Get_Perimeter		(int iPart);
	double						Get_Perimeter		(void);

	double						Get_Area			(int iPart);
	double						Get_Area			(void);

	TSG_Point					Get_Centroid		(int iPart);
	TSG_Point					Get_Centroid		(void);

	TSG_Polygon_Point_Relation	Get_Point_Relation	(const CSG_Point &p, int iPart);
	TSG_Polygon_Point_Relation	Get_Point_Relation	(double x, double y, int iPart);
	TSG_Polygon_Point_Relation	Get_Point_Relation	(const CSG_Point &p);
	TSG_Polygon_Point_Relation	Get_Point_Relation	(double x, double y);


	bool						is_OnEdge			(const CSG_Point &p, int iPart);
	bool						is_OnEdge			(double x, double y, int iPart);
	bool						is_OnEdge			(const CSG_Point &p);
	bool						is_OnEdge			(double x, double y);

	bool						Contains			(const CSG_Point &p, int iPart);
	bool						Contains			(double x, double y, int iPart);
	bool						Contains			(const CSG_Point &p);
	bool						Contains			(double x, double y);

	bool						is_Neighbour		(CSG_Shape_Polygon *pPolygon, bool bSimpleCheck = true);

	virtual double				Get_Distance		(TSG_Point Point, TSG_Point &Next, int iPart)	const;


protected:

	CSG_Shape_Polygon(class CSG_Shapes *pOwner, sLong Index);
	virtual ~CSG_Shape_Polygon(void);


	int							m_bUpdate_Lakes;


	virtual CSG_Shape_Part *	_Get_Part			(void)	{	return( new CSG_Shape_Polygon_Part(this) );	}

	virtual void				_Invalidate			(void);

	virtual TSG_Intersection	On_Intersects		(CSG_Shape *pShape);
	virtual TSG_Intersection	On_Intersects		(TSG_Rect Region);

};


///////////////////////////////////////////////////////////
//														 //
//						Shapes							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shapes : public CSG_Table
{
	friend class CSG_Shape;

public:

	CSG_Shapes(void);

									CSG_Shapes	(const CSG_Shapes &Shapes);
	bool							Create		(const CSG_Shapes &Shapes);

									CSG_Shapes	(const char       *File);
	bool							Create		(const char       *File);
									CSG_Shapes	(const wchar_t    *File);
	bool							Create		(const wchar_t    *File);
									CSG_Shapes	(const CSG_String &File);
	bool							Create		(const CSG_String &File);

									CSG_Shapes	(TSG_Shape_Type Type, const SG_Char *Name = NULL, CSG_Table *pTemplate = NULL, TSG_Vertex_Type Vertex_Type = SG_VERTEX_TYPE_XY);
	bool							Create		(TSG_Shape_Type Type, const SG_Char *Name = NULL, CSG_Table *pTemplate = NULL, TSG_Vertex_Type Vertex_Type = SG_VERTEX_TYPE_XY);

	virtual ~CSG_Shapes(void);

	virtual bool					Destroy					(void);

	virtual TSG_Data_Object_Type	Get_ObjectType			(void)	const			{	return( SG_DATAOBJECT_TYPE_Shapes );	}

	virtual bool					Assign					(CSG_Data_Object *pObject);

	virtual bool					Save					(const CSG_String &File, int Format = 0);
	virtual bool					Save					(const char       *File, int Format = 0)	{	return( Save(CSG_String(File), Format) );	}
	virtual bool					Save					(const wchar_t    *File, int Format = 0)	{	return( Save(CSG_String(File), Format) );	}

	virtual bool					is_Valid				(void)	const			{	return( m_Type != SHAPE_TYPE_Undefined && Get_Count() >= 0 );		}

	virtual TSG_Shape_Type			Get_Type				(void)	const			{	return( m_Type );		}

	TSG_Vertex_Type					Get_Vertex_Type			(void)	const			{	return( m_Vertex_Type );	}

	virtual const CSG_Rect &		Get_Extent				(void)					{	Update();	return( m_Extent );	}

	double							Get_ZMin				(void)					{	Update();	return( m_ZMin );	}
	double							Get_ZMax				(void)					{	Update();	return( m_ZMax );	}
	double							Get_MMin				(void)					{	Update();	return( m_MMin );	}
	double							Get_MMax				(void)					{	Update();	return( m_MMax );	}

	//-----------------------------------------------------
	virtual CSG_Shape *				Add_Shape				(CSG_Table_Record *pCopy = NULL, TSG_ADD_Shape_Copy_Mode mCopy = SHAPE_COPY);
	virtual bool					Del_Shape				(sLong Index);
	virtual bool					Del_Shape				(CSG_Shape *pShape);
	virtual bool					Del_Shapes				(void)					{	return( Del_Records() );	}

	virtual CSG_Shape *				Get_Shape				(const CSG_Point &Point, double Epsilon = 0.);
	virtual CSG_Shape *				Get_Shape				(sLong Index)	const	{	return( (CSG_Shape *)Get_Record        (Index) );	}
	virtual CSG_Shape *				Get_Shape_byIndex		(sLong Index)	const	{	return( (CSG_Shape *)Get_Record_byIndex(Index) );	}

	//-----------------------------------------------------
	bool							Make_Clean				(void);

	//-----------------------------------------------------
	virtual const CSG_Rect &		Get_Selection_Extent	(void);
	virtual CSG_Shape *				Get_Selection			(sLong Index = 0)		{	return( (CSG_Shape *)CSG_Table::Get_Selection(Index) );	};

	virtual bool					Select					(sLong Index                    , bool bInvert = false);
	virtual bool					Select					(CSG_Table_Record *pShape = NULL, bool bInvert = false);
	virtual bool					Select					(const TSG_Rect &Extent         , bool bInvert = false);
	virtual bool					Select					(const TSG_Point &Point         , bool bInvert = false);


protected:

	double							m_ZMin, m_ZMax, m_MMin, m_MMax;

	TSG_Shape_Type					m_Type;

	TSG_Vertex_Type					m_Vertex_Type;

	CSG_Rect						m_Extent_Selected;


	virtual bool					On_Update				(void);
	virtual bool					On_Reload				(void);
	virtual bool					On_Delete				(void);

	virtual void					_On_Construction		(void);

	virtual CSG_Table_Record *		_Get_New_Record			(sLong Index);


private:

	bool							_Load_GDAL				(const CSG_String &File);
	bool							_Save_GDAL				(const CSG_String &File, const CSG_String &Driver);

	bool							_Load_ESRI				(const CSG_String &File);
	bool							_Save_ESRI				(const CSG_String &File);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(void);

/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(const CSG_Shapes &Shapes);

/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(const char       *File);
/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(const wchar_t    *File);
/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(const CSG_String &File);

/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(TSG_Shape_Type Type, const SG_Char *Name = NULL, CSG_Table *pTemplate = NULL, TSG_Vertex_Type Vertex_Type = SG_VERTEX_TYPE_XY);

/** Safe shapes construction */
SAGA_API_DLL_EXPORT CSG_Shapes *	SG_Create_Shapes	(CSG_Shapes *pTemplate);


//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool					SG_Shapes_Set_File_Format_Default		(int Format);
SAGA_API_DLL_EXPORT TSG_Shape_File_Format	SG_Shapes_Get_File_Format_Default		(void);
SAGA_API_DLL_EXPORT CSG_String				SG_Shapes_Get_File_Extension_Default	(void);


///////////////////////////////////////////////////////////
//														 //
//				Point Region QuadTree					 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_PRQuadTree_Item
{
public:

	virtual bool			is_Leaf			(void)	const	{	return( false );	}
	virtual bool			is_Node			(void)	const	{	return( false );	}
	virtual bool			has_Statistics	(void)	const	{	return( false );	}

	const CSG_Rect &		Get_Extent		(void)	const	{	return( m_Extent );	}
	double					Get_xMin		(void)	const	{	return( m_Extent.xMin   );	}
	double					Get_xCenter		(void)	const	{	return( m_Extent.Get_XCenter() );	}
	double					Get_xMax		(void)	const	{	return( m_Extent.xMax   );	}
	double					Get_yMin		(void)	const	{	return( m_Extent.yMin   );	}
	double					Get_yCenter		(void)	const	{	return( m_Extent.Get_YCenter() );	}
	double					Get_yMax		(void)	const	{	return( m_Extent.yMax   );	}
	double					Get_Size		(void)	const	{	return( m_Extent.Get_XRange()  );	}

	bool					Contains		(const CSG_Point &p)	const	{	return(	m_Extent.Contains(p   ) );	}
	bool					Contains		(double x, double y)	const	{	return(	m_Extent.Contains(x, y) );	}

	class CSG_PRQuadTree_Leaf *	asLeaf		(void)	const	{	return( (class CSG_PRQuadTree_Leaf *)this );	}
	class CSG_PRQuadTree_Node *	asNode		(void)	const	{	return( (class CSG_PRQuadTree_Node *)this );	}


protected:

	CSG_PRQuadTree_Item(const CSG_Rect &Extent, int Quadrant = -1)	{	Set_Extent(Extent, Quadrant);	}

	virtual ~CSG_PRQuadTree_Item(void)	{}

	void					Set_Extent		(const CSG_Rect &Extent, int Quadrant = -1)
	{
		switch( Quadrant )
		{
		case  0: m_Extent.Assign(Extent.Get_XMin   (), Extent.Get_YMin   (), Extent.Get_XCenter(), Extent.Get_YCenter()); break; // bottom left
		case  1: m_Extent.Assign(Extent.Get_XMin   (), Extent.Get_YCenter(), Extent.Get_XCenter(), Extent.Get_YMax   ()); break; // top left
		case  2: m_Extent.Assign(Extent.Get_XCenter(), Extent.Get_YCenter(), Extent.Get_XMax   (), Extent.Get_YMax   ()); break; // top right
		case  3: m_Extent.Assign(Extent.Get_XCenter(), Extent.Get_YMin   (), Extent.Get_XMax   (), Extent.Get_YCenter()); break; // bottom right
		default: m_Extent.Assign(Extent); break;
		}
	}

	int						Get_Quadrant	(const TSG_Point &p)	const	{	return( Get_Quadrant(p.x, p.y) );	}

	int						Get_Quadrant	(double x, double y)	const
	{
		return( y < Get_yCenter() ? (x < Get_xCenter() ? 0 : 3) : (x < Get_xCenter() ? 1 : 2) );
	}


	CSG_Rect				m_Extent;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_PRQuadTree_Leaf : public CSG_PRQuadTree_Item
{
	friend class CSG_PRQuadTree_Node;

public:

	virtual bool			is_Leaf			(void)	const	{	return( true );		}

	const TSG_Point &		Get_Point		(void)	const	{	return( m_Point );		}
	double					Get_X			(void)	const	{	return( m_Point.x );	}
	double					Get_Y			(void)	const	{	return( m_Point.y );	}
	double					Get_Z			(void)	const	{	return( m_z );			}


protected:

	CSG_PRQuadTree_Leaf(const CSG_Rect &Extent, int Quadrant, double x, double y, double z)
		: CSG_PRQuadTree_Item(Extent, Quadrant)
	{
		m_Point.x	= x;
		m_Point.y	= y;
		m_z			= z;
	}

	virtual ~CSG_PRQuadTree_Leaf(void)	{}


	double					m_z;

	TSG_Point				m_Point;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_PRQuadTree_Leaf_List : public CSG_PRQuadTree_Leaf
{
	friend class CSG_PRQuadTree_Node;

public:

	virtual bool			has_Statistics	(void)	const	{	return( true );	}

	sLong					Get_Count		(void)		{	return( s_z.Get_Count   () );	}
	double					Get_Value		(sLong i)	{	return( s_z.Get_Value  (i) );	}
	double					Get_Minimum		(void)		{	return( s_z.Get_Minimum () );	}
	double					Get_Maximum		(void)		{	return( s_z.Get_Maximum () );	}
	double					Get_Range		(void)		{	return( s_z.Get_Range   () );	}
	double					Get_Sum			(void)		{	return( s_z.Get_Sum     () );	}
	double					Get_Mean		(void)		{	return( s_z.Get_Mean    () );	}
	double					Get_Variance	(void)		{	return( s_z.Get_Variance() );	}
	double					Get_StdDev		(void)		{	return( s_z.Get_StdDev  () );	}


protected:

	CSG_PRQuadTree_Leaf_List(const CSG_Rect &Extent, int Quadrant, double x, double y, double z)
		: CSG_PRQuadTree_Leaf(Extent, Quadrant, x, y, z)
	{
		s_z.Create(true);

		s_z.Add_Value(z);
	}

	virtual ~CSG_PRQuadTree_Leaf_List(void)	{}

	void					Add_Value		(double z)
	{
		s_z.Add_Value(z);

		m_z	= s_z.Get_Mean();
	}

	CSG_Simple_Statistics	s_z;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_PRQuadTree_Node : public CSG_PRQuadTree_Item
{
	friend class CSG_PRQuadTree;

public:

	virtual bool			is_Node			(void)	const	{	return( true );		}

	CSG_PRQuadTree_Item *	Get_Child		(int Quadrant)	const	{	return( Quadrant >= 0 && Quadrant < 4 ? m_pChildren[Quadrant] : NULL );	}
	CSG_PRQuadTree_Item *	Get_Child		(double x, double y);

	bool					Add_Point		(double x, double y, double z);

	virtual CSG_Simple_Statistics *	Get_X	(void)			{	return( NULL );	}
	virtual CSG_Simple_Statistics *	Get_Y	(void)			{	return( NULL );	}
	virtual CSG_Simple_Statistics *	Get_Z	(void)			{	return( NULL );	}


protected:

	CSG_PRQuadTree_Node(const CSG_Rect &Extent, int Quadrant = -1);
	CSG_PRQuadTree_Node(CSG_PRQuadTree_Leaf *pLeaf);
	virtual ~CSG_PRQuadTree_Node(void);


	CSG_PRQuadTree_Item		*m_pChildren[4];

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_PRQuadTree_Node_Statistics : public CSG_PRQuadTree_Node
{
	friend class CSG_PRQuadTree;
	friend class CSG_PRQuadTree_Node;

public:

	virtual bool			has_Statistics	(void)	const	{	return( true );	}

	virtual CSG_Simple_Statistics *	Get_X	(void)			{	return( &m_x );	}
	virtual CSG_Simple_Statistics *	Get_Y	(void)			{	return( &m_y );	}
	virtual CSG_Simple_Statistics *	Get_Z	(void)			{	return( &m_z );	}


protected:

	CSG_PRQuadTree_Node_Statistics(const CSG_Rect &Extent, int Quadrant = -1)
		: CSG_PRQuadTree_Node(Extent, Quadrant)
	{}

	CSG_PRQuadTree_Node_Statistics(CSG_PRQuadTree_Leaf *pLeaf)
		: CSG_PRQuadTree_Node(pLeaf)
	{}

	virtual ~CSG_PRQuadTree_Node_Statistics(void)	{}


	CSG_Simple_Statistics	m_x, m_y, m_z;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_PRQuadTree
{
public:
	CSG_PRQuadTree(void);
	virtual ~CSG_PRQuadTree(void);

								CSG_PRQuadTree			(const TSG_Rect &Extent, bool bStatistics = false);
	bool						Create					(const CSG_Rect &Extent, bool bStatistics = false);

								CSG_PRQuadTree			(CSG_Shapes *pShapes, int Attribute, bool bStatistics = false);
	bool						Create					(CSG_Shapes *pShapes, int Attribute, bool bStatistics = false);

	void						Destroy					(void);

	bool						Add_Point				(double x, double y, double z);
	bool						Add_Point				(const TSG_Point &p, double z);

	size_t						Get_Point_Count			(void)	const	{	return( m_nPoints );		}

	const CSG_PRQuadTree_Node &	Get_Root				(void)	const	{	return( *m_pRoot );			}
	CSG_PRQuadTree_Node *		Get_Root_Pointer		(void)	const	{	return(  m_pRoot );			}

	bool						is_Okay					(void)	const	{	return( m_pRoot != NULL );	}

	bool						is_Polar				(void)	const	{	return( m_bPolar );			}
	void						Set_Polar_Search		(bool bOn)		{	m_bPolar	= bOn;			}

	CSG_PRQuadTree_Leaf *		Get_Nearest_Leaf		(const TSG_Point &p, double &Distance)	const;
	CSG_PRQuadTree_Leaf *		Get_Nearest_Leaf		(double x, double y, double &Distance)	const;
	bool						Get_Nearest_Point		(const TSG_Point &p, TSG_Point &Point, double &Value, double &Distance)	const;
	bool						Get_Nearest_Point		(double x, double y, TSG_Point &Point, double &Value, double &Distance)	const;

	size_t						Get_Nearest_Points		(CSG_Points_3D &Points, const TSG_Point &p, size_t maxPoints, double Radius = 0., int iQuadrant = -1)	const;
	size_t						Get_Nearest_Points		(CSG_Points_3D &Points, double x, double y, size_t maxPoints, double Radius = 0., int iQuadrant = -1)	const;

	size_t						Select_Nearest_Points	(const TSG_Point &p, size_t maxPoints, double Radius = 0., int iQuadrant = -1);
	size_t						Select_Nearest_Points	(double x, double y, size_t maxPoints, double Radius = 0., int iQuadrant = -1);

	size_t						Get_Selected_Count		(void)     const	{	return( m_Selection.Get_Size() );	}
	CSG_PRQuadTree_Leaf *		Get_Selected_Leaf		(size_t i) const	{	return( i >= (size_t)m_Selection.Get_Size() ? NULL : (((TLeaf *)m_Selection.Get_Array()) + i)->pLeaf          );	}
	double						Get_Selected_Z			(size_t i) const	{	return( i >= (size_t)m_Selection.Get_Size() ?  0.  : (((TLeaf *)m_Selection.Get_Array()) + i)->pLeaf->Get_Z() );	}
	double						Get_Selected_Distance	(size_t i) const	{	return( i >= (size_t)m_Selection.Get_Size() ? -1.  : (((TLeaf *)m_Selection.Get_Array()) + i)->Distance       );	}
	bool						Get_Selected_Point		(size_t i, double &x, double &y, double &z) const
	{
		CSG_PRQuadTree_Leaf	*pLeaf	= Get_Selected_Leaf(i);

		if( pLeaf )
		{
			x	= pLeaf->Get_X();
			y	= pLeaf->Get_Y();
			z	= pLeaf->Get_Z();

			return( true );
		}

		return( false );
	}


private:

	typedef struct SLeaf
	{
		CSG_PRQuadTree_Leaf		*pLeaf;

		double					Distance;
	}
	TLeaf;


private:

	bool						m_bPolar;

	size_t						m_nPoints;

	CSG_Array					m_Selection;

	CSG_PRQuadTree_Node			*m_pRoot;

	bool						_Check_Root				(double x, double y);

	bool						_Quadrant_Contains		(double x, double y, int iQuadrant, const TSG_Point &p)						const;
	bool						_Radius_Contains		(double x, double y, double r, const TSG_Point &p)							const;
	bool						_Radius_Contains		(double x, double y, double r, int iQuadrant, const TSG_Point &p)			const;
	bool						_Quadrant_Intersects	(double x, double y, int iQuadrant, CSG_PRQuadTree_Item *pItem)				const;
	bool						_Radius_Intersects		(double x, double y, double r, CSG_PRQuadTree_Item *pItem)					const;
	bool						_Radius_Intersects		(double x, double y, double r, int iQuadrant, CSG_PRQuadTree_Item *pItem)	const;

	CSG_PRQuadTree_Leaf	*		_Get_Nearest_Point		(CSG_PRQuadTree_Item *pItem, double x, double y, double &Distance)			const;

	TLeaf *						_Get_Selected			(const CSG_Array &Selection, size_t i)										const;
	bool						_Add_Selected			(      CSG_Array &Selection          , CSG_PRQuadTree_Leaf *pLeaf, double Distance)	const;
	bool						_Set_Selected			(      CSG_Array &Selection, size_t i, CSG_PRQuadTree_Leaf *pLeaf, double Distance)	const;
	void						_Select_Nearest_Points	(      CSG_Array &Selection, CSG_PRQuadTree_Item *pItem, double x, double y, double &Distance, double Radius, size_t maxPoints, int iQuadrant)	const;
	size_t						_Select_Nearest_Points	(      CSG_Array &Selection, double x, double y, size_t maxPoints, double Radius, int iQuadrant)	const;

};


///////////////////////////////////////////////////////////
//                                                       //
//                 Point Search Engine                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_KDTree
{
public:
	CSG_KDTree(void);
	virtual ~CSG_KDTree(void);

	virtual bool				Destroy				(void);

	bool						is_Okay				(void)		const	{	return( m_pKDTree != NULL );	}

	size_t						Get_Match_Count		(void    )	const	{	return( m_Indices.Get_Size() );	}
	size_t						Get_Match_Index		(size_t i)	const	{	return( m_Indices  [i]       );	}
	double						Get_Match_Distance	(size_t i)	const	{	return( m_Distances[i]       );	}
	CSG_Shape *					Get_Match_Shape		(size_t i)	const;

	int							Get_Point_Count		(void)		const	{	return( m_Points.Get_NY()    );	}
	double *					Get_Point			(int    i)	const	{	return( m_Points[i]          );	}
	double *					Get_Point			(size_t i)	const	{	return( m_Points[i]          );	}
	double *					operator []			(int    i)	const	{	return( m_Points[i]          );	}
	double *					operator []			(size_t i)	const	{	return( m_Points[i]          );	}
	virtual double				Get_Point_Value		(int    i)	const	{	return( Get_Point_Value((size_t)i) );	}
	virtual double				Get_Point_Value		(size_t i)	const = 0;


protected:

	class CSG_KDTree_Adaptor	*m_pAdaptor;

	void						*m_pKDTree;

	CSG_Array_Int				m_Indices;

	CSG_Vector					m_Distances;

	CSG_Matrix					m_Points;


	void						_On_Construction	(void);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_KDTree_2D : public CSG_KDTree
{
public:
	CSG_KDTree_2D(void);
	virtual ~CSG_KDTree_2D(void);

								CSG_KDTree_2D		(CSG_Shapes *pPoints, int Field = -1);
	bool						Create				(CSG_Shapes *pPoints, int Field = -1);

								CSG_KDTree_2D		(CSG_PointCloud *pPoints);
	bool						Create				(CSG_PointCloud *pPoints);

								CSG_KDTree_2D		(const CSG_Matrix &Points);
	bool						Create				(const CSG_Matrix &Points);

								CSG_KDTree_2D		(const double **Points, size_t nPoints);
	bool						Create				(const double **Points, size_t nPoints);

	virtual bool				Destroy				(void);

	virtual double				Get_Point_Value		(size_t i)	const	{	return( m_Points[i][2] );	}

	virtual size_t				Get_Nearest_Points	(double Coordinate[2], size_t Count, double Radius);
	virtual size_t				Get_Nearest_Points	(double Coordinate[2], size_t Count, double Radius, CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual size_t				Get_Nearest_Points	(double Coordinate[2], size_t Count, size_t *Indices, double *Distances);
	virtual bool				Get_Nearest_Point	(double Coordinate[2], size_t &Index, double &Distance);
	virtual bool				Get_Nearest_Point	(double Coordinate[2], size_t &Index);
	virtual bool				Get_Nearest_Value	(double Coordinate[2], double &Value);
	virtual CSG_Shape *			Get_Nearest_Shape	(double Coordinate[2]);
	virtual size_t				Get_Duplicates		(double Coordinate[2], CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual size_t				Get_Duplicates		(double Coordinate[2]);

	virtual size_t				Get_Nearest_Points	(double x, double y, size_t Count, double Radius);
	virtual size_t				Get_Nearest_Points	(double x, double y, size_t Count, double Radius, CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual size_t				Get_Nearest_Points	(double x, double y, size_t Count, size_t *Indices, double *Distances);
	virtual bool				Get_Nearest_Point	(double x, double y, size_t &Index, double &Distance);
	virtual bool				Get_Nearest_Point	(double x, double y, size_t &Index);
	virtual bool				Get_Nearest_Value	(double x, double y, double &Value);
	virtual CSG_Shape *			Get_Nearest_Shape	(double x, double y);
	virtual size_t				Get_Duplicates		(double x, double y, CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual size_t				Get_Duplicates		(double x, double y);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_KDTree_3D : public CSG_KDTree
{
public:
	CSG_KDTree_3D(void);
	virtual ~CSG_KDTree_3D(void);

								CSG_KDTree_3D		(CSG_Shapes *pPoints, int Field = -1, int zField = -1, double zScale = 1.);
	bool						Create				(CSG_Shapes *pPoints, int Field = -1, int zField = -1, double zScale = 1.);

								CSG_KDTree_3D		(CSG_PointCloud *pPoints);
	bool						Create				(CSG_PointCloud *pPoints);

								CSG_KDTree_3D		(const CSG_Matrix &Points);
	bool						Create				(const CSG_Matrix &Points);

								CSG_KDTree_3D		(const double **Points, size_t nPoints);
	bool						Create				(const double **Points, size_t nPoints);

	virtual bool				Destroy				(void);

	virtual double				Get_Point_Value		(size_t i)	const	{	return( m_Points[i][3] );	}

	virtual size_t				Get_Nearest_Points	(double Coordinate[3], size_t Count, double Radius);

	virtual size_t				Get_Nearest_Points	(double Coordinate[3], size_t Count, size_t *Indices, double *Distances);
	virtual size_t				Get_Nearest_Points	(double Coordinate[3], size_t Count, double Radius, CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual bool				Get_Nearest_Point	(double Coordinate[3], size_t &Index, double &Distance);
	virtual bool				Get_Nearest_Point	(double Coordinate[3], size_t &Index);
	virtual bool				Get_Nearest_Value	(double Coordinate[3], double &Value);
	virtual CSG_Shape *			Get_Nearest_Shape	(double Coordinate[3]);
	virtual size_t				Get_Duplicates		(double Coordinate[3], CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual size_t				Get_Duplicates		(double Coordinate[3]);

	virtual size_t				Get_Nearest_Points	(double x, double y, double z, size_t Count, double Radius);
	virtual size_t				Get_Nearest_Points	(double x, double y, double z, size_t Count, double Radius, CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual size_t				Get_Nearest_Points	(double x, double y, double z, size_t Count, size_t *Indices, double *Distances);
	virtual bool				Get_Nearest_Point	(double x, double y, double z, size_t &Index, double &Distance);
	virtual bool				Get_Nearest_Point	(double x, double y, double z, size_t &Index);
	virtual bool				Get_Nearest_Value	(double x, double y, double z, double &Value);
	virtual CSG_Shape *			Get_Nearest_Shape	(double x, double y, double z);
	virtual size_t				Get_Duplicates		(double x, double y, double z, CSG_Array_Int &Indices, CSG_Vector &Distances);
	virtual size_t				Get_Duplicates		(double x, double y, double z);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Parameters_Point_Search
{
public:
	CSG_Parameters_Point_Search(void);
	virtual ~CSG_Parameters_Point_Search(void)	{}

	virtual bool				Create					(class CSG_Parameters *pParameters, const CSG_String &Parent = "", size_t minPoints = 0);

	virtual bool				On_Parameter_Changed	(class CSG_Parameters *pParameters, class CSG_Parameter *pParameter);
	virtual bool				On_Parameters_Enable	(class CSG_Parameters *pParameters, class CSG_Parameter *pParameter);

	bool						Do_Use_All				(void)	const	{ 	return( m_maxPoints == 0 && m_Radius <= 0. ); }
	bool						Do_Use_All				(bool bUpdate);

	size_t						Get_Min_Points			(void)	const	{	return( m_minPoints );	}
	size_t						Get_Max_Points			(void)	const	{	return( m_maxPoints );	}
	double						Get_Radius				(void)	const	{	return( m_Radius    );	}


protected:

	bool						Update					(void);


	size_t						m_minPoints, m_maxPoints;

	double						m_Radius;

	class CSG_Parameters		*m_pParameters;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Parameters_Point_Search_KDTree_2D : public CSG_Parameters_Point_Search
{
public:
	CSG_Parameters_Point_Search_KDTree_2D(void);
	virtual ~CSG_Parameters_Point_Search_KDTree_2D(void)	{}

	bool						Initialize				(CSG_Shapes *pPoints, int zField = -1);
	bool						Finalize				(void);

	sLong						Set_Location			(double x, double y);
	sLong						Set_Location			(const CSG_Point &p);

	sLong						Get_Count				(void)	const	{	return( m_pPoints ? m_pPoints->Get_Count() : m_Search.Get_Match_Count() ); }
	bool						Get_Point				(sLong Index, double &x, double &y, double &z);

	bool						Get_Points				(double x, double y, CSG_Points_3D &Points);
	bool						Get_Points				(const CSG_Point &p, CSG_Points_3D &Points);

	bool						Get_Points				(double x, double y, CSG_Array_Int &Indices);
	bool						Get_Points				(const CSG_Point &p, CSG_Array_Int &Indices);

	bool						Get_Points				(double x, double y, CSG_Array_Int &Indices, CSG_Vector &Distances);
	bool						Get_Points				(const CSG_Point &p, CSG_Array_Int &Indices, CSG_Vector &Distances);


private:

	int							m_zField;

	CSG_Shapes					*m_pPoints;

	CSG_KDTree_2D				m_Search;

};


///////////////////////////////////////////////////////////
//														 //
//					Polygon Tools						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool			SG_Shape_Get_Intersection	(CSG_Shape *pSubject, CSG_Shape_Polygon *pClip, CSG_Shape *pSolution = NULL);
SAGA_API_DLL_EXPORT bool			SG_Shape_Get_Difference		(CSG_Shape *pSubject, CSG_Shape_Polygon *pClip, CSG_Shape *pSolution = NULL);
SAGA_API_DLL_EXPORT bool			SG_Shape_Get_ExclusiveOr	(CSG_Shape *pSubject, CSG_Shape_Polygon *pClip, CSG_Shape *pSolution = NULL);
SAGA_API_DLL_EXPORT bool			SG_Shape_Get_Union			(CSG_Shape *pSubject, CSG_Shape_Polygon *pClip, CSG_Shape *pSolution = NULL);
SAGA_API_DLL_EXPORT bool			SG_Shape_Get_Dissolve		(CSG_Shape *pSubject                          , CSG_Shape *pSolution = NULL);
SAGA_API_DLL_EXPORT bool			SG_Shape_Get_Offset			(CSG_Shape *pSubject, double Size, double dArc, CSG_Shape *pSolution = NULL);

SAGA_API_DLL_EXPORT const char *	SG_Clipper_Get_Version		(void);


///////////////////////////////////////////////////////////
//														 //
//						OpenGIS							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum ESG_OGIS_ByteOrder
{
	SG_OGIS_BYTEORDER_XDR				= 0,	// Big Endian
	SG_OGIS_BYTEORDER_NDR				= 1		// Little Endian
};

//---------------------------------------------------------
typedef enum
{
	SG_OGIS_TYPE_Undefined				=    0,

	SG_OGIS_TYPE_Point					=    1,
	SG_OGIS_TYPE_LineString				=    2,
	SG_OGIS_TYPE_Polygon				=    3,
	SG_OGIS_TYPE_MultiPoint				=    4,
	SG_OGIS_TYPE_MultiLineString		=    5,
	SG_OGIS_TYPE_MultiPolygon			=    6,
	SG_OGIS_TYPE_GeometryCollection		=    7,
	SG_OGIS_TYPE_PolyhedralSurface		=   15,
	SG_OGIS_TYPE_TIN					=   16,
	SG_OGIS_TYPE_Triangle				=   17,

	SG_OGIS_TYPE_PointZ					= 1001,
	SG_OGIS_TYPE_LineStringZ			= 1002,
	SG_OGIS_TYPE_PolygonZ				= 1003,
	SG_OGIS_TYPE_MultiPointZ			= 1004,
	SG_OGIS_TYPE_MultiLineStringZ		= 1005,
	SG_OGIS_TYPE_MultiPolygonZ			= 1006,
	SG_OGIS_TYPE_GeometryCollectionZ	= 1007,
	SG_OGIS_TYPE_PolyhedralSurfaceZ		= 1015,
	SG_OGIS_TYPE_TINZ					= 1016,
	SG_OGIS_TYPE_TriangleZ				= 1017,

	SG_OGIS_TYPE_PointM					= 2001,
	SG_OGIS_TYPE_LineStringM			= 2002,
	SG_OGIS_TYPE_PolygonM				= 2003,
	SG_OGIS_TYPE_MultiPointM			= 2004,
	SG_OGIS_TYPE_MultiLineStringM		= 2005,
	SG_OGIS_TYPE_MultiPolygonM			= 2006,
	SG_OGIS_TYPE_GeometryCollectionM	= 2007,
	SG_OGIS_TYPE_PolyhedralSurfaceM		= 2015,
	SG_OGIS_TYPE_TINM					= 2016,
	SG_OGIS_TYPE_TriangleM				= 2017,

	SG_OGIS_TYPE_PointZM				= 3001,
	SG_OGIS_TYPE_LineStringZM			= 3002,
	SG_OGIS_TYPE_PolygonZM				= 3003,
	SG_OGIS_TYPE_MultiPointZM			= 3004,
	SG_OGIS_TYPE_MultiLineStringZM		= 3005,
	SG_OGIS_TYPE_MultiPolygonZM			= 3006,
	SG_OGIS_TYPE_GeometryCollectionZM	= 3007,
	SG_OGIS_TYPE_PolyhedralSurfaceZM	= 3015,
	SG_OGIS_TYPE_TINZM					= 3016,
	SG_OGIS_TYPE_TriangleZM				= 3017
}
TSG_OGIS_Type_Geometry;


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Shapes_OGIS_Converter
{
public:
	CSG_Shapes_OGIS_Converter(void)	{}

	static bool				from_WKText				(const CSG_String &Text, CSG_Shape *pShape);
	static bool				to_WKText				(CSG_Shape *pShape, CSG_String &Text);

	static bool				from_WKBinary			(CSG_Bytes &Bytes, CSG_Shape *pShape);
	static bool				to_WKBinary				(CSG_Shape *pShape, CSG_Bytes &Bytes);

	static bool				from_ShapeType			(      CSG_String &Type, TSG_Shape_Type  Shape, TSG_Vertex_Type  Vertex = SG_VERTEX_TYPE_XY);
	static CSG_String		from_ShapeType			(                        TSG_Shape_Type  Shape, TSG_Vertex_Type  Vertex = SG_VERTEX_TYPE_XY);
	static bool				to_ShapeType			(const CSG_String &Type, TSG_Shape_Type &Shape, TSG_Vertex_Type &Vertex);
	static TSG_Shape_Type	to_ShapeType			(const CSG_String &Type);
	static TSG_Vertex_Type	to_VertexType			(const CSG_String &Type);

	static bool				from_ShapeType			(DWORD            &Type, TSG_Shape_Type  Shape, TSG_Vertex_Type  Vertex = SG_VERTEX_TYPE_XY);
	static bool				to_ShapeType			(DWORD             Type, TSG_Shape_Type &Shape, TSG_Vertex_Type &Vertex);
	static TSG_Shape_Type	to_ShapeType			(DWORD             Type);
	static TSG_Vertex_Type	to_VertexType			(DWORD             Type);

	static CSG_String		Type_asWKText			(DWORD             Type);
	static DWORD			Type_asWKBinary			(const CSG_String &Type);


private:

	static bool				_WKT_Read_Point			(const CSG_String &Text, CSG_Shape *pShape, int iPart);
	static bool				_WKT_Read_Points		(const CSG_String &Text, CSG_Shape *pShape);
	static bool				_WKT_Read_Parts			(const CSG_String &Text, CSG_Shape *pShape);
	static bool				_WKT_Read_Polygon		(const CSG_String &Text, CSG_Shape *pShape);

	static bool				_WKT_Write_Point		(CSG_String &Text, CSG_Shape *pShape, int iPoint, int iPart);
	static bool				_WKT_Write_Points		(CSG_String &Text, CSG_Shape *pShape, int iPart);
	static bool				_WKT_Write_Parts		(CSG_String &Text, CSG_Shape *pShape);
	static bool				_WKT_Write_Polygon		(CSG_String &Text, CSG_Shape *pShape);

	static bool				_WKB_Read_Point			(CSG_Bytes &Bytes, bool bSwapBytes, TSG_Vertex_Type Vertex, CSG_Shape *pShape, int iPart);
	static bool				_WKB_Read_Points		(CSG_Bytes &Bytes, bool bSwapBytes, TSG_Vertex_Type Vertex, CSG_Shape *pShape);
	static bool				_WKB_Read_Polygon		(CSG_Bytes &Bytes, bool bSwapBytes, TSG_Vertex_Type Vertex, CSG_Shape *pShape);
	static bool				_WKB_Read_MultiPoint	(CSG_Bytes &Bytes, bool bSwapBytes, TSG_Vertex_Type Vertex, CSG_Shape *pShape);
	static bool				_WKB_Read_MultiLine		(CSG_Bytes &Bytes, bool bSwapBytes, TSG_Vertex_Type Vertex, CSG_Shape *pShape);
	static bool				_WKB_Read_MultiPolygon	(CSG_Bytes &Bytes, bool bSwapBytes, TSG_Vertex_Type Vertex, CSG_Shape *pShape);

	static bool				_WKB_Write_Point		(CSG_Bytes &Bytes, CSG_Shape *pShape, int iPoint, int iPart);
	static bool				_WKB_Write_Points		(CSG_Bytes &Bytes, CSG_Shape *pShape, int iPart);
	static bool				_WKB_Write_MultiPoint	(CSG_Bytes &Bytes, CSG_Shape *pShape);
	static bool				_WKB_Write_MultiLine	(CSG_Bytes &Bytes, CSG_Shape *pShape);
	static bool				_WKB_Write_MultiPolygon	(CSG_Bytes &Bytes, CSG_Shape *pShape);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Grid_OGIS_Converter
{
public:
	CSG_Grid_OGIS_Converter(void)	{}

	static bool				from_WKBinary			(CSG_Bytes &Bytes, class CSG_Grid *pGrid);
	static bool				to_WKBinary				(CSG_Bytes &Bytes, class CSG_Grid *pGrid, int SRID = -1);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__SAGA_API__shapes_H
