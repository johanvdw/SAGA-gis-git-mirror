
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
//                     datetime.cpp                      //
//                                                       //
//          Copyright (C) 2015 by Olaf Conrad            //
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
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <time.h>

#include <wx/datetime.h>

#include "datetime.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_TimeSpan::Format(const CSG_String &Format)	const
{
	wxTimeSpan	ts((wxLongLong(m_span)));

	wxString	s	= !Format.is_Empty() ? ts.Format(Format.c_str()) : ts.Format(wxDefaultTimeSpanFormat);

	return( CSG_String(&s) );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_DateTime::CSG_DateTime(void)
{
	m_pDateTime	= new wxDateTime(wxDateTime::Now());
}

//---------------------------------------------------------
CSG_DateTime::CSG_DateTime(const CSG_DateTime &DateTime)
{
	m_pDateTime	= new wxDateTime(*DateTime.m_pDateTime);
}

//---------------------------------------------------------
CSG_DateTime::CSG_DateTime(double JDN)
{
	m_pDateTime	= new wxDateTime(JDN);
}

//---------------------------------------------------------
CSG_DateTime::CSG_DateTime(const CSG_String &ISODate)
{
	m_pDateTime	= new wxDateTime(wxDateTime::Now());

	Set(ISODate);
}

//---------------------------------------------------------
CSG_DateTime::CSG_DateTime(TSG_DateTime Hour, TSG_DateTime Minute, TSG_DateTime Second, TSG_DateTime Millisec)
{
	m_pDateTime	= new wxDateTime(Hour, Minute, Second, Millisec);
}

//---------------------------------------------------------
CSG_DateTime::CSG_DateTime(TSG_DateTime Day, Month Month, int Year, TSG_DateTime Hour, TSG_DateTime Minute, TSG_DateTime Second, TSG_DateTime Millisec)
{
	m_pDateTime	= new wxDateTime(Day, (wxDateTime::Month)Month, Year, Hour, Minute, Second, Millisec);
}

//---------------------------------------------------------
CSG_DateTime::~CSG_DateTime(void)
{
	delete(m_pDateTime);
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set(const CSG_DateTime &DateTime)
{
	m_pDateTime->Set(DateTime.m_pDateTime->GetTm());

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set(double JDN)
{
	m_pDateTime->Set(JDN);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set(const CSG_String &ISODate)
{
	// 0123456789
	// YYYY-MM-DD

	if( ISODate.Length() >= 10 && ISODate[4] == '-' && ISODate[7] == '-' )
	{
		int	y	= ISODate.Left (   4).asInt();
		int	m	= ISODate.Mid  (5, 2).asInt();
		int	d	= ISODate.Right(   2).asInt();

		Set((TSG_DateTime)d, (Month)(m - 1), y);
	}

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set(TSG_DateTime Hour, TSG_DateTime Minute, TSG_DateTime Second, TSG_DateTime Millisec)
{
	m_pDateTime->Set(Hour, Minute, Second, Millisec);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set(TSG_DateTime Day, Month Month, int Year, TSG_DateTime Hour, TSG_DateTime Minute, TSG_DateTime Second, TSG_DateTime Millisec)
{
	m_pDateTime->Set(Day, (wxDateTime::Month)Month, Year, Hour, Minute, Second, Millisec);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Millisecond(unsigned short Value)
{
	m_pDateTime->SetMillisecond(Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Second(unsigned short Value)
{
	m_pDateTime->SetSecond(Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Minute(unsigned short Value)
{
	m_pDateTime->SetMinute(Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Hour(unsigned short Value)
{
	m_pDateTime->SetHour(Value);

	return( *this );
}

CSG_DateTime & CSG_DateTime::Set_Hour(double Value)
{
	m_pDateTime->ResetTime();

	Value	= fmod(Value, 24.0); if( Value < 0.0 ) Value += 24.0;

	m_pDateTime->SetHour       ((wxDateTime::wxDateTime_t)Value);	Value	= (Value - (wxDateTime::wxDateTime_t)Value) *   60.0;
	m_pDateTime->SetMinute     ((wxDateTime::wxDateTime_t)Value);	Value	= (Value - (wxDateTime::wxDateTime_t)Value) *   60.0;
	m_pDateTime->SetSecond     ((wxDateTime::wxDateTime_t)Value);	Value	= (Value - (wxDateTime::wxDateTime_t)Value) * 1000.0;
	m_pDateTime->SetMillisecond((wxDateTime::wxDateTime_t)Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Day(unsigned short Value)
{
	m_pDateTime->SetDay(Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Month(unsigned short Value)
{
	m_pDateTime->SetMonth((wxDateTime::Month)Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Year(int Value)
{
	m_pDateTime->SetYear(Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_DayOfYear(unsigned short Value)
{
	m_pDateTime->SetToYearDay(Value);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_To_Current(void)
{
	m_pDateTime->SetToCurrent();

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Reset_Time(void)
{
	m_pDateTime->ResetTime();

	return( *this );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_DateTime CSG_DateTime::From_UTC(bool noDST) const
{
	return( CSG_DateTime(m_pDateTime->FromUTC(noDST).GetJDN()) );

//	CSG_DateTime dt; dt.m_pDateTime->Set(m_pDateTime->FromUTC(noDST).GetTm()); return( dt );
}

//---------------------------------------------------------
CSG_DateTime CSG_DateTime::To_UTC(bool noDST) const
{
	return( CSG_DateTime(m_pDateTime->ToUTC(noDST).GetJDN()) );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Make_UTC(bool noDST)
{
	m_pDateTime->MakeUTC(noDST);

	return( *this );
}

//---------------------------------------------------------
bool CSG_DateTime::is_DST(void)	const
{
	return( m_pDateTime->IsDST() != 0 );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
sLong CSG_DateTime::Get_Value(void)	const
{
	return( m_pDateTime->GetValue().GetValue() );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_DateTime CSG_DateTime::Add(const CSG_TimeSpan &TimeSpan)	const
{
	CSG_DateTime	dt(*this);

	return( dt.Add(TimeSpan) );
}

CSG_DateTime & CSG_DateTime::Add(const CSG_TimeSpan &TimeSpan)
{
	m_pDateTime->Add(wxTimeSpan((wxLongLong)TimeSpan.m_span));

	return( *this);
}

//---------------------------------------------------------
CSG_DateTime CSG_DateTime::Subtract(const CSG_TimeSpan &TimeSpan)	const
{
	CSG_DateTime	dt(*this);

	return( dt.Subtract(TimeSpan) );
}

CSG_DateTime & CSG_DateTime::Subtract(const CSG_TimeSpan &TimeSpan)
{
	m_pDateTime->Subtract(wxTimeSpan((wxLongLong)TimeSpan.m_span));

	return( *this);
}

//---------------------------------------------------------
CSG_TimeSpan CSG_DateTime::Subtract(const CSG_DateTime &DateTime)	const
{
	CSG_TimeSpan	Span((sLong)m_pDateTime->Subtract(*DateTime.m_pDateTime).GetValue().GetValue());

	return( Span );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_DateTime::is_Valid(void)	const
{
	return( m_pDateTime->IsValid() );
}

//---------------------------------------------------------
unsigned short CSG_DateTime::Get_Millisecond(void)	const	{	return( m_pDateTime->GetMillisecond() );	}
unsigned short CSG_DateTime::Get_Second     (void)	const	{	return( m_pDateTime->GetSecond     () );	}
unsigned short CSG_DateTime::Get_Minute     (void)	const	{	return( m_pDateTime->GetMinute     () );	}
unsigned short CSG_DateTime::Get_Hour       (void)	const	{	return( m_pDateTime->GetHour       () );	}
unsigned short CSG_DateTime::Get_Day        (void)	const	{	return( m_pDateTime->GetDay        () );	}

CSG_DateTime::Month CSG_DateTime::Get_Month(void)	const
{
	return( (Month)m_pDateTime->GetMonth() );
}

int CSG_DateTime::Get_Year(void)	const
{
	return( m_pDateTime->GetYear() );
}

//---------------------------------------------------------
unsigned short CSG_DateTime::Get_DayOfYear(void)	const
{
	return( m_pDateTime->GetDayOfYear() );
}

CSG_DateTime::WeekDay CSG_DateTime::Get_WeekDay(void)	const
{
	return( (WeekDay)m_pDateTime->GetWeekDay() );
}

CSG_DateTime::TSG_DateTime CSG_DateTime::Get_WeekOfMonth(void)	const
{
	return( (TSG_DateTime)m_pDateTime->GetWeekOfMonth() );
}

CSG_DateTime::TSG_DateTime CSG_DateTime::Get_WeekOfYear(void)	const
{
	return( (TSG_DateTime)m_pDateTime->GetWeekOfYear() );
}

//---------------------------------------------------------
double CSG_DateTime::Get_JDN(void)	const
{
	return( m_pDateTime->GetJDN() );
}

double CSG_DateTime::Get_MJD(void)	const
{
	return( m_pDateTime->GetMJD() );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_DateTime::is_EarlierThan    (const CSG_DateTime &DateTime)	const	{	return( m_pDateTime->IsEarlierThan(*DateTime.m_pDateTime) );	}
bool CSG_DateTime::is_EqualTo        (const CSG_DateTime &DateTime)	const	{	return( m_pDateTime->IsEqualTo    (*DateTime.m_pDateTime) );	}
bool CSG_DateTime::is_LaterThan      (const CSG_DateTime &DateTime)	const	{	return( m_pDateTime->IsLaterThan  (*DateTime.m_pDateTime) );	}

//---------------------------------------------------------
bool CSG_DateTime::is_SameDate       (const CSG_DateTime &DateTime)	const	{	return( m_pDateTime->IsSameDate   (*DateTime.m_pDateTime) );	}
bool CSG_DateTime::is_SameTime       (const CSG_DateTime &DateTime)	const	{	return( m_pDateTime->IsSameTime   (*DateTime.m_pDateTime) );	}

//---------------------------------------------------------
bool CSG_DateTime::is_Between        (const CSG_DateTime &t1, const CSG_DateTime &t2)	const	{	return( m_pDateTime->IsBetween        (*t1.m_pDateTime, *t2.m_pDateTime) );	}
bool CSG_DateTime::is_StrictlyBetween(const CSG_DateTime &t1, const CSG_DateTime &t2)	const	{	return( m_pDateTime->IsStrictlyBetween(*t1.m_pDateTime, *t2.m_pDateTime) );	}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_DateTime::Format(const CSG_String &Format)	const
{
	wxString	s(m_pDateTime->Format(Format.c_str()));	CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Format_Date(void)	const
{
	wxString	s(m_pDateTime->FormatDate());			CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Format_Time(void)	const
{
	wxString	s(m_pDateTime->FormatTime());			CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Format_ISODate(void)	const
{
	wxString	s(m_pDateTime->FormatISODate());		CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Format_ISOTime(void)	const
{
	wxString	s(m_pDateTime->FormatISOTime());		CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Format_ISOCombined(char sep)	const
{
	wxString	s(m_pDateTime->FormatISOCombined(sep));	CSG_String _s(&s); return( _s );
}

//---------------------------------------------------------
bool CSG_DateTime::Parse_Date(const CSG_String &date)
{
	wxString::const_iterator	end;	return( m_pDateTime->ParseDate(date.c_str(), &end) );
}

bool CSG_DateTime::Parse_DateTime(const CSG_String &datetime)
{
	wxString::const_iterator	end;	return( m_pDateTime->ParseDateTime(datetime.c_str(), &end) );
}

bool CSG_DateTime::Parse_Format(const CSG_String &date, const CSG_String &format, const CSG_DateTime &dateDef)
{
	wxString::const_iterator	end;	return( m_pDateTime->ParseFormat(date.c_str(), format.c_str(), *dateDef.m_pDateTime, &end) );
}

bool CSG_DateTime::Parse_Format(const CSG_String &date, const CSG_String &format)
{
	wxString::const_iterator	end;	return( m_pDateTime->ParseFormat(date.c_str(), format.c_str(), &end) );
}

bool CSG_DateTime::Parse_Format(const CSG_String &date)
{
	wxString::const_iterator	end;	return( m_pDateTime->ParseFormat(date.c_str(), &end) );
}

//---------------------------------------------------------
bool CSG_DateTime::Parse_ISOCombined(const CSG_String &date, char sep)
{
	return( m_pDateTime->ParseISOCombined(date.c_str(), sep) );
}

bool CSG_DateTime::Parse_ISODate(const CSG_String &date)
{
	return( m_pDateTime->ParseISODate(date.c_str()) );
}

bool CSG_DateTime::Parse_ISOTime(const CSG_String &date)
{
	return( m_pDateTime->ParseISOTime(date.c_str()) );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_DateTime::TSG_DateTime CSG_DateTime::Get_Current_Day(void)
{
	CSG_DateTime Now;
	
	Now.Set_To_Current();
	
	return( Now.Get_Day() );
}

CSG_DateTime::Month CSG_DateTime::Get_Current_Month(void)
{
	return( (Month)wxDateTime::GetCurrentMonth() );
}

int CSG_DateTime::Get_Current_Year(void)
{
	return( wxDateTime::GetCurrentYear() );
}

//---------------------------------------------------------
CSG_String CSG_DateTime::Get_MonthName(Month month, NameFlags flags)
{
	wxString	s(wxDateTime::GetMonthName         ((wxDateTime::Month)month    , (wxDateTime::NameFlags)flags));	CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Get_EnglishMonthName(Month month, NameFlags flags)
{
	wxString	s(wxDateTime::GetEnglishMonthName  ((wxDateTime::Month)month    , (wxDateTime::NameFlags)flags));	CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Get_WeekDayName(WeekDay weekday, NameFlags flags)
{
	wxString	s(wxDateTime::GetWeekDayName       ((wxDateTime::WeekDay)weekday, (wxDateTime::NameFlags)flags));	CSG_String _s(&s); return( _s );
}

CSG_String CSG_DateTime::Get_EnglishWeekDayName(WeekDay weekday, NameFlags flags)
{
	wxString	s(wxDateTime::GetEnglishWeekDayName((wxDateTime::WeekDay)weekday, (wxDateTime::NameFlags)flags));	CSG_String _s(&s); return( _s );
}

//---------------------------------------------------------
CSG_DateTime::TSG_DateTime CSG_DateTime::Get_NumberOfDays(int year)
{
	return( (TSG_DateTime)wxDateTime::GetNumberOfDays(year) );
}

CSG_DateTime::TSG_DateTime CSG_DateTime::Get_NumberOfDays(Month month, int year)
{
	return( (TSG_DateTime)wxDateTime::GetNumberOfDays((wxDateTime::Month)month, year) );
}

//---------------------------------------------------------
bool CSG_DateTime::is_LeapYear(int year)
{
	return( wxDateTime::IsLeapYear(year) );
}

//---------------------------------------------------------
CSG_DateTime CSG_DateTime::Now(void)
{
	CSG_DateTime	DateTime;	*DateTime.m_pDateTime	= wxDateTime::Now();

	return( DateTime );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Unix_Time(sLong Seconds)
{
	time_t		tUnix	= Seconds;
	struct tm	t;

	#ifdef _SAGA_LINUX
		t	= *gmtime(&tUnix);
	#else
		gmtime_s(&t, &tUnix);
	#endif

	Set(t.tm_mday, (CSG_DateTime::Month)(t.tm_mon), t.tm_year + 1900, t.tm_hour, t.tm_min, t.tm_sec);

	return( *this );
}

//---------------------------------------------------------
CSG_DateTime & CSG_DateTime::Set_Hours_AD(int Hours)
{
	long	d, n;
	double	h;

	h		= 1721424.0 + (Hours - 12.0) / 24.0;
	d		= (long)h;		// Truncate to integral day
	h		= h - d + 0.5;	// Fractional part of calendar day
	if( h >= 1.0 )			// Is it really the next calendar day?
	{
		h--;
		d++;
	}

	int	day, mon, year, hour, min, sec;

	h		= 24.0 * (h);
	hour	= (int)h;
	h		= 60.0 * (h - hour);
	min		= (int)h;
	h		= 60.0 * (h - min);
	sec		= (int)h;

	d		= d + 68569;
	n		= 4 * d / 146097;
	d		= d - (146097 * n + 3) / 4;
	year	= 4000 * (d + 1) / 1461001;
	d		= d - 1461 * year / 4 + 31;	// 1461 = 365.25 * 4
	mon		= 80 * d / 2447;
	day		= d - 2447 * mon / 80;
	d		= mon / 11;
	mon		= mon + 2 - 12 * d;
	year	= 100 * (n - 49) + year + d;

	Set(day, (CSG_DateTime::Month)(mon - 1), year, hour, min, sec);

	return( *this );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_DateTime::Get_Month_Choices(void)
{
	CSG_String	Choices;

	Choices.Printf("%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|",
		_TL("January"), _TL("February"), _TL("March"    ), _TL("April"  ), _TL("May"     ), _TL("June"    ),
		_TL("July"   ), _TL("August"  ), _TL("September"), _TL("October"), _TL("November"), _TL("December")
	);

	return( Choices );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
/**
  * Expects 'Value' to be the Julian Day Number.
*/
//---------------------------------------------------------
bool		SG_JulianDayNumber_To_Date(double JDN, int &y, int &m, int &d)
{
	double Z = floor(JDN + 0.5);
	double F = JDN + 0.5 - Z;
	double a = floor((Z - 1867216.25) / 36524.25);
	double A = Z + 1 + a - floor(a / 4);
	double B = A + 1524;
	double C = floor((B - 122.1) / 365.25);
	double D = floor(365.25 * C);
	double E = floor((B - D) / 30.6001);

	d = int(B - D - floor(30.6001 * E) + F);

	if( E <= 13 )
	{
		m = E -  1; y = C - 4716;
	}
	else
	{
		m = E - 13; y = C - 4715;
	}

	return( true );
}

//---------------------------------------------------------
/**
  * Calculates Gregorian date from Julian Day Number and returns it as ISO 8601 string (yyyy-mm-dd).
*/
//---------------------------------------------------------
CSG_String		SG_JulianDayNumber_To_Date(double JDN)
{
	CSG_String Date; int y, m, d;

	if( SG_JulianDayNumber_To_Date(JDN, y, m, d) )
	{
		Date.Printf("%04d-%02d-%02d", y, m, d);	// yyyy-mm-dd (ISO 8601)
	}

	return( Date );
}

//---------------------------------------------------------
/**
  * Calculates Gregorian date from Julian Day Number and returns it as ISO 8601 string (yyyy-mm-dd).
*/
//---------------------------------------------------------
CSG_String		SG_JulianDayNumber_To_Date(int JDN)
{
	return( SG_JulianDayNumber_To_Date(JDN + 0.5) );
}

//---------------------------------------------------------
/**
  * Returns the Julian Day Number for the requested Gregorian
  * date (year, month, day). Month should be in the range of
  * 1 (= january) to 12 (= december), day in the range of 1
  * to 31.
*/
//---------------------------------------------------------
double			SG_Date_To_JulianDayNumber(int Year, int Month, int Day)
{
	if( Month <= 2 ) { Year--; Month += 12; }

	return( floor(365.25 * (Year + 4716)) + floor(30.6001 * (Month + 1)) + Day + 2. - floor(Year / 100) + floor(Year / 400) - 1524.5 );
}

//---------------------------------------------------------
/**
  * Returns the Julian Day Number for the requested date.
  * Date string is best provided be given in ISO format
  * (YYYY-MM-DD), or alternatively as YY-MM-DD or DD-MM-YYYY.
*/
//---------------------------------------------------------
double			SG_Date_To_JulianDayNumber(const CSG_String &Date)
{
	if( Date.Length() >= 10 )
	{
		bool BC = Date[0] == '-';

		CSG_Strings ymd(SG_String_Tokenize(BC ? Date.AfterFirst('-') : Date, "-./"));

		if( ymd.Get_Count() >= 3 )
		{
			bool inv = ymd[2].Length() == 4;

			int y = ymd[inv ? 2 : 0].asInt(); if( BC ) { y = -y; }
			int m = ymd[          1].asInt(); if( m < 1 ) { m = 1; } else if( m > 12 ) { m = 12; }
			int d = ymd[inv ? 0 : 2].asInt(); if( d < 1 ) { d = 1; } else if( d > 31 ) { d = 31; }

			return( SG_Date_To_JulianDayNumber(y, m, d) );
		}
	}

	return( 0. );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
/**
  * Returns the day number (starting with zero for the first
  * of January) for the mid of the requested month (January = 0).
*/
//---------------------------------------------------------
int SG_Get_Day_MidOfMonth(int Month, bool bLeapYear)
{
	static const int	MidOfMonth[12]	=
	// JAN  FEB  MAR  APR  MAY  JUN  JUL  AUG  SEP  OCT  NOV  DEC
	//	31,  28,  31,  30,  31,  30,  31,  31,  30,  31,  30,  31
	//	 0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334
	{	15,  45,  74, 105, 135, 166, 196, 227, 258, 288, 319, 349	};

	int	Day;

	if( Month < 0 )
	{
		Month	= (Month % 12) + 12;
		Day		= MidOfMonth[Month] - 365;
	}
	else if( Month >= 12 )
	{
		Month	= (Month % 12);
		Day		= MidOfMonth[Month] + 365;
	}
	else
	{
		Day		= MidOfMonth[Month];
	}

	if( bLeapYear && Month > 1 )
	{
		Day++;
	}

	return( Day );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "mat_tools.h"

//---------------------------------------------------------
/**
  * Calculates the Sun's position for given date and time
  * as declination and right ascension angles (radians).
  * Solar coordinates according to
  * Jean Meeus: Astronomical Algorithms, accuracy of 0.01 degree
*/
//---------------------------------------------------------
bool SG_Get_Sun_Position(double JDN, double &RA, double &Dec)
{
	double T = (JDN - 2451545.) / 36525.; // Number of Julian centuries since 2000-01-01 at 12 UT (JDN = 2451545.0)

	double M =  357.52910 + 35999.05030 * T - 0.0001559 * T*T - 0.00000048 * T*T*T; // mean anomaly

	M *= M_DEG_TO_RAD;

	double L = (280.46645 + 36000.76983 * T + 0.0003032 * T*T)                      // mean longitude
	         + ((1.914600 - 0.004817 * T - 0.000014  * T*T) * sin(M)
	         +  (0.019993 - 0.000101 * T) * sin(2. * M) + 0.000290 * sin(3. * M));  // true longitude

	L *= M_DEG_TO_RAD;

	//-----------------------------------------------------
	// convert ecliptic longitude to right ascension RA and declination delta

	static const double	Ecliptic_Obliquity = M_DEG_TO_RAD * 23.43929111;

	double X = cos(L);
	double Y = cos(Ecliptic_Obliquity) * sin(L);
	double Z = sin(Ecliptic_Obliquity) * sin(L);
	double R = sqrt(1. - Z*Z); 

	Dec	= atan2(Z, R);
	RA	= 2. * atan2(Y, (X + R));

	return( true );
}

//---------------------------------------------------------
bool SG_Get_Sun_Position(const CSG_DateTime &Time, double &RA, double &Dec)
{
	return( SG_Get_Sun_Position(Time.Get_JDN(), RA, Dec) );
}

//---------------------------------------------------------
/**
  * Calculates the Sun's position for given date and time
  * and position on Earth as given by longitude and latitude (radians).
  * Height and Azimuth are calculated as radians.
  * Returns true if Sun is above horizon.
*/
//---------------------------------------------------------
bool SG_Get_Sun_Position(double JDN, double Longitude, double Latitude, double &Height, double &Azimuth, bool bRefraction)
{
	//-----------------------------------------------------
	// 1. Get right ascension RA and declination delta

	double RA, Dec; SG_Get_Sun_Position(JDN, RA, Dec);

	//-----------------------------------------------------
	// 2. compute sidereal time (radians) at Greenwich local sidereal time at longitude (radians)

	double T     = (JDN - 2451545.) / 36525.;

	double Theta = Longitude + M_DEG_TO_RAD * (280.46061837 + 360.98564736629 * (JDN - 2451545.) + T*T * (0.000387933 - T / 38710000.));

	double HA    = Theta - RA; // compute local hour angle (radians)

	//-----------------------------------------------------
	// 3. convert (HA, Dec) to horizon coordinates (height, azimuth) of the observer

	Height	= asin ( sin(Latitude) * sin(Dec) + cos(Latitude) * cos(Dec) * cos(HA));
	Azimuth	= atan2(-sin(HA) * cos(Dec), cos(Latitude) * sin(Dec) - sin(Latitude) * cos(Dec) * cos(HA));
//	Azimuth	= atan2(-sin(HA), cos(Latitude) * tan(Dec) - sin(Latitude) * cos(HA)); // previous formula gives same result but is better because of division by zero effects...

	if( bRefraction )
	{
		double Refraction = SG_Get_Sun_Refraction(Height, true);

		if( Refraction >= 0. )
		{
			Height += Refraction;
		}
	}

	return( Height > 0. );
}

//---------------------------------------------------------
bool SG_Get_Sun_Position(const CSG_DateTime &Time, double Longitude, double Latitude, double &Height, double &Azimuth, bool bRefraction)
{
	return( SG_Get_Sun_Position(Time.Get_JDN(), Longitude, Latitude, Height, Azimuth, bRefraction) );
}

//---------------------------------------------------------
bool SG_Get_Sun_Position(double JDN, double Longitude, double Latitude, CSG_Vector &Position, bool bRefraction)
{
	Position.Create(2); return( SG_Get_Sun_Position(JDN, Longitude, Latitude, Position[0], Position[1], bRefraction) );
}

//---------------------------------------------------------
bool SG_Get_Sun_Position(const CSG_DateTime &Time, double Longitude, double Latitude, CSG_Vector &Position, bool bRefraction)
{
	Position.Create(2); return( SG_Get_Sun_Position(Time.Get_JDN(), Longitude, Latitude, Position[0], Position[1], bRefraction) );
}

//---------------------------------------------------------
/**
* Refraction correction for U.S. Standard Atmosphere.
* If bRadians is true input Height is expected to be
* given in radians and returned height will be in
* radians too.
*/
//---------------------------------------------------------
double SG_Get_Sun_Refraction(double Height, bool bRadians)
{
	// 3.51823 = 1013.25mb / 288K

	double z = bRadians ? M_RAD_TO_DEG * Height : Height;

	if( z > -0.766 )
	{
		if( z >= 19.225 )
		{
			z = 3.51823 * 0.00452 / tan(z * M_DEG_TO_RAD);
		}
		else
		{
			z = 3.51823 * (0.1594 + z * (0.0196 + 0.00002 * z) ) / (1. + z * (0.505 + 0.0845 * z));
		}

		return( bRadians ? M_DEG_TO_RAD * z : z );
	}

	return( -1. );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double SG_Get_Day_Length(int DayOfYear, double Latitude)
{
	double tanLat = tan(Latitude * M_DEG_TO_RAD);

	double JD     = DayOfYear * M_PI_360 / 365.;

	double SunDec = 0.4093 * sin(JD - 1.405); // solar declination

	double d      = -tanLat * tan(SunDec); // sunset hour angle

	return( acos(d < -1 ? -1 : d < 1 ? d : 1) * 24. / M_PI );
}

//---------------------------------------------------------
double SG_Get_Day_Length(const CSG_DateTime &Date, double Latitude)
{
	return( SG_Get_Day_Length(Date.Get_DayOfYear(), Latitude) );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
