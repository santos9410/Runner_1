

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>


using namespace Urho3D;

class Game1 : public Application
{
    URHO3D_OBJECT(Game1, Application)

    public:
        Game1 (Context* context);
        virtual void Setup();
        virtual void Start();
        virtual void Stop();
        void CreateScene();
        void SetupViewPort();
	    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	    void HandleUpdate(StringHash eventType, VariantMap& eventData);

	
    protected: 

        SharedPtr<Scene> scene_;
        SharedPtr<Node> cameraNode_;
        
		//bool play = false;
		
		Node* carro;
			
		float scaleX;
		float scaleY;
		
		Node* fondo1;
		Node* fondo2;

		AnimatedSprite2D* animatedSprite;

		Node* CreateCharacter(float friction, Vector3 position, float scale);
    
		const float MOVE_SPEED = 5.0f;
		float distancia = 0.2f;
		Timer* time_ = new Timer();
		bool play = true;
		int tiempo = 50;
		
		float fightTimer_ = 0;
		bool jump = true;
private:
	void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
	void ControlarCamino();
};

