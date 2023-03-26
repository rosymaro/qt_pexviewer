#include "formlayer.h"
#include "ui_formlayer.h"
#include "Rendering/Src/dtaorendersystem.h"
#include "Rendering/Src/lve_model.hpp"
#include <string>



FormLayer::FormLayer(LveWindow *w, QWidget *parent) :
    QDialog(parent), m_window(w),
    ui(new Ui::FormLayer)
{
    ui->setupUi(this);
    MakeLayerInformationTableHeader();
}

FormLayer::~FormLayer()
{
    delete ui;
}

void FormLayer::ReceiveLayerInformation(T2D &t2d)
{
    ui->tableWidget->clearContents();
    t2d_layer_information = &t2d;
    MakeLayerInformationTableHeader();
    MakeLayerInformationTalbe();
}

void FormLayer::MakeLayerInformationTalbe()
{
    // QTableWidget ¬ê¸° ¤ì 
    ui->tableWidget->setColumnCount(table_default_column);
    ui->tableWidget->setRowCount(t2d_layer_information->LayoutData10by10.size());
    ui->tableWidget->setColumnWidth(5,80);

    // Table ì±ì°ê¸
    for (int i=1; i < t2d_layer_information->LayoutData10by10.size() ; i++)
    {
        MakeTableWidgetItemForText(i);
        MakeCheckBox(i);
        MakeColorButton(i);
        MakeSliderForOpacity(i);
    }
    MakeHeaderCheckBoxAndSlider();

    // Table Edit ê¸ì, ¤ì¤  í ê¸ì
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void FormLayer::on_checkboxInTable_stateChanged()
{
    int checkboxrow = -1;
    QCheckBox *checkboxInTable = qobject_cast<QCheckBox*>(sender());
    if (!checkboxInTable)
        return;

    checkboxrow = FindCheckBoxRow(checkboxInTable);

    if (checkboxrow == 0) {
        ChangeAllCheckBoxState(checkboxInTable);
        return;
    }

    // selected checkbox layer name
    QTableWidgetItem *readcheckbox = ui->tableWidget->item(checkboxrow,2);
    QString selectedLayer = readcheckbox->text();
    QString printLayer = "selectedLayer = " + selectedLayer;

    // selected checkbox layer num
    QTableWidgetItem *ReadLayerNumber = ui->tableWidget->item(checkboxrow,3);
    float LayerNumber = stof(ReadLayerNumber->text().toStdString());

    if (checkboxInTable->isChecked()) {
        ChangeCheckBoxStateToOn(checkboxrow, LayerNumber, printLayer);
    } else {
        ChangeCheckBoxStateToOff(checkboxrow, LayerNumber, printLayer);
    }
}

void FormLayer::on_colorbutton_clicked()
{
    QString OldLayerColorRed = "";
    QString OldLayerColorGreen = "";
    QString OldLayerColorBlue = "";
    QString OldLayerColorAlpha = "";
    int OldLayerColorRedNum = 0;
    int OldLayerColorGreenNum = 0;
    int OldLayerColorBlueNum = 0;
    int OldLayerColorAlphaNum = 0;
    QRgb orc = qRgba(0, 0, 0, 0);

    QPushButton *ColorCheckButton = qobject_cast<QPushButton*>(sender());

    int colorbuttonRow = ui->tableWidget->currentRow();
    data_row_for_t2d = colorbuttonRow-1;

    GetPushButtonOldColor(ColorCheckButton, &OldLayerColorRedNum, &OldLayerColorGreenNum, &OldLayerColorBlueNum, &OldLayerColorAlphaNum, &orc);
    EnterPushButtonNewColor(&colorbuttonRow, &orc);
}

void FormLayer::on_opacityslider_valueChanged(int Opacity)
{
    // ui->tableWidget->currentRow() ë¡ì°¾ì¼¤ê³  ë©´, current row ê° -1 ë¡ì´
    int sliderrow = -1;
    int sliderrow_for_t2d = -1;
    QString LayerColorname;
    QString LayerColorText;
    QSlider *OpacityInTalbe = qobject_cast<QSlider*>(sender());

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QWidget *widget = ui->tableWidget->cellWidget(i, 5);
        QSlider *slider = widget->findChild<QSlider*>();

        if (slider == OpacityInTalbe) {
            sliderrow = i;
            sliderrow_for_t2d = sliderrow-1;
            if (sliderrow == 0){
                for (int k=1; k < ui->tableWidget->rowCount(); k++) {
                    QWidget *SliderChangeWidget = ui->tableWidget->cellWidget(k, 5);
                    QSlider *SliderChange = SliderChangeWidget->findChild<QSlider*>();
                    SliderChange->setValue(Opacity);
                    t2d_layer_information->LayoutData10by10[sliderrow_for_t2d].color.a = Opacity;
                    emit outputLayerStatus(empty_for_emit);
                }
            } else {
                QTableWidgetItem *ReadLayer = ui->tableWidget->item(sliderrow,2);
                QString selectedLayer = ReadLayer->text();
                QString printLayer = "selectedLayer = " + selectedLayer;
                printLayer = printLayer + "  Layer  Opacity = " + QString::number(Opacity);
                QString OldColorStyleSheet = ui->tableWidget->cellWidget(sliderrow,1)->styleSheet();
                QStringList splittext = OldColorStyleSheet.split("(");
                QString splitrgbtemp = splittext[1].remove(")");
                QStringList splitrgb = splitrgbtemp.split(",");

                QString OldLayerColorRed = splitrgb[0];
                QString OldLayerColorGreen = splitrgb[1];
                QString OldLayerColorBlue = splitrgb[2];
                QString OldLayerColorAlpha = QString::number(Opacity);

                LayerColorText = "background-color: rgba(" + OldLayerColorRed;
                LayerColorText = LayerColorText + "," + OldLayerColorGreen + "," + OldLayerColorBlue + "," + OldLayerColorAlpha + ")";
                ui->tableWidget->cellWidget(sliderrow,1)->setStyleSheet(LayerColorText);
                t2d_layer_information->LayoutData10by10[sliderrow_for_t2d].color.a = Opacity;
                emit outputLayerStatus(printLayer);

                QTableWidgetItem *ReadLayerNumber = ui->tableWidget->item(sliderrow,3);
                float LayerNumber = stof(ReadLayerNumber->text().toStdString());
                DtaoRenderSystem *renderer = this->m_window->getRenderer();
                renderer->getCustomOpacity(LayerNumber,Opacity/(float)255);
            }
            break;
        }
    }
}

