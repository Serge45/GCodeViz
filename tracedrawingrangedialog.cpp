#include "tracedrawingrangedialog.h"
#include "ui_tracedrawingrangedialog.h"
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>

TraceDrawingRangeDialog::TraceDrawingRangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TraceDrawingRangeDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Trace Drawing Settings"));
}

TraceDrawingRangeDialog::~TraceDrawingRangeDialog()
{
    delete ui;
}

QSlider *TraceDrawingRangeDialog::drawingRangeSlider()
{
    return ui->drawingRangeSlider;
}

QCheckBox *TraceDrawingRangeDialog::animateCheckBox()
{
    return ui->drawingAnimateCheckBox;
}
