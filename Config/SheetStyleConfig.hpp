#ifndef SHEETSTYLECONFIG_HPP
#define SHEETSTYLECONFIG_HPP

#include <QString>

namespace SheetStyle
{
    /*! ******************************************Genneral Sheet Style*******************************************/
    /*系统页面触摸滚动区域样式表*/
    static const QString QSCROLLAREA          =  "border:none;";

    /*系统页面控件QLineEdit样式表*/
    const QString QLINEEDIT                   =  "border-style:solid;\
                                                        border-radius:5px;\
                                                        border-width:1px;\
                                                        border-color:#e4e4e4;\
                                                        background-color:white;\
                                                        font: 16px;";
    /*系统页面控件QPushButton样式表*/
    const QString QPUSHBUTTON                 =  "color:white;\
                                                        background-color: #0090ff;\
                                                        border-radius: 3px;\
                                                        font:16px;";
    /*系统页面控件底部QToolButton样式表color:black*/
    const QString TOOLBUTTON_GREY_13          =  "background-color: white;\
                                                        border-width: 0px;\
                                                        border-radius: 3px;\
                                                        color: #9e9e9e;\
                                                        font: 13px;";
    /*系统页面控件底部QToolButton样式表color:#0090ff*/
    const QString TOOLBUTTON_BLUE_13          =  "background-color: white;\
                                                        border-width: 0px;\
                                                        border-radius: 3px;\
                                                        color: #0090ff;\
                                                        font: 13px;";
    /*系统页面控件底部QToolButton样式表color:black*/
    const QString TOOLBUTTON_GREY_15          =  "background-color: white;\
                                                        border-width: 0px;\
                                                        border-radius: 3px;\
                                                        color: #9e9e9e;\
                                                        font: 15px;";
    /*系统页面控件底部QToolButton样式表color:#0090ff*/
    const QString TOOLBUTTON_BLUE_15          =  "background-color: white;\
                                                        border-width: 0px;\
                                                        border-radius: 3px;\
                                                        color: #0090ff;\
                                                        font: 15px;";
    /*导航栏按钮样式表*/
    const QString NAVIGATION_BUTTON           =  "background-color: #0090ff;\
                                                        border:none;\
                                                        color:white;\
                                                        font:bold 16px;";
    /*导航栏标签样式表*/
    const QString NEVIGATION_LABEL            =  "background-color: #0090ff;\
                                                        color:white;\
                                                        border: none;\
                                                        font:bold 20px;";
    /*页面主体部分样式表*/
    const QString TOPBAR                      =  "background-color: #0090ff;";

