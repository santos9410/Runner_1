#pragma once
#include "Obstacle.h"
#include "Character.h"
#include "Moneda.h"

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>

#include <list>
using namespace Urho3D;
class Animator : public LogicComponent
{
	URHO3D_OBJECT(Animator, LogicComponent);
public:

	/// Construct.
	explicit Animator(Context* context);

	/// Register object factory and attributes.
	static void RegisterObject(Context* context);
	/// Handle update. Called by LogicComponent base class.
	void Update(float timeStep) override;

	void IniciarComponentes(Scene* scene_);

	bool play = false;
protected:
	Obstacle * obstacle_;
	Character* character_;
	Moneda* moneda_;

	Node* fondo1;
	Node* fondo2;
	
	float scaleX;
	float scaleY;

	Vector3 posInicial1;
	Vector3 posInicial2;

	Node* personaje_;

	const float MOVE_SPEED = 5.0f;
	float distancia = 0.07f;

	float distanciaMon = 0.07f;
	Timer* time_ = new Timer();
	

	int tiempo = 30;

	float nuevoObs_ = 10;
	float nuevaMoneda_ = 5;
	bool jump = true;
	float time2 = 0;
	float time3 =  0;
	float timeVelo = 0;
	float timePuntos = 0;
	std::list<Node*> nodeItems_;
	std::list<Node*> nodesMoneda_;
	
	Scene* scene_;
	
	int puntaje = 0;
private:
	void ControlarCamino();
	void ControlarSalto();
	void ControlarVelocidad(float timeStep);
	void ControlarMonedas(float timeStep);
	
	AnimatedSprite2D* animatedSprite;
	Text3D* puntajeTxt;
};