#pragma once

class KeyToggle {
public:
	KeyToggle(int key) :mKey(key), mActive(false) {}
	operator bool() {
		if (GetAsyncKeyState(mKey)) {
			if (!mActive) {
				mActive = true;
				return true;
			}
		}
		else
			mActive = false;
		return false;
	}
private:
	int mKey;
	bool mActive;
};

enum KeyPress
{
	VK_A = 0x41,
	VK_B = 0x42,
	VK_C = 0x43,
	VK_D = 0x44,
	VK_E = 0x45,
	VK_F = 0x46,
	VK_G = 0x47,
	VK_H = 0x48,
	VK_I = 0x49,
	VK_J = 0x4A,
	VK_K = 0x4B,
	VK_L = 0x4C,
	VK_M = 0x4D,
	VK_N = 0x4E,
	VK_O = 0x4F,
	VK_P = 0x50,
	VK_Q = 0x51,
	VK_R = 0x52,
	VK_S = 0x53,
	VK_T = 0x54,
	VK_U = 0x55,
	VK_V = 0x56,
	VK_W = 0x57,
	VK_X = 0x58,
	VK_Y = 0x59,
	VK_Z = 0x5A,
	VK_0 = 0x30,
	VK_1 = 0x31,
	VK_2 = 0x32,
	VK_3 = 0x33,
	VK_4 = 0x34,
	VK_5 = 0x35,
	VK_6 = 0x36,
	VK_7 = 0x37,
	VK_8 = 0x38,
	VK_9 = 0x39,
	NUM0 = 0x60,
	NUM1 = 0x61,
	NUM2 = 0x62,
	NUM3 = 0x63,
	NUM4 = 0x64,
	NUM5 = 0x65,
	NUM6 = 0x66,
	NUM7 = 0x67,
	NUM8 = 0x68,
	NUM9 = 0x69,
	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,
};

KeyToggle IsKeyPressed_F1(KeyPress::F1);
KeyToggle IsKeyPressed_F2(KeyPress::F2);
KeyToggle IsKeyPressed_F3(KeyPress::F3);
KeyToggle IsKeyPressed_F4(KeyPress::F4);
KeyToggle IsKeyPressed_F5(KeyPress::F5);
KeyToggle IsKeyPressed_F6(KeyPress::F6);
KeyToggle IsKeyPressed_F7(KeyPress::F7);
KeyToggle IsKeyPressed_F8(KeyPress::F8);
KeyToggle IsKeyPressed_F9(KeyPress::F9);
KeyToggle IsKeyPressed_F10(KeyPress::F10);
KeyToggle IsKeyPressed_F11(KeyPress::F11);
KeyToggle IsKeyPressed_F12(KeyPress::F12);
KeyToggle IsKeyPressed_NUMPAD0(KeyPress::NUM0);
KeyToggle IsKeyPressed_NUMPAD1(KeyPress::NUM1);
KeyToggle IsKeyPressed_W(KeyPress::VK_W);
KeyToggle IsKeyPressed_S(KeyPress::VK_S);
KeyToggle IsKeyPressed_D(KeyPress::VK_D);
KeyToggle IsKeyPressed_A(KeyPress::VK_A);