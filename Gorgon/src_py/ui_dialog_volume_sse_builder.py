# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_dialog_volume_sse_builder.ui'
#
# Created: Mon Jun 22 14:04:22 2009
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_DialogVolumeSSEBuilder(object):
    def setupUi(self, DialogVolumeSSEBuilder):
        DialogVolumeSSEBuilder.setObjectName("DialogVolumeSSEBuilder")
        DialogVolumeSSEBuilder.resize(QtCore.QSize(QtCore.QRect(0,0,183,202).size()).expandedTo(DialogVolumeSSEBuilder.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(DialogVolumeSSEBuilder)
        self.gridlayout.setObjectName("gridlayout")

        self.labelAtomScore = QtGui.QLabel(DialogVolumeSSEBuilder)
        self.labelAtomScore.setObjectName("labelAtomScore")
        self.gridlayout.addWidget(self.labelAtomScore,0,0,1,1)

        self.gridlayout1 = QtGui.QGridLayout()
        self.gridlayout1.setObjectName("gridlayout1")

        self.lineEditAtomScore = QtGui.QLineEdit(DialogVolumeSSEBuilder)
        self.lineEditAtomScore.setReadOnly(True)
        self.lineEditAtomScore.setObjectName("lineEditAtomScore")
        self.gridlayout1.addWidget(self.lineEditAtomScore,0,0,1,1)

        self.pushButtonBrowseAtomScore = QtGui.QPushButton(DialogVolumeSSEBuilder)

        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum,QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.pushButtonBrowseAtomScore.sizePolicy().hasHeightForWidth())
        self.pushButtonBrowseAtomScore.setSizePolicy(sizePolicy)
        self.pushButtonBrowseAtomScore.setObjectName("pushButtonBrowseAtomScore")
        self.gridlayout1.addWidget(self.pushButtonBrowseAtomScore,0,1,1,1)
        self.gridlayout.addLayout(self.gridlayout1,1,0,1,2)

        self.line = QtGui.QFrame(DialogVolumeSSEBuilder)
        self.line.setFrameShape(QtGui.QFrame.HLine)
        self.line.setFrameShadow(QtGui.QFrame.Sunken)
        self.line.setObjectName("line")
        self.gridlayout.addWidget(self.line,2,0,1,2)

        self.labelSelection = QtGui.QLabel(DialogVolumeSSEBuilder)
        self.labelSelection.setObjectName("labelSelection")
        self.gridlayout.addWidget(self.labelSelection,3,0,1,1)

        self.tableWidgetSelection = QtGui.QTableWidget(DialogVolumeSSEBuilder)
        self.tableWidgetSelection.setEnabled(False)
        self.tableWidgetSelection.setAlternatingRowColors(True)
        self.tableWidgetSelection.setObjectName("tableWidgetSelection")
        self.gridlayout.addWidget(self.tableWidgetSelection,4,0,1,2)

        self.pushButtonSelectionToHelix = QtGui.QPushButton(DialogVolumeSSEBuilder)
        self.pushButtonSelectionToHelix.setObjectName("pushButtonSelectionToHelix")
        self.gridlayout.addWidget(self.pushButtonSelectionToHelix,5,0,1,1)

        self.pushButtonSelectionToSheet = QtGui.QPushButton(DialogVolumeSSEBuilder)
        self.pushButtonSelectionToSheet.setObjectName("pushButtonSelectionToSheet")
        self.gridlayout.addWidget(self.pushButtonSelectionToSheet,5,1,1,1)

        self.retranslateUi(DialogVolumeSSEBuilder)
        QtCore.QMetaObject.connectSlotsByName(DialogVolumeSSEBuilder)
        DialogVolumeSSEBuilder.setTabOrder(self.lineEditAtomScore,self.pushButtonBrowseAtomScore)
        DialogVolumeSSEBuilder.setTabOrder(self.pushButtonBrowseAtomScore,self.tableWidgetSelection)
        DialogVolumeSSEBuilder.setTabOrder(self.tableWidgetSelection,self.pushButtonSelectionToHelix)
        DialogVolumeSSEBuilder.setTabOrder(self.pushButtonSelectionToHelix,self.pushButtonSelectionToSheet)

    def retranslateUi(self, DialogVolumeSSEBuilder):
        DialogVolumeSSEBuilder.setWindowTitle(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "SSE Builder", None, QtGui.QApplication.UnicodeUTF8))
        self.labelAtomScore.setText(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "Scored Atom File:", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButtonBrowseAtomScore.setText(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.labelSelection.setText(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "Atom Selection:", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidgetSelection.clear()
        self.tableWidgetSelection.setColumnCount(2)
        self.tableWidgetSelection.setRowCount(0)

        headerItem = QtGui.QTableWidgetItem()
        headerItem.setText(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "ID", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidgetSelection.setHorizontalHeaderItem(0,headerItem)

        headerItem1 = QtGui.QTableWidgetItem()
        headerItem1.setText(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "SSE Hunter Score", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidgetSelection.setHorizontalHeaderItem(1,headerItem1)
        self.pushButtonSelectionToHelix.setText(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "Add Helix", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButtonSelectionToSheet.setText(QtGui.QApplication.translate("DialogVolumeSSEBuilder", "Add Sheet", None, QtGui.QApplication.UnicodeUTF8))
