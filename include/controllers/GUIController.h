#pragma once
#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include "systems/KeyboardInputSystem.h"
#include "systems/MouseInputSystem.h"

namespace Sigma {
	class WebGUIView;
	namespace event {
		namespace handler {
			// A type of handler. This handler controls an OpenGL 6 DOF view.
			class GUIController : public IKeyboardEventHandler, public IMouseEventHandler{
			public:
				GUIController();
				void SetGUI(WebGUIView* gui) {
					this->gui = gui;
				}

				/**
				 * \brief Triggered whenever a key state change event happens
				 *
				 * This method adjusts the view mover according to various key state changes.
				 * \param key The key for which the state change is happening
				 * \param state The new state of the key (KS_UP or KS_DOWN)
				 * \return void
				 */
				void KeyStateChange(const unsigned int key, const KEY_STATE state);

				void CharDown(const unsigned int c);

				/**
				 * \brief Handles a change in mouse position.
				 *
				 * \param[in/out] float dx, dy The change in mouse position.
				 * \param[in/out] float dy
				 */
				virtual void MouseMove(float x, float y, float dx, float dy);

				// Not used but required to implement.
				virtual void MouseDown(Sigma::event::BUTTON btn, float x, float y);
				virtual void MouseUp(Sigma::event::BUTTON btn, float x, float y);
			private:
				WebGUIView* gui;
				bool hasFocus;
			};
		}
	}
}

#endif // GLFPSCONTROLLER_H

