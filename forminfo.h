#ifndef FORMINFO_H
#define FORMINFO_H

#include <QDialog>

namespace Ui {
class FormInfo;
}

class FormInfo : public QDialog
{
    Q_OBJECT

public:
    explicit FormInfo(QWidget *parent = nullptr);
    ~FormInfo();

public slots:
    void slotInfoText(QString funcName, float value);
    void moveRender(float x, float y);

private:
    Ui::FormInfo *ui;
    float pointX = 0;
    float pointY = 0;
    float pointZ = 0;
    float infoTilt = 90;
    float infoRot = 0;
    float infoZoom = 1; //Zoom é½‘å ¦èµ´è‡§›æ£Ž ï¿½GDS size æ¯³è‡§¢ƒï¿æ©ƒçˆ¼ ³æ

    float initRot = 0;
    float initTilt = 90;
    float initPointX = 0;
    float initPointY = 0;
    float initPointZ = 0;

    float x = 0;
    float y = 0;
};

#endif // FORMINFO_H
