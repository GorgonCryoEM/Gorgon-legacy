# Copyright (C) 2005-2008 Washington University in St Louis, Baylor College of Medicine.  All rights reserved
# Author:        Sasakthi S. Abeysinghe (sasakthi@gmail.com)
# Description:   A widget can be used to perform laplacian smoothing on a skeleton 

# CVS Meta Information: 
#   $Source$
#   $Revision$
#   $Date$
#   $Author$
#   $State$
#
# History Log: 
#   $Log$
#   Revision 1.7  2008/06/18 18:15:41  ssa1
#   Adding in CVS meta data
#

from PyQt4 import QtCore, QtGui
from ui_dialog_skeleton_laplacian_smoothing import Ui_DialogSkeletonLaplacianSmoothing
from base_dock_widget import BaseDockWidget

class SkeletonLaplacianSmoothingForm(BaseDockWidget):
    def __init__(self, main, skeletonViewer, parent=None):
        BaseDockWidget.__init__(self, 
                               main, 
                               "&Laplacian Smoothing", 
                               "Perform laplacian smoothing", 
                               "perform_SkeletonLaplacianSmoothing", 
                               "actions-skeleton-laplacianSmoothing", 
                               "actions-skeleton", 
                               QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea | QtCore.Qt.BottomDockWidgetArea, 
                               QtCore.Qt.RightDockWidgetArea, 
                               parent)
        self.app = main
        self.viewer = skeletonViewer
        self.connect(self.viewer, QtCore.SIGNAL("modelLoaded()"), self.modelLoaded)
        self.connect(self.viewer, QtCore.SIGNAL("modelUnloaded()"), self.modelUnloaded)
        self.connect(self.viewer, QtCore.SIGNAL("modelChanged()"), self.modelChanged)
        self.createUI()
        self.createActions()

    def createUI(self):
        self.ui = Ui_DialogSkeletonLaplacianSmoothing()
        self.ui.setupUi(self)       
            
    def modelChanged(self):
        if(not self.viewer.loaded) and self.app.actions.getAction("perform_SkeletonLaplacianSmoothing").isChecked():
            self.app.actions.getAction("perform_SkeletonLaplacianSmoothing").trigger()        
        
    def createActions(self):   
        self.displayAct.setEnabled(False)            
  
    def createMenus(self):
        self.app.menus.addAction("actions-skeleton-laplacianSmoothing", self.app.actions.getAction("perform_SkeletonLaplacianSmoothing"), "actions-skeleton")        
    
    def accept(self):
        self.setCursor(QtCore.Qt.BusyCursor)
        self.viewer.renderer.performSmoothLaplacian(self.ui.doubleSpinBoxConvergenceRate.value(), self.ui.spinBoxNoOfIterations.value())
        self.setCursor(QtCore.Qt.ArrowCursor)
        self.viewer.emitModelChanged()
        
    def reject(self):
        self.app.actions.getAction("perform_SkeletonLaplacianSmoothing").trigger()
        
    def modelLoaded(self):
        self.displayAct.setEnabled(True)
    
    def modelUnloaded(self):
        self.displayAct.setEnabled(False)
        