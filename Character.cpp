#include "Character.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionCircle2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>

Character::Character(Context * context, Scene * scene) : LogicComponent(context)
{
	scene_ = scene;
}

Node * Character::CreateCharacter(Vector3 position, float scale)
{
	auto* cache = GetSubsystem<ResourceCache>();
	Node* spriteNode = scene_->CreateChild("Imp");
	spriteNode->SetPosition(position);
	spriteNode->SetScale(scale);
	auto* animatedSprite = spriteNode->CreateComponent<AnimatedSprite2D>();
	// Get scml file and Play "idle" anim
	auto* animationSet = cache->GetResource<AnimationSet2D>("imp/imp.scml");
	animatedSprite->SetAnimationSet(animationSet);
	animatedSprite->SetAnimation("idle");
	animatedSprite->SetLayer(3); // Put character over tile map (which is on layer 0) and over Orcs (which are on layer 2)
	animatedSprite->SetFlipX(true);

	auto* impBody = spriteNode->CreateComponent<RigidBody2D>();
	impBody->SetBodyType(BT_DYNAMIC);
	impBody->SetAllowSleep(false);
	auto* shape = spriteNode->CreateComponent<CollisionCircle2D>();
	shape->SetRadius(1.0f); // Set shape size
	shape->SetFriction(0.8f); // Set friction
	shape->SetRestitution(0.0f); // Bounce
	impBody->SetGravityScale(2.0f);
	return spriteNode;
}
