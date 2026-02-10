#pragma once
#include "gameLogic.h"
#include <string>



namespace platform
{
	struct Button
	{
		char pressed = 0;
		char held = 0;
		char released = 0;
		char newState = -1; // this can be -1, used for internal logic
		char typed = 0;
		float typedTime = 0;


		enum
		{
			A = 0,
			B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			NR0, NR1, NR2, NR3, NR4, NR5, NR6, NR7, NR8, NR9,
			Space,
			Enter,
			Escape,
			Up,
			Down,
			Left,
			Right,
			LeftCtrl,
			Tab,
			BUTTONS_COUNT, //
		};

		void merge(const Button& b)
		{
			this->pressed |= b.pressed;
			this->released |= b.released;
			this->held |= b.held;
		}
	};

	namespace internal
	{
		inline void resetButtonToZero(Button& b)
		{
			b.pressed = 0;
			b.held = 0;
			b.released = 0;
		}
	}



	//Button::key
	int isButtonHeld(int key);
	int isButtonPressedOn(int key);
	int isButtonReleased(int key);
	int isButtonTyped(int key);

	int isLMousePressed();
	int isRMousePressed();

	int isLMouseReleased();
	int isRMouseReleased();

	int isLMouseHeld();
	int isRMouseHeld();

	std::string getTypedInput();

	namespace internal
	{

		void setButtonState(int button, int newState);

		void setLeftMouseState(int newState);
		void setRightMouseState(int newState);

		inline void processEventButton(Button& b, bool newState)
		{
			b.newState = newState;
		}

		inline void updateButton(Button& b, float deltaTime)
		{
			if (b.newState == 1)
			{
				if (b.held)
				{
					b.pressed = false;
				}
				else
				{
					b.pressed = true;
				}

				b.held = true;
				b.released = false;
			}
			else if (b.newState == 0)
			{
				b.held = false;
				b.pressed = false;
				b.released = true;
			}
			else
			{
				b.pressed = false;
				b.released = false;
			}

			//processing typed
			if (b.pressed)
			{
				b.typed = true;
				b.typedTime = 0.48f;
			}
			else if (b.held)
			{
				b.typedTime -= deltaTime;

				if (b.typedTime < 0.f)
				{
					b.typedTime += 0.07f;
					b.typed = true;
				}
				else
				{
					b.typed = false;
				}

			}
			else
			{
				b.typedTime = 0;
				b.typed = false;
			}


			b.newState = -1;
		}


		void updateAllButtons(float deltaTime);
		void resetInputsToZero();

		void addToTypedInput(char c);
		void resetTypedInput();

	};

};