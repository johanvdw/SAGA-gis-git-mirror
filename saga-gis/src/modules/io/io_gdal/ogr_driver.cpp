/**********************************************************
 * Version $Id: ogr_driver.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library                     //
//                                                       //
//                       io_gdal                         //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    ogr_driver.cpp                     //
//                                                       //
//            Copyright (C) 2008 by O. Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation; version 2 of the License.   //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not,       //
// write to the Free Software Foundation, Inc.,          //
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.de                    //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Bundesstr. 55                          //
//                D-20146 Hamburg                        //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "ogr_driver.h"

#include <ogr_api.h>
#include <cpl_string.h>
#include <ogr_srs_api.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_OGR_Drivers		gSG_OGR_Drivers;

const CSG_OGR_Drivers &	SG_Get_OGR_Drivers	(void)
{
	return gSG_OGR_Drivers;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_OGR_Drivers::CSG_OGR_Drivers(void)
{
#ifdef USE_GDAL_V2
	GDALAllRegister();
#else
	OGRRegisterAll();
#endif
}

//---------------------------------------------------------
CSG_OGR_Drivers::~CSG_OGR_Drivers(void)
{
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_OGR_Drivers::Get_Version(void) const
{
	return GDALVersionInfo("RELEASE_NAME");
}

//---------------------------------------------------------
int CSG_OGR_Drivers::Get_Count(void) const
{
	return GDALGetDriverCount();
}

#ifdef USE_GDAL_V2
//---------------------------------------------------------
GDALDriverH CSG_OGR_Drivers::Get_Driver(int Index) const
{
	return GDALGetDriver(Index);
}

GDALDriverH CSG_OGR_Drivers::Get_Driver(const CSG_String &Name) const
{
	return GDALGetDriverByName(Name);
}

//---------------------------------------------------------
CSG_String CSG_OGR_Drivers::Get_Name(int Index) const
{
	return GDALGetDriverLongName( GDALGetDriver(Index) );
}

//---------------------------------------------------------
CSG_String CSG_OGR_Drivers::Get_Description(int Index) const
{
	return GDALGetDescription( Get_Driver(Index) );
}

//---------------------------------------------------------
CSG_String CSG_OGR_Drivers::Get_Extension(int Index) const
{
	return GDALGetMetadataItem( Get_Driver(Index), GDAL_DMD_EXTENSION, "" );
}

//---------------------------------------------------------
bool CSG_OGR_Drivers::is_Vector(int Index) const
{
	return Get_Driver(Index) && CSLFetchBoolean( GDALGetMetadata( Get_Driver(Index), 0 ), GDAL_DCAP_VECTOR, false);
}

//---------------------------------------------------------
bool CSG_OGR_Drivers::Can_Read(int Index) const
{
	return( Get_Driver(Index) != NULL );
}

//---------------------------------------------------------
bool CSG_OGR_Drivers::Can_Write(int Index) const
{
	return Get_Driver(Index) && CSLFetchBoolean( GDALGetMetadata( Get_Driver(Index), 0 ), GDAL_DCAP_CREATE, false);
}

#else
//---------------------------------------------------------
OGRSFDriverH CSG_OGR_Drivers::Get_Driver(int Index) const
{
       return OGRGetDriver(Index);
}

OGRSFDriver * CSG_OGR_Drivers::Get_Driver(const CSG_String &Name) const
{
       return OGRGetDriverByName(Name);
}

//---------------------------------------------------------
CSG_String CSG_OGR_Drivers::Get_Name(int Index) const
{
       return OGR_Dr_GetName( OGRGetDriver(Index) );
}

//---------------------------------------------------------
CSG_String CSG_OGR_Drivers::Get_Description(int Index) const
{
       OGRSFDriverH pDriver = Get_Driver(Index);
       CSG_String      s;

       s       += OGR_Dr_TestCapability( pDriver, ODrCCreateDataSource)        ? SG_T("\n[x] ") :
       s       += _TL("create data source");

       s       += OGR_Dr_TestCapability( pDriver, ODrCDeleteDataSource)        ? SG_T("\n[x] ") :
       s       += _TL("delete data source");

/*     s       += OGR_Dr_TestCapability( pDriver, ODsCCreateLayer)                     ? SG_T("\n
       s       += _TL("create layer");

       s       += OGR_Dr_TestCapability( pDriver, ODsCDeleteLayer)                     ? SG_T("\n
       s       += _TL("delete layer");

       s       += OGR_Dr_TestCapability( pDriver, OLCDeleteFeature)            ? SG_T("\n[x]") :
       s       += _TL("delete feature");

       s       += OGR_Dr_TestCapability( pDriver, OLCRandomRead)                       ? SG_T("\n[x]") : SG_T("\n[ ]");
       s       += _TL("random read");

       s       += OGR_Dr_TestCapability( pDriver, OLCRandomWrite)                      ? SG_T("\n[x]") : SG_T("\n[ ]");
       s       += _TL("random write");

       s       += OGR_Dr_TestCapability( pDriver, OLCSequentialWrite)          ? SG_T("\n[x]") : SG_T("\n[ ]");
       s       += _TL("sequential write");
/**/

       return s;
}

