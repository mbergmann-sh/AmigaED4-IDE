from PIL import Image, ImageDraw, ImageFont

FONT_DIR = "/usr/share/fonts/truetype/liberation/"
def F(name, size):
    return ImageFont.truetype(FONT_DIR + name, size)

sans        = F("LiberationSans-Regular.ttf", 20)
sans_bold   = F("LiberationSans-Bold.ttf", 20)
sans_bold_title = F("LiberationSans-Bold.ttf", 24)
sans_small  = F("LiberationSans-Regular.ttf", 18)
sans_tiny   = F("LiberationSans-Regular.ttf", 16)
mono        = F("LiberationMono-Regular.ttf", 19)
mono_small  = F("LiberationMono-Regular.ttf", 17)

BLUE        = (43, 87, 143)
MENUBAR_BG  = (245, 245, 245)
TOOLBAR_BG  = (233, 236, 240)
TAB_BG      = (221, 221, 221)
WHITE       = (255, 255, 255)
DARK_BG     = (30, 30, 30)
ORANGE      = (217, 79, 26)
GREEN_DOT   = (40, 200, 64)
YELLOW_DOT  = (255, 189, 46)
RED_DOT     = (255, 95, 87)
TEXT_DARK   = (30, 30, 30)
DISABLED_GRAY = (150, 150, 150)
MENU_BG     = (250, 250, 250)
MENU_BORDER = (180, 180, 180)
SEPARATOR   = (215, 215, 215)
HILITE_BG   = (51, 141, 230)
CONSOLE_BG  = (32, 32, 36)
CONSOLE_FG  = (220, 220, 220)
CONSOLE_RED = (255, 110, 110)
CONSOLE_GREEN = (130, 220, 140)

ICON_DIR = "/home/claude/amigaed80/AmigaED_4_rev131/images/"

TOOLBAR_GROUPS = [
    ["new.png", "open.png", "save.png", "filesaveas.png", "printer.png"],
    ["undo.png", "redo.png"],
    ["cut.png", "copy.png", "paste.png"],
    ["search.png"],
    ["gotoline.png", "brackets.png"],
    ["dice.png", "build_project.png", "clean_project.png"],
    ["start-emu.png", "stop-emu.png"],
    ["fileexit.png"],
]

W = 1160

def new_window(title, height, menu_items=None, highlight_menu=None):
    """Draws title bar + menu bar + toolbar. Returns (img, draw, y_after_toolbar, icon_positions)."""
    if menu_items is None:
        menu_items = ["File", "Edit", "Inserts", "Build", "Navigation", "View", "Syntax", "Tools", "Help"]

    img = Image.new("RGB", (W, height), WHITE)
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, W-1, height-1], outline=MENU_BORDER, width=2)

    TITLE_H = 54
    d.rectangle([0, 0, W, TITLE_H], fill=BLUE)
    d.text((20, 14), title, font=sans_bold_title, fill=WHITE)
    for i, c in enumerate([GREEN_DOT, YELLOW_DOT, RED_DOT]):
        cx = W - 34 - (2-i)*30
        d.ellipse([cx-9, TITLE_H//2-9, cx+9, TITLE_H//2+9], fill=c)

    MENUBAR_H = 38
    y = TITLE_H
    d.rectangle([0, y, W, y+MENUBAR_H], fill=MENUBAR_BG)
    mx = 20
    for item in menu_items:
        tw = d.textlength(item, font=sans_small)
        if highlight_menu == item:
            d.rectangle([mx-6, y+4, mx+tw+6, y+MENUBAR_H-4], fill=HILITE_BG)
            d.text((mx, y+9), item, font=sans_small, fill=WHITE)
        else:
            d.text((mx, y+9), item, font=sans_small, fill=TEXT_DARK)
        mx += tw + 26
    y += MENUBAR_H

    TOOLBAR_H = 54
    d.rectangle([0, y, W, y+TOOLBAR_H], fill=TOOLBAR_BG)
    x = 16
    icon_positions = {}
    ICON = 30
    for gi, g in enumerate(TOOLBAR_GROUPS):
        for fname in g:
            icon = Image.open(ICON_DIR + fname).convert("RGBA")
            icon = icon.resize((ICON, ICON), Image.LANCZOS)
            img.paste(icon, (x, y + (TOOLBAR_H-ICON)//2), icon)
            icon_positions[fname] = (x, y + (TOOLBAR_H-ICON)//2, ICON, ICON)
            x += ICON + 8
        if gi < len(TOOLBAR_GROUPS)-1:
            d.line([x+7, y+8, x+7, y+TOOLBAR_H-8], fill=(200,200,200), width=1)
            x += 16
    y += TOOLBAR_H

    return img, d, y, icon_positions


def highlight_icon(d, icon_positions, fname, label=None, color=ORANGE):
    if fname not in icon_positions:
        return
    x, iy, w, h = icon_positions[fname]
    d.rounded_rectangle([x-5, iy-5, x+w+5, iy+h+5], radius=6, outline=color, width=3)
    if label:
        cx = x + w//2
        d.polygon([(cx-8, iy+h+9), (cx+8, iy+h+9), (cx, iy+h+21)], fill=color)
        tw = d.textlength(label, font=sans_tiny)
        ty = iy + h + 24
        d.rectangle([cx-tw//2-6, ty, cx+tw//2+6, ty+22], fill=color)
        d.text((cx-tw//2, ty+2), label, font=sans_tiny, fill=WHITE)


def draw_console(img, d, x0, y0, x1, y1, lines):
    """lines: list of (text, color) tuples"""
    d.rectangle([x0, y0, x1, y1], fill=CONSOLE_BG, outline=(90,90,90))
    ty = y0 + 10
    for text, color in lines:
        d.text((x0+12, ty), text, font=mono_small, fill=color)
        ty += 24
    return ty
