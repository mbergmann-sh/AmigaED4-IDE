# -*- coding: utf-8 -*-
"""
Regenerates html_assets/qs5-000.png, qs6-000.png, qs7-000.png - the three
illustrations for the "Creating an Assembler Project" quick-start
subsection added in rev.148 (see gen_html_manual2.py's quickstart
chapter). Standalone (not part of gen_html_manual2.py), following the
same pattern as gen_toolbar_strip.py - only needs to run again if this
specific quick-start flow changes visually.

Uses the same PIL-based mockup style as the rest of the manual's
illustrations (see mockup_common.py): hand-drawn but visually consistent
with the app - real toolbar icons pulled from images/, the app's own
colour palette, and wording copied verbatim from the actual QAction/
QMessageBox strings in mainwindow.cpp, not paraphrased.

Usage:
    python3 gen_qs_asm.py
"""
import os
from PIL import Image, ImageDraw
from mockup_common import (
    F, sans, sans_bold, sans_small, sans_tiny, mono, mono_small,
    BLUE, MENUBAR_BG, TOOLBAR_BG, TAB_BG, WHITE, DARK_BG, ORANGE,
    GREEN_DOT, YELLOW_DOT, RED_DOT, TEXT_DARK, MENU_BG, MENU_BORDER,
    SEPARATOR, HILITE_BG, CONSOLE_BG, CONSOLE_FG, CONSOLE_GREEN,
    new_window, highlight_icon, draw_console, W,
)

OUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "html_assets")

VERSION_TITLE = "AmigaED 4.0 rev.148"

# ---------------------------------------------------------------------
# qs5: File -> New Project submenu, "New Assembler Project" highlighted
# (the 7th, newest template entry).
# ---------------------------------------------------------------------
img, d, y_after_toolbar, icon_positions = new_window(
    "untitled.c - " + VERSION_TITLE, 560, highlight_menu="File")

MENU_X = 0
MENU_Y = 92  # top of the menu bar (TITLE_H=54 + a hair)
file_items = ["New", "Open...", "Save", "Save As...", None,
              "New Project", "Load Project...", "Save Project", "Close Project", None,
              "Add files to Project...", None, "Recent Projects", None, "Exit"]

fm_w = 260
fm_x = 20
fm_y = 92
row_h = 32
sep_h = 10

