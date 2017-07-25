#ifndef _QPOTASSE_DIALOG_H_
#define _QPOTASSE_DIALOG_H_

#include <QDialog>

class QWidget;
class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;

class qPotasseDialog : public QDialog {
   public:
    explicit qPotasseDialog(QWidget *parent = Q_NULLPTR);

    double neighborsDistance() const;

    double cellSize() const;
    int cellCardinal() const;

    bool plane() const;
    bool sphere() const;

   protected:
   private:
    QDoubleSpinBox *neighborsDistance_;

    QDoubleSpinBox *cellSize_;
    QSpinBox *cellCardinal_;

    QCheckBox *planeCheckBox_, *sphereCheckBox_;
};

#endif  //_QPOTASSE_DIALOG_H_
