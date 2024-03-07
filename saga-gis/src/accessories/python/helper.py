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
# Prints...
#________________________________________________________________________________

#################################################################################
#________________________________________________________________________________
def Print_Table_Fields(Table):
    '''
    Print the attribute fields of a saga_api.CSG_Table object.

    Arguments
    ---------
    Table : the table object of type `saga_api`.`CSG_Table`

    Returns
    ---------
    boolean : True on success
    '''
    try:
        Table.Get_Field_Count()
    except:
        return False
    print()
    print('=====================')
    print('{:s} [{:d} fields, {:d} records]'.format(Table.Get_Name(), Table.Get_Field_Count(), Table.Get_Count()))
    print('_____________________')
    for Field in range(0, Table.Get_Field_Count()):
        print('{:2d}. {:s} [{:s}]'.format(Field + 1, Table.Get_Field_Name(Field), saga_api.SG_Data_Type_Get_Name(Table.Get_Field_Type(Field)).c_str()))
    print('=====================')
    return True


#################################################################################
#________________________________________________________________________________
def Print_Table(Table, Separator='\t', Fields=[], NoData='---', Maximum=0):
    '''
    Print the contents of a saga_api.CSG_Table object.

    Arguments
    ---------
    Table : the table object of type `saga_api`.`CSG_Table`
    Separator : `string` used to separate fields
    Fields : list with field indices (0 based) to be printed.
    Prints all fields if empty or None (default)
    NoData : prints this string for no-data values, or the
    original (no-data) value if `None`
    Maximum : maximum number of records to become printed.
    Prints all records if zero (default)

    Returns
    ---------
    boolean : True on success
    '''
    try:
        Table.Get_Field_Count()
    except:
        return False
    print()
    print('=====================')
    print('{:s} [{:d} fields, {:d} records]'.format(Table.Get_Name(), Table.Get_Field_Count(), Table.Get_Count()))
    print('_____________________')
    if Maximum < 1 or Maximum > Table.Get_Count():
        nRecords = Table.Get_Count()
    else:
        nRecords = Maximum
    if Fields and len(Fields) > 0:
        _Separator = ''
        for Field in Fields:
            if Field >= 0 and Field < Table.Get_Field_Count():
                print(_Separator + Table.Get_Field_Name(Field), end=''); _Separator = Separator
        print('\n_____________________')
        for i in range(0, nRecords):
            Record = Table.Get_Record(i); _Separator = ''
            for Field in Fields:
                if Field >= 0 and Field < Table.Get_Field_Count():
                    if NoData != None and Record.is_NoData(Field):
                        print(_Separator + NoData, end='')
                    else:
                        print(_Separator + Record.asString(Field), end='')
                    _Separator = Separator
            print()
    else:
        for Field in range(0, Table.Get_Field_Count()):
            if Field > 0:
                print(Separator, end='')
            print(Table.Get_Field_Name(Field), end='')
        print('\n_____________________')
        for i in range(0, nRecords):
            Record = Table.Get_Record(i)
            for Field in range(0, Table.Get_Field_Count()):
                if Field > 0:
                    print(Separator, end='')
                if NoData != None and Record.is_NoData(Field):
                    print(NoData, end='')
                else:
                    print(Record.asString(Field), end='')
            print()
    print('=====================')
    return True



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
def PyList_from_Grid_Collection(Grids):
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

    List = []
    for i in range(0, Grids.Get_Grid_Count()):
        List.append(Grids.Get_Grid(i))
    return List

