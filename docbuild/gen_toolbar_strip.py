# -*- coding: utf-8 -*-
"""
Regenerates html_assets/toolbar-000.png - the numbered toolbar reference
strip shown at the top of the "Toolbar Reference" chapter in the manual.
Standalone (not part of gen_html_manual2.py) since it only needs to run
again when a toolbar button is added, removed, or reordered - not on
every manual regen.

Icons are pulled directly from the app's own images/ folder (see
mockup_common.ICON_DIR) - the strip is always visually in sync with the
app's real toolbar icons, never hand-drawn.

Usage:
    python3 gen_toolbar_strip.py
"""
import os
from PIL import Image, ImageDraw
from mockup_common import ICON_DIR, ORANGE, F

# Mirrors mockup_common.TOOLBAR_GROUPS, but is the authoritative numbering
# source for this strip - kept in lockstep with the "#" column of the
# Toolbar Reference table in gen_html_manual2.py by hand (both are
# updated together whenever a toolbar button changes).
GROUPS = [
    ["new.png", "open.png", "save.png", "filesaveas.png", "printer.png"],
    ["undo.png", "redo.png"],
    ["cut.png", "copy.png", "paste.png"],
    ["search.png"],
    ["gotoline.png", "brackets.png"],
    ["dice.png", "build_project.png", "clean_project.png", "open_shell.png"],
    ["start-emu.png", "stop-emu.png"],
    ["fileexit.png"],
]

ICON = 44
GAP = 14
SEP_GAP = 22
LABEL_H = 26
PAD = 10
num_font = F("LiberationSans-Bold.ttf", 18)

# --- pass 1: compute total width ---
n = 1
x = PAD
positions = []
for gi, g in enumerate(GROUPS):
    for fname in g:
        positions.append((fname, n, x))
        x += ICON + GAP
        n += 1
    if gi < len(GROUPS) - 1:
        x += SEP_GAP
width = x - GAP + PAD
height = PAD + ICON + 6 + LABEL_H + PAD

img = Image.new("RGB", (width, height), (255, 255, 255))
d = ImageDraw.Draw(img)

for fname, num, x in positions:
    icon = Image.open(os.path.join(ICON_DIR, fname)).convert("RGBA")
    icon = icon.resize((ICON, ICON), Image.LANCZOS)
    img.paste(icon, (x, PAD), icon)
    label = str(num)
    tw = d.textlength(label, font=num_font)
    d.text((x + ICON/2 - tw/2, PAD + ICON + 6), label, font=num_font, fill=ORANGE)

# separators, drawn at the midpoint of each configured gap between groups
x = PAD
for gi, g in enumerate(GROUPS):
    x += len(g) * (ICON + GAP)
    if gi < len(GROUPS) - 1:
        sx = x - GAP + SEP_GAP // 2
        d.line([sx, PAD, sx, PAD + ICON], fill=(200, 200, 200), width=1)
        x += SEP_GAP

out_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "html_assets", "toolbar-000.png")
img.save(out_path)
print("saved", out_path, img.size)
