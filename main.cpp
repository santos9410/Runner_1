#include "main.h"


#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/AudioEvents.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Container/HashSet.h>

#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/CollisionCircle2D.h>
#include <Urho3D/Urho2D/PhysicsEvents2D.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>


using namespace Urho3D;

Game1::Game1(Context* context) :
	Application(context)
{

}


void Game1::Setup() {
	engineParameters_["FullScreen"] = false;
	engineParameters_["WindowWidth"] = 1200;
	engineParameters_["WindowHeight"] = 720;

}

void Game1::Start() {
	engine_->SetMaxFps(60);

	CreateScene();

	SetupViewPort();

	CreateCharacter(0.8f, Vector3(0.0f, -3.0f, 0.0f), 0.8f);
	fondo1 = scene_->GetChild("background1");
	fondo2 = scene_->GetChild("background2");

	Node* nodep = scene_->CreateChild("piso");
	nodep->SetScale2D(Vector2(20, 3));
	nodep->SetPosition2D(Vector2(0, -6));
	auto body2 = nodep->CreateComponent<RigidBody2D>();
	body2->SetBodyType(BT_STATIC);
	auto box2 = nodep->CreateComponent<CollisionBox2D>();
	box2->SetSize(Vector2(20, 1));

	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Game1, HandleKeyDown));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Game1, HandleUpdate));
	SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Game1, HandlePostRenderUpdate));
}
//Cargamos todo lo relacionado a scene 
void Game1::CreateScene() {
	scene_ = new Scene(context_);
	auto* cache = GetSubsystem<ResourceCache>();
	SharedPtr<File> file = cache->GetFile("Scenes/Main.xml");
	scene_->LoadXML(*file);

	scene_->CreateComponent<Octree>();
	scene_->CreateComponent<DebugRenderer>();
	auto* physicsWorld = scene_->CreateComponent<PhysicsWorld2D>();
	physicsWorld->SetGravity(Vector2(2.f, 2.20f)); // Neutralize gravity as the character will always be grounded
	

}

void Game1::SetupViewPort() {
	//Create Camera
	Node* cameraNode = scene_->CreateChild("Camera");
	Camera* camera = cameraNode->CreateComponent<Camera>();
	camera->SetFarClip(200.0f);

	cameraNode->Translate(Vector3(0, 0, -10.0f));
	camera->SetOrthographic(true);
	auto* graphics = GetSubsystem<Graphics>();
	float he = (float)graphics->GetHeight() * PIXEL_SIZE;

	//camera->SetOrthoSize((float)graphics->GetHeight() * PIXEL_SIZE);
	camera->SetOrthoSize(15);
	SharedPtr<Viewport> viewport(new Viewport(context_, scene_, camera));

	Renderer* renderer = GetSubsystem<Renderer>();
	renderer->SetViewport(0, viewport);

}

void Game1::Stop() {

}


void Game1::HandleUpdate(StringHash eventType, VariantMap& eventData) {
	//using namespace Update;




	auto* input = GetSubsystem<Input>();

	if (input->GetKeyDown(KEY_SPACE))
	{
		auto* body = animatedSprite->GetComponent<RigidBody2D>();
		Vector2 veloc = body->GetLinearVelocity();

		if (veloc.y_ == 0) {
			jump = true;
		}
		else
		{
			jump = false;
		}


		if (jump) {
			body->ApplyLinearImpulseToCenter(Vector2(0.0f, 10.27f), true);
		}

	}

	else if (animatedSprite->GetAnimation() != "run")
	{
		animatedSprite->SetAnimation("run");
	}

	if (time_->GetMSec(false) > tiempo && play) { // controlar carretera

		ControlarCamino();

		time_->Reset();
	}

}



void Game1::HandleKeyDown(StringHash eventType, VariantMap& eventData) {

	using namespace KeyDown;

	// Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
	int key = eventData[P_KEY].GetInt();


	if (key == KEY_ESCAPE)
	{
		engine_->Exit();
	}



}

Node* Game1::CreateCharacter(float friction, Vector3 position, float scale)
{
	auto* cache = GetSubsystem<ResourceCache>();
	Node* spriteNode = scene_->CreateChild("Imp");
	spriteNode->SetPosition(position);
	spriteNode->SetScale(scale);
	animatedSprite = spriteNode->CreateComponent<AnimatedSprite2D>();
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
	shape->SetRadius(1.1f); // Set shape size
	shape->SetFriction(friction); // Set friction
	shape->SetRestitution(0.0f); // Bounce
	impBody->SetGravityScale(2.0f);
	return spriteNode;
}

void Game1::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{

	auto* physicsWorld = scene_->GetComponent<PhysicsWorld2D>();
	physicsWorld->DrawDebugGeometry();

}
void Game1::ControlarCamino() {
	float x = fondo1->GetPosition().x_;
	float y = fondo1->GetPosition().y_;
	float z = fondo1->GetPosition().z_;

	float x2 = fondo2->GetPosition().x_;
	float y2 = fondo2->GetPosition().y_;
	float z2 = fondo2->GetPosition().z_;


	if ((x - distancia) > -33.5) {
		fondo1->Translate2D(Vector2(-1 * distancia, 0));

	}
	else {
		x = 32;
		fondo1->SetPosition(Vector3(x, y, z));


	}

	if ((x2 - distancia) > -33.5) {
		fondo2->Translate2D(Vector2(-1 * distancia, 0));
	}
	else {
		x2 = 32;
		fondo2->SetPosition(Vector3(x2, y2, z2));
	}





}
URHO3D_DEFINE_APPLICATION_MAIN(Game1)
