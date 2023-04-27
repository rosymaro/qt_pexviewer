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

    //숫자 제약 설정
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
    //edit 값 초기화
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
    TEMP_POS = pos;         //TEMP_POS 역할 : enter 를 누르기 전까지 값을 갖고 있는 저장소
}

void FormInfo::changePos()
{
    if (isEndedEdit)
    {
        TEMP_POS = *POS;
        setTextPosTotal(POS);
    }
}

void FormInfo::addItemToPosList(POS_SET *position_item)    //POS_STACK 에 position 넣기
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

void FormInfo::inputPos()       //직접 입력한 값을 POS 구조체에 넣음
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

//입력이 들어왔을 때
void FormInfo::on_pos_x_textEdited(const QString &arg1)         {convertTextToValue(&TEMP_POS.x,arg1);}
void FormInfo::on_pos_y_textEdited(const QString &arg1)         {convertTextToValue(&TEMP_POS.y,arg1);}
void FormInfo::on_pos_z_textEdited(const QString &arg1)         {convertTextToValue(&TEMP_POS.z,arg1);}
void FormInfo::on_ang_tilt_textEdited(const QString &arg1)      {convertTextToValue(&TEMP_POS.tilt,arg1);}
void FormInfo::on_ang_rotation_textEdited(const QString &arg1)  {convertTextToValue(&TEMP_POS.rotation,arg1);}
void FormInfo::on_zoom_textEdited(const QString &arg1)
{   //zoom 은 100% 일 경우 1 값을 넣어야 하기 때문에 따로 처리
    double double_arg = arg1.toDouble();
    double_arg = double_arg/100;
    inputValueToTemp(&TEMP_POS.zoom,double_arg);
}

//enter 가 눌리거나 focus 를 잃을 때 TEMP_POS 에 있는 값을 출력함
void FormInfo::on_pos_x_editingFinished()       {setTextTrim(ui->pos_x, &TEMP_POS.x, 1, "");}
void FormInfo::on_pos_y_editingFinished()       {setTextTrim(ui->pos_y, &TEMP_POS.y, 1, "");}
void FormInfo::on_pos_z_editingFinished()       {setTextTrim(ui->pos_z, &TEMP_POS.z, 1, "");}
void FormInfo::on_ang_tilt_editingFinished()    {setTextTrim(ui->ang_tilt, &TEMP_POS.tilt, 1, "");}
void FormInfo::on_ang_rotation_editingFinished(){setTextTrim(ui->ang_rotation, &TEMP_POS.rotation, 1, "");}
void FormInfo::on_zoom_editingFinished()        {setTextTrim(ui->zoom, &TEMP_POS.zoom, 100, "%");}

//enter 를 눌렀을 때 TEMP_POS 에 있는 값을 POS 에 넣고 List 를 만듬
void FormInfo::on_pos_x_returnPressed()         {inputPos();}
void FormInfo::on_pos_y_returnPressed()         {inputPos();}
void FormInfo::on_pos_z_returnPressed()         {inputPos();}
void FormInfo::on_ang_tilt_returnPressed()      {inputPos();}
void FormInfo::on_ang_rotation_returnPressed()  {inputPos();}
void FormInfo::on_zoom_returnPressed()          {inputPos();}


void FormInfo::on_listWidget_currentRowChanged(int currentRow)
{
    current_item = currentRow;
    sendStatusBarMsg(makePosText());
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
    if(current_item != -1)
    {
        make_status_bar_msg = false;        //삭제 과정에서 msg 보내지 않도록 방지 : segment fault error 발생
        for (auto item : selected_item)
        {
            ui->listWidget->takeItem(ui->listWidget->row(item));
        }
        make_status_bar_msg = true;         //msg 보내는 것 원복
        current_item = -1;                  //msg 에 "" 넣음
        sendStatusBarMsg(makePosText());                 //msg 출력
    }
}

void FormInfo::on_listWidget_itemChanged(QListWidgetItem *item)
{
    if(current_item != -1)
    {        
        POS_STACK[item->data(Qt::UserRole).toInt()].name = item->text().toStdString();
        sendStatusBarMsg(makePosText());
    }
}


void FormInfo::on_listWidget_itemSelectionChanged()
{
    selected_item = ui->listWidget->selectedItems();
    int sel_item = selected_item.size();
    if(sel_item > 1)
    {
        sendStatusBarMsg("selected " + QString::number(sel_item) + " items");
    }
}
