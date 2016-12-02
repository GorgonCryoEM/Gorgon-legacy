# Copyright (C) 2005-2008 Washington University in St Louis, Baylor College of Medicine.  All rights reserved
# Author:        Sasakthi S. Abeysinghe (sasakthi@gmail.com)
# Description:   The Gorgon application 


###########################################################################################################
# Following is a hack to overcome a PyOpenGL bug with P2EXE Should be removed when py2exe adds egg support
import sys, os
pathname = os.path.abspath(os.path.dirname(sys.argv[0]))
if(sys.platform == 'win32'):
    sys.path.append(pathname)
    try:
        from OpenGL.platform import win32
    except AttributeError:
        pass
    except ImportError:
            pass
elif(sys.platform == 'darwin'):
    if os.environ.has_key('RESOURCEPATH') :
        sys.path = [os.path.join(os.environ['RESOURCEPATH'], 'lib', 'python2.7', 'lib-dynload')] + sys.path
###########################################################################################################
from ctypes import util

from PyQt4 import QtGui, QtCore
from main_window_form import MainWindowForm

import time


def main():
    gorgonVersion = '@Gorgon_VERSION@'
    app = QtGui.QApplication(sys.argv)    
    #setting different font sizes for different OSs
    if(sys.platform == 'win32'):
        pass
    elif(sys.platform == 'darwin'):
        app.setStyleSheet("* { font-size: 10px }")
    else :
        app.setStyleSheet("* { font-size: 10px }")
        
    pixmap = QtGui.QPixmap(pathname + "/splash.png")
    splash = QtGui.QSplashScreen(pixmap, QtCore.Qt.WindowStaysOnTopHint)
    splash.setMask(pixmap.mask())
    splash.show()
    app.processEvents()
    
    window = MainWindowForm(gorgonVersion)
    window.showNormal()
    window.raise_()
    
    # print dir(window.actions)
    # print window.actions.actionList
    window.windowManager.skeletonViewer.fileName = QtCore.QString("densityMap-skeleton.mrc")
    window.windowManager.sseViewer.helixFileName = QtCore.QString("helices-densityMap.wrl")

    window.actions.getAction("perform_SSEFindHelixCorrespondences").trigger()
    window.windowManager.sseViewer.helixCorrespondanceFinder.ui.lineEditSequenceFile.setText(QtCore.QString("groel-segment.seq"))
    # window.windowManager.sseViewer.correspondenceEngine.loadSequenceGraph()
    window.windowManager.skeletonViewer.loadData()
    window.windowManager.sseViewer.loadHelixData()
    # self.sheetFileName = ""
    # window.windowManager.sseViewer.helixCorrespondanceFinder.checkOk()
    
    # window.windowManager.sseViewer.helixCorrespondanceFinder.accept()
    window.windowManager.sseViewer.helixCorrespondanceFinder.createBasicCorrespondence()
    window.windowManager.sseViewer.correspondenceEngine.executeQuery()
    window.windowManager.sseViewer.correspondenceEngine.saveCorrespondenceToFile('legacy_corr.corr')
    # sys.exit(app.exec_())
    
if __name__ == '__main__':
    main()
