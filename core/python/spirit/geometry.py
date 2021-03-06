import spirit.spiritlib as spiritlib
import ctypes

### Load Library
_spirit = spiritlib.LoadSpiritLibrary()

### Imports
from spirit.scalar import scalar
from spirit import system
# import spirit.system as system

import numpy as np

### Get Bounds
_Get_Bounds          = _spirit.Geometry_Get_Bounds
_Get_Bounds.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), 
                        ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_int]
_Get_Bounds.restype  = None
def Get_Bounds(p_state, idx_image=-1, idx_chain=-1):
    _min = (3*ctypes.c_float)()
    _max = (3*ctypes.c_float)()
    _Get_Bounds(ctypes.c_void_p(p_state), _min, _max, 
                ctypes.c_int(idx_image), ctypes.c_int(idx_chain))
    return [_min[i] for i in range(3)], [_max[i] for i in range(3)] 

### Get Center
_Get_Center          = _spirit.Geometry_Get_Center
_Get_Center.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), ctypes.c_int, ctypes.c_int]
_Get_Center.restype  = None
def Get_Center(p_state, idx_image=-1, idx_chain=-1):
    _center = (3*ctypes.c_float)()
    _Get_Center(ctypes.c_void_p(p_state), _center, ctypes.c_int(idx_image), ctypes.c_int(idx_chain))
    return [_center[i] for i in range(3)]

### Get Basis vectors
_Get_Basis_Vectors          = _spirit.Geometry_Get_Basis_Vectors
_Get_Basis_Vectors.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), 
                               ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), 
                               ctypes.c_int, ctypes.c_int]
_Get_Basis_Vectors.restype  = None
def Get_Basis_Vectors(p_state, idx_image=-1, idx_chain=-1):
    _a = (3*ctypes.c_float)()
    _b = (3*ctypes.c_float)()
    _c = (3*ctypes.c_float)()
    _Get_Basis_Vectors(ctypes.c_void_p(p_state), _a, _b, _c, 
                       ctypes.c_int(idx_image), ctypes.c_int(idx_chain))
    return [_a[i] for i in range(3)], [_b[i] for i in range(3)], [_c[i] for i in range(3)]
    
### Get N Cells
_Get_N_Cells          = _spirit.Geometry_Get_N_Cells
_Get_N_Cells.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
_Get_N_Cells.restype  = None
def Get_N_Cells(p_state, idx_image=-1, idx_chain=-1):
    n_cells = (3*ctypes.c_int)()
    _Get_N_Cells(ctypes.c_void_p(p_state), n_cells, ctypes.c_int(idx_image), ctypes.c_int(idx_chain))
    return [n_cells[i] for i in range(3)]

### Get Translation Vectors
_Get_Translation_Vectors          = _spirit.Geometry_Get_Translation_Vectors
_Get_Translation_Vectors.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float), 
                                     ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float), 
                                     ctypes.c_int, ctypes.c_int]
_Get_Translation_Vectors.restype  = None
def Get_Translation_Vectors(p_state, idx_image=-1, idx_chain=-1):
    ta = (3*ctypes.c_float)()
    tb = (3*ctypes.c_float)()
    tc = (3*ctypes.c_float)()
    _Get_Translation_Vectors(ctypes.c_void_p(p_state), ta, tb, tc, 
                             ctypes.c_int(idx_image), ctypes.c_int(idx_chain))
    return ta, tb, tc

### Get Translation Vectors
_Get_Dimensionality          = _spirit.Geometry_Get_Dimensionality
_Get_Dimensionality.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
_Get_Dimensionality.restype  = ctypes.c_int
def Get_Dimensionality(p_state, idx_image=-1, idx_chain=-1):
    return int(_Get_Dimensionality(ctypes.c_void_p(p_state), ctypes.c_int(idx_image), 
                                   ctypes.c_int(idx_chain)))

### Get Pointer to Spin Positions
# NOTE: Changing the values of the array_view one can alter the value of the data of the state
_Get_Spin_Positions            = _spirit.Geometry_Get_Spin_Positions
_Get_Spin_Positions.argtypes   = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
_Get_Spin_Positions.restype    = ctypes.POINTER(scalar)
def Get_Spin_Positions(p_state, idx_image=-1, idx_chain=-1):
    nos = system.Get_NOS(p_state, idx_image, idx_chain)
    ArrayType = scalar*3*nos
    Data = _Get_Spin_Positions(ctypes.c_void_p(p_state), 
                               ctypes.c_int(idx_image), ctypes.c_int(idx_chain))
    array_pointer = ctypes.cast(Data, ctypes.POINTER(ArrayType))
    array = np.frombuffer(array_pointer.contents)
    array_view = array.view()
    array_view.shape = (nos, 3)
    return array_view

### Get Pointer to atom types
# NOTE: Changing the values of the array_view one can alter the value of the data of the state
_Get_Atom_Types            = _spirit.Geometry_Get_Atom_Types
_Get_Atom_Types.argtypes   = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
_Get_Atom_Types.restype    = ctypes.POINTER(ctypes.c_int)
def Get_Atom_Types(p_state, idx_image=-1, idx_chain=-1):
    nos = system.Get_NOS(p_state, idx_image, idx_chain)
    ArrayType = ctypes.c_int*nos
    Data = _Get_Atom_Types(ctypes.c_void_p(p_state), 
                           ctypes.c_int(idx_image), ctypes.c_int(idx_chain))
    array_pointer = ctypes.cast(Data, ctypes.POINTER(ArrayType))
    array = np.frombuffer(array_pointer.contents, dtype=ctypes.c_int)
    array_view = array.view()
    return array_view