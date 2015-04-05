#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QLineEdit>
#include <QDebug>
#include <QGridLayout>
#include <QString>
#include <QPushButton>
#include <QLineEdit>

#include "HintMsgPage.h"
#include "ScreenPara.h"
#include "SheetStyleConfig.hpp"

HintMsgPage::HintMsgPage(QWidget* parent, ButtonType type) : QWidget(parent), btnType(type)
{
    //置顶 透明显示
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->initializeWidget();
}

void HintMsgPage::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    /*Paint事件中使用Clear模式绘图,局部透明显示*/
    QPainter pointer(this);
    pointer.fillRect(this->rect(), QColor(0x2f,0x2f,0x2f,0x1f));
}

void HintMsgPage::initializeWidget()
{
    isJumpPage = false;
    hintWidth = ScreenPara::getInstance()->screenWidth();
    hintHeight = ScreenPara::getInstance()->screenHeight();
    this->setFixedWidth(hintWidth);
    this->setFixedHeight(hintHeight);

    btnTitle = new QPushButton(this);
    btnTitle->setFixedWidth(hintWidth*0.8);
    btnTitle->setFixedHeight(hintHeight*0.09);
    btnTitle->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    btnTitle->setStyleSheet(SheetStyle::HINTMSGBOX_LABEL);

    btnSubmit = new QPushButton(tr("确定"), this);
    this->setPushButton(btnSubmit);
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(on_btnSubmit_Clicked()));

    btnCancel = new QPushButton(tr("取消"), this);
    this->setPushButton(btnCancel);
    if(btnType == Submit){
       btnSubmit->setStyleSheet(SheetStyle::HINTMSGBOX_SUBMITBUTTON);
       btnCancel->hide();
    }else if(btnType == (SubmitAndCancel)){
        btnSubmit->setStyleSheet(SheetStyle::HINTMSGBOX_BUTTONRIGHTRADIUS);
        btnCancel->setStyleSheet(SheetStyle::HINTMSGBOX_CANCELBUTTON);
        btnCancel->show();
    }
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(on_btnCancel_Clicked()));

    QHBoxLayout* hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(btnCancel);
    hboxLayout->addWidget(btnSubmit);
    hboxLayout->setSpacing(0);
    hboxLayout->setMargin(0);

    QVBoxLayout* vbHint = new QVBoxLayout(this);
    vbHint->addWidget(btnTitle);
    vbHint->addLayout(hboxLayout);
    vbHint->setSpacing(0);
    vbHint->setAlignment(Qt::AlignCenter);
    this->setLayout(vbHint);
}

void HintMsgPage::setPushButton(QPushButton *btn)
{
    btn->setFixedHeight(hintHeight*0.09);
    btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
}

void HintMsgPage::setText(const QString &text)
{
    btnTitle->setText(text);
}

void HintMsgPage::setStatus(bool isJump)
{
    isJumpPage = isJump;
}

void HintMsgPage::on_btnSubmit_Clicked()
{
    emit submitClicked(isJumpPage);
}

void HintMsgPage::on_btnCancel_Clicked()
{
    emit cancelClicked();
}

HintMsgPage::~HintMsgPage(){}
