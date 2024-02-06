
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
//                     mat_tools.h                       //
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
#ifndef HEADER_INCLUDED__SAGA_API__mat_tools_H
#define HEADER_INCLUDED__SAGA_API__mat_tools_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
/** \file mat_tools.h
* A set of basic mathematical, numerical, statistical tools.
* @see CSG_Vector
* @see CSG_Matrix
* @see CSG_Formula
* @see CSG_Random
* @see CSG_Index
* @see CSG_Histogram
* @see CSG_Simple_Statistics
* @see CSG_Unique_Value_Statistics
* @see CSG_Regression
* @see CSG_Trend
* @see CSG_Spline
*/


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "geo_tools.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef M_PI
#define M_PI						3.141592653589793
#endif

#define M_PI_045					(M_PI / 4.)
#define M_PI_090					(M_PI / 2.)
#define M_PI_135					(M_PI * 3. / 4.)
#define M_PI_180					(M_PI)
#define M_PI_225					(M_PI * 5. / 4.)
#define M_PI_270					(M_PI * 3. / 2.)
#define M_PI_315					(M_PI * 7. / 4.)
#define M_PI_360					(M_PI * 2.)

#define M_RAD_TO_DEG				(180. / M_PI)
#define M_DEG_TO_RAD				(M_PI / 180.)

//---------------------------------------------------------
#define M_EULER						2.718281828459045

//---------------------------------------------------------
#define N_MEGABYTE_BYTES			0x100000

//---------------------------------------------------------
#define M_ALMOST_ZERO				(1.e-03)
#define M_TINY						(1.e-20)

//---------------------------------------------------------
#define M_SQR(x)					((x) * (x))
#define M_SQRT(x)					sqrt((double)(x))
#define M_GET_LENGTH(x, y)			sqrt((double)((x)*(x) + (y)*(y)))

#define M_GET_MIN(a, b)				(((a) < (b)) ? (a) : (b))
#define M_GET_MAX(a, b)				(((a) > (b)) ? (a) : (b))
#define M_SET_MINMAX(min, max, x)	if( min > x ) { min = x; } else if( max < x ) { max = x; }

#define M_SET_SIGN(x, sign)			((sign) < 0 ? (x < 0 ? x : -x) : (x > 0 ? x : -x))

//---------------------------------------------------------
#define SG_ROUND_TO_BYTE(x)			((BYTE )(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_CHAR(x)			((char )(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_WORD(x)			((WORD )(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_SHORT(x)		((short)(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_DWORD(x)		((DWORD)(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_INT(x)			((int  )(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_LONG(x)			((long )(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_ULONG(x)		((uLong)(x < 0. ? x - 0.5 : x + 0.5))
#define SG_ROUND_TO_SLONG(x)		((sLong)(x < 0. ? x - 0.5 : x + 0.5))


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
SAGA_API_DLL_EXPORT double		SG_Get_Square			(double Value);

