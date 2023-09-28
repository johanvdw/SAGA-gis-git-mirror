#! /usr/bin/env python

#################################################################################
# MIT License

# Copyright (c) 2023 Olaf Conrad

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#################################################################################
# Purpose
#################################################################################

'''
The PySAGA.helper module provides a collection of general helper functions.
'''


#################################################################################
#
# Globals
#________________________________________________________________________________

from PySAGA import saga_api


#################################################################################
#
# Defining the Areas of Interest...
#________________________________________________________________________________

#################################################################################
#________________________________________________________________________________
def Get_AOI_From_Extent(Xmin, Xmax, Ymin, Ymax, EPSG=4326):
    '''
    Create area of interest from extent coordinates and EPSG code for CRS.
    '''

    AOI = saga_api.CSG_Shapes(saga_api.SHAPE_TYPE_Polygon)
    AOI.Get_Projection().Create(EPSG)
    Shape = AOI.Add_Shape()
    Shape.Add_Point(Xmin, Ymin)
    Shape.Add_Point(Xmin, Ymax)
    Shape.Add_Point(Xmax, Ymax)
    Shape.Add_Point(Xmax, Ymin)
    return AOI


#################################################################################
#________________________________________________________________________________
def Get_AOI_From_Features(File):
    '''
    Create area of interest from file. Expects to represent vector data with CRS
    information set correctly.
    '''

    AOI = saga_api.SG_Create_Shapes(File)
    if not AOI:
        saga_api.SG_UI_Msg_Add_Error('failed to load AOI from file \n\t\'{:s}\''.format(File))
        return None

    if not AOI.Get_Projection().is_Okay():
        del(AOI)
        saga_api.SG_UI_Msg_Add_Error('coordinate reference system of AOI is not defined \n\t\'{:s}\''.format(File))
        return None

    return AOI


#################################################################################
#________________________________________________________________________________
def Get_AOI_From_Raster(File):
    '''
    Create area of interest from file. Expects to represent raster data with CRS
    information set correctly.
    '''

    Grid = saga_api.SG_Create_Grid(File)
    if not Grid:
        saga_api.SG_UI_Msg_Add_Error('failed to load AOI from file \n\t\'{:s}\''.format(File))
        return None

    if not Grid.Get_Projection().is_Okay():
        del(Grid)
        saga_api.SG_UI_Msg_Add_Error('coordinate reference system of AOI is not defined \n\t\'{:s}\''.format(File))
        return None

    AOI = saga_api.CSG_Shapes(saga_api.SHAPE_TYPE_Polygon)
    AOI.Get_Projection().Create(Grid.Get_Projection())
    Shape = AOI.Add_Shape()
    Shape.Add_Point(Grid.Get_XMin(), Grid.Get_YMin())
    Shape.Add_Point(Grid.Get_XMin(), Grid.Get_YMax())
    Shape.Add_Point(Grid.Get_XMax(), Grid.Get_YMax())
    Shape.Add_Point(Grid.Get_XMax(), Grid.Get_YMin())
    del(Grid)
    return AOI


#################################################################################
#
# Virtual Raster Tiles...
#________________________________________________________________________________

#################################################################################
#________________________________________________________________________________
def Get_VRT(Directory, Extension = 'tif', VRT_Name = 'tiles', VRT_Folder = None):
    '''
    This function creates or updates the virtual raster (VRT) file named 'File'
    using all raster tiles with specified *Extension* found in *Directory*.
    If *VRT_Folder* is None resulting VRT file will be stored in the same directory
    as the tiles.
    Returns the full file path to the VRT file on success or None otherwise.
    '''

    import glob; Files = glob.glob('{:s}/*.{:s}'.format(Directory, Extension))
    if len(Files) < 1:
        saga_api.SG_UI_Console_Print_StdErr('directory \'{:s}\' does not contain any file with extension \'{:s}\'!'.format(Directory, Extension))
        return None

    Tiles = ''
    for File in Files:
        Tiles += '\"{:s}\" '.format(File)

    Tool = saga_api.SG_Get_Tool_Library_Manager().Get_Tool('io_gdal', '12')
    if not Tool:
        saga_api.SG_UI_Console_Print_StdErr('Failed to request tool: Create Virtual Raster (VRT)')
        return None

    if not VRT_Folder:
        VRT_Folder = Directory

    Tool.Reset()
    Tool.Set_Parameter('FILES'   , Tiles)
    Tool.Set_Parameter('VRT_NAME', '{:s}/{:s}.vrt'.format(VRT_Folder, VRT_Name))

    saga_api.SG_UI_ProgressAndMsg_Lock(True)
    if not Tool.Execute():
        saga_api.SG_UI_ProgressAndMsg_Lock(False)
        saga_api.SG_UI_Console_Print_StdErr('failed to execute tool: ' + Tool.Get_Name().c_str())
        return None

    saga_api.SG_UI_ProgressAndMsg_Lock(False)
    return Tool.Get_Parameter("VRT_NAME").asString()


#################################################################################
#
# SAGA / Python Type Conversations...
#________________________________________________________________________________

#________________________________________________________________________________
def CSG_Grids_asPyArray(Grids):
    '''
    Create a Python list that provides access to the single grids of the
    input grid collection.

    Arguments
    ---------
    Grids : expects a `grid collection` of type `saga_api`.`CSG_Grids`

    Returns
    ---------
    list : a list with `saga_api`.`CSG_Grid` objects
    '''
    Array = []
    for i in range(0, Grids.Get_Grid_Count()):
        Array.append(Grids.Get_Grid(i));
    return Array