    /*页面主体部分样式表*/
    const QString BODY                        =  "background-color: #ececec;\
                                                        border:none;";
    /*页面底部任务栏样式表*/
    const QString TASKBAR                     =  "background-color: white;\
                                                        border-style:solid;\
                                                        border-width: 0px;\
                                                        border-top-width:1px;\
                                                        border-color:#e4e4e4;";
    /*系统页面搜索按钮样式表*/
    const QString SEARCH_BUTTON               =  "border-width:1px;\
                                                        background-color: white;\
                                                        border-style:solid;\
                                                        border-radius:5px;\
                                                        border-color:#e4e4e4;\
                                                        color:#8c8c8c;\
                                                        font: 15px;";
    /*系统页面搜索栏输入样式表*/
    const QString SEARCHBAR_LINEEDIT          =  "border-style:solid;\
                                                        border-width:1px;\
                                                        border-left-width:0px;\
                                                        border-right-width:0px;\
                                                        border-color:#e4e4e4;\
                                                        background-color:white;\
                                                        font: 16px;";
    /*系统页面搜索栏按钮样式表*/
    const QString SEARCHBAR_BUTTON            =  "border-width:1px;\
                                                        background-color: white;\
                                                        border-style:solid;\
                                                        border-left-width:0px;\
                                                        border-right-width:0px;\
                                                        border-top-left-radius:0px;\
                                                        border-bottom-left-radius:0px;\
                                                        border-color:#e4e4e4;\
                                                        color:#8c8c8c;\
                                                        font: 15px;";
    /*系统页面分组按钮顶部边线样式表*/
    const QString GROUPBUTTON_TOPBORDER       =  "font: bold 14px;\
                                                        background-color: white;\
                                                        border-style:solid;\
                                                        border-width:1px;\
                                                        border-left-width:0px;\
                                                        border-right-width:0px;\
                                                        border-bottom-width:0px;\
                                                        border-color:#e4e4e4;";
    /*系统页面分组按钮底部边线样式表*/
    const QString GROUPBUTTON_BOTTOMBORDER    =  "font: bold 15px;\
                                                        background-color: white;\
                                                        border-style:solid;\
                                                        border-width:1px;\
                                                        border-left-width:0px;\
                                                        border-right-width:0px;\
                                                        border-top-width:0px;\
                                                        border-color:#e4e4e4;";
    /*系统页面分组按钮顶部和底部边线样式表*/
    const QString GROUPBUTTON_UPANDDOWNBORDER =  "font: bold 15px;\
                                                        background-color: white;\
                                                        border-style:solid;\
                                                        border-width:1px;\
                                                        border-left-width:0px;\
                                                        border-right-width:0px;\
                                                        border-color:#e4e4e4;";
    /*系统页面分组按钮无边线样式表*/
    const QString GROUPBUTTON_NOBORDER        =  "border:none;\
                                                        font: bold 15px;\
                                                        background-color: white;";
    /*系统页面控件列表样式表*/
    const QString LISTWIDGET                  =  "background-color:white;\
                                                        border:none;";
    /*页面提示信息样式表设置*/
    const QString HINTMSGBOX_LABEL            =  "border-style:solid;\
                                                        border-top-left-radius:5px;\
                                                        border-top-right-radius:5px;\
                                                        border-width: 0px;\
                                                        border-bottom-width:1px;\
                                                        font:bold 18px;\
                                                        background-color:rgba(255,255,255,230);";
    /*页面提示信息确认样式表设置*/
    const QString HINTMSGBOX_SUBMITBUTTON     =  "border-style:solid;\
                                                        border-bottom-left-radius:5px;\
                                                        border-bottom-right-radius:5px;\
                                                        border-width: 0px;\
                                                        color:#0090ff;\
                                                        font:bold 18px;\
                                                        background-color:rgba(255,255,255,230);";
    /*页面提示信息确认样式表设置*/
    const QString HINTMSGBOX_CANCELBUTTON     =  "border-style:solid;\
                                                        border-bottom-left-radius:5px;\
                                                        border-width: 0px;\
                                                        color:#0090ff;\
                                                        font:bold 18px;\
                                                        background-color:rgba(255,255,255,230);";
    /*页面提示信息确认样式表设置*/
    const QString HINTMSGBOX_BUTTONRIGHTRADIUS=  "border-style:solid;\
                                                        border-bottom-right-radius:5px;\
                                                        border-width: 0px;\
                                                        border-color:#e4e4e4;\
                                                        color:#0090ff;\
                                                        font:bold 18px;\
                                                        border-left-width:1px;\
                                                        background-color:rgba(255,255,255,230);";

    /*! ********************************************Home Sheet Style*********************************************/
    /*"首页"页面图片显示栏样式表*/
    const QString HOMEPHOTO_LABEL             =  "border-width: 0px;\
                                                        border-color:#e4e4e4;\
                                                        background-color:white;\
                                                        font: 24px;";
    /*"首页"页面项目标签样式表*/
    const QString HOMEPROJECT_LABLE           =  "color:#b3b3b3;\
                                                        border:none;\
                                                        border-color: #e4e4e4;\
                                                        font:bold 16px;";
    /*"首页"页面项目按钮样式表*/
    const QString HOMEPROJECT_TOOLBUTTON      =  "border-style:solid;\
                                                        border-width: 0px;\
                                                        border-bottom-width: 1px;\
                                                        border-color: #e4e4e4;\
                                                        background-color:white;\
                                                        font:18px;";

