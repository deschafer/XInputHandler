

#include "XInputHandler.h"
#include "Messenger.h"


#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main()
{
	XInputHandler Handler;
	Messenger Server("ServerFile.txt");
	Controller* Gamepad = nullptr;

	long long duration = 0;
	long long OldDuration = 0;
	int Count = 0;

	// As long as a controller is connected
	while (Handler.IsConnected())
	{
		Gamepad = Handler.GetState();
		// Printing out the last received state
		if (Gamepad)
		{
			cout << "A: " << Gamepad->m_ButtonA << endl
				<< "B: " << Gamepad->m_ButtonB << endl
				<< "Y: " << Gamepad->m_ButtonX << endl
				<< "X: " << Gamepad->m_ButtonY << endl
				<< "Start: " << Gamepad->m_StartButton << endl
				<< "Select: " << Gamepad->m_SelectButton << endl
				<< "RBumper: " << Gamepad->m_RightBumper << endl
				<< "LBumper: " << Gamepad->m_LeftBumper << endl
				<< "LTrigger " << Gamepad->m_LeftTrigger << endl
				<< "RTrigger: " << Gamepad->m_RightTrigger << endl
				<< "LX: " << Gamepad->m_LeftJoystick.X << endl
				<< "LY: " << Gamepad->m_LeftJoystick.Y << endl
				<< "RX: " << Gamepad->m_RightJoystick.X << endl
				<< "RY: " << Gamepad->m_RightJoystick.Y << endl
				<< "CT: " << Count << endl
				<< "Queue: " << Handler.GetQueueCount() << endl
				<< "-------------------------------------\n";
		}

		// Dequeue a state, if it exists, and attempt to send it
		Gamepad = Handler.DeQueueFirstState();
		if (Gamepad)
		{
			// Getting the formatted message
			string Number = to_string(Count);
			string ButtonA = to_string(Gamepad->m_ButtonA) + "\n";
			string ButtonB = to_string(Gamepad->m_ButtonB) + "\n";
			string ButtonX = to_string(Gamepad->m_ButtonX) + "\n";
			string ButtonY = to_string(Gamepad->m_ButtonY) + "\n";
			string ButtonStart = to_string(Gamepad->m_StartButton) + "\n";
			string ButtonSelect = to_string(Gamepad->m_SelectButton) + "\n";
			string BumperLeft = to_string(Gamepad->m_LeftBumper) + "\n";
			string BumperRight = to_string(Gamepad->m_RightBumper) + "\n";
			string TriggerLeft = to_string(Gamepad->m_LeftTrigger) + "\n";
			string TriggerRight = to_string(Gamepad->m_RightTrigger) + "\n";
			string JoystickLeftX = to_string(Gamepad->m_LeftJoystick.X) + "\n";
			string JoystickLeftY = to_string(Gamepad->m_LeftJoystick.Y) + "\n";
			string JoystickRightX = to_string(Gamepad->m_RightJoystick.X) + "\n";
			string JoystickRightY = to_string(Gamepad->m_RightJoystick.Y) + "\n";

			// Writting the formatted message
			bool Written = Server.WriteToServer(
				ButtonA +
				ButtonB +
				ButtonX +
				ButtonY +
				ButtonStart +
				ButtonSelect +
				BumperLeft +
				BumperRight +
				TriggerLeft +
				TriggerRight +
				JoystickLeftX +
				JoystickLeftY +
				JoystickRightX +
				JoystickRightY +
				Number
			);

			// Only remove from the queue if we have sent the message
			if (Written)
			{
				cout << "State was sent successfully" << endl;
				Count++;
				Handler.PopFirstState();
			}
		}
	}

	cout << " Controller disconnected" << endl;
}