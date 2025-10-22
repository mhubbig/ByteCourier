/*
 * Copyright [2025] [Mojtaba Biglar]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef CLMAINWINDOW_H
#define CLMAINWINDOW_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QTabWidget>
#include <QTextEdit>
#include <QMainWindow>

#include "DataSourceStructure.h"

class clDataGeneratorThread;

class clMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit clMainWindow(QWidget *parent = nullptr);

    void disconnedted();
    void LogText(QString const& paMessage, QString const& paForegroundColor);
    void setClientEventLoop( clDataGeneratorThread* const paClientEventLoop);

private slots:
    void connectToServer();
    void openTextFile();
    void radioButtonsTcpClient();
    void radioButtonsTcpServer();
    void dataSourceComboBoxChanged(const QString&);
    void intervalCheckComboBoxChanged(const QString&);
    void intervalCheckBox(bool);

private:

    void fillDataSourceText();

    Q_INVOKABLE void invokeAbleLogText(QString const& paMessage, QString const& paForegroundColor);
    Q_INVOKABLE void invokabeDisconnedted();

    QFrame*  makeFrame(QWidget* paParent);
    QWidget* buildOperationWidget(QWidget *paParent);
    QWidget* buildLogWidget(QWidget *paParent);
    QWidget* buildDataSourceWidget(QWidget* paParent);

    clDataGeneratorThread* m_ClientEventLoop;

    //************* ConfigTab
    /* Tcp/Ip Client */
    QRadioButton* m_TcpIpRadioCleint;
    QTextEdit* m_IpAdressTcpClient;
    QTextEdit* m_PortNumberClient;

    /* Tcp/Ip Server */
    QRadioButton* m_TcpIpRadioServer;
    QTextEdit* m_IpAdressTcpServer;
    QTextEdit* m_PortNumberServer;

    //************** DataSourceTab

    QComboBox* m_DataSourceComboBox;
    QLabel* m_DataSourceLabel;

    QFrame* m_TextFrame;
    QTextEdit* m_DataText;
    QPushButton* m_OpenFileButton;

    QCheckBox* m_IntervalCheckBox;
    QComboBox* m_IntervalCheckComboBox;
    QDoubleSpinBox* m_FixIntervalSpinBox;
    QDoubleSpinBox* m_RandomIntervalSpinBox;

    QCheckBox* m_SaveTheLog;


    //************** LogTab
    QTextEdit* m_LogText;
    FILE*  m_LogFile;


    QGridLayout* gridLayout;
    QPushButton* m_ConnectButton;
    QTabWidget* m_MainTabWidget;
    QWidget* m_CentralWidget;

    bool m_ConnectedToServer;
    bool m_FileIsOpen;
    stDataSource m_CurrentDataSource;
    QString m_PathToSourceFile;
};

#endif // CLMAINWINDOW_H
