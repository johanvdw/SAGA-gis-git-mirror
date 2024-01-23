
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                      SAGA.cpp                         //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
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
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/image.h>
#include <wx/splash.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/wfstream.h>
#include <wx/busyinfo.h>

#include <saga_api/saga_api.h>

#include "helper.h"

#include "res_images.h"

#include "saga.h"
#include "saga_frame.h"

#include "wksp.h"
#include "wksp_data_manager.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSAGA *g_pSAGA = NULL;

//---------------------------------------------------------
IMPLEMENT_APP(CSAGA)

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CSAGA, wxApp)
	EVT_KEY_DOWN(CSAGA::On_Key_Down)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSAGA::CSAGA(void)
{
	g_pSAGA = this;

	m_Process_bContinue = true;
	m_Process_Frequency = 100;
}

//---------------------------------------------------------
CSAGA::~CSAGA(void)
{}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSAGA::OnInit(void)
{
	SetVendorName("www.saga-gis.org");
	SetAppName   ("saga_gui");

	wxInitAllImageHandlers();

	wxFileName App_Path(argv[0]); App_Path.MakeAbsolute();

	m_App_Path = App_Path.GetPath();

	#if !defined(_DEBUG)
		wxSetAssertHandler(NULL);	// disable all wx asserts in SAGA release builds
	#endif

	/* workaround: wxwidgets 2.9.3 is complaining about setlocale
	 * mismatch between c setlocale and wxLocale. since saga has its own
	 * translation system, we use english as default. this assures
	 * using . as decimal separator in printf like formatting of
	 * floating point values. wxXLocale is currently not fully
	 * implemented (wxPrintf_l and similar still missing). */
	//setlocale(LC_NUMERIC, "C");
	m_wxLocale.Init(wxLANGUAGE_ENGLISH);

	_Init_Config();

	//-----------------------------------------------------
	long Frequency;

	if( CONFIG_Read("/TOOLS", "PROCESS_UPDATE", Frequency) && Frequency > 0 )
	{
		m_Process_Frequency = Frequency;
	}

	//-----------------------------------------------------
	bool bLogo = argc <= 1 && CONFIG_Read("/TOOLS", "START_LOGO", bLogo) ? bLogo : true;

	wxSplashScreen *pLogo = !bLogo ? NULL :
		new wxSplashScreen(IMG_Get_Splash(),
			wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 0, NULL, -1,
			wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE
		);

	wxYield();

	//-----------------------------------------------------
	wxString File;

	if( !CONFIG_Read("/TOOLS", "LNG_FILE_DIC", File) || !wxFileExists(File) )
	{
		File = wxFileName(Get_App_Path(), "saga", "lng").GetFullPath();
	}

	if( !SG_Get_Translator().Create(&File, false) )
	{
		CONFIG_Delete("/TOOLS", "LNG_FILE_DIC");
	}

	//-----------------------------------------------------
	SG_Initialize_Environment(false, true, NULL, false);

	//-----------------------------------------------------
	SetTopWindow(new CSAGA_Frame());

//	#ifdef __WXMAC__
//	SetExitOnFrameDelete(true); // seems to cause crash in macos app bundle
//	#endif

	//-----------------------------------------------------
	if( pLogo )
	{
		pLogo->Destroy();

		wxYield();
	}

	//-----------------------------------------------------
	if( argc > 1 )
	{
		for(int i=1; i<argc; i++)
		{
			g_pWKSP->Open(argv[i]);
		}
	}
	else
	{
		g_pData->Initialise();
	}

	//-----------------------------------------------------
	g_pSAGA_Frame->Show_Tips(false);

	return( true );
}

