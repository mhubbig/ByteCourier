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


#include "MainWindow.h"
#include "DataGeneratorThread.h"

#include <QScrollBar>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>

clMainWindow::clMainWindow(QWidget *parent) : QMainWindow(parent)
{
    int loRow = 0;
    int loColumn = 0;
    m_ConnectedToServer = false;

    setGeometry(100, 100, 900, 600);
    setWindowTitle("Byte Courier");

    QIcon icon;
    icon.addFile(QString::fromUtf8("Images/Networked_Computers.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    m_CentralWidget = new QWidget(this);
    m_CentralWidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(m_CentralWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    m_MainTabWidget = new QTabWidget(m_CentralWidget);

    m_MainTabWidget->addTab(buildOperationWidget(m_CentralWidget), "Setting");
    m_MainTabWidget->addTab(buildDataSourceWidget(m_CentralWidget), "Data Source");
    m_MainTabWidget->addTab(buildLogWidget(m_CentralWidget), "Log");
    
    gridLayout->addWidget(m_MainTabWidget);

    m_ConnectButton = new QPushButton("Connect to Server");
    m_ConnectButton->setMaximumSize(QSize(120, 30));
    gridLayout->addWidget(m_ConnectButton);

    m_CentralWidget->setLayout(gridLayout);
    setCentralWidget(m_CentralWidget);

    buildSignalSlotConnection();
}

void clMainWindow::buildSignalSlotConnection()
{
    connect(m_ConnectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(m_TcpIpRadioClient     , SIGNAL(pressed()), this, SLOT(radioButtonsTcpClient()));
    connect(m_TcpIpRadioServer, SIGNAL(pressed()), this, SLOT(radioButtonsTcpServer()));
    connect(m_DataSourceComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(dataSourceComboBoxChanged(const QString&)));
    connect(m_IntervalCheckComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(intervalCheckComboBoxChanged(const QString&)));
    connect(m_IntervalCheckBox, SIGNAL(clicked(bool)), this, SLOT(intervalCheckBox(bool)));
    connect(m_OpenFileButton, SIGNAL(clicked()), this, SLOT(openTextFile()));
}

void clMainWindow::setClientEventLoop( clDataGeneratorThread* const paClientEventLoop)
{
    m_ClientEventLoop = paClientEventLoop;
}

QFrame* clMainWindow::makeFrame(QWidget *paParent)
{
    QFrame *loFrame = new QFrame(paParent);
    return loFrame;
}


QWidget* clMainWindow::buildOperationWidget(QWidget *paParent)
{
    int row(0), column(0);

    QWidget* loMainSettingWidget = new QWidget(paParent);
    QGridLayout* loMainSettingGuiLayout = new QGridLayout(loMainSettingWidget);
    loMainSettingWidget->setLayout(loMainSettingGuiLayout);

    QFrame *loTcpIpClientFrame = new QFrame(paParent);
    loTcpIpClientFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    loTcpIpClientFrame->setMaximumHeight(60);

    clientOperationWidget(loTcpIpClientFrame);


    QFrame* loTcpIpServerFrame = new QFrame(paParent);
    loTcpIpServerFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    loTcpIpServerFrame->setMaximumHeight(60);

    serverOperationWidget(loTcpIpServerFrame);

    loMainSettingGuiLayout->addWidget(loTcpIpClientFrame, ++row, column);
    loMainSettingGuiLayout->addWidget(loTcpIpServerFrame, ++row, column);

    loMainSettingGuiLayout->addItem(new QSpacerItem(20, 400, QSizePolicy::Expanding, QSizePolicy::Expanding), ++row, column);
    loMainSettingGuiLayout->addItem(new QSpacerItem(300, 100, QSizePolicy::Expanding, QSizePolicy::Expanding), row, ++column);

    return loMainSettingWidget;
}

void clMainWindow::serverOperationWidget(QFrame* loTcpIpServerFrame)
{
    m_TcpIpRadioServer = new QRadioButton("TCP/IP Server");
    m_TcpIpRadioServer->setMaximumSize(QSize(140, 30));
    m_TcpIpRadioClient->setChecked(true);
    m_TcpIpRadioClient->setObjectName("TCP/IPServer");

    m_IpAdressTcpServer = new QTextEdit(loTcpIpServerFrame);
    m_IpAdressTcpServer->setMaximumSize(QSize(140, 28));
    m_IpAdressTcpServer->setFontPointSize(qreal(9));
    m_IpAdressTcpServer->setText("127.0.0.1");

    m_PortNumberServer = new QTextEdit(loTcpIpServerFrame);
    m_PortNumberServer->setMaximumSize(QSize(70, 28));
    m_PortNumberServer->setFontPointSize(qreal(9));
    m_PortNumberServer->setText("5000");

    QHBoxLayout* loTcpServerOperationLayout = new QHBoxLayout(loTcpIpServerFrame);

    loTcpServerOperationLayout->addWidget(m_TcpIpRadioServer);
    loTcpServerOperationLayout->addWidget(m_IpAdressTcpServer);
    loTcpServerOperationLayout->addWidget(m_PortNumberServer);
    loTcpServerOperationLayout->addStretch();
    loTcpIpServerFrame->setLayout(loTcpServerOperationLayout);
}

void clMainWindow::clientOperationWidget(QFrame* loTcpIpClientFrame)
{
    m_TcpIpRadioClient = new QRadioButton("TCP/IP Client");
    m_TcpIpRadioClient->setMaximumSize(QSize(140, 30));
    m_TcpIpRadioClient->setObjectName("TCP/IPClient");

    m_IpAdressTcpClient = new QTextEdit(loTcpIpClientFrame);
    m_IpAdressTcpClient->setMaximumSize(QSize(140, 28));
    m_IpAdressTcpClient->setFontPointSize(qreal(9));
    m_IpAdressTcpClient->setText("127.0.0.1");

    m_PortNumberClient = new QTextEdit(loTcpIpClientFrame);
    m_PortNumberClient->setMaximumSize(QSize(70, 28));
    m_PortNumberClient->setFontPointSize(qreal(9));
    m_PortNumberClient->setText("5000");

    QHBoxLayout* loTcpClientOperationLayout = new QHBoxLayout(loTcpIpClientFrame);

    loTcpClientOperationLayout->addWidget(m_TcpIpRadioClient);
    loTcpClientOperationLayout->addWidget(m_IpAdressTcpClient);
    loTcpClientOperationLayout->addWidget(m_PortNumberClient);
    loTcpClientOperationLayout->addStretch();
    loTcpIpClientFrame->setLayout(loTcpClientOperationLayout);
}


QWidget* clMainWindow::buildDataSourceWidget(QWidget* paParent)
{
    int row(0), column(0);

    QWidget* loMainDataSourceWidget = new QWidget(paParent);
    QGridLayout* loMainDataSourceGuiLayout = new QGridLayout(loMainDataSourceWidget);
    loMainDataSourceWidget->setLayout(loMainDataSourceGuiLayout);

    /************************* Data source part */
    QFrame* loDataSourceFrame = new QFrame(paParent);
    loDataSourceFrame->setFrameStyle(QFrame::Box | QFrame::Raised);
    loDataSourceFrame->setMaximumHeight(60);

    dataSourceSelection(loDataSourceFrame);

    /************************* loading file and typing string part */
    m_TextFrame = new QFrame(paParent);
    //m_TextFrame->setMaximumHeight(60);
    m_TextFrame->hide();

    loadDataWidget();

    /************************* Sending message interval part */
    QFrame* loIntervalFrame = new QFrame(paParent);

    dataTransferInterval(loIntervalFrame);

    /************************* Sending message interval part */
    QFrame* loSaveLogFrame = new QFrame(paParent);

    saveDataWidget(loSaveLogFrame);
    
    //************************ Adding the widget to rhe main layout
    loMainDataSourceGuiLayout->addWidget(loDataSourceFrame, ++row, column);
    loMainDataSourceGuiLayout->addWidget(m_TextFrame, ++row, column);
    loMainDataSourceGuiLayout->addWidget(loIntervalFrame, ++row, column);
    loMainDataSourceGuiLayout->addWidget(loSaveLogFrame, ++row, column);

    loMainDataSourceGuiLayout->addItem(new QSpacerItem(20, 400, QSizePolicy::Expanding, QSizePolicy::Expanding), ++row, column);

    return loMainDataSourceWidget;
}

void clMainWindow::saveDataWidget(QFrame* loSaveLogFrame)
{
    m_SaveTheLog = new QCheckBox("Save log file", loSaveLogFrame);

    QHBoxLayout* loSaveTheLogLayout = new QHBoxLayout(loSaveLogFrame);

    loSaveTheLogLayout->addWidget(m_SaveTheLog);
    loSaveTheLogLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));

    loSaveLogFrame->setLayout(loSaveTheLogLayout);
}

void clMainWindow::loadDataWidget()
{
    m_DataText = new QTextEdit(m_TextFrame);

    m_OpenFileButton = new QPushButton(m_TextFrame);
    QIcon icon;
    icon.addFile(QString::fromUtf8("Images/open-file.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_OpenFileButton->setIcon(icon);

    QHBoxLayout* loTextLayout = new QHBoxLayout(m_TextFrame);

    loTextLayout->addWidget(m_DataText);
    loTextLayout->addWidget(m_OpenFileButton);
    m_TextFrame->setLayout(loTextLayout);
}

void clMainWindow::dataTransferInterval(QFrame* loIntervalFrame)
{
    m_IntervalCheckBox = new QCheckBox("Auto Sending Interval (ms)", loIntervalFrame);

    m_IntervalCheckComboBox = new QComboBox(loIntervalFrame);
    m_IntervalCheckComboBox->addItem("Fix");
    m_IntervalCheckComboBox->addItem("Random");
    m_IntervalCheckComboBox->setEnabled(false);

    m_FixIntervalSpinBox = new QDoubleSpinBox(loIntervalFrame);
    m_FixIntervalSpinBox->setRange(1.0, 1000000.0);
    m_FixIntervalSpinBox->setValue(3000.0);
    m_FixIntervalSpinBox->hide();


    m_RandomIntervalSpinBox = new QDoubleSpinBox(loIntervalFrame);
    m_RandomIntervalSpinBox->setRange(1.0, 1000000.0);
    m_RandomIntervalSpinBox->setValue(4000.0);
    m_RandomIntervalSpinBox->hide();


    QHBoxLayout* loIntervalLayout = new QHBoxLayout(loIntervalFrame);

    loIntervalLayout->addWidget(m_IntervalCheckBox);
    loIntervalLayout->addWidget(m_IntervalCheckComboBox);
    loIntervalLayout->addWidget(m_FixIntervalSpinBox);
    loIntervalLayout->addWidget(m_RandomIntervalSpinBox);
    loIntervalLayout->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));

    loIntervalFrame->setLayout(loIntervalLayout);
}

void clMainWindow::dataSourceSelection(QFrame* loDataSourceFrame)
{
    m_DataSourceLabel = new QLabel("Data Source", loDataSourceFrame);
    m_DataSourceLabel->setMaximumWidth(140);

    m_DataSourceComboBox = new QComboBox(loDataSourceFrame);
    m_DataSourceComboBox->addItem("Random text");
    m_DataSourceComboBox->addItem("Text file");
    m_DataSourceComboBox->addItem("Text string");


    QHBoxLayout* loDataSourceLayout = new QHBoxLayout(loDataSourceFrame);

    loDataSourceLayout->addWidget(m_DataSourceLabel);
    loDataSourceLayout->addWidget(m_DataSourceComboBox);
    loDataSourceFrame->setLayout(loDataSourceLayout);
}

QWidget* clMainWindow::buildLogWidget(QWidget *paParent)
{
    QFrame* loFrame = makeFrame(paParent);

    m_LogText = new QTextEdit(loFrame);
    m_LogText->setReadOnly(true);
    m_LogText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* loVBoxLayout = new QVBoxLayout(loFrame);
    loFrame->setLayout(loVBoxLayout);

    loVBoxLayout->addWidget(m_LogText);

    return loFrame;
}

void clMainWindow::LogText(QString const& paMessage, QString const& paForegroundColor)
{
    QMetaObject::invokeMethod
        (this,
            "invokeAbleLogText",
            Qt::QueuedConnection,
            Q_ARG(QString const& , paMessage),
            Q_ARG(const QString&, paForegroundColor)
         );
}

void clMainWindow::invokeAbleLogText(QString const& paMessage, QString const& paForegroundColor)
{
    m_LogText->setTextColor(paForegroundColor);
    if (paMessage.isEmpty())
    {
        m_LogText->append("");
    }
    else
    {
        m_LogText->setFontPointSize(qreal(9));
        QString loMessage(QString("%1     %2").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss:zzz")).arg(paMessage));
        m_LogText->append(loMessage);

        if (m_FileIsOpen)
            writeToFile(loMessage);
    }

    QScrollBar* loVerticalScrollBar = m_LogText->verticalScrollBar();
    loVerticalScrollBar->setValue(loVerticalScrollBar->maximum());
}

void clMainWindow::writeToFile(QString& loMessage)
{
    QByteArray szByte = loMessage.toLocal8Bit();
    std::string szMessage(szByte.constData(), szByte.length());

    for (const char loChar : szMessage)
    {
        char value[2];
        value[0] = loChar;
        value[1] = _T('\0');
        fprintf(m_LogFile, "%s", &value);
    }
    fprintf(m_LogFile, "\n");
}

void clMainWindow:: connectToServer()
{
    if (!m_ConnectedToServer)
    {
        QByteArray loPortNumber;
        QByteArray loIpAdress;

        if (!prepareConnection())
            return;

        if (m_TcpIpRadioClient->isChecked())
        {

            m_CurrentDataSource.m_ServerMode = false;

            loPortNumber = m_PortNumberClient->toPlainText().toLatin1();
            loIpAdress = m_IpAdressTcpClient->toPlainText().toLatin1();

            strncpy_s(m_CurrentDataSource.m_IpAddress, sizeof(m_CurrentDataSource.m_IpAddress), loIpAdress.constData(), loIpAdress.length());
            m_CurrentDataSource.m_IpAddress[sizeof(m_CurrentDataSource.m_IpAddress) - 1] = _T('\0');

            strncpy_s(m_CurrentDataSource.m_PortNummber, sizeof(m_CurrentDataSource.m_PortNummber), loPortNumber.constData(), loPortNumber.length());
            m_CurrentDataSource.m_PortNummber[sizeof(m_CurrentDataSource.m_PortNummber) - 1] = _T('\0');

            if (m_ClientEventLoop->startConnection(m_CurrentDataSource)) {
                m_ConnectButton->setText("Disconnect");
                m_ConnectedToServer = true;
            }
        }
        else if (m_TcpIpRadioServer->isChecked())
        {

            m_CurrentDataSource.m_ServerMode = true;

            loPortNumber = m_PortNumberServer->toPlainText().toLatin1();
            loIpAdress = m_IpAdressTcpServer->toPlainText().toLatin1();

            strncpy_s(m_CurrentDataSource.m_IpAddress, sizeof(m_CurrentDataSource.m_IpAddress), loIpAdress.constData(), loIpAdress.length());
            m_CurrentDataSource.m_IpAddress[sizeof(m_CurrentDataSource.m_IpAddress) - 1] = _T('\0');

            strncpy_s(m_CurrentDataSource.m_PortNummber, sizeof(m_CurrentDataSource.m_PortNummber), loPortNumber.constData(), loPortNumber.length());
            m_CurrentDataSource.m_PortNummber[sizeof(m_CurrentDataSource.m_PortNummber) - 1] = _T('\0');

            if (m_ClientEventLoop->startConnection(m_CurrentDataSource)) {
                m_ConnectButton->setText("Disconnect");
                m_ConnectedToServer = true;
            }
        }

    }
    else
    {
        if (m_ClientEventLoop->disConnect()) {

            if (m_ConnectedToServer)
            {
                m_ConnectedToServer = false;

                if (m_TcpIpRadioClient     ->isChecked())
                    m_ConnectButton->setText("Connect to Server");
                else if (m_TcpIpRadioServer->isChecked())
                    m_ConnectButton->setText("Start Server");

                if (m_FileIsOpen)
                {
                    m_FileIsOpen = false;

                    fflush(m_LogFile);
                    fclose(m_LogFile);
                }

            }
        }
        
    }
}

bool clMainWindow::prepareConnection()
{
    m_FileIsOpen = false;

    if (m_SaveTheLog->isChecked())
    {
        QString loDateTime = QDateTime::currentDateTime().toString("yyyyMMdd_hh.mm");
        std::string loFileName("LogFile_" + loDateTime.toStdString() + ".txt");
        const char* fname = loFileName.c_str();

        m_LogFile = fopen(fname, "a");
        if (m_LogFile)
            m_FileIsOpen = true;

    }

    if (m_IntervalCheckBox->isChecked() && m_IntervalCheckComboBox->currentText() == "Random")
    {
        if (m_FixIntervalSpinBox->value() > m_RandomIntervalSpinBox->value())
        {
            QMessageBox msgBox;
            msgBox.setText("The right value of interval must be bigger than the left value!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();

            return false;
        }
    }

    if (m_DataSourceComboBox->currentText() == "Random text") {
        m_CurrentDataSource.m_DataSize = 20;
    }
    else {

        if (m_DataText->toPlainText().isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("Fill the message text or load the file!");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
            return false;
        }

        fillDataSourceText();
    }

    m_CurrentDataSource.m_AutoSendData = m_IntervalCheckBox->isChecked();
    m_CurrentDataSource.m_Savelog = m_SaveTheLog->isChecked();

    const QByteArray& loDataSourceType = m_DataSourceComboBox->currentText().toLatin1();
    strncpy_s(m_CurrentDataSource.m_DataSourceType, sizeof(m_CurrentDataSource.m_DataSourceType), loDataSourceType.constData(), loDataSourceType.length());
    m_CurrentDataSource.m_DataSourceType[sizeof(m_CurrentDataSource.m_DataSourceType) - 1] = _T('\0');

    QByteArray loIntervalType = m_IntervalCheckComboBox->currentText().toLatin1();
    strncpy_s(m_CurrentDataSource.m_AutoSendIntervalType, sizeof(m_CurrentDataSource.m_AutoSendIntervalType), loIntervalType.constData(), loIntervalType.length());
    m_CurrentDataSource.m_AutoSendIntervalType[sizeof(m_CurrentDataSource.m_AutoSendIntervalType) - 1] = _T('\0');

    m_CurrentDataSource.m_AutoSendIntervalPair = { m_FixIntervalSpinBox->value() ,  m_RandomIntervalSpinBox->value() };

    return true;
}

void clMainWindow::radioButtonsTcpClient()
{
    m_ConnectButton->setText("Connect to Server");

    QList<QRadioButton*> allPRadioButtons = findChildren<QRadioButton*>();

    for (const auto& item: allPRadioButtons)
        item->setChecked(false);
}

void clMainWindow::radioButtonsTcpServer()
{
    m_ConnectButton->setText("Start Server");

    QList<QRadioButton*> allPRadioButtons = findChildren<QRadioButton*>();

    for (const auto& item : allPRadioButtons)
        item->setChecked(false);
}

void clMainWindow::dataSourceComboBoxChanged(const QString& paDataSourceType)
{
    if (m_DataSourceComboBox->currentText() == "Random text")
    {
        m_TextFrame->hide();
    }
    else if (m_DataSourceComboBox->currentText() == "Text file")
    {
        m_TextFrame->show();
        m_TextFrame->setMaximumHeight(60);
        m_OpenFileButton->show();
        m_DataText->clear();

    }
    else if (m_DataSourceComboBox->currentText() == "Text string")
    {
        m_TextFrame->show();
        m_TextFrame->setMaximumHeight(200);
        m_OpenFileButton->hide();
        m_DataText->clear();
    }
    else
    {
        LogText("Error with selecting data source type", "red");
    }
}

void clMainWindow::intervalCheckComboBoxChanged(const QString& paIntervalType)
{
    if (m_IntervalCheckComboBox->currentText() == "Fix")
        m_RandomIntervalSpinBox->hide();
    else if (m_IntervalCheckComboBox->currentText() == "Random")
        m_RandomIntervalSpinBox->show();
    else
        LogText("Error with selecting data source type", "red");
}

void clMainWindow::intervalCheckBox(bool paChecked)
{
    if (paChecked) {
        m_FixIntervalSpinBox->show();
        m_IntervalCheckComboBox->setEnabled(true);

        if (m_IntervalCheckComboBox->currentText() == "Random")
            m_RandomIntervalSpinBox->show();
    }
    else {
        m_IntervalCheckComboBox->setEnabled(false);
        m_FixIntervalSpinBox->hide();
        m_RandomIntervalSpinBox->hide();
    }

}

void clMainWindow::openTextFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text File"), "", tr("Text Files (*.txt)"));

    if (!fileName.isEmpty()){
        m_DataText->clear();
        m_DataText->append(fileName);
    }
}

void clMainWindow::fillDataSourceText()
{
    if (m_DataSourceComboBox->currentText() == "Text string")
    {
        m_CurrentDataSource.m_DataSize = m_DataText->toPlainText().length();
        m_CurrentDataSource.m_SourceDataText = std::shared_ptr<char>(new char[m_CurrentDataSource.m_DataSize](), std::default_delete<char[]>());

        QByteArray loDataText = m_DataText->toPlainText().toLocal8Bit();

        memset(m_CurrentDataSource.m_SourceDataText.get(), '\0', m_CurrentDataSource.m_DataSize);
        memcpy(m_CurrentDataSource.m_SourceDataText.get(), loDataText.constData(), m_CurrentDataSource.m_DataSize);

    }
    else if (m_DataSourceComboBox->currentText() == "Text file")
    {
        std::ifstream loFile(m_DataText->toPlainText().toStdString());

        if (!loFile.is_open()) {
            LogText("Error opening the file!", "red");
            return;
        }

        std::string loFileContent;
        std::string loReadLine;

        while (std::getline(loFile, loReadLine)) {
            std::cout << loReadLine << std::endl;
            loFileContent += loReadLine;
        }


        m_CurrentDataSource.m_DataSize = loFileContent.length();
        m_CurrentDataSource.m_SourceDataText = std::shared_ptr<char>(new char[m_CurrentDataSource.m_DataSize](), std::default_delete<char[]>());

        memset(m_CurrentDataSource.m_SourceDataText.get(), '\0', m_CurrentDataSource.m_DataSize);
        memcpy(m_CurrentDataSource.m_SourceDataText.get(), loFileContent.c_str(), m_CurrentDataSource.m_DataSize);

        loFile.close();
    }
    else
    {
        //other metod
    }

}

void clMainWindow::disconnedted()
{
    const auto& loDisconnect = [&]()
    {
        invokabeDisconnedted();
    };

    QMetaObject::invokeMethod(this, loDisconnect, Qt::QueuedConnection);
}
void clMainWindow::invokabeDisconnedted()
{
    if (m_ConnectedToServer && m_TcpIpRadioClient     ->isChecked())
    {
        m_ConnectedToServer = false;

        if (m_TcpIpRadioClient     ->isChecked())
            m_ConnectButton->setText("Connect to Server");
        else if (m_TcpIpRadioServer->isChecked())
            m_ConnectButton->setText("Start Server");

        if (m_FileIsOpen)
        {
            m_FileIsOpen = false;

            fflush(m_LogFile);
            fclose(m_LogFile);
        }

    }
}
