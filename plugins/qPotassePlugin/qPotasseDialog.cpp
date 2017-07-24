#include "qPotasseDialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QVBoxLayout>

QPotasseDialog::QPotasseDialog(QWidget *parent)
  : QDialog{parent}
  , neighborsDistance_{new QDoubleSpinBox{}}
  , graphCellSize_{new QDoubleSpinBox{}}
  , graphCellCardinal_{new QSpinBox{}}
  , treeCellSize_{new QDoubleSpinBox{}}
  , treeCellCardinal_{new QSpinBox{}}
  , cone_{new QCheckBox{"Cone"}}
  , cylinder_{new QCheckBox{"Cylinder"}}
  , plane_{new QCheckBox{"Plane"}}
  , sphere_{new QCheckBox{"Sphere"}}
  , torus_{new QCheckBox{"Torus"}}
{
  //////////////////////////////////////////////////////////////////////////////////////////
  neighborsDistance_->setSuffix("mm");
  neighborsDistance_->setValue(.5);
  neighborsDistance_->setMinimum(.01);
  neighborsDistance_->setMaximum(5);

  graphCellSize_->setSuffix("mm");
  graphCellSize_->setValue(8);
  graphCellSize_->setMinimum(1);

  graphCellCardinal_->setSuffix("pts");
  graphCellCardinal_->setValue(50);
  graphCellCardinal_->setMinimum(1);

  treeCellSize_->setSuffix("mm");
  treeCellSize_->setValue(1);
  treeCellSize_->setMinimum(1);

  treeCellCardinal_->setSuffix("pts");
  treeCellCardinal_->setValue(1);
  treeCellCardinal_->setMinimum(1);

  QFormLayout *normalFormLayout{new QFormLayout{}},
              *graphFormLayout{new QFormLayout{}},
              *treeFormLayout{new QFormLayout{}};

  normalFormLayout->addRow("Neighbors", neighborsDistance_);

  graphFormLayout->addRow("Graph cell size", graphCellSize_);
  graphFormLayout->addRow("Graph cell cardinal", graphCellCardinal_);

  treeFormLayout->addRow("Tree cell size", treeCellSize_);
  treeFormLayout->addRow("Tree cell cardinal", treeCellCardinal_);

  QGroupBox *normalsGroupBox{new QGroupBox{"Normals"}},
            *graphGroupBox{new QGroupBox{"Graph"}},
            *treeGroupBox{new QGroupBox{"Tree"}};

  normalsGroupBox->setLayout(normalFormLayout);
  graphGroupBox->setLayout(graphFormLayout);
  treeGroupBox->setLayout(treeFormLayout);

  QHBoxLayout *graphTreeLayout{new QHBoxLayout{}};
  graphTreeLayout->addWidget(graphGroupBox);
  graphTreeLayout->addWidget(treeGroupBox);
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////
  cone_->setChecked(true);
  cylinder_->setChecked(false);
  plane_->setChecked(true);
  sphere_->setChecked(true);
  torus_->setChecked(false);

  QVBoxLayout *shapesLayout{new QVBoxLayout{}};
  shapesLayout->addWidget(plane_);
  shapesLayout->addWidget(cone_);
  shapesLayout->addWidget(sphere_);
  shapesLayout->addWidget(cylinder_);
  shapesLayout->addWidget(torus_);

  QGroupBox *shapesGroupBox{new QGroupBox{"Shapes"}};
  shapesGroupBox->setLayout(shapesLayout);
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////
  QDialogButtonBox *dialogButtonBox{new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)};
  connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  //////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////
  QVBoxLayout *dialogLayout{new QVBoxLayout{}};
  dialogLayout->addWidget(normalsGroupBox);
  dialogLayout->addLayout(graphTreeLayout);
  dialogLayout->addWidget(shapesGroupBox);
  dialogLayout->addWidget(dialogButtonBox);
  //////////////////////////////////////////////////////////////////////////////////////////

  this->setLayout(dialogLayout);
}

double QPotasseDialog::neighborsDistance() const
{
  return neighborsDistance_->value();
}

bool QPotasseDialog::cone() const
{
  return cone_->isChecked();
}

bool QPotasseDialog::cylinder() const
{
  return cylinder_->isChecked();
}

bool QPotasseDialog::plane() const
{
  return plane_->isChecked();
}

bool QPotasseDialog::sphere() const
{
  return sphere_->isChecked();
}

bool QPotasseDialog::torus() const
{
  return torus_->isChecked();
}