//---------------------------------------------------------
CSG_String CSG_OGR_Drivers::Get_Extension(int Index) const
{
       return "";
}

//---------------------------------------------------------
bool CSG_OGR_Drivers::is_Vector(int Index) const
{
       return true;
}

//---------------------------------------------------------
bool CSG_OGR_Drivers::Can_Read(int Index) const
{
       return Get_Driver(Index);
}

//---------------------------------------------------------
bool CSG_OGR_Drivers::Can_Write(int Index) const
{
       return Get_Driver(Index);
}

#endif // #ifndef USE_GDAL_V2


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
TSG_Vertex_Type CSG_OGR_Drivers::Get_Vertex_Type(int Type)
{
	switch( Type )
	{
	case wkbPoint25D:				// 2.5D extension as per 99-402
	case wkbMultiPoint25D:			// 2.5D extension as per 99-402
	case wkbLineString25D:			// 2.5D extension as per 99-402
	case wkbMultiLineString25D:		// 2.5D extension as per 99-402
	case wkbPolygon25D:				// 2.5D extension as per 99-402
	case wkbMultiPolygon25D:		// 2.5D extension as per 99-402
	case wkbGeometryCollection25D:	// 2.5D extension as per 99-402 
		return( SG_VERTEX_TYPE_XYZ );

	default:
		return( SG_VERTEX_TYPE_XY );
	}
}

//---------------------------------------------------------
TSG_Shape_Type CSG_OGR_Drivers::Get_Shape_Type(int Type)
{
	switch( Type )
	{
	case wkbPoint:					// 0-dimensional geometric object, standard WKB
	case wkbPoint25D:				// 2.5D extension as per 99-402
		return( SHAPE_TYPE_Point );

	case wkbMultiPoint:				// GeometryCollection of Points, standard WKB
	case wkbMultiPoint25D:			// 2.5D extension as per 99-402
		return( SHAPE_TYPE_Points );

	case wkbLineString:				// 1-dimensional geometric object with linear interpolation between Points, standard WKB
	case wkbMultiLineString:		// GeometryCollection of LineStrings, standard WKB
	case wkbLineString25D:			// 2.5D extension as per 99-402
	case wkbMultiLineString25D:		// 2.5D extension as per 99-402
		return( SHAPE_TYPE_Line );

	case wkbPolygon:				// planar 2-dimensional geometric object defined by 1 exterior boundary and 0 or more interior boundaries, standard WKB
	case wkbMultiPolygon:			// GeometryCollection of Polygons, standard WKB
	case wkbPolygon25D:				// 2.5D extension as per 99-402
	case wkbMultiPolygon25D:		// 2.5D extension as per 99-402
		return( SHAPE_TYPE_Polygon );

	default:
	case wkbUnknown:				// unknown type, non-standard
	case wkbNone:					// non-standard, for pure attribute records
	case wkbLinearRing:				// non-standard, just for createGeometry()
	case wkbGeometryCollection:		// geometric object that is a collection of 1 or more geometric objects, standard WKB
	case wkbGeometryCollection25D:	// 2.5D extension as per 99-402 
		return( SHAPE_TYPE_Undefined );
	}
}

