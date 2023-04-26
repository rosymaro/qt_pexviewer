#include "forminfo.h"
#include "ui_forminfo.h"
#include <QDebug>
#include <QPushButton>
#include "cmath"
#include "defineParam.h"
#include <string>
#include <QDoubleValidator>
#include <QIntValidator>


FormInfo::FormInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormInfo)
{
    ui->setupUi(this);    

    //レ옄 쒖빟 ㅼ젙
    QDoubleValidator *validator_pos = new QDoubleValidator(-9999.0,9999.0,4,this);
    QIntValidator *validator_tilt = new QIntValidator(-90,90,this);
    QIntValidator *validator_rot = new QIntValidator(0,359,this);
    QDoubleValidator *validator_zoom = new QDoubleValidator(0,9999.0,4,this);
    ui->pos_x       ->setValidator(validator_pos);
    ui->pos_y       ->setValidator(validator_pos);
    ui->pos_z       ->setValidator(validator_pos);
    ui->ang_tilt    ->setValidator(validator_tilt);
    ui->ang_rotation->setValidator(validator_rot);
    ui->zoom        ->setValidator(validator_zoom);

    this->setEnabled(false);
    //edit 媛珥덇린
    setTextPosTotal(&TEMP_POS);
}

FormInfo::~FormInfo()
{
    delete ui;
}

void FormInfo::setTextPosTotal(POS_MONITORING *pos)
{
    if(isFileOpen)
    {
        double rot = pos->rotation;
        if (rot <0) {
            rot = rot+360;
        }
        QString str_rot = QString::number(rot);
        setTextTrim(ui->pos_x, &pos->x, 1, "");
        setTextTrim(ui->pos_y, &pos->y, 1, "");
        setTextTrim(ui->pos_z, &pos->z, 1, "");
        setTextTrim(ui->ang_tilt, &pos->tilt, 1, "");
        setTextTrim(ui->ang_rotation, str_rot);
        setTextTrim(ui->zoom, &pos->zoom, 100, "%");
    }
    else
    {
        setTextTrim(ui->pos_x, "");
        setTextTrim(ui->pos_y, "");
        setTextTrim(ui->pos_z, "");
        setTextTrim(ui->ang_tilt, "");
        setTextTrim(ui->ang_rotation, "");
        setTextTrim(ui->zoom, "");
    }
}

void FormInfo::receiveFile(T2D &t2d)
{
    isFileOpen = true;
    rendering_full = &t2d;
    this->setEnabled(true);
}

void FormInfo::receivePointPos(POS_MONITORING &pos)
{
    this->POS = &pos;
    TEMP_POS = pos;         //TEMP_POS 븷 : enter 瑜꾨Ⅴ湲꾧퉴吏 媛믪쓣 媛뽮퀬 덈뒗 μ냼
}

void FormInfo::changePos()
{
    if (isEndedEdit)
    {
        TEMP_POS = *POS;
        setTextPosTotal(POS);
    }
}

void FormInfo::addItemToPosList(POS_SET *position_item)    //POS_STACK position ｊ린
{
    POS_STACK.push_back(*position_item);
    item_data_i = POS_STACK.size() - 1 ;
    addItemToListWidget();
}

void FormInfo::addItemToListWidget()
{
    QListWidgetItem *widgetItem = new QListWidgetItem(QString::fromStdString(POS_STACK[item_data_i].name),ui->listWidget);
    widgetItem->setFlags (widgetItem->flags() | Qt::ItemIsEditable);
    widgetItem->setData(Qt::UserRole,item_data_i);
}

void FormInfo::setTextTrim(QLineEdit *text, double *value, int scale_, QString add_text_)
{
    text->setText(QString::number(round(*value*10000)/10000*scale_)+add_text_);
}

void FormInfo::setTextTrim(QLineEdit *text, QString text_)
{
    text->setText(text_);
}

void FormInfo::makePosListItem()
{
    std::string temp_str = "position";
    pos_number += 1;
    temp_str.append(to_string(pos_number));
    POS_SET TEMP;
    TEMP.name = temp_str;
    TEMP.position = *POS;
    addItemToPosList(&TEMP);
}

QString FormInfo::makePosText()
{
    QString msg;
    if(current_item != -1 && make_status_bar_msg)
    {
        int i = ui->listWidget->currentItem()->data(Qt::UserRole).toInt();
        msg =       QString::fromStdString(POS_STACK[i].name) + " :"
        + " ( " +      QString::number(POS_STACK[i].position.x) + ", "
        +              QString::number(POS_STACK[i].position.y) + ", "
        +              QString::number(POS_STACK[i].position.z) + " )"
        + " tilt : " + QString::number(POS_STACK[i].position.tilt)
        + " rot : "  + QString::number(POS_STACK[i].position.rotation)
        + " zoom : " + QString::number(POS_STACK[i].position.zoom * 100) + "%";
    }
    else{
        msg = "";
    }
    return msg;
}

void FormInfo::sendStatusBarMsg(QString msg)
{
    signalStatusBarMsg(msg);

}

void FormInfo::inputValueToTemp(double *point, double value_)
{
    *point = value_;
}

void FormInfo::convertTextToValue(double *point, const QString &arg1)
{
    double double_arg = arg1.toDouble();
    inputValueToTemp(point, double_arg);
}

