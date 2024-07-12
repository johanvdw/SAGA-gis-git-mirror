
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
//                 WKSP_Map_Control.cpp                  //
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
#include <wx/image.h>
#include <wx/imaglist.h>

#include <saga_api/saga_api.h>

#include "res_controls.h"
#include "res_images.h"

#include "helper.h"

#include "wksp_map_control.h"
#include "wksp_map_manager.h"
#include "wksp_map.h"
#include "wksp_map_layer.h"
#include "wksp_map_buttons.h"

#include "wksp_shapes.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	IMG_MAP_MANAGER		= 1,
	IMG_MAP,
	IMG_MAP_GRATICULE,
	IMG_MAP_BASEMAP,
	IMG_SHAPES_POINT,
	IMG_SHAPES_POINTS,
	IMG_SHAPES_LINE,
	IMG_SHAPES_POLYGON,
	IMG_TIN,
	IMG_POINTCLOUD,
	IMG_GRID,
	IMG_GRIDS
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CWKSP_Map_Control, CWKSP_Base_Control)

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CWKSP_Map_Control, CWKSP_Base_Control)
	EVT_TREE_BEGIN_DRAG(ID_WND_WKSP_MAPS, CWKSP_Map_Control::On_Drag_Begin)
	EVT_TREE_END_DRAG  (ID_WND_WKSP_MAPS, CWKSP_Map_Control::On_Drag_End  )
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Map_Control *g_pMap_Ctrl = NULL;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Map_Control::CWKSP_Map_Control(wxWindow *pParent)
	: CWKSP_Base_Control(pParent, ID_WND_WKSP_MAPS)
{
	g_pMap_Ctrl = this;

	//-----------------------------------------------------
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_MAP_MANAGER   );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_MAP           );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_MAP_GRATICULE );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_MAP_BASEMAP   );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_POINT  );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_POINTS );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_LINE   );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_SHAPES_POLYGON);
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_TIN           );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_POINTCLOUD    );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_GRID          );
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_GRIDS         );

	//-----------------------------------------------------
	_Set_Manager(new CWKSP_Map_Manager);
}

