#include "tracedrawingrangedialog.h"
#include "ui_tracedrawingrangedialog.h"
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QColorDialog>

TraceDrawingRangeDialog::TraceDrawingRangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TraceDrawingRangeDialog),
    m_traceColorDialog(new QColorDialog(this))
{
    ui->setupUi(this);
    setWindowTitle(tr("Trace Drawing Settings"));
    m_traceColorDialog->setVisible(false);
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

QColorDialog *TraceDrawingRangeDialog::traceColorDialog()
{
    return m_traceColorDialog;
}

void TraceDrawingRangeDialog::on_traceColorToolButton_clicked()
{
    auto currentColor = m_traceColorDialog->currentColor();

    auto returnState = m_traceColorDialog->exec();

    if (returnState == QDialog::Rejected) {
        m_traceColorDialog->setCurrentColor(currentColor);
    }
}
