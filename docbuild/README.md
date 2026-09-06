# AmigaED documentation build scripts

Regenerates both the in-app HTML manual (`help/manual_en.html` /
`help/manual_de.html`) and the two PDF manuals
(`DOC/AmigaED_Guide_EN.pdf` / `DOC/AmigaED_Anleitung_DE.pdf`) from a
single source: the HTML itself.

## Pipeline

1. **`gen_html_manual2.py`** - the actual manual content (both languages,
   `T(en, de)` pairs throughout) plus the CSS. Needs `html_assets/`
   (screenshots/diagrams, embedded as base64) alongside it. Run it to
   produce `manual_en.html` / `manual_de.html` in the same directory.

   The CSS here is deliberately conservative (no `line-height` anywhere,
   images centered via `<p align="center">` with explicit pixel
   width/height, `<a name="...">` anchors alongside `id=` attributes) -
   this isn't just style preference, it works around specific QScintilla/
   QTextBrowser HTML rendering bugs found the hard way (see Revisions.md
   rev.99/rev.100 era). Verify any CSS change by actually rendering the
   result through `render_qtextdoc.py` below before assuming it looks
   the same as it would in a real browser - QTextBrowser's HTML/CSS
   engine is a much smaller subset.

2. **`render_pdf.py <input.html> <output.pdf>`** - renders that same
   HTML to a paginated A4 PDF via `QTextDocument::print()` (PySide6,
   `QtPrintSupport`). Must use `QPrinter.ScreenResolution`, not
   `HighResolution` - the latter makes the page's device-pixel
   dimensions enormous relative to the HTML's own CSS-pixel units,
   which collapses the whole document onto a single page instead of
   paginating normally (confirmed by testing both).

3. **`render_qtextdoc.py <input.html> <output.png> [w] [h]`** - renders
   the HTML to a single tall PNG via `QTextBrowser`, for visually
   proofreading layout/images before trusting either output above -
   much faster than re-generating a PDF and flipping through pages one
   at a time.

4. **`mockup_common.py`** - shared PIL helpers for hand-drawn AmigaED
   screenshots (window chrome, menu bar, the real toolbar icons pulled
   directly from the app's own `images/` folder, a simple console
   renderer). Used for the Quick Start chapter's illustrations
   (`qs1`-`qs4` in `html_assets/`) and the toolbar reference strip
   (`toolbar-000.png`) - not real screenshots (no way to run the actual
   Qt app headless in this environment), but visually consistent with
   it since they reuse its real icon assets and colour scheme.

## To regenerate everything after a features/UI change

```
python3 gen_html_manual2.py
cp manual_en.html manual_de.html ../help/
QT_QPA_PLATFORM=offscreen python3 render_pdf.py manual_en.html ../DOC/AmigaED_Guide_EN.pdf
QT_QPA_PLATFORM=offscreen python3 render_pdf.py manual_de.html ../DOC/AmigaED_Anleitung_DE.pdf
```

## Superseded

The older `gen_chapter.py`/`gen_cover.py`/`gen_toc.py`/
`gen_recommendations.py` (ReportLab-based, one Python script per PDF
chapter, manually assembled) are no longer used - `render_pdf.py`
renders the whole manual, cover through recommendations, from the one
HTML source in a single pass. Kept out of this folder deliberately, to
avoid two divergent sources of truth for the same content.
