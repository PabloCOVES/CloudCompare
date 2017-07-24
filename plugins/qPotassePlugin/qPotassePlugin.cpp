//##########################################################################
//#                                                                        #
//#                       CLOUDCOMPARE PLUGIN: qPotasse                      #
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

//First: replace all occurrences of 'qPotassePlugin' by your own plugin class name in this file!
#include "qPotassePlugin.h"
#include "qPotasseDialog.h"

//Qt
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QApplication>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QProgressDialog>
#include <QtConcurrentRun>
#include <QtGui>

//qCC_db
#include <ccGenericPointCloud.h>
#include <ccPointCloud.h>
#include <ccGenericMesh.h>
#include <ccPlane.h>
#include <ccSphere.h>
#include <ccCylinder.h>
#include <ccCone.h>
#include <ccTorus.h>

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/vertex.hpp>
#include <potasse/cloud/normal.hpp>

#include <potasse/estimator/normalsEstimator.hpp>
#include <potasse/estimator/neighborsEstimator.hpp>

#include <iostream>

//Default constructor: should mainly be used to initialize
//actions (pointers) and other members
qPotassePlugin::qPotassePlugin(QObject* parent/*=0*/)
  : QObject(parent)
    , m_action(0)
{
}

// Makes the plugin available if, and only if, there is at least one pointcloud to work on.
void qPotassePlugin::onNewSelection(const ccHObject::Container& selectedEntities)
{
  bool active{false};
  for (auto const *entity : selectedEntities)
    active |= entity->isA(CC_TYPES::POINT_CLOUD);

  if (m_action)
    m_action->setEnabled(active);
}

//This method returns all 'actions' of your plugin.
//It will be called only once, when plugin is loaded.
void qPotassePlugin::getActions(QActionGroup& group)
{
  //default action (if it has not been already created, it's the moment to do it)
  if (!m_action)
  {
    //here we use the default plugin name, description and icon,
    //but each action can have its own!
    m_action = new QAction(getName(),this);
    m_action->setToolTip(getDescription());
    m_action->setIcon(getIcon());
    //connect appropriate signal
    connect(m_action, SIGNAL(triggered()), this, SLOT(doAction()));
  }

  group.addAction(m_action);
}

//This is an example of an action's slot called when the corresponding action
//is triggered (i.e. the corresponding icon or menu entry is clicked in CC's
//main interface). You can access to most of CC components (database,
//3D views, console, etc.) via the 'm_app' attribute (ccMainAppInterface
//object).
void qPotassePlugin::doAction()
{
  //m_app should have already been initialized by CC when plugin is loaded!
  //(--> pure internal check)
  assert(m_app);
  if (!m_app)
    return;

  typedef typename potasse::cloud::Normal3 Normal;
  typedef typename potasse::cloud::Vertex3 Vertex;

  typedef typename potasse::cloud::Cloud<Vertex> VertexCloud;

  const ccHObject::Container& selectedEntities = m_app->getSelectedEntities();

  VertexCloud vertexCloud;
  for (auto *entity : selectedEntities)
  {
    if (!entity->isA(CC_TYPES::POINT_CLOUD))
      continue;

    ccPointCloud *pc{static_cast<ccPointCloud *>(entity)};
    for (std::size_t i{0}, i_{pc->size()}; i < i_; ++i)
    {
      CCVector3 const *p{pc->getPoint(i)};

      Vertex vertex{};
      vertex[0] = static_cast<float>(p->x);
      vertex[1] = static_cast<float>(p->y);
      vertex[2] = static_cast<float>(p->z);

      vertexCloud.push_back(vertex);
    }
  }

  m_app->dispToConsole(QString("[qPotassePlugin] Cloud size : ") + QString::number(vertexCloud.size()));

  QPotasseDialog *dialog{new QPotasseDialog{m_app->getMainWindow()}};

  if (!dialog->exec())
    return;

  typedef typename potasse::cloud::Cloud<Normal> NormalCloud;
  typedef typename potasse::estimator::NormalsEstimator<VertexCloud, NormalCloud> NormalsEstimator;
  typedef typename potasse::estimator::RadiusNeighborsEstimator<VertexCloud> NeighborsEstimator;

  std::shared_ptr<NeighborsEstimator> neighborsEstimator{std::make_shared<NeighborsEstimator>(dialog->neighborsDistance())};
  NormalsEstimator normalsEstimator(neighborsEstimator);
  NormalCloud normalCloud{normalsEstimator.compute(vertexCloud)};
  m_app->dispToConsole("[qPotassePlugin] Normals computed.",ccMainAppInterface::STD_CONSOLE_MESSAGE);

  //This is how you can output messages
  //m_app->dispToConsole("[qPotassePlugin] Hello world!",ccMainAppInterface::STD_CONSOLE_MESSAGE); //a standard message is displayed in the console
  //m_app->dispToConsole("[qPotassePlugin] Warning: potasse plugin shouldn't be used as is!",ccMainAppInterface::WRN_CONSOLE_MESSAGE); //a warning message is displayed in the console
  //m_app->dispToConsole("Potasse plugin shouldn't be used as is!",ccMainAppInterface::ERR_CONSOLE_MESSAGE); //an error message is displayed in the console AND an error box will pop-up!

  /*** HERE ENDS THE ACTION ***/

}

//This method should return the plugin icon (it will be used mainly
//if your plugin as several actions in which case CC will create a
//dedicated sub-menu entry with this icon.
QIcon qPotassePlugin::getIcon() const
{
  //open qPotassePlugin.qrc (text file), update the "prefix" and the
  //icon(s) filename(s). Then save it with the right name (yourPlugin.qrc).
  //(eventually, remove the original qPotassePlugin.qrc file!)
  return QIcon(":/CC/plugin/qPotassePlugin/icon.png");
}
