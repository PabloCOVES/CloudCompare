#include <QDialog>

class QWidget;
class QDoubleSpinBox;
class QCheckBox;
class QSpinBox;

class QPotasseDialog
: public QDialog
{
  public:
    explicit QPotasseDialog(QWidget *parent = Q_NULLPTR);

    double neighborsDistance() const;

    bool cone() const;
    bool cylinder() const;
    bool plane() const;
    bool sphere() const;
    bool torus() const;

  protected:
  private:
    QDoubleSpinBox *neighborsDistance_,
                   *graphCellSize_,
                   *treeCellSize_;

    QSpinBox *graphCellCardinal_,
             *treeCellCardinal_;

    QCheckBox *cone_;
    QCheckBox *cylinder_;
    QCheckBox *plane_;
    QCheckBox *sphere_;
    QCheckBox *torus_;
};
