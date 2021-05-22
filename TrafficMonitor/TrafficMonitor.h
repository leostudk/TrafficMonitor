﻿
// TrafficMonitor.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
    #error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "Common.h"
#include "IniHelper.h"
#include "WinVersionHelper.h"
#include "SimpleXML.h"
#include "TaskbarDefaultStyle.h"
#include <map>
#include "OpenHardwareMonitor/OpenHardwareMonitorApi.h"

// CTrafficMonitorApp:
// 有关此类的实现，请参阅 TrafficMonitor.cpp
//


class CTrafficMonitorApp : public CWinApp
{
public:
    //各种路径
    static CTrafficMonitorApp* self;
    wstring m_module_dir;       //程序exe文件的目录
    wstring m_appdata_dir;
    wstring m_module_path;      //程序exe文件的路径
    wstring m_module_path_reg;  //用于作为写入注册表开机自项的exe文件的路径（如果路径中有空格，加上引号）
    wstring m_config_path;
    wstring m_history_traffic_path;
    wstring m_log_path;
    wstring m_skin_path;
    wstring m_system_dir;
    wstring m_config_dir;

    //以下数据定义为App类中的公共成员，以便于在主对话框和任务栏窗口中都能访问
    unsigned __int64 m_in_speed{};      //下载速度
    unsigned __int64 m_out_speed{};     //上传速度
    int m_cpu_usage{ -1 };      //CPU利用率
    int m_memory_usage{ -1 };   //内存利用率
    int m_used_memory{};    //可用物理内存（单位为KB）
    int m_total_memory{};   //物理内存总量（单位为KB）
    float m_cpu_temperature{ -1 };  //CPU温度
    float m_gpu_temperature{ -1 };  //显卡温度
    float m_hdd_temperature{ -1 };  //硬盘温度
    float m_main_board_temperature{ -1 };    //主板温度
    int m_gpu_usage{ -1 };      //显卡利用率

    unsigned __int64 m_today_up_traffic{};  //今天已使用的上传流量
    unsigned __int64 m_today_down_traffic{};    //今天已使用的下载流量

    bool m_cannot_save_config_warning{ true };  //指示是否会在无法保存设置时弹出提示框
    bool m_cannot_save_global_config_warning{ true };   //指示是否会在无法保存设置时弹出提示框

    bool m_module_dir_writable{ true };         //指示程序所在目录是否可写

    //选项设置数据
    MainWndSettingData m_main_wnd_data;
    TaskBarSettingData m_taskbar_data;
    GeneralSettingData m_general_data;
    //其他设置数据
    MainConfigData m_cfg_data;
    int m_notify_interval;      //弹出通知消息的时间间隔
    bool m_debug_log{};
    bool m_taksbar_transparent_color_enable{};
    bool m_last_light_mode{};
    bool m_show_mouse_panetrate_tip{};  //是否显示开启“鼠标穿透”时的提示消息。
    bool m_show_dot_net_notinstalled_tip{};

    //bool m_is_windows10_fall_creator;
    CWinVersionHelper m_win_version;        //当前Windows的版本

    HICON m_notify_icons[MAX_NOTIFY_ICON];

    CTaskbarDefaultStyle m_taskbar_default_style;

    HWND m_option_dlg{};        //选项设置对话框的句柄


    CMenu m_main_menu;
    CMenu m_taskbar_menu;

#ifndef WITHOUT_TEMPERATURE
    //OpenHardwareMonitor 接口的指针
    std::shared_ptr<OpenHardwareMonitorApi::IOpenHardwareMonitor> m_pMonitor{};
#endif // !WITHOUT_TEMPERATURE


public:
    CTrafficMonitorApp();

    void LoadConfig();
    void SaveConfig();

    void LoadGlobalConfig();
    void SaveGlobalConfig();

    int DPI(int pixel);
    void DPI(CRect& rect);
    void DPIFromWindow(CWnd* pWnd);
    int GetDpi() const { return m_dpi; }
    void SetDPI(int dpi) { m_dpi = dpi; }

    void CheckUpdate(bool message);     //检查更新，如果message为true，则在检查时弹出提示信息
    void CheckUpdateInThread(bool message); //在后台线程中检查更新
    //启动时检查更新线程函数
    static UINT CheckUpdateThreadFunc(LPVOID lpParam);
    static UINT InitOpenHardwareMonitorLibThreadFunc(LPVOID lpParam);

    void SetAutoRun(bool auto_run);
    bool GetAutoRun();

    void SetAutoRunByRegistry(bool auto_run);       //通过注册表实现开机自启动
    void SetAutoRunByTaskScheduler(bool auto_run);  //通过任务计划实现开机自启动

    //获取系统信息文本
    CString GetSystemInfoString();

    void InitMenuResourse();

    //获取一个图标资源，如果资源还未加载，会自动加载。
    //由于本函数中使用了CTrafficMonitorApp::DPI函数，因此本函数必须确保在CTrafficMonitorApp::DPIFromWindow之后调用
    HICON GetMenuIcon(UINT id);

    void AutoSelectNotifyIcon();

    bool IsCheckingForUpdate() const { return m_checking_update; }      //是否正在检查更新

    void InitOpenHardwareLibInThread();     //开启一个后台线程初始化OpenHardwareMonitor
    void UpdateOpenHardwareMonitorEnableState();    //更新硬件监控的启用/禁用状态

private:
    //int m_no_multistart_warning_time{};       //用于设置在开机后多长时间内不弹出“已经有一个程序正在运行”的警告提示
    bool m_no_multistart_warning{};         //如果为false，则永远都不会弹出“已经有一个程序正在运行”的警告提示
    bool m_exit_when_start_by_restart_manager{ true };      //如果程序被Windows重启管理器重新启动，则退出程序
    int m_dpi{ 96 };

    bool m_checking_update{ false };        //是否正在检查更新
    CCriticalSection m_minitor_lib_init_critical;   //用于对OpenHardwareMonitor初始化进行线程同步的临界区对象

    std::map<UINT, HICON> m_menu_icons;      //菜单图标资源。key是图标资源的ID，vlaue是图标的句柄

// 重写
public:
    virtual BOOL InitInstance();

// 实现

    DECLARE_MESSAGE_MAP()
    afx_msg void OnHelp();
    afx_msg void OnFrequentyAskedQuestions();
    afx_msg void OnUpdateLog();
};

extern CTrafficMonitorApp theApp;
