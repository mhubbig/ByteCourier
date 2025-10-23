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


#ifndef DATAGENERATORTHREAD_H
#define DATAGENERATORTHREAD_H

#include "Connection.h"
#include "MainWindow.h"

#include <functional>   // std::bind

#include <QMutex>
#include <QTimer>
#include <QThread>
#include <QWaitCondition>

#include "DataSourceStructure.h"

class clDataGeneratorThread : public QThread
{
    Q_OBJECT
public:
    explicit clDataGeneratorThread(clMainWindow* const paMainWindow, QThread *parent = nullptr);
    ~clDataGeneratorThread();

    bool startConnection(const stDataSource& paDataSource);
    bool disConnect();

private slots:

    void writeResponse();

private:

    bool initialize();
    void logData(const QString& paMessage, bool paError);
    int  generateRandomInteger(const int paLowerLimit, const int paUpperLimit);
    void sendData();
    void stopSendingData(bool paStopSending);
    void Start();
    void readDataFromSocket();
    void reset();
    void run() override;

    Q_INVOKABLE  bool invokeableStartConnection(const stDataSource& paDataSource);
    Q_INVOKABLE  bool invokeableDeinitialize();

    clMainWindow* m_MainWindow;

    std::unique_ptr<clConnection> m_TcpDevice = nullptr;
    std::unique_ptr<stDataSource> m_DataSource = nullptr;

    bool m_Initialized;
    bool m_StopSending;
    bool m_ServerMode;
    QByteArray m_Out;
    QMutex m_ThreadMutex;
    QString m_PortNumber;
    QString m_IpAdressTcpClient;
    QTimer* m_SendingDataTimer = nullptr;
    QWaitCondition m_ThreadWaitCondition;
};

#endif // CLCLIENTEVENTLOOP_H