//---------------------------------------------------------
int CSG_OGR_Drivers::Get_Shape_Type(TSG_Shape_Type Type, bool bZ)
{
	switch( Type )
	{
	case SHAPE_TYPE_Point:		return( bZ ? wkbPoint25D           : wkbPoint           );	// point
	case SHAPE_TYPE_Points:		return( bZ ? wkbMultiPoint25D      : wkbMultiPoint      );	// points
	case SHAPE_TYPE_Line:		return( bZ ? wkbMultiLineString25D : wkbMultiLineString );	// line
	case SHAPE_TYPE_Polygon:	return( bZ ? wkbMultiPolygon25D    : wkbMultiPolygon    );	// polygon

	default:	return( wkbUnknown );
	}
}

//---------------------------------------------------------
TSG_Data_Type CSG_OGR_Drivers::Get_Data_Type(int Type)
{
	switch( Type )
	{
	case OFTInteger:		// Simple 32bit integer
		return( SG_DATATYPE_Int );

	case OFTReal:			// Double Precision floating point
		return( SG_DATATYPE_Double );

	case OFTString:			// String of ASCII chars
		return( SG_DATATYPE_String );

	case OFTDateTime:		// Date and Time 
		return( SG_DATATYPE_Date );

	default:
	case OFTIntegerList:	// List of 32bit integers
	case OFTRealList:		// List of doubles
	case OFTStringList:		// Array of strings
	case OFTWideString:		// deprecated
	case OFTWideStringList:	// deprecated
	case OFTBinary:			// Raw Binary data
	case OFTDate:			// Date
	case OFTTime:			// Time
		return( SG_DATATYPE_String );
	}
}

