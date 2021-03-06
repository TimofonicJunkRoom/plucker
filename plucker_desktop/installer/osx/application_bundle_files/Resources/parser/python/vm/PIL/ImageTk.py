#
# The Python Imaging Library.
# $Id: ImageTk.py,v 1.1 2003/06/18 18:04:20 robertoconnor Exp $
#
# a Tk display interface
#
# History:
# 96-04-08 fl   Created
# 96-09-06 fl   Added getimage method
# 96-11-01 fl   Rewritten, removed image attribute and crop method
# 97-05-09 fl   Use PyImagingPaste method instead of image type
# 97-05-12 fl   Minor tweaks to match the IFUNC95 interface
# 97-05-17 fl   Support the "pilbitmap" booster patch
# 97-06-05 fl   Added file= and data= argument to image constructors
# 98-03-09 fl   Added width and height methods to Image classes
# 98-07-02 fl   Use default mode for "P" images without palette attribute
# 98-07-02 fl   Explicitly destroy Tkinter image objects
# 99-07-24 fl   Support multiple Tk interpreters (from Greg Couch)
# 99-07-26 fl   Automatically hook into Tkinter (if possible)
# 99-08-15 fl   Hook uses _imagingtk instead of _imaging
#
# Copyright (c) 1997-1999 by Secret Labs AB
# Copyright (c) 1996-1997 by Fredrik Lundh
#
# See the README file for information on usage and redistribution.
#

import Tkinter, Image

# --------------------------------------------------------------------
# Check for Tkinter interface hooks

_pilbitmap_ok = None

def _pilbitmap_check():
    global _pilbitmap_ok
    if _pilbitmap_ok is None:
        try:
            im = Image.new("1", (1,1))
            Tkinter.BitmapImage(data="PIL:%d" % im.im.id)
            _pilbitmap_ok = 1
        except:
            _pilbitmap_ok = 0
    return _pilbitmap_ok

# --------------------------------------------------------------------
# PhotoImage

class PhotoImage:

    def __init__(self, image=None, size=None, **kw):

        # Tk compatibility: file or data
        if image is None:
            if kw.has_key("file"):
                image = Image.open(kw["file"])
                del kw["file"]
            elif kw.has_key("data"):
                from StringIO import StringIO
                image = Image.open(StringIO(kw["data"]))
                del kw["data"]

        if hasattr(image, "mode") and hasattr(image, "size"):
            # got an image instead of a mode
            mode = image.mode
            if mode == "P":
                # palette mapped data
                image.load()
                try:
                    mode = image.palette.mode
                except AttributeError:
                    mode = "RGB" # default
            size = image.size
            kw["width"], kw["height"] = size
        else:
            mode = image
            image = None

        if mode not in ["1", "L", "RGB", "RGBA"]:
            mode = Image.getmodebase(mode)

        self.__mode = mode
        self.__size = size
        self.__photo = apply(Tkinter.PhotoImage, (), kw)
        if image:
            self.paste(image)

    def __del__(self):
        name = self.__photo.name
        self.__photo.name = None
        try:
            self.__photo.tk.call("image", "delete", name)
        except:
            pass # ignore internal errors

    def __str__(self):
        return str(self.__photo)

    def width(self):
        return self.__size[0]

    def height(self):
        return self.__size[1]

    def paste(self, im, box = None):

        # convert to blittable
        im.load()
        image = im.im
        if image.isblock() and im.mode == self.__mode:
            block = image
        else:
            block = image.new_block(self.__mode, im.size)
            image.convert2(block, image) # convert directly between buffers

        tk = self.__photo.tk

        try:
            tk.call("PyImagingPhoto", self.__photo, block.id)
        except Tkinter.TclError, v:
            # activate Tkinter hook
            try:
                import _imagingtk
                try:
                    _imagingtk.tkinit(tk.interpaddr(), 1)
                except AttributeError:
                    _imagingtk.tkinit(id(tk), 0)
                tk.call("PyImagingPhoto", self.__photo, block.id)
            except (ImportError, AttributeError, Tkinter.TclError):
                raise # configuration problem; cannot attach to Tkinter

# --------------------------------------------------------------------
# BitmapImage

class BitmapImage:

    def __init__(self, image = None, **kw):

        # Tk compatibility: file or data
        if image is None:
            if kw.has_key("file"):
                image = Image.open(kw["file"])
                del kw["file"]
            elif kw.has_key("data"):
                from StringIO import StringIO
                image = Image.open(StringIO(kw["data"]))
                del kw["data"]

        self.__mode = image.mode
        self.__size = image.size

        if _pilbitmap_check():
            # fast way (requires the pilbitmap booster patch)
            image.load()
            kw["data"] = "PIL:%d" % image.im.id
            self.__im = image # must keep a reference
        else:
            # slow but safe way
            kw["data"] = image.tobitmap()
        self.__photo = apply(Tkinter.BitmapImage, (), kw)

    def __del__(self):
        name = self.__photo.name
        self.__photo.name = None
        try:
            self.__photo.tk.call("image", "delete", name)
        except:
            pass # ignore internal errors

    def width(self):
        return self.__size[0]

    def height(self):
        return self.__size[1]

    def __str__(self):
        return str(self.__photo)


# --------------------------------------------------------------------
# Helper for the Image.show method.

def _show(image, title):

    class UI(Tkinter.Label):
        def __init__(self, master, im):
            if im.mode == "1":
                self.image = BitmapImage(im, foreground="white", master=master)
            else:
                self.image = PhotoImage(im, master=master)
            Tkinter.Label.__init__(self, master, image=self.image,
                bg="black", bd=0)

    if not Tkinter._default_root:
        raise IOError, "tkinter not initialized"
    top = Tkinter.Toplevel()
    if title:
        top.title(title)
    UI(top, image).pack()
