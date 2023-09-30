import sys
from PyQt5.QtWidgets import QWidget, QApplication, QMainWindow

from WA import WA

if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = WA()
    sys.exit(app.exec())
