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


#include "Connection.h"

clConnection::clConnection(QObject *parent) : QObject(parent)
{
}

clConnection::~clConnection()
{
}


std::tuple<bool, QString> clConnection::Connect(QHostAddress const& paHost,unsigned short paPort)
{
    m_Socket = std::make_unique<QTcpSocket>(this);
    connectSignalSlot(false);

    fnLogData("Connecting...", false);

    m_Socket->connectToHost(paHost,paPort);


    if(!(m_Socket->waitForConnected(5000)))
    {
        return {false, m_Socket->errorString()};
    }

    return { true, ""};
}

void clConnection::connectSignalSlot(const bool& paServerMode)
{
    if (!paServerMode)
        connect(m_Socket.get(), SIGNAL(connected()), this, SLOT(connected()));

    connect(m_Socket.get(), SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_Socket.get(), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_Socket.get(), SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
}

std::tuple<bool, QString> clConnection::Listen(QHostAddress const& paHost, unsigned short paPort)
{
    m_TcpServer = std::make_unique<QTcpServer>(this);

    connect(m_TcpServer.get(), SIGNAL(newConnection()), this, SLOT(newConnection()));

    fnLogData("Listenning...", false);

    if (!(m_TcpServer->listen(paHost, paPort)))
    {
        return std::make_pair(false, m_TcpServer->errorString());
    }
    
    return std::make_pair(true, "");
}

void clConnection::bytesWritten(qint64 bytes)
{
    //std::cout <<"We wrote: "<< bytes << std::endl;
}

void clConnection::connected()
{
    fnLogData("Connected...", false);
}

void clConnection::newConnection()
{
    if (!m_Socket)
    {
        fnLogData("There is a new connection...", false);

        m_Socket = std::unique_ptr<QTcpSocket>(m_TcpServer->nextPendingConnection());
        connectSignalSlot(true);

        if (fnStartSeningdData)
            fnStartSeningdData();
    }
    else
    {
        fnLogData("New connection rejected!", true);
    }
}

void clConnection::disconnected()
{
    fnLogData("Disconnecting...", false);
    
    if (fnStopSendingData)
        fnStopSendingData(true);

    Disconnect();
}

void clConnection::Disconnect()
{
    if (m_Socket)
    {
        fnLogData("Disconnect from host.", false);
        m_Socket->disconnectFromHost();

        m_Socket.release();
        m_Socket = nullptr;
    }
}

void clConnection::readyRead()
{
    if (fnReadyRead)
        fnReadyRead();
}

QByteArray clConnection::Read()
{
    return m_Socket->readAll();
}

void clConnection::Write(QByteArray const& paBytes)
{
    if (m_Socket)
    {
        m_Socket->write(paBytes);
        m_Socket->flush();
    }
}

void clConnection::Write(const char* paBytes, int64_t paLenght)
{
    if (m_Socket)
    {
        m_Socket->write(paBytes, paLenght);
        m_Socket->flush();
    }
}

