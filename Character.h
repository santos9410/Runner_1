#pragma once
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
using namespace Urho3D;

class Character : public LogicComponent
{
public:
	/// Construct.
	explicit Character(Context* context, Scene* scene_);

	Node* CreateCharacter(Vector3 position, float scale);

private:
	Scene* scene_;

};