//---------------------------------------------------------
CWKSP_Map_Control::~CWKSP_Map_Control(void)
{
	_Del_Item(m_pManager, true);

	g_pMap_Ctrl = NULL;
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Map_Control::OnCompareItems(const wxTreeItemId &item1, const wxTreeItemId &item2)
{
	CWKSP_Base_Item *p1, *p2;

	if( (p1 = (CWKSP_Base_Item *)GetItemData(item1)) != NULL && (p1->Get_Type() == WKSP_ITEM_Map_Layer || p1->Get_Type() == WKSP_ITEM_Map_Graticule || p1->Get_Type() == WKSP_ITEM_Map_BaseMap)
	&&  (p2 = (CWKSP_Base_Item *)GetItemData(item2)) != NULL && (p2->Get_Type() == WKSP_ITEM_Map_Layer || p2->Get_Type() == WKSP_ITEM_Map_Graticule || p2->Get_Type() == WKSP_ITEM_Map_BaseMap) )
	{
		return( p1->Get_Index() - p2->Get_Index() );
	}

	return( wxTreeCtrl::OnCompareItems(item1, item2) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
inline int CWKSP_Map_Control::_Get_Image_ID(CWKSP_Base_Item *pItem)
{
	if( pItem )
	{
		if( pItem->Get_Type() == WKSP_ITEM_Map_Graticule )
		{
			return( IMG_MAP_GRATICULE );
		}

		if( pItem->Get_Type() == WKSP_ITEM_Map_BaseMap )
		{
			return( IMG_MAP_BASEMAP );
		}

		if( pItem->Get_Type() == WKSP_ITEM_Map_Layer )
		{
			pItem = ((CWKSP_Map_Layer *)pItem)->Get_Layer();

			switch( pItem->Get_Type() )
			{
			case WKSP_ITEM_Grid        :	return( IMG_GRID );
			case WKSP_ITEM_Grids       :	return( IMG_GRIDS );
			case WKSP_ITEM_TIN         :	return( IMG_TIN );
			case WKSP_ITEM_PointCloud  :	return( IMG_POINTCLOUD );
			case WKSP_ITEM_Shapes      :
				switch( ((CWKSP_Shapes *)pItem)->Get_Shapes()->Get_Type() )
				{
				case SHAPE_TYPE_Point  :	return( IMG_SHAPES_POINT );
				case SHAPE_TYPE_Points :	return( IMG_SHAPES_POINTS );
				case SHAPE_TYPE_Line   :	return( IMG_SHAPES_LINE );
				case SHAPE_TYPE_Polygon:	return( IMG_SHAPES_POLYGON );
				default: break;
				}
			default: break;
			}
		}
	}

	return( 0 );
}

//---------------------------------------------------------
void CWKSP_Map_Control::Add_Item(CWKSP_Base_Manager *pManager, CWKSP_Base_Item *pItem)
{
	if( pManager && pItem )
	{
		if( pManager == Get_Manager() )
		{
			_Add_Item(pItem, IMG_MAP, IMG_MAP);

			return;
		}

		AppendItem	(pManager->GetId(), pItem->Get_Name(), _Get_Image_ID(pItem), _Get_Image_ID(pItem), pItem);
		SortChildren(pManager->GetId());
		Expand		(pManager->GetId());

		if( g_pMap_Buttons ) { g_pMap_Buttons->Update_Buttons(); }
	}
}

//---------------------------------------------------------
bool CWKSP_Map_Control::Del_Item(CWKSP_Map *pMap, CWKSP_Base_Item *pItem)
{
	if( pMap && pItem )
	{
		switch( pItem->Get_Type() )
		{
		case WKSP_ITEM_Map_Graticule:
		case WKSP_ITEM_Map_BaseMap  :
			break;

		case WKSP_ITEM_Map_Layer    :
			pItem = pMap->Get_Map_Layer(((CWKSP_Map_Layer *)pItem)->Get_Layer());
			break;

		default:
			pItem = pMap->Get_Map_Layer(((CWKSP_Layer     *)pItem));
			break;
		}

		bool bRefresh = pMap->Get_Count() > 1;

		if( pItem && _Del_Item(pItem, true) )
		{
			if( bRefresh )
			{
				pMap->View_Refresh(false);
			}

			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Map_Control::On_Drag_Begin(wxTreeEvent &event)
{
	CWKSP_Base_Item *pItem = (CWKSP_Base_Item *)GetItemData(event.GetItem());

	if( pItem
	&& (pItem->Get_Type() == WKSP_ITEM_Map_Layer
	 || pItem->Get_Type() == WKSP_ITEM_Map_Graticule
	 || pItem->Get_Type() == WKSP_ITEM_Map_BaseMap) )
	{
		m_draggedItem	= event.GetItem();

		event.Allow();
	}
}

//---------------------------------------------------------
void CWKSP_Map_Control::On_Drag_End(wxTreeEvent &event)
{
	if( event.GetItem().IsOk()
	&& (((CWKSP_Base_Item *)GetItemData(m_draggedItem))->Get_Type() == WKSP_ITEM_Map_Layer
	 || ((CWKSP_Base_Item *)GetItemData(m_draggedItem))->Get_Type() == WKSP_ITEM_Map_Graticule
	 || ((CWKSP_Base_Item *)GetItemData(m_draggedItem))->Get_Type() == WKSP_ITEM_Map_BaseMap ) )
	{
		CWKSP_Base_Item *pDst = (CWKSP_Base_Item *)GetItemData(event.GetItem());
		CWKSP_Base_Item *pSrc = (CWKSP_Base_Item *)GetItemData(m_draggedItem), *pCpy;

		CWKSP_Map *pDst_Map, *pSrc_Map = (CWKSP_Map *)pSrc->Get_Manager();

		switch( pDst->Get_Type() )
		{
		default:
			pDst_Map = NULL;
			break;

		case WKSP_ITEM_Map_Layer    :
		case WKSP_ITEM_Map_Graticule:
		case WKSP_ITEM_Map_BaseMap  :
			pDst_Map = (CWKSP_Map *)pDst->Get_Manager();
			break;

		case WKSP_ITEM_Map          :
			pDst_Map = (CWKSP_Map *)pDst;
			pDst     = NULL;
			break;
		}

		if( pDst_Map )
		{
			Freeze();

			if( pDst_Map == pSrc_Map )
			{
				pDst_Map->Move_To(pSrc, pDst);

				pDst_Map->View_Refresh(false);
			}
			else if( (pCpy = pDst_Map->Add_Copy(pSrc)) != NULL )
			{
				pDst_Map->Move_To(pCpy, pDst);

				if( pCpy && !wxGetKeyState(WXK_CONTROL) )
				{
					Del_Item(pSrc_Map, pSrc);
				}

				pDst_Map->View_Refresh(false);
			}

			Thaw();
		}
	}

	m_draggedItem = (wxTreeItemId)0l;
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Map_Control::Close(bool bSilent)
{
	return( _Del_Item(m_pManager, bSilent) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
