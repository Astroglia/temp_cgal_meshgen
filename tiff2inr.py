import numpy as np
from tifffile import imread
from os import listdir
from os.path import isfile, join, splitext

#### from pygalmesh

def saveinr(vol, h, fname):
    """
    Save a volume (described as a numpy array) to INR format.
    Code inspired by iso2mesh (http://iso2mesh.sf.net) by Q. Fang
    INPUTS:
    - vol: volume as numpy array
    - h: voxel sizes as list or numpy array
    - fname: filename for saving the inr file
    """

    fid = open(fname, "wb")
    vol_type = vol.dtype

    if vol_type == "uint8":
        btype = "unsigned fixed"
        bitlen = 8
    elif vol_type == "uint16":
        btype = "unsigned fixed"
        bitlen = 16
    elif vol_type == "float32":
        btype = "float"
        bitlen = 32
    elif vol_type == "float64":
        btype = "float"
        bitlen = 64
    else:
        raise (vol_type)

    header = (
        "#INRIMAGE-4#{8:s}\nXDIM={0:d}\nYDIM={1:d}\nZDIM={2:d}\nVDIM=1\nTYPE={3:s}\n"
        + "PIXSIZE={4:d} bits\nCPU=decm\nVX={5:f}\nVY={6:f}\nVZ={7:f}\n"
    ).format(*vol.shape, btype, bitlen, h[0], h[1], h[2], "{")

    header = header + "\n" * (256 - 4 - len(header)) + "##}\n"
    fid.write(header.encode("ascii"))
    fid.write(vol.tobytes(order="F"))

base_filepath = './'
filename = 'test_aligned.tiff'

image = imread( base_filepath + filename )
saveinr( image, [1.0]*3, 'test_aligned.inr')