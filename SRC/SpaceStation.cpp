#include "SpaceStation.h"
#include "GameUtil.h"
#include "Entity.h"
#include "Model_3DS.h"

SpaceStation::SpaceStation() : Entity("spacestation")
{
	mRadius = 10;
	SmartPtr<Model_3DS> model = new Model_3DS();
	model->Load("3DModels//SpaceStation//spaceStation.3DS");
	SetModel(model);
}