void FormLayer::OpacitySliderValueChange(int OpacityValue){

    qDebug() << "test number ======================================= " << OpacityValue;
    emit outputLayerStatus(empty_for_emit);
}

void FormLayer::MakeLayerInformationTableHeader(){
    QString TalbeHeader[] = {"","Color","Layer\nName","Layer\nNum","Layer\nType","Opacity\n"};
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnWidth(0,20);
    ui->tableWidget->setColumnWidth(1,40);
    ui->tableWidget->setColumnWidth(2,40);
    ui->tableWidget->setColumnWidth(3,40);
    ui->tableWidget->setColumnWidth(4,40);
    ui->tableWidget->setColumnWidth(5,60);

    /// TableWidget Title Format ¤ì 
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    QFont font;
    font.setPointSize(8);
    font.setBold(true);
    font.setWeight(55);

    /// TableWidget Title ë ¥ ë°ë°ì
    for (int i=1; i<6; i++){
        QTableWidgetItem *TableHeaderItem = new QTableWidgetItem;
        TableHeaderItem->setText(TalbeHeader[i]);
        TableHeaderItem->setTextAlignment(Qt::AlignCenter);
        TableHeaderItem->setFont(font);
        TableHeaderItem->setForeground(brush);
        ui->tableWidget->setItem(0,i,TableHeaderItem);
        ui->tableWidget->setRowHeight(0,50);
        ui->tableWidget->item(0,i)->setBackground(QBrush(QColor(100,100,100)));
    }
}

void FormLayer::MakeTableWidgetItemForText(int table_row){
    data_row_for_t2d = table_row-1;
    QTableWidgetItem *tableitem_name = new QTableWidgetItem;
    QTableWidgetItem *tableitem_num = new QTableWidgetItem;
    QTableWidgetItem *tableitem_type = new QTableWidgetItem;

    tableitem_name->setText(t2d_layer_information->LayoutData10by10[data_row_for_t2d].layername.c_str());
    tableitem_num->setText(QString::number(t2d_layer_information->LayoutData10by10[data_row_for_t2d].layernum));
    tableitem_type->setText(QString::number(t2d_layer_information->LayoutData10by10[data_row_for_t2d].datatype));

    tableitem_name->setTextAlignment(Qt::AlignCenter);
    tableitem_num->setTextAlignment(Qt::AlignCenter);
    tableitem_type->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem((table_row),2,tableitem_name);
    ui->tableWidget->setItem((table_row),3,tableitem_num);
    ui->tableWidget->setItem((table_row),4,tableitem_type);
}

void FormLayer::MakeSliderForOpacity(int table_row){
    data_row_for_t2d = table_row-1;
    QSlider *OpacitySlider = new QSlider(Qt::Horizontal);
    QWidget *OpacitySliderWidget = new QWidget();
    OpacitySlider->setRange(0,255);
    OpacitySlider->setValue(t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.a);
    OpacitySlider->setFocusPolicy(Qt::StrongFocus);
    QHBoxLayout *OpacityLayout = new QHBoxLayout(OpacitySliderWidget);
    OpacityLayout->addWidget(OpacitySlider);
    OpacitySliderWidget->setLayout(OpacityLayout);
    ui->tableWidget->setCellWidget(table_row, 5, OpacitySliderWidget);
    QObject::connect(OpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(on_opacityslider_valueChanged(int)));
}

