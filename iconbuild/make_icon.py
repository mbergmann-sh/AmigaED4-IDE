# -*- coding: utf-8 -*-
"""
Build a classic, valid Amiga Workbench .info icon (DiskObject + embedded
Gadget + one Image, 2 bitplanes = 4 colours, using the stock Workbench
1.3/2.0/3.x default 4-colour palette) entirely from scratch, byte for byte,
per the documented on-disk format (Amiga ROM Kernel Reference Manual:
Libraries / Devices - icon.library; workbench/workbench.h; intuition/
intuition.h). No external Amiga tools/libraries used.

do_StackSize is written as 0 here (a placeholder) - AmigaED patches this
one 4-byte big-endian field at a fixed, precomputed byte offset before
writing the icon to disk, per target (OS1.3/OS3.x/MUI/ReAction).
"""
import struct
from PIL import Image as PILImage, ImageDraw

WIDTH, HEIGHT, DEPTH = 48, 48, 2

# Stock Workbench default 4-colour palette (indices 0-3):
# 0 = light gray/white (background), 1 = black (outline), 2 = blue, 3 = orange
PALETTE = [
    (0xAA, 0xAA, 0xAA),  # 0 background
    (0x00, 0x00, 0x00),  # 1 black
    (0x00, 0x55, 0xAA),  # 2 blue
    (0xFF, 0x88, 0x00),  # 3 orange
]

def draw_design():
    """Draw a simple, bold, recognizable 'AmigaED' tool icon: a classic
    monitor/screen shape (echoing the manual's own Amiga-1200-with-monitor
    motif) with a colourful screen and a bold 'A' on it, quantized to
    exactly the 4 indices above."""
    img = PILImage.new("P", (WIDTH, HEIGHT), 0)
    img.putpalette([c for rgb in PALETTE for c in rgb] + [0, 0, 0] * (256 - len(PALETTE)))
    d = ImageDraw.Draw(img)

    # monitor body (black outline, background fill)
    d.rectangle([6, 6, 41, 32], fill=0, outline=1, width=2)
    # screen (blue), inset
    d.rectangle([10, 10, 37, 28], fill=2, outline=1, width=1)
    # bold orange "A" on the screen
    d.polygon([(15, 27), (23, 11), (25, 11), (33, 27), (29, 27), (27.5, 23),
               (20.5, 23), (19, 27)], fill=3)
    d.rectangle([21, 19, 27, 21], fill=3)
    # monitor stand
    d.rectangle([20, 33, 27, 37], fill=1)
    # base
    d.rectangle([12, 38, 35, 42], fill=1, outline=1)
    d.rectangle([13, 39, 34, 41], fill=0)

    return img

def image_to_planes(img):
    """Convert a PIL 'P' mode image (palette indices 0..3) into Amiga
    bitplane data: DEPTH planes, each row padded to a whole number of
    WORDs (16 pixels), planes stored consecutively (plane-major)."""
    row_words = (WIDTH + 15) // 16
    row_bytes = row_words * 2
    planes = [bytearray(row_bytes * HEIGHT) for _ in range(DEPTH)]

    px = img.load()
    for y in range(HEIGHT):
        for x in range(WIDTH):
            idx = px[x, y] & ((1 << DEPTH) - 1)
            byte_index = y * row_bytes + (x // 8)
            bit = 7 - (x % 8)
            for p in range(DEPTH):
                if (idx >> p) & 1:
                    planes[p][byte_index] |= (1 << bit)

    data = bytearray()
    for p in planes:
        data += p
    return bytes(data), row_bytes


def build_icon_bytes():
    img = draw_design()
    plane_data, row_bytes = image_to_planes(img)

    # --- struct Gadget (44 bytes), embedded in DiskObject ---
    gadget = struct.pack(
        ">lhhhhHHH",
        0,          # NextGadget (LONG marker, 0)
        0, 0,       # LeftEdge, TopEdge
        WIDTH, HEIGHT,   # Width, Height
        0x0004,     # Flags = GFLG_GADGIMAGE
        0x0000,     # Activation
        0x0001,     # GadgetType
    )
    gadget += struct.pack(
        ">lll l l h l",
        1,          # GadgetRender marker (non-zero => Image follows)
        0,          # SelectRender marker (0 => none)
        0,          # GadgetText pointer
        0,          # MutualExclude
        0,          # SpecialInfo pointer
        0,          # GadgetID
        0,          # UserData pointer
    )
    assert len(gadget) == 44, len(gadget)

    # --- DiskObject header ---
    WBTOOL = 3
    do_header = struct.pack(">HH", 0xE310, 1)   # do_Magic, do_Version
    do_header += gadget
    do_header += struct.pack(">B", WBTOOL)      # do_Type
    do_header += struct.pack(">B", 0)           # pad byte (alignment)
    do_header += struct.pack(">l", 0)           # do_DefaultTool marker (none)
    do_header += struct.pack(">l", 0)           # do_ToolTypes marker (none)
    do_header += struct.pack(">l", 0)           # do_CurrentX
    do_header += struct.pack(">l", 0)           # do_CurrentY
    do_header += struct.pack(">l", 0)           # do_DrawerData marker (none)
    do_header += struct.pack(">l", 0)           # do_ToolWindow marker (none)
    stacksize_offset = len(do_header)
    do_header += struct.pack(">l", 0)           # do_StackSize - PLACEHOLDER, patched later
    assert len(do_header) == 78, len(do_header)

    # --- struct Image (20 bytes) ---
    image_struct = struct.pack(
        ">hhhhh l BB l",
        0, 0,           # LeftEdge, TopEdge
        WIDTH, HEIGHT, DEPTH,
        1,              # ImageData marker (non-zero => bitplane data follows)
        (1 << DEPTH) - 1,  # PlanePick = 0x3 (both planes used)
        0,              # PlaneOnOff
        0,              # NextImage marker (0 => none)
    )
    assert len(image_struct) == 20, len(image_struct)

    icon_bytes = do_header + image_struct + plane_data
    return icon_bytes, stacksize_offset


if __name__ == "__main__":
    icon_bytes, stacksize_offset = build_icon_bytes()
    with open("/home/claude/iconbuild/amigaed_tool.info", "wb") as f:
        f.write(icon_bytes)
    print("wrote", len(icon_bytes), "bytes; do_StackSize at offset", stacksize_offset)
