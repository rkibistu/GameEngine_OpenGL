#pragma once

#include <unordered_map>



struct KeyInfo {

	bool KeyDown;
	bool KeyUp;
	bool KeyHold;

	KeyInfo() 
		: KeyDown(false), KeyUp(false), KeyHold(false) {

	}
};

class Input {
	
public:

	//called at the start of the applicaition
	static void Init();

	//called once every frame, at the end of the frame (to reset values)
	static void Update();


	//called when a key is pressed/released
	static void UpdateKey(unsigned char key, bool bIsPressed);


	//true if key is held down
	static bool GetKey(unsigned char key);

	//true one frame when key is pressed
	static bool GetKeyDown(unsigned char key);

	//true one frame when key is released
	static bool GetKeyUp(unsigned char key);


	static float GetAxis(std::string axis);

private:
	Input() { ; }
	Input(const Input& other) = delete;
	Input& operator=(const Input& other) = delete;


private:

	static std::unordered_map<unsigned char, KeyInfo*> _keys;
};

static class KeyCode {

public:
	static const unsigned char A = 'A';
	static const unsigned char B = 'B';
	static const unsigned char C = 'C';
	static const unsigned char D = 'D';
	static const unsigned char E = 'E';
	static const unsigned char F = 'F';
	static const unsigned char G = 'G';
	static const unsigned char H = 'H';
	static const unsigned char I = 'I';
	static const unsigned char J = 'J';
	static const unsigned char K = 'K';
	static const unsigned char L = 'L';
	static const unsigned char M = 'M';
	static const unsigned char N = 'N';
	static const unsigned char O = 'O';
	static const unsigned char P = 'P';
	static const unsigned char Q = 'Q';
	static const unsigned char R = 'R';
	static const unsigned char S = 'S';
	static const unsigned char T = 'T';
	static const unsigned char U = 'U';
	static const unsigned char V = 'V';
	static const unsigned char W = 'W';
	static const unsigned char X = 'X';
	static const unsigned char Y = 'Y';
	static const unsigned char Z = 'Z';

};
