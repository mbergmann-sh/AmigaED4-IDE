import sys
from PySide6.QtWidgets import QApplication
from PySide6.QtGui import QTextDocument, QPageLayout, QPageSize
from PySide6.QtCore import QUrl, QMarginsF, QSizeF
from PySide6.QtPrintSupport import QPrinter

app = QApplication(sys.argv)

html_path = sys.argv[1]
pdf_path = sys.argv[2]

with open(html_path, "r", encoding="utf-8") as f:
    html = f.read()

doc = QTextDocument()
doc.setBaseUrl(QUrl.fromLocalFile(html_path))

printer = QPrinter(QPrinter.ScreenResolution)
printer.setOutputFormat(QPrinter.PdfFormat)
printer.setOutputFileName(pdf_path)
printer.setPageSize(QPageSize(QPageSize.A4))
printer.setPageMargins(QMarginsF(18, 14, 18, 16), QPageLayout.Millimeter)

# Set the document's page width to match the printer's usable page width
# (in device pixels at the printer's own resolution) BEFORE loading HTML,
# so images/tables reflow to the actual printable width instead of
# whatever default QTextDocument width was last used.
page_rect = printer.pageRect(QPrinter.DevicePixel)
doc.setPageSize(QSizeF(page_rect.width(), page_rect.height()))
doc.setHtml(html)

doc.print_(printer)
print("saved", pdf_path)
