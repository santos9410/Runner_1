#include "AnimatorController.h"

#include <Urho3D/UI/Text3D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>

#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>


// Character2D logic component
Animator::Animator(Context* context) :
	LogicComponent(context)
{
	SetUpdateEventMask(USE_UPDATE);


}
void Animator::RegisterObject(Context* context)
{
	context->RegisterFactory<Animator>();

}

void Animator::IniciarComponentes(Scene* scene)
{
	scene_ = scene;

	obstacle_ = new Obstacle(context_, scene_);
	character_ = new Character(context_, scene_);
	moneda_ = new Moneda(context_, scene_);

	personaje_ =  character_->CreateCharacter(Vector3(0.0f, -3.0f, 0.0f), 0.8f);
	animatedSprite = personaje_->GetComponent<AnimatedSprite2D>();

	fondo1 = scene_->GetChild("background1");
	fondo2 = scene_->GetChild("background2");
	
	//play = true;

	puntajeTxt = scene_->GetChild("Puntos")->GetChild("texto")->GetComponent<Text3D>();

	puntajeTxt->SetText(String("0"));

	auto* body = animatedSprite->GetComponent<RigidBody2D>();
	body->SetGravityScale(gravedad);

	posInit_C = personaje_->GetPosition();
	CreateUI();
	inicio = true;
}

void Animator::Update(float timeStep)
{
	auto* input = GetSubsystem<Input>();
	if (timeCount_->GetMSec(false) > tiempoConteo && inicio) { // controlar conteo de inicio

		ControlarInicio();
		timeCount_->Reset();
	}

	if (input->GetKeyDown(KEY_SPACE))
	{
		ControlarSalto();
		
	}

	else if (animatedSprite->GetAnimation() != "run" && play == true)
	{
		animatedSprite->SetAnimation("run");
		
	}
	else  {
		//animatedSprite->SetAnimation("idle");
	}

	if (play) {

		ControlarMonedas(timeStep);

		ControlarCamino();

		ControlarVelocidad(timeStep);
		
		

		if (timePuntos > 2) {
			puntaje = puntaje + 1;
			puntajeTxt->SetText(String(puntaje));
			timePuntos = 0;
		}
		else {
			timePuntos += timeStep;
		}
	}
	else {
		animatedSprite->SetAnimation("idle");

	}
	
}

void Animator::ControlarInicio() {
	auto* txtTurbo = layoutRoot_->GetChildStaticCast<Text>("TxtTurbo", true);

	switch (caso)
	{
	case 0:
		txtTurbo->SetVisible(true);
		txtTurbo->SetText("3");
		caso = 1;
		break;
	case 1:
		txtTurbo->SetText("2");
		caso = 2;
		break;
	case 2:
		txtTurbo->SetText("1");
		caso = 3;
		break;
	case 3:
		txtTurbo->SetVisible(false);
		caso = 4;
		tiempoConteo = 400;
		break;
	case 4:
		inicio = false;
		layoutRoot_->SetVisible(false);
		play = true;
		//IniciarSonido();
		break;
	}
}

void Animator::ControlarCamino() {
	float x = fondo1->GetPosition().x_;
	float y = fondo1->GetPosition().y_;
	float z = fondo1->GetPosition().z_;

	float x2 = fondo2->GetPosition().x_;
	float y2 = fondo2->GetPosition().y_;
	float z2 = fondo2->GetPosition().z_;


	if ((x - distancia) > -33.5) {
		//fondo1->Translate2D(Vector2(-1 * distancia, 0));
		fondo1->Translate(Vector3(-1 * distancia, 0, 0));
	}
	else {

		x = 33.5;
		fondo1->SetPosition(Vector3(x, y, z));


	}

	if ((x2 - distancia) > -33.5) {
		//fondo2->Translate2D(Vector2(-1 * distancia, 0));
		fondo2->Translate(Vector3(-1 * distancia, 0, 0));
	}
	else {
		x2 = 33.5;
		fondo2->SetPosition(Vector3(x2, y2, z2));
	}


	for (Node* node : nodeItems_) {

		node->Translate2D(Vector2(-1 * distancia, 0));
	}

}

