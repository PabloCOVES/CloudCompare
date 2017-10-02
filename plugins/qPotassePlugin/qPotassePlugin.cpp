//##########################################################################
//#                                                                        #
//#                       CLOUDCOMPARE PLUGIN: qPotasse                    #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                             COPYRIGHT: XXX                             #
//#                                                                        #
//##########################################################################

// First: replace all occurrences of 'qPotassePlugin' by your own plugin class
// name in this file!
#include "qPotassePlugin.h"
#include "qPotasseCell.h"
#include "qPotasseDialog.h"

// Qt
#include <QMainWindow>
#include <QSharedPointer>
#include <QtGui>

// CC
#include <ReferenceCloud.h>
#include <ccPlane.h>
#include <ccPointCloud.h>
#include <ccProgressDialog.h>

#include <Eigen/Dense>

// Default constructor: should mainly be used to initialize
// actions (pointers) and other members
qPotassePlugin::qPotassePlugin(QObject *parent /*=0*/)
    : QObject(parent), m_action(0) {}

// This method should enable or disable each plugin action
// depending on the currently selected entities ('selectedEntities').
// For example: if none of the selected entities is a cloud, and your
// plugin deals only with clouds, call 'm_action->setEnabled(false)'
void qPotassePlugin::onNewSelection(
    const ccHObject::Container &selectedEntities) {
    bool enable{false};

    for (auto const *entity : selectedEntities)
        enable |= entity->isA(CC_TYPES::POINT_CLOUD);

    m_action->setEnabled(enable);
}

// This method returns all 'actions' of your plugin.
// It will be called only once, when plugin is loaded.
void qPotassePlugin::getActions(QActionGroup &group) {
    // default action (if it has not been already created, it's the moment to do
    // it)
    if (!m_action) {
        // here we use the default plugin name, description and icon,
        // but each action can have its own!
        m_action = new QAction(getName(), this);
        m_action->setToolTip(getDescription());
        m_action->setIcon(getIcon());
        // connect appropriate signal
        connect(m_action, SIGNAL(triggered()), this, SLOT(doAction()));
    }

    group.addAction(m_action);
}

