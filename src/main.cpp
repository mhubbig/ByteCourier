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


#include <crtdbg.h>

#include "MainWindow.h"
#include "DataGeneratorThread.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    clMainWindow loMainWindow;

    clDataGeneratorThread loClientEventLoop(&loMainWindow);
    loMainWindow.setClientEventLoop(&loClientEventLoop);

    loMainWindow.show();

    return a.exec();
}
