
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
//                      api_core.h                       //
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
#ifndef HEADER_INCLUDED__SAGA_API__api_core_H
#define HEADER_INCLUDED__SAGA_API__api_core_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
/** \file api_core.h
* Classes, functions, type definitions for basic helpers 
* like data types, memory allocation, string handling, file
* access, translations, colors and user interface communication.
* @see CSG_String
* @see CSG_File
* @see CSG_Array
* @see CSG_Bytes
*/


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifdef _SAGA_MSW
	#define	_SAGA_DLL_EXPORT		__declspec( dllexport )
	#define	_SAGA_DLL_IMPORT		__declspec( dllimport )
#else
	#define	_SAGA_DLL_EXPORT
	#define	_SAGA_DLL_IMPORT
#endif

//---------------------------------------------------------
#ifdef _SAGA_API_EXPORTS
	#define	SAGA_API_DLL_EXPORT		_SAGA_DLL_EXPORT
#else
	#define	SAGA_API_DLL_EXPORT		_SAGA_DLL_IMPORT
#endif


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef SWIG

#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <string>

#endif	// #ifdef SWIG


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// this is defined by configure, but will not be on a normal application build

#ifndef SIZEOF_LONG
	#if defined(__alpha) || defined(__sparcv9) || defined(__LP64__) || (defined(__HOS_AIX__) && defined(_LP64))
		#define SIZEOF_LONG		8
	#else
		#define SIZEOF_LONG		4
	#endif
#endif

//---------------------------------------------------------
#ifdef _TYPEDEF_BOOL
	typedef unsigned int		bool;
	#define true				((bool)1)
	#define false				((bool)0)
#endif	// _TYPEDEF_BOOL

//---------------------------------------------------------
#ifdef _TYPEDEF_BYTE
	typedef unsigned char		BYTE;
#endif	// _TYPEDEF_BYTE

//---------------------------------------------------------
#ifdef _TYPEDEF_WORD
	typedef unsigned short		WORD;
	#if SIZEOF_LONG == 4
		typedef unsigned long	DWORD;
	#else
		typedef unsigned int	DWORD;
	#endif
#endif	// _TYPEDEF_WORD

//---------------------------------------------------------
	typedef   signed long long	sLong;
	typedef unsigned long long	uLong;

//---------------------------------------------------------
#if defined(_SAGA_MSW)
	#include <float.h>
	#define SG_is_NaN			_isnan
#elif defined(isnan)
	#define SG_is_NaN			isnan
#else
	#define SG_is_NaN(x)		(x != x)
#endif


///////////////////////////////////////////////////////////
//														 //
//                   Parallelization                     //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
SAGA_API_DLL_EXPORT void			SG_OMP_Set_Max_Num_Threads	(int iCores);
SAGA_API_DLL_EXPORT int				SG_OMP_Get_Max_Num_Threads	(void);
SAGA_API_DLL_EXPORT int				SG_OMP_Get_Max_Num_Procs	(void);
SAGA_API_DLL_EXPORT int				SG_OMP_Get_Thread_Num		(void);


///////////////////////////////////////////////////////////
//														 //
//						Memory							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SG_GET_LONG(b0, b1, b2, b3)	((long) (((BYTE)(b0) | ((WORD)(b1) << 8)) | (((DWORD)(BYTE)(b2)) << 16) | (((DWORD)(BYTE)(b3)) << 24)))

#define SG_GET_BYTE_0(vLong)		((BYTE) ((vLong)      ))
#define SG_GET_BYTE_1(vLong)		((BYTE) ((vLong) >>  8))
#define SG_GET_BYTE_2(vLong)		((BYTE) ((vLong) >> 16))
#define SG_GET_BYTE_3(vLong)		((BYTE) ((vLong) >> 24))

//---------------------------------------------------------
SAGA_API_DLL_EXPORT void *			SG_Malloc			(size_t size);
SAGA_API_DLL_EXPORT void *			SG_Calloc			(size_t num, size_t size);
SAGA_API_DLL_EXPORT void *			SG_Realloc			(void *memblock, size_t size);
SAGA_API_DLL_EXPORT void			SG_Free				(void *memblock);

#define SG_FREE_SAFE(PTR)			{ if( PTR ) { SG_Free (PTR); PTR = NULL; } }
#define SG_DELETE_SAFE(PTR)			{ if( PTR ) { delete  (PTR); PTR = NULL; } }
#define SG_DELETE_ARRAY(PTR)		{ if( PTR ) { delete[](PTR); PTR = NULL; } }

//---------------------------------------------------------
SAGA_API_DLL_EXPORT void			SG_Swap_Bytes		(void *Buffer, int nBytes);

SAGA_API_DLL_EXPORT int				SG_Mem_Get_Int		(const char *Buffer        , bool bSwapBytes);
SAGA_API_DLL_EXPORT void			SG_Mem_Set_Int		(char *Buffer, int Value   , bool bSwapBytes);
SAGA_API_DLL_EXPORT double			SG_Mem_Get_Double	(const char *Buffer        , bool bSwapBytes);
SAGA_API_DLL_EXPORT void			SG_Mem_Set_Double	(char *Buffer, double Value, bool bSwapBytes);


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Buffer
{
public:

						CSG_Buffer		(void);
	bool				Create			(void);

						CSG_Buffer		(const CSG_Buffer &Buffer);
	bool				Create			(const CSG_Buffer &Buffer);

						CSG_Buffer		(size_t Size);
	bool				Create			(size_t Size);

	virtual				~CSG_Buffer		(void);
	void				Destroy			(void);

	bool				Set_Size		(size_t Size, bool bShrink = true);
	bool				Inc_Size		(size_t Size)				{	return( Set_Size(m_Size + Size) );	}
	size_t				Get_Size		(void)				const	{	return( m_Size );	}

	bool				Set_Data		(const char *Data, size_t Size, bool bShrink = true);
	char *				Get_Data		(int Offset = 0)	const	{	return( m_Data + Offset );		}

	CSG_Buffer &		operator =		(const CSG_Buffer &Buffer)	{	Create(Buffer);	return( *this );	}
	char &				operator []		(int Position)				{	return( m_Data[Position] );	}
	char				operator []		(int Position)		const	{	return( m_Data[Position] );	}

	void				Add_Value		(char   Value, bool bBigEndian = false)	{	if( Inc_Size(sizeof(Value)) ) Set_Value((int)m_Size - sizeof(Value), Value, bBigEndian);	}
	void				Add_Value		(short  Value, bool bBigEndian = false)	{	if( Inc_Size(sizeof(Value)) ) Set_Value((int)m_Size - sizeof(Value), Value, bBigEndian);	}
	void				Add_Value		(int    Value, bool bBigEndian = false)	{	if( Inc_Size(sizeof(Value)) ) Set_Value((int)m_Size - sizeof(Value), Value, bBigEndian);	}
	void				Add_Value		(float  Value, bool bBigEndian = false)	{	if( Inc_Size(sizeof(Value)) ) Set_Value((int)m_Size - sizeof(Value), Value, bBigEndian);	}
	void				Add_Value		(double Value, bool bBigEndian = false)	{	if( Inc_Size(sizeof(Value)) ) Set_Value((int)m_Size - sizeof(Value), Value, bBigEndian);	}

	CSG_Buffer &		operator +=		(char   Value)				{	Add_Value(Value);	return( *this );	}
	CSG_Buffer &		operator +=		(short  Value)				{	Add_Value(Value);	return( *this );	}
	CSG_Buffer &		operator +=		(int    Value)				{	Add_Value(Value);	return( *this );	}
	CSG_Buffer &		operator +=		(float  Value)				{	Add_Value(Value);	return( *this );	}
	CSG_Buffer &		operator +=		(double Value)				{	Add_Value(Value);	return( *this );	}

	void				Set_Value		(int Offset, char   Value, bool bBigEndian = false)	{	m_Data[Offset]	= Value;	}
	void				Set_Value		(int Offset, short  Value, bool bBigEndian = false)	{	if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); *(short  *)(m_Data + Offset) = Value;	}
	void				Set_Value		(int Offset, int    Value, bool bBigEndian = false)	{	if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); *(int    *)(m_Data + Offset) = Value;	}
	void				Set_Value		(int Offset, float  Value, bool bBigEndian = false)	{	if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); *(float  *)(m_Data + Offset) = Value;	}
	void				Set_Value		(int Offset, double Value, bool bBigEndian = false)	{	if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); *(double *)(m_Data + Offset) = Value;	}

	short				asShort			(int Offset, bool bBigEndian = false) const	{	short  Value = *(short  *)(m_Data + Offset); if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); return( Value );	}
	int					asInt			(int Offset, bool bBigEndian = false) const	{	int    Value = *(int    *)(m_Data + Offset); if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); return( Value );	}
	float				asFloat			(int Offset, bool bBigEndian = false) const	{	float  Value = *(float  *)(m_Data + Offset); if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); return( Value );	}
	double				asDouble		(int Offset, bool bBigEndian = false) const	{	double Value = *(double *)(m_Data + Offset); if( bBigEndian ) SG_Swap_Bytes(&Value, sizeof(Value)); return( Value );	}