//---------------------------------------------------------
int CSAGA::OnExit(void)
{
	while( Set_Busy(false) ); // should not be necessary!

	delete(wxConfigBase::Set((wxConfigBase *)NULL));

	return( 0 );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA::_Init_Config(void)
{
	wxConfigBase *pConfig;

	#if defined(_SAGA_MSW)
		wxFileName fLocal(Get_App_Path(), "saga_gui", "ini");

		if( ( fLocal.FileExists() && (!fLocal.IsFileReadable() || !fLocal.IsFileWritable()))
		||  (!fLocal.FileExists() && (!fLocal.IsDirReadable () || !fLocal.IsDirWritable ())) )
		{
			wxFileName fUser(wxGetHomeDir(), "saga_gui", "ini");
		//	wxFileName fUser(wxStandardPaths::Get().GetUserConfigDir(), "saga_gui", "ini");

			if(	fLocal.FileExists() && fLocal.IsFileReadable() && !fUser.FileExists() )	// create a copy in user's home directory
			{
				wxFileInputStream  is(fLocal.GetFullPath());
				wxFileOutputStream os(fUser .GetFullPath());
				wxFileConfig       ic(is); ic.Save(os);
			}

			fLocal = fUser;
		}

		if( (fLocal.FileExists() && fLocal.IsFileWritable()) || (!fLocal.FileExists() && fLocal.IsDirWritable()) )
		{
			pConfig = new wxFileConfig(wxEmptyString, wxEmptyString, fLocal.GetFullPath(), fLocal.GetFullPath(), wxCONFIG_USE_LOCAL_FILE|wxCONFIG_USE_GLOBAL_FILE|wxCONFIG_USE_RELATIVE_PATH);
		}
		else
		{
			pConfig = new wxConfig;	// this might go to registry !
		}
	#else
		pConfig = new wxConfig;
	#endif

	wxConfigBase::Set(pConfig);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA::On_Key_Down(wxKeyEvent &event)
{
	switch( event.GetKeyCode() )
	{
	default:
		event.Skip();
		break;

	case WXK_ESCAPE:
		m_Process_bContinue = false;
		break;
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSAGA::Process_Wait(bool bEnforce)
{
	static bool bYield = false; static wxDateTime tYield = wxDateTime::UNow();

	if( !bYield && (bEnforce || m_Process_Frequency <= (wxDateTime::UNow() - tYield).GetMilliseconds()) )
	{
		bYield = true;

		Yield(true);

		ProcessPendingEvents();	// while( Pending() && Dispatch() );

		if( m_Process_Frequency > 0 )
		{
			tYield= wxDateTime::UNow();
		}

		bYield = false;
	}

	return( true );
}

//---------------------------------------------------------
bool CSAGA::Process_Set_Okay(bool bOkay)
{
	m_Process_bContinue = bOkay;

	return( m_Process_bContinue );
}

//---------------------------------------------------------
bool CSAGA::Process_Get_Okay(void)
{
	Process_Wait();

	return( m_Process_bContinue );
}

//---------------------------------------------------------
bool CSAGA::Process_Set_Frequency(size_t Milliseconds)
{
	if( Milliseconds > 0 && Milliseconds != m_Process_Frequency )
	{
		m_Process_Frequency = Milliseconds;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSAGA::Set_Busy(bool bOn, const CSG_String &Message)
{
	static size_t Count = 0; static wxWindowDisabler *pDisabler = NULL; static wxBusyInfo *pInfo = NULL;

	if( bOn )
	{
		if( Count == 0 )
		{
			wxBeginBusyCursor();

			pDisabler = new wxWindowDisabler;

			if( !Message.is_Empty() )
			{
				wxBusyInfoFlags Flags;

				Flags.Parent      (g_pSAGA_Frame);
			//	Flags.Icon        (IMG_Get_Icon(ID_IMG_SAGA_ICON_32)); // #include "res_images.h"
			//	Flags.Label       (wxString::Format("SAGA - %s", _TL("Processing")));
			//	Flags.Title       (_TL("Please wait, working..."));
			//	Flags.Background  (*wxWHITE);
			//	Flags.Background  (*wxBLACK);
				Flags.Transparency(4 * wxALPHA_OPAQUE / 5);
				Flags.Text(Message.c_str());

				pInfo = new wxBusyInfo(Flags);
			//	pInfo = new wxBusyInfo(_TL("Please wait, working..."), g_pSAGA_Frame);
			}
		}

		Count++;
	}
	else if( Count > 0 )
	{
		Count--;

		if( Count == 0 )
		{
			if( pInfo )
			{
				delete(pInfo); pInfo = NULL;
			}

			delete(pDisabler); pDisabler = NULL;

			wxEndBusyCursor();
		}
	}

	return( Count > 0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