SAGA_API_DLL_EXPORT double		SG_Get_Rounded							(double Value, int Decimals = 0);
SAGA_API_DLL_EXPORT double		SG_Get_Rounded_To_SignificantFigures	(double Value, int Decimals);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT int			SG_Get_Digit_Count		(int Number);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT	CSG_String	SG_Get_Double_asString	(double Number, int Width = -1, int Precision = -1, bool bScientific = false);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT	int			SG_Compare_Int			(const void *a, const void *b);
SAGA_API_DLL_EXPORT	int			SG_Compare_Double		(const void *a, const void *b);
SAGA_API_DLL_EXPORT	int			SG_Compare_Char_Ptr		(const void *a, const void *b);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT	double		SG_Degree_To_Decimal	(              double  Deg, double  Min, double  Sec);
SAGA_API_DLL_EXPORT	void		SG_Decimal_To_Degree	(double Value, double &Deg, double &Min, double &Sec);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef int (* TSG_PFNC_Compare) (const sLong a, const sLong b);

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Index
{
public:
	class CSG_Index_Compare
	{
	public:
		CSG_Index_Compare(void) {}
		virtual ~CSG_Index_Compare(void) {}

		virtual int				Compare				(const sLong a, const sLong b) = 0;
	};


public:
	CSG_Index(void);
	virtual ~CSG_Index(void);

								CSG_Index			(sLong nValues, CSG_Index_Compare &Compare);
	bool						Create				(sLong nValues, CSG_Index_Compare &Compare);

								CSG_Index			(sLong nValues, CSG_Index_Compare *pCompare);
	bool						Create				(sLong nValues, CSG_Index_Compare *pCompare);

								CSG_Index			(sLong nValues, int    *Values, bool bAscending = true);
	bool						Create				(sLong nValues, int    *Values, bool bAscending = true);

								CSG_Index			(sLong nValues, double *Values, bool bAscending = true);
	bool						Create				(sLong nValues, double *Values, bool bAscending = true);

								CSG_Index			(sLong nValues, TSG_PFNC_Compare fCompare);
	bool						Create				(sLong nValues, TSG_PFNC_Compare fCompare);

	bool						Destroy				(void);

	void						Show_Progress		(bool bProgress = true);

	bool						Add_Entry			(sLong Position = -1);
	bool						Del_Entry			(sLong Position = -1);

	bool						is_Okay				(void)	const	{	return( m_nValues > 0 );	}
	sLong						Get_Count			(void)	const	{	return( m_nValues     );	}

	sLong						Get_Index			(sLong Position, bool Ascending = true)	const
	{
		return( Position < 0 || Position >= m_nValues ? -1 : m_Index[Ascending ? Position : m_nValues - 1 - Position] );
	}

	sLong						operator []			(sLong Position)	const
	{
		return( Position < 0 || Position >= m_nValues ? -1 : m_Index[Position] );
	}


private:

	bool						m_bProgress;

	sLong						m_nValues, *m_Index;


	void						_On_Construction	(void);

	bool						_Set_Array			(sLong nValues);
	bool						_Set_Index			(CSG_Index_Compare *pCompare);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_PriorityQueue
{
public:

	//-----------------------------------------------------
	class CSG_PriorityQueueItem
	{
	public:
		CSG_PriorityQueueItem(void)	{}

		virtual int			Compare		(CSG_PriorityQueueItem *pItem)	= 0;

	};

	//-----------------------------------------------------
	CSG_PriorityQueue(size_t maxSize = 256);

	virtual ~CSG_PriorityQueue(void);

	void						Create			(size_t maxSize = 256);
	void						Destroy			(void);

	//-----------------------------------------------------
	bool						is_Empty		(void)		const	{	return( m_nItems == 0 );	}
	size_t						Get_Size		(void)		const	{	return( m_nItems      );	}
	CSG_PriorityQueueItem *		Get_Item		(size_t i)	const	{	return( m_Items[i]    );	}

	void						Add				(CSG_PriorityQueueItem *pItem);

	CSG_PriorityQueueItem *		Peek			(void)		const	{	return( Minimum() );	}
	CSG_PriorityQueueItem *		Poll			(void);

	//-----------------------------------------------------
	CSG_PriorityQueueItem *		Minimum			(void)		const
	{
		if( m_nItems )
		{
			if( m_pLeaf[0] )
			{
				return( m_pLeaf[0]->Minimum() );
			}

			return( m_Items[0] );
		}

		return( NULL );
	}

	CSG_PriorityQueueItem *		Maximum			(void)		const
	{
		if( m_nItems )
		{
			if( m_pLeaf[1] )
			{
				return( m_pLeaf[1]->Maximum() );
			}

			return( m_Items[m_nItems - 1] );
		}

		return( NULL );
	}


private:

	size_t						m_nItems, m_maxSize;

	CSG_PriorityQueue			*m_pLeaf[2];

	CSG_PriorityQueueItem		**m_Items;


	size_t						_Insert_Position	(CSG_PriorityQueueItem *pItem);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Vector
{
public:
	CSG_Vector(void);
	virtual ~CSG_Vector(void);

								CSG_Vector			(const CSG_Vector &Vector);
	bool						Create				(const CSG_Vector &Vector);

								CSG_Vector			(sLong n, const double *Data = NULL);
	bool						Create				(sLong n, const double *Data = NULL);

	bool						Destroy				(void);

	bool						Set_Rows			(sLong  nRows);
	bool						Add_Rows			(sLong  nRows);
	bool						Del_Rows			(sLong  nRows);
	bool						Add_Row				(double Value = 0.);
	bool						Del_Row				(sLong  Row = -1);

	sLong						Get_Row_Count		(void)		const	{	return( m_Array.Get_Size () );	}
	sLong						Get_Size			(void)		const	{	return( m_Array.Get_Size () );	}
	size_t						Get_uSize			(void)		const	{	return( m_Array.Get_uSize() );	}
	int							Get_N				(void)		const	{	return( (int)m_Array.Get_Size() );	}

	double *					Get_Data			(void)		const	{	return( (double *)m_Array.Get_Array() );	}
	double						Get_Data			(sLong  x)	const	{	return( Get_Data()[x] );	}
	double						operator ()			(sLong  x)	const	{	return( Get_Data()[x] );	}
	double &					operator []			(sLong  x)			{	return( Get_Data()[x] );	}
	double &					operator []			(size_t x)			{	return( Get_Data()[x] );	}
	double &					operator []			(int    x)			{	return( Get_Data()[x] );	}
	const double &				operator []			(sLong  x)	const	{	return( Get_Data()[x] );	}
	const double &				operator []			(size_t x)	const	{	return( Get_Data()[x] );	}
	const double &				operator []			(int    x)	const	{	return( Get_Data()[x] );	}
	operator const double *							(void)		const	{	return( Get_Data() );		}

	CSG_String					to_String			(int Width = -1, int Precision = -1, bool bScientific = false, const SG_Char *Separator = NULL)	const;
	bool						from_String			(const CSG_String &String);

	bool						is_Null				(void)						const;
	bool						is_Equal			(const CSG_Vector &Vector)	const;
	bool						is_Collinear		(const CSG_Vector &Vector)	const;

	bool						Assign				(double Scalar);
	bool						Assign				(const CSG_Vector &Vector);
	bool						Add					(double Scalar);
	bool						Add					(const CSG_Vector &Vector);
	bool						Subtract			(const CSG_Vector &Vector);
	bool						Multiply			(double Scalar);
	bool						Multiply			(const CSG_Vector &Vector);
	bool						Multiply_Cross		(const CSG_Vector &Vector);
	double						Multiply_Scalar		(const CSG_Vector &Vector)	const;
	bool						Multiply			(const class CSG_Matrix &Matrix);

	bool						operator ==			(const CSG_Vector &Vector)	const { return(  is_Equal(Vector) ); }
	bool						operator !=			(const CSG_Vector &Vector)	const { return( !is_Equal(Vector) ); }
	CSG_Vector &				operator =			(double Scalar);
	CSG_Vector &				operator =			(const CSG_Vector &Vector);
	CSG_Vector &				operator +=			(double Scalar);
	CSG_Vector &				operator +=			(const CSG_Vector &Vector);
	CSG_Vector &				operator -=			(double Scalar);
	CSG_Vector &				operator -=			(const CSG_Vector &Vector);
	CSG_Vector &				operator *=			(double Scalar);
	CSG_Vector &				operator *=			(const CSG_Vector &Vector);
	CSG_Vector &				operator *=			(const class CSG_Matrix &Matrix);
	CSG_Vector					operator +			(double Scalar)				const;
	CSG_Vector					operator +			(const CSG_Vector &Vector)	const;
	CSG_Vector					operator -			(double Scalar)				const;
	CSG_Vector					operator -			(const CSG_Vector &Vector)	const;
	CSG_Vector					operator *			(double Scalar)				const;
	double						operator *			(const CSG_Vector &Vector)	const;

	bool						Set_Zero			(void);
	bool						Set_Unity			(void);

	bool						Flip_Values			(void);

	bool						Sort				(bool bAscending = true);

	double						Get_Length			(void)						const;
	CSG_Vector					Get_Scalar_Product	(const CSG_Vector &Vector)	const;
	CSG_Vector					Get_Cross_Product	(const CSG_Vector &Vector)	const;
	double						Get_Angle			(const CSG_Vector &Vector)	const;
	CSG_Vector					Get_Unity			(void)						const;

	typedef double const *		const_iterator;

	const_iterator				begin				(void)	const	{	return( Get_Data()              );	}
	const_iterator				end					(void)	const	{	return( Get_Data() + Get_Size() );	}
	const_iterator				cbegin				(void)	const	{	return( Get_Data()              );	}
	const_iterator				cend				(void)	const	{	return( Get_Data() + Get_Size() );	}


private:

	CSG_Array					m_Array;

};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_Vector	operator *			(double Scalar, const CSG_Vector &Vector);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool	SG_VectorR2_Rotate		(double &x, double &y, double Angle);
SAGA_API_DLL_EXPORT bool	SG_VectorR2_Rotate		(double     Vector[2], double Angle);
SAGA_API_DLL_EXPORT bool	SG_VectorR2_Rotate		(CSG_Vector &Vector  , double Angle);

SAGA_API_DLL_EXPORT bool	SG_VectorR3_Rotate		(double     Vector[3], size_t Axis, double Angle);
SAGA_API_DLL_EXPORT bool	SG_VectorR3_Rotate		(CSG_Vector &Vector  , size_t Axis, double Angle);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Matrix
{
public:
	CSG_Matrix(void);
	virtual ~CSG_Matrix(void);

								CSG_Matrix			(const CSG_Matrix &Matrix);
	bool						Create				(const CSG_Matrix &Matrix);

								CSG_Matrix			(sLong nCols, sLong nRows, const double *Data = NULL);
	bool						Create				(sLong nCols, sLong nRows, const double *Data = NULL);

								CSG_Matrix			(sLong nCols, sLong nRows, const double **Data);
	bool						Create				(sLong nCols, sLong nRows, const double **Data);

	bool						Destroy				(void);

	bool						Set_Size			(sLong nRows, sLong nCols);
	bool						Set_Cols			(sLong nCols);
	bool						Set_Rows			(sLong nRows);
	bool						Add_Cols			(sLong nCols);
	bool						Add_Rows			(sLong nRows);
	bool						Del_Cols			(sLong nCols);
	bool						Del_Rows			(sLong nRows);
	bool						Add_Col				(           const double     *Data = NULL);
	bool						Add_Col				(           const CSG_Vector &Data);
	bool						Add_Row				(           const double     *Data = NULL);
	bool						Add_Row				(           const CSG_Vector &Data);
	bool						Ins_Col				(sLong Col, const double     *Data = NULL);
	bool						Ins_Col				(sLong Col, const CSG_Vector &Data);
	bool						Ins_Row				(sLong Row, const double     *Data = NULL);
	bool						Ins_Row				(sLong Row, const CSG_Vector &Data);
	bool						Set_Col				(sLong Col, const double     *Data);
	bool						Set_Col				(sLong Col, const CSG_Vector &Data);
	bool						Set_Col				(           const CSG_Vector &Data);
	bool						Set_Row				(sLong Row, const double     *Data);
	bool						Set_Row				(sLong Row, const CSG_Vector &Data);
	bool						Set_Row				(           const CSG_Vector &Data);
	bool						Del_Col				(sLong Col);
	bool						Del_Row				(sLong Row);
	CSG_Vector					Get_Col				(sLong Col) const;
	CSG_Vector					Get_Row				(sLong Row) const;

	int							Get_NX				(void) const { return( (int)m_nx ); }
	int							Get_NY				(void) const { return( (int)m_ny ); }
	sLong						Get_NCols			(void) const { return(      m_nx ); }
	sLong						Get_NRows			(void) const { return(      m_ny ); }

	operator const double **						(void) const { return( (const double **)m_z ); }
	double **					Get_Data			(void) const { return(                  m_z ); }

	double						operator ()			(sLong  Row, sLong Col) const { return( m_z[Row][Col] ); }
	double *					operator []			(sLong  Row)            const { return( m_z[Row]      ); }
	double *					operator []			(size_t Row)            const { return( m_z[Row]      ); }
	double *					operator []			(int    Row)            const { return( m_z[Row]      ); }

	CSG_String					to_String			(int Width = -1, int Precision = -1, bool bScientific = false, const SG_Char *Separator = NULL)	const;
	bool						from_String			(const CSG_String &String);

	bool						is_Square			(void)	const	{	return( m_nx > 0 && m_nx == m_ny );	}
	bool						is_Equal			(const CSG_Matrix &Matrix)	const;

	bool						Assign				(double Scalar);
	bool						Assign				(const CSG_Matrix &Matrix);
	bool						Add					(double Scalar);
	bool						Add					(const CSG_Matrix &Matrix);
	bool						Subtract			(const CSG_Matrix &Matrix);
	bool						Multiply			(double Scalar);
	CSG_Vector					Multiply			(const CSG_Vector &Vector)	const;
	CSG_Matrix					Multiply			(const CSG_Matrix &Matrix)	const;

	bool						operator ==			(const CSG_Matrix &Matrix)	const;
	CSG_Matrix &				operator =			(double Scalar);
	CSG_Matrix &				operator =			(const CSG_Matrix &Matrix);
	CSG_Matrix &				operator +=			(double Scalar);
	CSG_Matrix &				operator +=			(const CSG_Matrix &Matrix);
	CSG_Matrix &				operator -=			(double Scalar);
	CSG_Matrix &				operator -=			(const CSG_Matrix &Matrix);
	CSG_Matrix &				operator *=			(double Scalar);
	CSG_Matrix &				operator *=			(const CSG_Matrix &Matrix);
	CSG_Matrix					operator +			(double Scalar)				const;
	CSG_Matrix					operator +			(const CSG_Matrix &Matrix)	const;
	CSG_Matrix					operator -			(double Scalar)				const;
	CSG_Matrix					operator -			(const CSG_Matrix &Matrix)	const;
	CSG_Matrix					operator *			(double Scalar)				const;
	CSG_Vector					operator *			(const CSG_Vector &Vector)	const;
	CSG_Matrix					operator *			(const CSG_Matrix &Matrix)	const;

	bool						Set_Zero			(void);
	bool						Set_Identity		(void);
	bool						Set_Transpose		(void);
	bool						Set_Inverse			(bool bSilent = true, int nSubSquare = 0);

	double						Get_Determinant		(void)						const;
	CSG_Matrix					Get_Transpose		(void)						const;
	CSG_Matrix					Get_Inverse			(bool bSilent = true, int nSubSquare = 0)	const;


private:

	sLong						m_nx, m_ny;

	double						**m_z;


	void						_On_Construction	(void);

};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_Matrix	operator *			(double Scalar, const CSG_Matrix &Matrix);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_Matrix	SG_Matrix_Get_Rotation		(double R                       , bool bDegree = false);
SAGA_API_DLL_EXPORT CSG_Matrix	SG_Matrix_Get_Rotation		(double Rx, double Ry, double Rz, bool bDegree = false);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool		SG_Matrix_LU_Decomposition	(int n,       int *Permutation,       double **Matrix                , bool bSilent = true, int *nRowChanges = NULL);
SAGA_API_DLL_EXPORT bool		SG_Matrix_LU_Solve			(int n, const int *Permutation, const double **Matrix, double *Vector, bool bSilent = true);

SAGA_API_DLL_EXPORT bool		SG_Matrix_Solve				(CSG_Matrix &Matrix, CSG_Vector &Vector, bool bSilent = true);
SAGA_API_DLL_EXPORT bool		SG_Matrix_Eigen_Reduction	(const CSG_Matrix &Matrix, CSG_Matrix &Eigen_Vectors, CSG_Vector &Eigen_Values, bool bSilent = true);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Grid_Radius
{
public:
	CSG_Grid_Radius(int maxRadius = 0);
	~CSG_Grid_Radius(void);

	bool						Create				(int maxRadius);
	void						Destroy				(void);

	int							Get_Maximum			(void)			{	return( m_maxRadius );	}

	int							Get_nPoints			(void)			{	return( m_nPoints );	}
	int							Get_nPoints			(int iRadius)	{	return( iRadius >= 0 && iRadius < m_maxRadius ? m_nPoints_R[iRadius] : 0 );	}

	double						Get_Point			(int iPoint, int &x, int &y)
	{
		if( iPoint >= 0 && iPoint < m_nPoints )
		{
			x	= m_Points[iPoint].x;
			y	= m_Points[iPoint].y;

			return( m_Points[iPoint].d );				// Distance...
		}

		return( -1. );
	}

	double						Get_Point			(int iPoint, int xOffset, int yOffset, int &x, int &y)
	{
		double	d;

		if( (d = Get_Point(iPoint, x, y)) >= 0. )
		{
			x	+= xOffset;
			y	+= yOffset;
		}

		return( d );
	}

	double						Get_Point			(int iRadius, int iPoint, int &x, int &y)
	{
		if( iRadius >= 0 && iRadius <= m_maxRadius && iPoint >= 0 && iPoint < m_nPoints_R[iRadius] )
		{
			x	= m_Points_R[iRadius][iPoint].x;
			y	= m_Points_R[iRadius][iPoint].y;

			return( m_Points_R[iRadius][iPoint].d );	// Distance...
		}

		return( -1. );
	}

	double						Get_Point			(int iRadius, int iPoint, int xOffset, int yOffset, int &x, int &y)
	{
		double	d;

		if( (d = Get_Point(iRadius, iPoint, x, y)) >= 0. )
		{
			x	+= xOffset;
			y	+= yOffset;
		}

		return( d );
	}


private:

	int							m_maxRadius, m_nPoints, *m_nPoints_R;

	typedef struct SSG_Grid_Radius
	{
		int						x, y;

		double					d;
	}
	TSG_Grid_Radius;

	TSG_Grid_Radius				*m_Points, **m_Points_R;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Random
{
public:
	CSG_Random(void);

	static void			Initialize		(void);
	static void			Initialize		(unsigned int Value);

	static double		Get_Uniform		(void);
	static double		Get_Uniform		(double min, double max);

	static double		Get_Gaussian	(double mean, double stddev);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Simple_Statistics
{
public:
	CSG_Simple_Statistics(void);
	CSG_Simple_Statistics(bool bHoldValues);
	CSG_Simple_Statistics(const CSG_Simple_Statistics &Statistics);
	CSG_Simple_Statistics(double Mean, double StdDev, sLong Count = 1000);
	CSG_Simple_Statistics(const CSG_Vector &Values, bool bHoldValues = false);

	bool						Create				(bool bHoldValues = false);
	bool						Create				(const CSG_Simple_Statistics &Statistics);
	bool						Create				(double Mean, double StdDev, sLong Count = 1000);
	bool						Create				(const CSG_Vector &Values, bool bHoldValues = false);

	void						Invalidate			(void);
	bool						Evaluate			(void);

	int							is_Evaluated		(void)	const	{	return( m_bEvaluated );	}

	bool						Set_Count			(sLong Count);

	sLong						Get_Count			(void)	const	{	return( m_nValues );	}
	double						Get_Weights			(void)	const	{	return( m_Weights );	}

	double						Get_Minimum			(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_Minimum  );	}
	double						Get_Maximum			(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_Maximum  );	}
	double						Get_Range			(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_Range    );	}
	double						Get_Sum				(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_Sum      );	}
	double						Get_Sum_Of_Squares	(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_Sum2     );	}
	double						Get_Mean			(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_Mean     );	}
	double						Get_Variance		(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_Variance );	}
	double						Get_StdDev			(void)		{	if( m_bEvaluated < 1 )	_Evaluate(1); return( m_StdDev   );	}

	double						Get_Kurtosis		(void)		{	if( m_bEvaluated < 2 )	_Evaluate(2); return( m_Kurtosis );	}
	double						Get_Skewness		(void)		{	if( m_bEvaluated < 2 )	_Evaluate(2); return( m_Skewness );	}
	double						Get_SkewnessPearson	(void);

	double						Get_Quantile		(double   Quantile);
	double						Get_Percentile		(double Percentile);
	double						Get_Median			(void);
	double						Get_Gini			(void);

	sLong						Get_IndexOfMinimum	(void);
	sLong						Get_IndexOfMaximum	(void);

	sLong						Get_nValues_Above	(double Threshold, bool bEquals = false);
	sLong						Get_nValues_Below	(double Threshold, bool bEquals = false);

	void						Add					(const CSG_Simple_Statistics &Statistics);

	void						Add_Value			(double Value, double Weight = 1.);

	double *					Get_Values			(void)		const	{	return( (double *)m_Values.Get_Array() );	}
	double						Get_Value			(sLong i)	const	{	return( i >= 0 && i < (sLong)m_Values.Get_Size() ? Get_Values()[i] : m_Mean );	}
	double						operator []			(sLong i)	const	{	return( i >= 0 && i < (sLong)m_Values.Get_Size() ? Get_Values()[i] : m_Mean );	}


	CSG_Simple_Statistics &		operator  =			(const CSG_Simple_Statistics &Statistics)	{	Create(Statistics);	return( *this );	}
	CSG_Simple_Statistics &		operator +=			(const CSG_Simple_Statistics &Statistics)	{	Add(Statistics);	return( *this );	}
	CSG_Simple_Statistics &		operator +=			(double Value)								{	Add_Value(Value);	return( *this );	}


