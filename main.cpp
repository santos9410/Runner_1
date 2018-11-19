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
	Animator::RegisterObject(context);
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

	CrearPiso();


	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Game1, HandleKeyDown));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Game1, HandleUpdate));
	SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Game1, HandlePostRenderUpdate));
	SubscribeToEvent(E_PHYSICSBEGINCONTACT2D, URHO3D_HANDLER(Game1, HandleCollisionBegin));

	animator_ = scene_->CreateChild("animator")->CreateComponent<Animator>();
	animator_->IniciarComponentes(scene_);
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

void Game1::CrearPiso() {
	Node* nodep = scene_->CreateChild("piso");
	nodep->SetScale2D(Vector2(20, 3));
	nodep->SetPosition2D(Vector2(0, -6));
	auto body2 = nodep->CreateComponent<RigidBody2D>();
	body2->SetBodyType(BT_STATIC);
	auto box2 = nodep->CreateComponent<CollisionBox2D>();
	box2->SetSize(Vector2(20, 1));
}



void Game1::HandleUpdate(StringHash eventType, VariantMap& eventData) {

	using namespace Update;

	//float timeStep = eventData[P_TIMESTEP].GetFloat();

	
}

void Game1::HandleKeyDown(StringHash eventType, VariantMap& eventData) {

	using namespace KeyDown;

	// Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
	int key = eventData[P_KEY].GetInt();


	if (key == KEY_ESCAPE)
	{
		engine_->Exit();
	}

	if (key == KEY_R) {
		if (!animator_->play) {
			
			animator_->ReiniciarValores();
		}
	}
}	


void Game1::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{

	/*auto* physicsWorld = scene_->GetComponent<PhysicsWorld2D>();
	physicsWorld->DrawDebugGeometry();*/

}

void Game1::HandleCollisionBegin(StringHash eventType, VariantMap& eventData)
{
	Log::Write(1, String(eventType));
	auto* hitNodeA = static_cast<Node*>(eventData[PhysicsBeginContact2D::P_NODEA].GetPtr());
	auto* hitNodeB = static_cast<Node*>(eventData[PhysicsBeginContact2D::P_NODEB].GetPtr());
	if (hitNodeA->GetName() == "Imp" && hitNodeB->GetName() == "item1") {
		Log::Write(1, String("fin del juego"));
		animator_->play = false;
	}

}

URHO3D_DEFINE_APPLICATION_MAIN(Game1)
