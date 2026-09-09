# -*- coding: utf-8 -*-
"""
Builds a promotional "virtual packaging box" mockup for AmigaED 4.0:
a 3D-ish box (flat front face facing the camera, skewed top/side faces
for depth) standing on a soft studio backdrop with a drop shadow. The
front face carries a dark-mode AmigaED screenshot mockup (built the same
way as the manual's own illustrations - real toolbar icons from
images/, the app's own colour palette), "AmigaED 4.0", and "Open Source
Software" underneath in smaller type.

Not a diffusion-model "AI image" - PIL can't do that. This composes the
box, screenshot mockup, and text programmatically instead, in the same
spirit as (and reusing the palette/approach of) the manual's other
mockup illustrations (see mockup_common.py).

Usage:
    python3 gen_promo_box.py [output_path]
"""
import os
import sys
from PIL import Image, ImageDraw, ImageFont, ImageFilter

_DOCBUILD_DIR = os.path.dirname(os.path.abspath(__file__))
ICON_DIR = os.path.join(os.path.dirname(_DOCBUILD_DIR), "images") + "/"
FONT_DIR = "/usr/share/fonts/truetype/liberation/"

def F(name, size):
    return ImageFont.truetype(FONT_DIR + name, size)

sans_regular   = F("LiberationSans-Regular.ttf", 20)
sans_bold      = F("LiberationSans-Bold.ttf", 20)
title_font     = F("LiberationSans-Bold.ttf", 66)
tagline_font   = F("LiberationSans-Regular.ttf", 26)
oss_font       = F("LiberationSans-Bold.ttf", 30)
mono_small     = F("LiberationMono-Regular.ttf", 15)
menu_font      = F("LiberationSans-Regular.ttf", 15)

BLUE       = (43, 87, 143)
ORANGE     = (217, 79, 26)
WHITE      = (255, 255, 255)
DARK_BG    = (28, 28, 30)
DARK_PANE  = (22, 22, 24)

