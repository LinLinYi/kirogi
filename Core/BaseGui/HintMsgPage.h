#ifndef HINTMSGPAGE_H
#define HINTMSGPAGE_H

class QString;
class QPushButton;
class QLineEdit;

#include <QWidget>

class HintMsgPage: public QWidget
{
    Q_OBJECT
public:
    enum ButtonType{
        Submit = 0x010,
        Cancel = 0x100,
        SubmitAndCancel = 0x110,
    };

    HintMsgPage(QWidget* parent = 0, ButtonType type = Submit);
     ~HintMsgPage();

public:
    void setText(const QString& text);
    void setStatus(bool isJump);

private:
    unsigned int hintWidth;
    unsigned int hintHeight;

protected:
    void paintEvent(QPaintEvent*);

private:
    QPushButton* btnTitle;
    QPushButton* btnSubmit;
    QPushButton* btnCancel;
    ButtonType btnType;
    bool isJumpPage;

private:
    void initializeWidget();
    void setPushButton(QPushButton* btn);

signals:
    void submitClicked(bool isJump);
    void cancelClicked();

protected slots:
    void on_btnSubmit_Clicked();
    void on_btnCancel_Clicked();
};

#endif // HINTMSGPAGE_H