#################################################################################
#
# Tools and Tool Execution...
#________________________________________________________________________________

#________________________________________________________________________________
def Create_Toolboxes(Path='', SingleFile=False, UseToolName=True, Clean=True):
    '''
    PySAGA Tool Interface Generator
    ----------
    Create the Python interface for SAGA tools.

    Arguments
    ----------
    - Path [`string`] : either the single target file name or the target folder
    - SingleFile [`boolean`] : if `True` a single Python module will be created for all function calls, else one module for each tool library will be created
    - UseToolName [`boolean`]: if `True` function names will be based on the tool names, else these will named from `library` + `tool id`
    - Clean [boolean]: if `True` and not a single file is targeted all files from the target folder will be deleted before the new ones are generated

    Returns
    ----------
    `boolean` : `True` on success, `False` on failure.
    '''

    return saga_api.SG_Get_Tool_Library_Manager().Create_Python_ToolBox(saga_api.CSG_String(Path), Clean, UseToolName, SingleFile)


#########################################
#
# class Tool_Wrapper
#
#________________________________________
class Tool_Wrapper:
    '''
    The Tool_Wrapper class is smart wrapper for SAGA tools facilitating tool execution.
    '''

    Tool   = None
    Input  = []
    Output = []

    #____________________________________
    def __init__(self, Library, Tool, Expected):
        self.Tool = saga_api.SG_Get_Tool_Library_Manager().Create_Tool(Library, Tool)
        if not self.Tool:
            saga_api.SG_UI_Msg_Add_Error('failed to request tool: {:s}'.format(Expected))
        else:
            self.Tool.Create_Manager()

    #____________________________________
    def __del__(self):
        self.Destroy()

    #____________________________________
    def Destroy(self):
        if self.is_Okay():
            self.Tool.Delete_Manager(True) # detach only, should be nothing but input!
            saga_api.SG_Get_Tool_Library_Manager().Delete_Tool(self.Tool)

        self.Tool = None
        self.Input .clear()
        self.Output.clear()

    #____________________________________
    def is_Okay(self):
        return self.Tool != None

    #____________________________________
    def Set_Input(self, ID, Object):
        if self.is_Okay() and Object:
            Parameter = self.Tool.Get_Parameter(ID)
            if Parameter and Parameter.is_Input():
                if Parameter.is_DataObject():
                    self.Input.append(Object)
                    self.Tool.Get_Manager().Add(Object)
                    Parameter.Set_Value(Object)
                elif Parameter.is_DataObject_List():
                    for Item in Object:
                        self.Input.append(Item)
                        self.Tool.Get_Manager().Add(Item)
                        Parameter.asList().Add_Item(Item)

    #____________________________________
    def Set_Output(self, ID, Object):
        if self.is_Okay() and Object != None:
            Parameter = self.Tool.Get_Parameter(ID)
            if Parameter and Parameter.is_Output():
                if Parameter.is_DataObject() and Parameter.is_Optional():
                    Parameter.Set_Value(saga_api.SG_Get_Create_Pointer())
                self.Output.append([Object, ID])

    #____________________________________
    def Set_Option(self, ID, Value):
        if self.is_Okay() and Value != None:
            self.Tool.Set_Parameter(ID, Value)

    #____________________________________
    def Execute(self):
        def _Copy_Output(Object, Parameter):
            if Object.Get_ObjectType() == saga_api.SG_DATAOBJECT_TYPE_Grid:
                Object.asGrid().Create(Parameter.asGrid())
            if Object.Get_ObjectType() == saga_api.SG_DATAOBJECT_TYPE_Grids:
                Object.asGrids().Create(Parameter.asGrids())
            if Object.Get_ObjectType() == saga_api.SG_DATAOBJECT_TYPE_Table:
                Object.asTable().Create(Parameter.asTable())
            if Object.Get_ObjectType() == saga_api.SG_DATAOBJECT_TYPE_Shapes:
                Object.asShapes().Create(Parameter.asShapes())
            if Object.Get_ObjectType() == saga_api.SG_DATAOBJECT_TYPE_PointCloud:
                Object.asPointCloud().Create(Parameter.asPointCloud())
            if Object.Get_ObjectType() == saga_api.SG_DATAOBJECT_TYPE_TIN:
                Object.asTIN().Create(Parameter.asTIN())

        def _Process_Output(Save):
            for Data in self.Output:
                Parameter = self.Tool.Get_Parameter(Data[1])
                if Parameter.is_DataObject():
                    if Parameter.asDataObject() and Parameter.asDataObject() != saga_api.SG_Get_Create_Pointer():
                        if Save:
                            _Copy_Output(Data[0], Parameter.asDataObject())
                        self.Tool.Get_Manager().Delete(Parameter.asDataObject(), False)
                elif Parameter.is_DataObject_List():
                    for i in range(0, Parameter.asList().Get_Item_Count()):
                        Item = Parameter.asList().Get_Item(i)
                        if Save:
                            Data[0].append(Item)
                        self.Tool.Get_Manager().Delete(Item, Save)

        if self.is_Okay():
            if self.Tool.Execute():
                _Process_Output(True)
                self.Destroy()
                return True

            saga_api.SG_UI_Msg_Add_Error('failed to execute tool: ' + self.Tool.Get_Name().c_str())
            _Process_Output(False)
            self.Destroy()
        return False

#________________________________________
#
# class Tool_Wrapper
#
#########################################


#################################################################################
#
#________________________________________________________________________________