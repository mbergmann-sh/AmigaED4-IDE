import sys
from PySide6.QtWidgets import QApplication, QTextBrowser
from PySide6.QtGui import QPixmap
from PySide6.QtCore import QUrl, Qt

app = QApplication(sys.argv)

path = sys.argv[1]
out = sys.argv[2]
w = int(sys.argv[3]) if len(sys.argv) > 3 else 900
h = int(sys.argv[4]) if len(sys.argv) > 4 else 3000

browser = QTextBrowser()
browser.resize(w, h)
browser.setOpenExternalLinks(True)
browser.setSource(QUrl.fromLocalFile(path))

pixmap = QPixmap(w, h)
pixmap.fill(Qt.white)
browser.render(pixmap)
pixmap.save(out)
print("saved", out, "doc size:", browser.document().size())
