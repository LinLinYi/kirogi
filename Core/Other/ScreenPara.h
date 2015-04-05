#ifndef SCREENPARA_H
#define SCREENPARA_H

#include <QObject>
#include <QString>

/*! \class ScreenPara
 *  \brief 获取屏幕参数的类
 */
class ScreenPara : public QObject
{
    Q_OBJECT
private:
    explicit ScreenPara(QObject *parent = 0);
    ~ScreenPara();

private:
    quint32 m_screenHeight;
    quint32 m_screenWidth;
    static ScreenPara* m_pInstance;

public:
    quint32 screenHeight();
    quint32 screenWidth();
    static bool is_iPhone();

public:
    /*! 单例模式*/
    static ScreenPara* getInstance()
    {
        if(m_pInstance == NULL)
            m_pInstance = new ScreenPara();

        return m_pInstance;
    }

    /*! 删除单例模式对象*/
    static void DeleteInstance()
    {
        if(m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

};

#endif // SCREENPARA_H
