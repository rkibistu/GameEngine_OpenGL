#include "stdafx.h"
#include "Input.h"




std::unordered_map<unsigned char, KeyInfo*> Input::_keys;





void Input::Update() {

	//this is called every frame. We reset the KeyDown and KeyUp because this should be active only one frame

	for (auto it = _keys.begin(); it != _keys.end(); it++) {

		it->second->KeyDown = false;
		it->second->KeyUp = false;
	}
}
void Input::UpdateKey(unsigned char key, bool bIsPressed) {

	auto it = _keys.find(key);
	if (it == _keys.end()) {

		std::cout << "Key is not treated in Input class: " << key << std::endl;
		return;
	}

	KeyInfo* keyInfo = it->second;

	if (bIsPressed) {
		if (keyInfo->KeyHold == false) {
			keyInfo->KeyDown = true;
			keyInfo->KeyHold = true;
			keyInfo->KeyUp = false;
		}
		else {
			keyInfo->KeyDown = false;
			keyInfo->KeyHold = true;
			keyInfo->KeyUp = false;
		}
	}
	else {
		if (keyInfo->KeyHold == true) {
			keyInfo->KeyDown = false;
			keyInfo->KeyHold = false;
			keyInfo->KeyUp = true;
		}
		else {
			keyInfo->KeyDown = false;
			keyInfo->KeyHold = false;
			keyInfo->KeyUp = false;
		}

	}

}

bool Input::GetKey(unsigned char key) {

	auto it = _keys.find(key);
	if (it == _keys.end()) {

		std::cout << "Key is not treated in Input class: " << key << std::endl;
		return false;
	}

	return it->second->KeyHold;
}
bool Input::GetKeyDown(unsigned char key) {

	auto it = _keys.find(key);
	if (it == _keys.end()) {

		std::cout << "Key is not treated in Input class: " << key << std::endl;
		return false;
	}

	return it->second->KeyDown;
}
bool Input::GetKeyUp(unsigned char key) {

	auto it = _keys.find(key);
	if (it == _keys.end()) {

		std::cout << "Key is not treated in Input class: " << key << std::endl;
		return false;
	}

	return it->second->KeyUp;
}

float Input::GetAxis(std::string axis) {

	if (axis == "Horizontal") {

		if (GetKey(KeyCode::A)) {
			return -1;
		}
		if (GetKey(KeyCode::D)) {
			return 1;
		}
		return 0;
	}

	if (axis == "Vertical") {

		if (GetKey(KeyCode::E)) {
			return -1;
		}
		if (GetKey(KeyCode::Q)) {
			return 1;
		}
		return 0;
	}

	if (axis == "Depth") {

		if (GetKey(KeyCode::W)) {
			return -1;
		}
		if (GetKey(KeyCode::S)) {
			return 1;
		}
		return 0;
	}
}

void Input::Init() {

	unsigned char c;
	KeyInfo* keyInfo;
	for (c = 'A'; c <= 'Z'; c++) {

		keyInfo = new KeyInfo();
		_keys.insert({ c,keyInfo });
	}

	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::LEFT_ARROW,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::UP_ARROW,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::DOWN_ARROW,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::RIGHT_ARROW,keyInfo });
}