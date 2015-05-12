#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'v01.ui'
#
# Created: Fri May  8 12:48:11 2015
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
from plotting_v02 import *
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
import numpy
from PyQt4.QtGui import *


try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Form(object):
    def setupUi(self, Form):
        
        
        def myfunc_d():
            xmin = self.lineEdit.displayText()        
            xmin = float(xmin)
            xmax = self.lineEdit_2.displayText()        
            xmax = float(xmax)
            lam = self.lineEdit_8.displayText()        
            lam = float(lam)
            f = self.lineEdit_7.displayText()        
            f = float(f)
            T = self.lineEdit_6.displayText()        
            T = float(T)
            SDD = self.lineEdit_10.displayText()        
            SDD = float(SDD)
            filename = self.lineEdit_3.displayText() 
            filename = str(filename)
            g2 = self.lineEdit_12.displayText()
            g2 = float(g2)
            x,y = diameter(xmin, xmax,lam, f, T,SDD, filename,g2)
            
            ax = self.figure.add_subplot(111)
            ax.plot(x,y, c=numpy.random.rand(3,))
            self.canvas.draw()
            
        def myfunc_lam():
            xmin = self.lineEdit.displayText()        
            xmin = float(xmin)
            xmax = self.lineEdit_2.displayText()        
            xmax = float(xmax)
            f = self.lineEdit_7.displayText()        
            f = float(f)
            T = self.lineEdit_6.displayText()        
            T = float(T)
            SDD = self.lineEdit_10.displayText()        
            SDD = float(SDD)
            D = self.lineEdit_9.displayText()        
            D = float(D)
            filename = self.lineEdit_3.displayText() 
            filename = str(filename)
            g2 = self.lineEdit_12.displayText()
            g2 = float(g2)
            x,y = wavelength(xmin, xmax,D, f, T,SDD, filename,g2)
            
            ax = self.figure.add_subplot(111)
            ax.plot(x,y, c=numpy.random.rand(3,))
            self.canvas.draw()
            
        def myfunc_conc():
            xmin = self.lineEdit.displayText()        
            xmin = float(xmin)
            xmax = self.lineEdit_2.displayText()        
            xmax = float(xmax)
            lam = self.lineEdit_8.displayText()        
            lam = float(lam)
            T = self.lineEdit_6.displayText()        
            T = float(T)
            SDD = self.lineEdit_10.displayText()        
            SDD = float(SDD)
            D = self.lineEdit_9.displayText()        
            D = float(D)
            filename = self.lineEdit_3.displayText() 
            filename = str(filename)
            g2 = self.lineEdit_12.displayText()
            g2 = float(g2)
            x,y = concentration(xmin, xmax,D, lam, T,SDD, filename,g2)
            
            ax = self.figure.add_subplot(111)
            ax.plot(x,y, c=numpy.random.rand(3,))
            self.canvas.draw()
            
        def myfunc_T():
            xmin = self.lineEdit.displayText()        
            xmin = float(xmin)
            xmax = self.lineEdit_2.displayText()        
            xmax = float(xmax)
            lam = self.lineEdit_8.displayText()        
            lam = float(lam)
            f = self.lineEdit_7.displayText()        
            f = float(f)
            SDD = self.lineEdit_10.displayText()        
            SDD = float(SDD)
            D = self.lineEdit_9.displayText()        
            D = float(D)
            filename = self.lineEdit_3.displayText() 
            filename = str(filename)
            g2 = self.lineEdit_12.displayText()
            g2 = float(g2)
            x,y = thickness(xmin, xmax,D, lam, f,SDD, filename,g2)
            
            ax = self.figure.add_subplot(111)
            ax.plot(x,y, c=numpy.random.rand(3,))
            self.canvas.draw()
        
        def myfunc_SDD():
            xmin = self.lineEdit.displayText()        
            xmin = float(xmin)
            xmax = self.lineEdit_2.displayText()        
            xmax = float(xmax)
            lam = self.lineEdit_8.displayText()        
            lam = float(lam)
            f = self.lineEdit_7.displayText()        
            f = float(f)
            T = self.lineEdit_6.displayText()        
            T = float(T)
            D = self.lineEdit_9.displayText()        
            D = float(D)
            filename = self.lineEdit_3.displayText()        
            filename = str(filename)
            g2 = self.lineEdit_12.displayText()
            g2 = float(g2)
            x,y = SDD(xmin, xmax,D, lam, f,T, filename,g2)
    
            ax = self.figure.add_subplot(111)
            ax.plot(x,y, c=numpy.random.rand(3,))
            self.canvas.draw()
        
        def myfunc_g2():
            xmin = self.lineEdit.displayText()        
            xmin = float(xmin)
            xmax = self.lineEdit_2.displayText()        
            xmax = float(xmax)
            lam = self.lineEdit_8.displayText()        
            lam = float(lam)
            f = self.lineEdit_7.displayText()        
            f = float(f)
            T = self.lineEdit_6.displayText()        
            T = float(T)
            D = self.lineEdit_9.displayText()        
            D = float(D)
            filename = self.lineEdit_3.displayText()        
            filename = str(filename)
            SDD = self.lineEdit_10.displayText()        
            SDD = float(SDD)
            x,y = g2(xmin, xmax,D, lam, f,T, filename,SDD)
    
            ax = self.figure.add_subplot(111)
            ax.plot(x,y, c=numpy.random.rand(3,))
            self.canvas.draw()
            
        def eval_var():
            if self.radioButton.isChecked():
                myfunc_d()
            elif self.radioButton_2.isChecked():
                myfunc_lam()
            elif self.radioButton_3.isChecked():
                myfunc_conc()
            elif self.radioButton_4.isChecked():
                myfunc_T()
            elif self.radioButton_5.isChecked():
                myfunc_SDD()
            elif self.radioButton_6.isChecked():
                myfunc_g2()
                
        def change_default_lam():
            self.lineEdit.setText('2.9e-10')
            self.lineEdit_2.setText('6.1e-10')
        
        def change_default_conc():
            self.lineEdit.setText('0.01')
            self.lineEdit_2.setText('0.2')
            
        def change_default_d():
            self.lineEdit.setText('0.0')
            self.lineEdit_2.setText('10e-6')
            
        def change_default_T():
            self.lineEdit.setText('0.001')
            self.lineEdit_2.setText('0.01')
            
        def change_default_SDD():
            self.lineEdit.setText('0.005')
            self.lineEdit_2.setText('0.045')
            
        def change_default_g2():
            self.lineEdit.setText('5e-7')
            self.lineEdit_2.setText('10e-6')
            
        def clear():
            ax = self.figure.add_subplot(111)
            ax.clear()
            self.canvas.draw()
            

        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(880, 350)
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.lineEdit_9 = QtGui.QLineEdit(Form)
        self.lineEdit_9.setObjectName(_fromUtf8("lineEdit_9"))
        self.lineEdit_9.setText('1e-6')
        self.gridLayout.addWidget(self.lineEdit_9, 0, 0, 1, 1)
        self.lineEdit_8 = QtGui.QLineEdit(Form)
        self.lineEdit_8.setObjectName(_fromUtf8("lineEdit_8"))
        self.lineEdit_8.setText('4.1e-10')
        self.gridLayout.addWidget(self.lineEdit_8, 0, 1, 1, 1)
        self.lineEdit_7 = QtGui.QLineEdit(Form)
        self.lineEdit_7.setObjectName(_fromUtf8("lineEdit_7"))
        self.lineEdit_7.setText('0.1')
        self.gridLayout.addWidget(self.lineEdit_7, 0, 2, 1, 1)
        self.lineEdit_6 = QtGui.QLineEdit(Form)
        self.lineEdit_6.setObjectName(_fromUtf8("lineEdit_6"))
        self.lineEdit_6.setText('0.005')
        self.gridLayout.addWidget(self.lineEdit_6, 0, 3, 1, 1)
        self.lineEdit_10 = QtGui.QLineEdit(Form)
        self.lineEdit_10.setObjectName(_fromUtf8("lineEdit_10"))
        self.lineEdit_10.setText('0.025')
        self.gridLayout.addWidget(self.lineEdit_10, 0, 4, 1, 1)
        self.label_4 = QtGui.QLabel(Form)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.gridLayout.addWidget(self.label_4, 1, 0, 1, 1)
        self.label_5 = QtGui.QLabel(Form)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.gridLayout.addWidget(self.label_5, 1, 1, 1, 1)
        self.label_6 = QtGui.QLabel(Form)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.gridLayout.addWidget(self.label_6, 1, 2, 1, 1)
        self.label_7 = QtGui.QLabel(Form)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.gridLayout.addWidget(self.label_7, 1, 3, 1, 1)
        self.label_11 = QtGui.QLabel(Form)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.gridLayout.addWidget(self.label_11, 1, 4, 1, 1)
        self.label_8 = QtGui.QLabel(Form)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.gridLayout.addWidget(self.label_8, 2, 2, 1, 1)
        self.lineEdit_3 = QtGui.QLineEdit(Form)
        self.lineEdit_3.setObjectName(_fromUtf8("lineEdit_3"))
        self.lineEdit_3.setPlaceholderText('Filename')
        self.gridLayout.addWidget(self.lineEdit_3, 2, 3, 1, 1)
        self.label_2 = QtGui.QLabel(Form)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.gridLayout.addWidget(self.label_2, 3, 2, 1, 1)
        self.lineEdit = QtGui.QLineEdit(Form)
        self.lineEdit.setObjectName(_fromUtf8("lineEdit"))
        self.lineEdit.setText('0')
        self.gridLayout.addWidget(self.lineEdit, 3, 3, 1, 1)
        self.label_3 = QtGui.QLabel(Form)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.gridLayout.addWidget(self.label_3, 4, 2, 1, 1)
        self.lineEdit_2 = QtGui.QLineEdit(Form)
        self.lineEdit_2.setObjectName(_fromUtf8("lineEdit_2"))
        self.lineEdit_2.setText('10e-6')
        self.gridLayout.addWidget(self.lineEdit_2, 4, 3, 1, 1)
        self.label = QtGui.QLabel(Form)
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout.addWidget(self.label, 5, 0, 1, 1)
        self.radioButton = QtGui.QRadioButton(Form)
        self.radioButton.setObjectName(_fromUtf8("radioButton"))
        QtCore.QObject.connect(self.radioButton, QtCore.SIGNAL(_fromUtf8("clicked()")), change_default_d)
        self.gridLayout.addWidget(self.radioButton, 6, 0, 1, 1)
        self.radioButton_3 = QtGui.QRadioButton(Form)
        self.radioButton_3.setObjectName(_fromUtf8("radioButton_3"))
        QtCore.QObject.connect(self.radioButton_3, QtCore.SIGNAL(_fromUtf8("clicked()")), change_default_conc)
        self.gridLayout.addWidget(self.radioButton_3, 6, 2, 1, 1)
        self.radioButton_2 = QtGui.QRadioButton(Form)
        self.radioButton_2.setObjectName(_fromUtf8("radioButton_2"))
        QtCore.QObject.connect(self.radioButton_2, QtCore.SIGNAL(_fromUtf8("clicked()")), change_default_lam)
        self.gridLayout.addWidget(self.radioButton_2, 7, 0, 1, 1)
        self.radioButton_4 = QtGui.QRadioButton(Form)
        self.radioButton_4.setObjectName(_fromUtf8("radioButton_4"))
        QtCore.QObject.connect(self.radioButton_4, QtCore.SIGNAL(_fromUtf8("clicked()")), change_default_T)
        self.gridLayout.addWidget(self.radioButton_4, 7, 2, 1, 1)
        self.radioButton_5 = QtGui.QRadioButton(Form)
        self.radioButton_5.setObjectName(_fromUtf8("radioButton_5"))
        QtCore.QObject.connect(self.radioButton_5, QtCore.SIGNAL(_fromUtf8("clicked()")), change_default_SDD)
        self.gridLayout.addWidget(self.radioButton_5, 8, 2, 1, 1)
        self.pushButton = QtGui.QPushButton(Form)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        QtCore.QObject.connect(self.pushButton, QtCore.SIGNAL(_fromUtf8("clicked()")), eval_var)
        self.gridLayout.addWidget(self.pushButton, 8, 4, 1, 1)
        self.pushButton_2 = QtGui.QPushButton(Form)
        self.pushButton_2.setObjectName(_fromUtf8("pushButton_2"))
        QtCore.QObject.connect(self.pushButton_2, QtCore.SIGNAL(_fromUtf8("clicked()")), clear)
        self.gridLayout.addWidget(self.pushButton_2, 8, 6, 1, 1)
        
        self.label_10 = QtGui.QLabel(Form)
        self.label_10.setVisible(False)
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.gridLayout.addWidget(self.label_10, 4, 0, 1, 1)
        
        self.label_logo = QtGui.QLabel(Form)
        self.label_logo.setObjectName(_fromUtf8("label_logo"))
        self.gridLayout.addWidget(self.label_logo, 2, 0, 1, 2)
        
        self.figure = plt.figure()
        self.canvas = FigureCanvas(self.figure)
        self.gridLayout.addWidget(self.canvas, 2, 4, 6, 3)
        
        self.lineEdit_12 = QtGui.QLineEdit(Form)
        self.lineEdit_12.setObjectName(_fromUtf8("lineEdit_12"))
        self.lineEdit_12.setText('4e-6')
        self.gridLayout.addWidget(self.lineEdit_12, 0, 6, 1, 1)
        self.label_12 = QtGui.QLabel(Form)
        self.label_12.setObjectName(_fromUtf8("label_12"))
        self.gridLayout.addWidget(self.label_12, 1, 6, 1, 1)
        self.radioButton_6 = QtGui.QRadioButton(Form)
        self.radioButton_6.setObjectName(_fromUtf8("radioButton_6"))
        QtCore.QObject.connect(self.radioButton_6, QtCore.SIGNAL(_fromUtf8("clicked()")), change_default_g2)
        self.gridLayout.addWidget(self.radioButton_6, 8, 0, 1, 1)

        self.retranslateUi(Form)
        QtCore.QObject.connect(self.radioButton_3, QtCore.SIGNAL(_fromUtf8("clicked(bool)")), self.lineEdit_7.clear)
        QtCore.QObject.connect(self.radioButton_4, QtCore.SIGNAL(_fromUtf8("clicked(bool)")), self.lineEdit_6.clear)
        QtCore.QObject.connect(self.radioButton, QtCore.SIGNAL(_fromUtf8("clicked()")), self.lineEdit_9.clear)
        QtCore.QObject.connect(self.radioButton_2, QtCore.SIGNAL(_fromUtf8("clicked()")), self.lineEdit_8.clear)
        QtCore.QObject.connect(self.radioButton_5, QtCore.SIGNAL(_fromUtf8("clicked()")), self.lineEdit_10.clear)
        QtCore.QObject.connect(self.radioButton_6, QtCore.SIGNAL(_fromUtf8("clicked()")), self.lineEdit_12.clear)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Form", None))
        self.label_4.setText(_translate("Form", "Diameter", None))
        self.label_5.setText(_translate("Form", "Wavelength", None))
        self.label_6.setText(_translate("Form", "Concentration", None))
        self.label_7.setText(_translate("Form", "Thickness", None))
        self.label_11.setText(_translate("Form", "SDD", None))
        self.label_8.setText(_translate("Form", "Filename: ", None))
        self.label_2.setText(_translate("Form", "Min x-value:", None))
        self.label_3.setText(_translate("Form", "Max x-value", None))
        self.label.setText(_translate("Form", "Scan variable:", None))
        self.radioButton.setText(_translate("Form", "Particle Diameter", None))
        self.radioButton_3.setText(_translate("Form", "Concentration", None))
        self.radioButton_2.setText(_translate("Form", "Wavelength", None))
        self.radioButton_4.setText(_translate("Form", "Thickness", None))
        self.radioButton_5.setText(_translate("Form", "SDD", None))
        self.pushButton.setText(_translate("Form", "Plot", None))
        self.pushButton_2.setText(_translate("Form", "Clear", None))
        pixmap = QPixmap(os.getcwd() + '/PSI-Logo.png')
        pixmap2 = pixmap.scaledToWidth(400)
        self.label_logo.setPixmap(pixmap2)
        self.label_12.setText(_translate("Form", "period G2", None))
        self.radioButton_6.setText(_translate("Form", "G2 period", None))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Form = QtGui.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())

