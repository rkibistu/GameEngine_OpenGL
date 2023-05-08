#include "stdafx.h"
#include "Input.h"


#include "Utilities/utilities.h" // if you use STL, please include this line AFTER all other include


std::unordered_map<unsigned char, KeyInfo*> Input::_keys;
std::unordered_map<std::string, AxisMovement*> Input::_movementAxes;


Vector2 Input::_mouseCurrentPosition;
Vector2 Input::_mouseOldPosition;
Vector2 Input::_mouseMoveDirection ;


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
void Input::UpdateMouse(unsigned char mouseButton, unsigned int mouseEvent, int x, int y) {

	if (mouseButton != MouseButtons::None) {

		auto it = _keys.find(mouseButton);
		if (it == _keys.end()) {

			std::cout << "Key is not treated in Input class: " << mouseButton << std::endl;
			return;
		}

		KeyInfo* keyInfo = it->second;

		if (mouseEvent == MouseEvents::Down) {

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
		else if (mouseEvent == MouseEvents::Up) {
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
	

	//No button pressed -> mouse is just moved. We update positions
	if (mouseEvent == MouseEvents::MoveStart) {
		_mouseCurrentPosition = Vector2(x, y);
		_mouseMoveDirection = (_mouseOldPosition - _mouseCurrentPosition);
		_mouseOldPosition = _mouseCurrentPosition;
		//std::cout << _mouseMoveDirection.x << " " << _mouseMoveDirection.y << std::endl;
	}
	else {
		_mouseMoveDirection = Vector2();
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

float Input::GetAxisOld(std::string axis) {

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

	if (axis == "Mouse X") {

		return _mouseMoveDirection.x;
	}
	if (axis == "Mouse Y") {

		return _mouseMoveDirection.y;
	}

	return 0;
}
float Input::GetAxis(std::string axis) {

	auto it = _movementAxes.find(axis);
	if (it == _movementAxes.end())
		return 0;

	if (GetKey(it->second->PozitiveKey))
		return 1;
	if (GetKey(it->second->NegativeKey))
		return -1;
	return 0;
}

void Input::Init() {

	InitKeys();
	InitMovementAxis();
}
void Input::Destroy() {

	//keys
	for (auto it = _keys.begin(); it != _keys.end(); it++) {

		if (it->second)
			delete it->second;
	}
	_keys.clear();
}

void Input::InitKeys() {
	unsigned char c;
	KeyInfo* keyInfo;
	for (c = 'A'; c <= 'Z'; c++) {

		keyInfo = new KeyInfo();
		_keys.insert({ c,keyInfo });
	}

	//arrows
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::LEFT_ARROW,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::UP_ARROW,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::DOWN_ARROW,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::RIGHT_ARROW,keyInfo });

	//mouse
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::MOUSE_BUTTON_0,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::MOUSE_BUTTON_1,keyInfo });
	keyInfo = new KeyInfo();
	_keys.insert({ KeyCode::MOUSE_BUTTON_2,keyInfo });

}
void Input::InitMovementAxis() {

	AxisMovement* axis;

	axis = new AxisMovement();
	axis->AxisName = "Horizontal";
	axis->PozitiveKey = KeyCode::D;
	axis->NegativeKey = KeyCode::A;
	_movementAxes.insert({ axis->AxisName, axis });

	axis = new AxisMovement();
	axis->AxisName = "Depth";
	axis->PozitiveKey = KeyCode::S;
	axis->NegativeKey = KeyCode::W;
	_movementAxes.insert({ axis->AxisName, axis });

	axis = new AxisMovement();
	axis->AxisName = "Vertical";
	axis->PozitiveKey = KeyCode::Q;
	axis->NegativeKey = KeyCode::E;
	_movementAxes.insert({ axis->AxisName, axis });
}