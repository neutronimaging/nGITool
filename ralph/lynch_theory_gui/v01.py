# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'v01.ui'
#
# Created: Tue May 12 10:24:17 2015
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

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
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(880, 369)
        self.gridLayout = QtGui.QGridLayout(Form)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.lineEdit_8 = QtGui.QLineEdit(Form)
        self.lineEdit_8.setObjectName(_fromUtf8("lineEdit_8"))
        self.gridLayout.addWidget(self.lineEdit_8, 0, 2, 1, 1)
        self.lineEdit_7 = QtGui.QLineEdit(Form)
        self.lineEdit_7.setObjectName(_fromUtf8("lineEdit_7"))
        self.gridLayout.addWidget(self.lineEdit_7, 0, 3, 1, 1)
        self.label_4 = QtGui.QLabel(Form)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.gridLayout.addWidget(self.label_4, 4, 1, 1, 1)
        self.label_3 = QtGui.QLabel(Form)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.gridLayout.addWidget(self.label_3, 7, 2, 1, 1)
        self.lineEdit_3 = QtGui.QLineEdit(Form)
        self.lineEdit_3.setObjectName(_fromUtf8("lineEdit_3"))
        self.gridLayout.addWidget(self.lineEdit_3, 5, 3, 1, 1)
        self.label_2 = QtGui.QLabel(Form)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.gridLayout.addWidget(self.label_2, 6, 2, 1, 1)
        self.lineEdit_2 = QtGui.QLineEdit(Form)
        self.lineEdit_2.setObjectName(_fromUtf8("lineEdit_2"))
        self.gridLayout.addWidget(self.lineEdit_2, 7, 3, 1, 1)
        self.label = QtGui.QLabel(Form)
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout.addWidget(self.label, 9, 1, 1, 1)
        self.label_11 = QtGui.QLabel(Form)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.gridLayout.addWidget(self.label_11, 4, 5, 1, 1)
        self.pushButton = QtGui.QPushButton(Form)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.gridLayout.addWidget(self.pushButton, 12, 4, 1, 1)
        self.label_10 = QtGui.QLabel(Form)
        self.label_10.setVisible(False)
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.gridLayout.addWidget(self.label_10, 7, 1, 1, 1)
        self.label_9 = QtGui.QLabel(Form)
        self.label_9.setVisible(False)
        self.label_9.setFrameShape(QtGui.QFrame.NoFrame)
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.gridLayout.addWidget(self.label_9, 6, 1, 1, 1)
        self.lineEdit_6 = QtGui.QLineEdit(Form)
        self.lineEdit_6.setObjectName(_fromUtf8("lineEdit_6"))
        self.gridLayout.addWidget(self.lineEdit_6, 0, 4, 1, 1)
        self.radioButton_2 = QtGui.QRadioButton(Form)
        self.radioButton_2.setObjectName(_fromUtf8("radioButton_2"))
        self.gridLayout.addWidget(self.radioButton_2, 11, 1, 1, 1)
        self.lineEdit = QtGui.QLineEdit(Form)
        self.lineEdit.setObjectName(_fromUtf8("lineEdit"))
        self.gridLayout.addWidget(self.lineEdit, 6, 3, 1, 1)
        self.radioButton_4 = QtGui.QRadioButton(Form)
        self.radioButton_4.setObjectName(_fromUtf8("radioButton_4"))
        self.gridLayout.addWidget(self.radioButton_4, 11, 3, 1, 1)
        self.radioButton_3 = QtGui.QRadioButton(Form)
        self.radioButton_3.setObjectName(_fromUtf8("radioButton_3"))
        self.gridLayout.addWidget(self.radioButton_3, 10, 3, 1, 1)
        self.label_7 = QtGui.QLabel(Form)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.gridLayout.addWidget(self.label_7, 4, 4, 1, 1)
        self.radioButton = QtGui.QRadioButton(Form)
        self.radioButton.setObjectName(_fromUtf8("radioButton"))
        self.gridLayout.addWidget(self.radioButton, 10, 1, 1, 1)
        self.label_8 = QtGui.QLabel(Form)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.gridLayout.addWidget(self.label_8, 5, 2, 1, 1)
        self.label_5 = QtGui.QLabel(Form)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.gridLayout.addWidget(self.label_5, 4, 2, 1, 1)
        self.lineEdit_9 = QtGui.QLineEdit(Form)
        self.lineEdit_9.setObjectName(_fromUtf8("lineEdit_9"))
        self.gridLayout.addWidget(self.lineEdit_9, 0, 1, 1, 1)
        self.label_6 = QtGui.QLabel(Form)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.gridLayout.addWidget(self.label_6, 4, 3, 1, 1)
        self.listWidget = QtGui.QListWidget(Form)
        self.listWidget.setObjectName(_fromUtf8("listWidget"))
        self.gridLayout.addWidget(self.listWidget, 5, 4, 7, 2)
        self.graphicsView = QtGui.QGraphicsView(Form)
        self.graphicsView.setObjectName(_fromUtf8("graphicsView"))
        self.gridLayout.addWidget(self.graphicsView, 5, 1, 1, 1)
        self.lineEdit_4 = QtGui.QLineEdit(Form)
        self.lineEdit_4.setObjectName(_fromUtf8("lineEdit_4"))
        self.gridLayout.addWidget(self.lineEdit_4, 0, 5, 1, 1)
        self.lineEdit_10 = QtGui.QLineEdit(Form)
        self.lineEdit_10.setObjectName(_fromUtf8("lineEdit_10"))
        self.gridLayout.addWidget(self.lineEdit_10, 0, 6, 1, 1)
        self.label_12 = QtGui.QLabel(Form)
        self.label_12.setObjectName(_fromUtf8("label_12"))
        self.gridLayout.addWidget(self.label_12, 4, 6, 1, 1)
        self.radioButton_5 = QtGui.QRadioButton(Form)
        self.radioButton_5.setObjectName(_fromUtf8("radioButton_5"))
        self.gridLayout.addWidget(self.radioButton_5, 12, 1, 1, 1)

        self.retranslateUi(Form)
        QtCore.QObject.connect(self.radioButton_3, QtCore.SIGNAL(_fromUtf8("clicked(bool)")), self.lineEdit_7.clear)
        QtCore.QObject.connect(self.radioButton_4, QtCore.SIGNAL(_fromUtf8("clicked(bool)")), self.lineEdit_6.clear)
        QtCore.QObject.connect(self.radioButton, QtCore.SIGNAL(_fromUtf8("clicked()")), self.lineEdit_9.clear)
        QtCore.QObject.connect(self.radioButton_2, QtCore.SIGNAL(_fromUtf8("clicked()")), self.lineEdit_8.clear)
        QtCore.QObject.connect(self.radioButton, QtCore.SIGNAL(_fromUtf8("toggled(bool)")), self.label_9.setVisible)
        QtCore.QObject.connect(self.radioButton, QtCore.SIGNAL(_fromUtf8("toggled(bool)")), self.label_10.setVisible)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Form", None))
        self.label_4.setText(_translate("Form", "Diameter", None))
        self.label_3.setText(_translate("Form", "Max x-value", None))
        self.label_2.setText(_translate("Form", "Min x-value:", None))
        self.label.setText(_translate("Form", "Scan variable:", None))
        self.label_11.setText(_translate("Form", "SDD", None))
        self.pushButton.setText(_translate("Form", "Plot", None))
        self.label_10.setText(_translate("Form", "10 micron", None))
        self.label_9.setText(_translate("Form", "0", None))
        self.radioButton_2.setText(_translate("Form", "Wavelength", None))
        self.radioButton_4.setText(_translate("Form", "Thickness", None))
        self.radioButton_3.setText(_translate("Form", "Concentration", None))
        self.label_7.setText(_translate("Form", "Thickness", None))
        self.radioButton.setText(_translate("Form", "Particle Diameter", None))
        self.label_8.setText(_translate("Form", "Filename: ", None))
        self.label_5.setText(_translate("Form", "Wavelength", None))
        self.label_6.setText(_translate("Form", "Concentration", None))
        self.label_12.setText(_translate("Form", "period G2", None))
        self.radioButton_5.setText(_translate("Form", "G2 period", None))

