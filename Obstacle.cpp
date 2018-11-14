#include "Obstacle.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionCircle2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
Obstacle::Obstacle(Context * context, Scene * scene) : LogicComponent(context)
{
	scene_ = scene;
}

Node * Obstacle::CreateObstacle(Vector3 position)
{
	auto* cache = GetSubsystem<ResourceCache>();
	Node* node = scene_->CreateChild("item1");
	node->SetPosition(position);
	//Vector3(14.0f, -4.2f, -1.0f)

	node->SetScale(Vector3(1.1, 1.1, 1));

	auto* staticSprite = node->CreateComponent<StaticSprite2D>();
	
	auto* spriteS = cache->GetResource<Sprite2D>("Images/pinchos.png");
	staticSprite->SetSprite(spriteS);

	auto* impBody = node->CreateComponent<RigidBody2D>();
	impBody->SetBodyType(BT_KINEMATIC);
	impBody->SetAllowSleep(false);
	auto* shape = node->CreateComponent<CollisionCircle2D>();
	shape->SetRadius(0.5f); // Set shape size
	//
	
	return node;
}
