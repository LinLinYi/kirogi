#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QLineEdit>
#include <QDebug>
#include <QGridLayout>

#include "MD5.h"
#include "Keys.h"
#include "ScreenPara.h"
#include "UserProtocol.hpp"
#include "UserVerifyPasswordPage.h"

UserVerifyPasswordPage::UserVerifyPasswordPage(QWidget* parent, quint64 accountID)
    : QWidget(parent)
    , curAccountID(accountID)
    , userPwdModifyPage(NULL)
{
    this->setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0xff));
    setPalette(pal);

    //置顶 透明显示
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->initializeWidget();
}

void UserVerifyPasswordPage::paintEvent(QPaintEvent *)
{
    /*Paint事件中使用Clear模式绘图,局部透明显示*/
    QPainter p(this);
    p.fillRect(rect(), QColor(0x2f,0x2f,0x2f,0x2f));
}

void UserVerifyPasswordPage::initializeWidget()
{
    //页面参数
    screenWidth = ScreenPara::getInstance()->screenWidth();
    screenHeight = ScreenPara::getInstance()->screenHeight();
    this->setFixedWidth(screenWidth);
    this->setFixedHeight(screenHeight);

    lblTitle = new QLabel(this);
    lblTitle->setText(tr("验证原密码"));
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setFixedHeight(this->screenHeight*0.06);
    lblTitle->setStyleSheet("color:black; font:bold 17px; background-color:white;\
                             border-style:solid; border-width: 0px; border-Top-right-radius:3px;\
                             border-Top-Left-radius:3px;");

    lblHintInfo = new QLabel(this);
    lblHintInfo->adjustSize();
    lblHintInfo->setWordWrap(true);
    lblHintInfo->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    lblHintInfo->setText(tr("为保障你的数据安全,请填写原密码."));
    lblHintInfo->setStyleSheet("color:black; font:14px; background-color:white;");

    editCurAccountPwd = new QLineEdit(this);
    editCurAccountPwd->setPlaceholderText(QString("密码"));
    editCurAccountPwd->setEchoMode(QLineEdit::Password);
    editCurAccountPwd->setFixedHeight(screenHeight*0.06);
    editCurAccountPwd->setStyleSheet("border-style:solid; border-width:1px; border-color:rgb(208, 208, 208);\
                                     border-radius:1px;");

    QVBoxLayout* vblHint = new QVBoxLayout;
    vblHint->addWidget(lblTitle);
    vblHint->addWidget(lblHintInfo);
    vblHint->addSpacing(screenWidth*0.02);
    vblHint->addWidget(editCurAccountPwd);
    vblHint->setMargin(screenWidth*0.02);
    vblHint->setSpacing(0);

    btnSubmit = new QPushButton(this);
    btnSubmit->setText(tr("确定"));
    btnSubmit->setFixedHeight(screenHeight*0.07);
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(on_btnSubmit_clicked()), Qt::QueuedConnection);
    btnSubmit->setStyleSheet("color:#0090ff; font:bold 17px; background-color:white;\
                             border-style:solid; border-width:1px; border-right-width:0px;\
                             border-bottom-width:0px;border-color:rgb(208, 208, 208);\
                             border-radius:0px;border-bottom-right-radius:3px;");

    btnCancel = new QPushButton(this);
    btnCancel->setText(QString("取消"));
    btnCancel->setFixedHeight(screenHeight*0.07);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(on_btnCancel_clicked()), Qt::QueuedConnection);
    btnCancel->setStyleSheet("color:#0090ff; font:17px; background-color:white;\
                             border-style:solid; border-width: 0px; border-top-width:1px;\
                             border-color:rgb(208, 208, 208);border-radius:0px;\
                             border-bottom-left-radius:3px;");

    QHBoxLayout* hblButton = new QHBoxLayout;
    hblButton->addWidget(btnCancel);
    hblButton->addWidget(btnSubmit);
    hblButton->setSpacing(0);
    hblButton->setMargin(0);

    QVBoxLayout* vblAccountSafe = new QVBoxLayout;
    vblAccountSafe->addLayout(vblHint);
    vblAccountSafe->addLayout(hblButton);
    vblAccountSafe->setMargin(0);
    vblAccountSafe->setSpacing(0);

    QFrame* accountFrame = new QFrame(this);
    accountFrame->setLayout(vblAccountSafe);
    accountFrame->setContentsMargins(0,0,0,0);
    accountFrame->setStyleSheet("background-color:white; border-style:solid;\
                                border-width: 0px;  border-radius:3px;");

    QVBoxLayout* vblTotalLayout = new QVBoxLayout(this);
    vblTotalLayout->addSpacing(screenHeight*0.16);
    vblTotalLayout->addWidget(accountFrame);
    vblTotalLayout->setMargin(screenWidth*0.1);
    vblTotalLayout->setAlignment(Qt::AlignCenter);
    vblTotalLayout->setSpacing(0);
    vblTotalLayout->addStretch(0);
    this->setLayout(vblTotalLayout);
}

void UserVerifyPasswordPage::fetchData(InputPacket &inpack)
{
    this->transferDataToAnotherPage(inpack, userPwdModifyPage);
}

void UserVerifyPasswordPage::displayConcretePage(BasePage *page)
{
    page->show();
    connect(page, SIGNAL(destroyed()), this, SLOT(on_deleteSubPages()));
    connect(page, SIGNAL(readyRead(quint16,char*,quint16)), this, SLOT(on_recvDataFromOtherPage(quint16,char*,quint16)));
}

void UserVerifyPasswordPage::transferDataToAnotherPage(InputPacket &inpack, BasePage *page)
{
    if(page == NULL) return;
    page->fetchData(inpack);
}

void UserVerifyPasswordPage::on_btnSubmit_clicked()
{
    QString strPwd = editCurAccountPwd->text().trimmed();
    if(strPwd.isEmpty()){
        this->hide();
        this->deleteLater();
        return;
    }

    QByteArray bytePwd = strPwd.toLatin1();
    char* chPwd = bytePwd.data();
    if(!Keys::isMatchLoginPwd(chPwd, bytePwd.length())){
        QString strText = tr("密码错误, 请重新输入");
        lblHintInfo->setText(strText);
        return;
    }

    userPwdModifyPage = new UserAlterPasswordPage(this, curAccountID);
    this->displayConcretePage(userPwdModifyPage);
}

void UserVerifyPasswordPage::on_btnCancel_clicked()
{
    this->hide();
    this->deleteLater();
}

void UserVerifyPasswordPage::on_deleteSubPages()
{
    userPwdModifyPage = NULL;
}

void UserVerifyPasswordPage::on_recvDataFromOtherPage(quint16 cmdCode, char *data, quint16 dataLen)
{
    if(cmdCode == CommandCode::BACK_ACCOUNT_SAFE_PAGE){
        this->hide();
        this->deleteLater();
        return;
    }
    emit readyRead(cmdCode, data, dataLen);
}

UserVerifyPasswordPage::~UserVerifyPasswordPage()
{

}

