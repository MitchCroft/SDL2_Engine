#include "StringToInputValues.hpp"

namespace SDL2_Engine {
	namespace Input {
		/*
			stringToInputType - Convert a String value to a corresponding input type
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EInputDevice value

			return EInputDevice - Returns an EInputDevice value representing the passed in String value or EInputDevice::Null if not recognised
		*/
		EInputDevice SDL2_Engine::Input::stringToInputType(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "gamepad")	return EInputDevice::GamePad;
			else if (CMP == "keyboard") return EInputDevice::Keyboard;
			else if (CMP == "mouse")	return EInputDevice::Mouse;
			else						return EInputDevice::Null;
		}

		/*
			stringToGamePadID - Convert a String value to a corresponding GamePad ID value
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadID value

			return EGamePadID - Returns an EGamePadID value representing the passed in String value or EGamePadID::Null if not recognised
		*/
		EGamePadID stringToGamePadID(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "one")		return EGamePadID::One;
			else if (CMP == "two")		return EGamePadID::Two;
			else if (CMP == "three")	return EGamePadID::Three;
			else if (CMP == "four")		return EGamePadID::Four;
			else if (CMP == "all")		return EGamePadID::All;
			else						return EGamePadID::Null;
		}

		/*
			stringToGamePadInputType - Convert a String value to a corresponding GamePad input type
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadInputType value

			return EGamePadInputType - Returns an EGamePadInputType value representing the passed in String value or EGamePadInputType::Null if not recognised
		*/
		EGamePadInputType stringToGamePadInputType(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "button")	return EGamePadInputType::Button;
			else if (CMP == "axis")		return EGamePadInputType::Axis;
			else						return EGamePadInputType::Null;
		}

		/*
			stringToGamePadAxis - Convert a String value to a corresponding GamePad axis code
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadAxisCode value

			return EGamePadAxisCode - Returns an EGamePadAxisCode value representing the passed in String value or EGamePadAxisCode::Null if not recognised
		*/
		EGamePadAxisCode stringToGamePadAxis(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "left_trigger")		return EGamePadAxisCode::Left_Trigger;
			else if (CMP == "right_trigger")	return EGamePadAxisCode::Right_Trigger;
			else if (CMP == "left_x")			return EGamePadAxisCode::Left_X;
			else if (CMP == "left_y")			return EGamePadAxisCode::Left_Y;
			else if (CMP == "right_x")			return EGamePadAxisCode::Right_X;
			else if (CMP == "right_y")			return EGamePadAxisCode::Right_Y;
			else								return EGamePadAxisCode::Null_Axis;
		}

		/*
			stringToGamePadBtn - Convert a String value to a corresponding GamePad button
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadBtnCodes value

			return EGamePadBtnCodes - Returns an EGamePadBtnCodes value representing the passed in String value or EGamePadBtnCodes::Null_Input if not recognised
		*/
		EGamePadBtnCodes stringToGamePadBtn(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "a")				return EGamePadBtnCodes::A;
			else if (CMP == "b")				return EGamePadBtnCodes::B;
			else if (CMP == "x")				return EGamePadBtnCodes::X;
			else if (CMP == "y")				return EGamePadBtnCodes::Y;
			else if (CMP == "left_shoulder")	return EGamePadBtnCodes::Left_Shoulder;
			else if (CMP == "right_shoulder")	return EGamePadBtnCodes::Right_Shoulder;
			else if (CMP == "left_thumbstick")	return EGamePadBtnCodes::Left_Thumbstick;
			else if (CMP == "right_thumbstick")	return EGamePadBtnCodes::Right_Thumbstick;
			else if (CMP == "start")			return EGamePadBtnCodes::Start;
			else if (CMP == "back")				return EGamePadBtnCodes::Back;
			else if (CMP == "dpad_up")			return EGamePadBtnCodes::DPad_Up;
			else if (CMP == "dpad_down")		return EGamePadBtnCodes::DPad_Down;
			else if (CMP == "dpad_left")		return EGamePadBtnCodes::DPad_Left;
			else if (CMP == "dpad_right")		return EGamePadBtnCodes::DPad_Right;
			else if (CMP == "any")				return EGamePadBtnCodes::Any;
			else								return EGamePadBtnCodes::Null_Input;
		}

		/*
			stringToKeyboardKey - Convert a String value to a corresponding Keyboard Key
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EKeyboardKeyCode value

			return EKeyboardKeyCode - Returns an EKeyboardKeyCode value representing the passed in String value or EKeyboardKeyCode::Null if not recognised
		*/
		EKeyboardKeyCode stringToKeyboardKey(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "space")			return EKeyboardKeyCode::Space;
			else if (CMP == "a")				return EKeyboardKeyCode::A;
			else if (CMP == "b")				return EKeyboardKeyCode::B;
			else if (CMP == "c")				return EKeyboardKeyCode::C;
			else if (CMP == "d")				return EKeyboardKeyCode::D;
			else if (CMP == "e")				return EKeyboardKeyCode::E;
			else if (CMP == "f")				return EKeyboardKeyCode::F;
			else if (CMP == "g")				return EKeyboardKeyCode::G;
			else if (CMP == "h")				return EKeyboardKeyCode::H;
			else if (CMP == "i")				return EKeyboardKeyCode::I;
			else if (CMP == "j")				return EKeyboardKeyCode::J;
			else if (CMP == "k")				return EKeyboardKeyCode::K;
			else if (CMP == "l")				return EKeyboardKeyCode::L;
			else if (CMP == "m")				return EKeyboardKeyCode::M;
			else if (CMP == "n")				return EKeyboardKeyCode::N;
			else if (CMP == "o")				return EKeyboardKeyCode::O;
			else if (CMP == "p")				return EKeyboardKeyCode::P;
			else if (CMP == "q")				return EKeyboardKeyCode::Q;
			else if (CMP == "r")				return EKeyboardKeyCode::R;
			else if (CMP == "s")				return EKeyboardKeyCode::S;
			else if (CMP == "t")				return EKeyboardKeyCode::T;
			else if (CMP == "u")				return EKeyboardKeyCode::U;
			else if (CMP == "v")				return EKeyboardKeyCode::V;
			else if (CMP == "w")				return EKeyboardKeyCode::W;
			else if (CMP == "x")				return EKeyboardKeyCode::X;
			else if (CMP == "y")				return EKeyboardKeyCode::Y;
			else if (CMP == "z")				return EKeyboardKeyCode::Z;

			else if (CMP == "num0")				return EKeyboardKeyCode::Num0;
			else if (CMP == "num1")				return EKeyboardKeyCode::Num1;
			else if (CMP == "num2")				return EKeyboardKeyCode::Num2;
			else if (CMP == "num3")				return EKeyboardKeyCode::Num3;
			else if (CMP == "num4")				return EKeyboardKeyCode::Num4;
			else if (CMP == "num5")				return EKeyboardKeyCode::Num5;
			else if (CMP == "num6")				return EKeyboardKeyCode::Num6;
			else if (CMP == "num7")				return EKeyboardKeyCode::Num7;
			else if (CMP == "num8")				return EKeyboardKeyCode::Num8;
			else if (CMP == "num9")				return EKeyboardKeyCode::Num9;
			else if (CMP == "num_pad0")			return EKeyboardKeyCode::Num_Pad0;
			else if (CMP == "num_pad1")			return EKeyboardKeyCode::Num_Pad1;
			else if (CMP == "num_pad2")			return EKeyboardKeyCode::Num_Pad2;
			else if (CMP == "num_pad3")			return EKeyboardKeyCode::Num_Pad3;
			else if (CMP == "num_pad4")			return EKeyboardKeyCode::Num_Pad4;
			else if (CMP == "num_pad5")			return EKeyboardKeyCode::Num_Pad5;
			else if (CMP == "num_pad6")			return EKeyboardKeyCode::Num_Pad6;
			else if (CMP == "num_pad7")			return EKeyboardKeyCode::Num_Pad7;
			else if (CMP == "num_pad8")			return EKeyboardKeyCode::Num_Pad8;
			else if (CMP == "num_pad9")			return EKeyboardKeyCode::Num_Pad9;

			else if (CMP == "arrow_up")			return EKeyboardKeyCode::Arrow_Up;
			else if (CMP == "arrow_down")		return EKeyboardKeyCode::Arrow_Down;
			else if (CMP == "arrow_left")		return EKeyboardKeyCode::Arrow_Left;
			else if (CMP == "arrow_right")		return EKeyboardKeyCode::Arrow_Right;

			else if (CMP == "num_pad_multiply") return EKeyboardKeyCode::Num_Pad_Multiply;
			else if (CMP == "num_pad_subtract") return EKeyboardKeyCode::Num_Pad_Subtract;
			else if (CMP == "num_pad_decimal")	return EKeyboardKeyCode::Num_Pad_Decimal;
			else if (CMP == "num_pad_divide")	return EKeyboardKeyCode::Num_Pad_Divide;

			else if (CMP == "semi_colon")		return EKeyboardKeyCode::Semi_Colon;
			else if (CMP == "plus")				return EKeyboardKeyCode::Plus;
			else if (CMP == "comma")			return EKeyboardKeyCode::Comma;
			else if (CMP == "minus")			return EKeyboardKeyCode::Minus;
			else if (CMP == "period")			return EKeyboardKeyCode::Period;
			else if (CMP == "slash_forward")	return EKeyboardKeyCode::Slash_Forward;
			else if (CMP == "slash_backward")	return EKeyboardKeyCode::Slash_Backward;
			else if (CMP == "tilde")			return EKeyboardKeyCode::Tilde;
			else if (CMP == "square_left")		return EKeyboardKeyCode::Square_Left;
			else if (CMP == "square_right")		return EKeyboardKeyCode::Square_Right;
			else if (CMP == "quote")			return EKeyboardKeyCode::Quote;

			else if (CMP == "F1")				return EKeyboardKeyCode::F1;
			else if (CMP == "F2")				return EKeyboardKeyCode::F2;
			else if (CMP == "F3")				return EKeyboardKeyCode::F3;
			else if (CMP == "F4")				return EKeyboardKeyCode::F4;
			else if (CMP == "F5")				return EKeyboardKeyCode::F5;
			else if (CMP == "F6")				return EKeyboardKeyCode::F6;
			else if (CMP == "F7")				return EKeyboardKeyCode::F7;
			else if (CMP == "F8")				return EKeyboardKeyCode::F8;
			else if (CMP == "F9")				return EKeyboardKeyCode::F9;
			else if (CMP == "F10")				return EKeyboardKeyCode::F10;
			else if (CMP == "F11")				return EKeyboardKeyCode::F11;
			else if (CMP == "F12")				return EKeyboardKeyCode::F12;
			else if (CMP == "F13")				return EKeyboardKeyCode::F13;
			else if (CMP == "F14")				return EKeyboardKeyCode::F14;
			else if (CMP == "F15")				return EKeyboardKeyCode::F15;
			else if (CMP == "F16")				return EKeyboardKeyCode::F16;
			else if (CMP == "F17")				return EKeyboardKeyCode::F17;
			else if (CMP == "F18")				return EKeyboardKeyCode::F18;
			else if (CMP == "F19")				return EKeyboardKeyCode::F19;
			else if (CMP == "F20")				return EKeyboardKeyCode::F20;
			else if (CMP == "F21")				return EKeyboardKeyCode::F21;
			else if (CMP == "F22")				return EKeyboardKeyCode::F22;
			else if (CMP == "F23")				return EKeyboardKeyCode::F23;
			else if (CMP == "F24")				return EKeyboardKeyCode::F24;

			else if (CMP == "escape")			return EKeyboardKeyCode::Escape;
			else if (CMP == "backspace")		return EKeyboardKeyCode::Backspace;
			else if (CMP == "tab")				return EKeyboardKeyCode::TAB;
			else if (CMP == "return")			return EKeyboardKeyCode::Return;
			else if (CMP == "shift")			return EKeyboardKeyCode::Shift;
			else if (CMP == "shift_left")		return EKeyboardKeyCode::Shift_Left;
			else if (CMP == "shift_right")		return EKeyboardKeyCode::Shift_Right;
			else if (CMP == "control")			return EKeyboardKeyCode::Control;
			else if (CMP == "control_left")		return EKeyboardKeyCode::Control_Left;
			else if (CMP == "control_right")	return EKeyboardKeyCode::Control_Right;
			else if (CMP == "alt")				return EKeyboardKeyCode::Alt;
			else if (CMP == "alt_left")			return EKeyboardKeyCode::Alt_Left;
			else if (CMP == "alt_right")		return EKeyboardKeyCode::Alt_Right;
			else if (CMP == "windows_left")		return EKeyboardKeyCode::Windows_Left;
			else if (CMP == "windows_right")	return EKeyboardKeyCode::Windows_Right;
			else if (CMP == "apps")				return EKeyboardKeyCode::Apps;
			else if (CMP == "page_up")			return EKeyboardKeyCode::Page_Up;
			else if (CMP == "page_down")		return EKeyboardKeyCode::Page_Down;
			else if (CMP == "end")				return EKeyboardKeyCode::End;
			else if (CMP == "home")				return EKeyboardKeyCode::Home;
			else if (CMP == "cancel")			return EKeyboardKeyCode::Cancel;
			else if (CMP == "clear")			return EKeyboardKeyCode::Clear;
			else if (CMP == "select")			return EKeyboardKeyCode::Select;
			else if (CMP == "print")			return EKeyboardKeyCode::Print;
			else if (CMP == "execute")			return EKeyboardKeyCode::Execute;
			else if (CMP == "print_screen")		return EKeyboardKeyCode::Print_Screen;
			else if (CMP == "insert")			return EKeyboardKeyCode::Insert;
			else if (CMP == "delete")			return EKeyboardKeyCode::Delete;
			else if (CMP == "help")				return EKeyboardKeyCode::Help;
			else if (CMP == "sleep")			return EKeyboardKeyCode::Sleep;

			else if (CMP == "caps_lock")		return EKeyboardKeyCode::Caps_Lock;
			else if (CMP == "num_lock")			return EKeyboardKeyCode::Num_Lock;
			else if (CMP == "scroll_lock")		return EKeyboardKeyCode::Scroll_Lock;

			else								return EKeyboardKeyCode::Null;
		}

		/*
			stringToMouseInputType - Convert a String value to a corresponding Mouse Input type
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EMouseInputType value

			return EMouseInputType - Returns an EMouseInputType value representing the passed in String value or EMouseInputType::Null if not recognised
		*/
		EMouseInputType stringToMouseInputType(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "button")	return EMouseInputType::Button;
			else if (CMP == "axis")		return EMouseInputType::Axis;
			else						return EMouseInputType::Null;
		}

		/*
			stringToMouseButton - Convert a String value to a corresponding Mouse Button
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EMouseButton value

			return EMouseButton - Returns an EMouseButton value representing the passed in String value or EMouseButton::Null if not recognised
		*/
		EMouseButton stringToMouseButton(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "left")		return EMouseButton::Left;
			else if (CMP == "right")	return EMouseButton::Right;
			else if (CMP == "middle")	return EMouseButton::Middle;
			else if (CMP == "extra1")	return EMouseButton::Extra1;
			else if (CMP == "extra2")	return EMouseButton::Extra2;
			else						return EMouseButton::Null;
		}

		/*
			stringToMouseAxis - Convert a String value to a corresponding Mouse Axis
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EMouseAxis value

			return EMouseAxis - Returns an EMouseButton value representing the passed in String value or EMouseAxis::Null if not recognised
		*/
		EMouseAxis stringToMouseAxis(const compareStr& pString) {
			//Convert the String to lowercase
			const compareStr CMP = pString.toLower();

			//Check the values
			if		(CMP == "x") return EMouseAxis::X;
			else if (CMP == "y") return EMouseAxis::Y;
			else				 return EMouseAxis::Null;
		}
	}
}
