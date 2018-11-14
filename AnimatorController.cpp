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
	
	play = true;

	puntajeTxt = scene_->GetChild("Puntos")->GetChild("texto")->GetComponent<Text3D>();

	puntajeTxt->SetText(String("0"));
}

void Animator::Update(float timeStep)
{
	auto* input = GetSubsystem<Input>();

	if (input->GetKeyDown(KEY_SPACE))
	{
		ControlarSalto();
		
	}

	else if (animatedSprite->GetAnimation() != "run" && play == true)
	{
		animatedSprite->SetAnimation("run");
	}
	else  {
		animatedSprite->SetAnimation("iddle");
	}

	if (play) {

		ControlarCamino();

		ControlarVelocidad(timeStep);
		
		ControlarMonedas(timeStep);

		if (timePuntos > 2) {
			puntaje = puntaje + 1;
			puntajeTxt->SetText(String(puntaje));
			timePuntos = 0;
		}
		else {
			timePuntos += timeStep;
		}
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
		body->ApplyLinearImpulseToCenter(Vector2(0.0f, 9.50f), true);
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

	if (timeVelo > 30) { // cada 30 segundos aumenta la velocidad del camino
		distancia = distancia + 0.01f;
		timeVelo = 0;
	}
	else {
		timeVelo = timeVelo + timeStep;
	}

	if (time3 > nuevaMoneda_) {
		nuevaMoneda_ = Random(10, 30);
		Node* node = moneda_->Create(Vector3(14.0f, -1.0f, -1.0f) );
		nodesMoneda_.push_back(node);
		
		time3 = 0;
	}
	else {
		time3 = time3 + timeStep;
	}
}

void Animator::ControlarMonedas(float timeStep) {
	
	//for (Node* node : nodesMoneda_) {

	//	node->Translate2D(Vector2(-1 * distanciaMon, 0));

	//	//node->Roll(0.04f);
	//	//node->Rotate2D(0.1f);
	//}
	float xc = personaje_->GetPosition().x_;
	float yc = personaje_->GetPosition().y_;

	for (int i = 0; i < nodesMoneda_.size(); i++) {
		Node* node2 = nodesMoneda_.back();
		nodesMoneda_.pop_back();

		node2->Translate2D(Vector2( -1 * distanciaMon,0));
		float x3 = node2->GetPosition().x_;
		float y3 = node2->GetPosition().y_;

		if ((y3 - 0.5f) < yc && (x3 > xc - 0.5f && x3 < xc + 0.5f)) {

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