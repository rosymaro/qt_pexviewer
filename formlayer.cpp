#include "formlayer.h"
#include "ui_formlayer.h"



FormLayer::FormLayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLayer)
{
    ui->setupUi(this);
    QStringList defalutTableHeader;
    QString TalbeHeader[] = {"","COLOR","Layer\nName","Layer\nNum","Layer\nType","Opacity\n"};

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnWidth(0,15);
    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->setColumnWidth(2,60);
    ui->tableWidget->setColumnWidth(3,60);
    ui->tableWidget->setColumnWidth(4,60);
    ui->tableWidget->setColumnWidth(5,100);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TableWidget Title Format 설정
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    QBrush brush(QColor(255, 255, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    QFont font;
    font.setBold(true);
    font.setWeight(75);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TableWidget Title 입력 및 반영
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i=1; i<6; i++){
        QTableWidgetItem *TableHeaderItem = new QTableWidgetItem;
        TableHeaderItem->setText(TalbeHeader[i]);
        TableHeaderItem->setTextAlignment(Qt::AlignCenter);
        TableHeaderItem->setFont(font);
        TableHeaderItem->setForeground(brush);
        ui->tableWidget->setItem(0,i,TableHeaderItem);
        ui->tableWidget->setRowHeight(0,50);
        ui->tableWidget->item(0,i)->setBackground(QBrush(QColor(80,80,80)));
    }
}

FormLayer::~FormLayer()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief FormLayer::ReceiveSplitData
/// \param row
/// \param column
/// \param inputDataVector
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FormLayer::ReceiveSplitData(int row, int column, const QVector <QVector <QString>> &inputDataVector)
{
// QTableWidget 에서 QStringList 로만 받기때문에 자료형변환
    QStringList vectorTOqstringlist;
    QStringList vectorTOqstringlistHoriLabels;
    QStringList vectorTOqstringcolorlist;
    QString LayerColorRed;
    QString LayerColorGreen;
    QString LayerColorBlue;
    QString LayerColorText;
    vectorTOqstringlistHoriLabels << " ";

// QTableWidget 크기 설정
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(row);
//    ui->tableWidget->setItemDelegateForColumn(5,new SliderDelegate);
    ui->tableWidget->setColumnWidth(5,150);

// Table 채우기
    int TalbeColumnCount = 3;       //input data 에서 가져 올 column 갯수
    for (int i=0; i<row ; i++)
    {
       // table value 채우기
        for (int j=0; j < TalbeColumnCount ; j++)
        {
            vectorTOqstringlist << inputDataVector.value(i+1).value(j);
//            vectorTOqstringlistHoriLabels << inputDataVector.value(0).value(j+1);
            QTableWidgetItem *tableitem = new QTableWidgetItem;
            tableitem->setText(vectorTOqstringlist[i*TalbeColumnCount+j]);
            tableitem->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem((i+1),j+2,tableitem);
        }

        //Opacity 채우기
        QSlider *OpacitySlider = new QSlider(Qt::Horizontal);
        QWidget *OpacitySliderWidget = new QWidget();
        OpacitySlider->setRange(0,100);
        OpacitySlider->setValue(100);
        OpacitySlider->setFocusPolicy(Qt::StrongFocus);
        QHBoxLayout *OpacityLayout = new QHBoxLayout(OpacitySliderWidget);
        OpacityLayout->addWidget(OpacitySlider);
        OpacitySliderWidget->setLayout(OpacityLayout);

//        OpacitySlider->setTickPosition(QSlider::TicksBothSides);
//        OpacitySlider->setTickInterval(10);
        ui->tableWidget->setCellWidget(i+1, 5, OpacitySliderWidget);
        QObject::connect(OpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(handleOpacitySlider(int)));

        // checkbox 채우기
        QCheckBox *checkBoxItem = new QCheckBox();
        checkBoxItem->setCheckState(Qt::Checked);
        QWidget *checkboxWidget = new QWidget();
        QHBoxLayout *checkboxLayout = new QHBoxLayout(checkboxWidget);
        checkboxLayout->addWidget(checkBoxItem);
        checkboxLayout->setAlignment(Qt::AlignCenter);
        checkboxLayout->setContentsMargins(0,0,0,0);
        checkboxWidget->setLayout(checkboxLayout);
        ui->tableWidget->setCellWidget(i+1,0,checkboxWidget);

        QWidget *widget = ui->tableWidget->cellWidget(i+1, 0);
        QCheckBox *checkboxInTable = widget->findChild<QCheckBox *>();
        QObject::connect(checkboxInTable, SIGNAL(stateChanged(int)), this, SLOT(tableWidget_checkBoxChanged()));

        // Layer color rgb defalut value
        LayerColorRed = inputDataVector.value(i+1).value(6);
        LayerColorGreen = inputDataVector.value(i+1).value(7);
        LayerColorBlue = inputDataVector.value(i+1).value(8);

        LayerColorText = "background-color: rgb(" + LayerColorRed;
        LayerColorText = LayerColorText + "," + LayerColorGreen + "," + LayerColorBlue + ")";

        // color pushbutton 채우기
        QPushButton *colorbutton = new QPushButton();
        colorbutton->setStyleSheet(LayerColorText);
        ui->tableWidget->setCellWidget(i+1,1,colorbutton);
        QObject::connect(colorbutton, SIGNAL(clicked()), this, SLOT(on_colorbutton_clicked()));
    }

///////////////////////////////tableWidget title 에 checkbox ///////////////////////////////
        QCheckBox *checkBoxItem = new QCheckBox();
        checkBoxItem->setCheckState(Qt::Checked);
        QWidget *checkboxWidget = new QWidget();
        QHBoxLayout *checkboxLayout = new QHBoxLayout(checkboxWidget);
        checkboxLayout->addWidget(checkBoxItem);
        checkboxLayout->setAlignment(Qt::AlignCenter);
        checkboxLayout->setContentsMargins(0,0,0,0);
        checkboxWidget->setLayout(checkboxLayout);
        ui->tableWidget->setCellWidget(0,0,checkboxWidget);

        QWidget *findwidget = ui->tableWidget->cellWidget(0, 0);
        QCheckBox *checkboxInTable = findwidget->findChild<QCheckBox *>();
        QObject::connect(checkboxInTable, SIGNAL(stateChanged(int)), this, SLOT(tableWidget_checkBoxChanged()));

///////////////////////////////tableWidget title 에 slider ///////////////////////////////

        QSpacerItem *HeaderVerticalSpacer = new QSpacerItem(0,20,QSizePolicy::Fixed, QSizePolicy::Fixed);
        QSlider *HeaderOpacitySlider = new QSlider();
        HeaderOpacitySlider->setOrientation(Qt::Horizontal);
        HeaderOpacitySlider->setValue(100);
        HeaderOpacitySlider->setRange(0,100);
        QWidget *HeaderSliderWidget = new QWidget();
        QVBoxLayout *HeaderSliderLayout = new QVBoxLayout(HeaderSliderWidget);
        HeaderSliderLayout->addItem(HeaderVerticalSpacer);
        HeaderSliderLayout->addWidget(HeaderOpacitySlider);
        HeaderSliderWidget->setLayout(HeaderSliderLayout);
        ui->tableWidget->setCellWidget(0,5,HeaderSliderWidget);

        QObject::connect(HeaderOpacitySlider, SIGNAL(valueChanged(int)), this, SLOT(handleOpacitySlider(int)));


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief FormLayer::on_tableWidget_itemChanged
/// \param checkBoxItem
/// description : checkbox 선택에 따라 Layer On/Off mainwindow status 창에 출력
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FormLayer::tableWidget_checkBoxChanged()
{
    QCheckBox *checkboxInTable = qobject_cast<QCheckBox*>(sender());
    if (!checkboxInTable)
        return;

    int checkboxrow = -1;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QWidget *widget = ui->tableWidget->cellWidget(i, 0);
        QCheckBox *checkbox = widget->findChild<QCheckBox*>();
        if (checkbox == checkboxInTable) {
            checkboxrow = i;
            break;
        }
    }

    if (checkboxrow == 0) {
        QString CommentAllCheckedCheckBox = "All Layer Checked";
        QString CommentAllUncheckedCheckBox = "All Layer Unchecked";

        if (checkboxInTable->isChecked()) {
            for (int i = 1; i < ui->tableWidget->rowCount(); i++) {
                QWidget *widget = ui->tableWidget->cellWidget(i, 0);
                QCheckBox *checkbox = widget->findChild<QCheckBox*>();
                checkbox->setCheckState(Qt::Checked);
            }
            qDebug() << "All Layer Checkbox Checked.";
            emit outputLayerStatus(CommentAllCheckedCheckBox);

        } else {
                for (int i = 1; i < ui->tableWidget->rowCount(); i++) {
                    QWidget *widget = ui->tableWidget->cellWidget(i, 0);
                    QCheckBox *checkbox = widget->findChild<QCheckBox*>();
                    checkbox->setCheckState(Qt::Unchecked);
                }
                qDebug() << "All Layer Checkbox Unchecked.";
                emit outputLayerStatus(CommentAllUncheckedCheckBox);
            }
        return;
    }

    QTableWidgetItem *readcheckbox = ui->tableWidget->item(checkboxrow,2);
    QString selectedLayer = readcheckbox->text();
    QString printLayer = "selectedLayer = " + selectedLayer;

    if (checkboxInTable->isChecked()) {
        printLayer = printLayer + "  Layer  On";
        emit outputLayerStatus(printLayer);
        qDebug() << "Checkbox at row" << checkboxrow << "is checked.";
    } else {
        printLayer = printLayer + "  Layer  Off";
        emit outputLayerStatus(printLayer);
        qDebug() << "Checkbox at row" << checkboxrow << "is unchecked.";
    }
}

void FormLayer::on_colorbutton_clicked()
{
    QString LayerColorname;
    QString LayerColorText;
    QPushButton *ColorCheckButton = qobject_cast<QPushButton*>(sender());

    int colorbuttonRow = ui->tableWidget->currentRow();

    QString OldColorStyleSheet = ColorCheckButton->styleSheet();

    QStringList splittext = OldColorStyleSheet.split("(");
    QString splitrgbtemp = splittext[1].remove(")");
    QStringList splitrgb = splitrgbtemp.split(",");

    qDebug() << "OldColor = " << OldColorStyleSheet;
    qDebug() << "OldColorsplit = " << splitrgb;

//    int LayerColorRed = splitrgb[0].toInt();
//    int LayerColorGreen = splitrgb[1].toInt();
//    int LayerColorBlue = splitrgb[2].toInt();

    QString OldLayerColorRed = splitrgb[0];
    QString OldLayerColorGreen = splitrgb[1];
    QString OldLayerColorBlue = splitrgb[2];
    QString PrintOldLayerColor = "Red Num = " + OldLayerColorRed;
    PrintOldLayerColor = PrintOldLayerColor + "     Green Num = " + OldLayerColorGreen + "      Blue Num = " + OldLayerColorBlue;

//    QRgb LayerColorDefault = qRgb(LayerColorRed, LayerColorGreen, LayerColorBlue);

    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if (color.isValid()){
        LayerColorname = color.name();

        QColor HexColor(LayerColorname);
        int NewColorRed = HexColor.red();
        int NewColorGreen = HexColor.green();
        int NewColorBlue = HexColor.blue();

        LayerColorText = "background-color: rgb(" + QString::number(NewColorRed);
        LayerColorText = LayerColorText + "," + QString::number(NewColorGreen) + "," + QString::number(NewColorBlue) + ")";

//        QStringList newcolorsplittemp = LayerColorText.split(":");
//        QString newcolorsplitrgb = newcolorsplittemp[1].remove(" ");
//        qDebug() << newcolorsplitrgb;



//        qDebug() << "Red = " << NewColorRed << "Green = " << NewColorGreen << "Blue = " << NewColorBlue;
        qDebug() << "NewColor = " <<LayerColorText;

        ui->tableWidget->cellWidget(colorbuttonRow,1)->setStyleSheet(LayerColorText);

    } else {
        ui->tableWidget->cellWidget(colorbuttonRow,1)->setStyleSheet(OldColorStyleSheet);
        emit outputLayerStatus(PrintOldLayerColor);
        qDebug() << "Color is not selected";
    }

//    QRgb test = QColorDialog::getRgba(orc);


}

void FormLayer::handleOpacitySlider(int Opacity)
{
    int sliderrow=-1;
    QSlider *OpacityInTalbe = qobject_cast<QSlider*>(sender());
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QWidget *widget = ui->tableWidget->cellWidget(i, 5);
        QSlider *slider = widget->findChild<QSlider*>();

        if (slider == OpacityInTalbe) {
            sliderrow = i;

            if (sliderrow == 0){

                for (int k=1; k < ui->tableWidget->rowCount(); k++) {
                    QWidget *SliderChangeWidget = ui->tableWidget->cellWidget(k, 5);
                    QSlider *SliderChange = SliderChangeWidget->findChild<QSlider*>();
                    SliderChange->setValue(Opacity);
                }
            } else {
                QTableWidgetItem *ReadLayer = ui->tableWidget->item(sliderrow,2);
                QString selectedLayer = ReadLayer->text();
                QString printLayer = "selectedLayer = " + selectedLayer;
                printLayer = printLayer + "  Layer  Opacity = " + QString::number(Opacity);
                emit outputLayerStatus(printLayer);
                qDebug() << printLayer;
            }
            break;
        }
    }
}
