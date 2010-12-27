#
# The Python Imaging Library.
# $Id: IptcImagePlugin.py,v 1.1 2003/06/18 18:04:21 robertoconnor Exp $
#
# IPTC/NAA file handling
#
# history:
# 95-10-01 fl   Created
# 98-03-09 fl   Cleaned up and added to PIL
#
# Copyright (c) Secret Labs AB 1997-2000.
# Copyright (c) Fredrik Lundh 1995.
#
# See the README file for information on usage and redistribution.
#


__version__ = "0.2"


import Image, ImageFile
import os, tempfile


COMPRESSION = {
    1: "raw",
    5: "jpeg"
}

PAD = chr(0) * 4

#
# Helpers

def i16(c):
    return ord(c[1]) + (ord(c[0])<<8)

def i32(c):
    return ord(c[3]) + (ord(c[2])<<8) + (ord(c[1])<<16) + (ord(c[0])<<24)

def i(c):
    return i32((PAD + c)[-4:])

def dump(c):
    for i in c:
        print "%02x" % ord(i),
    print


#
# Parser

class IptcImageFile(ImageFile.ImageFile):

    format = "IPTC"
    format_description = "IPTC/NAA"

    def getint(self, key):
        return i(self.info[key])

    def field(self):
        #
        # get a IPTC field header
        s = self.fp.read(5)
        if not len(s):
            return None, 0

        tag = ord(s[1]), ord(s[2])

        # syntax
        if ord(s[0]) != 0x1C or tag[0] < 1 or tag[0] > 9:
            raise SyntaxError, "invalid IPTC/NAA file"

        # field size
        size = ord(s[3])
        if size > 132:
            raise IOError, "illegal field length in IPTC/NAA file"
        elif size == 128:
            size = 0
        elif size > 128:
            size = i(self.fp.read(size-128))
        else:
            size = i16(s[3:])

        return tag, size

    def _is_raw(self, offset, size):
        #
        # check if the file can be mapped

        # DISABLED: the following only slows things down...
        return 0

        self.fp.seek(offset)
        t, sz = self.field()
        if sz != size[0]:
            return 0
        y = 1
        while 1:
            self.fp.seek(sz, 1)
            t, s = self.field()
            if t != (8, 10):
                break
            if s != sz:
                return 0
            y = y + 1
        return y == size[1]

    def _open(self):

        # load descriptive fields
        while 1:
            offset = self.fp.tell()
            tag, size = self.field()
            if not tag or tag == (8,10):
                break
            if size:
                self.info[tag] = self.fp.read(size)
            else:
                self.info[tag] = None
            # print tag, self.info[tag]

        # mode
        layers = ord(self.info[(3,60)][0])
        component = ord(self.info[(3,60)][1])
        if self.info.has_key((3,65)):
            id = ord(self.info[(3,65)][0])-1
        else:
            id = 0
        if layers == 1 and not component:
            self.mode = "L"
        elif layers == 3 and component:
            self.mode = "RGB"[id]
        elif layers == 4 and component:
            self.mode = "CMYK"[id]

        # size
        self.size = self.getint((3,20)), self.getint((3,30))

        # compression
        try:
            compression = COMPRESSION[self.getint((3,120))]
        except KeyError:
            raise IOError, "Unknown IPTC image compression"

        # tile
        if tag == (8,10):
            if compression == "raw" and self._is_raw(offset, self.size):
                self.tile = [(compression, (offset, size + 5, -1),
                             (0, 0, self.size[0], self.size[1]))]
            else:
                self.tile = [("iptc", (compression, offset),
                             (0, 0, self.size[0], self.size[1]))]

    def load(self):

        if len(self.tile) != 1 or self.tile[0][0] != "iptc":
            return ImageFile.ImageFile.load(self)

        type, tile, box = self.tile[0]

        encoding, offset = tile

        self.fp.seek(offset)

        # Copy image data to temporary file
        outfile = tempfile.mktemp()
        o = open(outfile, "wb")
        if encoding == "raw":
            # To simplify access to the extracted file,
            # prepend a PPM header
            o.write("P5\n%d %d\n255\n" % self.size)
        while 1:
            type, size = self.field()
            if type != (8, 10):
                break
            while size > 0:
                s = self.fp.read(min(size, 8192))
                if not s:
                    break
                o.write(s)
                size = size - len(s)
        o.close()

        try:
            try:
                # fast
                self.im = Image.core.open_ppm(outfile)
            except:
                # slightly slower
                im = Image.open(outfile)
                im.load()
                self.im = im.im
        finally:
            try: os.unlink(outfile)
            except: pass


Image.register_open("IPTC", IptcImageFile)

Image.register_extension("IPTC", ".iim")
