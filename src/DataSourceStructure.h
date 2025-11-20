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


#ifndef DATASOURCESTRUCTURE_H
#define DATASOURCESTRUCTURE_H

#include <tuple>
#include <tchar.h>

#include <QString>

struct stDataSource
{
	stDataSource()
	{
		m_AutoSendData = false;
		m_Savelog = false;
		m_ServerMode = false;
		std::fill(std::begin(m_IpAdress), std::end(m_IpAdress), '\0');
		std::fill(std::begin(m_PortNummber), std::end(m_PortNummber), '\0');
		std::fill(std::begin(m_AutoSendIntervalType), std::end(m_AutoSendIntervalType), '\0');
		std::fill(std::begin(m_DataSourceType), std::end(m_DataSourceType), '\0');
		m_AutoSendIntervalPair = { 3000.0, 3000.0 };
		m_DataSize = 0;
		m_SourceDataText = nullptr;
	}

	bool m_AutoSendData;
	bool m_Savelog;
	bool m_ServerMode;
	char m_IpAdress[50];
	char m_PortNummber[50];
	char m_AutoSendIntervalType[50];
	char m_DataSourceType[50];
	size_t m_DataSize;
	std::tuple<double, double> m_AutoSendIntervalPair;
	std::shared_ptr <char> m_SourceDataText;
};

#endif // DATASOURCESTRUCTURE_H