void Animator::ControlarSalto() {
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
		body->ApplyLinearImpulseToCenter(Vector2(0.0f, salto), true);
	}
}

void Animator::ControlarVelocidad(float timeStep) {
		

	if (time2 > nuevoObs_) { // cada cierto tiempo aparece un nuevo obstaculo
		Node* node = obstacle_->CreateObstacle(Vector3(14.0f, -4.2f, -1.0f));
		nodeItems_.push_back(node);

		time2 = 0;
	}
	else {
		time2 = time2 + timeStep;
	}

	if (timeVelo > 4) { // cada x segundos aumenta la velocidad del camino
		distancia = distancia + 0.01f;
		if (nuevoObs_ > 0.9f) {
			nuevoObs_ = nuevoObs_ - 0.4f;
			//salto = salto + 0.009f;
			//gravedad = gravedad + 0.009f;

		}
		timeVelo = 0;
	}
	else {
		timeVelo = timeVelo + timeStep;
	}

	if (time3 > nuevaMoneda_) {
		nuevaMoneda_ = Random(5, 20);
		Node* node = moneda_->Create(Vector3(14.0f, -1.0f, -1.0f) );
		nodesMoneda_.push_back(node);
		
		time3 = 0;
	}
	else {
		time3 = time3 + timeStep;
	}
}

void Animator::ControlarMonedas(float timeStep) {
	
	
	float xc = personaje_->GetPosition().x_;
	float yc = personaje_->GetPosition().y_;

	for (int i = 0; i < nodesMoneda_.size(); i++) {
		Node* node2 = nodesMoneda_.back();
		nodesMoneda_.pop_back();

		//node2->Translate2D(Vector2( -1 * distanciaMon,0));
		node2->Translate2D(Vector2(-1 * distancia, 0));
		float x3 = node2->GetPosition().x_;
		float y3 = node2->GetPosition().y_;

		if ((y3 - 0.9f) < yc && (x3 > xc - 1.2f && x3 < xc + 1.2f)) {

			node2->Remove();
			puntaje = puntaje + 10;
			puntajeTxt->SetText(String(puntaje));
		}
		else {
			if (x3 > -20) {
			nodesMoneda_.push_front(node2);
			}
			else {
				node2->Remove();
			}
		}


	}
}

void Animator::ReiniciarValores() {

	distancia = 0.07f;
	nuevoObs_ = 7.0f;
	puntaje = 0;
	puntajeTxt->SetText(String(puntaje));

	while (!nodeItems_.empty()) {
		Node* node = nodeItems_.back();
		nodeItems_.pop_back();
		node->Remove();
	}

	while (!nodesMoneda_.empty()) {
		Node* node2 = nodesMoneda_.back();
		nodesMoneda_.pop_back();
		node2->Remove();
	}

	personaje_->SetPosition(posInit_C);
	
	caso = 0;
	tiempoConteo = 800;
	timeCount_->Reset();
	inicio = true;
	layoutRoot_->SetVisible(true);

	animatedSprite->SetAnimation("idle");

}

void Animator::CreateUI()
{
	auto* cache = GetSubsystem<ResourceCache>();
	auto* ui = GetSubsystem<UI>();

	// Set up global UI style into the root UI element
	auto* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
	ui->GetRoot()->SetDefaultStyle(style);

	// Load UI content prepared in the editor and add to the UI hierarchy
	layoutRoot_ = ui->LoadLayout(cache->GetResource<XMLFile>("UI/UILoadExample.xml"));
	ui->GetRoot()->AddChild(layoutRoot_);

	layoutRoot_->SetSize(1200,720);
	auto* txtTurbo = layoutRoot_->GetChildStaticCast<Text>("TxtTurbo", true);
	txtTurbo->SetText("Runner");

}