def draw_dark_mode_screenshot(w, h):
    """A compact dark-mode AmigaED window mockup, self-contained."""
    img = Image.new("RGB", (w, h), (20, 20, 22))
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, w - 1, h - 1], outline=(70, 70, 74), width=2)

    TITLE_H = 34
    d.rectangle([0, 0, w, TITLE_H], fill=(24, 42, 68))
    d.text((14, 8), "hello.asm - AmigaED 4.0", font=menu_font, fill=WHITE)
    for i, c in enumerate([(60, 190, 90), (230, 170, 40), (220, 80, 80)]):
        cx = w - 22 - (2 - i) * 20
        d.ellipse([cx - 6, TITLE_H // 2 - 6, cx + 6, TITLE_H // 2 + 6], fill=c)

    MENU_H = 26
    y = TITLE_H
    d.rectangle([0, y, w, y + MENU_H], fill=(34, 34, 38))
    mx = 14
    for item in ["File", "Edit", "Build", "View", "Syntax", "Tools"]:
        d.text((mx, y + 5), item, font=menu_font, fill=(210, 210, 212))
        mx += d.textlength(item, font=menu_font) + 18
    y += MENU_H

    TOOLBAR_H = 34
    d.rectangle([0, y, w, y + TOOLBAR_H], fill=(30, 30, 34))
    icon_files = ["new.png", "open.png", "save.png", "search.png",
                  "build_project.png", "clean_project.png", "start-emu.png"]
    ix = 12
    ICON = 20
    for fname in icon_files:
        p = ICON_DIR + fname
        if os.path.exists(p):
            icon = Image.open(p).convert("RGBA").resize((ICON, ICON), Image.LANCZOS)
            img.paste(icon, (ix, y + (TOOLBAR_H - ICON) // 2), icon)
        ix += ICON + 10
    y += TOOLBAR_H

    TREE_W = int(w * 0.24)
    d.rectangle([0, y, TREE_W, h], fill=(26, 26, 30))
    ty = y + 10
    d.text((10, ty), "Project", font=menu_font, fill=(220, 220, 222)); ty += 22
    for label, col in [("Assembler Sources", (190, 190, 195)), ("  hello.asm [main]", (110, 170, 240)),
                        ("Executable", (190, 190, 195)), ("  hello", (190, 190, 195)),
                        ("Makefiles", (190, 190, 195)), ("  Makefile.gcc", (190, 190, 195))]:
        d.text((10, ty), label, font=mono_small, fill=col)
        ty += 19

    d.line([TREE_W, y, TREE_W, h], fill=(55, 55, 60), width=1)
    ex = TREE_W + 14
    ey = y + 12
    code_lines = [
        ("1", "| hello.asm - GNU as dialect", (110, 160, 110)),
        ("2", "", None),
        ("3", ".text", (190, 130, 210)),
        ("4", ".globl start", (190, 130, 210)),
        ("5", "start:", (225, 225, 230)),
        ("6", "    move.l  #msg,d1", (210, 210, 215)),
        ("7", "    jsr     _LVOPutStr(a6)", (210, 210, 215)),
        ("8", "", None),
        ("9", "msg:", (225, 225, 230)),
        ("10", '    .asciz  "Hello, Amiga!\\n"', (230, 190, 130)),
        ("11", ".even", (190, 130, 210)),
    ]
    for num, text, col in code_lines:
        d.text((ex, ey), num, font=mono_small, fill=(90, 90, 95))
        if text:
            d.text((ex + 34, ey), text, font=mono_small, fill=col)
        ey += 21

    # bottom output strip
    out_h = 48
    d.rectangle([TREE_W, h - out_h, w, h], fill=(10, 10, 12))
    d.text((ex, h - out_h + 8), "GNU as: Project build finished successfully.", font=mono_small, fill=(120, 210, 130))

    return img


def rounded_rect(draw, box, radius, fill):
    draw.rounded_rectangle(box, radius=radius, fill=fill)


def build(out_path):
    W, H = 1600, 1300
    canvas = Image.new("RGB", (W, H), WHITE)

    # --- soft studio-backdrop gradient ---
    for yy in range(H):
        t = yy / H
        r = int(238 - 18 * t)
        g = int(240 - 18 * t)
        b = int(244 - 14 * t)
        ImageDraw.Draw(canvas).line([(0, yy), (W, yy)], fill=(r, g, b))

    # --- box geometry: flat front rectangle + a THIN skewed spine/top -
    # book/slipcase ("Einschubbox") thickness, not a deep cube. The
    # depth vector's length IS the visible spine width in pixels, so it
    # is kept small and shallow-angled (mostly horizontal) rather than
    # the much larger, steeper vector a real cube-shaped box would use.
    FW, FH = 780, 1000
    x0, y0 = 420, 190
    dx, dy = 58, -40   # thin, book-like spine/top thickness

    front_tl = (x0, y0)
    front_tr = (x0 + FW, y0)
    front_br = (x0 + FW, y0 + FH)
    front_bl = (x0, y0 + FH)

    top_poly = [front_tl, front_tr, (front_tr[0] + dx, front_tr[1] + dy), (front_tl[0] + dx, front_tl[1] + dy)]
    side_poly = [front_tr, (front_tr[0] + dx, front_tr[1] + dy), (front_br[0] + dx, front_br[1] + dy), front_br]

    # --- drop shadow: tight, low contact shadow (a thin slipcase lying
    # almost flat casts a much smaller shadow than a deep box) ---
    shadow_layer = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    sd = ImageDraw.Draw(shadow_layer)
    sd.ellipse([x0 - 30, front_bl[1] + 6, x0 + FW + dx + 60, front_bl[1] + 70], fill=(20, 20, 25, 110))
    shadow_layer = shadow_layer.filter(ImageFilter.GaussianBlur(20))
    canvas.paste(shadow_layer, (0, 0), shadow_layer)

    d = ImageDraw.Draw(canvas)

    BOX_FRONT = (32, 48, 78)
    BOX_TOP = (96, 128, 178)
    BOX_SIDE = (16, 24, 42)

    d.polygon(top_poly, fill=BOX_TOP)
    d.polygon(side_poly, fill=BOX_SIDE)
    d.rectangle([front_tl[0], front_tl[1], front_br[0], front_br[1]], fill=BOX_FRONT)

    # crease/fold lines - a slip-in sleeve's cardboard is scored (not
    # just cut) where the front cover folds into the thin spine/top, and
    # a second, fainter score a little further in from the true edge is
    # typical of a double-wall slipcase construction.
    fold_dark = (8, 12, 22)
    fold_light = (120, 150, 195)
    for inset in (0, 12):
        d.line([(front_tr[0] - inset, front_tl[1]), (front_tr[0] - inset, front_br[1])],
               fill=fold_dark, width=1)
        d.line([(front_tr[0] - inset - 2, front_tl[1]), (front_tr[0] - inset - 2, front_br[1])],
               fill=fold_light, width=1)
        d.line([(front_tl[0], front_tl[1] + (inset if inset else 0)),
                (front_tr[0], front_tl[1] + (inset if inset else 0))],
               fill=fold_dark, width=1)

    # outer edge lines for definition
    d.line([front_tl, front_tr], fill=(10, 15, 26), width=2)
    d.line([front_tr, front_br], fill=(10, 15, 26), width=2)
    d.line(top_poly + [top_poly[0]], fill=(10, 15, 26), width=1)
    d.line(side_poly + [side_poly[0]], fill=(10, 15, 26), width=1)

    # --- front-face content ---
    pad = 46
    cx = (front_tl[0] + front_br[0]) // 2

    title = "AmigaED 4.0"
    tw = d.textlength(title, font=title_font)
    d.text((cx - tw / 2, y0 + pad), title, font=title_font, fill=WHITE)

    tagline = "Cross-Development for classic Amiga, all in one place"
    tlw = d.textlength(tagline, font=tagline_font)
    d.text((cx - tlw / 2, y0 + pad + 82), tagline, font=tagline_font, fill=(190, 205, 230))

    # attribution sits directly under the tagline now, small print -
    # the screenshot panel below is shifted down to make room for it.
    attribution = "This Software interfaces Bebbo's amiga-gcc and its forks"
    aw = d.textlength(attribution, font=mono_small)
    attrib_y = y0 + pad + 82 + 40
    d.text((cx - aw / 2, attrib_y), attribution, font=mono_small, fill=(140, 155, 180))

    # embedded dark-mode screenshot panel, framed
    shot_w, shot_h = FW - 2 * pad, 560
    shot = draw_dark_mode_screenshot(shot_w, shot_h)
    shot_x = front_tl[0] + pad
    shot_y = attrib_y + 46
    frame_pad = 8
    d.rectangle([shot_x - frame_pad, shot_y - frame_pad, shot_x + shot_w + frame_pad, shot_y + shot_h + frame_pad],
                fill=(12, 18, 30))
    canvas.paste(shot, (shot_x, shot_y))

    # bottom: "Open Source Software" + toolchain line
    oss_y = shot_y + shot_h + frame_pad + 46
    oss_text = "OPEN SOURCE SOFTWARE"
    spaced = "  ".join(list(oss_text))   # crude letter-spacing for a poster look
    ow = d.textlength(spaced, font=oss_font)
    d.text((cx - ow / 2, oss_y), spaced, font=oss_font, fill=ORANGE)

    feat_text = "vbcc  ·  GCC/G++  ·  SAS/C  ·  vasm  ·  GNU as"
    fw = d.textlength(feat_text, font=tagline_font)
    d.text((cx - fw / 2, oss_y + 46), feat_text, font=tagline_font, fill=(170, 185, 210))

    # --- lighting/material pass, for a more product-photo feel: soft
    # top-to-bottom shading (a single key light from upper-left, like a
    # studio softbox), a diagonal glossy highlight streak, and a faint
    # paper/cardboard grain - all confined to the box's own faces via a
    # mask, using numpy for correct, artifact-free alpha blending. ---
    import numpy as np

    box_mask_img = Image.new("L", (W, H), 0)
    md = ImageDraw.Draw(box_mask_img)
    md.polygon(top_poly, fill=255)
    md.polygon(side_poly, fill=255)
    md.rectangle([front_tl[0], front_tl[1], front_br[0], front_br[1]], fill=255)
    box_mask = np.asarray(box_mask_img, dtype=np.float32) / 255.0   # (H, W), 0..1

    canvas_arr = np.asarray(canvas, dtype=np.float32)   # (H, W, 3)

    # 1) gentle top-to-bottom darkening across the front face's own
    #    vertical span (brighter near the top, darker toward the
    #    bottom), scaled by how much of the box is at that row.
    yy = np.arange(H, dtype=np.float32).reshape(H, 1)
    t = np.clip((yy - y0) / max(FH, 1), 0.0, 1.0)
    darken = (t * 55.0) * box_mask   # (H, W), 0..~55
    canvas_arr -= darken[:, :, None]

    # 2) diagonal glossy highlight streak (soft, blurred band of white)
    sheen_img = Image.new("L", (W, H), 0)
    shd = ImageDraw.Draw(sheen_img)
    band_w = 150
    bx = front_tl[0] + 100
    shd.polygon([(bx, front_tl[1]), (bx + band_w, front_tl[1]),
                 (bx + band_w - 260, front_br[1]), (bx - 260, front_br[1])], fill=255)
    sheen_img = sheen_img.filter(ImageFilter.GaussianBlur(40))
    sheen_arr = np.asarray(sheen_img, dtype=np.float32) / 255.0 * box_mask * 60.0   # up to +60 brightness
    canvas_arr += sheen_arr[:, :, None]

    # 3) faint paper/cardboard grain, confined to the box faces
    grain_img = Image.effect_noise((W, H), 24).convert("L")
    grain_arr = (np.asarray(grain_img, dtype=np.float32) - 128.0) * 0.10 * box_mask
    canvas_arr += grain_arr[:, :, None]

    canvas_arr = np.clip(canvas_arr, 0, 255).astype(np.uint8)
    canvas = Image.fromarray(canvas_arr, mode="RGB")

    canvas.save(out_path)
    print("saved", out_path, canvas.size)


if __name__ == "__main__":
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(_DOCBUILD_DIR), "AmigaED_4.0_Packaging_Mockup.png")
    build(out)