// This is an example of an action's slot called when the corresponding action
// is triggered (i.e. the corresponding icon or menu entry is clicked in CC's
// main interface). You can access to most of CC components (database,
// 3D views, console, etc.) via the 'm_app' attribute (ccMainAppInterface
// object).
void qPotassePlugin::doAction() {
    // m_app should have already been initialized by CC when plugin is loaded!
    //(--> pure internal check)
    assert(m_app);
    if (!m_app) return;

    qPotasseDialog dialog{m_app->getMainWindow()};

    if (!dialog.exec()) return;

    ////////////////////////////////////////////////////////////////////////////
    // Cloud
    ////////////////////////////////////////////////////////////////////////////
    const ccHObject::Container &selectedEntities = m_app->getSelectedEntities();
    ccPointCloud *pc{new ccPointCloud{}};
    for (auto *entity : selectedEntities) {
        if (!entity->isA(CC_TYPES::POINT_CLOUD)) continue;

        ccPointCloud *pcEntity{static_cast<ccPointCloud *>(entity)};
        pc->operator+=(pcEntity);

        entity->setEnabled(false);  // Hide source cloud in GUI.
    }

    m_app->dispToConsole(
        QString("[qPotassePlugin] Cloud size : ") + QString::number(pc->size()),
        ccMainAppInterface::STD_CONSOLE_MESSAGE);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Normals
    ////////////////////////////////////////////////////////////////////////////
    {
        ccProgressDialog progress{false, m_app->getMainWindow()};
        pc->computeNormalsWithOctree(CC_LOCAL_MODEL_TYPES::LS,
                                     ccNormalVectors::Orientation::PLUS_X,
                                     dialog.neighborsDistance(), &progress);
    }
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Octree
    ////////////////////////////////////////////////////////////////////////////
    ccOctree::Shared octree{pc->getOctree()};
    unsigned int level{1};
    while (octree->getCellSize(level) > dialog.cellSize()) ++level;
    octree->setDisplayedLevel(level);

    m_app->dispToConsole(QString("[qPotassePlugin] Cell size set to ") +
                             QString::number(octree->getCellSize(level)),
                         ccMainAppInterface::STD_CONSOLE_MESSAGE);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Shapes
    ////////////////////////////////////////////////////////////////////////////

    std::vector<Cell> cells{};
    for (unsigned int i{0}; i < octree->getCellNumber(level); ++i) {
        CCLib::ReferenceCloud referenceCloud{pc};
        octree->getPointsInCellByCellIndex(&referenceCloud, i, level);

        std::vector<Eigen::Vector3f> vertices, normals;
        for (unsigned int i{0}, i_{referenceCloud.size()}; i < i_; ++i) {
            unsigned int globalIndex{referenceCloud.getPointGlobalIndex(i)};

            const CCVector3 *p{pc->getPoint(globalIndex)};
            Eigen::Vector3f vertex{static_cast<float>(p->x),
                                   static_cast<float>(p->y),
                                   static_cast<float>(p->z)};
            vertices.push_back(vertex);

            const CCVector3 &n{pc->getPointNormal(globalIndex)};
            Eigen::Vector3f normal{static_cast<float>(n.x),
                                   static_cast<float>(n.y),
                                   static_cast<float>(n.z)};
            normals.push_back(normal);
        }

        Cell cell(i, level, .05, 10);
        cell(referenceCloud, vertices, normals, true, true);
        cells.push_back(cell);

#ifndef NDEBUG
        m_app->dispToConsole(QString("[qPotassePlugin] Cell size : ") +
                                 QString::number(referenceCloud.size()),
                             ccMainAppInterface::STD_CONSOLE_MESSAGE);
#endif
    }

    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Display
    ////////////////////////////////////////////////////////////////////////////
    ccHObject *group = new ccHObject("Potasse");

    pc->setName("Potasse cloud");
    pc->setEnabled(true);
    group->addChild(pc);

    for (auto cell : cells) {
        typename Cell::PlanePtr planePtr{cell.planePtr()};
        if (planePtr) {
            group->addChild(cell.planePtr().get());
        }

#ifndef NDEBUG
        if (planePtr) {
            m_app->dispToConsole(
                QString("[qPotassePlugin] Plane Center : ") +
                    QString::number(
                        static_cast<float>(planePtr->getCenter()[0])) +
                    QString(" ,") +
                    QString::number(
                        static_cast<float>(planePtr->getCenter()[1])) +
                    QString(" ,") +
                    QString::number(
                        static_cast<float>(planePtr->getCenter()[2])),
                ccMainAppInterface::STD_CONSOLE_MESSAGE);

            m_app->dispToConsole(
                QString("[qPotassePlugin] Plane Normal : ") +
                    QString::number(
                        static_cast<float>(planePtr->getNormal()[0])) +
                    QString(" ,") +
                    QString::number(
                        static_cast<float>(planePtr->getNormal()[1])) +
                    QString(" ,") +
                    QString::number(
                        static_cast<float>(planePtr->getNormal()[2])),
                ccMainAppInterface::STD_CONSOLE_MESSAGE);
        }
#endif
    }

    m_app->addToDB(group);
}

// This method should return the plugin icon (it will be used mainly
// if your plugin as several actions in which case CC will create a
// dedicated sub-menu entry with this icon.
QIcon qPotassePlugin::getIcon() const {
    // open qPotassePlugin.qrc (text file), update the "prefix" and the
    // icon(s) filename(s). Then save it with the right name
    // (yourPlugin.qrc).
    //(eventually, remove the original qPotassePlugin.qrc file!)
    return QIcon(":/CC/plugin/qPotassePlugin/icon.png");
}
