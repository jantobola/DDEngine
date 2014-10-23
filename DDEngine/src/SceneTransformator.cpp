#include "SceneTransformator.h"
#include "ShaderHolder.h"
#include "RenderContext.h"
#include "ResourceProvider.h"
#include "DDEUtils.h"
#include <Object3D.h>
#include <d3d11.h>

using namespace DirectX;
using namespace DDEngine::SceneTransformatorStatic;

DDEngine::SceneTransformator::SceneTransformator()
{

}

DDEngine::SceneTransformator::~SceneTransformator()
{

}

void DDEngine::SceneTransformator::transform(const DDERenderPackage pkg)
{
	if (selectedObject > -1 && enableTransformFlag) {

		Object3D* o = objects.at(selectedObject);

		XMMATRIX rot = XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotationQuaternions));
		XMMATRIX scale = XMMatrixScalingFromVector(DirectX::XMLoadFloat4(&scaleVector));
		XMMATRIX translation = XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&translationVector));

		XMFLOAT4X4 rotMat;
		XMFLOAT4X4 scaleMat;
		XMFLOAT4X4 transMat;

		DirectX::XMStoreFloat4x4(&rotMat, rot);
		DirectX::XMStoreFloat4x4(&scaleMat, scale);
		DirectX::XMStoreFloat4x4(&transMat, translation);

		o->setRotationMatrix(rotMat);
		o->setScaleMatrix(scaleMat);
		o->setTranslationMatrix(transMat);
	}
}

void DDEngine::SceneTransformator::selectObject(const int index)
{
	SceneTransformatorStatic::selectObject(index);
}

void DDEngine::SceneTransformatorStatic::updateSelection(const int index)
{
	for (size_t i = 0; i < selections.size(); i++) {
		Picker* p = &selections[i];
		Object3D* o = objects[i];
		if (index == i) { 
			p->picked = true;
			o->selected = true;
		} else {
			p->picked = false;
			o->selected = false;
		}
	}
}

void DDEngine::SceneTransformatorStatic::selectObject(const int index)
{
	SceneTransformatorStatic::selectedObject = index;
	updateSelection(index);
	Object3D* o = objects.at(index);

	XMVECTOR rotationQuat;
	XMVECTOR scaleVec;
	XMVECTOR translateVec;

	XMMatrixDecompose(&scaleVec, &rotationQuat, &translateVec, o->getWorldMatrix());

	XMStoreFloat4(&rotationQuaternions, rotationQuat);
	XMStoreFloat4(&scaleVector, scaleVec);
	XMStoreFloat3(&translationVector, translateVec);

	scaleVector.w = 0.0f;
}

void TW_CALL DDEngine::SceneTransformator::SetScaleCallback(const void *value, void *clientData)
{
	XMFLOAT4* scale = static_cast<XMFLOAT4*>(clientData);
	float val = *static_cast<const float *>(value);
	float scaleRatio = 1.0f;
	
	if (val >= scaleCheck) {
		scaleRatio = 1.0f + scaleStep;
	} else {
		scaleRatio = 1.0f - scaleStep;
	}

	XMVECTOR vec = XMLoadFloat4(scale);
	XMMATRIX mat = XMMatrixScaling(scaleRatio, scaleRatio, scaleRatio);
	XMMATRIX mat2 = XMMatrixScalingFromVector(vec);

	XMMATRIX res = mat * mat2;
	XMVECTOR scl, tmp1, tmp2;
	XMMatrixDecompose(&scl, &tmp1, &tmp2, res);

	XMFLOAT3 nw;
	XMStoreFloat3(&nw, scl);

	scaleCheck = val;

	scale->x = nw.x;
	scale->y = nw.y;
	scale->z = nw.z;
	scale->w = scaleCheck;

}

void DDEngine::SceneTransformator::setObjects(const std::vector<Object3D*> objects)
{
	SceneTransformatorStatic::objects = objects;

	for (size_t i = 0; i < objects.size(); i++) {
		Object3D* o = objects[i];
		Picker item;
		item.index = i;
		item.name = o->getName();
		selections.push_back(item);
	}

}



// --------------------- AntTweakBar CALLBACKS & settings ---------------------------



void TW_CALL DDEngine::SceneTransformator::GetScaleCallback(void *value, void *clientData)
{
	*static_cast<float *>(value) = static_cast<const XMFLOAT4*>(clientData)->w;
}

