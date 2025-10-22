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


#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <functional>   // std::function

#include <QAbstractSocket>
#include <QFile>
#include <QHostAddress>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>


class clConnection : public QObject
{
    Q_OBJECT
public:
    explicit clConnection(QObject *parent = 0);
    ~clConnection();

    std::tuple<bool, QString> Connect(QHostAddress const& paHost,unsigned short paPort);
    void connectSignalSlot(const bool& paServerMode);
    void Disconnect();
    std::tuple<bool, QString> Listen(QHostAddress const& paHost, unsigned short paPort);
    QByteArray Read();
    void Write(QByteArray const& paBytes);
    void Write(const char* paBytes, int64_t paLenght);


    std::function<void()> fnReadyRead;
    std::function<void()> fnStartSeningdData;
    std::function<void(bool)> fnStopSendingData;
    std::function<void(QString, bool)> fnLogData;

private slots:

    void bytesWritten(qint64 bytes);
    void connected();
    void disconnected();
    void newConnection();
    void readyRead();

private:

    std::unique_ptr<QTcpServer> m_TcpServer;
    std::unique_ptr<QTcpSocket> m_Socket;
};

#endif // CONNECTTOCLIENT_H
