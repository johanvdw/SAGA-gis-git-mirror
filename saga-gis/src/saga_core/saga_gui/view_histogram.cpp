
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
//                  VIEW_Histogram.cpp                   //
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
//                University of Goettingen               //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/window.h>
#include <wx/toolbar.h>
#include <wx/clipbrd.h>

#include <saga_gdi/sgdi_helper.h>

#include "res_commands.h"
#include "res_controls.h"
#include "res_images.h"

#include "helper.h"
#include "res_dialogs.h"

#include "active.h"

#include "wksp_data_manager.h"
#include "wksp_layer_classify.h"
#include "wksp_grid.h"
#include "wksp_shapes.h"
#include "wksp_pointcloud.h"

#include "view_histogram.h"


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define IS_BAND_WISE_FIT (m_pLayer->Get_Type() == WKSP_ITEM_Grids && m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_OVERLAY && m_pLayer->Get_Parameter("OVERLAY_FIT")->asInt() != 0)


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CVIEW_Histogram, CVIEW_Base);

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVIEW_Histogram, CVIEW_Base)
	EVT_PAINT     (CVIEW_Histogram::On_Paint)
	EVT_SIZE      (CVIEW_Histogram::On_Size)

	EVT_MOTION    (CVIEW_Histogram::On_Mouse_Motion)
	EVT_LEFT_DOWN (CVIEW_Histogram::On_Mouse_LDown)
	EVT_LEFT_UP   (CVIEW_Histogram::On_Mouse_LUp)
	EVT_RIGHT_DOWN(CVIEW_Histogram::On_Mouse_RDown)

	EVT_MENU      (ID_CMD_HISTOGRAM_PARAMETERS  , CVIEW_Histogram::On_Properties)
	EVT_MENU      (ID_CMD_HISTOGRAM_CUMULATIVE  , CVIEW_Histogram::On_Cumulative)
	EVT_MENU      (ID_CMD_HISTOGRAM_GAUSSIAN    , CVIEW_Histogram::On_Gaussian)
	EVT_MENU      (ID_CMD_HISTOGRAM_SET_MINMAX  , CVIEW_Histogram::On_Set_MinMax)
	EVT_MENU      (ID_CMD_HISTOGRAM_AS_TABLE    , CVIEW_Histogram::On_AsTable)
	EVT_MENU      (ID_CMD_VIEW_CLIPBOARD_COPY   , CVIEW_Histogram::On_ToClipboard)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CVIEW_Histogram::CVIEW_Histogram(CWKSP_Layer *pLayer)
	: CVIEW_Base(pLayer, ID_VIEW_HISTOGRAM, wxString::Format("%s | %s", _TL("Histogram"), pLayer->Get_Object()->Get_Name()), ID_IMG_WND_HISTOGRAM)
{
	m_pLayer         = pLayer;

	m_bCumulative    = false;
	m_bGaussian      = false;
	m_Gaussian_Size  = 1;
	m_bColored       = true;

	m_XLabeling      = 0;

	m_Color_Mode     = 0;
	m_Margin_Left    = 30;
	m_Margin_Bottom  = m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_LUT ? 100 : 30;

	m_bMouse_Down    = false;

	Do_Update();
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxMenu * CVIEW_Histogram::_Create_Menu(void)
{
	wxMenu *pMenu = new wxMenu;

	CMD_Menu_Add_Item(pMenu, false, ID_CMD_HISTOGRAM_PARAMETERS);
	pMenu->AppendSeparator();
	CMD_Menu_Add_Item(pMenu, true , ID_CMD_HISTOGRAM_CUMULATIVE);
	CMD_Menu_Add_Item(pMenu, true , ID_CMD_HISTOGRAM_GAUSSIAN);
	pMenu->AppendSeparator();
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_HISTOGRAM_AS_TABLE);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_VIEW_CLIPBOARD_COPY);

	return( pMenu );
}