protected:

	bool						m_bSorted;

	int							m_bEvaluated;

	sLong						m_nValues;

	double						m_Weights, m_Sum, m_Sum2, m_Minimum, m_Maximum, m_Range, m_Mean, m_Variance, m_StdDev, m_Kurtosis, m_Skewness, m_Gini;

	CSG_Array					m_Values;


	void						_Evaluate			(int Level = 1);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Unique_Value_Statistics
{
public:
	CSG_Unique_Value_Statistics					(void)	{}
	virtual ~CSG_Unique_Value_Statistics		(void)	{}

	virtual void			Create				(bool bWeights = false)	{	m_bWeights	= bWeights;	}

	int						Get_Count			(void)	const	{	return( (int)m_Count.Get_Size() );	}
	int						Get_Count			(int i)	const	{	return( m_Count[i] );	}
	double					Get_Weight			(int i)	const	{	return( m_bWeights ? m_Weight[i] : m_Count[i] );	}

	virtual int				Get_Majority		(bool bWeighted = false)	const;
	virtual int				Get_Minority		(bool bWeighted = false)	const;


protected:

	bool					m_bWeights{false};

	CSG_Array_Int			m_Count;

	CSG_Vector				m_Weight;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Unique_Number_Statistics : public CSG_Unique_Value_Statistics
{
public:
	CSG_Unique_Number_Statistics				(bool bWeights = false)	{	Create(bWeights);	}
	virtual ~CSG_Unique_Number_Statistics		(void)	{}

	virtual void			Create				(bool bWeights = false);

	void					operator +=			(double Value)	{	Add_Value(Value);	}
	void					Add_Value			(double Value, double Weight = 1.);
	double					Get_Value			(int Index)	const	{	return( m_Value[Index] );	}
	int						Get_Class_Index		(double Value)	const;
	bool					Get_Class			(int Index, double &Value, int &Count)	const
	{
		if( Index < 0 || Index >= Get_Count() )	return( false );

		Count	= m_Count[Index];
		Value	= m_Value[Index];

		return( true );
	}

	bool					Get_Majority		(double &Value            )	const	{	int	Count; return( Get_Class(CSG_Unique_Value_Statistics::Get_Majority(), Value, Count) );	}
	bool					Get_Majority		(double &Value, int &Count)	const	{	           return( Get_Class(CSG_Unique_Value_Statistics::Get_Majority(), Value, Count) && Count > 0 );	}
	bool					Get_Minority		(double &Value            )	const	{	int	Count; return( Get_Class(CSG_Unique_Value_Statistics::Get_Minority(), Value, Count) );	}
	bool					Get_Minority		(double &Value, int &Count)	const	{	           return( Get_Class(CSG_Unique_Value_Statistics::Get_Minority(), Value, Count) && Count > 0 );	}


private:

	CSG_Vector				m_Value;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Unique_String_Statistics : public CSG_Unique_Value_Statistics
{
public:
	CSG_Unique_String_Statistics				(bool bWeights = false)	{	Create(bWeights);	}
	virtual ~CSG_Unique_String_Statistics		(void)	{}

	virtual void			Create				(bool bWeights = false);

	void					operator +=			(const CSG_String &Value)	{	Add_Value(Value);	}
	void					Add_Value			(const CSG_String &Value, double Weight = 1.);
	const SG_Char *			Get_Value			(int Index)	const	{	return( m_Value[Index] );	}
	int						Get_Class_Index		(const CSG_String &Value)	const;
	bool					Get_Class			(int Index, CSG_String &Value, int &Count)	const
	{
		if( Index < 0 || Index >= Get_Count() )	return( false );

		Count	= m_Count[Index];
		Value	= m_Value[Index];

		return( true );
	}

	bool					Get_Majority		(CSG_String &Value            )	const	{	int	Count; return( Get_Class(CSG_Unique_Value_Statistics::Get_Majority(), Value, Count) );	}
	bool					Get_Majority		(CSG_String &Value, int &Count)	const	{	           return( Get_Class(CSG_Unique_Value_Statistics::Get_Majority(), Value, Count) && Count > 0 );	}
	bool					Get_Minority		(CSG_String &Value            )	const	{	int	Count; return( Get_Class(CSG_Unique_Value_Statistics::Get_Minority(), Value, Count) );	}
	bool					Get_Minority		(CSG_String &Value, int &Count)	const	{	           return( Get_Class(CSG_Unique_Value_Statistics::Get_Minority(), Value, Count) && Count > 0 );	}


private:

	CSG_Strings				m_Value;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Category_Statistics
{
public:
	         CSG_Category_Statistics		(TSG_Data_Type Type = SG_DATATYPE_String);
	virtual ~CSG_Category_Statistics		(void);

	void			Create					(TSG_Data_Type Type = SG_DATATYPE_String);
	void			Destroy					(void);

	TSG_Data_Type	Get_Category_Type		(void)	const;

	int				Add_Value				(int               Value);
	int				Add_Value				(double            Value);
	int				Add_Value				(const CSG_String &Value);

	void			operator +=				(int               Value)	{	Add_Value(Value);	}
	void			operator +=				(double            Value)	{	Add_Value(Value);	}
	void			operator +=				(const CSG_String &Value)	{	Add_Value(Value);	}

	bool			Sort					(void);

	int				Get_Count				(void         )	const;
	int				Get_Count				(int iCategory)	const;

	int				asInt					(int iCategory)	const;
	double			asDouble				(int iCategory)	const;
	CSG_String		asString				(int iCategory)	const;

	int				Get_Category			(int               Value)	const;
	int				Get_Category			(double            Value)	const;
	int				Get_Category			(const CSG_String &Value)	const;

	bool			Get_Category			(int iCategory, int        &Value, int &Count)	const
	{
		Count	= Get_Count(iCategory);
		Value	= asInt    (iCategory);

		return( iCategory >= 0 && iCategory < Get_Count() );
	}

	bool			Get_Category			(int iCategory, double     &Value, int &Count)	const
	{
		Count	= Get_Count(iCategory);
		Value	= asDouble (iCategory);

		return( iCategory >= 0 && iCategory < Get_Count() );
	}

	bool			Get_Category			(int iCategory, CSG_String &Value, int &Count)	const
	{
		Count	= Get_Count(iCategory);
		Value	= asString (iCategory);

		return( iCategory >= 0 && iCategory < Get_Count() );
	}

	int				Get_Majority			(void);
	bool			Get_Majority			(int        &Value            )	{	int	Count; return( Get_Category(Get_Majority(), Value, Count) );	}
	bool			Get_Majority			(double     &Value            )	{	int	Count; return( Get_Category(Get_Majority(), Value, Count) );	}
	bool			Get_Majority			(CSG_String &Value            )	{	int	Count; return( Get_Category(Get_Majority(), Value, Count) );	}
	bool			Get_Majority			(int        &Value, int &Count)	{	           return( Get_Category(Get_Majority(), Value, Count) && Count > 0 );	}
	bool			Get_Majority			(double     &Value, int &Count)	{	           return( Get_Category(Get_Majority(), Value, Count) && Count > 0 );	}
	bool			Get_Majority			(CSG_String &Value, int &Count)	{	           return( Get_Category(Get_Majority(), Value, Count) && Count > 0 );	}

	int				Get_Minority			(void);
	bool			Get_Minority			(int        &Value            )	{	int	Count; return( Get_Category(Get_Minority(), Value, Count) );	}
	bool			Get_Minority			(double     &Value            )	{	int	Count; return( Get_Category(Get_Minority(), Value, Count) );	}
	bool			Get_Minority			(CSG_String &Value            )	{	int	Count; return( Get_Category(Get_Minority(), Value, Count) );	}
	bool			Get_Minority			(int        &Value, int &Count)	{	           return( Get_Category(Get_Minority(), Value, Count) && Count > 0 );	}
	bool			Get_Minority			(double     &Value, int &Count)	{	           return( Get_Category(Get_Minority(), Value, Count) && Count > 0 );	}
	bool			Get_Minority			(CSG_String &Value, int &Count)	{	           return( Get_Category(Get_Minority(), Value, Count) && Count > 0 );	}


private:

	class CSG_Table	*m_pTable;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Histogram
{
public:
	CSG_Histogram(void);
	virtual ~CSG_Histogram(void);

	bool			Destroy				(void);

	CSG_Histogram						(const CSG_Histogram &Histogram);
	bool			Create				(const CSG_Histogram &Histogram);

	CSG_Histogram						(size_t nClasses, double Minimum, double Maximum);
	bool			Create				(size_t nClasses, double Minimum, double Maximum);

	CSG_Histogram						(size_t nClasses, double Minimum, double Maximum, const CSG_Vector &Values          , size_t maxSamples = 0);
	bool			Create				(size_t nClasses, double Minimum, double Maximum, const CSG_Vector &Values          , size_t maxSamples = 0);

	CSG_Histogram						(size_t nClasses, double Minimum, double Maximum, class CSG_Table *pTable, int Field, size_t maxSamples = 0);
	bool			Create				(size_t nClasses, double Minimum, double Maximum, class CSG_Table *pTable, int Field, size_t maxSamples = 0);

	CSG_Histogram						(size_t nClasses, double Minimum, double Maximum, class CSG_Grid  *pGrid            , size_t maxSamples = 0);
	bool			Create				(size_t nClasses, double Minimum, double Maximum, class CSG_Grid  *pGrid            , size_t maxSamples = 0);

	CSG_Histogram						(size_t nClasses, double Minimum, double Maximum, class CSG_Grids *pGrids           , size_t maxSamples = 0);
	bool			Create				(size_t nClasses, double Minimum, double Maximum, class CSG_Grids *pGrids           , size_t maxSamples = 0);

	//-----------------------------------------------------
	void			Add_Value			(double Value);

	bool			Scale_Element_Count	(double Scale);

	bool			Update				(void);

	size_t			Get_Class_Count		(void)		const	{	return( m_nClasses );	}

	size_t			Get_Element_Count	(void)		const	{	return( m_nClasses > 0 ? m_Cumulative[m_nClasses - 1] : 0 );	}
	size_t			Get_Element_Maximum	(void)		const	{	return( m_nMaximum );	}

	size_t			Get_Elements		(int    i)	const	{	return( Get_Elements((size_t)i) );	}
	size_t			Get_Elements		(size_t i)	const	{	return( i < m_nClasses ? m_Elements[i] : 0 );	}

	size_t			Get_Cumulative		(int    i)	const	{	return( Get_Cumulative((size_t)i) );	}
	size_t			Get_Cumulative		(size_t i)	const	{	return( i < m_nClasses ? m_Cumulative[i] : 0 );	}

	double			Get_Value			(double i)	const	{	return( m_nClasses < 1 ? m_Minimum : m_Minimum + i * m_ClassWidth );	}

	double			Get_Break			(int    i)	const	{	return( Get_Value((double)(i)) );	}
	double			Get_Break			(size_t i)	const	{	return( Get_Value((double)(i)) );	}

	double			Get_Center			(int    i)	const	{	return( Get_Value((double)(i + 0.5)) );	}
	double			Get_Center			(size_t i)	const	{	return( Get_Value((double)(i + 0.5)) );	}

	//-----------------------------------------------------
	CSG_Histogram &	operator =			(const CSG_Histogram &Histogram);

	void			operator +=			(double Value)		{	Add_Value(Value);	}

	size_t			operator []			(int    i)	const	{	return( Get_Elements(i) );	}
	size_t			operator []			(size_t i)	const	{	return( Get_Elements(i) );	}

	double			Get_Quantile		(double   Quantile)	const;
	double			Get_Quantile_Value	(double      Value)	const;

	double			Get_Percentile		(double Percentile)	const;
	double			Get_Percentile_Value(double      Value)	const;

	const CSG_Simple_Statistics &	Get_Statistics	(void)	const	{	return( m_Statistics );	}


private:

	size_t					m_nClasses, m_nMaximum, *m_Elements, *m_Cumulative;

	double					m_Minimum, m_Maximum, m_ClassWidth;

	CSG_Simple_Statistics	m_Statistics;


	void					_On_Construction	(void);

	bool					_Create				(size_t nClasses, double Minimum, double Maximum);

	bool					_Update				(sLong nElements);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Natural_Breaks
{
public:
	CSG_Natural_Breaks(void);
	virtual ~CSG_Natural_Breaks(void);

	CSG_Natural_Breaks				(class CSG_Table *pTable, int Field, int nClasses, int Histogram = 0);
	bool			Create			(class CSG_Table *pTable, int Field, int nClasses, int Histogram = 0);

	CSG_Natural_Breaks				(class CSG_Grid  *pGrid            , int nClasses, int Histogram = 0);
	bool			Create			(class CSG_Grid  *pGrid            , int nClasses, int Histogram = 0);

	CSG_Natural_Breaks				(class CSG_Grids *pGrids           , int nClasses, int Histogram = 0);
	bool			Create			(class CSG_Grids *pGrids           , int nClasses, int Histogram = 0);

	CSG_Natural_Breaks				(const CSG_Vector &Values          , int nClasses, int Histogram = 0);
	bool			Create			(const CSG_Vector &Values          , int nClasses, int Histogram = 0);

	int				Get_Count		(void)	const	{	return( m_Breaks.Get_N() );	}
	double			Get_Break		(int i)	const	{	return( m_Breaks[i] );	}

	double			operator []		(int i)	const	{	return( m_Breaks[i] );	}


private:

	CSG_Histogram	m_Histogram;

	CSG_Vector		m_Breaks, m_Values;


	bool			_Histogram		(int nClasses);

	double			_Get_Value		(int i);

	bool			_Calculate		(int nClasses);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum ESG_Cluster_Analysis_Method
{
	SG_CLUSTERANALYSIS_Minimum_Distance	= 0,
	SG_CLUSTERANALYSIS_Hill_Climbing,
	SG_CLUSTERANALYSIS_Combined
};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Cluster_Analysis
{
public:
	CSG_Cluster_Analysis(void);
	virtual ~CSG_Cluster_Analysis(void);

	bool					Create				(int nFeatures);
	bool					Destroy				(void);

	bool					Add_Element			(void);
	bool					Set_Feature			(sLong iElement, int iFeature, double Value);

	sLong					Get_Cluster			(sLong iElement) const	{	return( iElement >= 0 && iElement < Get_nElements() ? m_Clusters[iElement] : -1 );	}

	bool					Execute				(int Method, int nClusters, int nMaxIterations = 0, int Initialization = 0);

	sLong					Get_nElements		(void)	const	{	return(      m_Features.Get_Size() );	}
	int						Get_nFeatures		(void)	const	{	return(      m_nFeatures           );	}
	int						Get_nClusters		(void)	const	{	return( (int)m_nMembers.Get_Size() );	}

	int						Get_Iteration		(void)	const	{	return( m_Iteration );	}

	double					Get_SP				(void)	const	{	return( m_SP );			}

	int						Get_nMembers		(int iCluster)					const	{	return( m_nMembers[iCluster] );	}
	double					Get_Variance		(int iCluster)					const	{	return( m_Variance[iCluster] );	}
	double					Get_Centroid		(int iCluster, int iFeature)	const	{	return( m_Centroid[iCluster][iFeature] );	}


private:

	int						m_Iteration, m_nFeatures;

	double					m_SP;

	CSG_Array_Int			m_Clusters, m_nMembers;

	CSG_Array				m_Features;

	CSG_Vector				m_Variance;

	CSG_Matrix				m_Centroid;


	bool					_Minimum_Distance	(bool bInitialize, int nMaxIterations);

	bool					_Hill_Climbing		(bool bInitialize, int nMaxIterations);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum ESG_Classify_Supervised
{
	SG_CLASSIFY_SUPERVISED_BinaryEncoding	= 0,
	SG_CLASSIFY_SUPERVISED_ParallelEpiped,
	SG_CLASSIFY_SUPERVISED_MinimumDistance,
	SG_CLASSIFY_SUPERVISED_Mahalonobis,
	SG_CLASSIFY_SUPERVISED_MaximumLikelihood,
	SG_CLASSIFY_SUPERVISED_SAM,
	SG_CLASSIFY_SUPERVISED_WTA,
	SG_CLASSIFY_SUPERVISED_SID,
	SG_CLASSIFY_SUPERVISED_SVM
};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Classifier_Supervised
{
public:
	CSG_Classifier_Supervised(void);
	virtual ~CSG_Classifier_Supervised(void);

	void						Create						(int nFeatures);
	void						Destroy						(void);

	bool						Load						(const CSG_String &File);
	bool						Save						(const CSG_String &File, const SG_Char *Feature_Info = NULL);

	bool						Train_Clr_Samples			(void);
	bool						Train_Add_Sample			(const CSG_String &Class_ID, const CSG_Vector &Features);
	bool						Train						(bool bClr_Samples = false);

	bool						Add_Class					(const CSG_String &Class_ID, const CSG_Vector &Mean, const CSG_Vector &Min, const CSG_Vector &Max, const CSG_Matrix &Cov);

	CSG_String					Print						(void);

	//-----------------------------------------------------
	int							Get_Feature_Count			(void)			{	return( m_nFeatures );				}

	int							Get_Class_Count				(void)			{	return( m_nClasses );				}

	const CSG_String &			Get_Class_ID				(int iClass)				{	return( m_pClasses[iClass]->m_ID );	}
	double						Get_Class_Mean				(int iClass, int iFeature)	{	return( m_pClasses[iClass]->m_Mean[iFeature] );	}
	double						Get_Class_Minimum			(int iClass, int iFeature)	{	return( m_pClasses[iClass]->m_Min [iFeature] );	}
	double						Get_Class_Maximum			(int iClass, int iFeature)	{	return( m_pClasses[iClass]->m_Max [iFeature] );	}

	int							Get_Class					(const CSG_String &Class_ID);
	bool						Get_Class					(const CSG_Vector &Features, int &Class, double &Quality, int Method);

	//-----------------------------------------------------
	void						Set_Threshold_Distance		(double Value);
	double						Get_Threshold_Distance		(void);

	void						Set_Threshold_Angle			(double Value);
	double						Get_Threshold_Angle			(void);

	void						Set_Threshold_Probability	(double Value);
	double						Get_Threshold_Probability	(void);

	void						Set_Probability_Relative	(bool   Value);
	bool						Get_Probability_Relative	(void);

	void						Set_WTA						(int Method, bool bOn);
	bool						Get_WTA						(int Method);

	static CSG_String			Get_Name_of_Method			(int Method);
	static CSG_String			Get_Name_of_Quality			(int Method);


private:

	//-----------------------------------------------------
	class CClass
	{
	public:
		CClass(const CSG_String &ID) : m_ID(ID)	{}

		CSG_String				m_ID;

		double					m_Cov_Det, m_Mean_Spectral;

		CSG_Vector				m_Mean, m_Min, m_Max;

		CSG_Matrix				m_Cov, m_Cov_Inv, m_Samples;


		bool					Train						(void);

	};

	//-----------------------------------------------------
	bool						m_Probability_Relative, m_bWTA[SG_CLASSIFY_SUPERVISED_WTA];

	int							m_nFeatures, m_nClasses;

	double						m_Threshold_Distance, m_Threshold_Probability, m_Threshold_Angle;

	CSG_String					m_Info;

	CClass						**m_pClasses;


	void						_Get_Binary_Encoding		(const CSG_Vector &Features, int &Class, double &Quality);
	void						_Get_Parallel_Epiped		(const CSG_Vector &Features, int &Class, double &Quality);
	void						_Get_Minimum_Distance		(const CSG_Vector &Features, int &Class, double &Quality);
	void						_Get_Mahalanobis_Distance	(const CSG_Vector &Features, int &Class, double &Quality);
	void						_Get_Maximum_Likelihood		(const CSG_Vector &Features, int &Class, double &Quality);
	void						_Get_Spectral_Angle_Mapping	(const CSG_Vector &Features, int &Class, double &Quality);
	void						_Get_Spectral_Divergence	(const CSG_Vector &Features, int &Class, double &Quality);
	void						_Get_Winner_Takes_All		(const CSG_Vector &Features, int &Class, double &Quality);

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifdef WITH_MRMR

//---------------------------------------------------------
enum ESG_mRMR_Method
{
	SG_mRMR_Method_MID	= 0,	// Mutual Information Difference (MID)
	SG_mRMR_Method_MIQ			// Mutual Information Quotient (MIQ)
};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_mRMR
{
public: ////// public members and functions: //////////////

	CSG_mRMR(void);
	virtual ~CSG_mRMR(void);

	void						Destroy				(void);

	static CSG_String			Get_Description		(void);

	static bool					Parameters_Add		(class CSG_Parameters *pParameters, class CSG_Parameter *pNode = NULL);
	static int					Parameters_Enable	(class CSG_Parameters *pParameters, class CSG_Parameter *pParameter);

	void						Set_Verbose			(bool bOn = true)	{	m_bVerbose	= bOn;	}

	bool						Set_Data			(CSG_Table  &Data, int ClassField, class CSG_Parameters *pParameters);
	bool						Set_Data			(CSG_Matrix &Data, int ClassField, class CSG_Parameters *pParameters);
	bool						Set_Data			(CSG_Table  &Data, int ClassField = 0, double Threshold = -1.);
	bool						Set_Data			(CSG_Matrix &Data, int ClassField = 0, double Threshold = -1.);

	bool						Get_Selection		(class CSG_Parameters *pParameters);
	bool						Get_Selection		(int nFeatures, int Method);

	int							Get_Count			(void)	const;
	int							Get_Index			(int i)	const;
	CSG_String					Get_Name			(int i)	const;
	double						Get_Score			(int i)	const;


private: ///// private members and functions: /////////////

	bool						m_bDiscretized, m_bVerbose;

	long						m_nSamples, m_nVars;

	double						**m_Samples;

	CSG_Strings					m_VarNames;

	class CSG_Table				*m_pSelection;


	bool						Get_Memory			(int nVars, int nSamples);

	bool						Discretize			(double Threshold);

	double						Get_MutualInfo		(long v1, long v2);
	double						Get_MutualInfo		(double *pab, long pabhei, long pabwid);

	template <class T> double *	Get_JointProb		(T *img1, T *img2, long len, long maxstatenum, int &nstate1, int &nstate2);
	template <class T> void		Copy_Vector			(T *srcdata, long len, int *desdata, int &nstate);


	typedef struct SPool
	{
		char	Mask;
		long	Index;
		double	mival;
	}
	TPool;

	static int					Pool_Compare		(const void *a, const void *b);

};

//---------------------------------------------------------
#endif // WITH_MRMR


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Spline
{
public:
	CSG_Spline(void);
	virtual ~CSG_Spline(void);

	void						Destroy				(void);

	bool						Create				(double *xValues, double *yValues, int nValues, double yA = 1.0e30, double yB = 1.0e30);
	bool						Create				(double yA = 1.0e30, double yB = 1.0e30);

	void						Add					(double x, double y);

	int							Get_Count			(void)	const	{	return( m_x.Get_N() );	}
	double						Get_xMin			(void)	const	{	return( m_x.Get_N() > 0 ? m_x(0                 ) : 0. );	}
	double						Get_xMax			(void)	const	{	return( m_x.Get_N() > 0 ? m_x(m_x.Get_Size() - 1) : 0. );	}
	double						Get_x				(int i)	const	{	return( i >= 0 && i < m_x.Get_N() ? m_x(i) : 0. );	}
	double						Get_y				(int i)	const	{	return( i >= 0 && i < m_y.Get_N() ? m_y(i) : 0. );	}

	bool						Get_Value			(double x, double &y);
	double						Get_Value			(double x);


protected:

	bool						m_bCreated;

	CSG_Vector					m_x, m_y, m_z;


	bool						_Create				(double yA, double yB);

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Thin_Plate_Spline
{
public:
	CSG_Thin_Plate_Spline(void);
	virtual ~CSG_Thin_Plate_Spline(void);

	bool					Destroy				(void);

	bool					Set_Point_Count		(int Count)	{	return( m_Points.Set_Count(Count) );	}
	int						Get_Point_Count		(void)		{	return( (int)m_Points.Get_Count() );	}

	CSG_Points_3D &			Get_Points			(void)		{	return( m_Points );	}

	bool					Add_Point			(double x, double y, double z)	{	return( m_Points.Add(  x,   y, z) );	}
	bool					Add_Point			(const TSG_Point &p, double z)	{	return( m_Points.Add(p.x, p.y, z) );	}

	bool					Set_Point			(int Index, double x, double y, double z)
	{
		if( Index >= 0 && Index < m_Points.Get_Count() )
		{
			m_Points[Index].x	= x;
			m_Points[Index].y	= y;
			m_Points[Index].z	= z;

			return( true );
		}

		return( false );
	}

	bool					Set_Point			(int Index, const TSG_Point &p, double z)	{	return( Set_Point(Index, p.x, p.y, z) );	}

	bool					Create				(double Regularization = 0., bool bSilent = true);

	bool					is_Okay				(void)	{	return( m_V.Get_N() > 0 );		}

	double					Get_Value			(double x, double y);


private:

	CSG_Points_3D			m_Points;

	CSG_Vector				m_V;


	double					_Get_hDistance		(TSG_Point_3D A, TSG_Point_3D B);
	double					_Get_Base_Funtion	(double x);
	double					_Get_Base_Funtion	(TSG_Point_3D A, double x, double y);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	TESTDIST_TYPE_Left	= 0,
	TESTDIST_TYPE_Right,
	TESTDIST_TYPE_Middle,
	TESTDIST_TYPE_TwoTail
}
TSG_Test_Distribution_Type;

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Test_Distribution
{
public:

	static double			Get_F_Tail_from_R2	(double R2, int nPredictors, int nSamples, TSG_Test_Distribution_Type Type = TESTDIST_TYPE_Right);

	static double			Get_F_Tail			(double F    , int dfn, int dfd, TSG_Test_Distribution_Type Type = TESTDIST_TYPE_Right);
	static double			Get_F_Inverse		(double alpha, int dfn, int dfd, TSG_Test_Distribution_Type Type = TESTDIST_TYPE_Right);

	static double			Get_T_Tail			(double T    , int df, TSG_Test_Distribution_Type Type = TESTDIST_TYPE_Right);
	static double			Get_T_Inverse		(double alpha, int df, TSG_Test_Distribution_Type Type = TESTDIST_TYPE_Right);

	static double			Get_Norm_P			(double Z);
	static double			Get_Norm_Z			(double P);


private:

	static double			Get_Gamma			(double F, double dfn, double dfd);
	static double			Get_Log_Gamma		(double a);

	static double			Get_T_P				(double T, int df);
	static double			Get_T_Z				(double T, int df);
	static double			Get_T_Inv			(double p, int df);

	static double			_Change_Tail_Type	(double p, TSG_Test_Distribution_Type from, TSG_Test_Distribution_Type to, bool bNegative);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// Values: (matrix) array with number of variables = number of rows (x), number of samples = number of columns (y)
SAGA_API_DLL_EXPORT CSG_Matrix	SG_Get_Correlation_Matrix		(const CSG_Matrix &Values, bool bCovariances = false);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	REGRESSION_CORR_None	= 0,
	REGRESSION_CORR_Smith,
	REGRESSION_CORR_Wherry_1,
	REGRESSION_CORR_Wherry_2,
	REGRESSION_CORR_Olkin_Pratt,
	REGRESSION_CORR_Pratt,
	REGRESSION_CORR_Claudy_3
}
TSG_Regression_Correction;

//---------------------------------------------------------
SAGA_API_DLL_EXPORT double		SG_Regression_Get_Adjusted_R2	(double R2, int nSamples, int nPredictors, TSG_Regression_Correction Correction = REGRESSION_CORR_Wherry_1);


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	REGRESSION_Linear	= 0,	// Y = a + b * X
	REGRESSION_Rez_X,			// Y = a + b / X
	REGRESSION_Rez_Y,			// Y = a / (b - X)
	REGRESSION_Pow,				// Y = a * X^b
	REGRESSION_Exp,				// Y = a * e^(b * X)
	REGRESSION_Log				// Y = a + b * ln(X)
}
TSG_Regression_Type;

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Regression
{
public:
	CSG_Regression(void);
	virtual ~CSG_Regression(void);

	void						Destroy				(void);

	bool						Set_Values			(int nValues, double *x, double *y);
	bool						Add_Values			(double x, double y);

	int							Get_Count			(void)		const	{	return( (int)m_x.Get_Size() );	}

	double						Get_xValue			(int i)		const	{	return( i >= 0 && i < Get_Count() ? m_x[i] : 0. );	}
	double						Get_yValue			(int i)		const	{	return( i >= 0 && i < Get_Count() ? m_y[i] : 0. );	}
	bool						Get_Values			(int i, double &x, double &y)	const
	{
		if( i >= 0 && i < Get_Count() )
		{
			x = m_x[i];
			y = m_y[i];

			return( true );
		}

		return( false );
	}

	double						Get_xMin			(void)		const	{	return( m_xMin   );	}
	double						Get_xMax			(void)		const	{	return( m_xMax   );	}
	double						Get_xMean			(void)		const	{	return( m_xMean  );	}
	double						Get_xVariance		(void)		const	{	return( m_xVar   );	}
	double						Get_x				(double y)	const;	// returns INF on error, this can be checked using the _finite() function (libc, include <float.h>)...

	double						Get_yMin			(void)		const	{	return( m_yMin   );	}
	double						Get_yMax			(void)		const	{	return( m_yMax   );	}
	double						Get_yMean			(void)		const	{	return( m_yMean  );	}
	double						Get_yVariance		(void)		const	{	return( m_yVar   );	}
	double						Get_y				(double x)	const;	// returns INF on error, this can be checked using the _finite() function (libc, include <float.h>)...

	double						Get_Constant		(void)		const	{	return( m_RConst );	}
	double						Get_Coefficient		(void)		const	{	return( m_RCoeff );	}
	double						Get_R				(void)		const	{	return( m_R      );	}
	double						Get_R2				(void)		const	{	return( m_R2     );	}
	double						Get_R2_Adj			(void)		const	{	return( m_R2_Adj );	}
	double						Get_P				(void)		const	{	return( m_P      );	}
	double						Get_StdError		(void)		const	{	return( m_SE     );	}

	const SG_Char *				asString			(void);

	TSG_Regression_Type			Get_Type			(void)		const	{	return( m_Type );	}

	bool						Calculate			(                                   TSG_Regression_Type Type = REGRESSION_Linear, bool bStdError = false);
	bool						Calculate			(int nValues, double *x, double *y, TSG_Regression_Type Type = REGRESSION_Linear, bool bStdError = false);


protected:

	double						m_RConst, m_RCoeff, m_R, m_R2, m_R2_Adj, m_P, m_SE,
								m_xMin, m_xMax, m_xMean, m_xVar,
								m_yMin, m_yMax, m_yMean, m_yVar;

	CSG_Vector					m_x, m_y;

	TSG_Regression_Type			m_Type;


	double						_Y_Transform		(double x);
	double						_X_Transform		(double y);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum ESG_Multiple_Regression_Info_Vars
{
	MLR_VAR_ID	= 0,
	MLR_VAR_NAME,
	MLR_VAR_RCOEFF,
	MLR_VAR_R,
	MLR_VAR_R2,
	MLR_VAR_R2_ADJ,
	MLR_VAR_SE,
	MLR_VAR_T,
	MLR_VAR_SIG,
	MLR_VAR_P
};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Regression_Multiple
{
public:
	CSG_Regression_Multiple(bool bIntercept = true);
	virtual ~CSG_Regression_Multiple(void);

	void						Destroy				(void);

	bool						Set_Data			(const CSG_Matrix &Samples, CSG_Strings *pNames = NULL);

	void						Set_With_Intercept	(bool bOn = true)		{	m_bIntercept	= bOn;	}
	bool						Get_With_Intercept	(void)			const	{	return( m_bIntercept );	}

	bool						Get_Model			(const CSG_Matrix &Samples                           , CSG_Strings *pNames = NULL);
	bool						Get_Model_Forward	(const CSG_Matrix &Samples, double P_in              , CSG_Strings *pNames = NULL);
	bool						Get_Model_Backward	(const CSG_Matrix &Samples, double P_out             , CSG_Strings *pNames = NULL);
	bool						Get_Model_Stepwise	(const CSG_Matrix &Samples, double P_in, double P_out, CSG_Strings *pNames = NULL);

	bool						Get_Model			(void);
	bool						Get_Model_Forward	(double P_in);
	bool						Get_Model_Backward	(double P_out);
	bool						Get_Model_Stepwise	(double P_in, double P_out);

	bool						Get_CrossValidation	(int nSubSamples = 0);

	CSG_String					Get_Info			(void)			const;
	class CSG_Table *			Get_Info_Regression	(void)			const	{	return( m_pRegression );	}
	class CSG_Table *			Get_Info_Model		(void)			const	{	return( m_pModel );			}
	class CSG_Table *			Get_Info_Steps		(void)			const	{	return( m_pSteps );			}

	double						Get_R2				(void)			const;
	double						Get_R2_Adj			(void)			const;
	double						Get_StdError		(void)			const;
	double						Get_F				(void)			const;
	double						Get_P				(void)			const;
	double						Get_CV_RMSE			(void)			const;
	double						Get_CV_NRMSE		(void)			const;
	double						Get_CV_R2			(void)			const;
	int							Get_CV_nSamples		(void)			const;
	int							Get_DegFreedom		(void)			const;
	int							Get_nSamples		(void)			const;
	int							Get_nPredictors		(void)			const;
	int							Get_Predictor		(int i)			const	{	return( i >= 0 && i < Get_nPredictors() ? m_Predictor[i] : -1 );	}

	double						Get_RConst			(void)			const;
	const SG_Char *				Get_Name			(int iVariable)	const;
	double						Get_ID				(int iVariable)	const	{	return( Get_Parameter(iVariable, MLR_VAR_ID    ) );	}
	double						Get_RCoeff			(int iVariable)	const	{	return( Get_Parameter(iVariable, MLR_VAR_RCOEFF) );	}
	double						Get_R2_Partial		(int iVariable)	const	{	return( Get_Parameter(iVariable, MLR_VAR_R2    ) );	}
	double						Get_R2_Partial_Adj	(int iVariable)	const	{	return( Get_Parameter(iVariable, MLR_VAR_R2_ADJ) );	}
	double						Get_StdError		(int iVariable)	const	{	return( Get_Parameter(iVariable, MLR_VAR_SE    ) );	}
	double						Get_T				(int iVariable)	const	{	return( Get_Parameter(iVariable, MLR_VAR_T     ) );	}
	double						Get_P				(int iVariable)	const	{	return( Get_Parameter(iVariable, MLR_VAR_SIG   ) );	}

	double						Get_Parameter		(int iVariable, int Parameter)	const;

	double						Get_Value			(const CSG_Vector &Predictors)					const;
	bool						Get_Value			(const CSG_Vector &Predictors, double &Value)	const;

	double						Get_Residual		(int iSample)					const;
	bool						Get_Residual		(int iSample, double &Residual)	const;

	bool						Get_Residuals		(CSG_Vector &Residuals)			const;


protected:

	bool						m_bIntercept;

	int							*m_bIncluded, *m_Predictor, m_nPredictors;

	CSG_Strings					m_Names;

	CSG_Matrix					m_Samples, m_Samples_Model;

	class CSG_Table				*m_pRegression, *m_pModel, *m_pSteps;


	bool						_Initialize			(bool bInclude);

	double						_Get_F				(int nPredictors, int nSamples, double r2_full, double r2_reduced);
	double						_Get_P				(int nPredictors, int nSamples, double r2_full, double r2_reduced);

	bool						_Get_Regression		(const class CSG_Matrix &Samples);

	int							_Get_Step_In		(CSG_Matrix &X, double P_in , double &R2, const CSG_Matrix &Samples);
	int							_Get_Step_Out		(CSG_Matrix &X, double P_out, double &R2);

	bool						_Set_Step_Info		(const CSG_Matrix &X);
	bool						_Set_Step_Info		(const CSG_Matrix &X, double R2_prev, int iVariable, bool bIn);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Regression_Weighted
{
public:
	CSG_Regression_Weighted(void);
	virtual ~CSG_Regression_Weighted(void);

	bool						Destroy				(void);

	bool						Add_Sample			(double Weight, double Dependent, const CSG_Vector &Predictors);
	int							Get_Sample_Count	(void)	const	{	return( m_X.Get_NY()     );	}
	int							Get_Predictor_Count	(void)	const	{	return( m_X.Get_NX() - 1 );	}

	bool						Calculate			(const CSG_Vector &Weights, const CSG_Vector &Dependents, const CSG_Matrix &Predictors, bool bLogistic = false);
	bool						Calculate			(bool bLogistic = false);

	double						Get_R2				(void)	const	{	return( m_r2   );	}
	const CSG_Vector &			Get_RCoeff			(void)	const	{	return( m_b    );	}
	double						Get_RCoeff			(int i)	const	{	return( m_b[i] );	}
	double						operator []			(int i)	const	{	return( m_b[i] );	}

	//-----------------------------------------------------
	int							Get_Log_maxIter		(void)	const	{	return( m_Log_maxIter    );	}
	double						Get_Log_Epsilon		(void)	const	{	return( m_Log_Epsilon    );	}
	double						Get_Log_Difference	(void)	const	{	return( m_Log_Difference );	}

	bool						Set_Log_maxIter		(int    maxIter   );
	bool						Set_Log_Epsilon		(double Epsilon   );
	bool						Set_Log_Difference	(double Difference);

	//-----------------------------------------------------
	bool						Get_CrossValidation	(int nSubSamples = 0);

	int							Get_CV_nSamples		(void)	const	{	return( m_CV_nSamples );	}
	double						Get_CV_RMSE			(void)	const	{	return( m_CV_RMSE     );	}
	double						Get_CV_NRMSE		(void)	const	{	return( m_CV_NRMSE    );	}
	double						Get_CV_R2			(void)	const	{	return( m_CV_R2       );	}


private:

	int							m_Log_maxIter, m_CV_nSamples;

	double						m_r2, m_Log_Epsilon, m_Log_Difference, m_CV_RMSE, m_CV_NRMSE, m_CV_R2;

	CSG_Vector					m_y, m_w, m_b;

	CSG_Matrix					m_X;


	CSG_Vector					_Log_Get_Beta		(const CSG_Matrix &X, const CSG_Vector &y, const CSG_Vector &w);
	CSG_Vector					_Log_Get_Beta		(const CSG_Vector &b, const CSG_Matrix &X, const CSG_Vector &y, const CSG_Vector &w, const CSG_Vector &p);
	CSG_Matrix					_Log_Get_Xwp		(const CSG_Vector &p, const CSG_Matrix &X, const CSG_Vector &w);
	CSG_Vector					_Log_Get_Ywp		(const CSG_Vector &p, const CSG_Vector &y, const CSG_Vector &w);
	CSG_Vector					_Log_Get_Props		(const CSG_Matrix &X, const CSG_Vector &b);
	bool						_Log_NoChange		(const CSG_Vector &b_old, const CSG_Vector &b_new);
	bool						_Log_OutOfControl	(const CSG_Vector &b_old, const CSG_Vector &b_new);

};


///////////////////////////////////////////////////////////
//														 //
//				Formula Parser (A. Ringeler)			 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef double (*TSG_Formula_Function_0)(void);
typedef double (*TSG_Formula_Function_1)(double);
typedef double (*TSG_Formula_Function_2)(double, double);
typedef double (*TSG_Formula_Function_3)(double, double, double);

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Formula
{
public:
	CSG_Formula(void);
	virtual ~CSG_Formula(void);

	bool						Destroy				(void);

	static CSG_String			Get_Help_Operators	(bool bHTML = true, const CSG_String Additional[][2] = NULL);

	bool						Get_Error			(CSG_String &Message);

	bool						Add_Function		(const char *Name, TSG_Formula_Function_1 Function, int nParameters, bool bVarying = false);

	bool						Set_Formula			(const CSG_String &Formula);
	CSG_String					Get_Formula			(void)	const	{	return( m_sFormula );	}

	void						Set_Variable		(char Variable, double Value);

	double						Get_Value			(void                       )	const;
	double						Get_Value			(double x                   )	const;
	double						Get_Value			(const CSG_Vector &Values   )	const;
	double						Get_Value			(double *Values, int nValues)	const;
	double						Get_Value			(const char *Arguments, ... )	const;

	const char *				Get_Used_Variables	(void);


	//-----------------------------------------------------
	typedef struct SSG_Function
	{
		const char				*Name;

		TSG_Formula_Function_1	Function;

		int						nParameters;
		
		bool					bVarying;
	}
	TSG_Function;


private:

	//-----------------------------------------------------
	typedef struct SSG_Formula
	{
		char					*code;

		double					*ctable;
	}
	TSG_Formula;


	//-----------------------------------------------------
	bool						m_bError, m_Vars_Used[256];

	int							m_Error_Position, m_Length;

	TSG_Formula					m_Formula;

	TSG_Function				*m_Functions;


	CSG_String					m_sFormula, m_sError;

	const char					*m_error;

	int							m_pctable;

	double						m_Parameters[32], *m_ctable;


	void						_Set_Error			(const CSG_String &Error = "");

	double						_Get_Value			(const double *Parameters, TSG_Formula Function)	const;

	int							_is_Operand			(char c);
	int							_is_Operand_Code	(char c);
	int							_is_Number			(char c);

	int							_Get_Function		(int i, char *Name, int *nParameters, int *bVarying);
	int							_Get_Function		(const char *Name);

	TSG_Formula					_Translate			(const char *source, const char *args, int *length, int *error);

	char *						_i_trans			(char *function, char *begin, char *end);
	char *						_comp_time			(char *function, char *fend, int npars);
	int							_max_size			(const char *source);
	char *						_my_strtok			(char *s);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum ESG_Trend_String
{
	SG_TREND_STRING_Formula	= 0,
	SG_TREND_STRING_Function,
	SG_TREND_STRING_Formula_Parameters,
	SG_TREND_STRING_Complete,
	SG_TREND_STRING_Compact
};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Trend
{
public:
	CSG_Trend(void);

	bool						Set_Formula			(const CSG_String &Formula);
	CSG_String					Get_Formula			(int Type = SG_TREND_STRING_Complete);

	int							Get_Parameter_Count	(void) const	{	return( m_Params   .Get_Count() );	}
	double *					Get_Parameters		(void) const	{	return( m_Params.m_A.Get_Data() );	}
	bool						Init_Parameter		(const SG_Char &Variable, double Value);

	void						Clr_Data			(void);
	bool						Add_Data			(double  x, double  y);
	void						Set_Data			(double *x, double *y, int n, bool bAdd = false);
	void						Set_Data			(const CSG_Points &Data     , bool bAdd = false);
	int							Get_Data_Count		(void)	const	{	return( (int)m_Data[0].Get_Count() );	}
	double						Get_Data_X			(int i)	const	{	return( m_Data[0][i] );	}
	double						Get_Data_XMin		(void)			{	return( m_Data[0].Get_Minimum() );	}
	double						Get_Data_XMax		(void)			{	return( m_Data[0].Get_Maximum() );	}
	CSG_Simple_Statistics &		Get_Data_XStats		(void)			{	return( m_Data[0] );	}
	double						Get_Data_Y			(int i)	const	{	return( m_Data[1][i] );	}
	double						Get_Data_YMin		(void)			{	return( m_Data[1].Get_Minimum() );	}
	double						Get_Data_YMax		(void)			{	return( m_Data[1].Get_Maximum() );	}
	CSG_Simple_Statistics &		Get_Data_YStats		(void)			{	return( m_Data[1] );	}

	bool						Set_Max_Iterations	(int Iterations);
	int							Get_Max_Iterations	(void)	const	{	return( m_Iter_Max);	}
	bool						Set_Max_Lambda		(double Lambda);
	double						Get_Max_Lambda		(void)	const	{	return( m_Lambda_Max);	}

	bool						Get_Trend			(double *x, double *y, int n, const CSG_String &Formula);
	bool						Get_Trend			(const CSG_Points &Data     , const CSG_String &Formula);
	bool						Get_Trend			(const CSG_String &Formula);
	bool						Get_Trend			(void);

	bool						is_Okay				(void)	const	{	return( m_bOkay );	}

	CSG_String					Get_Error			(void);

	double						Get_ChiSquare		(void)	const	{	return( m_bOkay ? m_ChiSqr   : 0. );	}
	double						Get_R2				(void)	const	{	return( m_bOkay ? m_ChiSqr_o : 0. );	}

	double						Get_Value			(double x)	const	{	return( m_bOkay ? m_Formula.Get_Value(x) : 0. );	}


private:

	//-----------------------------------------------------
	class SAGA_API_DLL_EXPORT CParams
	{
	public:
		CParams(void)	{}

		bool					Create				(const CSG_String &Variables);
		bool					Destroy				(void);

		int						Get_Count			(void)	const	{	return( (int)m_Variables.Length() );	}


		CSG_String				m_Variables;

		CSG_Vector				m_A, m_Atry, m_dA, m_dA2, m_Beta;

		CSG_Matrix				m_Alpha, m_Covar;

	};


	//-----------------------------------------------------
	bool						m_bOkay;

	int							m_Iter_Max;

	double						m_ChiSqr, m_ChiSqr_o, m_Lambda, m_Lambda_Max;

	CParams						m_Params;

	CSG_Simple_Statistics		m_Data[2];

	CSG_Formula					m_Formula;


	bool						_Fit_Function		(void);
	bool						_Get_Gaussj			(void);
	bool						_Get_mrqcof			(CSG_Vector &Parameters, CSG_Matrix &Alpha, CSG_Vector &Beta);

	void						_Get_Function		(double &y, double *dy_da, double x, const double *Parameters);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Trend_Polynom
{
public:
	CSG_Trend_Polynom(void);

	bool						Destroy				(void);

	bool						Set_Order			(int Order = 1);

	bool						Clr_Data			(void);
	bool						Set_Data			(double *x, double *y, int n, bool bAdd = false);
	bool						Add_Data			(double  x, double  y);
	int							Get_Data_Count		(void)		const	{	return( m_x.Get_N() );	}
	double						Get_Data_X			(int i)		const	{	return( m_x(i) );	}
	double						Get_Data_Y			(int i)		const	{	return( m_y(i) );	}

	bool						Get_Trend			(void);

	int							Get_Order			(void)		const	{	return( m_Order     );	}
	int							Get_nCoefficients	(void)		const	{	return( m_Order + 1 );	}
	double						Get_Coefficient		(int i)		const	{	return( m_a(i)      );	}
	double						Get_R2				(void)		const	{	return( m_r2        );	}

	double						Get_Value			(double x)	const;


private:

	double						m_r2;

	int							m_Order;

	CSG_Vector					m_x, m_y, m_a;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__SAGA_API__mat_tools_H
