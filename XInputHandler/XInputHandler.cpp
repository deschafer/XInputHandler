#include "XInputHandler.h"



XInputHandler::XInputHandler()
{
}


XInputHandler::~XInputHandler()
{
}

Controller *XInputHandler::GetState()
{

	// Save the last state
	m_LastController = m_Controller;

	// Reset and get the new state
	ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_ControllerState);

	m_Controller.Reset();
	// The values of the Left and Right Triggers go from 0 to 255. We just convert them to 0.0f=>1.0f
	if (m_ControllerState.Gamepad.bRightTrigger && m_ControllerState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		m_Controller.m_RightTrigger = static_cast<int>(m_ControllerState.Gamepad.bRightTrigger / 50);
	}

	if (m_ControllerState.Gamepad.bLeftTrigger && m_ControllerState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		m_Controller.m_LeftTrigger = static_cast<int>(m_ControllerState.Gamepad.bLeftTrigger / 50);
	}

	// Get the Buttons
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) m_Controller.m_ButtonA = true;
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) m_Controller.m_ButtonB = true;
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) m_Controller.m_ButtonX = true;
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) m_Controller.m_ButtonY = true;
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) m_Controller.m_SelectButton = true;
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) m_Controller.m_StartButton = true;
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) m_Controller.m_LeftBumper = true;
	if (m_ControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) m_Controller.m_RightBumper = true;

	// Check to make sure we are not moving during the dead zone
	// Let's check the Left DeadZone
	if ((m_ControllerState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_ControllerState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_ControllerState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_ControllerState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		m_ControllerState.Gamepad.sThumbLX = 0;
		m_ControllerState.Gamepad.sThumbLY = 0;
	}

	// Check left thumbStick
	float LeftThumbY = m_ControllerState.Gamepad.sThumbLY;
	if (LeftThumbY)
	{
		m_Controller.m_LeftJoystick.Y = static_cast<int>(LeftThumbY) / 6400;
	}
	float LeftThumbX = m_ControllerState.Gamepad.sThumbLX;
	if (LeftThumbX)
	{
		m_Controller.m_LeftJoystick.X = static_cast<int>(LeftThumbX) / 6400;
	}

	// Check to make sure we are not moving during the dead zone
	// Let's check the Left DeadZone
	if ((m_ControllerState.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_ControllerState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_ControllerState.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_ControllerState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		m_ControllerState.Gamepad.sThumbRX = 0;
		m_ControllerState.Gamepad.sThumbRY = 0;
	}

	// Check left thumbStick
	float RightThumbY = m_ControllerState.Gamepad.sThumbRY;
	if (RightThumbY)
	{
		m_Controller.m_RightJoystick.Y = static_cast<int>(RightThumbY) / 6400;
	}
	float RightThumbX = m_ControllerState.Gamepad.sThumbRX;
	if (RightThumbX)
	{
		m_Controller.m_RightJoystick.X = static_cast<int>(RightThumbX) / 6400;
	}

	// Now if the new state is not the same as the old state, we return it
	// Otherwise return nullptr
	if (!(m_LastController == m_Controller))
	{
		// Queueing this command
		m_QueuedCommands.push_back(&m_Controller);

		return &m_Controller;
	}
	else return nullptr;
}

void XInputHandler::Vibrate(int Left, int Right)
{

	XINPUT_VIBRATION Vibration;

	Vibration.wLeftMotorSpeed = Left;
	Vibration.wRightMotorSpeed = Right;

	XInputSetState(0, &Vibration);
}

bool XInputHandler::IsConnected()
{

	ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));

	DWORD Error = XInputGetState(0, &m_ControllerState);

	if (Error == ERROR_SUCCESS) return true;
	else return false;
}

Controller* XInputHandler::DeQueueFirstState()
{
	Controller* NewGamepad;

	if (!m_QueuedCommands.empty())
		return NewGamepad = m_QueuedCommands.front();
	else return nullptr;
}

void XInputHandler::PopFirstState()
{
	if (!m_QueuedCommands.empty())
		m_QueuedCommands.pop_front();
}
