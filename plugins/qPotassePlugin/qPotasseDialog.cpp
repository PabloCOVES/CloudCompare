#include "qPotasseDialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QVBoxLayout>

qPotasseDialog::qPotasseDialog(QWidget *parent)
    : QDialog(parent),
      neighborsDistance_{new QDoubleSpinBox{}},
      cellSize_{new QDoubleSpinBox{}},
      cellCardinal_{new QSpinBox{}},
      planeCheckBox_{new QCheckBox{"Plane"}},
      sphereCheckBox_{new QCheckBox{"Sphere"}} {
    // Normals
    neighborsDistance_->setValue(.5);
    neighborsDistance_->setMinimum(.1);
    neighborsDistance_->setMaximum(2);
    neighborsDistance_->setSuffix("mm");

    QFormLayout *normalsLayout{new QFormLayout{}};
    normalsLayout->addRow("Neighbors distance", neighborsDistance_);

    QGroupBox *normalsGroupBox{new QGroupBox{"Normals"}};
    normalsGroupBox->setLayout(normalsLayout);

    // Tree
    cellSize_->setMaximum(15);
    cellSize_->setMinimum(1);
    cellSize_->setValue(8);
    cellSize_->setSuffix("mm");

    cellCardinal_->setMinimum(50);
    cellCardinal_->setMaximum(1e5);
    cellCardinal_->setValue(500);
    cellCardinal_->setSuffix("pts");

    QFormLayout *treeFormLayout{new QFormLayout{}};
    treeFormLayout->addRow("Cell size", cellSize_);
    treeFormLayout->addRow("Cell cardinal", cellCardinal_);

    QGroupBox *treeGroupBox{new QGroupBox{"Tree"}};
    treeGroupBox->setLayout(treeFormLayout);

    // Shapes
    planeCheckBox_->setChecked(true);
    sphereCheckBox_->setChecked(true);

    QVBoxLayout *shapesVBoxLayout{new QVBoxLayout{}};
    shapesVBoxLayout->addWidget(planeCheckBox_);
    shapesVBoxLayout->addWidget(sphereCheckBox_);

    QGroupBox *shapesGroupBox{new QGroupBox{"Shapes"}};
    shapesGroupBox->setLayout(shapesVBoxLayout);

    // Buttons
    QDialogButtonBox *buttonBox{
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel)};
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *dialogLayout{new QVBoxLayout{}};
    dialogLayout->addWidget(normalsGroupBox);
    dialogLayout->addWidget(treeGroupBox);
    dialogLayout->addWidget(shapesGroupBox);
    dialogLayout->addWidget(buttonBox);

    this->setLayout(dialogLayout);
}

double qPotasseDialog::neighborsDistance() const {
    return neighborsDistance_->value();
}

double qPotasseDialog::cellSize() const { return cellSize_->value(); }

int qPotasseDialog::cellCardinal() const { return cellCardinal_->value(); }

bool qPotasseDialog::plane() const { return planeCheckBox_->isChecked(); }
bool qPotasseDialog::sphere() const { return sphereCheckBox_->isChecked(); }
