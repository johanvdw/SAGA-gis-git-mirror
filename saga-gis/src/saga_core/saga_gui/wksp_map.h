
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
//                     WKSP_Map.h                        //
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
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Map_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_Map_H


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <saga_api/saga_api.h>

#include "wksp_base_manager.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Map_Extents : public CSG_Rects
{
public:
	CWKSP_Map_Extents(void);

	bool						is_First				(void)		{	return( m_iExtent <= 0 );				}
	bool						is_Last					(void)		{	return( m_iExtent >= m_nExtents - 1 );	}

	CSG_Rect					Set_Back				(void);
	CSG_Rect					Set_Forward				(void);

	bool						Add_Extent				(const CSG_Rect &Extent, bool bReset = false);
	const CSG_Rect &			Get_Extent				(void)		{	return( m_iExtent >= 0 ? Get_Rect(m_iExtent) : m_Dummy );	}


private:

	int							m_iExtent, m_nExtents;

	static CSG_Rect				m_Dummy;

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Map : public CWKSP_Base_Manager
{
public:
	CWKSP_Map(void);
	virtual ~CWKSP_Map(void);

	virtual TWKSP_Item			Get_Type				(void)		{	return( WKSP_ITEM_Map );	}

	virtual wxString			Get_Name				(void);
	virtual wxString			Get_Description			(void);

	virtual wxMenu *			Get_Menu(void);

	virtual bool				On_Command				(int Cmd_ID);
	virtual bool				On_Command_UI			(wxUpdateUIEvent &event);

	virtual void				Parameters_Changed		(void);

	bool						Serialize				(CSG_MetaData &Root, const wxString &ProjectDir, bool bSave);

	const CSG_Rect &			Get_Extent				(void)		{	return( m_Extents.Get_Extent() );	}
	bool						Set_Extent				(const CSG_Rect &Extent, bool bReset = false, bool bPan = false);
	bool						Set_Extent				(const CSG_Rect &Extent, const CSG_Projection &Projection, bool bPan = false);
	bool						Set_Extent				(void);
	bool						Set_Extent_Full			(void);
	bool						Set_Extent_Active		(bool bPan);
	bool						Set_Extent_Selection	(bool bPan);
	bool						Set_Extent_Back			(bool bCheck_Only = false);
	bool						Set_Extent_Forward		(bool bCheck_Only = false);

	bool						is_Synchronising		(void);
	void						Set_Synchronising		(bool bOn);
	void						Lock_Synchronising		(bool bOn);

	bool						is_North_Arrow			(void);
	void						Set_North_Arrow			(bool bOn);

	bool						is_ScaleBar				(bool bFrame = false);
	void						Set_ScaleBar			(bool bOn);

	bool						is_Image_Save_Mode		(void)	{	return( m_Img_bSave );		}

	void						Set_Mouse_Position		(const TSG_Point &Point);
	void						Set_CrossHair			(const TSG_Point &Point, const CSG_Projection &Projection);
	void						Set_CrossHair_Off		(void);

	bool						Update					(class CWKSP_Layer *pLayer, bool bMapOnly);
	int							Get_Map_Layer_Index		(class CWKSP_Layer *pLayer);
	class CWKSP_Map_Layer *		Get_Map_Layer			(class CWKSP_Layer *pLayer);
	class CWKSP_Map_Layer *		Get_Map_Layer_Active	(bool bEditable = false);
	class CWKSP_Map_Layer *		Add_Layer				(class CWKSP_Layer *pLayer);
	class CWKSP_Map_Graticule *	Add_Graticule			(CSG_MetaData *pEntry = NULL);
	class CWKSP_Map_BaseMap *	Add_BaseMap				(CSG_MetaData *pEntry = NULL);
	class CWKSP_Base_Item *		Add_Copy				(CWKSP_Base_Item *pItem);

    bool                        View_Opened             (class MDI_ChildFrame *pView);
	void						View_Closes				(class MDI_ChildFrame *pView);
	void						View_Refresh			(bool bMapOnly);
	class CVIEW_Map *			View_Get				(void)		{	return( m_pView );		}
	void						View_Show				(bool bShow);
	void						View_Toggle				(void);
	class CVIEW_Map_3D *		View_3D_Get				(void)		{	return( m_pView_3D );	}
	void						View_3D_Show			(bool bShow);
	void						View_3D_Toggle			(void);
	class CVIEW_Layout *		View_Layout_Get			(void)		{	return( m_pLayout );	}
	void						View_Layout_Show		(bool bShow);
	void						View_Layout_Toggle		(void);

	CSG_Rect					Get_World				(wxRect rClient);
	CSG_Point					Get_World				(wxRect rClient, wxPoint ptClient);

	bool						Get_Image				(wxImage &Image, const CSG_Grid_System &System);
	bool						Get_Image				(wxImage &Image, CSG_Rect &rWorld);
	bool						SaveAs_Image			(void);
	bool						SaveAs_Image_Clipboard	(int nx, int ny, int frame);
	bool						SaveAs_Image_Clipboard	(bool bLegend = false);
	bool						SaveAs_Image_To_KMZ		(int nx, int ny);
	bool						SaveAs_Image_To_Memory	(int nx, int ny);
	bool						SaveAs_Image_To_Grid	(CSG_Grid &Grid, int Size);
	bool						SaveAs_Image_On_Change	(void);

	wxColour					Get_Background			(void);

	void						Draw_Map				(wxDC &dc                        , double Zoom, const wxRect &rClient, int Flags = 0, int Background = -1);
	void						Draw_Map				(wxDC &dc, const CSG_Rect &rWorld, double Zoom, const wxRect &rClient, int Flags = 0, int Background = -1);
	void						Draw_Map				(class CSG_Map_DC &dc, int Flags = 0);

	void						Draw_Frame				(wxDC &dc, wxRect rMap, int Width);
	void						Draw_Frame				(wxDC &dc, const CSG_Rect &rWorld, wxRect rMap, int Width, bool bScaleBar, bool bUseDCFont = false);
	bool						Draw_Legend				(wxDC &dc, double Zoom_Map, double Zoom, wxPoint Position, wxSize *pSize = NULL);

	bool						Draw_North_Arrow		(class CSG_Map_DC &dc);
	bool						Draw_ScaleBar			(class CSG_Map_DC &dc);
	bool						Draw_Extent				(class CSG_Map_DC &dc);

	const wxBitmap &			Get_Thumbnail			(int nx, int ny);

	bool						Get_Legend_Size			(wxSize &Size, double Zoom_Map = 1.0, double Zoom = 1.0);

	int							Get_Frame_Width			(void);

	void						Set_Projection			(void);
	CSG_Projection &			Get_Projection			(void)	{	return( m_Projection );	}

	void						Show_Coordinate			(const CSG_Point &Coordinate)	const;


protected:

	virtual void				On_Create_Parameters	(void);

	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags);


private:

	bool						m_Img_bSave;

	int							m_Img_Type, m_Img_Count, m_Sync_bLock;

	wxString					m_Name, m_Img_File;

	wxBitmap					m_Thumbnail;

	CSG_Parameters				m_Img_Parms;

	CSG_Projection				m_Projection;

	CWKSP_Map_Extents			m_Extents;

	class CVIEW_Map				*m_pView;

	class CVIEW_Map_3D			*m_pView_3D;

	class CVIEW_Layout			*m_pLayout;

	class CVIEW_Layout_Info		*m_pLayout_Info;


	bool						_Set_Extent				(const CSG_Rect &Extent);

	void						_Img_Save				(wxString file, int type);
	void						_Img_Save_On_Change		(void);

	wxString					_Set_Description_Image	(void);
	bool						_Get_Thumbnail			(wxBitmap &Thumbnail, int nx, int ny);
	bool						_Set_Thumbnail			(wxBitmap &Thumbnail);

};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Map_H
