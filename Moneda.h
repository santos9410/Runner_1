#pragma once
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Urho2D/Sprite2D.h>
using namespace Urho3D;
class Moneda : public LogicComponent {

public:
	/// Construct.
	explicit Moneda(Context* context, Scene* scene_);
	Node* Create(Vector3 position);
private:
	Scene * scene_;
	Sprite2D* spriteCoin;
	float scaleX;
	float scaleY;
	int lastX = 0;
	
};