//---------------------------------------------------------
wxToolBarBase * CVIEW_Histogram::_Create_ToolBar(void)
{
	wxToolBarBase *pToolBar = CMD_ToolBar_Create(ID_TB_VIEW_HISTOGRAM);

	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_HISTOGRAM_PARAMETERS);
	CMD_ToolBar_Add_Item(pToolBar, true , ID_CMD_HISTOGRAM_CUMULATIVE);
	CMD_ToolBar_Add_Item(pToolBar, true , ID_CMD_HISTOGRAM_GAUSSIAN);
	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_HISTOGRAM_AS_TABLE);
	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_VIEW_CLIPBOARD_COPY);

	CMD_ToolBar_Add(pToolBar, _TL("Histogram"));

	return( pToolBar );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram::Do_Update(void)
{
	if( IS_BAND_WISE_FIT )
	{
		m_pLayer->Get_Classifier()->Set_Metric(0, 1., m_pLayer->Get_Stretch_Minimum(), m_pLayer->Get_Stretch_Maximum());
	}

	if( m_pLayer->Get_Classifier()->Histogram_Update() )
	{
		Refresh();
	}

	Update();
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram::On_Size(wxSizeEvent &WXUNUSED(event))
{
	Refresh();
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Paint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	
	Draw_Edge(dc, EDGE_STYLE_SUNKEN, wxRect(GetClientSize()));
	Draw     (dc);
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram::Draw(wxDC &dc)
{
	wxFont Font; Font.SetFamily(wxFONTFAMILY_SWISS); dc.SetFont(Font);

	dc.SetPen           (SYS_Get_Color_Foreground(m_Color_Mode));
	dc.SetTextForeground(SYS_Get_Color_Foreground(m_Color_Mode));

	Draw_Histogram(dc);
	Draw_Frame    (dc);
}

//---------------------------------------------------------
void CVIEW_Histogram::Draw_Bar(wxDC &dc, wxColour Color, int x[2], int y[3], int Invert[2], int StdDev[2])
{
	wxPen oldPen(dc.GetPen());

	wxColour Current(Color); dc.SetPen(Current);

	if( y[0] >= y[1] )
	{
		for(int ix=x[0]; ix<x[1]; ix++)
		{
			wxColour Next(Color);

			if( Invert[0] <= ix && ix <= Invert[1] )
			{
				Next = wxColour(255 - Next.Red(), 255 - Next.Green(), 255 - Next.Blue());
			}

			if( Current != Next )
			{
				dc.SetPen(Current = Next);
			}

			dc.DrawLine(ix, y[0], ix, y[1]);
		}
	}

	if( y[1] >= y[2] )
	{
		bool bDark = m_Color_Mode == 0 ? wxSystemSettings::GetAppearance().IsUsingDarkBackground() : m_Color_Mode == 2;

		Color = SYS_Get_Color_Background(m_Color_Mode);

		for(int ix=x[0]; ix<x[1]; ix++)
		{
			wxColour Next(StdDev[0] <= ix && ix <= StdDev[1] ? (bDark ? wxColour(96, 96, 96) : wxColour(192, 192, 192)) : Color);

			if( Invert[0] <= ix && ix <= Invert[1] )
			{
				Next = wxColour(255 - Next.Red(), 255 - Next.Green(), 255 - Next.Blue());
			}

			if( Current != Next )
			{
				dc.SetPen(Current = Next);
			}

			dc.DrawLine(ix, y[1], ix, y[2]);
		}
	}

	dc.SetPen(oldPen);
}

//---------------------------------------------------------
void CVIEW_Histogram::Draw_Histogram(wxDC &dc)
{
	wxRect r(Get_Histogram_Box(dc.GetSize()));

	const CSG_Histogram &Histogram = m_pLayer->Get_Classifier()->Histogram_Get();

	if( Histogram.Get_Class_Count() < 1 || Histogram.Get_Element_Count() < 1 )
	{
		Draw_Text(dc, TEXTALIGN_CENTER, r.GetLeft() + r.GetWidth() / 2, r.GetBottom() - r.GetHeight() / 2, _TL("invalid histogram"));

		return;
	}

	//-----------------------------------------------------
	int StdDev[2]; StdDev[0] = StdDev[1] = -1; CSG_Simple_Statistics s;

	if( m_bGaussian && m_pLayer->Get_Classifier()->Get_Mode() != CLASSIFY_LUT )
	{
		s.Create(m_pLayer->Get_Classifier()->Statistics_Get());

		if( s.Get_Count() > 0 )
		{
			double Minimum = m_pLayer->Get_Classifier()->Get_RelativeToMetric(0.);
			double Maximum = m_pLayer->Get_Classifier()->Get_RelativeToMetric(1.);

			double dx = (Maximum - Minimum) / (double)r.GetWidth();

			StdDev[0] = r.GetLeft() + (int)(((s.Get_Mean() - s.Get_StdDev()) - Minimum) / dx); if( StdDev[0] < 0            ) { StdDev[0] = 0.          ; }
			StdDev[1] = r.GetLeft() + (int)(((s.Get_Mean() + s.Get_StdDev()) - Minimum) / dx); if( StdDev[1] > r.GetWidth() ) { StdDev[1] = r.GetWidth(); }
		}
	}

	int Invert[2]; Invert[0] = Invert[1] = -1;

	if( m_bMouse_Down && m_Mouse_Down.x != m_Mouse_Move.x )
	{
		Invert[0] = m_Mouse_Down.x <  m_Mouse_Move.x ? m_Mouse_Down.x : m_Mouse_Move.x;
		Invert[1] = m_Mouse_Down.x >= m_Mouse_Move.x ? m_Mouse_Down.x : m_Mouse_Move.x;
	}

	//-----------------------------------------------------
	bool bDark = m_Color_Mode == 0 ? wxSystemSettings::GetAppearance().IsUsingDarkBackground() : m_Color_Mode == 2;

	wxColor Color = bDark ? wxColour(200, 200, 200) : SYS_Get_Color(wxSYS_COLOUR_ACTIVECAPTION); // wxSYS_COLOUR_BTNSHADOW);

	double dx = (double)r.GetWidth() / (double)Histogram.Get_Class_Count();

	int x[2], y[3]; x[1] = r.GetLeft(); y[0] = r.GetBottom(); y[2] = r.GetTop();

	for(size_t iClass=0; iClass<Histogram.Get_Class_Count(); iClass++)
	{
		double Value = m_bCumulative
			? Histogram.Get_Cumulative(iClass) / (double)Histogram.Get_Element_Count  ()
			: Histogram.Get_Elements  (iClass) / (double)Histogram.Get_Element_Maximum();

		x[0] = x[1]; x[1] = r.GetLeft() + (int)(dx * (iClass + 1.));
		y[1] = y[0] - (int)((r.GetHeight() - 1) * Value);

		if( m_bColored && m_pLayer->Get_Classifier()->Get_Mode() != CLASSIFY_OVERLAY && m_pLayer->Get_Classifier()->Get_Mode() != CLASSIFY_SHADE )
		{
			Color = m_pLayer->Get_Classifier()->Get_Class_Color(iClass);
		}

		Draw_Bar(dc, Color, x, y, Invert, StdDev);
	}

	//-----------------------------------------------------
	if( s.Get_Count() > 0 ) // plot gaussian normal distribution
	{
		#define FUNC_NORMAL(X) (m_bCumulative\
			? (1. / (1. + exp(-((X - s.Get_Mean()) / (0.5 * s.Get_StdDev())))) - y0)\
			: (exp(-SG_Get_Square(X - s.Get_Mean()) / (2. * s.Get_Variance())))\
		)

		#define DRAW_LINE(X) if( Minimum < (X) && (X) < Maximum ) {\
			int ix = r.GetLeft() + (int)(((X) - Minimum) / dx);\
			dc.DrawLine(ix, r.GetBottom(), ix, r.GetBottom() - (int)(dy * FUNC_NORMAL(X)));\
		}

		double Minimum = m_pLayer->Get_Classifier()->Get_RelativeToMetric(0.);
		double Maximum = m_pLayer->Get_Classifier()->Get_RelativeToMetric(1.);

		double dx = (Maximum - Minimum) / (double)r.GetWidth(), dy = r.GetHeight() - 1;

		double y0 = 0.; double yMin = FUNC_NORMAL(Minimum), yMax = FUNC_NORMAL(Maximum);

		if( m_bCumulative )
		{
			dy /= (yMax - yMin); y0 = yMin;
		}
		else if( s.Get_Mean() < Minimum )
		{
			dy /= yMin;
		}
		else if( s.Get_Mean() > Maximum )
		{
			dy /= yMax;
		}

		wxPen oldPen(dc.GetPen()); dc.SetPen(wxPen(SYS_Get_Color_Foreground(m_Color_Mode), m_Gaussian_Size));

		DRAW_LINE(s.Get_Mean()                 );
		DRAW_LINE(s.Get_Mean() - s.Get_StdDev());
		DRAW_LINE(s.Get_Mean() + s.Get_StdDev());

		int y = (int)(dy * FUNC_NORMAL(Minimum));

		for(size_t i=1; i<(size_t)r.GetWidth(); i++)
		{
			double x = Minimum + i * dx; int ay = y; y = (int)(dy * FUNC_NORMAL(x));

			dc.DrawLine(r.GetLeft() + i - 1, r.GetBottom() - ay, r.GetLeft() + i, r.GetBottom() - y);
		}

		dc.SetPen(oldPen);
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::Draw_Frame(wxDC &dc)
{
	wxRect r(Get_Histogram_Box(dc.GetSize()));

	const int FontSize  = 12;

	wxPen oldPen(dc.GetPen()); dc.SetPen(SYS_Get_Color_Foreground(m_Color_Mode));

	Draw_Edge(dc, EDGE_STYLE_SIMPLE, r);

	const CSG_Histogram	&Histogram	= m_pLayer->Get_Classifier()->Histogram_Get();

	if( Histogram.Get_Class_Count() < 1 || Histogram.Get_Element_Count() < 1 )
	{
		return;
	}

	//-----------------------------------------------------
	if( Histogram.Get_Element_Maximum() > 0 )
	{
		Draw_Scale(dc, wxRect(r.GetLeft() - 20, r.GetTop(), 20, r.GetHeight()), 0, m_bCumulative ? 100 : Histogram.Get_Element_Maximum(), false, false, false);
	}

	//-----------------------------------------------------
	wxFont Font(dc.GetFont()); Font.SetPointSize((int)(0.7 * FontSize)); dc.SetFont(Font);

	//-----------------------------------------------------
	if( m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_LUT )
	{
		double dx = r.GetWidth() / (double)Histogram.Get_Class_Count(), n = 0.;

		for(size_t iClass=0; iClass<Histogram.Get_Class_Count(); iClass++, n+=dx)
		{
			if( iClass == 0 || n > (FontSize + 5) )
			{
				n = 0.;

				int ix = r.GetLeft() + (int)(dx * (0.5 + iClass));
				dc.DrawLine(ix, r.GetBottom(), ix, r.GetBottom() + 5);
				Draw_Text(dc, TEXTALIGN_TOPRIGHT, ix, r.GetBottom() + 7, 45.,
					m_pLayer->Get_Classifier()->Get_Class_Name(iClass)
				);
			}
		}
	}

	//-----------------------------------------------------
	else switch( m_XLabeling )
	{
	case  0: {
		Draw_Scale(dc, wxRect(r.GetLeft(), r.GetBottom(), r.GetWidth(), 20),
			m_pLayer->Get_Classifier()->Get_RelativeToMetric(0.),
			m_pLayer->Get_Classifier()->Get_RelativeToMetric(1.),
			true , true , true
		);
		break; }

	default:
		{
			double	dx	= r.GetWidth() / (double)Histogram.Get_Class_Count();

			if( dx < (FontSize + 5) )
			{
				dx	*= 1 + (int)((FontSize + 5) / dx);
			}

			double	dz	= dx / (double)r.GetWidth();

			for(int i=0, n=(int)(r.GetWidth()/dx); i<=n; i++)
			{
				int	ix	= r.GetLeft() + (int)(dx * i);
				dc.DrawLine(ix, r.GetBottom(), ix, r.GetBottom() + 5);
				Draw_Text(dc, TEXTALIGN_CENTERRIGHT, ix, r.GetBottom() + 7, 45.,
					SG_Get_String(m_pLayer->Get_Classifier()->Get_RelativeToMetric(dz * i), -2).c_str()
				);
			}
		break; }
	}

	dc.SetPen(oldPen);
}

//---------------------------------------------------------
wxRect CVIEW_Histogram::Get_Histogram_Box(const wxSize &Size)
{
	wxRect r(Size);

	if( m_XLabeling != 0 || m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_LUT )
	{
		return(	wxRect(
			wxPoint(r.GetLeft () + m_Margin_Left, r.GetTop   () + 10),
			wxPoint(r.GetRight() - 10           , r.GetBottom() - m_Margin_Bottom)
		));
	}

	return(	wxRect(
		wxPoint(r.GetLeft () + m_Margin_Left, r.GetTop   () + 10),
		wxPoint(r.GetRight() - 10           , r.GetBottom() - 30)
	));
}

//---------------------------------------------------------
wxRect CVIEW_Histogram::Get_Histogram_Box(void)
{
	return( Get_Histogram_Box(GetClientSize()) );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram::On_Mouse_Motion(wxMouseEvent &event)
{
	if( m_bMouse_Down )
	{
		m_Mouse_Move = event.GetPosition();

		RefreshRect(Get_Histogram_Box().Deflate(1), false);
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Mouse_LDown(wxMouseEvent &event)
{
	if( IS_BAND_WISE_FIT )
	{
		return;
	}

	switch( m_pLayer->Get_Classifier()->Get_Mode() )
	{
	case CLASSIFY_GRADUATED:
	case CLASSIFY_DISCRETE :
	case CLASSIFY_SHADE    :
	case CLASSIFY_OVERLAY  :
		m_bMouse_Down = true;
		m_Mouse_Move = m_Mouse_Down = event.GetPosition();

		CaptureMouse();

	default: break;
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Mouse_LUp(wxMouseEvent &event)
{
	if( m_bMouse_Down )
	{
		ReleaseMouse();

		m_bMouse_Down = false; m_Mouse_Move = event.GetPosition(); wxRect r(Get_Histogram_Box());

		m_pLayer->Set_Stretch_Range(
			m_pLayer->Get_Classifier()->Get_RelativeToMetric((double)(m_Mouse_Down.x - r.GetLeft()) / (double)r.GetWidth()),
			m_pLayer->Get_Classifier()->Get_RelativeToMetric((double)(m_Mouse_Move.x - r.GetLeft()) / (double)r.GetWidth())
		);
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Mouse_RDown(wxMouseEvent &event)
{
	wxMenu Menu;

	if( !IS_BAND_WISE_FIT )
	{
		CMD_Menu_Add_Item(&Menu, false, ID_CMD_HISTOGRAM_SET_MINMAX);
		Menu.AppendSeparator();
	}

	CMD_Menu_Add_Item(&Menu, true , ID_CMD_HISTOGRAM_CUMULATIVE);
	if( m_pLayer->Get_Classifier()->Get_Mode() != CLASSIFY_LUT )
	{
		CMD_Menu_Add_Item(&Menu, true , ID_CMD_HISTOGRAM_GAUSSIAN);
	}

	Menu.AppendSeparator();
	CMD_Menu_Add_Item(&Menu, false, ID_CMD_HISTOGRAM_AS_TABLE);
	CMD_Menu_Add_Item(&Menu, false, ID_CMD_VIEW_CLIPBOARD_COPY);

	Menu.AppendSeparator();
	CMD_Menu_Add_Item(&Menu, false, ID_CMD_HISTOGRAM_PARAMETERS);

	PopupMenu(&Menu, event.GetPosition());
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	case ID_CMD_HISTOGRAM_CUMULATIVE:
		if( m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_SINGLE
		||  m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_RGB )
		{
			event.Enable(false);
			event.Check (false);
		}
		else
		{
			event.Enable(true);
			event.Check (m_bCumulative);
		}
		break;

	case ID_CMD_HISTOGRAM_GAUSSIAN:
		if( m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_DISCRETE
		||  m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_GRADUATED
		||  m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_SHADE
		||  m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_OVERLAY )
		{
			event.Enable(true);
			event.Check (m_bGaussian);
		}
		else
		{
			event.Enable(false);
			event.Check (false);
		}
		break;

	default: break;
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Properties(wxCommandEvent &event)
{
	CSG_Parameters P(_TL("Histogram"));

	P.Add_Int   (""        , "NCLASSES"      , _TL("Number of Bins"     ), _TL(""), m_pLayer->Get_Classifier()->Get_Class_Count(), 1, true);

	P.Add_Bool  (""        , "CUMULATIVE"    , _TL("Cumulative"         ), _TL(""), m_bCumulative);
	P.Add_Bool  (""        , "COLORED"       , _TL("Colored"            ), _TL(""), m_bColored);

	P.Add_Choice(""        , "XLABELING"     , _TL("Class Labeling"     ), _TL(""),
		CSG_String::Format("%s|%s", _TL("horizontal"), _TL("diagonal"), _TL("vertical")), m_XLabeling
	);

	P.Add_Bool  (""        , "GAUSSIAN"      , _TL("Normal Distribution"), _TL(""), m_bGaussian  );
	P.Add_Int   ("GAUSSIAN", "GAUSSIAN_SIZE" , _TL("Line Width"         ), _TL(""), m_Gaussian_Size, 1, true);

	P.Add_Choice(""        , "COLOR_MODE"    , _TL("Color Mode"         ), _TL(""),
		CSG_String::Format("%s|%s|%s", _TL("system"), _TL("light"), _TL("dark"))  , m_Color_Mode
	);

	P.Add_Node  (""        , "MARGINS"       , _TL("Margins"            ), _TL(""));
	P.Add_Int   ("MARGINS" , "MARGIN_LEFT"   , _TL("Left"               ), _TL(""), m_Margin_Left  , 10, true);
	P.Add_Int   ("MARGINS" , "MARGIN_BOTTOM" , _TL("Bottom"             ), _TL(""), m_Margin_Bottom, 10, true);

	//-----------------------------------------------------
	P.Set_Enabled("NCLASSES",
		m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_GRADUATED
	||	m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_SHADE
	||	m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_OVERLAY
	);

	P.Set_Enabled("CUMULATIVE", m_pLayer->Get_Classifier()->Get_Mode() != CLASSIFY_LUT);
	P.Set_Enabled("GAUSSIAN"  , m_pLayer->Get_Classifier()->Get_Mode() != CLASSIFY_LUT);
	P.Set_Enabled("XLABELING" , m_pLayer->Get_Classifier()->Get_Mode() != CLASSIFY_LUT);

	//-----------------------------------------------------
	if( DLG_Parameters(&P) )
	{
		m_pLayer->Get_Classifier()->Set_Class_Count(P("NCLASSES")->asInt());

		m_bCumulative    = P("CUMULATIVE"    )->asBool();
		m_bColored       = P("COLORED"       )->asBool();
		m_XLabeling      = P("XLABELING"     )->asInt ();
		m_bGaussian      = P("GAUSSIAN"      )->asBool();
		m_Gaussian_Size  = P("GAUSSIAN_SIZE" )->asInt ();
		m_Color_Mode     = P("COLOR_MODE"    )->asInt ();
		m_Margin_Left    = P("MARGIN_LEFT"   )->asInt ();
		m_Margin_Bottom  = P("MARGIN_BOTTOM" )->asInt ();

		SetBackgroundColour(SYS_Get_Color_Background(m_Color_Mode));

		Refresh();
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Cumulative(wxCommandEvent &event)
{
	m_bCumulative = !m_bCumulative;

	Refresh();
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Gaussian(wxCommandEvent &event)
{
	m_bGaussian = !m_bGaussian;

	Refresh();
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Set_MinMax(wxCommandEvent &event)
{
	m_pLayer->Set_Stretch_FullRange();
}

//---------------------------------------------------------
void CVIEW_Histogram::On_AsTable(wxCommandEvent &event)
{
	CWKSP_Layer_Classify *pClassifier = ((CWKSP_Layer *)m_pOwner)->Get_Classifier();

	if( pClassifier->Get_Class_Count() > 0 )
	{
		CSG_Data_Object *pObject = ((CWKSP_Layer *)m_pOwner)->Get_Object();

		CSG_Table *pTable = SG_Create_Table();

		pTable->Fmt_Name("%s: %s", _TL("Histogram"), pObject->Get_Name());

		pTable->Add_Field(_TL("CLASS" ), SG_DATATYPE_Int   );
		pTable->Add_Field(_TL("AREA"  ), SG_DATATYPE_Double);
		pTable->Add_Field(_TL("COUNT" ), SG_DATATYPE_Long  );
		pTable->Add_Field(_TL("CUMUL" ), SG_DATATYPE_Long  );
		pTable->Add_Field(_TL("NAME"  ), SG_DATATYPE_String);
		pTable->Add_Field(_TL("MIN"   ), SG_DATATYPE_Double);
		pTable->Add_Field(_TL("CENTER"), SG_DATATYPE_Double);
		pTable->Add_Field(_TL("MAX"   ), SG_DATATYPE_Double);

		double dArea = pObject->asGrid() != NULL ? pObject->asGrid()->Get_Cellarea() : 1.;

		for(int i=0; i<pClassifier->Get_Class_Count(); i++)
		{
			CSG_Table_Record &Class = *pTable->Add_Record();

			Class.Set_Value(0, i + 1);
			Class.Set_Value(1, pClassifier->Histogram_Get().Get_Elements  (i) * dArea);
			Class.Set_Value(2, pClassifier->Histogram_Get().Get_Elements  (i));
			Class.Set_Value(3, pClassifier->Histogram_Get().Get_Cumulative(i));
			Class.Set_Value(4, pClassifier->Get_Class_Name                (i).wx_str());
			Class.Set_Value(5, pClassifier->Get_Class_Value_Minimum       (i));
			Class.Set_Value(6, pClassifier->Get_Class_Value_Center        (i));
			Class.Set_Value(7, pClassifier->Get_Class_Value_Maximum       (i));
		}

		g_pData->Add(pTable);
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::On_ToClipboard(wxCommandEvent &event)
{
	wxBitmap Bitmap(GetClientSize());

	wxMemoryDC dc(Bitmap);

	dc.SetBackground(SYS_Get_Color_Background(m_Color_Mode)); dc.Clear();

	Draw(dc);

	dc.SelectObject(wxNullBitmap);

	if( wxTheClipboard->Open() )
	{
		wxBitmapDataObject *pBitmap = new wxBitmapDataObject;
		pBitmap->SetBitmap(Bitmap);
		wxTheClipboard->SetData(pBitmap);
		wxTheClipboard->Close();
	}
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
