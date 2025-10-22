# ByteCourier (TCP/IP Client-Server Communication Tool) 
=======================================



Overview

This is a cross-platform TCP/IP communication software developed in C++ using the Qt framework. It provides a graphical user interface (GUI) that allows users to operate in either client or server mode and exchange data over a TCP connection.

Features

Client and Server Modes:

Users can select whether to run the application as a TCP client or server.

Data Transmission Options:

Send a text file.

Send randomly generated text.

Send a custom string written by the user.

Choose between fixed or random time intervals for sending data.

Logging:

A dedicated log tab displays connection status and data transmission history.

Users can choose to save the log history to a file.

Technology Stack

Programming Language: C++

GUI Framework: Qt

Build System: CMake

Installation & Usage

Clone the repository: git clone https://github.com/mhubbig/ByteCourier.git

Build the project using CMake: 
mkdir build 
cd build 
cmake .. 
make

Run the application:

The compiled binaries are located in the bin folder, which is in the same directory as the main CMake file.

Launch the executable from the bin folder to start the GUI.

## License
This project is licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).  