void TW_CALL DDEngine::SceneTransformator::SetScaleXCallback(const void *value, void *clientData)
{
	XMFLOAT4* scale = static_cast<XMFLOAT4*>(clientData);
	float val = *static_cast<const float *>(value);
	scale->x = val;
	scale->w = 1.0f;
}

void TW_CALL DDEngine::SceneTransformator::GetScaleXCallback(void *value, void *clientData)
{
	*static_cast<float *>(value) = static_cast<const XMFLOAT4*>(clientData)->x;
}

void TW_CALL DDEngine::SceneTransformator::SetScaleYCallback(const void *value, void *clientData)
{
	XMFLOAT4* scale = static_cast<XMFLOAT4*>(clientData);
	float val = *static_cast<const float *>(value);
	scale->y = val;
	scale->w = 1.0f;
}

void TW_CALL DDEngine::SceneTransformator::GetScaleYCallback(void *value, void *clientData)
{
	*static_cast<float *>(value) = static_cast<const XMFLOAT4*>(clientData)->y;
}

void TW_CALL DDEngine::SceneTransformator::SetScaleZCallback(const void *value, void *clientData)
{
	XMFLOAT4* scale = static_cast<XMFLOAT4*>(clientData);
	float val = *static_cast<const float *>(value);
	scale->z = val;
	scale->w = 1.0f;
}

void TW_CALL DDEngine::SceneTransformator::GetScaleZCallback(void *value, void *clientData)
{
	*static_cast<float *>(value) = static_cast<const XMFLOAT4*>(clientData)->z;
}

void TW_CALL DDEngine::SceneTransformator::SetScaleStepCallback(const void *value, void *clientData)
{
	float val = *static_cast<const float *>(value);
	*static_cast<float*>(clientData) = val;

	TwDefine(std::string("DDE_transform_bar/'Scale XYZ' step=" + std::to_string(val)).c_str());
	TwDefine(std::string("DDE_transform_bar/'Scale X' step=" + std::to_string(val)).c_str());
	TwDefine(std::string("DDE_transform_bar/'Scale Y' step=" + std::to_string(val)).c_str());
	TwDefine(std::string("DDE_transform_bar/'Scale Z' step=" + std::to_string(val)).c_str());
}

void TW_CALL DDEngine::SceneTransformator::GetScaleStepCallback(void *value, void *clientData)
{
	*static_cast<float *>(value) = *static_cast<const float*>(clientData);
}

void TW_CALL DDEngine::SceneTransformator::SetTransStepCallback(const void *value, void *clientData)
{
	float val = *static_cast<const float *>(value);
	*static_cast<float*>(clientData) = val;

	TwDefine(std::string("DDE_transform_bar/'Translation X' step=" + std::to_string(val)).c_str());
	TwDefine(std::string("DDE_transform_bar/'Translation Y' step=" + std::to_string(val)).c_str());
	TwDefine(std::string("DDE_transform_bar/'Translation Z' step=" + std::to_string(val)).c_str());
}

void TW_CALL DDEngine::SceneTransformator::GetTransStepCallback(void *value, void *clientData)
{
	*static_cast<float *>(value) = *static_cast<const float*>(clientData);
}

void TW_CALL DDEngine::SceneTransformator::SetPickerCallback(const void *value, void *clientData)
{
	Picker* picker = static_cast<Picker*>(clientData);
	SceneTransformatorStatic::selectObject(picker->index);
}

void TW_CALL DDEngine::SceneTransformator::GetPickerCallback(void *value, void *clientData)
{
	*static_cast<bool *>(value) = static_cast<const Picker*>(clientData)->picked;
}

void DDEngine::SceneTransformator::setEnabledTransformations(const bool isEnabled)
{
	this->enableTransformFlag = isEnabled;

	if (isEnabled) {
		TwDefine(" DDE_transform_bar visible=true ");
		TwDefine(" DDE_pick_bar visible=true ");
	}
	else {
		TwDefine(" DDE_transform_bar visible=false ");
		TwDefine(" DDE_pick_bar visible=false ");
		updateSelection(-1);
	}
}