    /*! ******************************************Contacts Sheet Style*******************************************/
    /*好友分组标签样式表*/
    const QString BUDDY_GROUPING_LABEL        =  "color:#b3b3b3;\
                                                        font:bold 15px;";
    /*创建群输入控件QlineEdit样式表*/
    const QString CREATECLUSTER_QLINEEDIT     =  "border-style:solid;\
                                                        border:none;\
                                                        background-color:white;\
                                                        font: 16px;";

    const QString CREATECLUSTER_QTEXTEDIT     =  "border-style:solid;\
                                                        border-width:1px;\
                                                        border-left-width:0px;\
                                                        border-right-width:0px;\
                                                        padding-left:12px;\
                                                        padding:12px;\
                                                        border-color:#e4e4e4;\
                                                        background-color:white;\
                                                        font: 16px;";

    /*! ******************************************Device Sheet Style*******************************************/
    /*"设备"页面设备分组标签*/
    const QString QLABEL                      =  "color:#b3b3b3;\
                                                        border-style:solid;\
                                                        border-width: 0px;\
                                                        border-top-width:1px;\
                                                        border-color: #e4e4e4;\
                                                        font:bold 15px;";
    /*"创建设备群成功"页面确认按钮样式表*/
    const QString CREATECLUSTER_SUCCESS_BUTTON = "color:white;\
                                                        background-color: #0090ff;\
                                                        border-radius: 3px;\
                                                        font:16px;";
    /*"设备群通知"页面确认按钮样式表*/
    const QString CHECKBUTTON_BOTTOM_BORDER    = "background-color: white;\
                                                        border-style:solid;\
                                                        border-width: 0px;\
                                                        border-bottom-width:1px;\
                                                        border-color:#e4e4e4;";

    /*! *************************************Login && Register Sheet Style***************************************/
    /*"登录"页面登录名称样式表*/
    const QString LOGINNAME_LABEL              = "border-style:solid;\
                                                        border-width:0px;\
                                                        background-color:white;\
                                                        font:16px;";
    /*"登录"页面登录密码样式表*/
    const QString LOGINPWD_LABEL               = "border-style:solid;\
                                                        border-width:0px;\
                                                        background-color:white;\
                                                        font:16px;";
    /*"登录"页面用户名输入样式表*/
    const QString LOGINNAME_LINEEDIT           = "border-style:solid;\
                                                        border-width:0px;\
                                                        border-bottom-width:1px;\
                                                        border-color:#e4e4e4;\
                                                        background-color:white;\
                                                        font: 15px;";
    /*"登录"页面密码输入样式表*/
    const QString LOGIPWD_LINEEDIT             = "border-style:solid;\
                                                        border-width:0px;\
                                                        border-top-width:0px;\
                                                        border-color:#e4e4e4;\
                                                        background-color:white;\
                                                        font: 15px;";
    /*"登录"页面登录按钮样式表*/
    const QString LOGIN_BUTTON                 = "color:white;\
                                                        background-color: #0090ff;\
                                                        border-radius: 3px;\
                                                        font:16px";
    /*注册按钮按下动作样式表*/
    const QString REGISTER_BUTTON              = "color:white;\
                                                        background-color: #0090ff;\
                                                        border-radius: 3px;";

    /*注册验证页面输入验证码框样式表*/
    const QString REGISTER_VALIDATION_LINEEDIT = "color: black;\
                                                        background-color: white;\
                                                        border-radius: 3px;\
                                                        font: bold 18px;";
    /*忘记密码验证页面输入验证码框样式表*/
    const QString FORGETPWD_VALIDATION_LINEEDIT= "color: black;\
                                                        background-color: white;\
                                                        border-radius: 3px;\
                                                        font: bold 18px;";
}
#endif // SHEETSTYLECONFIG_HPP
