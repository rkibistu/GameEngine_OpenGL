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

	static Input& GetInstance();
	void DestroyInstance();

	void Update();
	void UpdateKey(unsigned char key, bool bIsPressed);

	bool GetKey(unsigned char key);
	bool GetKeyDown(unsigned char key);
	bool GetKeyUp(unsigned char key);

private:
	Input();
	Input(const Input& other) = delete;
	Input& operator=(const Input& other) = delete;

	void InitKeys();

private:

	static Input* _spInstance;

	std::unordered_map<unsigned char, KeyInfo*> _keys;
};