void FormLayer::MakeCheckBox(int table_row){
    data_row_for_t2d = table_row-1;
    QCheckBox *checkBoxItem = new QCheckBox();
    checkBoxItem->setCheckState(Qt::Unchecked);
    QWidget *checkboxWidget = new QWidget();
    QHBoxLayout *checkboxLayout = new QHBoxLayout(checkboxWidget);
    checkboxLayout->addWidget(checkBoxItem);
    checkboxLayout->setAlignment(Qt::AlignCenter);
    checkboxLayout->setContentsMargins(0,0,0,0);
    checkboxWidget->setLayout(checkboxLayout);
    ui->tableWidget->setCellWidget(table_row,0,checkboxWidget);
    QObject::connect(checkBoxItem, SIGNAL(stateChanged(int)), this, SLOT(on_checkboxInTable_stateChanged()));
}

void FormLayer::MakeColorButton(int table_row){
    QString LayerColorRed;
    QString LayerColorGreen;
    QString LayerColorBlue;
    QString LayerColorAlpha;
    QString LayerColorText;
    data_row_for_t2d = table_row-1;
    // Layer color rgb defalut value
    int DefaultRedColor = t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.r;
    int DefaultGreenColor = t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.g;
    int DefaultBlueColor = t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.b;
    int DefaultAlpha = t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.a;
    QColor DefaultColor = QColor(DefaultRedColor, DefaultGreenColor, DefaultBlueColor, DefaultAlpha);

    // Layer color rgb defalut value
    LayerColorRed = QString::number(t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.r);
    LayerColorGreen = QString::number(t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.g);
    LayerColorBlue = QString::number(t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.b);
    LayerColorAlpha = QString::number(t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.a);

    LayerColorText = "background-color: rgba(" + LayerColorRed;
    LayerColorText = LayerColorText + "," + LayerColorGreen + "," + LayerColorBlue + "," + LayerColorAlpha + ")";

    // color pushbutton ì±ì°ê¸
    QPushButton *colorbutton = new QPushButton();
    colorbutton->setStyleSheet(LayerColorText);
    ui->tableWidget->setCellWidget(table_row,1,colorbutton);
    QObject::connect(colorbutton, SIGNAL(clicked()), this, SLOT(on_colorbutton_clicked()));
}

void FormLayer::MakeHeaderCheckBoxAndSlider(){
    //tableWidget title checkbox
    QCheckBox *checkBoxItem = new QCheckBox();
    checkBoxItem->setCheckState(Qt::Unchecked);
    QWidget *checkboxWidget = new QWidget();
    QHBoxLayout *checkboxLayout = new QHBoxLayout(checkboxWidget);
    checkboxLayout->addWidget(checkBoxItem);
    checkboxLayout->setAlignment(Qt::AlignCenter);
    checkboxLayout->setContentsMargins(0,0,0,0);
    checkboxWidget->setLayout(checkboxLayout);
    ui->tableWidget->setCellWidget(0,0,checkboxWidget);
    QObject::connect(checkBoxItem, SIGNAL(stateChanged(int)), this, SLOT(on_checkboxInTable_stateChanged()));

    //tableWidget title slider
    QSpacerItem *HeaderVerticalSpacer = new QSpacerItem(0,20,QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSlider *HeaderOpacitySlider = new QSlider();
    HeaderOpacitySlider->setOrientation(Qt::Horizontal);
    HeaderOpacitySlider->setRange(0,255);
    HeaderOpacitySlider->setValue(200);
    QWidget *HeaderSliderWidget = new QWidget();
    QVBoxLayout *HeaderSliderLayout = new QVBoxLayout(HeaderSliderWidget);
    HeaderSliderLayout->addItem(HeaderVerticalSpacer);
    HeaderSliderLayout->addWidget(HeaderOpacitySlider);
    HeaderSliderWidget->setLayout(HeaderSliderLayout);
    ui->tableWidget->setCellWidget(0,5,HeaderSliderWidget);
    QObject::connect(HeaderOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(on_opacityslider_valueChanged(int)));
}

int FormLayer::FindCheckBoxRow(QCheckBox *checkBoxInCell){
    QCheckBox &temp = *checkBoxInCell;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QWidget *widget = ui->tableWidget->cellWidget(i, 0);
        QCheckBox *checkbox = widget->findChild<QCheckBox*>();
        if (checkbox == &temp) {
            checkboxrow = i;
            break;
        }
    }
    return checkboxrow;
}

