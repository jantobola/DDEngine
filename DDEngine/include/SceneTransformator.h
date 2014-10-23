#pragma once

#include "DDERenderPackage.h"
#include <DirectXMath.h>
#include <AntTweakBar/AntTweakBar.h>
#include <string>
#include <vector>

namespace DDEngine {

	class Object3D;

	namespace SceneTransformatorStatic {

		struct Picker
		{
			int index;
			Object3D* object;
			std::string name;
			bool picked = false;
		};

		static float scaleCheck = 0.0f;
		static float scaleStep = 0.020f;
		static float transStep = 0.020f;
		static int selectedObject = -1;

		static DirectX::XMFLOAT4 rotationQuaternions = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		static DirectX::XMFLOAT3 translationVector = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		static DirectX::XMFLOAT4 scaleVector = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		
		static std::vector<Picker> selections;
		static std::vector<Object3D*> objects;

		static void updateSelection(const int index);
		static void selectObject(const int index);
	}

	class SceneTransformator
	{

		private:

			TwBar* transformBar = nullptr;
			TwBar* pickBar = nullptr;

			bool enableTransformFlag = false;

		public:

			SceneTransformator();
			~SceneTransformator();

			void setEnabledTransformations(const bool isEnabled);
			bool isEnabledTransformations() { return enableTransformFlag; }

			void selectObject(const int index);
			void setObjects(const std::vector<Object3D*> objects);

			void initBar(const int width, const int height);
			void transform(const DDERenderPackage pkg);

			static void TW_CALL SetScaleCallback(const void *value, void *clientData);
			static void TW_CALL GetScaleCallback(void *value, void *clientData);
			
			static void TW_CALL SetScaleXCallback(const void *value, void *clientData);
			static void TW_CALL GetScaleXCallback(void *value, void *clientData);

			static void TW_CALL SetScaleYCallback(const void *value, void *clientData);
			static void TW_CALL GetScaleYCallback(void *value, void *clientData);

			static void TW_CALL SetScaleZCallback(const void *value, void *clientData);
			static void TW_CALL GetScaleZCallback(void *value, void *clientData);

			static void TW_CALL SetScaleStepCallback(const void *value, void *clientData);
			static void TW_CALL GetScaleStepCallback(void *value, void *clientData);

			static void TW_CALL SetTransStepCallback(const void *value, void *clientData);
			static void TW_CALL GetTransStepCallback(void *value, void *clientData);

			static void TW_CALL SetPickerCallback(const void *value, void *clientData);
			static void TW_CALL GetPickerCallback(void *value, void *clientData);

	};

}