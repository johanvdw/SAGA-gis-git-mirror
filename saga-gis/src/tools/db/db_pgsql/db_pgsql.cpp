
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                       db_pgsql                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     db_pgsql.cpp                      //
//                                                       //
//                 Copyright (C) 2013 by                 //
//                     Olaf Conrad                       //
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
//    e-mail:     oconrad@saga-gis.de                    //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "db_pgsql.h"

extern "C" {
#include <libpq-fe.h>
}

#include <ctype.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_PG_Connections	g_Connections;

//---------------------------------------------------------
CSG_PG_Connections &	SG_PG_Get_Connection_Manager(void)
{
	return( g_Connections );
}

//---------------------------------------------------------
bool	SG_PG_is_Supported(void)
{
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void _Error_Message(const CSG_String &Message, const CSG_String &Additional = "")
{
	SG_UI_Msg_Add_Execution(Message, true, SG_UI_MSG_STYLE_FAILURE);

	CSG_String	s(Message);

	if( Additional.Length() > 0 )
	{
		s	+= ":\n";
		s	+= Additional;
		s	+= "\n";
	}

	SG_UI_Msg_Add_Error(s);
}

//---------------------------------------------------------
void _Error_Message(const CSG_String &Message, PGconn *pConnection)
{
	CSG_String	Error	= PQerrorMessage(pConnection);

	_Error_Message(Message, Error);
}

//---------------------------------------------------------
void _Error_Message(PGconn *pConnection)
{
	CSG_String	Error	= PQerrorMessage(pConnection);

	_Error_Message(Error);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define m_pgConnection	((PGconn   *)m_pConnection)


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_PG_Connection::CSG_PG_Connection(void)
{
	m_bTransaction	= false;

	m_pConnection	= NULL;
}

//---------------------------------------------------------
CSG_PG_Connection::CSG_PG_Connection(const CSG_String &Host, int Port, const CSG_String &Name, const CSG_String &User, const CSG_String &Password, bool bAutoCommit)
{
	Create(Host, Port, Name, User, Password, bAutoCommit);
}

bool CSG_PG_Connection::Create(const CSG_String &Host, int Port, const CSG_String &Name, const CSG_String &User, const CSG_String &Password, bool bAutoCommit)
{
	m_bTransaction	= false;

	m_pConnection	= Name.is_Empty()
		? PQsetdbLogin(Host, CSG_String::Format("%d", Port), NULL, NULL, NULL, User, Password)
		: PQsetdbLogin(Host, CSG_String::Format("%d", Port), NULL, NULL, Name, User, Password);

	if( PQstatus(m_pgConnection) != CONNECTION_OK )
	{
		_Error_Message((_TL("Connection to database failed"), m_pgConnection));

		Destroy();

		return( false );
	}

	CSG_String	Encoding	= pg_encoding_to_char(PQclientEncoding(m_pgConnection));

	if( Encoding.CmpNoCase("UTF8") )
	{
		SG_UI_Msg_Add_Execution(CSG_String::Format("\n%s: %s", _TL("client encoding"), Encoding.c_str()), false);

		if( !PQsetClientEncoding(m_pgConnection, "UTF8") )
		{
			SG_UI_Msg_Add_Execution(CSG_String::Format("\n%s", _TL("client encoding changed to UTF-8")), false);
		}
		else
		{
			SG_UI_Msg_Add_Execution(CSG_String::Format("\n%s", _TL("failed to change client encoding to UTF-8")), false);
		}
	}

	return( true );
}

//---------------------------------------------------------
CSG_PG_Connection::~CSG_PG_Connection(void)
{
	Destroy();
}

bool CSG_PG_Connection::Destroy(void)
{
	if( !is_Connected() )	{	return( false );	}

	PQfinish(m_pgConnection);

	m_pConnection	= NULL;

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SG_PG_BIT		1560
#define SG_PG_BOOL		16
#define	SG_PG_BYTEA		17
#define	SG_PG_CHAR		18
#define SG_PG_DATE		1082
#define	SG_PG_FLOAT4	700
#define	SG_PG_FLOAT8	701
#define	SG_PG_INT2		21
#define	SG_PG_INT4		23
#define	SG_PG_INT8		20
#define	SG_PG_MONEY		790
#define	SG_PG_NAME		19
#define	SG_PG_NUMERIC	1700
#define	SG_PG_OID		26
#define	SG_PG_TEXT		25
#define	SG_PG_TIME		1083
#define	SG_PG_TIMESTAMP	1114
#define SG_PG_VARCHAR	1043

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Type_To_SQL(TSG_Data_Type Type, int Size)
{
	switch( Type )
	{
	case SG_DATATYPE_String: return( CSG_String::Format("varchar(%d)", Size > 0 ? Size : 1) );
	case SG_DATATYPE_Date  : return( "date"             );
	case SG_DATATYPE_Char  : return( "char(1)"          );
	case SG_DATATYPE_Byte  : return( "smallint"         );
	case SG_DATATYPE_Short : return( "smallint"         );
	case SG_DATATYPE_Int   : return( "integer"          );
	case SG_DATATYPE_Long  : return( "bigint"           );
	case SG_DATATYPE_Color : return( "integer"          );
	case SG_DATATYPE_Float : return( "real"             );
	case SG_DATATYPE_Double: return( "double precision" );
	case SG_DATATYPE_Binary: return( "bytea"            );
	default                : return( "text"             );
	}
}

//---------------------------------------------------------
TSG_Data_Type CSG_PG_Connection::Get_Type_From_SQL(int Type)
{
	switch( Type )
	{
	case SG_PG_VARCHAR:	return( SG_DATATYPE_String );
	case SG_PG_NAME   :	return( SG_DATATYPE_String );
	case SG_PG_TEXT   :	return( SG_DATATYPE_String );
	case SG_PG_INT2   :	return( SG_DATATYPE_Short  );	// 2 bytes integer
	case SG_PG_INT4   :	return( SG_DATATYPE_Int    );	// 4 bytes integer
	case SG_PG_INT8   :	return( SG_DATATYPE_Long   );	// 8 bytes integer
	case SG_PG_DATE   :	return( SG_DATATYPE_Date   );
	case SG_PG_NUMERIC:	return( SG_DATATYPE_Float  );
	case SG_PG_FLOAT4 :	return( SG_DATATYPE_Float  );	// 4 bytes floating point, inexact
	case SG_PG_FLOAT8 :	return( SG_DATATYPE_Double );	// 8 bytes floating point, inexact
	case SG_PG_BYTEA  :	return( SG_DATATYPE_Binary );
	default           :	return( SG_DATATYPE_String );
	}
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Raster_Type_To_SQL(TSG_Data_Type Type)
{
	switch( Type )
	{
	case SG_DATATYPE_Bit   :	return( "1BB"   );
	case SG_DATATYPE_Char  :	return( "8BSI"  );
	case SG_DATATYPE_Byte  :	return( "8BUI"  );
	case SG_DATATYPE_Short :	return( "16BSI" );
	case SG_DATATYPE_Word  :	return( "16BUI" );
	case SG_DATATYPE_Int   :	return( "32BSI" );
	case SG_DATATYPE_DWord :	return( "32BUI" );
	case SG_DATATYPE_Long  :	return( "32BSI" );
	case SG_DATATYPE_Float :	return( "32BF"  );
	case SG_DATATYPE_Double:	return( "64BF"  );
	default                :	return( "64BF"  );
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_PG_Connection::GUI_Update(void) const
{
	SG_UI_ODBC_Update(Get_Connection());
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Connection(int Style) const
{
	switch( Style )
	{
	default: return( Get_DBName() + " [" + Get_Host() + ":" + Get_Port() + "]" );

	case  1: return( Get_DBName() );

	case  2: return( "PGSQL:" + Get_Host() + ":" + Get_Port() + ":" + Get_DBName() );
	}
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Host(void) const
{
	return( PQhost(m_pgConnection) );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Port(void) const
{
	return( PQport(m_pgConnection) );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_User(void) const
{
	return( PQuser(m_pgConnection) );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_DBName(void) const
{
	return( PQdb  (m_pgConnection) );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Version(void) const
{
	int	Version	= PQserverVersion(m_pgConnection);

	return( CSG_String::Format("%d.%d.%d",
		(Version / 100) / 100,
		(Version / 100) % 100,
		(Version % 100)
	));
}

//---------------------------------------------------------
bool CSG_PG_Connection::has_Version(int Major, int Minor, int Revision) const
{
	int	Version	= PQserverVersion(m_pgConnection);

	return( Version >= (Major * 10000 + Minor * 100 + Revision) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_PostGIS(void) const
{
	CSG_Table	t;

	if( _Table_Load(t, "SELECT PostGIS_Lib_Version()", "") && t.Get_Count() == 1 && t.Get_Field_Count() == 1 )
	{
		return( t[0][0].asString() );
	}

	return( "" );
}

//---------------------------------------------------------
bool CSG_PG_Connection::has_PostGIS(double minVersion)
{
	CSG_Table	t;

	return( _Table_Load(t, "SELECT PostGIS_Lib_Version()", "") && t.Get_Count() == 1 && t.Get_Field_Count() == 1
		&& t[0][0].asDouble() >= minVersion
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_MetaData & CSG_PG_Connection::Add_MetaData(CSG_Data_Object &Object, const CSG_String &Table, const CSG_String &Select)
{
	CSG_String	Name(Object.Get_Name());
	Object.Set_File_Name(Get_Connection(2) + ":" + Table);
	Object.Set_Name(Name);

	CSG_MetaData	&DB	= Object.Get_MetaData_DB();

	DB.Del_Children();

	DB.Add_Child("DBMS" , "PostgreSQL");
	DB.Add_Child("HOST" , Get_Host      ());
	DB.Add_Child("PORT" , Get_Port      ());
	DB.Add_Child("USER" , Get_User      ());
	DB.Add_Child("NAME" , Get_DBName    ());
	DB.Add_Child("TABLE", Table);

	if( !Select.is_Empty() )
	{
		DB.Add_Child("SELECT", Select);
	}

	return( DB );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_PG_Connection::Get_Tables(CSG_Strings &Tables) const
{
	Tables.Clear();

	if( is_Connected() )
	{
		PGresult	*pResult	= PQexec(m_pgConnection,
			"SELECT table_name FROM information_schema.tables WHERE table_schema='public' ORDER BY table_name"
		);

		if( PQresultStatus(pResult) != PGRES_TUPLES_OK )
		{
			_Error_Message(_TL("listing of database tables failed"), m_pgConnection);
		}
		else
		{
			for(int iRecord=0; iRecord<PQntuples(pResult); iRecord++)
			{
				Tables	+= PQgetvalue(pResult, iRecord, 0);
			}
		}

		PQclear(pResult);
	}

	return( Tables.Get_Count() );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Tables(void) const
{
	CSG_String	List;
	CSG_Strings	Tables;

	if( Get_Tables(Tables) )
	{
		for(int i=0; i<Tables.Get_Count(); i++)
		{
			List	+= Tables[i] + '|';
		}
	}

	return( List );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Table_Exists(const CSG_String &Table_Name) const
{
	CSG_Strings	Tables;

	if( Get_Tables(Tables) )
	{
		for(int i=0; i<Tables.Get_Count(); i++)
		{
			if( Table_Name.Cmp(Tables[i]) == 0 )
			{
				return( true );
			}
		}
	}

	return( false );
}

//---------------------------------------------------------
CSG_Table CSG_PG_Connection::Get_Field_Desc(const CSG_String &Table_Name, bool bVerbose) const
{
	CSG_Table	Fields;

	Fields.Fmt_Name("%s [%s]", Table_Name.c_str(), _TL("Field Description"));

	if( bVerbose )
	{
		Fields.Add_Field(_TL("Name"     ), SG_DATATYPE_String);
		Fields.Add_Field(_TL("Type"     ), SG_DATATYPE_String);
		Fields.Add_Field(_TL("Primary"  ), SG_DATATYPE_String);
		Fields.Add_Field(_TL("NotNull"  ), SG_DATATYPE_String);
		Fields.Add_Field(_TL("Default"  ), SG_DATATYPE_String);
		Fields.Add_Field(_TL("Comment"  ), SG_DATATYPE_String);
	}
	else
	{
		Fields.Add_Field(_TL("Name"     ), SG_DATATYPE_String);
		Fields.Add_Field(_TL("Type"     ), SG_DATATYPE_String);
		Fields.Add_Field(_TL("Size"     ), SG_DATATYPE_Int   );
		Fields.Add_Field(_TL("Precision"), SG_DATATYPE_Int   );
	}

	if( is_Connected() )
	{
		CSG_String	s;

		if( bVerbose )
		{
			s	+= "SELECT DISTINCT ";
			s	+= "  a.attnum, ";
			s	+= "  a.attname, ";
			s	+= "  format_type(a.atttypid, a.atttypmod), ";
			s	+= "  coalesce(i.indisprimary,false), ";
			s	+= "  a.attnotnull, ";
			s	+= "  def.adsrc, ";
			s	+= "  com.description ";
			s	+= "FROM pg_attribute a ";
			s	+= "  JOIN pg_class pgc ON pgc.oid=a.attrelid ";
			s	+= "  LEFT JOIN pg_index i ON ";
			s	+= "    (pgc.oid=i.indrelid AND i.indkey[0]=a.attnum) ";
			s	+= "  LEFT JOIN pg_description com on ";
			s	+= "    (pgc.oid=com.objoid AND a.attnum=com.objsubid) ";
			s	+= "  LEFT JOIN pg_attrdef def ON ";
			s	+= "    (a.attrelid=def.adrelid AND a.attnum=def.adnum) ";
			s	+= "WHERE a.attnum>0 AND pgc.oid=a.attrelid ";
			s	+= "  AND pg_table_is_visible(pgc.oid) ";
			s	+= "  AND NOT a.attisdropped ";
			s	+= "  AND pgc.relname='" + Table_Name + "' ";
			s	+= "ORDER BY a.attnum ";
		}
		else
		{
			s	+= "SELECT column_name, udt_name, character_maximum_length, numeric_precision ";
			s	+= "FROM information_schema.columns ";
			s	+= "WHERE table_name='" + Table_Name + "' ";
			s	+= "ORDER BY ordinal_position";
		}

		PGresult	*pResult	= PQexec(m_pgConnection, s);

		if( PQresultStatus(pResult) != PGRES_TUPLES_OK )
		{
			_Error_Message(_TL("listing of table columns failed"), m_pgConnection);
		}
		else
		{
			for(int iRecord=0; iRecord<PQntuples(pResult); iRecord++)
			{
				CSG_Table_Record	*pRecord	= Fields.Add_Record();

				for(int iField=0; iField<Fields.Get_Field_Count(); iField++)
				{
					char	*Value	= PQgetvalue(pResult, iRecord, iField + (bVerbose ? 1 : 0));

					if( bVerbose && (iField == 2 || iField == 3) )
					{
						pRecord->Set_Value(iField, *Value == 't' ? _TL("yes") : _TL("no"));
					}
					else
					{
						pRecord->Set_Value(iField, Value);
					}
				}
			}
		}

		PQclear(pResult);
	}

	return( Fields );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Get_Field_Names(const CSG_String &Table_Name) const
{
	CSG_Table	Fields	= Get_Field_Desc(Table_Name);

	CSG_String	Names;

	for(int i=0; i<Fields.Get_Count(); i++)
	{
		Names	+= Fields[i].asString(3);
		Names	+= "|";
	}

	return( Names );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::Execute(const CSG_String &SQL, CSG_Table *pTable, bool bToUTF8)
{
	if( !is_Connected() )	{	_Error_Message(_TL("no database connection"));	return( false );	}

	PGresult	*pResult;

	if( bToUTF8 )
	{
		CSG_Buffer	SQL_UTF8	= SQL.to_UTF8();

		pResult	= PQexec(m_pgConnection, SQL_UTF8.Get_Data());
	}
	else
	{
		pResult	= PQexec(m_pgConnection, SQL);
	}

	bool	bResult;

	switch( PQresultStatus(pResult) )
	{
	default:
		bResult	= false;
		PQclear(pResult);
		_Error_Message(_TL("SQL execution failed"), m_pgConnection);
		break;

	case PGRES_COMMAND_OK:
		bResult	= true;
		PQclear(pResult);
		break;

	case PGRES_TUPLES_OK:
		if( pTable )
		{
			bResult	= _Table_Load(*pTable, pResult);
			pTable->Set_Name(_TL("Query Result"));
		}
		else
		{
			bResult	= true;
			PQclear(pResult);
		}
		break;
	}

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::Begin(const CSG_String &SavePoint)
{
	CSG_String	Command;

	if( SavePoint.is_Empty() )
	{
		if( !is_Connected() )	{	_Error_Message(_TL("no database connection"));	return( false );	}
		if( is_Transaction() )	{	_Error_Message(_TL("already in transaction"));	return( false );	}

		Command	= "BEGIN";
	}
	else
	{
		if( !is_Transaction() )	{	_Error_Message(_TL("not in transaction"));	return( false );	}

		Command	= "SAVEPOINT " + SavePoint;
	}

	PGresult	*pResult	= PQexec(m_pgConnection, Command);
	bool		 bResult	= PQresultStatus(pResult) == PGRES_COMMAND_OK;

	if( bResult )
	{
		m_bTransaction	= true;
	}
	else if( SavePoint.is_Empty() )
	{
		_Error_Message(_TL("begin transaction command failed"), m_pgConnection);
	}
	else
	{
		_Error_Message(_TL("could not add save point"), m_pgConnection);
	}

	PQclear(pResult);

	return( bResult );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Rollback(const CSG_String &SavePoint)
{
	if( !is_Transaction() )	{	_Error_Message(_TL("not in transaction"));	return( false );	}

	CSG_String	Command	= "ROLLBACK";

	if( !SavePoint.is_Empty() )
	{
		Command	+= " TO SAVEPOINT " + SavePoint;
	}

	PGresult	*pResult	= PQexec(m_pgConnection, Command);

	if( PQresultStatus(pResult) != PGRES_COMMAND_OK )
	{
		_Error_Message(_TL("rollback transaction command failed"), m_pgConnection);

		PQclear(pResult);

		return( false );
	}

	if( SavePoint.is_Empty() )
	{
		m_bTransaction	= false;
	}

	PQclear(pResult);

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Commit(const CSG_String &SavePoint)
{
	if( !is_Transaction() )	{	_Error_Message(_TL("not in transaction"));	return( false );	}

	CSG_String	Command;

	if( SavePoint.is_Empty() )
	{
		Command	= "COMMIT";
	}
	else
	{
		Command	= "RELEASE SAVEPOINT " + SavePoint;
	}

	PGresult	*pResult	= PQexec(m_pgConnection, Command);

	if( PQresultStatus(pResult) != PGRES_COMMAND_OK )
	{
		_Error_Message(_TL("commit transaction command failed"), m_pgConnection);

		PQclear(pResult);

		return( false );
	}

	m_bTransaction	= false;

	PQclear(pResult);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool	SG_String_Replace_Characters(CSG_String &String, const CSG_String Characters, const CSG_String &Replacement)
{
	for(size_t i=0; i<Characters.Length(); i++)
	{
		String.Replace(Characters[i], Replacement);
	}

	return( true );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Make_Table_Name(const CSG_String &Table_Name)
{
	CSG_String	Name(Table_Name);

	SG_String_Replace_Characters(Name, ".,;:({[]})#+-", '_');

	Name.Replace("ä", "ae"); Name.Replace("Ä", "AE");
	Name.Replace("ö", "oe"); Name.Replace("Ö", "OE");
	Name.Replace("ü", "ue"); Name.Replace("Ü", "UE");
	Name.Replace("ß", "sz");

	if( !Name.is_Empty() && isdigit(Name[0]) )
	{
		Name.Prepend("_");
	}

	return( Name );
}

//---------------------------------------------------------
CSG_String CSG_PG_Connection::Make_Table_Field_Name(const CSG_Table &Table, int Field)
{
	CSG_String	Name(Table.Get_Field_Name(Field));

	SG_String_Replace_Characters(Name, ".,;:({[]})#+-", '_');

	Name.Replace("ä", "ae"); Name.Replace("Ä", "Ae");
	Name.Replace("ö", "oe"); Name.Replace("Ö", "Oe");
	Name.Replace("ü", "ue"); Name.Replace("Ü", "Ue");
	Name.Replace("ß", "sz");

	return( Name );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Table_Create(const CSG_String &_Table_Name, const CSG_Table &Table, const CSG_Buffer &Flags, bool bCommit)
{
	if( Table.Get_Field_Count() <= 0 )
	{
		_Error_Message(_TL("no attributes in table"));

		return( false );
	}

	//-----------------------------------------------------
	CSG_String SQL, Table_Name(Make_Table_Name(_Table_Name));

	SQL.Printf("CREATE TABLE \"%s\"(", Table_Name.c_str());

	//-----------------------------------------------------
	for(int iField=0; iField<Table.Get_Field_Count(); iField++)
	{
		CSG_String s = Get_Type_To_SQL(Table.Get_Field_Type(iField), Table.Get_Field_Length(iField));

		char Flag = (int)Flags.Get_Size() == Table.Get_Field_Count() ? Flags[iField] : 0;

		if( (Flag & SG_PG_PRIMARY_KEY) == 0 )
		{
			if( (Flag & SG_PG_UNIQUE) != 0 )
			{
				s += " UNIQUE";
			}

			if( (Flag & SG_PG_NOT_NULL) != 0 )
			{
				s += " NOT NULL";
			}
		}

		if( iField > 0 )
		{
			SQL += ", ";
		}

		SQL += CSG_String::Format("\"%s\" %s", Make_Table_Field_Name(Table, iField).c_str(), s.c_str());
	}

	//-----------------------------------------------------
	if( (int)Flags.Get_Size() == Table.Get_Field_Count() )
	{
		CSG_String s;

		for(int iField=0; iField<Table.Get_Field_Count(); iField++)
		{
			if( (Flags[iField] & SG_PG_PRIMARY_KEY) != 0 )
			{
				s += s.Length() == 0 ? ", PRIMARY KEY(" : ", ";
				s += Make_Table_Field_Name(Table, iField);
			}
		}

		if( s.Length() > 0 )
		{
			SQL += s + ")";
		}
	}

	//-----------------------------------------------------
	SQL += ")";

	return( Execute(SQL) );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Table_Drop(const CSG_String &_Table_Name, bool bCommit)
{
	CSG_String	Table_Name(Make_Table_Name(_Table_Name));

	if( !Table_Exists(Table_Name) )
	{
		_Error_Message(_TL("database table does not exist"));

		return( false );
	}

	return( Execute(CSG_String::Format("DROP TABLE \"%s\"", Table_Name.c_str())) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::Table_Insert(const CSG_String &_Table_Name, const CSG_Table &Table, bool bCommit)
{
	if( !is_Connected() ) { _Error_Message(_TL("no database connection")); return( false ); }

	//-----------------------------------------------------
	CSG_String Table_Name(Make_Table_Name(_Table_Name));

	if( !Table_Exists(Table_Name) )
	{
		return( false );
	}

	if( Table.Get_Field_Count() <= 0 || Table.Get_Field_Count() != Get_Field_Desc(Table_Name).Get_Count() )
	{
		return( false );
	}

	//-----------------------------------------------------
	int nFields = Table.Get_Field_Count();

	char **paramValues  = (char **)SG_Malloc(nFields * sizeof(char *));
	int   *paramLengths = (int   *)SG_Malloc(nFields * sizeof(int   ));
	int   *paramFormats = (int   *)SG_Malloc(nFields * sizeof(int   ));
//	Oid   *paramTypes   = (Oid   *)SG_Malloc(nFields * sizeof(Oid   ));

	CSG_Buffer *Values = new CSG_Buffer[nFields];

	CSG_String Insert("INSERT INTO \"" + Table_Name + "\" VALUES(");

	for(int iField=0; iField<nFields; iField++)
	{
		Insert	+= CSG_String::Format("$%d%c", 1 + iField, iField < nFields - 1 ? SG_T(',') : SG_T(')'));

		paramFormats[iField]	= Table.Get_Field_Type(iField) == SG_DATATYPE_Binary ? 1 : 0;

		switch( Table.Get_Field_Type(iField) )
		{
		case SG_DATATYPE_Binary:
		case SG_DATATYPE_String:
			break;

		case SG_DATATYPE_Date  :
			Values[iField].Set_Size(16);
			break;
 
		default                :
		case SG_DATATYPE_Byte  :
		case SG_DATATYPE_Short :
		case SG_DATATYPE_Int   :
		case SG_DATATYPE_Color :
		case SG_DATATYPE_Long  :
		case SG_DATATYPE_Float :
		case SG_DATATYPE_Double:
			Values[iField].Set_Size(256);
			break;
		}
	}

	//-----------------------------------------------------
	bool bResult = true;

	for(sLong iRecord=0; iRecord<Table.Get_Count() && bResult && SG_UI_Process_Set_Progress(iRecord, Table.Get_Count()); iRecord++)
	{
		CSG_Table_Record *pRecord = Table.Get_Record(iRecord);

		for(int iField=0; iField<Table.Get_Field_Count(); iField++)
		{
			if( pRecord->is_NoData(iField) )
			{
				paramValues [iField] = NULL;
			}
			else if( Table.Get_Field_Type(iField) == SG_DATATYPE_Binary )
			{
				paramValues [iField] = (char *)pRecord->Get_Value(iField)->asBinary().Get_Bytes();
				paramLengths[iField] =         pRecord->Get_Value(iField)->asBinary().Get_Count();
			}
			else
			{
				CSG_String Value = pRecord->asString(iField);

				if( Table.Get_Field_Type(iField) == SG_DATATYPE_String )
				{
					Values[iField] = Value.to_UTF8();
				}
				else
				{
					strcpy(Values[iField].Get_Data(), Value.b_str());
				}

				paramValues[iField] = Values[iField].Get_Data();
			}
		}

		PGresult *pResult = PQexecParams(m_pgConnection, Insert, nFields, NULL, paramValues, paramLengths, paramFormats, 0);

		if( PQresultStatus(pResult) != PGRES_COMMAND_OK )
		{
			_Error_Message(CSG_String::Format("%s (record: %d)", _TL("Record insertion failed"), 1 + iRecord), m_pgConnection);

			bResult = false;
		}

		PQclear(pResult);
	}

	//-----------------------------------------------------
	delete[](Values);

	SG_Free(paramValues );
	SG_Free(paramLengths);
	SG_Free(paramFormats);
//	SG_Free(paramTypes  );

	SG_UI_Process_Set_Progress(0., 0.);

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::Table_Save(const CSG_String &_Table_Name, const CSG_Table &Table, const CSG_Buffer &Flags, bool bCommit)
{
	if( !is_Connected() )
	{
		_Error_Message(_TL("no database connection"));

		return( false );
	}

	CSG_String Table_Name(Make_Table_Name(_Table_Name));

	if( Table_Exists(Table_Name) && !Table_Drop(Table_Name, bCommit) )
	{
		return( false );
	}

	if( !Table_Create(Table_Name, Table, Flags, bCommit) )
	{
		return( false );
	}

	if( !Table_Insert(Table_Name, Table, bCommit) )
	{
		return( false );
	}

	//-----------------------------------------------------
	Add_MetaData(*((CSG_Table *)&Table), Table_Name);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::_Table_Load(CSG_Table &Table, const CSG_String &Select, const CSG_String &Name) const
{
	if( !is_Connected() ) { _Error_Message(_TL("no database connection")); return( false ); }

	//-----------------------------------------------------
	bool bResult = _Table_Load(Table, PQexec(m_pgConnection, Select));

	Table.Set_Name(Name);

	return( bResult );
}

//---------------------------------------------------------
bool CSG_PG_Connection::_Table_Load(CSG_Table &Table, void *_pResult) const
{
	PGresult *pResult = (PGresult *)_pResult;

	if( PQresultStatus(pResult) != PGRES_TUPLES_OK )
	{
		_Error_Message(_TL("SQL execution failed"), m_pgConnection);

		PQclear(pResult);

		return( false );
	}

	//-----------------------------------------------------
	int nFields = PQnfields(pResult);

	if( nFields <= 0 )
	{
		_Error_Message(_TL("no fields in selection"));

		return( false );
	}

	//-----------------------------------------------------
	Table.Destroy();

	for(int iField=0; iField<nFields; iField++)
	{
		Table.Add_Field(PQfname(pResult, iField), Get_Type_From_SQL(PQftype(pResult, iField)));
	}

	//-----------------------------------------------------
	int nRecords = PQntuples(pResult);

	for(int iRecord=0; iRecord<nRecords && SG_UI_Process_Set_Progress(iRecord, nRecords); iRecord++)
	{
		CSG_Table_Record *pRecord = Table.Add_Record();

		for(int iField=0; pRecord && iField<nFields; iField++)
		{
			if( PQgetisnull(pResult, iRecord, iField) )
			{
				pRecord->Set_NoData(iField);
			}
			else switch( Table.Get_Field_Type(iField) )
			{
			default:
				pRecord->Set_Value(iField, PQgetvalue(pResult, iRecord, iField));
				break;

			case SG_DATATYPE_String:
				pRecord->Set_Value(iField, CSG_String::from_UTF8(PQgetvalue(pResult, iRecord, iField)));
				break;

			case SG_DATATYPE_Binary: {
				CSG_Bytes	Binary; Binary.fromHexString(PQgetvalue(pResult, iRecord, iField) + 2);

				pRecord->Set_Value(iField, Binary);
				break; }
			}
		}
	}

	//-----------------------------------------------------
	PQclear(pResult);

	SG_UI_Process_Set_Progress(0., 0.);

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Table_Load(CSG_Table &Table, const CSG_String &Table_Name)
{
	if( _Table_Load(Table, CSG_String::Format("SELECT * FROM \"%s\"", Table_Name.c_str()), Table_Name) )
	{
		Add_MetaData(Table, Table_Name);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Table_Load(CSG_Table &Table, const CSG_String &Tables, const CSG_String &Fields, const CSG_String &Where, const CSG_String &Group, const CSG_String &Having, const CSG_String &Order, bool bDistinct, bool bVerbose)
{
	CSG_String	Select("SELECT");

	Select	+= bDistinct ? " DISTINCT" : " ALL";

	if( Fields.is_Empty() )
	{
		Select	+= " *";
	}
	else
	{
		Select	+= " " + Fields;
	}

	Select	+= " FROM ";

	CSG_Strings	s	= SG_String_Tokenize(Tables, ",");

	for(int i=0; i<s.Get_Count(); i++)
	{
		s[i].Trim_Both();

		if( s[i][0] != '\"')
		{
			s[i].Prepend("\"").Append("\"");
		}

		if( i > 0 )
		{
			Select	+= ",";
		}

		Select	+= s[i];
	}

	if( Where.Length() )
	{
		Select	+= " WHERE " + Where;
	}

	if( Group.Length() )
	{
		Select	+= " GROUP BY " + Group;

		if( Having.Length() )
		{
			Select	+= " HAVING " + Having;
		}
	}

	if( Order.Length() )
	{
		Select	+= " ORDER BY " + Order;
	}

	if( bVerbose )
	{
		SG_UI_Msg_Add_Execution(CSG_String::Format("\n%s: '%s'", _TL("SQL Query"), Select.c_str()), false);
	}

	if( _Table_Load(Table, Select, Table.Get_Name()) )
	{
		Add_MetaData(Table, Table.Get_Name(), Select);

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::Shapes_Geometry_Info(const CSG_String &geoTable, CSG_String *geoField, int *geoSRID)
{
	CSG_Table Table_Info;

	if( Table_Load(Table_Info, "geometry_columns", "*", "f_table_name='" + geoTable + "'") && Table_Info.Get_Count() == 1 )
	{
		if( geoField ) *geoField = Table_Info[0].asString("f_geometry_column");
		if( geoSRID  ) *geoSRID  = Table_Info[0].asInt   ("srid"             );

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GEOMETRY_FIELD	SG_T("__geometry__")

//---------------------------------------------------------
bool CSG_PG_Connection::_Shapes_Load(const CSG_String &geoTable, CSG_String &Fields)
{
	CSG_Table Field_Info = Get_Field_Desc(geoTable);

	if( Field_Info.Get_Count() > 0 )
	{
		CSG_String geoField;

		if( Shapes_Geometry_Info(geoTable, &geoField, NULL) )
		{
			for(int i=0; i<Field_Info.Get_Count(); i++)
			{
				if( geoField.Cmp(Field_Info[i].asString(0)) )
				{
					Fields += CSG_String::Format("%s\"%s\"", Fields.is_Empty() ? SG_T("") : SG_T(","), Field_Info[i].asString(0));
				}
			}

			return( true );
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PG_Connection::_Shapes_Load(const CSG_String &geoTable, const CSG_String &Tables, const CSG_String &Fields, const CSG_String &Where, const CSG_String &Geometry, bool bBinary, int &SRID, CSG_String &Select)
{
	CSG_String geoField;

	if( Shapes_Geometry_Info(geoTable, &geoField, &SRID) )
	{
		Select.Printf("SELECT %s, ST_As%s(%s) AS %s FROM \"%s\" ",
			Fields.c_str(),
			bBinary ? SG_T("Binary") : SG_T("Text"),
			geoField.c_str(),
			Geometry.c_str(),
			geoTable.c_str()
		);

		if( !Tables.is_Empty() ) { Select +=       "," + Tables; }
		if( !Where .is_Empty() ) { Select += " WHERE " + Where ; }

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void * CSG_PG_Connection::_Shapes_Load(const CSG_String &Select, const CSG_String &geoFieldName, int &nFields, int &nRecords, int &geoField)
{
	if( !is_Connected() ) { _Error_Message(_TL("no database connection")); return( NULL ); }
	if( !has_PostGIS () ) { _Error_Message(_TL("not a PostGIS database")); return( NULL ); }

	//-----------------------------------------------------
	PGresult *pResult = PQexec(m_pgConnection, Select);

	if( PQresultStatus(pResult) != PGRES_TUPLES_OK )
	{
		_Error_Message(_TL("SQL execution failed"), m_pgConnection);

		PQclear(pResult); return( NULL );
	}

	//-----------------------------------------------------
	if( (nFields = PQnfields(pResult)) <= 0 )
	{
		_Error_Message(_TL("no fields in selection"));

		PQclear(pResult); return( NULL );
	}

	//-----------------------------------------------------
	if( (nRecords = PQntuples(pResult)) <= 0 )
	{
		_Error_Message(_TL("no records in selection"));

		PQclear(pResult); return( NULL );
	}

	//-----------------------------------------------------
	geoField = -1;

	for(int iField=0; geoField<0 && iField<nFields; iField++)
	{
		if( !geoFieldName.CmpNoCase(PQfname(pResult, iField)) )
		{
			geoField = iField;
		}
	}

	if( geoField < 0 )
	{
		_Error_Message(_TL("no geometry in selection"));

		PQclear(pResult); return( NULL );
	}

	//-----------------------------------------------------
	return( pResult );
}

//---------------------------------------------------------
inline bool CSG_PG_Connection::_Shape_Get_Type(const char *WKBytes, bool bBinary, TSG_Shape_Type &Geometry, TSG_Vertex_Type &Vertex)
{
	if( bBinary )
	{
		CSG_Bytes Binary; Binary.fromHexString(WKBytes + 2);

		return( CSG_Shapes_OGIS_Converter::to_ShapeType(Binary.asDWord(1, false), Geometry, Vertex) );
	}

	return( CSG_Shapes_OGIS_Converter::to_ShapeType(CSG_String(WKBytes).BeforeFirst('('), Geometry, Vertex) );
}

//---------------------------------------------------------
inline TSG_Shape_Type CSG_PG_Connection::_Shape_Get_Type(const char *WKBytes, bool bBinary)
{
	if( bBinary )
	{
		CSG_Bytes Binary; Binary.fromHexString(WKBytes + 2);

		return( CSG_Shapes_OGIS_Converter::to_ShapeType(Binary.asDWord(1, false)) );
	}

	return( CSG_Shapes_OGIS_Converter::to_ShapeType(CSG_String(WKBytes).BeforeFirst('(')) );
}

//---------------------------------------------------------
inline bool CSG_PG_Connection::_Shape_Load_Record(void *_pResult, int iRecord, int geoField, bool bBinary, CSG_Shapes *pShapes)
{
	PGresult *pResult = (PGresult *)_pResult; CSG_Shape *pRecord = pShapes->Add_Shape();

	if( bBinary )
	{
		CSG_Bytes Binary; Binary.fromHexString(PQgetvalue(pResult, iRecord, geoField) + 2);

		CSG_Shapes_OGIS_Converter::from_WKBinary(Binary, pRecord);
	}
	else
	{
		CSG_Shapes_OGIS_Converter::from_WKText(PQgetvalue(pResult, iRecord, geoField), pRecord);
	}

	for(int iField=0, jField=0; iField<=pShapes->Get_Field_Count(); iField++)
	{
		if( iField != geoField )
		{
			if( PQgetisnull(pResult, iRecord, iField) )
			{
				pRecord->Set_NoData(jField++);
			}
			else switch( pShapes->Get_Field_Type(jField) )
			{
			default: {
				pRecord->Set_Value(jField++, PQgetvalue(pResult, iRecord, iField));
				break; }

			case SG_DATATYPE_String: {
				pRecord->Set_Value(jField++, CSG_String::from_UTF8(PQgetvalue(pResult, iRecord, iField)));
				break; }

			case SG_DATATYPE_Binary: {
				CSG_Bytes Binary; Binary.fromHexString(PQgetvalue(pResult, iRecord, iField) + 2);

				pRecord->Set_Value(jField++, Binary);
				break; }
			}
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::Shapes_Load(CSG_Shapes *pShapes, const CSG_String &geoTable)
{
	CSG_String Fields;

	return( _Shapes_Load(geoTable, Fields) && Shapes_Load(pShapes, geoTable, geoTable, "", Fields, "") );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Shapes_Load(CSG_Shapes *pShapes, const CSG_String &Name, const CSG_String &geoTable, const CSG_String &Tables, const CSG_String &Fields, const CSG_String &Where)
{
	CSG_String Select; int SRID;

	return( _Shapes_Load(geoTable, Tables, Fields, Where, GEOMETRY_FIELD, has_Version(9), SRID, Select)
		&&   Shapes_Load(pShapes, Name, Select          , GEOMETRY_FIELD, has_Version(9), SRID) );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Shapes_Load(CSG_Shapes *pShapes, const CSG_String &Name, const CSG_String &Select, const CSG_String &geoFieldName, bool bBinary, int SRID)
{
	int nFields, nRecords, geoField; PGresult *pResult = (PGresult *)_Shapes_Load(Select, geoFieldName, nFields, nRecords, geoField);

	if( !pResult )
	{
		return( false );
	}

	//-----------------------------------------------------
	for(int iRecord=0; iRecord<nRecords && SG_UI_Process_Set_Progress(iRecord, nRecords); iRecord++)
	{
		TSG_Shape_Type Geometry; TSG_Vertex_Type Vertex; _Shape_Get_Type(PQgetvalue(pResult, iRecord, geoField), bBinary, Geometry, Vertex);

		if( Geometry == SHAPE_TYPE_Undefined || (Geometry != pShapes->Get_Type() && pShapes->Get_Type() != SHAPE_TYPE_Undefined) )
		{
			continue;
		}

		if( pShapes->Get_Type() == SHAPE_TYPE_Undefined )
		{
			pShapes->Create(Geometry, Name, NULL, Vertex); pShapes->Get_Projection().Create(SRID);

			for(int iField=0; iField<nFields; iField++)
			{
				if( iField != geoField )
				{
					pShapes->Add_Field(PQfname(pResult, iField), Get_Type_From_SQL(PQftype(pResult, iField)));
				}
			}
		}

		_Shape_Load_Record(pResult, iRecord, geoField, bBinary, pShapes);
	}

	SG_UI_Process_Set_Progress(0., 0.);

	PQclear(pResult);

	//-----------------------------------------------------
	if( pShapes->is_Valid() )
	{
		Add_MetaData(*pShapes, Name, Select);

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CSG_PG_Connection::Shapes_Load(CSG_Shapes *pShapes[4], const CSG_String &geoTable)
{
	CSG_String Fields;

	return( _Shapes_Load(geoTable, Fields) ? Shapes_Load(pShapes, geoTable, geoTable, "", Fields, "") : 0 );
}

//---------------------------------------------------------
int CSG_PG_Connection::Shapes_Load(CSG_Shapes *pShapes[4], const CSG_String &Name, const CSG_String &geoTable, const CSG_String &Tables, const CSG_String &Fields, const CSG_String &Where)
{
	CSG_String Select; int SRID;

	return( _Shapes_Load(geoTable, Tables, Fields, Where, GEOMETRY_FIELD, has_Version(9), SRID, Select)
		?    Shapes_Load(pShapes, Name, Select          , GEOMETRY_FIELD, has_Version(9), SRID) : 0 );
}

//---------------------------------------------------------
int CSG_PG_Connection::Shapes_Load(CSG_Shapes *pShapes[4], const CSG_String &Name, const CSG_String &Select, const CSG_String &geoFieldName, bool bBinary, int SRID)
{
	int nFields, nRecords, geoField; PGresult *pResult = (PGresult *)_Shapes_Load(Select, geoFieldName, nFields, nRecords, geoField);

	if( !pResult )
	{
		return( false );
	}

	for(int i=0; i<4; i++) { pShapes[i] = NULL; }

	//-----------------------------------------------------
	for(int iRecord=0; iRecord<nRecords && SG_UI_Process_Set_Progress(iRecord, nRecords); iRecord++)
	{
		TSG_Shape_Type Geometry; TSG_Vertex_Type Vertex; _Shape_Get_Type(PQgetvalue(pResult, iRecord, geoField), bBinary, Geometry, Vertex);

		if( Geometry == SHAPE_TYPE_Undefined )
		{
			continue;
		}

		int i = Geometry == SHAPE_TYPE_Point   ? 0
			  : Geometry == SHAPE_TYPE_Points  ? 1
			  : Geometry == SHAPE_TYPE_Line    ? 2
			  : Geometry == SHAPE_TYPE_Polygon ? 3 : -1;

		if( pShapes[i] == NULL )
		{
			pShapes[i] = SG_Create_Shapes(Geometry, Name, NULL, Vertex); pShapes[i]->Get_Projection().Create(SRID);

			for(int iField=0; iField<nFields; iField++)
			{
				if( iField != geoField )
				{
					pShapes[i]->Add_Field(PQfname(pResult, iField), Get_Type_From_SQL(PQftype(pResult, iField)));
				}
			}
		}

		_Shape_Load_Record(pResult, iRecord, geoField, bBinary, pShapes[i]);
	}

	SG_UI_Process_Set_Progress(0., 0.);

	PQclear(pResult);

	//-----------------------------------------------------
	int nValid = 0;

	for(int i=0; i<4; i++)
	{
		if( pShapes[i] )
		{
			nValid++; Add_MetaData(*pShapes[i], Name, Select);
		}
	}

	return( nValid );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::Shapes_Insert(CSG_Shapes *pShapes, const CSG_String &_geoTable)
{
	if( !is_Connected() ) { _Error_Message(_TL("no database connection")); return( false ); }
	if( !has_PostGIS () ) { _Error_Message(_TL("not a PostGIS database")); return( false ); }

	CSG_String geoTable(Make_Table_Name(_geoTable));

	if( !Table_Exists(geoTable) )
	{
		return( false );
	}

	//-----------------------------------------------------
	CSG_String geoField; int geoSRID;

	if( !Shapes_Geometry_Info(geoTable, &geoField, &geoSRID) )
	{
		return( false );
	}

	//-----------------------------------------------------
	int nFields = pShapes->Get_Field_Count();

	char **paramValues  = (char **)SG_Malloc((1 + (size_t)nFields) * sizeof(char *));
	int   *paramLengths = (int   *)SG_Malloc((1 + (size_t)nFields) * sizeof(int   ));
	int   *paramFormats = (int   *)SG_Malloc((1 + (size_t)nFields) * sizeof(int   ));
//	Oid   *paramTypes   = (Oid   *)SG_Malloc((1 + (size_t)nFields) * sizeof(Oid   ));

	CSG_Buffer *Values = new CSG_Buffer[1 + (size_t)nFields];

	CSG_String Insert("INSERT INTO \"" + geoTable + "\" (");

	for(int iField=0; iField<nFields; iField++)
	{
		Insert += "\"" + Make_Table_Field_Name(pShapes, iField) + "\", ";
	}

	Insert += "\"" + geoField + "\") VALUES (";

	for(int iField=0; iField<nFields; iField++)
	{
		Insert += CSG_String::Format("$%d, ", 1 + iField);

		paramFormats[iField] = pShapes->Get_Field_Type(iField) == SG_DATATYPE_Binary ? 1 : 0;

		switch( pShapes->Get_Field_Type(iField) )
		{
		case SG_DATATYPE_Binary:
		case SG_DATATYPE_String:
			break;

		case SG_DATATYPE_Date  :
			Values[iField].Set_Size(16);
			break;

		default                :
		case SG_DATATYPE_Byte  :
		case SG_DATATYPE_Short :
		case SG_DATATYPE_Int   :
		case SG_DATATYPE_Color :
		case SG_DATATYPE_Long  :
		case SG_DATATYPE_Float :
		case SG_DATATYPE_Double:
			Values[iField].Set_Size(256);
			break;
		}
	}

	bool bBinary = has_Version(9);

	if( bBinary )
	{
		Insert += CSG_String::Format("ST_GeomFromWKB($%d, %d))" , 1 + nFields, geoSRID);

		paramFormats[nFields] = 1;
	}
	else
	{
		Insert += CSG_String::Format("ST_GeomFromText($%d, %d))", 1 + nFields, geoSRID);

		paramFormats[nFields] = 0;
	}

	//-----------------------------------------------------
	bool bResult = true;

	for(sLong iShape=0; iShape<pShapes->Get_Count() && bResult && SG_UI_Process_Set_Progress(iShape, pShapes->Get_Count()); iShape++)
	{
		CSG_Shape *pShape = pShapes->Get_Shape(iShape);

		if( !pShape->is_Valid() )
		{
			bResult = false;

			continue;
		}

		//-------------------------------------------------
		for(int iField=0; iField<pShapes->Get_Field_Count(); iField++)
		{
			if( pShape->is_NoData(iField) )
			{
				paramValues [iField] = NULL;
			}
			else if( pShapes->Get_Field_Type(iField) == SG_DATATYPE_Binary )
			{
				paramValues [iField] = (char *)pShape->Get_Value(iField)->asBinary().Get_Bytes();
				paramLengths[iField] =         pShape->Get_Value(iField)->asBinary().Get_Count();
			}
			else
			{
				CSG_String Value = pShape->asString(iField);

				if( pShapes->Get_Field_Type(iField) == SG_DATATYPE_String )
				{
					Values[iField] = Value.to_UTF8();
				}
				else
				{
					strcpy(Values[iField].Get_Data(), Value.b_str());
				}

				paramValues[iField] = Values[iField].Get_Data();
			}
		}

		//-------------------------------------------------
		CSG_Bytes WKB; CSG_String WKT;

		if( bBinary )
		{
			CSG_Shapes_OGIS_Converter::to_WKBinary(pShape, WKB);

			paramValues [nFields] = (char *)WKB.Get_Bytes();
			paramLengths[nFields] =         WKB.Get_Count();
		}
		else
		{
			CSG_Shapes_OGIS_Converter::to_WKText  (pShape, WKT);

			Values[nFields].Set_Data(WKT.b_str(), WKT.Length() + 1); WKT.Clear();
			paramValues[nFields] = Values[nFields].Get_Data();
		}

		//-------------------------------------------------
		PGresult *pResult = PQexecParams(m_pgConnection, Insert, 1 + nFields, NULL, paramValues, paramLengths, paramFormats, 0);

		if( PQresultStatus(pResult) != PGRES_COMMAND_OK )
		{
			_Error_Message(CSG_String::Format("%s (record: %d)", _TL("Record insertion failed"), 1 + iShape), m_pgConnection);

			bResult	= false;
		}

		PQclear(pResult);
	}

	//-----------------------------------------------------
	delete[](Values);

	SG_Free(paramValues );
	SG_Free(paramLengths);
	SG_Free(paramFormats);
//	SG_Free(paramTypes  );

	SG_UI_Process_Set_Progress(0., 0.);

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connection::_Raster_Open(CSG_Table &Info, const CSG_String &Table, const CSG_String &Where, const CSG_String &Order, bool bBinary)
{
	//-----------------------------------------------------
	if( !Table_Load(Info, "raster_columns", "*", CSG_String("r_table_name = '") + Table + "'") || Info.Get_Count() != 1 )
	{
		SG_UI_Msg_Add_Error(CSG_String::Format("[PostGIS] %s (%s)", _TL("could not access table"), SG_T("raster_columns")));

		return( false );
	}

	CSG_String	Fields, Field	= Info[0].asString("r_raster_column");

	//-----------------------------------------------------
	Info	= Get_Field_Desc(Table);

	for(int i=0; i<Info.Get_Count(); i++)
	{
		if( SG_STR_CMP(Info[i].asString(1), "raster") )
		{
			if( !Fields.is_Empty() )
			{
				Fields	+= ",";
			}

			Fields	+= Info[i].asString(0);
		}
	}

	//-----------------------------------------------------
	if( !Table_Load(Info, Table, Fields, Where, "", "", Order) )
	{
		SG_UI_Msg_Add_Error(CSG_String::Format("[PostGIS] %s (%s)", _TL("could not access raster table"), Table.c_str()));

		return( false );
	}

	//-----------------------------------------------------
	CSG_String	Select	= "COPY (SELECT ST_AsBinary(\"" + Field + "\") AS rastbin FROM \"" + Table + "\"";

	if( Where.Length() )	Select	+= " WHERE "    + Where;
	if( Order.Length() )	Select	+= " ORDER BY " + Order;

	Select	+= ") TO STDOUT";	if( bBinary )	Select	+= " WITH (FORMAT 'binary')";

	//-----------------------------------------------------
	PGresult	*pResult	= PQexec(m_pgConnection, Select);

	if( PQresultStatus(pResult) != PGRES_COPY_OUT )
	{
		_Error_Message(_TL("SQL execution failed"), m_pgConnection);

		PQclear(pResult);

		return( false );
	}

	PQclear(pResult);

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Connection::_Raster_Load(CSG_Grid *pGrid, bool bFirst, bool bBinary)
{
	//-----------------------------------------------------
	char	*Bytes;
	int		nBytes;

	if( (nBytes = PQgetCopyData(m_pgConnection, &Bytes, 0)) <= 0 )
	{
		return( false );
	}

	//-----------------------------------------------------
	CSG_Bytes	Band;

	if( bBinary )
	{
		int	Offset	= bFirst ? 25 : 6;

		if( *((short *)Bytes) > 0 && nBytes > Offset )
		{
			Band.Create((BYTE *)(Bytes + Offset), nBytes - Offset);
		}
	}
	else if( nBytes > 3 )
	{
		Band.fromHexString(Bytes + 3);
	}

	PQfreemem(Bytes);

	return( Band.Get_Count() > 0 && CSG_Grid_OGIS_Converter::from_WKBinary(Band, pGrid) );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Raster_Load(CSG_Data_Manager &Grids, const CSG_String &Table, const CSG_String &Where, const CSG_String &Order, CSG_Table *pInfo)
{
	//-----------------------------------------------------
	bool		bBinary	= true;

	CSG_Table	Info;

	if( !_Raster_Open(Info, Table, Where, Order, bBinary) )
	{
		return( false );
	}

	//-----------------------------------------------------
	for(int iBand=0; iBand<Info.Get_Count() && SG_UI_Process_Get_Okay(); iBand++)
	{
		CSG_Grid	*pGrid	= SG_Create_Grid();

		if( !_Raster_Load(pGrid, iBand == 0, bBinary) )
		{
			delete(pGrid);

			return( false );
		}

		pGrid->Set_Name(Table + " [" + Info[iBand].asString("name") + "]");

		Add_MetaData(*pGrid, Table + CSG_String::Format(":rid=%d", Info[iBand].asInt("rid"))).Add_Child("ID", Info[iBand].asInt("rid"));

		Grids.Add(pGrid);
	}

	//-----------------------------------------------------
	if( pInfo )
	{
		pInfo->Create(Info);
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Raster_Load(CSG_Parameter_Grid_List *pGrids, const CSG_String &Table, const CSG_String &Where, const CSG_String &Order, int OutputType)
{
	//-----------------------------------------------------
	CSG_Data_Manager	Grids;

	CSG_Table	Info;

	if( !Raster_Load(Grids, Table, Where, Order, &Info) )
	{
		return( false );
	}

	//-----------------------------------------------------
	for(size_t jSystem=0, iSystem=Grids.Grid_System_Count()-1; jSystem<Grids.Grid_System_Count(); jSystem++, iSystem--)
	{
		CSG_Grid_Collection	*pSystem	= Grids.Get_Grid_System(iSystem);

		if( OutputType == 0 || (OutputType == 2 && pSystem->Count() == 1) )	// OutputType: 0 == single grid(s), 1 == grid collection, 2 == automatic
		{
			for(size_t iGrid=0; iGrid<pSystem->Count(); iGrid++)
			{
				pGrids->Add_Item(pSystem->Get(iGrid));
			}
		}
		else if( pSystem->Count() > 0 )
		{
			bool	*bAdded	= (bool *)SG_Calloc(pSystem->Count(), sizeof(bool));

			for(size_t nAdded=0; nAdded<pSystem->Count(); )
			{
				CSG_Grids	*pCollection	= SG_Create_Grids();

				pCollection->Get_Attributes_Ptr()->Create(&Info);
				pCollection->Set_Z_Attribute(0);

				CSG_String	rids;

				for(size_t iGrid=0; iGrid<pSystem->Count(); iGrid++)
				{
					if( bAdded[iGrid] )
					{
						continue;
					}

					CSG_Grid	*pGrid	= (CSG_Grid *)pSystem->Get(iGrid);

					CSG_String	rid(pGrid->Get_MetaData_DB().Get_Content("ID"));

					CSG_Table_Record	*pInfo	= Info.Find_Record(0, rid);

					if( pInfo ? pCollection->Add_Grid(       *pInfo, pGrid, true)
							  : pCollection->Add_Grid((double)iGrid, pGrid, true) )
					{
						bAdded[iGrid]	= true;
						nAdded++;

						if( !rid.is_Empty() )
						{
							if( !rids.is_Empty() )
							{
								rids	+= ",";
							}

							rids	+= rid;
						}
					}
				}

				pCollection->Set_Name(Table);
				pCollection->Set_Modified(false);

				Add_MetaData(*pCollection, Table + ":rid=" + rids);

				pGrids->Add_Item(pCollection);
			}

			delete[](bAdded);
		}
	}

	Grids.Delete_All(true);

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Raster_Load(CSG_Grid *pGrid, const CSG_String &Table, const CSG_String &Where)
{
	CSG_Table	Info;

	if( _Raster_Open(Info, Table, Where) && _Raster_Load(pGrid, true) )
	{
		pGrid->Set_Name(Table + " [" + Info[0].asString(1) + "]");

		Add_MetaData(*pGrid, Table + CSG_String::Format(":rid=%d", Info[0].asInt(0))).Add_Child("ID", Info[0].asInt(0));

		SG_Get_Data_Manager().Add(pGrid);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Raster_Save(CSG_Grid *pGrid, int SRID, const CSG_String &Table, const CSG_String &Name)
{
	CSG_Table	Info;

	if( !pGrid || !Table_Load(Info, "raster_columns", "*", CSG_String("r_table_name = '") + Table + "'") || Info.Get_Count() != 1 )
	{
		SG_UI_Msg_Add_Error(CSG_String::Format("[PostGIS] %s (%s)", _TL("could not access table"), SG_T("raster_columns")));

		return( false );
	}

	CSG_String	Geometry	= Info[0].asString("r_raster_column");

	//-----------------------------------------------------
	bool	bBinary	= false;	// binary raster import not (yet??!!) supported

	CSG_String	Select	= "COPY \"" + Table + "\" (\"" + Geometry + "\") FROM STDIN";	if( bBinary )	Select	+= " WITH (FORMAT 'binary')";

	//-----------------------------------------------------
	PGresult	*pResult	= PQexec(m_pgConnection, Select);

	if( PQresultStatus(pResult) != PGRES_COPY_IN )
	{
		_Error_Message(_TL("SQL execution failed"), m_pgConnection);

		PQclear(pResult);

		return( false );
	}

	PQclear(pResult);

	//-----------------------------------------------------
	CSG_Bytes	Band;

	if( CSG_Grid_OGIS_Converter::to_WKBinary(Band, pGrid, SRID) )
	{
		if( bBinary )
		{
			PQputCopyData(m_pgConnection, (const char *)Band.Get_Bytes(), Band.Get_Count());
		}
		else
		{
			CSG_String	hex(Band.toHexString());

			PQputCopyData(m_pgConnection, hex, (int)hex.Length());
		}

		PQputCopyEnd (m_pgConnection, NULL);
	}

	//-----------------------------------------------------
	Table_Load(Info, Table, "rid");	// CSG_String::Format("SELECT currval('%s_rid_seq')", Table.c_str()));

	int	rid	= Info[Info.Get_Count() - 1].asInt(0);

	//-----------------------------------------------------
	Info	= Get_Field_Desc(Table);

	if( !Name.is_Empty() && Info.Get_Count() > 2 && !SG_STR_CMP("varchar", Info[2].asString(1)) )
	{
		if( !Execute(CSG_String::Format("UPDATE \"%s\" SET %s='%s' WHERE rid=%d", Table.c_str(), Info[2].asString(0), Name.c_str(), rid), NULL, true) )
		{
			return( false );
		}
	}

	Add_MetaData(*pGrid, Table + CSG_String::Format(":rid=%d", rid));

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Connection::Rasters_Save(CSG_Grids *pGrids, int SRID, const CSG_String &_Table)
{
	const CSG_String	Geometry	= "raster";

	CSG_String	Table(_Table);	Table.Make_Lower();

	if( !pGrids || Table_Exists(Table) )
	{
		return( false );
	}

	//-----------------------------------------------------
	{
		CSG_String	SQL;

		for(int iField=0; iField<pGrids->Get_Attributes().Get_Field_Count(); iField++)
		{
			SQL	+= CSG_String::Format(", %s ", pGrids->Get_Attributes().Get_Field_Name(iField));
			SQL	+= Get_Type_To_SQL(pGrids->Get_Attributes().Get_Field_Type(iField), pGrids->Get_Attributes().Get_Field_Length(iField));
		}

		if( !Execute("CREATE TABLE \"" + Table + "\" (\"rid\" serial PRIMARY KEY, \"" + Geometry + "\" raster" + SQL + ")") )
		{
			return( false );
		}
	}

	//-----------------------------------------------------
	bool	bBinary	= false;	// binary raster export not (yet??!!) supported

	CSG_String	Copy	= "COPY \"" + Table + "\" (\"" + Geometry + "\") FROM STDIN";	if( bBinary )	Copy	+= " WITH (FORMAT 'binary')";

	//-----------------------------------------------------
	for(int i=0; i<pGrids->Get_Grid_Count(); i++)
	{
		SG_UI_Process_Set_Text(CSG_String::Format("%s: [%d/%d]", _TL("export grid"), i + 1, pGrids->Get_Grid_Count()));

		PGresult	*pResult	= PQexec(m_pgConnection, Copy);

		if( PQresultStatus(pResult) != PGRES_COPY_IN )
		{
			PQclear(pResult);

			_Error_Message(_TL("Raster band export"), m_pgConnection);

			return( false );
		}

		PQclear(pResult);

		//-------------------------------------------------
		CSG_Bytes	Grid;

		if( CSG_Grid_OGIS_Converter::to_WKBinary(Grid, pGrids->Get_Grid_Ptr(i), SRID) )
		{
			if( bBinary )
			{
				PQputCopyData(m_pgConnection, (const char *)Grid.Get_Bytes(), Grid.Get_Count());
			}
			else
			{
				CSG_String	hex(Grid.toHexString());

				PQputCopyData(m_pgConnection, hex, (int)hex.Length());
			}

			PQputCopyEnd (m_pgConnection, NULL);
		}

		//-------------------------------------------------
		CSG_Table	Info;	Table_Load(Info, Table, "rid");	// CSG_String::Format("SELECT currval('%s_rid_seq')", Table.c_str()));

		int	rid	= Info.Get_Count() > 0 ? Info[Info.Get_Count() - 1].asInt(0) : i + 1;

		CSG_String	SQL;

		for(int iField=0; iField<pGrids->Get_Attributes().Get_Field_Count(); iField++)
		{
			if( iField > 0 )
			{
				SQL	+= ", ";
			}

			SQL	+= CSG_String::Format("%s='%s'",
				pGrids->Get_Attributes().Get_Field_Name(iField),
				pGrids->Get_Attributes(i).asString(iField)
			);
		}

		Execute(CSG_String::Format("UPDATE %s SET %s WHERE rid=%d", Table.c_str(), SQL.c_str(), rid));
	}

	//-----------------------------------------------------
	Add_MetaData(*pGrids, Table);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_PG_Connections::CSG_PG_Connections(void)
{
	m_nConnections	= 0;
	m_pConnections	= NULL;

	Create();
}

//---------------------------------------------------------
CSG_PG_Connections::~CSG_PG_Connections(void)
{
	Destroy();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Connections::Create(void)
{
	Destroy();

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Connections::Destroy(void)
{
	if( m_pConnections )
	{
		for(int i=0; i<m_nConnections; i++)
		{
			delete(m_pConnections[i]);
		}

		SG_Free(m_pConnections);

		m_nConnections	= 0;
		m_pConnections	= NULL;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_PG_Connection * CSG_PG_Connections::Add_Connection(const CSG_String &Name, const CSG_String &User, const CSG_String &Password, const CSG_String &Host, int Port)
{
	CSG_PG_Connection	*pConnection	= new CSG_PG_Connection(Host, Port, Name, User, Password);

	if( pConnection )
	{
		if( pConnection->is_Connected() )
		{
			m_pConnections	= (CSG_PG_Connection **)SG_Realloc(m_pConnections, ((size_t)m_nConnections + 1) * sizeof(CSG_PG_Connection *));

			m_pConnections[m_nConnections++]	= pConnection;
		}
		else
		{
			delete(pConnection);

			pConnection	= NULL;
		}
	}

	return( pConnection );
}

//---------------------------------------------------------
CSG_PG_Connection *  CSG_PG_Connections::Get_Connection(const CSG_String &Name)
{
	for(int i=0; i<m_nConnections; i++)
	{
		if( Name.Cmp(m_pConnections[i]->Get_Connection()) == 0 )
		{
			return( m_pConnections[i] );
		}
	}

	return( NULL );
}

//---------------------------------------------------------
bool CSG_PG_Connections::Del_Connection(int Index, bool bCommit)
{
	if( Index >= 0 && Index < m_nConnections )
	{
		if( m_pConnections[Index]->is_Transaction() )
		{
			if( bCommit )
			{
				m_pConnections[Index]->Commit();
			}
			else
			{
				m_pConnections[Index]->Rollback();
			}
		}

		delete(m_pConnections[Index]);

		for(m_nConnections--; Index<m_nConnections; Index++)
		{
			m_pConnections[Index]	= m_pConnections[Index + 1];
		}

		m_pConnections	= (CSG_PG_Connection **)SG_Realloc(m_pConnections, m_nConnections * sizeof(CSG_PG_Connection *));

		return( true );
	}

	return( false );
}

bool CSG_PG_Connections::Del_Connection(const CSG_String &Name, bool bCommit)
{
	for(int i=0; i<m_nConnections; i++)
	{
		if( Name.Cmp(m_pConnections[i]->Get_Connection()) == 0 )
		{
			return( Del_Connection(i, bCommit) );
		}
	}

	return( false );
}

bool CSG_PG_Connections::Del_Connection(CSG_PG_Connection *pConnection, bool bCommit)
{
	return( !pConnection ? false : Del_Connection(pConnection->Get_Connection(), bCommit) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Strings CSG_PG_Connections::Get_Servers(void)
{
	CSG_Strings	Servers;

	return( Servers );
}

//---------------------------------------------------------
int CSG_PG_Connections::Get_Servers(CSG_Strings &Servers)
{
	Servers	= Get_Servers();

	return( Servers.Get_Count() );
}

//---------------------------------------------------------
int CSG_PG_Connections::Get_Servers(CSG_String &Servers)
{
	CSG_Strings		s	= Get_Servers();

	for(int i=0; i<s.Get_Count(); i++)
	{
		Servers	+= s[i] + "|";
	}

	return( s.Get_Count() );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Strings CSG_PG_Connections::Get_Connections(void)
{
	CSG_Strings		Connections;

	for(int i=0; i<m_nConnections; i++)
	{
		Connections.Add(m_pConnections[i]->Get_Connection());
	}

	return( Connections );
}

//---------------------------------------------------------
int CSG_PG_Connections::Get_Connections(CSG_String &Connections)
{
	Connections.Clear();

	for(int i=0; i<m_nConnections; i++)
	{
		Connections	+= m_pConnections[i]->Get_Connection() + "|";
	}

	return( m_nConnections );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_PG_Tool::CSG_PG_Tool(void)
{
	Parameters.Add_String("", "PG_HOST"   , _TL("Host"       ), _TL(""), "127.0.0.1"    )->Set_UseInGUI(false);
	Parameters.Add_Int   ("", "PG_PORT"   , _TL("Port"       ), _TL(""), 5432, 0, true  )->Set_UseInGUI(false);
	Parameters.Add_String("", "PG_NAME"   , _TL("Database"   ), _TL(""), ""             )->Set_UseInGUI(false);
	Parameters.Add_String("", "PG_USER"   , _TL("User"       ), _TL(""), ""             )->Set_UseInGUI(false);
	Parameters.Add_String("", "PG_PWD"    , _TL("Password"   ), _TL(""), "", false, true)->Set_UseInGUI(false);

	Parameters.Add_Choice("", "CONNECTION", _TL("Connections"), _TL(""), ""             )->Set_UseInCMD(false);

	m_pConnection = NULL;
}

//---------------------------------------------------------
bool CSG_PG_Tool::On_Before_Execution(void)
{
	if( !has_GUI() )
	{
		m_pConnection = SG_PG_Get_Connection_Manager().Add_Connection(
			Parameters("PG_NAME")->asString(),
			Parameters("PG_USER")->asString(),
			Parameters("PG_PWD" )->asString(),
			Parameters("PG_HOST")->asString(),
			Parameters("PG_PORT")->asInt   ()
		);

		if( m_pConnection )
		{
			On_Connection_Changed(&Parameters);
		}
	}
	else
	{
		CSG_String Connections;

		int nConnections = SG_PG_Get_Connection_Manager().Get_Connections(Connections);

		if( nConnections <= 0 )
		{
			Message_Dlg(
				_TL("No PostgreSQL connection available!"),
				_TL("PostgreSQL Database Connection Error")
			);

			return( false );
		}

		CSG_PG_Connection *pConnection = NULL;

		if( nConnections == 1 || !(pConnection = SG_PG_Get_Connection_Manager().Get_Connection(Parameters("CONNECTION")->asString())) )
		{
			pConnection = SG_PG_Get_Connection_Manager().Get_Connection(0);
		}

		if( m_pConnection != pConnection )
		{
			m_pConnection = pConnection;

			On_Connection_Changed(&Parameters);
		}

		Parameters("CONNECTION")->asChoice()->Set_Items(Connections);
		Parameters("CONNECTION")->Set_Enabled(nConnections > 1);
		Parameters("CONNECTION")->Set_Value(m_pConnection->Get_Connection());
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Tool::On_After_Execution(void)
{
	if( !has_GUI() )
	{
		SG_PG_Get_Connection_Manager().Del_Connection(m_pConnection, true);
	}

	return( true );
}

//---------------------------------------------------------
int CSG_PG_Tool::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( has_GUI() )
	{
		if(	pParameter->Cmp_Identifier("CRS_EPSG_GEOGCS")
		||  pParameter->Cmp_Identifier("CRS_EPSG_PROJCS") )
		{
			int EPSG;

			if( pParameter->asChoice()->Get_Data(EPSG) )
			{
				pParameters->Get_Parameter("CRS_EPSG")->Set_Value(EPSG);
			}
		}

		//-------------------------------------------------
		if( pParameter->Cmp_Identifier("CONNECTION") )
		{
			CSG_PG_Connection *pConnection = SG_PG_Get_Connection_Manager().Get_Connection(pParameter->asString());

			if( m_pConnection != pConnection )
			{
				m_pConnection = pConnection;

				On_Connection_Changed(pParameters);
			}
		}
	}

	//-----------------------------------------------------
	return( CSG_Tool::On_Parameter_Changed(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Tool::Add_SRID_Picker(CSG_Parameters *pParameters)
{
	if( !pParameters )
	{
		pParameters = &Parameters;
	}

	if( pParameters->Get_Parameter("CRS_EPSG") )
	{
		return( false ); // don't add twice ...
	}

	pParameters->Add_Int("", "CRS_EPSG", _TL("EPSG Code"), _TL(""), -1, -1, true);

	if( has_GUI() )
	{
		pParameters->Add_Choice(
			"CRS_EPSG", "CRS_EPSG_GEOGCS", _TL("Geographic Coordinate Systems"), _TL(""),
			SG_Get_Projections().Get_Names_List(SG_PROJ_TYPE_CS_Geographic)
		);

		pParameters->Add_Choice(
			"CRS_EPSG", "CRS_EPSG_PROJCS", _TL("Projected Coordinate Systems" ), _TL(""),
			SG_Get_Projections().Get_Names_List(SG_PROJ_TYPE_CS_Projected)
		);
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_PG_Tool::Set_SRID_Picker_Enabled(CSG_Parameters *pParameters, bool bEnable)
{
	CSG_Parameter *pParameter = pParameters ? pParameters->Get_Parameter("CRS_EPSG") : NULL;

	if( pParameter )
	{
		pParameter->Set_Enabled(bEnable);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_PG_Tool::Set_SRID(CSG_Parameters *pParameters, int SRID)
{
	CSG_Parameter *pParameter = pParameters ? pParameters->Get_Parameter("CRS_EPSG") : NULL;

	if( pParameter )
	{
		CSG_Projection Projection;

		if( SG_Get_Projections().Get_Projection(Projection, SRID) )
		{
			pParameter->Set_Value(SRID);

			return( true );
		}
	}

	return( false );
}

//---------------------------------------------------------
int CSG_PG_Tool::Get_SRID(CSG_Parameters *pParameters)
{
	if( !pParameters )
	{
		pParameters = &Parameters;
	}

	CSG_Parameter *pParameter = pParameters->Get_Parameter("CRS_EPSG");

	return( pParameter ? pParameter->asInt() : -1 );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_PG_Tool::Set_Constraints(CSG_Parameters *pParameters, const CSG_String &Identifier)
{
	CSG_Parameter *pParent = pParameters ? pParameters->Get_Parameter(Identifier) : NULL;

	if( !pParent || (pParent->Get_Type() != PARAMETER_TYPE_Table && pParent->Get_Type() != PARAMETER_TYPE_Shapes) )
	{
		return( false );
	}

	pParameters->Add_Table_Fields(pParent, Identifier + "_PK", _TL("Primary Key"), _TL(""));
	pParameters->Add_Table_Fields(pParent, Identifier + "_NN", _TL("Not Null"   ), _TL(""));
	pParameters->Add_Table_Fields(pParent, Identifier + "_UQ", _TL("Unique"     ), _TL(""));

	return( true );
}

//---------------------------------------------------------
CSG_Buffer CSG_PG_Tool::Get_Constraints(CSG_Parameters *pParameters, const CSG_String &Identifier)
{
	CSG_Buffer Flags;

	CSG_Parameter *pParent = pParameters ? pParameters->Get_Parameter(Identifier) : NULL;

	if( !pParent || (pParent->Get_Type() != PARAMETER_TYPE_Table && pParent->Get_Type() != PARAMETER_TYPE_Shapes) )
	{
		return( Flags );
	}

	CSG_Parameter *pFields;

	Flags.Set_Size(((CSG_Table *)pParent->asDataObject())->Get_Field_Count());

	memset(Flags.Get_Data(), 0, Flags.Get_Size());

	if( (pFields = pParameters->Get_Parameter(Identifier + "_PK")) != NULL && pFields->Get_Type() == PARAMETER_TYPE_Table_Fields )
	{
		for(int i=0, Index; i<pFields->asTableFields()->Get_Count(); i++)
		{
			if( (Index = pFields->asTableFields()->Get_Index(i)) >= 0 && Index < (int)Flags.Get_Size() )
			{
				Flags.Get_Data()[Index]	|= SG_PG_PRIMARY_KEY;
			}
		}
	}

	if( (pFields = pParameters->Get_Parameter(Identifier + "_NN")) != NULL && pFields->Get_Type() == PARAMETER_TYPE_Table_Fields )
	{
		for(int i=0, Index; i<pFields->asTableFields()->Get_Count(); i++)
		{
			if( (Index = pFields->asTableFields()->Get_Index(i)) >= 0 && Index < (int)Flags.Get_Size() )
			{
				Flags.Get_Data()[Index]	|= SG_PG_NOT_NULL;
			}
		}
	}

	if( (pFields = pParameters->Get_Parameter(Identifier + "_UQ")) != NULL && pFields->Get_Type() == PARAMETER_TYPE_Table_Fields )
	{
		for(int i=0, Index; i<pFields->asTableFields()->Get_Count(); i++)
		{
			if( (Index = pFields->asTableFields()->Get_Index(i)) >= 0 && Index < (int)Flags.Get_Size() )
			{
				Flags.Get_Data()[Index]	|= SG_PG_UNIQUE;
			}
		}
	}

	return( Flags );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
