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


#include "DataGeneratorThread.h"

#include <memory>
#include <random>


clDataGeneratorThread::clDataGeneratorThread(clMainWindow* const paMainWindow, QThread *parent)
    : QThread{parent}, m_MainWindow(paMainWindow)
{
    Start();
    reset();
}
clDataGeneratorThread::~clDataGeneratorThread()
{
    wait();
    quit();
}

void clDataGeneratorThread::reset(){
m_Initialized = false;
m_ServerMode = false;
}

void clDataGeneratorThread::run()
{
    m_ThreadMutex.lock();
    m_ThreadMutex.unlock();


    m_ThreadWaitCondition.wakeAll();


    QThread::exec();
}

void clDataGeneratorThread::Start()
{
    m_ThreadMutex.lock();

    moveToThread(this);


    start();

    m_ThreadWaitCondition.wait(&m_ThreadMutex);

    m_ThreadMutex.unlock();

}


bool clDataGeneratorThread::startConnection(const stDataSource& paDataSource)
{
    bool loStatus(false);

    const auto StartConnectionFunc = [&]()
    {
        loStatus = invokeableStartConnection(paDataSource);
    };

    const auto ok = QMetaObject::invokeMethod(this,
        StartConnectionFunc,
        Qt::BlockingQueuedConnection
    );

    return ok && loStatus;
}

Q_INVOKABLE  bool clDataGeneratorThread::invokeableStartConnection(const stDataSource& paDataSource)
{
    m_DataSource = std::make_unique<stDataSource>(paDataSource);
    return initialize();
}

bool clDataGeneratorThread::initialize()
{
    m_TcpDevice.reset(new clConnection);

    m_TcpDevice->fnLogData = std::bind(&clDataGeneratorThread::logData, this, std::placeholders::_1, std::placeholders::_2);
    m_TcpDevice->fnReadyRead = std::bind(&clDataGeneratorThread::readDataFromSocket,this);

    std::string strPortNumber(m_DataSource->m_PortNummber);
    unsigned short portNumber = static_cast<unsigned short>(std::stoi(strPortNumber));
    std::string strIpAdress(m_DataSource->m_IpAdress);

    if (m_DataSource->m_ServerMode)
    {
        //m_MainWindow->LogText("Listenning ...", "black");

        const auto& loStatus = m_TcpDevice->Listen(QHostAddress(QString::fromStdString(strIpAdress)), portNumber);

        if (std::get<0>(loStatus)){
            m_Initialized = true;
            m_MainWindow->LogText("Waiting for clients ...", "black");
        }
        else{
            m_MainWindow->LogText(std::get<1>(loStatus), "red");
        }
    }
    else
    {
        m_MainWindow->LogText("Connecting ...", "black");

        const auto& loStatus = m_TcpDevice->Connect(QHostAddress(QString::fromStdString(strIpAdress)), portNumber);

        if (std::get<0>(loStatus)) {
            m_Initialized = true;
            m_MainWindow->LogText("The connection to Server was succesful!", "black");
        }
        else {
            m_MainWindow->LogText(std::get<1>(loStatus), "red");
        }
    }


    if (m_Initialized && m_DataSource->m_AutoSendData)
    {

        m_TcpDevice->fnStopSendingData = std::bind(&clDataGeneratorThread::stopSendingData, this, std::placeholders::_1);

        if (m_DataSource->m_ServerMode)
            m_TcpDevice->fnStartSeningdData = std::bind(&clDataGeneratorThread::sendData, this);
        else
            sendData();
    }

    return  m_Initialized;
}

void clDataGeneratorThread::sendData()
{
    std::string loIntervalType(m_DataSource->m_AutoSendIntervalType);
    m_StopSending = false;

    if (loIntervalType=="Fix")
    {
        if (m_SendingDataTimer == nullptr)
        {
            m_SendingDataTimer = new QTimer(this);
            connect(m_SendingDataTimer, &QTimer::timeout, this, &clDataGeneratorThread::writeResponse);
        }

        m_SendingDataTimer->start(static_cast<int>(std::get<0>(m_DataSource->m_AutoSendIntervalPair)));
    }
    else
    {
        const auto writeResponseFcn = [&]()
        {
            writeResponse();
        };

        QTimer::singleShot(generateRandomIntiger(std::get<0>(m_DataSource->m_AutoSendIntervalPair), std::get<1>(m_DataSource->m_AutoSendIntervalPair)), this, writeResponseFcn);
    }
}

