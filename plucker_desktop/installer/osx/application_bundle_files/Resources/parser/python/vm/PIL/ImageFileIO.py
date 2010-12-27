#
# The Python Imaging Library.
# $Id: ImageFileIO.py,v 1.1 2003/06/18 18:04:20 robertoconnor Exp $
#
# kludge to get basic ImageFileIO functionality
#
# History:
# 98-08-06 fl   Recreated
#
# Copyright (c) Secret Labs AB 1998.
#
# See the README file for information on usage and redistribution.
#

from StringIO import StringIO

# this module is deprecated

class ImageFileIO(StringIO):
    def __init__(self, fp):
        data = fp.read()
        StringIO.__init__(self, data)

if __name__ == "__main__":

    import Image
    fp = open("/images/clenna.im", "rb")
    im = Image.open(ImageFileIO(fp))
    im.load() # make sure we can read the raster data
    print im.mode, im.size
