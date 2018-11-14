
#include "AnimatorController.h"

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/UI/Text3D.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>

#include <list>


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
        
		Animator* animator_;
private:
	
	void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
	
	void HandleCollisionBegin(StringHash eventType, VariantMap& eventData);

	void CrearPiso();
	
};