void FormInfo::inputPos()       //吏곸젒 낅젰媛믪쓣 POS 援ъ“泥댁뿉 ｌ쓬
{
    if(isFileOpen)
    {
        isEndedEdit = false;
        *POS = TEMP_POS;
        emit signalDirectlyInputPos();
        makePosListItem();
        isEndedEdit = true;
    }
}

//낅젰ㅼ뼱붿쓣 
void FormInfo::on_pos_x_textEdited(const QString &arg1)         {convertTextToValue(&TEMP_POS.x,arg1);}
void FormInfo::on_pos_y_textEdited(const QString &arg1)         {convertTextToValue(&TEMP_POS.y,arg1);}
void FormInfo::on_pos_z_textEdited(const QString &arg1)         {convertTextToValue(&TEMP_POS.z,arg1);}
void FormInfo::on_ang_tilt_textEdited(const QString &arg1)      {convertTextToValue(&TEMP_POS.tilt,arg1);}
void FormInfo::on_ang_rotation_textEdited(const QString &arg1)  {convertTextToValue(&TEMP_POS.rotation,arg1);}
void FormInfo::on_zoom_textEdited(const QString &arg1)
{   //zoom  100% 寃쎌슦 1 媛믪쓣 ｌ뼱섍린 뚮Ц곕줈 泥섎━
    double double_arg = arg1.toDouble();
    double_arg = double_arg/100;
    inputValueToTemp(&TEMP_POS.zoom,double_arg);
}

//enter 媛 뚮━嫄곕굹 focus 瑜껋쓣 TEMP_POS 덈뒗 媛믪쓣 異쒕젰
void FormInfo::on_pos_x_editingFinished()       {setTextTrim(ui->pos_x, &TEMP_POS.x, 1, "");}
void FormInfo::on_pos_y_editingFinished()       {setTextTrim(ui->pos_y, &TEMP_POS.y, 1, "");}
void FormInfo::on_pos_z_editingFinished()       {setTextTrim(ui->pos_z, &TEMP_POS.z, 1, "");}
void FormInfo::on_ang_tilt_editingFinished()    {setTextTrim(ui->ang_tilt, &TEMP_POS.tilt, 1, "");}
void FormInfo::on_ang_rotation_editingFinished(){setTextTrim(ui->ang_rotation, &TEMP_POS.rotation, 1, "");}
void FormInfo::on_zoom_editingFinished()        {setTextTrim(ui->zoom, &TEMP_POS.zoom, 100, "%");}

//enter 瑜뚮TEMP_POS 덈뒗 媛믪쓣 POS ｊ퀬 List 瑜留뚮벉
void FormInfo::on_pos_x_returnPressed()         {inputPos();}
void FormInfo::on_pos_y_returnPressed()         {inputPos();}
void FormInfo::on_pos_z_returnPressed()         {inputPos();}
void FormInfo::on_ang_tilt_returnPressed()      {inputPos();}
void FormInfo::on_ang_rotation_returnPressed()  {inputPos();}
void FormInfo::on_zoom_returnPressed()          {inputPos();}


void FormInfo::on_listWidget_currentRowChanged(int currentRow)
{
    if(isDeleting == false)
    {
        current_item = currentRow;
        sendStatusBarMsg(makePosText());
    }
}

void FormInfo::on_pushButton_go_clicked()
{
    if(current_item != -1)
    {
        int i = ui->listWidget->currentItem()->data(Qt::UserRole).toInt();
        *POS = POS_STACK[i].position;
        emit signalDirectlyInputPos();
        qDebug() << "GO  : " << ui->listWidget->currentItem()->text() << "data : " << ui->listWidget->currentItem()->data(Qt::UserRole).toInt();
    }
}

void FormInfo::on_pushButton_del_clicked()
{
    isDeleting = true;
    if(current_item != -1)
    {
//        qDebug() << "    all item";
//        for (int i = 0; i < ui->listWidget->count(); i++)
//        {
//            qDebug() << "a : " << i << "item name : "<< ui->listWidget->item(i)->text() << "number" << ui->listWidget->item(i)->data(Qt::UserRole).toInt() ;
//        }
        make_status_bar_msg = false;        //젣 怨쇱젙먯꽌 msg 蹂대궡吏 딅룄濡諛⑹ : segment fault error 諛쒖깮

        for (auto item : selected_item)
        {
            int i = ui->listWidget->row(item);
            ui->listWidget->takeItem(i);
        }

        make_status_bar_msg = true;         //msg 蹂대궡寃먮났
        isDeleting = false;
        QString msg = "Deleted Items";
        sendStatusBarMsg(msg);                 //msg 異쒕젰
    }
}

void FormInfo::on_listWidget_itemChanged(QListWidgetItem *item)
{
    if(isDeleting == false)
    {
        POS_STACK[item->data(Qt::UserRole).toInt()].name = item->text().toStdString();
        sendStatusBarMsg(makePosText());
    }
}


void FormInfo::on_listWidget_itemSelectionChanged()
{
    if(isDeleting == false)
    {
        selected_item = ui->listWidget->selectedItems();
        int sel_item = selected_item.size();

        if(sel_item > 1)
        {
            sendStatusBarMsg("selected " + QString::number(sel_item) + " items");
        }
    }
}
