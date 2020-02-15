#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <vector>
#include <list>

struct Point
{
	int X;
	int Y;

	bool operator!=(Point Other)
	{
		if (X != Other.X) return true;
		if (Y != Other.Y) return true;

		return false;
	}
};

struct Controller
{
	
	bool m_ButtonA;
	bool m_ButtonB;
	bool m_ButtonY;
	bool m_ButtonX;
	bool m_StartButton;
	bool m_SelectButton;

	bool m_RightBumper;
	bool m_LeftBumper;

	Point m_LeftJoystick;
	Point m_RightJoystick;

	int m_LeftTrigger;
	int m_RightTrigger;

	void Reset()
	{
		m_ButtonA = 0;
		m_ButtonB = 0;
		m_ButtonY = 0;
		m_ButtonX = 0;
		m_StartButton = 0;
		m_SelectButton = 0;
		m_RightBumper = 0;
		m_LeftBumper = 0;
		m_LeftJoystick.X = 0;
		m_LeftJoystick.Y = 0;
		m_RightJoystick.X = 0;
		m_RightJoystick.Y = 0;
		m_LeftTrigger = 0;
		m_RightTrigger = 0;
	}

	bool operator==(Controller Other)
	{

		if (Other.m_ButtonA != m_ButtonA) return false;
		if (Other.m_ButtonB != m_ButtonB) return false;
		if (Other.m_ButtonX != m_ButtonX) return false;
		if (Other.m_ButtonY != m_ButtonY) return false;
		if (Other.m_StartButton != m_StartButton) return false;
		if (Other.m_SelectButton != m_SelectButton) return false;
		if (Other.m_RightBumper != m_RightBumper) return false;
		if (Other.m_LeftBumper != m_LeftBumper) return false;
		if (Other.m_LeftJoystick != m_LeftJoystick) return false;
		if (Other.m_RightJoystick != m_RightJoystick) return false;
		if (Other.m_LeftTrigger != m_LeftTrigger) return false;
		if (Other.m_RightTrigger != m_RightTrigger) return false;
		return true;
	}

};

class XInputHandler
{
private:

	XINPUT_STATE m_ControllerState;
	Controller m_Controller;
	Controller m_LastController;

	std::list<Controller*> m_QueuedCommands;

public:

	Controller *GetState();

	void Vibrate(int Left, int Right);

	bool IsConnected();

	Controller* DeQueueFirstState();
	void PopFirstState();
	int GetQueueCount() { return m_QueuedCommands.size(); }
	

	XInputHandler();
	~XInputHandler();
};

// Every state that is gathered and different is added to the queue
