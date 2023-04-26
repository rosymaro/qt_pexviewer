#ifndef FORMLAYER_H
#define FORMLAYER_H

#include <QDialog>
#include <QColorDialog>
#include <QtWidgets/QCheckBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QPushButton>
#include <QtWidgets/QSlider>
#include <QRgba64>
#include <QDebug>
#include "Rendering/Src/lve_window.hpp"
#include "Rendering/Src/dtaorendersystem.h"
#include "T2D.h"


namespace Ui {
class FormLayer;
}

class FormLayer : public QDialog
{
    Q_OBJECT

public:
    explicit FormLayer(LveWindow *w, QWidget *parent = nullptr);
    ~FormLayer();
    void ReceiveLayerInformation(T2D &t2d);
    void MakeLayerInformationTableHeader();
    void MakeLayerInformationTalbe();
    void MakeTableWidgetItemForText(int);
    void MakeSliderForOpacity(int);
    void MakeCheckBox(int);
    void MakeColorButton(int);
    void MakeHeaderCheckBoxAndSlider();
    void ChangeAllCheckBoxState(QCheckBox *);
    void ChangeCheckBoxStateToOn(int, string, QString);
    void ChangeCheckBoxStateToOff(int, string, QString);
    void GetPushButtonOldColor(QPushButton *, int *, int *, int *, int *, QRgb *);
    void EnterPushButtonNewColor(int *, QRgb *);
    int FindCheckBoxRow(QCheckBox *);

    bool m_layer_all_check = false;

public slots:
    void on_checkboxInTable_stateChanged();
    void on_colorbutton_clicked();
    void on_opacityslider_valueChanged(int);
    void OpacitySliderValueChange(int);

private:
    LveWindow *m_window;
    Ui::FormLayer *ui;
    T2D *t2d_layer_information;
    int data_row_for_t2d = -1;
    int checkboxrow = -1;
    int table_default_column = 6;
    QString empty_for_emit = "";
    QString CommentAllCheckedCheckBox = "All Layer Checked";
    QString CommentAllUncheckedCheckBox = "All Layer Unchecked";


signals:
    void outputLayerStatus(QString printLayer);

};

#endif // FORMLAYER_H