private:

	char				*m_Data;

	size_t				m_Size;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum class TSG_Array_Growth
{
	SG_ARRAY_GROWTH_0 = 0,
	SG_ARRAY_GROWTH_1,
	SG_ARRAY_GROWTH_2,
	SG_ARRAY_GROWTH_3,
	SG_ARRAY_GROWTH_FIX_8,
	SG_ARRAY_GROWTH_FIX_16,
	SG_ARRAY_GROWTH_FIX_32,
	SG_ARRAY_GROWTH_FIX_64,
	SG_ARRAY_GROWTH_FIX_128,
	SG_ARRAY_GROWTH_FIX_256,
	SG_ARRAY_GROWTH_FIX_512,
	SG_ARRAY_GROWTH_FIX_1024
};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Array
{
public:
	CSG_Array(void);
	~CSG_Array(void);

	CSG_Array							(const CSG_Array &Array);
	void *				Create			(const CSG_Array &Array);

	CSG_Array							(size_t Value_Size, sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0);
	void *				Create			(size_t Value_Size, sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0);

	bool				Destroy			(void);

	CSG_Array &			operator =		(const CSG_Array &Array)    {	Create(Array); return( *this );	}

	bool				Set_Growth		(TSG_Array_Growth Growth);
	TSG_Array_Growth	Get_Growth		(void)             const    {	return( m_Growth );		}

	size_t				Get_Value_Size	(void)             const    {	return( m_Value_Size      );	}
	sLong				Get_Size		(void)             const    {	return( m_nValues         );	}
	size_t				Get_uSize		(void)             const    {	return( (size_t)m_nValues );	}

	/// Returns a pointer to the memory address of the requested variable. You have to type cast and dereference the pointer to get access to the variable itself.
	void *				Get_Entry		(sLong  Index)     const	{	return( Index >= 0 && Index < m_nValues ? (char *)m_Values + Index * m_Value_Size : NULL );	}
	void *				operator []		(sLong  Index)     const    {	return( Get_Entry(Index) );	}

	bool				Del_Entry		(sLong Index, bool bShrink = true);

	void *				Get_Array		(void)             const    {	return( m_Values );		}
	void *				Get_Array		(sLong nValues)             {	Set_Array(nValues);	return( m_Values );	}

	bool				Set_Array		(sLong nValues, bool bShrink = true);
	bool				Set_Array		(sLong nValues, void **pArray, bool bShrink = true);

	bool				Inc_Array		(sLong nValues = 1);
	bool				Inc_Array		(void **pArray);

	bool				Dec_Array		(bool bShrink = true);
	bool				Dec_Array		(void **pArray, bool bShrink = true);


private:

	TSG_Array_Growth	m_Growth;

	size_t				m_Value_Size;

	sLong				m_nValues, m_nBuffer;

	void				*m_Values;

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Array_Pointer
{
public:
	CSG_Array_Pointer					(const CSG_Array_Pointer &Array)        {	Create(Array);	}
	void **				Create			(const CSG_Array_Pointer &Array);

	CSG_Array_Pointer					(sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0)	{	Create(nValues, Growth);	}
	void **				Create			(sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0);

	void				Destroy			(void)                                  {	m_Array.Destroy();	}

	bool				Set_Growth		(TSG_Array_Growth Growth)               {	return( m_Array.Set_Growth(Growth) );	}
	TSG_Array_Growth	Get_Growth		(void)                         const    {	return( m_Array.Get_Growth()       );	}

	sLong				Get_Size		(void)                         const    {	return( m_Array.Get_Size() );	}
	size_t				Get_uSize		(void)                         const    {	return( m_Array.Get_uSize() );	}

	void **				Get_Array		(void)                         const    {	return( (void **)m_Array.Get_Array()        );	}
	void **				Get_Array		(sLong nValues)                         {	return( (void **)m_Array.Get_Array(nValues) );	}

	bool				Set_Array		(sLong nValues, bool bShrink = true)    {	return( m_Array.Set_Array(nValues, bShrink) );	}
	bool				Inc_Array		(sLong nValues = 1)                     {	return( m_Array.Inc_Array(nValues) );	}
	bool				Dec_Array		(               bool bShrink = true)    {	return( m_Array.Dec_Array(bShrink) );	}

	bool				Add				(void *Value);
	bool				Add				(const CSG_Array_Pointer &Array);
	bool				Set				(sLong Index, void *Value)              {	if( !m_Array.Get_Entry(Index) ) return( false ); *((void **)m_Array.Get_Entry(Index)) = Value; return( true );	}
	void *&				Get				(sLong Index)                           {	return( *((void **)m_Array.Get_Entry(Index)) );	}
	void *				Get				(sLong Index)                  const    {	return( *((void **)m_Array.Get_Entry(Index)) );	}
	bool				Del				(sLong Index);
	sLong				Del				(void *Value);

	CSG_Array_Pointer &	operator =		(const CSG_Array_Pointer &Array)		{	Create(Array);	return( *this );	}

	void *&				operator []		(sLong Index)                           {	return( Get(Index) );	}
	void *				operator []		(sLong Index)                  const    {	return( Get(Index) );	}

	CSG_Array_Pointer &	operator +=		(void *Value)                           {	Add(Value);	return( *this );	}
	CSG_Array_Pointer &	operator +=		(const CSG_Array_Pointer &Array)        {	Add(Array);	return( *this );	}


private:

	CSG_Array			m_Array;

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Array_Int
{
public:
	CSG_Array_Int						(const CSG_Array_Int &Array)            {	Create(Array);	}
	int *				Create			(const CSG_Array_Int &Array);

	CSG_Array_Int						(sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0)	{	Create(nValues, Growth);	}
	int *				Create			(sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0);

	void				Destroy			(void)                                  {	m_Array.Destroy();	}

	bool				Set_Growth		(TSG_Array_Growth Growth)               {	return( m_Array.Set_Growth(Growth) );	}
	TSG_Array_Growth	Get_Growth		(void)                         const    {	return( m_Array.Get_Growth()       );	}

	sLong				Get_Size		(void)                         const    {	return( m_Array.Get_Size() );	}
	size_t				Get_uSize		(void)                         const    {	return( m_Array.Get_uSize() );	}

	int *				Get_Array		(void)                         const    {	return( (int *)m_Array.Get_Array()        );	}
	int *				Get_Array		(sLong nValues)                         {	return( (int *)m_Array.Get_Array(nValues) );	}

	bool				Set_Array		(sLong nValues, bool bShrink = true)    {	return( m_Array.Set_Array(nValues, bShrink) );	}
	bool				Inc_Array		(sLong nValues = 1)                     {	return( m_Array.Inc_Array(nValues) );	}
	bool				Dec_Array		(               bool bShrink = true)    {	return( m_Array.Dec_Array(bShrink) );	}

	bool				Add				(int Value);
	bool				Add				(const CSG_Array_Int &Array);
	bool				Set				(sLong Index, int Value)                {	if( !m_Array.Get_Entry(Index) ) return( false ); *((int *)m_Array.Get_Entry(Index)) = Value; return( true );	}
	int &				Get				(sLong Index)                           {	return( *((int *)m_Array.Get_Entry(Index)) );	}
	int	 				Get				(sLong Index)                  const    {	return( *((int *)m_Array.Get_Entry(Index)) );	}

	bool				Assign			(int Value);

	CSG_Array_Int &		operator =		(const CSG_Array_Int &Array)            {	Create(Array);	return( *this );	}
	CSG_Array_Int &		operator =		(int Value)                             {	Assign(Value);	return( *this );	}

	int &				operator []		(sLong Index)                           {	return( Get(Index) );	}
	int  				operator []		(sLong Index)                  const    {	return( Get(Index) );	}

	CSG_Array_Int &		operator +=		(int Value)                             {	Add(Value);	return( *this );	}
	CSG_Array_Int &		operator +=		(const CSG_Array_Int &Array)            {	Add(Array);	return( *this );	}


private:

	CSG_Array			m_Array;

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Array_sLong
{
public:
	CSG_Array_sLong						(const CSG_Array_sLong &Array)          {	Create(Array);	}
	sLong *				Create			(const CSG_Array_sLong &Array);

	CSG_Array_sLong						(sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0)	{	Create(nValues, Growth);	}
	sLong *				Create			(sLong nValues = 0, TSG_Array_Growth Growth = TSG_Array_Growth::SG_ARRAY_GROWTH_0);

	void				Destroy			(void)                                  {	m_Array.Destroy();	}

	bool				Set_Growth		(TSG_Array_Growth Growth)               {	return( m_Array.Set_Growth(Growth) );	}
	TSG_Array_Growth	Get_Growth		(void)                         const    {	return( m_Array.Get_Growth()       );	}

	sLong				Get_Size		(void)                         const    {	return( m_Array.Get_Size() );	}
	size_t				Get_uSize		(void)                         const    {	return( m_Array.Get_uSize() );	}

	sLong *				Get_Array		(void)                         const    {	return( (sLong *)m_Array.Get_Array()        );	}
	sLong *				Get_Array		(sLong nValues)                         {	return( (sLong *)m_Array.Get_Array(nValues) );	}

	bool				Set_Array		(sLong nValues, bool bShrink = true)    {	return( m_Array.Set_Array(nValues, bShrink) );	}
	bool				Inc_Array		(sLong nValues = 1)                     {	return( m_Array.Inc_Array(nValues) );	}
	bool				Dec_Array		(               bool bShrink = true)    {	return( m_Array.Dec_Array(bShrink) );	}

	bool				Add				(sLong Value);
	bool				Add				(const CSG_Array_sLong &Array);
	bool				Set				(sLong Index, sLong Value)              {	if( !m_Array.Get_Entry(Index) ) return( false ); *((sLong *)m_Array.Get_Entry(Index)) = Value; return( true );	}
	sLong &				Get				(sLong Index)                           {	return( *((sLong *)m_Array.Get_Entry(Index)) );	}
	sLong	 			Get				(sLong Index)                  const    {	return( *((sLong *)m_Array.Get_Entry(Index)) );	}

	bool				Assign			(sLong Value);

	CSG_Array_sLong &	operator =		(const CSG_Array_sLong &Array)          {	Create(Array);	return( *this );	}
	CSG_Array_sLong &	operator =		(sLong Value)                           {	Assign(Value);	return( *this );	}

	sLong &				operator []		(sLong Index)                           {	return( Get(Index) );	}
	sLong  				operator []		(sLong Index)                  const    {	return( Get(Index) );	}

	CSG_Array_sLong &	operator +=		(sLong Value)                           {	Add(Value);	return( *this );	}
	CSG_Array_sLong &	operator +=		(const CSG_Array_sLong &Array)          {	Add(Array);	return( *this );	}


private:

	CSG_Array			m_Array;

};


///////////////////////////////////////////////////////////
//														 //
//						String							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SG_Char				wchar_t
#define SG_T(s)				L ## s
#define SG_PRINTF			wprintf
#define SG_FPRINTF			fwprintf
#define SG_SSCANF			swscanf
#define SG_STR_CPY			wcscpy
#define SG_STR_LEN			wcslen
#define SG_STR_TOD			wcstod
#define SG_STR_CMP(s1, s2)	CSG_String(s1).Cmp(s2)
#define SG_STR_MBTOSG(s)	CSG_String(s).w_str()

//---------------------------------------------------------
typedef enum
{
	SG_FILE_ENCODING_ANSI,
	SG_FILE_ENCODING_UTF7,
	SG_FILE_ENCODING_UTF8,
	SG_FILE_ENCODING_UTF16LE,
	SG_FILE_ENCODING_UTF16BE,
	SG_FILE_ENCODING_UTF32LE,
	SG_FILE_ENCODING_UTF32BE,
	SG_FILE_ENCODING_UNDEFINED
}
TSG_File_Flags_Encoding;

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_String
{
public:
	CSG_String(void);
	CSG_String(const CSG_String &String);
	CSG_String(const char       *String);
	CSG_String(const wchar_t    *String);
	CSG_String(char    Character, size_t nRepeat = 1);
	CSG_String(wchar_t Character, size_t nRepeat = 1);

									CSG_String			(const class wxString *pString);
	bool							Create				(const class wxString *pString);

	virtual ~CSG_String(void);

	CSG_String &					operator =			(const CSG_String &String);
	CSG_String &					operator =			(const char       *String);
	CSG_String &					operator =			(const wchar_t    *String);
	CSG_String &					operator =			(char    Character);
	CSG_String &					operator =			(wchar_t Character);

	size_t							Length				(void)	const;

	bool							is_Empty			(void)	const;

	SG_Char							operator []			(int    i)	const;
	SG_Char							operator []			(size_t i)	const;
	SG_Char							Get_Char			(size_t i)	const;

	void							Set_Char			(size_t i, char    Character);
	void							Set_Char			(size_t i, wchar_t Character);

	const char *					b_str				(void)	const;
	const wchar_t *					w_str				(void)	const;
	const SG_Char *					c_str				(void)	const;

	operator const char *								(void)	const	{	return( b_str() );	}
	operator const wchar_t *							(void)	const	{	return( w_str() );	}

	CSG_String &					Prepend				(const CSG_String &String);

	CSG_String &					Append				(const CSG_String &String);
	CSG_String &					Append				(const char       *String);
	CSG_String &					Append				(const wchar_t    *String);
	CSG_String &					Append				(char    Character, size_t nRepeat = 1);
	CSG_String &					Append				(wchar_t Character, size_t nRepeat = 1);

	void							operator +=			(const CSG_String &String);
	void							operator +=			(const char       *String);
	void							operator +=			(const wchar_t    *String);
	void							operator +=			(char    Character);
	void							operator +=			(wchar_t Character);

	CSG_String						operator +			(const CSG_String &String)	const;
	CSG_String						operator +			(const char       *String)	const;
	CSG_String						operator +			(const wchar_t    *String)	const;
	CSG_String						operator +			(char    Character)			const;
	CSG_String						operator +			(wchar_t Character)			const;

	int								Cmp					(const CSG_String &String)	const;
	int								CmpNoCase			(const CSG_String &String)	const;

	bool							is_Same_As			(const CSG_String &String, bool bCase = true)	const;
	bool							is_Same_As			(const char     Character, bool bCase = true)	const;
	bool							is_Same_As			(const wchar_t  Character, bool bCase = true)	const;

	CSG_String &					Make_Lower			(void);
	CSG_String &					Make_Upper			(void);

	void							Clear				(void);

	static CSG_String				Format				(const char    *Format, ...);
	static CSG_String				Format				(const wchar_t *Format, ...);
	int								Printf				(const char    *Format, ...);
	int								Printf				(const wchar_t *Format, ...);

	size_t							Replace				(const CSG_String &sOld, const CSG_String &sNew, bool bReplaceAll = true);

	CSG_String &					Remove				(size_t pos);
	CSG_String &					Remove				(size_t pos, size_t len);

	int								Trim				(bool fromRight = false);
	int								Trim_Both			(void);

	int								Find				(char    Character, bool fromEnd = false)	const;
	int								Find				(wchar_t Character, bool fromEnd = false)	const;
	int								Find				(const CSG_String &String)				const;
	bool							Contains			(const CSG_String &String)				const;

	CSG_String						AfterFirst			(char    Character)						const;
	CSG_String						AfterFirst			(wchar_t Character)						const;
	CSG_String						AfterLast			(char    Character)						const;
	CSG_String						AfterLast			(wchar_t Character)						const;
	CSG_String						BeforeFirst			(char    Character)						const;
	CSG_String						BeforeFirst			(wchar_t Character)						const;
	CSG_String						BeforeLast			(char    Character)						const;
	CSG_String						BeforeLast			(wchar_t Character)						const;

	CSG_String						Right				(size_t count)							const;
	CSG_String						Mid					(size_t first, size_t count = 0)		const;
	CSG_String						Left				(size_t count)							const;

	bool							is_Number			(void)	const;

	int								asInt				(void)									const;
	bool							asInt				(int    &Value)							const;
	sLong							asLongLong			(void)									const;
	bool							asLongLong			(sLong  &Value)							const;

	double							asDouble			(void)									const;
	bool							asDouble			(double &Value)							const;

	static CSG_String				from_UTF8			(const char *String, size_t Length = 0);

	size_t							to_UTF8				(char **pString)						const;
	CSG_Buffer						to_UTF8				(void)									const;
	size_t							to_MBChar			(char **pString, int Encoding)			const;
	CSG_Buffer						to_MBChar			(                int Encoding)			const;
	bool							to_ASCII			(char **pString, char Replace = '_')	const;
	CSG_Buffer						to_ASCII			(                char Replace = '_')	const;
    std::string						to_StdString		(void)                                  const;
    std::wstring					to_StdWstring		(void)                                  const;

protected:

	class wxString					*m_pString;

};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_String		operator +			(const char    *A, const CSG_String &B);
SAGA_API_DLL_EXPORT CSG_String		operator +			(const wchar_t *A, const CSG_String &B);
SAGA_API_DLL_EXPORT CSG_String		operator +			(char           A, const CSG_String &B);
SAGA_API_DLL_EXPORT CSG_String		operator +			(wchar_t        A, const CSG_String &B);

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Strings
{
public:
	CSG_Strings(void);
	CSG_Strings(const CSG_Strings &Strings);
	CSG_Strings(int nStrings, const SG_Char **Strings);

	virtual ~CSG_Strings(void);

	bool							Create				(const CSG_Strings &Strings);
	bool							Destroy				(void);

	bool							Set_Count			(int    Count)	{	return( Set_Count((size_t)Count) );	}
	bool							Set_Count			(size_t Count);
	int								Get_Count			(void)	const	{	return( (int)m_Strings.Get_Size() );	}
	size_t							Get_Size			(void)	const	{	return(      m_Strings.Get_Size() );	}

	bool							Add					(const CSG_Strings &Strings);
	bool							Add					(const CSG_String  &String );
	bool							Ins					(const CSG_String  &String , int    Index)	{	return( Ins(String, (size_t)Index) );	}
	bool							Ins					(const CSG_String  &String , size_t Index);
	bool							Del					(int    Index)	{	return( Del((size_t)Index) );	}
	bool							Del					(size_t Index);

	CSG_String &					Get_String			(int    Index) const	{	return( *((CSG_String *)m_Strings[Index]) );	}
	CSG_String &					Get_String			(size_t Index) const	{	return( *((CSG_String *)m_Strings[Index]) );	}

	CSG_String &					operator []			(int    Index) const	{	return( Get_String(Index) );	}
	CSG_String &					operator []			(size_t Index) const	{	return( Get_String(Index) );	}

	CSG_Strings &					operator  =			(const CSG_Strings &Strings)	{	Create(Strings); return( *this );	}
	CSG_Strings &					operator +=			(const CSG_Strings &Strings)	{	Add   (Strings); return( *this );	}
	CSG_Strings &					operator +=			(const CSG_String  &String )	{	Add   (String ); return( *this );	}

	bool							Sort				(bool Ascending = true);

	void							Clear				(void)	{	Destroy();	}
	bool							Assign				(const CSG_Strings &Strings)	{	return( Create(Strings) );	}


protected:

	CSG_Array_Pointer				m_Strings;

};

//---------------------------------------------------------
#define SG_DEFAULT_DELIMITERS		" \t\r\n"

typedef enum
{
	SG_TOKEN_INVALID,
	SG_TOKEN_DEFAULT,
	SG_TOKEN_RET_EMPTY,
	SG_TOKEN_RET_EMPTY_ALL,
	SG_TOKEN_RET_DELIMS,
	SG_TOKEN_STRTOK
}
TSG_String_Tokenizer_Mode;

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_String_Tokenizer
{
public:
	CSG_String_Tokenizer(void);
	CSG_String_Tokenizer(const CSG_String &String, const CSG_String &Delimiters = SG_DEFAULT_DELIMITERS, TSG_String_Tokenizer_Mode Mode = SG_TOKEN_DEFAULT);

	~CSG_String_Tokenizer(void);

  
	size_t							Get_Tokens_Count	(void)	const;
	SG_Char							Get_Last_Delimiter	(void)	const;
	CSG_String						Get_Next_Token		(void);
	size_t							Get_Position		(void)	const;
	CSG_String						Get_String			(void)	const;
	bool							Has_More_Tokens		(void)	const;
	void							Set_String			(const CSG_String &String, const CSG_String &Delimiters = SG_DEFAULT_DELIMITERS, TSG_String_Tokenizer_Mode Mode = SG_TOKEN_DEFAULT);


private:

	class wxStringTokenizer			*m_pTokenizer;

};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_Strings		SG_String_Tokenize				(const CSG_String &String, const CSG_String &Delimiters = SG_DEFAULT_DELIMITERS, TSG_String_Tokenizer_Mode Mode = SG_TOKEN_DEFAULT);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool			SG_is_Character_Numeric			(int Character);

SAGA_API_DLL_EXPORT CSG_String		SG_Get_CurrentTimeStr			(bool bWithDate = true);

SAGA_API_DLL_EXPORT double			SG_Degree_To_Double				(const CSG_String &String);
SAGA_API_DLL_EXPORT CSG_String		SG_Double_To_Degree				(double Value);

SAGA_API_DLL_EXPORT int				SG_Get_Significant_Decimals		(double Value, int maxDecimals = 6);

SAGA_API_DLL_EXPORT void			SG_Flip_Decimal_Separators		(CSG_String &String);

SAGA_API_DLL_EXPORT CSG_String		SG_Get_String					(double Value, int Precision = -99);
SAGA_API_DLL_EXPORT CSG_String		SG_Get_String					(int    Value, int Precision = 0);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Bytes
{
public:
							CSG_Bytes		(void);
	bool					Create			(void);

							CSG_Bytes		(const CSG_Bytes &Bytes);
	bool					Create			(const CSG_Bytes &Bytes);

							CSG_Bytes		(const BYTE *Bytes, int nBytes);
	bool					Create			(const BYTE *Bytes, int nBytes);

	virtual ~CSG_Bytes(void);

	bool					Destroy			(void);
	bool					Clear			(void);

	void					Rewind			(void)				{	m_Cursor	= 0;	}
	bool					is_EOF			(void)				{	return( m_Cursor >= m_nBytes );	}

	int						Get_Count		(void)		const	{	return( m_nBytes );	}
	BYTE *					Get_Bytes		(void)		const	{	return( m_Bytes );	}

	CSG_Bytes				Get_Bytes		(int i)		const	{	if( i < 0 || i >= m_nBytes ) i = 0; return( CSG_Bytes(m_Bytes + i, m_nBytes - i) );	}
	CSG_Bytes				operator +		(int i)		const	{	return( Get_Bytes(i) );	}

	BYTE					Get_Byte		(int i)		const	{	return( i >= 0 && i < m_nBytes ? m_Bytes[i]  : 0 );		}
	BYTE					operator []		(int i)		const	{	return( Get_Byte(i) );	}

	bool					Assign			(const CSG_Bytes &Bytes);
	CSG_Bytes &				operator =		(const CSG_Bytes &Bytes)	{	Assign(Bytes);	return( *this );	}

	bool					Add				(const CSG_Bytes &Bytes);
	CSG_Bytes &				operator +=		(const CSG_Bytes &Bytes)	{	Add(Bytes);		return( *this );	}

	bool					Add				(void *Bytes, int nBytes, bool bSwapBytes);

	bool					Add				(BYTE   Value)							{	return( Add(&Value, sizeof(Value), false) );	}
	CSG_Bytes &				operator +=		(BYTE   Value)							{	Add(Value);		return( *this );	}
	bool					Add				(char   Value)							{	return( Add(&Value, sizeof(Value), false) );	}
	CSG_Bytes &				operator +=		(char   Value)							{	Add(Value);		return( *this );	}
	bool					Add				(short  Value, bool bSwapBytes = false)	{	return( Add(&Value, sizeof(Value), bSwapBytes) );	}
	CSG_Bytes &				operator +=		(short  Value)							{	Add(Value);		return( *this );	}
	bool					Add				(WORD   Value, bool bSwapBytes = false)	{	return( Add(&Value, sizeof(Value), bSwapBytes) );	}
	CSG_Bytes &				operator +=		(WORD   Value)							{	Add(Value);		return( *this );	}
	bool					Add				(int    Value, bool bSwapBytes = false)	{	return( Add(&Value, sizeof(Value), bSwapBytes) );	}
	CSG_Bytes &				operator +=		(int    Value)							{	Add(Value);		return( *this );	}
	bool					Add				(DWORD  Value, bool bSwapBytes = false)	{	return( Add(&Value, sizeof(Value), bSwapBytes) );	}
	CSG_Bytes &				operator +=		(DWORD  Value)							{	Add(Value);		return( *this );	}
	bool					Add				(float  Value, bool bSwapBytes = false)	{	return( Add(&Value, sizeof(Value), bSwapBytes) );	}
	CSG_Bytes &				operator +=		(float  Value)							{	Add(Value);		return( *this );	}
	bool					Add				(double Value, bool bSwapBytes = false)	{	return( Add(&Value, sizeof(Value), bSwapBytes) );	}
	CSG_Bytes &				operator +=		(double Value)							{	Add(Value);		return( *this );	}

	BYTE					asByte			(int i)								const	{	return(       Get_Byte (i) );	}
	char					asChar			(int i)								const	{	return( (char)Get_Byte (i) );	}
	short					asShort			(int i, bool bSwapBytes = false)	const	{	short	v = *((short  *)(m_Bytes + i)); if( bSwapBytes ) SG_Swap_Bytes(&v, sizeof(v)); return( v );	}
	WORD					asWord			(int i, bool bSwapBytes = false)	const	{	WORD	v = *((WORD   *)(m_Bytes + i)); if( bSwapBytes ) SG_Swap_Bytes(&v, sizeof(v)); return( v );	}
	int						asInt			(int i, bool bSwapBytes = false)	const	{	int		v = *((int    *)(m_Bytes + i)); if( bSwapBytes ) SG_Swap_Bytes(&v, sizeof(v)); return( v );	}
	DWORD					asDWord			(int i, bool bSwapBytes = false)	const	{	DWORD	v = *((DWORD  *)(m_Bytes + i)); if( bSwapBytes ) SG_Swap_Bytes(&v, sizeof(v)); return( v );	}
	float					asFloat			(int i, bool bSwapBytes = false)	const	{	float	v = *((float  *)(m_Bytes + i)); if( bSwapBytes ) SG_Swap_Bytes(&v, sizeof(v)); return( v );	}
	double					asDouble		(int i, bool bSwapBytes = false)	const	{	double	v = *((double *)(m_Bytes + i)); if( bSwapBytes ) SG_Swap_Bytes(&v, sizeof(v)); return( v );	}

	BYTE					Read_Byte		(void)						{	BYTE	v = asByte  (m_Cursor);             m_Cursor += sizeof(v); return( v ); }
	char					Read_Char		(void)						{	char	v = asByte  (m_Cursor);             m_Cursor += sizeof(v); return( v ); }
	short					Read_Short		(bool bSwapBytes = false)	{	short	v = asShort (m_Cursor, bSwapBytes); m_Cursor += sizeof(v); return( v ); }
	WORD					Read_Word		(bool bSwapBytes = false)	{	WORD	v = asWord  (m_Cursor, bSwapBytes); m_Cursor += sizeof(v); return( v ); }
	int						Read_Int		(bool bSwapBytes = false)	{	int		v = asInt   (m_Cursor, bSwapBytes); m_Cursor += sizeof(v); return( v ); }
	DWORD					Read_DWord		(bool bSwapBytes = false)	{	DWORD	v = asDWord (m_Cursor, bSwapBytes); m_Cursor += sizeof(v); return( v ); }
	float					Read_Float		(bool bSwapBytes = false)	{	float	v = asFloat (m_Cursor, bSwapBytes); m_Cursor += sizeof(v); return( v ); }
	double					Read_Double		(bool bSwapBytes = false)	{	double	v = asDouble(m_Cursor, bSwapBytes); m_Cursor += sizeof(v); return( v ); }

	CSG_String				toHexString		(void)	const;
	bool					fromHexString	(const CSG_String &HexString);


private:

	int						m_nBytes, m_nBuffer, m_Cursor;

	BYTE					*m_Bytes;


	bool					_Inc_Array		(int nBytes);

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Bytes_Array
{
public:
	CSG_Bytes_Array(void);
	virtual ~CSG_Bytes_Array(void);

	bool					Destroy			(void);

	int						Get_Count		(void)	const	{	return( m_nBytes );		}
	CSG_Bytes *				Get_Bytes		(int i)			{	return( i >= 0 && i < m_nBytes ? m_pBytes[i] : NULL );	}
	CSG_Bytes &				operator []		(int i)			{	return( *Get_Bytes(i) );	}

	CSG_Bytes *				Add				(void);


private:

	int						m_nBytes, m_nBuffer;

	CSG_Bytes				**m_pBytes;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CSG_Stack
{
public:
	CSG_Stack(size_t RecordSize)
	{
		m_Stack.Create(RecordSize, 1, TSG_Array_Growth::SG_ARRAY_GROWTH_FIX_256);
	}

	virtual ~CSG_Stack(void) {}

	bool					Destroy			(void)       { return( m_Stack.Set_Array(1) ); }

	size_t					Get_RecordSize	(void) const { return( m_Stack.Get_Value_Size() ); }
	size_t					Get_Size		(void) const { return( m_Stack.Get_uSize() - 1 ); }

	bool					Clear			(bool bFreeMemory = false)
	{
		return( m_Stack.Set_Array(1, bFreeMemory) );
	}


protected:

	void *					Get_Record		(size_t i) const
	{
		return( m_Stack.Get_Entry((sLong)(i + 1)) );
	}

	void *					Get_Record_Push	(void)
	{
		return( m_Stack.Inc_Array() ? m_Stack.Get_Entry(m_Stack.Get_Size() - 1) : NULL );
	}

	void *					Get_Record_Pop	(void)
	{
		if( m_Stack.Get_Size() > 0 )
		{
			memcpy(m_Stack.Get_Array(), m_Stack.Get_Entry(m_Stack.Get_Size() - 1), m_Stack.Get_Value_Size());

			m_Stack.Dec_Array(true);

			return( m_Stack.Get_Array() );
		}

		return( NULL );
	}


private:

	CSG_Array				m_Stack;

};


///////////////////////////////////////////////////////////
//														 //
//						Data Types						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	SG_DATATYPE_Bit = 0,
	SG_DATATYPE_Byte,
	SG_DATATYPE_Char,
	SG_DATATYPE_Word,
	SG_DATATYPE_Short,
	SG_DATATYPE_DWord,
	SG_DATATYPE_Int,
	SG_DATATYPE_ULong,
	SG_DATATYPE_Long,
	SG_DATATYPE_Float,
	SG_DATATYPE_Double,
	SG_DATATYPE_String,
	SG_DATATYPE_Date,
	SG_DATATYPE_Color,
	SG_DATATYPE_Binary,
	SG_DATATYPE_Undefined
}
TSG_Data_Type;

//---------------------------------------------------------
#define SG_DATATYPES_Undefined 0x0000
#define SG_DATATYPES_Bit       0x0001
#define SG_DATATYPES_Byte      0x0002
#define SG_DATATYPES_Char      0x0004
#define SG_DATATYPES_Word      0x0008
#define SG_DATATYPES_Short     0x0010
#define SG_DATATYPES_DWord     0x0020
#define SG_DATATYPES_Int       0x0040
#define SG_DATATYPES_ULong     0x0080
#define SG_DATATYPES_Long      0x0100
#define SG_DATATYPES_Float     0x0200
#define SG_DATATYPES_Double    0x0400
#define SG_DATATYPES_String    0x0800
#define SG_DATATYPES_Date      0x1000
#define SG_DATATYPES_Color     0x2000
#define SG_DATATYPES_Binary    0x4000
#define SG_DATATYPES_Standard  0xFFFF // all flags set
#define SG_DATATYPES_SInteger  (SG_DATATYPES_Char|SG_DATATYPES_Short|SG_DATATYPES_Int  |SG_DATATYPES_Long)
#define SG_DATATYPES_UInteger  (SG_DATATYPES_Byte|SG_DATATYPES_Word |SG_DATATYPES_DWord|SG_DATATYPES_ULong)
#define SG_DATATYPES_Integer   (SG_DATATYPES_SInteger|SG_DATATYPES_UInteger)
#define SG_DATATYPES_Real      (SG_DATATYPES_Float|SG_DATATYPES_Double)
#define SG_DATATYPES_Numeric   (SG_DATATYPES_Integer|SG_DATATYPES_Real)
#define SG_DATATYPES_Table     (SG_DATATYPES_String|SG_DATATYPES_Date|SG_DATATYPES_Color|SG_DATATYPES_Numeric|SG_DATATYPES_Binary)

//---------------------------------------------------------
const char	gSG_Data_Type_Identifier[][32]	=
{
	"BIT",
	"BYTE_UNSIGNED",
	"BYTE",
	"SHORTINT_UNSIGNED",
	"SHORTINT",
	"INTEGER_UNSIGNED",
	"INTEGER",
	"LONGINT_UNSIGNED",
	"LONGINT",
	"FLOAT",
	"DOUBLE",
	"STRING",
	"DATE",
	"COLOR",
	"BINARY",
	"UNDEFINED"
};

//---------------------------------------------------------
inline size_t	SG_Data_Type_Get_Size	(TSG_Data_Type Type)
{
	switch( Type )
	{
	case SG_DATATYPE_Bit   : return( 0 );
	case SG_DATATYPE_Byte  : return( sizeof(unsigned char) );
	case SG_DATATYPE_Char  : return( sizeof(char) );
	case SG_DATATYPE_Word  : return( sizeof(unsigned short int) );
	case SG_DATATYPE_Short : return( sizeof(short int) );
	case SG_DATATYPE_DWord : return( sizeof(unsigned int) );
	case SG_DATATYPE_Int   : return( sizeof(int) );
	case SG_DATATYPE_ULong : return( sizeof(uLong) );
	case SG_DATATYPE_Long  : return( sizeof(sLong) );
	case SG_DATATYPE_Float : return( sizeof(float) );
	case SG_DATATYPE_Double: return( sizeof(double) );
	case SG_DATATYPE_String: return( 0 );
	case SG_DATATYPE_Date  : return( 0 );
	case SG_DATATYPE_Color : return( sizeof(unsigned int) );
	case SG_DATATYPE_Binary: return( 0 );
	default                : return( 0 );
	}
}

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_String		SG_Data_Type_Get_Name		(TSG_Data_Type Type, bool bShort = false);
SAGA_API_DLL_EXPORT CSG_String		SG_Data_Type_Get_Identifier	(TSG_Data_Type Type);
SAGA_API_DLL_EXPORT TSG_Data_Type	SG_Data_Type_Get_Type		(const CSG_String &Identifier);
SAGA_API_DLL_EXPORT int				SG_Data_Type_Get_Flag		(TSG_Data_Type Type);
SAGA_API_DLL_EXPORT bool			SG_Data_Type_is_Numeric		(TSG_Data_Type Type);
SAGA_API_DLL_EXPORT bool			SG_Data_Type_Range_Check	(TSG_Data_Type Type, double &Value);


///////////////////////////////////////////////////////////
//														 //
//						File							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	SG_FILE_TYPE_NORMAL,
	SG_FILE_TYPE_ZIP
}
TSG_File_Type;

//---------------------------------------------------------
enum ESG_File_Flags_Open
{
	SG_FILE_R,
	SG_FILE_W,
	SG_FILE_RW
};

//---------------------------------------------------------
enum ESG_File_Flags_Seek
{
	SG_FILE_START,
	SG_FILE_CURRENT,
	SG_FILE_END
};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_File
{
public:

	CSG_File(void);
	virtual ~CSG_File(void);

									CSG_File			(const CSG_String &FileName, int Mode = SG_FILE_R, bool bBinary = true, int Encoding = SG_FILE_ENCODING_ANSI);
	virtual bool					Open				(const CSG_String &FileName, int Mode = SG_FILE_R, bool bBinary = true, int Encoding = SG_FILE_ENCODING_ANSI);

	virtual bool					Close				(void);

	virtual const CSG_String &		Get_File_Name		(void)	const	{	return( m_FileName );	}
	virtual TSG_File_Type			Get_File_Type		(void)	const	{	return( SG_FILE_TYPE_NORMAL );	}

	class wxStreamBase *			Get_Stream			(void)	const	{	return( (class wxStreamBase *)m_pStream );	}

	bool							Set_Encoding		(int Encoding);
	int								Get_Encoding		(void)	const	{	return( m_Encoding );	}

	bool							is_Open				(void)	const	{	return( m_pStream != NULL );	}
	bool							is_Reading			(void)	const	{	return( m_pStream != NULL && m_Mode != SG_FILE_W );	}
	bool							is_Writing			(void)	const	{	return( m_pStream != NULL && m_Mode != SG_FILE_R );	}
	bool							is_EOF				(void)	const;

	sLong							Length				(void)	const;

	bool							Seek				(sLong Offset, int Origin = SG_FILE_START)	const;
	bool							Seek_Start			(void)	const;
	bool							Seek_End			(void)	const;

	sLong							Tell				(void)	const;

	int								Printf				(const char    *Format, ...);
	int								Printf				(const wchar_t *Format, ...);

	size_t							Read				(void             *Buffer, size_t Size, size_t Count = 1)	const;
	size_t							Write				(void             *Buffer, size_t Size, size_t Count = 1)	const;
	size_t							Read				(      CSG_String &Buffer, size_t Size)	const;
	size_t							Write				(const CSG_String &Buffer)				const;

	bool							Read_Line			(CSG_String &sLine)	const;

	int								Read_Char			(void)	const;
	int								Read_Int			(				bool bBigEndian = false)	const;
	bool							Write_Int			(int    Value,	bool bBigEndian = false);
	double							Read_Double			(				bool bBigEndian = false)	const;
	bool							Write_Double		(double Value,	bool bBigEndian = false);

	bool							Scan				(int        &Value)	const;
	bool							Scan				(double     &Value)	const;
	bool							Scan				(CSG_String &Value, SG_Char Separator)	const;

	int								Scan_Int			(void)				const;
	double							Scan_Double			(void)				const;
	CSG_String						Scan_String			(SG_Char Separator)	const;


protected:

	int								m_Mode, m_Encoding;

	CSG_String						m_FileName;

	void							*m_pStream, *m_pConvert;


	void							On_Construction		(void);

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_File_Zip : public CSG_File
{
public:

	CSG_File_Zip(void);
	virtual ~CSG_File_Zip(void);

									CSG_File_Zip		(const CSG_String &FileName, int Mode = SG_FILE_R, int Encoding = SG_FILE_ENCODING_ANSI);
	virtual bool					Open				(const CSG_String &FileName, int Mode = SG_FILE_R, int Encoding = SG_FILE_ENCODING_ANSI);

	virtual bool					Close				(void);

	virtual TSG_File_Type			Get_File_Type		(void)	const	{	return( SG_FILE_TYPE_ZIP );	}

	bool							Add_Directory		(const CSG_String &Name);
	bool							Add_File			(const CSG_String &Name, bool bBinary = true);

	size_t							Get_File_Count		(void)	{	return( m_Files.Get_Size() );	}
	bool							Get_File			(const CSG_String &Name);
	bool							Get_File			(size_t Index);
	virtual CSG_String				Get_File_Name		(size_t Index);
	bool							is_Directory		(size_t Index);


protected:

	CSG_Array_Pointer				m_Files;

};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool			SG_Dir_Exists				(const CSG_String &Directory);
SAGA_API_DLL_EXPORT bool			SG_Dir_Create				(const CSG_String &Directory, bool bFullPath = false);
SAGA_API_DLL_EXPORT bool			SG_Dir_Delete				(const CSG_String &Directory, bool bRecursive = false);
SAGA_API_DLL_EXPORT CSG_String		SG_Dir_Get_Current			(void);
SAGA_API_DLL_EXPORT CSG_String		SG_Dir_Get_Temp				(void);
SAGA_API_DLL_EXPORT bool			SG_Dir_List_Subdirectories	(CSG_Strings &List, const CSG_String &Directory);
SAGA_API_DLL_EXPORT bool			SG_Dir_List_Files			(CSG_Strings &List, const CSG_String &Directory);
SAGA_API_DLL_EXPORT bool			SG_Dir_List_Files			(CSG_Strings &List, const CSG_String &Directory, const CSG_String &Extension);

SAGA_API_DLL_EXPORT bool			SG_File_Exists				(const CSG_String &FileName);
SAGA_API_DLL_EXPORT bool			SG_File_Delete				(const CSG_String &FileName);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Get_Name_Temp		(const CSG_String &Prefix);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Get_Name_Temp		(const CSG_String &Prefix, const CSG_String &Directory);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Get_Name			(const CSG_String &full_Path, bool bExtension);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Get_Path			(const CSG_String &full_Path);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Get_Path_Absolute	(const CSG_String &full_Path);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Get_Path_Relative	(const CSG_String &Directory, const CSG_String &full_Path);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Make_Path			(const CSG_String &Directory, const CSG_String &Name);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Make_Path			(const CSG_String &Directory, const CSG_String &Name, const CSG_String &Extension);
SAGA_API_DLL_EXPORT bool			SG_File_Cmp_Extension		(const CSG_String &FileName, const CSG_String &Extension);
SAGA_API_DLL_EXPORT CSG_String		SG_File_Get_Extension		(const CSG_String &FileName);
SAGA_API_DLL_EXPORT bool			SG_File_Set_Extension		(      CSG_String &FileName, const CSG_String &Extension);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool			SG_Get_Environment			(const CSG_String &Variable,       CSG_String *Value = NULL);
SAGA_API_DLL_EXPORT bool			SG_Set_Environment			(const CSG_String &Variable, const CSG_String &Value);


///////////////////////////////////////////////////////////
//														 //
//						Colors							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SG_GET_RGB( r, g, b   )	((DWORD) (((BYTE)(r) | ((WORD)(g) << 8)) | (((DWORD)(BYTE)(b)) << 16)))
#define SG_GET_RGBA(r, g, b, a)	((DWORD) (((BYTE)(r) | ((WORD)(g) << 8)) | (((DWORD)(BYTE)(b)) << 16) | (((DWORD)(BYTE)(a)) << 24)))

#define SG_GET_R(rgb)			((BYTE) ((rgb)      ))
#define SG_GET_G(rgb)			((BYTE) ((rgb) >>  8))
#define SG_GET_B(rgb)			((BYTE) ((rgb) >> 16))
#define SG_GET_A(rgb)			((BYTE) ((rgb) >> 24))

//---------------------------------------------------------
#define SG_COLOR_BLACK			SG_GET_RGB(  0,   0,   0)
#define SG_COLOR_GREY			SG_GET_RGB(128, 128, 128)
#define SG_COLOR_GREY_LIGHT		SG_GET_RGB(192, 192, 192)
#define SG_COLOR_WHITE			SG_GET_RGB(255, 255, 255)
#define SG_COLOR_RED			SG_GET_RGB(255,   0,   0)
#define SG_COLOR_RED_DARK		SG_GET_RGB(128,   0,   0)
#define SG_COLOR_YELLOW			SG_GET_RGB(255, 255,   0)
#define SG_COLOR_YELLOW_DARK	SG_GET_RGB(128, 128,   0)
#define SG_COLOR_GREEN			SG_GET_RGB(  0, 255,   0)
#define SG_COLOR_GREEN_DARK		SG_GET_RGB(  0, 128,   0)
#define SG_COLOR_GREEN_LIGHT	SG_GET_RGB(  0, 255,   0)
#define SG_COLOR_BLUE			SG_GET_RGB(  0,   0, 255)
#define SG_COLOR_BLUE_DARK		SG_GET_RGB(  0,   0, 128)
#define SG_COLOR_BLUE_LIGHT		SG_GET_RGB(  0, 255, 255)
#define SG_COLOR_BLUE_GREEN		SG_GET_RGB(  0, 128, 128)
#define SG_COLOR_PURPLE			SG_GET_RGB(128,   0, 128)
#define SG_COLOR_PINK			SG_GET_RGB(255,   0, 255)
#define SG_COLOR_NONE			-1
#define SG_COLOR_RANDOM			-2

//---------------------------------------------------------
enum ESG_Colors
{
	SG_COLORS_DEFAULT			= 0,
	SG_COLORS_DEFAULT_BRIGHT,
	SG_COLORS_BLACK_WHITE,
	SG_COLORS_BLACK_RED,
	SG_COLORS_BLACK_GREEN,
	SG_COLORS_BLACK_BLUE,
	SG_COLORS_WHITE_RED,
	SG_COLORS_WHITE_GREEN,
	SG_COLORS_WHITE_BLUE,
	SG_COLORS_YELLOW_RED,
	SG_COLORS_YELLOW_GREEN,
	SG_COLORS_YELLOW_BLUE,
	SG_COLORS_GREEN_RED,
	SG_COLORS_RED_GREEN,
	SG_COLORS_RED_BLUE,
	SG_COLORS_GREEN_BLUE,
	SG_COLORS_RED_GREY_BLUE,
	SG_COLORS_RED_GREY_GREEN,
	SG_COLORS_GREEN_GREY_BLUE,
	SG_COLORS_RED_GREEN_BLUE,
	SG_COLORS_RED_BLUE_GREEN,
	SG_COLORS_GREEN_RED_BLUE,
	SG_COLORS_RAINBOW,
	SG_COLORS_NEON,
	SG_COLORS_TOPOGRAPHY,
	SG_COLORS_TOPOGRAPHY_2,
	SG_COLORS_TOPOGRAPHY_3,
	SG_COLORS_PRECIPITATION,
	SG_COLORS_ASPECT_1,
	SG_COLORS_ASPECT_2,
	SG_COLORS_ASPECT_3,
	SG_COLORS_COUNT
};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_String		SG_Colors_Get_Name	(int Index);

SAGA_API_DLL_EXPORT long			SG_Color_Get_Random	(void);

SAGA_API_DLL_EXPORT bool			SG_Color_From_Text	(const CSG_String &Text, long &Color);
SAGA_API_DLL_EXPORT CSG_String		SG_Color_To_Text	(long Color, bool bHexadecimal = true);

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Colors
{
public:
									CSG_Colors			(void);
	bool							Create				(void);

									CSG_Colors			(const CSG_Colors &Colors);
	bool							Create				(const CSG_Colors &Colors);

									CSG_Colors			(int nColors, int Palette = SG_COLORS_DEFAULT, bool bRevert = false);
	bool							Create				(int nColors, int Palette = SG_COLORS_DEFAULT, bool bRevert = false);

	virtual ~CSG_Colors(void);

	void							Destroy				(void);

	bool							Set_Count			(int nColors);
	int								Get_Count			(void) const		{	return( m_nColors );	}

	CSG_Colors &					operator  =			(const CSG_Colors &Colors);
	long &							operator []			(int Index)			{	return( m_Colors[Index < 0 ? 0 : Index >= m_nColors ? m_nColors - 1 : Index] );	}

	bool							Set_Color			(int Index, long Color);
	bool							Set_Color			(int Index, int Red, int Green, int Blue);
	bool							Set_Red				(int Index, int Value);
	bool							Set_Green			(int Index, int Value);
	bool							Set_Blue			(int Index, int Value);
	bool							Set_Brightness		(int Index, int Value);

	long							Get_Color			(int Index)	const	{	return( m_nColors > 0 ? m_Colors[Index < 0 ? 0 : Index >= m_nColors ? m_nColors - 1 : Index] : 0 );	}
	long							Get_Red				(int Index)	const	{	return( SG_GET_R(Get_Color(Index)) );	}
	long							Get_Green			(int Index)	const	{	return( SG_GET_G(Get_Color(Index)) );	}
	long							Get_Blue			(int Index)	const	{	return( SG_GET_B(Get_Color(Index)) );	}
	long							Get_Brightness		(int Index)	const	{	return( (Get_Red(Index) + Get_Green(Index) + Get_Blue(Index)) / 3 );	}

	long							Get_Interpolated	(double Index)	const
	{
		if( m_nColors <= 0              ) return( 0 );
		if( Index     <= 0.             ) return( m_Colors[0] );
		if( Index     >= m_nColors - 1. ) return( m_Colors[m_nColors - 1] );

		int i = (int)Index; Index -= i;
		int r = (int)(Get_Red  (i) + Index * ((double)Get_Red  (i + 1) - Get_Red  (i)));
		int g = (int)(Get_Green(i) + Index * ((double)Get_Green(i + 1) - Get_Green(i)));
		int b = (int)(Get_Blue (i) + Index * ((double)Get_Blue (i + 1) - Get_Blue (i)));

		return( SG_GET_RGB(r, g, b) );
	}

	static int						Get_Predefined_Count(void);
	static CSG_String				Get_Predefined_Name	(int Identifier);
	bool							Set_Predefined		(int Index, bool bRevert = false, int nColors = 11);
	bool							Set_Palette			(int Index, bool bRevert = false, int nColors = 11)	{	return( Set_Predefined(Index, bRevert, nColors) );	}
	bool							Set_Default			(int nColors = 11);
	bool							Set_Ramp			(long Color_A, long Color_B);
	bool							Set_Ramp			(long Color_A, long Color_B, int iColor_A, int iColor_B);
	bool							Set_Ramp_Brighness	(int Brightness_A, int Brightness_B);
	bool							Set_Ramp_Brighness	(int Brightness_A, int Brightness_B, int iColor_A, int iColor_B);

	bool							Random				(void);
	bool							Invert				(void);
	bool							Revert				(void);
	bool							Greyscale			(void);

	bool							Assign				(const CSG_Colors &Colors);
	bool							Assign				(CSG_Colors *pSource);

	bool							Load				(const CSG_String &File_Name);
	bool							Save				(const CSG_String &File_Name, bool bBinary);

	bool							Serialize			(CSG_File &Stream, bool bSave, bool bBinary);

	bool							to_Text				(      CSG_String &String);
	bool							from_Text			(const CSG_String &String);


private:

	int								m_nColors;

	long							*m_Colors;


	void							_Set_Brightness		(double &a, double &b, double &c, int Pass = 0);

};


///////////////////////////////////////////////////////////
//														 //
//						Translator						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Translator
{
public:
	CSG_Translator(void);
	virtual ~CSG_Translator(void);

									CSG_Translator			(const CSG_String &File_Name, bool bSetExtension = true, int iText = 0, int iTranslation = 1, bool bCmpNoCase = false);
	bool							Create					(const CSG_String &File_Name, bool bSetExtension = true, int iText = 0, int iTranslation = 1, bool bCmpNoCase = false);

									CSG_Translator			(class CSG_Table *pTranslations, int iText = 0, int iTranslation = 1, bool bCmpNoCase = false);
	bool							Create					(class CSG_Table *pTranslations, int iText = 0, int iTranslation = 1, bool bCmpNoCase = false);

	void							Destroy					(void);

	bool							is_CaseSensitive		(void)		const	{	return( !m_bCmpNoCase );	}

	int								Get_Count				(void)		const	{	return( m_nTranslations );	}
	const SG_Char *					Get_Text				(int i)		const	{	return( i >= 0 && i < m_nTranslations ? m_Translations[i]->m_Text        : SG_T("") );	}
	const SG_Char *					Get_Translation			(int i)		const	{	return( i >= 0 && i < m_nTranslations ? m_Translations[i]->m_Translation : SG_T("") );	}

	const SG_Char *					Get_Translation			(const SG_Char *Text, bool bReturnNullOnNotFound = false)	const;
	bool							Get_Translation			(const SG_Char *Text, CSG_String &Translation)				const;


private:

	class CSG_Translation
	{
	public:
		CSG_Translation(const CSG_String &Text, const CSG_String &Translation)
		{
			m_Text					= Text;
			m_Translation			= Translation;
		}

		CSG_String					m_Text, m_Translation;
	};


private:

	bool							m_bCmpNoCase;

	int								m_nTranslations;

	CSG_Translation					**m_Translations;


	int								_Get_Index				(const CSG_String &Text)	const;

};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_Translator &	SG_Get_Translator	(void);

SAGA_API_DLL_EXPORT const SG_Char *		SG_Translate		(const CSG_String &Text);

#define _TL(s)	SG_Translate(L ## s)
#define _TW(s)	SG_Translate(CSG_String(s))


///////////////////////////////////////////////////////////
//														 //
//						UI Callback						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum
{
	SG_UI_MSG_STYLE_NORMAL	= 0,
	SG_UI_MSG_STYLE_BOLD,
	SG_UI_MSG_STYLE_ITALIC,
	SG_UI_MSG_STYLE_SUCCESS,
	SG_UI_MSG_STYLE_FAILURE,
	SG_UI_MSG_STYLE_BIG,
	SG_UI_MSG_STYLE_SMALL,
	SG_UI_MSG_STYLE_01,
	SG_UI_MSG_STYLE_02,
	SG_UI_MSG_STYLE_03
}
TSG_UI_MSG_STYLE;

//---------------------------------------------------------
typedef enum
{
	SG_UI_DATAOBJECT_UPDATE = 0,
	SG_UI_DATAOBJECT_SHOW_MAP,
	SG_UI_DATAOBJECT_SHOW_MAP_ACTIVE,
	SG_UI_DATAOBJECT_SHOW_MAP_NEW,
	SG_UI_DATAOBJECT_SHOW_MAP_LAST
}
TSG_UI_DataObject_Update;

//---------------------------------------------------------
typedef enum
{
	SG_UI_MAP_ACTIVE = 0,
	SG_UI_MAP_LAST,
	SG_UI_MAP_ALL
}
TSG_UI_Maps;

//---------------------------------------------------------
typedef enum
{
	SG_UI_WINDOW_ARRANGE_MDI_CASCADE      = 0x0001,
	SG_UI_WINDOW_ARRANGE_MDI_TILE_VER     = 0x0002,
	SG_UI_WINDOW_ARRANGE_MDI_TILE_HOR     = 0x0004,
	SG_UI_WINDOW_ARRANGE_TDI_TILE_VER     = 0x0008,
	SG_UI_WINDOW_ARRANGE_TDI_TILE_HOR     = 0x0010,
	SG_UI_WINDOW_ARRANGE_TDI_SPLIT_LEFT   = 0x0020,
	SG_UI_WINDOW_ARRANGE_TDI_SPLIT_RIGHT  = 0x0040,
	SG_UI_WINDOW_ARRANGE_TDI_SPLIT_TOP    = 0x0080,
	SG_UI_WINDOW_ARRANGE_TDI_SPLIT_BOTTOM = 0x0100
}
TSG_UI_Window_Arrange;

//---------------------------------------------------------
typedef enum
{
	CALLBACK_PROCESS_GET_OKAY,
	CALLBACK_PROCESS_SET_OKAY,
	CALLBACK_PROCESS_SET_BUSY,
	CALLBACK_PROCESS_SET_PROGRESS,
	CALLBACK_PROCESS_SET_READY,
	CALLBACK_PROCESS_SET_TEXT,

	CALLBACK_STOP_EXECUTION,

	CALLBACK_DLG_MESSAGE,
	CALLBACK_DLG_CONTINUE,
	CALLBACK_DLG_ERROR,
	CALLBACK_DLG_INFO,

	CALLBACK_DLG_PARAMETERS,

	CALLBACK_MESSAGE_ADD,
	CALLBACK_MESSAGE_ADD_ERROR,
	CALLBACK_MESSAGE_ADD_EXECUTION,

	CALLBACK_DATAOBJECT_ADD,
	CALLBACK_DATAOBJECT_UPDATE,
	CALLBACK_DATAOBJECT_SHOW,
	CALLBACK_DATAOBJECT_ASIMAGE,

	CALLBACK_DATAOBJECT_COLORS_GET,
	CALLBACK_DATAOBJECT_COLORS_SET,
	CALLBACK_DATAOBJECT_PARAMS_GET,
	CALLBACK_DATAOBJECT_PARAMS_SET,

	CALLBACK_SET_MAP_EXTENT,

	CALLBACK_DIAGRAM_SHOW,

	CALLBACK_DATABASE_UPDATE,

	CALLBACK_WINDOW_ARRANGE,

	CALLBACK_GET_APP_WINDOW,
	CALLBACK_GET_APP_PATH
}
TSG_UI_Callback_ID;

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_UI_Parameter
{
public:
	CSG_UI_Parameter(void)                    : Boolean(false), Number( 0.  ), Pointer(NULL)	{}
	CSG_UI_Parameter(bool              Value) : Boolean(Value), Number( 0.  ), Pointer(NULL)	{}
	CSG_UI_Parameter(int               Value) : Boolean(false), Number(Value), Pointer(NULL)	{}
	CSG_UI_Parameter(double            Value) : Boolean(false), Number(Value), Pointer(NULL)	{}
	CSG_UI_Parameter(const CSG_String &Value) : Boolean(false), Number( 0.  ), Pointer(NULL), String(Value)	{}
	CSG_UI_Parameter(void             *Value) : Boolean(false), Number( 0.  ), Pointer(Value)	{}
	CSG_UI_Parameter(const CSG_UI_Parameter &Copy);

	bool		Boolean;

	double		Number;

	void		*Pointer;

	CSG_String	String;

};

//---------------------------------------------------------
typedef int (* TSG_PFNC_UI_Callback) (TSG_UI_Callback_ID ID, CSG_UI_Parameter &Param_1, CSG_UI_Parameter &Param_2);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool					SG_Set_UI_Callback			(TSG_PFNC_UI_Callback Function);
SAGA_API_DLL_EXPORT TSG_PFNC_UI_Callback	SG_Get_UI_Callback			(void);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT void					SG_UI_Console_Set_UTF8		(bool bOn);
SAGA_API_DLL_EXPORT bool					SG_UI_Console_Get_UTF8		(void);

SAGA_API_DLL_EXPORT void					SG_UI_Console_Print_StdOut	(const char       *Text, SG_Char End = '\n', bool bFlush = true);
SAGA_API_DLL_EXPORT void					SG_UI_Console_Print_StdOut	(const wchar_t    *Text, SG_Char End = '\n', bool bFlush = true);
SAGA_API_DLL_EXPORT void					SG_UI_Console_Print_StdOut	(const CSG_String &Text, SG_Char End = '\n', bool bFlush = true);
SAGA_API_DLL_EXPORT void					SG_UI_Console_Print_StdErr	(const char       *Text, SG_Char End = '\n', bool bFlush = true);
SAGA_API_DLL_EXPORT void					SG_UI_Console_Print_StdErr	(const wchar_t    *Text, SG_Char End = '\n', bool bFlush = true);
SAGA_API_DLL_EXPORT void					SG_UI_Console_Print_StdErr	(const CSG_String &Text, SG_Char End = '\n', bool bFlush = true);

//---------------------------------------------------------
SAGA_API_DLL_EXPORT int						SG_UI_Progress_Lock			(bool bOn);
SAGA_API_DLL_EXPORT int						SG_UI_Progress_Reset		(void);
SAGA_API_DLL_EXPORT bool					SG_UI_Process_Get_Okay		(bool bBlink = false);
SAGA_API_DLL_EXPORT bool					SG_UI_Process_Set_Okay		(bool bOkay = true);
SAGA_API_DLL_EXPORT bool					SG_UI_Process_Set_Busy		(bool bOn = true, const CSG_String &Message = "");
SAGA_API_DLL_EXPORT bool					SG_UI_Process_Set_Progress	(int    Position, int    Range);
SAGA_API_DLL_EXPORT bool					SG_UI_Process_Set_Progress	(sLong  Position, sLong  Range);
SAGA_API_DLL_EXPORT bool					SG_UI_Process_Set_Progress	(double Position, double Range);
SAGA_API_DLL_EXPORT bool					SG_UI_Process_Set_Ready		(void);
SAGA_API_DLL_EXPORT void					SG_UI_Process_Set_Text		(const CSG_String &Text);

SAGA_API_DLL_EXPORT bool					SG_UI_Stop_Execution		(bool bDialog);

SAGA_API_DLL_EXPORT void					SG_UI_Dlg_Message			(const CSG_String &Message, const CSG_String &Caption);
SAGA_API_DLL_EXPORT bool					SG_UI_Dlg_Continue			(const CSG_String &Message, const CSG_String &Caption);
SAGA_API_DLL_EXPORT int						SG_UI_Dlg_Error				(const CSG_String &Message, const CSG_String &Caption);
SAGA_API_DLL_EXPORT void					SG_UI_Dlg_Info				(const CSG_String &Message, const CSG_String &Caption);
SAGA_API_DLL_EXPORT bool					SG_UI_Dlg_Parameters		(class CSG_Parameters *pParameters, const CSG_String &Caption);

SAGA_API_DLL_EXPORT int						SG_UI_Msg_Lock				(bool bOn);
SAGA_API_DLL_EXPORT int						SG_UI_Msg_Reset				(void);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add				(const char       *Message, bool bNewLine = true, TSG_UI_MSG_STYLE Style = SG_UI_MSG_STYLE_NORMAL);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add				(const wchar_t    *Message, bool bNewLine = true, TSG_UI_MSG_STYLE Style = SG_UI_MSG_STYLE_NORMAL);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add				(const CSG_String &Message, bool bNewLine = true, TSG_UI_MSG_STYLE Style = SG_UI_MSG_STYLE_NORMAL);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add_Execution		(const char       *Message, bool bNewLine = true, TSG_UI_MSG_STYLE Style = SG_UI_MSG_STYLE_NORMAL);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add_Execution		(const wchar_t    *Message, bool bNewLine = true, TSG_UI_MSG_STYLE Style = SG_UI_MSG_STYLE_NORMAL);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add_Execution		(const CSG_String &Message, bool bNewLine = true, TSG_UI_MSG_STYLE Style = SG_UI_MSG_STYLE_NORMAL);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add_Error			(const char       *Message);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add_Error			(const wchar_t    *Message);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Add_Error			(const CSG_String &Message);
SAGA_API_DLL_EXPORT void					SG_UI_Msg_Flush				(void);

SAGA_API_DLL_EXPORT void					SG_UI_ProgressAndMsg_Lock	(bool bOn);
SAGA_API_DLL_EXPORT void					SG_UI_ProgressAndMsg_Reset	(void);

SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_Add		(class CSG_Data_Object *pDataObject, int Show);
SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_Update		(class CSG_Data_Object *pDataObject, int Show, class CSG_Parameters *pParameters);
SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_Show		(class CSG_Data_Object *pDataObject, int Show);
SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_asImage	(class CSG_Data_Object *pDataObject, class CSG_Grid *pGrid);

SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_Colors_Get	(class CSG_Data_Object *pDataObject, class CSG_Colors *pColors);
SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_Colors_Set	(class CSG_Data_Object *pDataObject, class CSG_Colors *pColors);
SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_Params_Get	(class CSG_Data_Object *pDataObject, class CSG_Parameters *pParameters);
SAGA_API_DLL_EXPORT bool					SG_UI_DataObject_Params_Set	(class CSG_Data_Object *pDataObject, class CSG_Parameters *pParameters);

SAGA_API_DLL_EXPORT bool					SG_UI_Set_Map_Extent		(double xMin, double yMin, double xMax, double yMax, int Maps = SG_UI_MAP_ACTIVE);

SAGA_API_DLL_EXPORT bool					SG_UI_Diagram_Show			(class CSG_Table *pTable, class CSG_Parameters *pParameters);

SAGA_API_DLL_EXPORT bool					SG_UI_ODBC_Update			(const CSG_String &Server);

SAGA_API_DLL_EXPORT int						SG_UI_Window_Arrange		(int Arrange);

SAGA_API_DLL_EXPORT void *					SG_UI_Get_Window_Main		(void);
SAGA_API_DLL_EXPORT CSG_String				SG_UI_Get_Application_Path	(bool bPathOnly = false);


///////////////////////////////////////////////////////////
//														 //
//                     Environment                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
SAGA_API_DLL_EXPORT bool					SG_Initialize_Environment	(bool bLibraries = true, bool bProjections = true, const SG_Char *Directory = NULL, bool bInitializeWX = true);
SAGA_API_DLL_EXPORT bool					SG_Uninitialize_Environment	(void);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__SAGA_API__api_core_H