void FormLayer::ChangeAllCheckBoxState(QCheckBox *checkBoxInCell){
    if (checkBoxInCell->isChecked()) {
        for (int i = 1; i < ui->tableWidget->rowCount(); i++) {
            data_row_for_t2d = i-1;
            QWidget *widget = ui->tableWidget->cellWidget(i, 0);
            QCheckBox *checkbox = widget->findChild<QCheckBox*>();
            checkbox->setCheckState(Qt::Checked);
            t2d_layer_information->LayoutData10by10[data_row_for_t2d].checking = true;
        }
        qDebug() << "All Layer Checkbox Checked.";
        emit outputLayerStatus(CommentAllCheckedCheckBox);

    } else {
        for (int i = 1; i < ui->tableWidget->rowCount(); i++) {
            data_row_for_t2d = i-1;
            QWidget *widget = ui->tableWidget->cellWidget(i, 0);
            QCheckBox *checkbox = widget->findChild<QCheckBox*>();
            checkbox->setCheckState(Qt::Unchecked);
            t2d_layer_information->LayoutData10by10[data_row_for_t2d].checking = false;
        }
        qDebug() << "All Layer Checkbox Unchecked.";
        emit outputLayerStatus(CommentAllUncheckedCheckBox);
    }
}

void FormLayer::ChangeCheckBoxStateToOn(int check_box_row, float layer_num, QString print_comment){
    data_row_for_t2d = check_box_row-1;
    DtaoRenderSystem *renderer = this->m_window->getRenderer();
    renderer->getCustomVisiblity(layer_num, true);
    t2d_layer_information->LayoutData10by10[data_row_for_t2d].checking = true;
    print_comment = print_comment + "  Layer  On";
    emit outputLayerStatus(print_comment);
}

void FormLayer::ChangeCheckBoxStateToOff(int check_box_row, float layer_num, QString print_comment){
    data_row_for_t2d = check_box_row-1;
    DtaoRenderSystem *renderer = this->m_window->getRenderer();
    renderer->getCustomVisiblity(layer_num, false);
    print_comment = print_comment + "  Layer  Off";
    t2d_layer_information->LayoutData10by10[data_row_for_t2d].checking = false;
    emit outputLayerStatus(print_comment);
}

void FormLayer::GetPushButtonOldColor(QPushButton *old_push_button, int *oldred, int *oldgreen, int *oldblue, int *oldalpha, QRgb *oldrgba){
    QPushButton &oldcolor = *old_push_button;
    QString OldColorStyleSheet = oldcolor.styleSheet();
    QStringList splittext = OldColorStyleSheet.split("(");
    QString splitrgbtemp = splittext[1].remove(")");
    QStringList splitrgb = splitrgbtemp.split(",");

    QString OldLayerColorRed = splitrgb[0];
    QString OldLayerColorGreen = splitrgb[1];
    QString OldLayerColorBlue = splitrgb[2];
    QString OldLayerColorAlpha = splitrgb[3];

    *oldred = OldLayerColorRed.toInt();
    *oldgreen = OldLayerColorGreen.toInt();
    *oldblue = OldLayerColorBlue.toInt();
    *oldalpha = OldLayerColorAlpha.toInt();

    *oldrgba = qRgba(*oldred, *oldgreen, *oldblue, *oldalpha);
}

void FormLayer::EnterPushButtonNewColor(int *selected_color_button_row, QRgb *oldrgba){
    QString LayerColorText;
    QRgb chc = QColorDialog::getRgba(*oldrgba);
    int NewColorRed = qRed(chc);
    int NewColorGreen = qGreen(chc);
    int NewColorBlue = qBlue(chc);
    int NewColorAlpha = qAlpha(chc);

    LayerColorText = "background-color: rgba(" + QString::number(NewColorRed);
    LayerColorText = LayerColorText + "," + QString::number(NewColorGreen) + "," + QString::number(NewColorBlue) + "," + QString::number(NewColorAlpha) + ")";
    ui->tableWidget->cellWidget(*selected_color_button_row,1)->setStyleSheet(LayerColorText);

    t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.r = NewColorRed;
    t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.g = NewColorGreen;
    t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.b = NewColorBlue;
    t2d_layer_information->LayoutData10by10[data_row_for_t2d].color.a = NewColorAlpha;

    float LayerNumber = stof(ui->tableWidget->item(*selected_color_button_row,3)->text().toStdString());
    DtaoRenderSystem *renderer = this->m_window->getRenderer();
    renderer->getCustomColor(LayerNumber, glm::vec3 {NewColorRed/(float)255,NewColorGreen/(float)255,NewColorBlue/(float)255});

    emit outputLayerStatus(empty_for_emit);

    QWidget *SliderChangeWidget = ui->tableWidget->cellWidget(*selected_color_button_row, 5);
    QSlider *SliderChange = SliderChangeWidget->findChild<QSlider*>();
    SliderChange->setValue(NewColorAlpha);
}
