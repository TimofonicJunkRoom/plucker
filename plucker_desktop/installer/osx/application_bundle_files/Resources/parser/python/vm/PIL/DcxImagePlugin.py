#
# The Python Imaging Library.
# $Id: DcxImagePlugin.py,v 1.1 2003/06/18 18:04:20 robertoconnor Exp $
#
# DCX file handling
#
# DCX is a container file format defined by Intel, commonly used
# for fax applications.  Each DCX file consists of a directory
# (a list of file offsets) followed by a set of (usually 1-bit)
# PCX files.
#
# History:
#       95-09-09 fl     Created
#       96-03-20 fl     Properly derived from PcxImageFile.
#       98-07-15 fl     Renamed offset attribute to avoid name clash
#
# Copyright (c) Secret Labs AB 1997-98.
# Copyright (c) Fredrik Lundh 1995-96.
#
# See the README file for information on usage and redistribution.
#


__version__ = "0.1"


import Image, ImageFile

from PcxImagePlugin import PcxImageFile


MAGIC = 0x3ADE68B1 # QUIZ: what's this value, then?


def i32(c):
    return ord(c[0]) + (ord(c[1])<<8) + (ord(c[2])<<16) + (ord(c[3])<<24)

def _accept(prefix):
    return i32(prefix) == MAGIC

class DcxImageFile(PcxImageFile):

    format = "DCX"
    format_description = "Intel DCX"

    def _open(self):

        # Header
        s = self.fp.read(4)
        if i32(s) != MAGIC:
            raise SyntaxError, "not a DCX file"

        # Component directory
        self._offset = []
        for i in range(1024):
            offset = i32(self.fp.read(4))
            if not offset:
                break
            self._offset.append(offset)

        self.seek(0)

    def seek(self, frame):
        if frame >= len(self._offset):
            raise IndexError, "attempt to seek outside DCX directory"
        self.frame = frame
        self.fp.seek(self._offset[frame])
        PcxImageFile._open(self)

    def tell(self):
        return self.frame


Image.register_open("DCX", DcxImageFile, _accept)

Image.register_extension("DCX", ".dcx")
