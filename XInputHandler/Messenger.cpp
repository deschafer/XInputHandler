#include "Messenger.h"

#include <iostream>

// Create the file
Messenger::Messenger(std::string ServerName) : 
	m_FileServerName(ServerName)
{
	std::ofstream File(ServerName);
	File.close();
}

bool Messenger::OpenFile()
{
	// We open this file for output
	m_OpenFile.open(m_FileServerName, std::fstream::out | std::fstream::in);

	if (!m_OpenFile.fail())
	{
		return true;
	}
	else
	{
		std::cout << "File was unable to be opened" << std::endl;
		return false;
	}
}

bool Messenger::IsServerFull()
{
	bool Result = OpenFile();

	// If the file was able to be opened
	if (Result)
	{
		bool Full = m_OpenFile.peek() == std::ifstream::traits_type::eof();
		m_OpenFile.close();
		return !Full;
	}
	else
	{
		m_OpenFile.close();
		return false;
	}
}

bool Messenger::WriteToServer(std::string FormattedString)
{
	if (!IsServerFull())
	{
		if (OpenFile())
		{
			m_OpenFile.write(FormattedString.c_str(), FormattedString.size());
			if (m_OpenFile.fail())
			{
				m_OpenFile.close();
				return false;
			}

			m_OpenFile.close();


			return true;
		}
		else return false;
	}
	else
	{
		return false;
	}
}