void DDEngine::SceneTransformator::initBar(const int width, const int height)
{
	transformBar = TwNewBar("DDE_transform_bar");

	TwDefine(" DDE_transform_bar size='290 530' ");
	TwDefine(" DDE_transform_bar resizable=false label='Transform operations' ");
	TwDefine(" DDE_transform_bar valueswidth=100");
	AntUtils::setAppearance("DDE_transform_bar");

	int barPos[2] = { width - 290 - 10, 10 };
	TwSetParam(transformBar, nullptr, "position", TW_PARAM_INT32, 2, &barPos);

	std::string scaleCommand = "step=" + std::to_string(scaleStep) +" group=Scale";

	TwAddVarCB(transformBar, "Scale XYZ", TW_TYPE_FLOAT, SceneTransformator::SetScaleCallback, SceneTransformator::GetScaleCallback, &scaleVector, scaleCommand.c_str());
	TwAddSeparator(transformBar, NULL, "group=Scale");
	TwAddVarCB(transformBar, "Scale X", TW_TYPE_FLOAT, SceneTransformator::SetScaleXCallback, SceneTransformator::GetScaleXCallback, &scaleVector, std::string("min=0 " + scaleCommand).c_str());
	TwAddVarCB(transformBar, "Scale Y", TW_TYPE_FLOAT, SceneTransformator::SetScaleYCallback, SceneTransformator::GetScaleYCallback, &scaleVector, std::string("min=0 " + scaleCommand).c_str());
	TwAddVarCB(transformBar, "Scale Z", TW_TYPE_FLOAT, SceneTransformator::SetScaleZCallback, SceneTransformator::GetScaleZCallback, &scaleVector, std::string("min=0 " + scaleCommand).c_str());
	TwAddSeparator(transformBar, NULL, "group=Scale");
	TwAddVarCB(transformBar, "Scale Step", TW_TYPE_FLOAT, SceneTransformator::SetScaleStepCallback, SceneTransformator::GetScaleStepCallback, &scaleStep, "min=0 step=0.001 group=Scale");

	TwAddSeparator(transformBar, NULL, NULL);

	TwAddVarRW(transformBar, "Rotation XYZ", TW_TYPE_QUAT4F, &rotationQuaternions, "opened=true showval=true group=Rotation");
	TwAddSeparator(transformBar, NULL, "group=Rotation");
	TwAddVarRW(transformBar, "Rotation X", TW_TYPE_QUAT4F, &rotationQuaternions, "group=Rotation");
	TwAddVarRW(transformBar, "Rotation Y", TW_TYPE_QUAT4F, &rotationQuaternions, "group=Rotation");
	TwAddVarRW(transformBar, "Rotation Z", TW_TYPE_QUAT4F, &rotationQuaternions, "group=Rotation");

	TwDefine(" DDE_transform_bar/'Rotation XYZ' axisx=x axisy=y axisz=-z ");
	TwDefine(" DDE_transform_bar/'Rotation X' arrow='1 0 0' axisx=x axisy=y axisz=-z ");
	TwDefine(" DDE_transform_bar/'Rotation Y' arrow='0 1 0' axisx=x axisy=y axisz=-z ");
	TwDefine(" DDE_transform_bar/'Rotation Z' arrow='0 0 1' axisx=x axisy=y axisz=-z ");

	TwAddSeparator(transformBar, NULL, NULL);

	std::string transCommand = "step=" + std::to_string(transStep) + " group=Translation";

	TwAddVarRW(transformBar, "Translation X", TW_TYPE_FLOAT, &translationVector.x, transCommand.c_str());
	TwAddVarRW(transformBar, "Translation Y", TW_TYPE_FLOAT, &translationVector.y, transCommand.c_str());
	TwAddVarRW(transformBar, "Translation Z", TW_TYPE_FLOAT, &translationVector.z, transCommand.c_str());
	TwAddSeparator(transformBar, NULL, "group=Translation");
	TwAddVarCB(transformBar, "Translation Step", TW_TYPE_FLOAT, SceneTransformator::SetTransStepCallback, SceneTransformator::GetTransStepCallback, &transStep, "min=0 step=0.001 group=Translation");


	// -----------------------------------------------------


	pickBar = TwNewBar("DDE_pick_bar");

	TwDefine(" DDE_pick_bar size='290 300' ");
	TwDefine(" DDE_pick_bar resizable=false label='Registered objects' ");
	TwDefine(" DDE_pick_bar valueswidth=5");
	AntUtils::setAppearance("DDE_pick_bar");

	int pickerPos[2] = { width - 290 - 10, 550 };
	TwSetParam(pickBar, nullptr, "position", TW_PARAM_INT32, 2, &pickerPos);

	for (size_t i = 0; i < selections.size(); i++) {
		Picker* item = &selections[i];
		TwAddVarCB(pickBar, item->name.c_str(), TW_TYPE_BOOLCPP, SceneTransformator::SetPickerCallback, SceneTransformator::GetPickerCallback, item, "");
	}

	updateSelection(-1);

}