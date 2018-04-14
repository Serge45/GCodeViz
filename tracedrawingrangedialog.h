#ifndef TRACEDRAWINGRANGEDIALOG_H
#define TRACEDRAWINGRANGEDIALOG_H

#include <QDialog>

namespace Ui {
class TraceDrawingRangeDialog;
}

class QSlider;
class QSpinBox;
class QCheckBox;
class QColorDialog;

class TraceDrawingRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TraceDrawingRangeDialog(QWidget *parent = 0);
    ~TraceDrawingRangeDialog();

    QSlider *drawingRangeSlider();
    QCheckBox *animateCheckBox();
    QColorDialog *traceColorDialog();

private slots:
    void on_traceColorToolButton_clicked();

private:
    Ui::TraceDrawingRangeDialog *ui;
    QColorDialog *m_traceColorDialog;
};

#endif // TRACEDRAWINGRANGEDIALOG_H
