#ifndef TRACEDRAWINGRANGEDIALOG_H
#define TRACEDRAWINGRANGEDIALOG_H

#include <QDialog>

namespace Ui {
class TraceDrawingRangeDialog;
}

class QSlider;
class QSpinBox;
class QCheckBox;

class TraceDrawingRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TraceDrawingRangeDialog(QWidget *parent = 0);
    ~TraceDrawingRangeDialog();

    QSlider *drawingRangeSlider();
    QCheckBox *animateCheckBox();

private:
    Ui::TraceDrawingRangeDialog *ui;
};

#endif // TRACEDRAWINGRANGEDIALOG_H
