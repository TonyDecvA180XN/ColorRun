#include "DebugHelper.h"
#include "MathUtils.h"

W4_USE_UNSTRICT_INTERFACE

void DebugHelper::buildGizmo()
{
	const float cArrowLength = 1.0f;
	const float cThickness = 0.05f;
	const float cRadius = 0.1f;
	const uint32_t cResolution = 16;

	auto origin = Mesh::create::sphere(cRadius, cResolution / 2, cResolution);

	auto xTrunk = Mesh::create::cylinder(cArrowLength, cThickness, cResolution);
	auto yTrunk = Mesh::create::cylinder(cArrowLength, cThickness, cResolution);
	auto zTrunk = Mesh::create::cylinder(cArrowLength, cThickness, cResolution);
	auto xCap = Mesh::create::cone(0, cRadius, cThickness * 3, cResolution, 1);
	auto yCap = Mesh::create::cone(0, cRadius, cThickness * 3, cResolution, 1);
	auto zCap = Mesh::create::cone(0, cRadius, cThickness * 3, cResolution, 1);

	xTrunk->setWorldRotation(Rotator(0, 0, -90_deg));
	xCap->setWorldRotation(Rotator(0, 0, -90));

	// yTrunk & yCap are correctly oriented by default

	zTrunk->setWorldRotation(Rotator(90_deg, 0, 0));
	zCap->setWorldRotation(Rotator(90_deg, 0, 0));

	xTrunk->setWorldTranslation({ cArrowLength / 2, 0, 0 });
	xCap->setWorldTranslation({ cArrowLength, 0, 0 });

	yTrunk->setWorldTranslation({ 0, cArrowLength / 2, 0 });
	yCap->setWorldTranslation({ 0, cArrowLength, 0 });

	zTrunk->setWorldTranslation({ 0, 0,cArrowLength / 2, });
	zCap->setWorldTranslation({ 0, 0, cArrowLength });


	auto redMat = Material::getDefault()->createInstance();
	auto greenMat = Material::getDefault()->createInstance();
	auto blueMat = Material::getDefault()->createInstance();
	auto whiteMat = Material::getDefault()->createInstance();

	redMat->setParam("baseColor", vec4(1, 0, 0, 1));
	greenMat->setParam("baseColor", vec4(0, 1, 0, 1));
	blueMat->setParam("baseColor", vec4(0, 0, 1, 1));
	whiteMat->setParam("baseColor", vec4(1, 1, 1, 1));

	redMat->setParam("specColor", vec4(0, 0, 0, 1));
	greenMat->setParam("specColor", vec4(0, 0, 0, 1));
	blueMat->setParam("specColor", vec4(0, 0, 0, 1));
	whiteMat->setParam("specColor", vec4(0, 0, 0, 1));

	origin->setMaterialInst(whiteMat);
	xTrunk->setMaterialInst(redMat);
	xCap->setMaterialInst(redMat);
	yTrunk->setMaterialInst(greenMat);
	yCap->setMaterialInst(greenMat);
	zTrunk->setMaterialInst(blueMat);
	zCap->setMaterialInst(blueMat);

	xTrunk->addChild(xCap);
	yTrunk->addChild(yCap);
	zTrunk->addChild(zCap);
	origin->addChild(xTrunk);
	origin->addChild(yTrunk);
	origin->addChild(zTrunk);
	Render::getRoot()->addChild(origin);
	

	//auto x = Mesh::create::cylinder(2.0f, 0.05f, 16);
	//auto y = Mesh::create::cylinder(2.0f, 0.05f, 16);
	//auto z = Mesh::create::cylinder(2.0f, 0.05f, 16);
	//auto xc = Mesh::create::cone(0.0f, 0.1f, 0.15f, 16, 1);
	//auto yc = Mesh::create::cone(0.0f, 0.1f, 0.15f, 16, 1);
	//auto zc = Mesh::create::cone(0.0f, 0.1f, 0.15f, 16, 1);

	//x->rotateWorld(Rotator(0.0f, 0.0f, -math::PI / 2));
	//x->translateWorld({ 1.0f, 0.0f, 0.0f });
	//xc->rotateWorld(Rotator(0.0f, 0.0f, -math::PI / 2));
	//xc->translateWorld({ 2.0f, 0.0f, 0.0f });

	//y->rotateWorld(Rotator(0.0f, 0.0f, 0.0f));
	//y->translateWorld({ 0.0f, 1.0f, 0.0f });
	//yc->rotateWorld(Rotator(0.0f, 0.0f, 0.0f));
	//yc->translateWorld({ 0.0f, 2.0f, 0.0f });

	//z->rotateWorld(Rotator(math::PI / 2, 0.0f, 0.0f));
	//z->translateWorld({ 0.0f, 0.0f, 1.0f });
	//zc->rotateWorld(Rotator(math::PI / 2, 0.0f, 0.0f));
	//zc->translateWorld({ 0.0f, 0.0f, 2.0f });

	//auto xM = Material::getDefault()->createInstance();
	//auto yM = Material::getDefault()->createInstance();
	//auto zM = Material::getDefault()->createInstance();

	//xM->setParam("baseColor", vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//xM->setParam("specColor", vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//yM->setParam("baseColor", vec4(0.0f, 1.0f, 0.0f, 1.0f));
	//yM->setParam("specColor", vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//zM->setParam("baseColor", vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//zM->setParam("specColor", vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//x->setMaterialInst(xM);
	//y->setMaterialInst(yM);
	//z->setMaterialInst(zM);
	//xc->setMaterialInst(xM);
	//yc->setMaterialInst(yM);
	//zc->setMaterialInst(zM);

	//Render::getRoot()->addChild(x);
	//Render::getRoot()->addChild(y);
	//Render::getRoot()->addChild(z);
	//Render::getRoot()->addChild(xc);
	//Render::getRoot()->addChild(yc);
	//Render::getRoot()->addChild(zc);
}
