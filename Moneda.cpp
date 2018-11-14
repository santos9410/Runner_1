#include "Moneda.h"
#include <Urho3D/Urho2D/StaticSprite2D.h>

#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>


Moneda::Moneda(Context * context, Scene * scene2_) : LogicComponent(context)
{
	scene_ = scene2_;
	auto* cache = GetSubsystem<ResourceCache>();
	spriteCoin = cache->GetResource<Sprite2D>("Images/coin.png");
	
	auto rect = spriteCoin->GetRectangle();
	scaleX = PIXEL_SIZE * rect.Width();
	scaleY = PIXEL_SIZE * rect.Height();
}

Node * Moneda::Create(Vector3 position)
{
	Node* node_ = scene_->CreateChild("Coins");

	node_->SetPosition(position);
	node_->SetScale(Vector3(1, 1, 1));

	/*auto* staticSprite = node_->CreateComponent<StaticSprite2D>();
	staticSprite->SetFlipY(true);

	staticSprite->SetSprite(spriteCoin);*/

	// Get animation set
	auto* cache = GetSubsystem<ResourceCache>();
	auto* animationSet = cache->GetResource<AnimationSet2D>("imp/GoldIcon.scml");
	

	/*SharedPtr<Node> spriteNode(scene_->CreateChild("AnimatedSprite2D"));
	spriteNode->SetPosition(Vector3(0.0f, 0.0f, -1.0f));*/

	auto* animatedSprite = node_->CreateComponent<AnimatedSprite2D>();
	// Set animation
	animatedSprite->SetAnimationSet(animationSet);
	animatedSprite->SetAnimation("idle");

	return node_;
}