//---------------------------------------------------------
int CSG_OGR_Drivers::Get_Data_Type(TSG_Data_Type Type)
{
	switch( Type )
	{
	default:
	case SG_DATATYPE_String:
	case SG_DATATYPE_Date:
		return( OFTString );

	case SG_DATATYPE_Byte:
	case SG_DATATYPE_Char:
	case SG_DATATYPE_Word:
	case SG_DATATYPE_Short:
	case SG_DATATYPE_DWord:
	case SG_DATATYPE_Int:
	case SG_DATATYPE_ULong:
	case SG_DATATYPE_Long:
	case SG_DATATYPE_Color:
		return( OFTInteger );

	case SG_DATATYPE_Float:
	case SG_DATATYPE_Double:
		return( OFTReal );
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_OGR_DataSet::CSG_OGR_DataSet(void)
{
	m_pDataSet	= NULL;
}

CSG_OGR_DataSet::CSG_OGR_DataSet(const CSG_String &File)
{
	m_pDataSet	= NULL;

	Create(File);
}

//---------------------------------------------------------
CSG_OGR_DataSet::~CSG_OGR_DataSet(void)
{
	Destroy();
}

#ifdef USE_GDAL_V2
//---------------------------------------------------------
bool CSG_OGR_DataSet::Create(const CSG_String &File)
{
	Destroy();

	m_pDataSet = GDALOpenEx(File, GDAL_OF_VECTOR, NULL, NULL, NULL);

	return( m_pDataSet != NULL );
}

bool CSG_OGR_DataSet::Create(const CSG_String &File, const CSG_String &DriverName)
{
	GDALDriverH pDriver;

	Destroy();

	if( (pDriver = gSG_OGR_Drivers.Get_Driver(DriverName)) != NULL )
	{
		m_pDataSet = GDALCreate( pDriver, File, 0, 0, 0, GDT_Unknown, NULL);
	}

	return( m_pDataSet != NULL );
}

//---------------------------------------------------------
bool CSG_OGR_DataSet::Destroy(void)
{
	if( m_pDataSet )
	{
		GDALClose(m_pDataSet);

		m_pDataSet	= NULL;
	}

	return( true );
}

//---------------------------------------------------------
CSG_String CSG_OGR_DataSet::Get_DriverID(void)	const
{
	return m_pDataSet && GDALGetDatasetDriver(m_pDataSet) && GDALGetDescription( GDALGetDatasetDriver(m_pDataSet) ) ? GDALGetDescription( GDALGetDatasetDriver(m_pDataSet) ) : "";
}

//---------------------------------------------------------
CSG_String CSG_OGR_DataSet::Get_Description(void)	const
{
	return GDALGetDescription( m_pDataSet ) ? GDALGetDescription( m_pDataSet ) : "";
}

//---------------------------------------------------------
CSG_String CSG_OGR_DataSet::Get_Description(int i)	const
{
	CSG_String		Description;

	OGRLayerH	pLayer	= Get_Layer(i);

	if( pLayer != NULL )
	{
		char	**pMetaData = GDALGetMetadata( pLayer, 0 );

		if( pMetaData )
		{
			while( *pMetaData )
			{
				CSG_String	s(*pMetaData);

				Description	+= s + "\n";

				pMetaData++;
			}
		}
	}

	return( Description );
}

#else
//---------------------------------------------------------
bool CSG_OGR_DataSet::Create(const CSG_String &File)
{
	Destroy();

	m_pDataSet	= OGRSFDriverRegistrar::Open(File);

	return( m_pDataSet != NULL );
}

bool CSG_OGR_DataSet::Create(const CSG_String &File, const CSG_String &DriverName)
{
	OGRSFDriverH pDriver;

	Destroy();

	if( (pDriver = gSG_OGR_Drivers.Get_Driver(DriverName)) != NULL )
	{
		m_pDataSet	= OGR_Dr_CreateDataSource( pDriver, File, NULL );
	}

	return( m_pDataSet != NULL );
}

//---------------------------------------------------------
bool CSG_OGR_DataSet::Destroy(void)
{
	if( m_pDataSet )
	{
		OGRReleaseDataSource(m_pDataSet);

		m_pDataSet	= NULL;
	}

	return( true );
}

//---------------------------------------------------------
CSG_String CSG_OGR_DataSet::Get_DriverID   (void)	const	{	return( "" );	}
CSG_String CSG_OGR_DataSet::Get_Description(void)	const	{	return( "" );	}
CSG_String CSG_OGR_DataSet::Get_Description(int i)	const	{	return( "" );	}

#endif


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_OGR_DataSet::Get_Count(void)	const
{
	if( m_pDataSet )
	{
		return OGR_DS_GetLayerCount( m_pDataSet );
	}

	return( 0 );
}

//---------------------------------------------------------
OGRLayerH CSG_OGR_DataSet::Get_Layer(int iLayer)	const
{
	if( m_pDataSet && iLayer >= 0 && iLayer < Get_Count() )
	{
		return OGR_DS_GetLayer( m_pDataSet, iLayer);
	}

	return( NULL );
}

//---------------------------------------------------------
TSG_Shape_Type CSG_OGR_DataSet::Get_Type(int iLayer)	const
{
	if( Get_Layer(iLayer) )
	{
		return( CSG_OGR_Drivers::Get_Shape_Type( OGR_FD_GetGeomType( OGR_L_GetLayerDefn( Get_Layer(iLayer) ) ) ) );
	}

	return( SHAPE_TYPE_Undefined );
}

//---------------------------------------------------------
TSG_Vertex_Type CSG_OGR_DataSet::Get_Coordinate_Type(int iLayer)	const
{
	if( Get_Layer(iLayer) )
	{
		return( CSG_OGR_Drivers::Get_Vertex_Type( OGR_FD_GetGeomType( OGR_L_GetLayerDefn( Get_Layer(iLayer) ) ) ) );
	}

	return( SG_VERTEX_TYPE_XY );
}

//---------------------------------------------------------
CSG_Projection CSG_OGR_DataSet::Get_Projection(int iLayer)	const
{
	CSG_Projection	Projection;

	if( Get_Layer(iLayer) && OGR_L_GetSpatialRef( Get_Layer(iLayer) ) )
	{
		char	*p	= NULL;

		//-------------------------------------------------
		if( !Projection.is_Okay() && OSRExportToWkt( OGR_L_GetSpatialRef( Get_Layer(iLayer) ), &p ) == OGRERR_NONE && p && *p )
		{
			Projection.Create(p, SG_PROJ_FMT_WKT);
		}

		if( p )	{	OGRFree(p);	p	= NULL;	}

		//-------------------------------------------------
		if( !Projection.is_Okay() && OSRExportToProj4( OGR_L_GetSpatialRef( Get_Layer(iLayer) ), &p) == OGRERR_NONE && p && *p )
		{
			Projection.Create(p, SG_PROJ_FMT_Proj4);
		}

		if( p )	{	OGRFree(p);	p	= NULL;	}
	}

	return( Projection );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Shapes * CSG_OGR_DataSet::Read(int iLayer, int iGeomTypeChoice)
{
	//-----------------------------------------------------
	OGRLayerH pLayer	= Get_Layer(iLayer);

	if( !pLayer )
	{
		return( NULL );
	}

	if( iGeomTypeChoice != 0 )
	{
		OGR_FD_SetGeomType( OGR_L_GetLayerDefn( pLayer ),  (OGRwkbGeometryType)_Get_GeomType_Choice(iGeomTypeChoice) );
	}

	if( Get_Type(iLayer) == SHAPE_TYPE_Undefined )
	{
		return( NULL );
	}

	//-----------------------------------------------------
	OGRFeatureDefnH pDef = OGR_L_GetLayerDefn( pLayer );
	CSG_Shapes		*pShapes	= SG_Create_Shapes(Get_Type(iLayer), CSG_String(OGR_Fld_GetNameRef(pDef)), NULL, Get_Coordinate_Type(iLayer));

	pShapes->Get_Projection()	= Get_Projection(iLayer);

	//-----------------------------------------------------
	int		iField;

	for(iField=0; iField< OGR_FD_GetFieldCount(pDef); iField++)
	{
		OGRFieldDefnH pDefField	= OGR_FD_GetFieldDefn( pDef, iField);

		pShapes->Add_Field( OGR_Fld_GetNameRef( pDefField ), CSG_OGR_Drivers::Get_Data_Type( OGR_Fld_GetType( pDefField ) ) );
	}

	//-----------------------------------------------------
	OGRFeatureH pFeature;

	OGR_L_ResetReading( pLayer );

	while( (pFeature = OGR_L_GetNextFeature( pLayer ) ) != NULL && SG_UI_Process_Get_Okay(false) )
	{
		OGRGeometryH pGeometry = OGR_F_GetGeometryRef( pFeature );

		if( pGeometry != NULL )
		{
			CSG_Shape	*pShape	= pShapes->Add_Shape();

			for(iField=0; iField<OGR_FD_GetFieldCount(pDef); iField++)
			{
				OGRFieldDefnH pDefField	= OGR_FD_GetFieldDefn(pDef, iField);

				switch( OGR_Fld_GetType( pDefField ) )
				{
				default:			pShape->Set_Value(iField, OGR_F_GetFieldAsString( pFeature, iField));	break;
				case OFTString:		pShape->Set_Value(iField, OGR_F_GetFieldAsString( pFeature, iField));	break;
				case OFTInteger:	pShape->Set_Value(iField, OGR_F_GetFieldAsInteger( pFeature, iField));	break;
				case OFTReal:		pShape->Set_Value(iField, OGR_F_GetFieldAsDouble( pFeature, iField));	break;
				}
			}

			//---------------------------------------------
			if( _Read_Geometry(pShape, pGeometry) == false )
			{
				pShapes->Del_Shape(pShape);
			}
		}

		OGR_F_Destroy(pFeature);
	}

	return( pShapes );
}

//---------------------------------------------------------
int CSG_OGR_DataSet::_Get_GeomType_Choice(int iGeomTypeChoice)
{
	switch( iGeomTypeChoice )
	{
	default:
	case AUTOMATIC:					return( wkbUnknown );
	case WKBPOINT:					return( wkbPoint );
	case WKBPOINT25D:				return( wkbPoint25D );
	case WKBMULTIPOINT:				return( wkbMultiPoint );
	case WKBMULTIPOINT25D:			return( wkbMultiPoint25D );
	case WKBLINESTRING:				return( wkbLineString );
	case WKBLINESTRING25D:			return( wkbLineString25D );
	case WKBMULTILINESTRING:		return( wkbMultiLineString );
	case WKBMULTILINESTRING25D:		return( wkbMultiLineString25D );
	case WKBPOLYGON:				return( wkbPolygon );
	case WKBPOLYGON25D:				return( wkbPolygon25D );
	case WKBMULTIPOLYGON:			return( wkbMultiPolygon );
	case WKBMULTIPOLYGON25D:		return( wkbMultiPolygon25D );
	case WKBGEOMETRYCOLLECTION:		return( wkbGeometryCollection );
	case WKBGEOMETRYCOLLECTION25D:	return( wkbGeometryCollection25D );
	}
}

//---------------------------------------------------------
bool CSG_OGR_DataSet::_Read_Geometry(CSG_Shape *pShape, OGRGeometryH pGeometry)
{
	if( pShape && pGeometry )
	{
		switch( OGR_G_GetGeometryType(pGeometry) )
		{
		//-------------------------------------------------
		case wkbPoint:				// 0-dimensional geometric object, standard WKB
		case wkbPoint25D:			// 2.5D extension as per 99-402
			pShape->Add_Point(OGR_G_GetX(pGeometry, 0), OGR_G_GetY(pGeometry, 0));
			pShape->Set_Z(OGR_G_GetZ(pGeometry, 0), 0);
			return( true );

		//-------------------------------------------------
		case wkbLineString:			// 1-dimensional geometric object with linear interpolation between Points, standard WKB
		case wkbLineString25D:		// 2.5D extension as per 99-402
			return( _Read_Line(pShape, pGeometry) );

		//-------------------------------------------------
		case wkbPolygon:			// planar 2-dimensional geometric object defined by 1 exterior boundary and 0 or more interior boundaries, standard WKB
		case wkbPolygon25D:			// 2.5D extension as per 99-402
			return( _Read_Polygon(pShape, pGeometry) );

		//-------------------------------------------------
		case wkbMultiPoint:			// GeometryCollection of Points, standard WKB
		case wkbMultiPoint25D:		// 2.5D extension as per 99-402
		case wkbMultiLineString:	// GeometryCollection of LineStrings, standard WKB
		case wkbMultiLineString25D:	// 2.5D extension as per 99-402
		case wkbMultiPolygon:		// GeometryCollection of Polygons, standard WKB
		case wkbMultiPolygon25D:	// 2.5D extension as per 99-402
			{
				for(int i=0; i< OGR_G_GetGeometryCount(pGeometry); i++)
				{
					if( _Read_Geometry(pShape, OGR_G_GetGeometryRef(pGeometry, i)) == false )
					{
						return( false );
					}
				}
			}

			return( true );

		//-------------------------------------------------
		default:
			break;
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_OGR_DataSet::_Read_Line(CSG_Shape *pShape, OGRGeometryH pLine)
{
	if( pShape && pLine && OGR_G_GetPointCount(pLine) > 0 )
	{
		int		iPart	= pShape->Get_Part_Count();

		for(int iPoint=0; iPoint<OGR_G_GetPointCount(pLine); iPoint++)
		{
			pShape->Add_Point(OGR_G_GetX(pLine, iPoint), OGR_G_GetY(pLine, iPoint), iPart);

			pShape->Set_Z(OGR_G_GetZ(pLine, iPoint), iPoint, iPart);
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_OGR_DataSet::_Read_Polygon(CSG_Shape *pShape, OGRGeometryH pPolygon)
{
	if( pShape && pPolygon )
	{
		OGRGeometryH r = OGR_G_GetGeometryRef(pPolygon, 0);
		_Read_Line(pShape, r);

		for(int i=1; i<OGR_G_GetGeometryCount(pPolygon); i++)
		{
			r = OGR_G_GetGeometryRef(pPolygon, i);
			_Read_Line(pShape, r);
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_OGR_DataSet::Write(CSG_Shapes *pShapes)
{
	if( !m_pDataSet || !pShapes || !pShapes->is_Valid() )
	{
		return( false );
	}

	//-----------------------------------------------------
	OGRSpatialReferenceH pSRS = NULL;

	if( pShapes->Get_Projection().is_Okay() )
	{
		pSRS = OSRNewSpatialReference( pShapes->Get_Projection().Get_WKT() );
	//	pSRS	= new OGRSpatialReference();
	//	pSRS	->importFromProj4(pShapes->Get_Projection().Get_Proj4());
	}

	OGRLayerH pLayer = GDALDatasetCreateLayer( m_pDataSet, CSG_String(pShapes->Get_Name()), pSRS,
		(OGRwkbGeometryType)gSG_OGR_Drivers.Get_Shape_Type(pShapes->Get_Type(), pShapes->Get_Vertex_Type() != SG_VERTEX_TYPE_XY), NULL
	);

	if( !pLayer )
	{
		return( false );
	}

	//-------------------------------------------------
#ifdef USE_GDAL_V2
	if( SG_STR_CMP( GDALGetDescription( GDALGetDatasetDriver( m_pDataSet ) ), "DXF") )
	{
		// the dxf driver does not support arbitrary field creation and returns OGRERR_FAILURE;
		// it seems like there is no method in OGR to check whether a driver supports field creation or not;
		// another issue with the dxf driver: 3D polygon data is not supported (would require e.g. "3DFACE"
		// entity implementation in GDAL/OGR), so we would need to treat them as polylines (not implemented,
		// currently it is necessary to convert to a line shapefile a priori)

		for(int iField=0; iField<pShapes->Get_Field_Count(); iField++)
		{
			OGRFieldDefnH DefField = OGR_Fld_Create(CSG_String(pShapes->Get_Field_Name(iField)), (OGRFieldType)gSG_OGR_Drivers.Get_Data_Type(pShapes->Get_Field_Type(iField)));

			if( OGR_L_CreateField(pLayer, DefField, TRUE) != OGRERR_NONE )
			{
				return( false );
			}
		}
	}
#endif

	//-----------------------------------------------------
	for(int iShape=0; iShape<pShapes->Get_Count() && SG_UI_Process_Set_Progress(iShape, pShapes->Get_Count()); iShape++)
	{
		CSG_Shape	*pShape		= pShapes->Get_Shape(iShape);
		OGRFeatureH	pFeature        = OGR_F_Create( OGR_L_GetLayerDefn( pLayer ) );

		if( _Write_Geometry(pShape, pFeature, pShapes->Get_Vertex_Type() != SG_VERTEX_TYPE_XY) )
		{
			// no need for a special treatment of DXF here, as pFeature->SetField() just silently ignores iFields out of range
			for(int iField=0; iField<pShapes->Get_Field_Count(); iField++)
			{
				switch( pShapes->Get_Field_Type(iField) )
				{
				default:
				case SG_DATATYPE_Char:
				case SG_DATATYPE_String:
				case SG_DATATYPE_Date:
					OGR_F_SetFieldString(pFeature, iField, CSG_String(pShape->asString(iField)));
					break;

				case SG_DATATYPE_Short:
				case SG_DATATYPE_Int:
				case SG_DATATYPE_Long:
				case SG_DATATYPE_Color:
					OGR_F_SetFieldInteger(pFeature, iField, pShape->asInt(iField));
					break;

				case SG_DATATYPE_Float:
				case SG_DATATYPE_Double:
					OGR_F_SetFieldDouble(pFeature, iField, pShape->asDouble(iField));
					break;
				}
			}

			OGR_L_CreateFeature(pLayer, pFeature);
		}

		OGR_F_Destroy(pFeature);
	}

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CSG_OGR_DataSet::_Write_Geometry(CSG_Shape *pShape, OGRFeatureH pFeature, bool bZ)
{
	if( !pShape || !pFeature )
	{
		return( false );
	}

	switch( pShape->Get_Type() )
	{
	//-----------------------------------------------------
	case SHAPE_TYPE_Point:
		{
			TSG_Point	p	= pShape->Get_Point(0);

			OGRGeometryH g = OGR_G_CreateGeometry(wkbPoint);
			OGR_G_SetPoint(g, 0, p.x, p.y, pShape->Get_Z(0) );
			return OGR_F_SetGeometryDirectly(pFeature, g) == OGRERR_NONE;
		}

	//-----------------------------------------------------
	case SHAPE_TYPE_Points:
		{
			OGRGeometryH g = OGR_G_CreateGeometry(wkbMultiPoint);

			for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
			{
				for(int iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
				{
					TSG_Point	p	= pShape->Get_Point(iPoint, iPart);

					OGR_G_AddPoint(g, p.x, p.y, pShape->Get_Z(0));
				}
			}

			return OGR_F_SetGeometryDirectly(pFeature, g) == OGRERR_NONE;
		}

	//-----------------------------------------------------
	case SHAPE_TYPE_Line:
		if( pShape->Get_Part_Count() == 1 )
		{
			OGRGeometryH g = OGR_G_CreateGeometry(wkbLineString);

			_Write_Line(pShape, g, 0, bZ);

			return OGR_F_SetGeometryDirectly(pFeature, g) == OGRERR_NONE;
		}
		else
		{
			OGRGeometryH g = OGR_G_CreateGeometry(wkbMultiLineString);

			for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
			{
				OGRGeometryH l = OGR_G_CreateGeometry(wkbLineString);

				if( _Write_Line(pShape, l, iPart, bZ) )
				{
					OGR_G_AddGeometry(g, l);
				}
			}

			return OGR_F_SetGeometryDirectly(pFeature, g) == OGRERR_NONE;
		}

	//-----------------------------------------------------
	case SHAPE_TYPE_Polygon:
		{
			OGRGeometryH g = OGR_G_CreateGeometry(wkbPolygon);

			for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
			{
				OGRGeometryH r = OGR_G_CreateGeometry(wkbLinearRing);

				if( _Write_Line(pShape, r, iPart, bZ) )
				{
					OGR_G_AddGeometry(g, r);
				}
			}

			return OGR_F_SetGeometryDirectly(pFeature, g) == OGRERR_NONE;
		}

	//-------------------------------------------------
	default:
		return( false );
	}
}

//---------------------------------------------------------
bool CSG_OGR_DataSet::_Write_Line(CSG_Shape *pShape, OGRGeometryH pLine, int iPart, bool bZ)
{
	if( pLine && pShape && iPart >= 0 && iPart < pShape->Get_Part_Count() )
	{
		OGR_G_SetPointCount(pLine, 0);

		for(int iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
		{
			TSG_Point	p	= pShape->Get_Point(iPoint, iPart);

			OGR_G_AddPoint(pLine, p.x, p.y, pShape->Get_Z(iPoint, iPart));
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// vim: ts=4 :
