#pragma once

#include <string>
#include <fstream>

class Messenger
{

	std::fstream m_OpenFile;
	Messenger() {}

	std::string m_FileServerName;
	
	bool OpenFile();

public:
	Messenger(std::string ServerName);

	bool IsServerFull();
	bool WriteToServer(std::string FormattedString);

	~Messenger() { m_OpenFile.close(); }
};

