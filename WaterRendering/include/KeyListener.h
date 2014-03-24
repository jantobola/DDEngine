#pragma once

#include <DDEngine.h>

class CustomRenderer;

class KeyListener : public DDEngine::InputHandler {

	private:
		bool tweakBarHidden = true;
		CustomRenderer* renderer = nullptr;

	public:
		KeyListener(CustomRenderer* renderer);
		~KeyListener() { }
	
		virtual void onKeyDownAsync() override;
		virtual void onKeyTyped(WPARAM wParam) override;
};
