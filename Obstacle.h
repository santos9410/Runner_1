#pragma once
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
using namespace Urho3D;

class Obstacle : public LogicComponent
{
public:
	/// Construct.
	explicit Obstacle(Context* context, Scene* scene_);

	Node* CreateObstacle(Vector3 position);

private:
	Scene * scene_;

};