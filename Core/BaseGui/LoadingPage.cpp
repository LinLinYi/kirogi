#include <QLabel>
#include <QPainter>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

#include "QProgressIndicator.h"
#include "LoadingPage.h"

LoadingPage::LoadingPage(QWidget *parent) : BaseWidget(parent), timeMesc(0)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0xff));
    this->setPalette(pal);
    this->setAutoFillBackground(false);

    //置顶 透明显示(半透明)
    m_screenWidth = this->screenWidth();
    m_screenheight = this->screenHeight();
    this->initializeWidget();
}

void LoadingPage::initializeWidget()
{
    this->setFixedWidth(m_screenWidth);
    this->setFixedHeight(m_screenheight);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateHint()));

    proIndicator = new QProgressIndicator(this);
    proIndicator->startAnimation();
    proIndicator->setAnimationDelay(80);
    QVBoxLayout* vbProIndicator = new QVBoxLayout();
    vbProIndicator->addWidget(proIndicator);
    vbProIndicator->setAlignment(Qt::AlignCenter);

    loadingText = new QLabel(this);
    loadingText->setText(tr("请稍后..."));
    loadingText->setAlignment(Qt::AlignCenter);
    loadingText->setStyleSheet("color: white;background-color: transparent;");
    QVBoxLayout* vbLoadingText = new QVBoxLayout();
    vbLoadingText->addWidget(loadingText);
    vbLoadingText->setAlignment(Qt::AlignCenter);

    QVBoxLayout* vbLayout = new QVBoxLayout(this);
    vbLayout->addLayout(vbProIndicator);
    vbLayout->addLayout(vbLoadingText);
    vbLayout->setAlignment(Qt::AlignCenter);
    vbLayout->setSpacing(m_screenheight*0.01);

    QFrame* hintFrame = new QFrame(this);
    hintFrame->setLayout(vbLayout);
    hintFrame->setFixedWidth(m_screenWidth*0.3);
    hintFrame->setFixedHeight(m_screenWidth*0.3);
    hintFrame->setStyleSheet("border-radius:5px;color:#0090ff;font:bold 15px;background-color:rgba(96,96,96,230);");

    QVBoxLayout* vbTotalLayout = new QVBoxLayout();
    vbTotalLayout->addWidget(hintFrame);
    vbTotalLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(vbTotalLayout);
}

void LoadingPage::start(int mesc)
{
    if(mesc != 0)
        timeMesc = mesc;

    timer->start(timeMesc);
}

void LoadingPage::stop()
{
    if(timer == NULL) return;
    timer->stop();
}

void LoadingPage::setTimeOut(int mesc)
{
    timeMesc = mesc;
}

void LoadingPage::paintEvent(QPaintEvent *)
{
    /*Paint事件中使用Clear模式绘图,局部透明显示*/
    QPainter p(this);
    p.fillRect(rect(), QColor(0x2f,0x2f,0x2f,0x00));
}

void LoadingPage::updateHint()
{
    emit timeOut();
}

LoadingPage::~LoadingPage()
{

}