#________________________________________________________________________________
def PyList_to_Grid_Collection(List, Name='Grid Collection'):
    '''
    Create a grid collection from single grids provided by a Python list.

    Arguments
    ---------
    List : a Python list with objects of type `saga_api`.`CSG_Grid`

    Returns
    ---------
    `saga_api`.`CSG_Grids` : a grid collection of type `saga_api`.`CSG_Grids`
    '''

    Grids = saga_api.CSG_Grids()
    Grids.Fmt_Name(Name)
    for i in range(0, len(List)):
        Grids.Add_Grid(i, List[i].asGrid())
    return Grids


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
    - Path [`string`] : either the single target file name or the target folder. If empty function tries to install targets in PySAGA directory (this file's directory).
    - SingleFile [`boolean`] : if `True` a single Python module will be created for all function calls, else one module for each tool library will be created
    - UseToolName [`boolean`]: if `True` function names will be based on the tool names, else these will named from `library` + `tool id`
    - Clean [`boolean`]: if `True` and not a single file is targeted all files from the target folder will be deleted before the new ones are generated

    Returns
    ----------
    `boolean` : `True` on success, `False` on failure.
    '''

    if not Path:
        import os; Path = os.path.dirname(__file__) + '/tools'
        if SingleFile:
            Path = Path + '.py'

    return saga_api.SG_Get_Tool_Library_Manager().Create_Python_ToolBox(saga_api.CSG_String(Path), Clean, UseToolName, SingleFile)


#################################################################################
#
# class Tool_Wrapper
#
#________________________________________________________________________________
class Tool_Wrapper:
    '''
    The Tool_Wrapper class is a smart wrapper for SAGA tools facilitating tool execution.
    '''

    Tool = None; Input = []; Output = []; Options = []

    #____________________________________
    def __init__(self, Library, Tool, Expected):
        self.Tool = saga_api.SG_Get_Tool_Library_Manager().Create_Tool(Library, Tool)
        if not self.Tool:
            saga_api.SG_UI_Msg_Add_Error('failed to request tool: {:s}'.format(Expected))
        else:
            self.Tool.Set_Manager(None)

    #____________________________________
    def __del__(self):
        self.Destroy()

    #____________________________________
    def Destroy(self):
        if self.is_Okay():
            saga_api.SG_Get_Tool_Library_Manager().Delete_Tool(self.Tool)
        self.Tool = None; self.Input.clear(); self.Output.clear(); self.Options.clear()

    #____________________________________
    def is_Okay(self):
        return self.Tool != None

    #____________________________________
    def Set_Input(self, ID, Object):
        if self.is_Okay() and Object:
            self.Input.append([ID, Object])

    #____________________________________
    def Set_Output(self, ID, Object):
        if self.is_Okay() and Object:
            self.Output.append([ID, Object])

    #____________________________________
    def Set_Option(self, ID, Value):
        if self.is_Okay() and Value != None:
            self.Options.append([ID, Value])

    #____________________________________
    def Execute(self, Verbose=2):
        if Verbose >= 2 and saga_api.SG_UI_Msg_is_Locked():
            Verbose = 1

        #________________________________
        def _Initialize():
            if Verbose <= 1:
                saga_api.SG_UI_ProgressAndMsg_Lock(True)
                if Verbose == 1:
                    saga_api.SG_UI_Console_Print_StdOut('[{:s}] execution... '.format(self.Tool.Get_Name().c_str()), '')

            if not self.Tool.On_Before_Execution():
                return False

            for Option in self.Options: # 1st pass -> dis-/enabling data object parameters
                self.Tool.Set_Parameter(Option[0], Option[1])

            for Data in self.Input:
                Parameter = self.Tool.Get_Parameter(Data[0])
                if Parameter and Parameter.is_Input():
                    if Parameter.is_DataObject():
                        Parameter.Set_Value(Data[1])
                    elif Parameter.is_DataObject_List():
                        for Item in Data[1]:
                            Parameter.asList().Add_Item(Item)

            for Option in self.Options: # 2nd pass -> set options that are dependent of input data (-> table fields)
                self.Tool.Set_Parameter(Option[0], Option[1])

            for Data in self.Output:
                Parameter = self.Tool.Get_Parameter(Data[0])
                if Parameter and Parameter.is_Output() and Parameter.is_DataObject():
                    Parameter.Set_Value(Data[1])

            return True

        #________________________________
        def _Finalize(Success):
            self.Tool.On_After_Execution()

            for Data in self.Output:
                Parameter = self.Tool.Get_Parameter(Data[0])
                if Parameter and Parameter.is_Output():
                    if Parameter.is_DataObject():
                        if Parameter.asDataObject() and not Data[1]: # delete output created by tool but not requested by user
                            saga_api.SG_Data_Object_Delete(Parameter.asDataObject())
                    elif Parameter.is_DataObject_List():
                        if Data[1] != None:
                            Data[1].clear() # should be empty Python list to become filled with the results now
                        for i in range(0, Parameter.asList().Get_Item_Count()):
                            Item = Parameter.asList().Get_Item(i)
                            if Data[1] != None and Success:
                                Data[1].append(Item)
                            else:
                                saga_api.SG_Data_Object_Delete(Item)

            self.Tool.Reset(); self.Input.clear(); self.Output.clear()

            if Verbose <= 1:
                saga_api.SG_UI_ProgressAndMsg_Lock(False)
                if Verbose == 1:
                    if Success:
                        saga_api.SG_UI_Console_Print_StdOut('succeeded')
                    else:
                        saga_api.SG_UI_Console_Print_StdOut('failed')

        #________________________________
        if self.is_Okay():
            if _Initialize() and self.Tool.Execute():
                _Finalize(True)
                return True
            _Finalize(False)
        return False

#________________________________________________________________________________
#
# class Tool_Wrapper
#
#################################################################################


#################################################################################
#
#________________________________________________________________________________