cy = fm_y
d.rectangle([fm_x, fm_y, fm_x + fm_w, fm_y + 1], fill=MENU_BORDER)  # placeholder, overwritten below
item_rects = {}
cy = fm_y
for item in file_items:
    if item is None:
        d.line([fm_x + 4, cy + sep_h // 2, fm_x + fm_w - 4, cy + sep_h // 2], fill=SEPARATOR, width=1)
        cy += sep_h
        continue
    item_rects[item] = (fm_x, cy, fm_x + fm_w, cy + row_h)
    cy += row_h
fm_bottom = cy

d.rectangle([fm_x, fm_y, fm_x + fm_w, fm_bottom], fill=MENU_BG, outline=MENU_BORDER, width=1)
for item, (x0, y0, x1, y1) in item_rects.items():
    if item == "New Project":
        d.rectangle([x0, y0, x1, y1], fill=HILITE_BG)
        d.text((x0 + 14, y0 + 7), item, font=sans_small, fill=WHITE)
        tw = d.textlength("\u25b7", font=sans_small)
        d.text((x1 - 22, y0 + 7), "\u25b7", font=sans_small, fill=WHITE)
    else:
        d.text((x0 + 14, y0 + 7), item, font=sans_small, fill=TEXT_DARK)
        if item in ("Recent Projects",):
            d.text((x1 - 22, y0 + 7), "\u25b7", font=sans_small, fill=TEXT_DARK)

# submenu: the 7 New Project templates, "New Assembler Project" highlighted
templates = ["Empty C Project", "Shell Project", "AmigaOS 1.3 Project",
             "AmigaOS 3.x Project", "ReAction Project", "MUI Project",
             "New Assembler Project"]
sm_x = fm_x + fm_w
sm_y = item_rects["New Project"][1]
sm_w = 300
sm_h = len(templates) * row_h
d.rectangle([sm_x, sm_y, sm_x + sm_w, sm_y + sm_h], fill=MENU_BG, outline=MENU_BORDER, width=1)
ty = sm_y
for t in templates:
    if t == "New Assembler Project":
        d.rectangle([sm_x, ty, sm_x + sm_w, ty + row_h], fill=HILITE_BG)
        d.text((sm_x + 14, ty + 7), t, font=sans_small, fill=WHITE)
    else:
        d.text((sm_x + 14, ty + 7), t, font=sans_small, fill=TEXT_DARK)
    ty += row_h

img.save(os.path.join(OUT_DIR, "qs5-000.png"))
print("saved qs5-000.png", img.size)

# ---------------------------------------------------------------------
# qs6: the vasm / GNU as choice dialog - the FIRST prompt shown when
# creating a new Assembler Project. Text and button labels copied
# verbatim from mainwindow.cpp's asmChoiceBox.
# ---------------------------------------------------------------------
DW, DH = 700, 280
dlg = Image.new("RGB", (DW, DH), WHITE)
dd = ImageDraw.Draw(dlg)
dd.rectangle([0, 0, DW - 1, DH - 1], outline=MENU_BORDER, width=2)

DTITLE_H = 40
dd.rectangle([0, 0, DW, DTITLE_H], fill=BLUE)
dd.text((14, 10), VERSION_TITLE, font=sans_bold, fill=WHITE)
for i, c in enumerate([GREEN_DOT, YELLOW_DOT, RED_DOT]):
    cx = DW - 24 - (2 - i) * 24
    dd.ellipse([cx - 7, DTITLE_H // 2 - 7, cx + 7, DTITLE_H // 2 + 7], fill=c)

# question-mark icon
qx, qy, qr = 46, 110, 22
dd.ellipse([qx - qr, qy - qr, qx + qr, qy + qr], fill=(74, 144, 226))
qtw = dd.textlength("?", font=sans_bold)
dd.text((qx - qtw / 2, qy - 13), "?", font=sans_bold, fill=WHITE)

msg_lines = [
    "Due to Compiler differences, you can't have both a vasm- or",
    "GNU as-driven Assembler Project.",
    "Do you want to create this assembler Project for vasm",
    "or GNU as Compiler?",
]
ty = 74
for line in msg_lines:
    dd.text((88, ty), line, font=sans_small, fill=TEXT_DARK)
    ty += 24

# buttons: "vasm" and "GNU as" - GNU as shown as the one about to be
# picked (this quick-start walks through the GNU as path, the newer
# toolchain option), so it gets the hover/focus highlight.
btn_h = 34
btn_y = DH - 60
def button(x, w, label, highlighted):
    fill = HILITE_BG if highlighted else (238, 238, 238)
    txt_col = WHITE if highlighted else TEXT_DARK
    dd.rounded_rectangle([x, btn_y, x + w, btn_y + btn_h], radius=5, fill=fill, outline=MENU_BORDER, width=1)
    tw = dd.textlength(label, font=sans_small)
    dd.text((x + w / 2 - tw / 2, btn_y + 8), label, font=sans_small, fill=txt_col)

button(DW - 210, 90, "vasm", False)
button(DW - 110, 90, "GNU as", True)

dlg.save(os.path.join(OUT_DIR, "qs6-000.png"))
print("saved qs6-000.png", dlg.size)

# ---------------------------------------------------------------------
# qs7: the resulting GNU-as Assembler Project - project tree with a
# single generated Makefile.gcc, the GNU-as-dialect template open
# (with its "|" comment highlighted the way the m68k lexer now
# recognizes it), and a successful Build Project run in the output
# panel below, assembling with m68k-amigaos-as and linking with the
# new GNU ld field.
# ---------------------------------------------------------------------
img2, d2, y2, icon_positions2 = new_window("hello.asm - " + VERSION_TITLE, 700)
highlight_icon(d2, icon_positions2, "build_project.png", "Build Project")

TREE_W = 240
d2.rectangle([0, y2, TREE_W, 700], fill=(247, 247, 247), outline=(225, 225, 225))
tx, tyy = 14, y2 + 14
d2.text((tx, tyy), "Project", font=sans_bold, fill=TEXT_DARK); tyy += 30
d2.text((tx, tyy), "Assembler Sources", font=sans_small, fill=TEXT_DARK); tyy += 24
d2.text((tx + 14, tyy), "hello.asm [main]", font=sans_small, fill=BLUE); tyy += 30
d2.text((tx, tyy), "Executable", font=sans_small, fill=TEXT_DARK); tyy += 24
d2.text((tx + 14, tyy), "hello", font=sans_small, fill=TEXT_DARK); tyy += 30
d2.text((tx, tyy), "Makefiles", font=sans_small, fill=TEXT_DARK); tyy += 24
d2.text((tx + 14, tyy), "Makefile.gcc", font=sans_small, fill=TEXT_DARK); tyy += 24

# tab strip
d2.rectangle([TREE_W, y2, W, y2 + 38], fill=TAB_BG)
d2.rectangle([TREE_W, y2, TREE_W + 130, y2 + 38], fill=WHITE)
d2.text((TREE_W + 14, y2 + 9), "hello.asm", font=sans_small, fill=TEXT_DARK)
ey = y2 + 38

EDITOR_H = 260
d2.rectangle([TREE_W, ey, W, ey + EDITOR_H], fill=WHITE)
GUTTER = (150, 150, 150)
COMMENT = (90, 150, 90)
DIRECTIVE = (150, 60, 140)
LABELCOL = (30, 30, 30)
lines = [
    ("1", "| hello.asm - GNU as dialect", COMMENT),
    ("2", "| Assembled with m68k-amigaos-as, linked with m68k-amigaos-ld", COMMENT),
    ("3", "", None),
    ("4", ".text", DIRECTIVE),
    ("5", ".globl start", DIRECTIVE),
    ("6", "start:", LABELCOL),
    ("7", "    move.l  #msg,d1", TEXT_DARK),
    ("8", "    ...", TEXT_DARK),
    ("9", "", None),
    ("10", "msg:", LABELCOL),
    ("11", '    .asciz  "Hello, Amiga!\\n"', TEXT_DARK),
    ("12", ".even", DIRECTIVE),
]
ly = ey + 12
for num, text, col in lines:
    d2.text((TREE_W + 10, ly), num, font=mono_small, fill=GUTTER)
    if text:
        d2.text((TREE_W + 50, ly), text, font=mono_small, fill=col)
    ly += 20

oy = ey + EDITOR_H
d2.text((TREE_W + 14, oy + 8), "Compiler output", font=sans_bold)
oy += 34
console_lines = [
    ("D:/GCC/bin/m68k-amigaos-as.exe -m68000 hello.asm -o hello.o", CONSOLE_FG),
    ("D:/GCC/bin/m68k-amigaos-ld.exe hello.o -o hello", CONSOLE_FG),
    ("", CONSOLE_FG),
    ("GNU as: Project build finished successfully.", CONSOLE_GREEN),
]
draw_console(img2, d2, TREE_W, oy, W, 700, console_lines)

img2.save(os.path.join(OUT_DIR, "qs7-000.png"))
print("saved qs7-000.png", img2.size)