bool clDataGeneratorThread::disConnect()
{
    bool loStatus(false);

    const auto& DeinitializeFunc = [&]()
    {
        loStatus = invokeableDeinitialize();
    };

    const bool& ok = QMetaObject::invokeMethod
   (this,
     DeinitializeFunc,
     Qt::BlockingQueuedConnection
   );

    return ok && loStatus;
}

bool clDataGeneratorThread::invokeableDeinitialize()
{
    if (!m_Initialized)
        return true;

    m_Initialized = false;
    m_MainWindow->LogText("Disconnecting ...", "black");
    
    if (m_SendingDataTimer != nullptr)
        m_SendingDataTimer->stop();

    m_TcpDevice->Disconnect();


    return true;
}


void clDataGeneratorThread::readDataFromSocket()
{
    QByteArray loByteArray = m_TcpDevice->Read();

    m_MainWindow->LogText("-->" + QString::fromLocal8Bit(loByteArray), "Black");

    //QFile file("data.txt");

    //if (!file.open(QIODevice::WriteOnly))
    //{
    //    qDebug() << "Failed to open the file.";
    //}
    //else
    //{
    //    QByteArray data;
    //    data.append(loByteArray);
    //    file.write(loByteArray);
    //    data.clear();
    //}

    //file.close();
}

void clDataGeneratorThread::writeResponse()
{
    if (m_StopSending){

        if (m_SendingDataTimer != nullptr) 
            m_SendingDataTimer->stop();

        return;
    }

    QString loDataSourceType(m_DataSource->m_DataSourceType);
    if (loDataSourceType == "Random text")
    {
        size_t sizeofArray = size_t(m_DataSource->m_DataSize * sizeof(int));
        auto m_RandomChar = std::unique_ptr<char, decltype(free)*>{ reinterpret_cast<char*>(malloc(sizeofArray)), free };

        for (size_t i = 0; i < sizeofArray; i += sizeof(int))
        {
            int loValue = generateRandomIntiger(0, 1000);
            char* loChar = reinterpret_cast<char*>(&loValue);

            *(m_RandomChar.get() + i) = *(loChar);
            *(m_RandomChar.get() + i + 1) = *(loChar + 1);
            *(m_RandomChar.get() + i + 2) = *(loChar + 2);
            *(m_RandomChar.get() + i + 3) = *(loChar + 3);
        }

        m_TcpDevice->Write(m_RandomChar.get(), static_cast<int64_t>(sizeofArray));
        m_MainWindow->LogText("<--" + QString::fromLocal8Bit(m_RandomChar.get(), static_cast<int>(sizeofArray)), "Black");
    }
    else
    { 
        m_TcpDevice->Write(QByteArray(m_DataSource->m_SourceDataText.get(), static_cast<int64_t>(m_DataSource->m_DataSize)));

        QString loLogTheMessage(QString::fromLocal8Bit(QByteArray(m_DataSource->m_SourceDataText.get(), static_cast<int64_t>(m_DataSource->m_DataSize))));
        m_MainWindow->LogText("<--" + loLogTheMessage, "Black");
    }

    std::string loIntervalType(m_DataSource->m_AutoSendIntervalType);
    if (loIntervalType=="Random")
    {
        const auto writeResponseFcn = [&]()
        {
            writeResponse();
        };

        QTimer::singleShot(generateRandomIntiger(std::get<0>(m_DataSource->m_AutoSendIntervalPair), std::get<1>(m_DataSource->m_AutoSendIntervalPair)), this, writeResponseFcn);
    }

}

int clDataGeneratorThread::generateRandomIntiger(const int paLowerLimit, const int paUpperLimit)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomNum(paLowerLimit, paUpperLimit);
    
    return randomNum(gen);
}

void clDataGeneratorThread::stopSendingData(bool paStopSending)
{
    m_StopSending = paStopSending;

    if (!m_DataSource->m_ServerMode)
        m_Initialized = false;

    m_MainWindow->disconnedted();
}

void clDataGeneratorThread::logData(const QString& paMessage, bool paError)
{
    if (paError)
        m_MainWindow->LogText(paMessage, "red");
    else
        m_MainWindow->LogText(paMessage, "black");
}