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

private:
	Input() { ; }
	Input(const Input& other) = delete;
	Input& operator=(const Input& other) = delete;


private:

	static std::unordered_map<unsigned char, KeyInfo*> _keys;
};