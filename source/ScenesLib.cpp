
#include "pch.h"
#include "ScenesLib.h"

#include "Scene.h"
#include "Camera.h"
#include "TriMesh.h"
#include "Light.h"
#include "MRawColor.h"
#include "MRawTexture.h"
#include "MLambertDiffuse.h"
#include "MLambertPhong.h"
#include "MLambertCookTorrance.h"

#include "Albedo.h"

#include "LDirection.h"
#include "LPoint.h"

Scene* PremadeScenes::DefaultEmptyScene()
{
	Scene* pScene = new Scene{};

	Camera* pFrontCam = new Camera{
		FPoint3{0.f, 0.f, 60.f},
		110.f,
		10.f,
		Frustrum{10.0f, 500.f}
	};

	Camera* pRightAboveCam = new Camera{
		FPoint3{40.f, 40.f, 30.f},
		FPoint3{0.f, 0.f, 0.f},
		90.f,
		10.f,
		Frustrum{10.0f, 500.f}
	};

	LDirection* pSunLight = new LDirection{ FVector3{.577f, -.577f, -.577f}, RGBColor{1.f, 1.f, 1.f}, 15.f, true};
	LDirection* pFrontLight = new LDirection{ FVector3{0.f, 0.f, -1.f}, RGBColor{1.f, 1.f, 1.f}, 1.f, false };

	LPoint* pLight0 = new LPoint{ FPoint3{0.f, 0.f, 20.f}, RGBColor{1.f, 1.f, 1.f}, 1000.f, true };
	LPoint* pLight1 = new LPoint{ FPoint3{10.f, 0.f, 20.f}, RGBColor{1.f, 1.f, 1.f}, 500.f, false };

	LPoint* pLight2 = new LPoint{ FPoint3{30.f, 0.f, 0.f}, RGBColor{1.f, 1.f, 1.f}, 500.f, false };
	LPoint* pLight3 = new LPoint{ FPoint3{-30.f, 0.f, 0.f}, RGBColor{1.f, 1.f, 1.f}, 500.f, false };

	pScene->AddCamera(pFrontCam);
	pScene->AddCamera(pRightAboveCam);
	pScene->AddLight(pFrontLight);
	pScene->AddLight(pSunLight);
	pScene->AddLight(pLight0);
	pScene->AddLight(pLight1);
	pScene->AddLight(pLight2);
	pScene->AddLight(pLight3);

	return pScene;
}

Scene* PremadeScenes::Box()
{
	Scene* finalScene = DefaultEmptyScene();

	TriMesh* pMesh = new TriMesh{ "boxTest", {}, 10.f, true, 1.f, true };

	// Lambert + Phong
	pMesh->AddMaterial(new MLambertPhong{
		new ValueMap{"Resources/vehicle_specular.png"},
		new ValueMap{"Resources/vehicle_gloss.png"},
		new NormalMap{"Resources/vehicle_normal.png"},
		new Texture{"Resources/vehicle_diffuse.png" } });

	// Lambert + CookTorrance
	pMesh->AddMaterial(new MLambertCookTorrance{ RGBColor{1.f, 0.782f, 0.344f}, 0.2f, true });

	// Lambert
	MLambertDiffuse* pLambert = new MLambertDiffuse{ new Texture{"Resources/vehicle_diffuse.png"}, new NormalMap{ "Resources/emptyNormals.png" } };
	pLambert->OverwriteDiffuseMap(nullptr);
	pLambert->SetDefaultColor(0.5f, 0.5f, 0.5f);
	pMesh->AddMaterial(pLambert);

	// Rawcolor
	pMesh->AddMaterial(new MRawColor{ 1.f, 0.f, 0.f });

	// Rawtexture
	pMesh->AddMaterial(new MRawTexture{
		new Texture{"Resources/uv_grid_2.png" } });

	finalScene->AddMesh(pMesh);

	return finalScene;
}

Scene* PremadeScenes::Vehicle()
{
	Scene* finalScene = DefaultEmptyScene();

	TriMesh* pMesh = new TriMesh{ "vehicle", {}, 10.f, true, 1.f, true };

	// Lambert + CookTorrance
	MLambertCookTorrance* pLambertCookTorr = new MLambertCookTorrance{ RGBColor{1.f, 0.782f, 0.344f}, 0.4f , true };
	pLambertCookTorr->OverwriteNormalMap(new NormalMap{ "Resources/vehicle_normal.png" });
	pMesh->AddMaterial(pLambertCookTorr);
	

	// Lambert + Phong																				 
	pMesh->AddMaterial(new MLambertPhong{
		new ValueMap{"Resources/vehicle_specular.png"},
		new ValueMap{"Resources/vehicle_gloss.png"},
		new NormalMap{"Resources/vehicle_normal.png"},
		new Texture{"Resources/vehicle_diffuse.png" } });

	// Lambert
	MLambertDiffuse* pLambert = new MLambertDiffuse{ new Texture{"Resources/vehicle_diffuse.png"}, new NormalMap{ "Resources/vehicle_normal.png" } };
	pLambert->OverwriteDiffuseMap(nullptr);
	pLambert->SetDefaultColor(0.5f, 0.5f, 0.5f);
	pMesh->AddMaterial(pLambert);

	// Rawcolor
	pMesh->AddMaterial(new MRawColor{ 1.f, 0.f, 0.f });

	// Rawtexture
	pMesh->AddMaterial(new MRawTexture{
		new Texture{"Resources/uv_grid_2.png" } });

	pMesh->AddMaterial(new MRawTexture{
		new Texture{"Resources/vehicle_diffuse.png"} });

	finalScene->AddMesh(pMesh);

	return finalScene;
}

Scene* PremadeScenes::Tie()
{
	Scene* pFinal = DefaultEmptyScene();

	TriMesh* pMesh = new TriMesh{ "tie", {}, 10.0f, true, 1.0f, true };

	// Lambert + CookTorrance
	MLambertCookTorrance* pLambertCookTorr = new MLambertCookTorrance{ RGBColor{1.f, 0.782f, 0.344f}, 0.4f , true };
	pLambertCookTorr->OverwriteNormalMap(new NormalMap{ "Resources/TIE_IN/TIE_IN_Normal.png" });
	pMesh->AddMaterial(pLambertCookTorr);

	// Lambert + Phong
	MLambertPhong* pLamPhong = new MLambertPhong{
		nullptr,
		nullptr,
		new NormalMap{ "Resources/TIE_IN/TIE_IN_Normal.png" },
		new Texture{"Resources/TIE_IN/TIE_IN_Diff.png"} };
	
	pMesh->AddMaterial(pLamPhong);
	

	pMesh->AddMaterial(new MLambertDiffuse{ new Texture{"Resources/TIE_IN/TIE_IN_Diff.png"}, nullptr });

	LPoint* pLight0 = new LPoint{ FPoint3{0.f, 10.f, 100.f}, RGBColor{1.f, 1.f, 1.f}, 3000.f, true };
	pFinal->AddLight(pLight0);
	pFinal->AddMesh(pMesh);
	return pFinal;
}

Scene* PremadeScenes::PBRBoxes()
{
	Scene* finalScene = DefaultEmptyScene();

	TriMesh* pMesh0 = new TriMesh{ "boxTest", {-40.f, -20.f, 0.f}, 10.f, true, 1.f, true };
	TriMesh* pMesh1 = new TriMesh{ "boxTest", {0.f, -20.f, 0.f}, 10.f, true, 1.f, true};
	TriMesh* pMesh2 = new TriMesh{ "boxTest", {40.f, -20.f, 0.f}, 10.f, true, 1.f, true};
	TriMesh* pMesh3 = new TriMesh{ "boxTest", {-40.f, 20.f, 0.f}, 10.f, true, 1.f, true };
	TriMesh* pMesh4 = new TriMesh{ "boxTest", {0.f, 20.f, 0.f}, 10.f, true, 1.f, true };
	TriMesh* pMesh5 = new TriMesh{ "boxTest", {40.f, 20.f, 0.f}, 10.f, true, 1.f, true };

	std::string normalmapFilepath{ "Resources/vehicle_normal.png" };

	MLambertCookTorrance* mat0 = new MLambertCookTorrance{ Albedo::Gold, 1.f , true };
	MLambertCookTorrance* mat1 = new MLambertCookTorrance{ Albedo::Gold, 0.6f , true };
	MLambertCookTorrance* mat2 = new MLambertCookTorrance{ Albedo::Gold, 0.2f , true };
	MLambertCookTorrance* mat3 = new MLambertCookTorrance{ Albedo::Gold, 1.f , false };
	MLambertCookTorrance* mat4 = new MLambertCookTorrance{ Albedo::Gold, 0.6f , false };
	MLambertCookTorrance* mat5 = new MLambertCookTorrance{ Albedo::Gold, 0.2f , false };

	/*mat0->OverwriteNormalMap(new NormalMap{ normalmapFilepath });
	mat1->OverwriteNormalMap(new NormalMap{ normalmapFilepath });
	mat2->OverwriteNormalMap(new NormalMap{ normalmapFilepath });
	mat3->OverwriteNormalMap(new NormalMap{ normalmapFilepath });
	mat4->OverwriteNormalMap(new NormalMap{ normalmapFilepath });
	mat5->OverwriteNormalMap(new NormalMap{ normalmapFilepath });*/

	pMesh0->AddMaterial(mat0);
	pMesh1->AddMaterial(mat1);
	pMesh2->AddMaterial(mat2);
	pMesh3->AddMaterial(mat3);
	pMesh4->AddMaterial(mat4);
	pMesh5->AddMaterial(mat5);

	finalScene->AddMesh(pMesh0);
	finalScene->AddMesh(pMesh1);
	finalScene->AddMesh(pMesh2);
	finalScene->AddMesh(pMesh3);
	finalScene->AddMesh(pMesh4);
	finalScene->AddMesh(pMesh5);

	return finalScene;
}

Scene* PremadeScenes::Plane()
{
	Scene* finalScene = DefaultEmptyScene();

	TriMesh* pMesh = new TriMesh{ "TestMesh", {}, 10.f, true, 1.f, true };

	pMesh->AddMaterial(new MLambertPhong{
		new ValueMap{"Resources/vehicle_specular.png"},
		new ValueMap{"Resources/vehicle_gloss.png"},
		new NormalMap{"Resources/vehicle_normal.png"},
		new Texture{"Resources/vehicle_diffuse.png" } });

	pMesh->AddMaterial(new MRawColor{ 1.f, 1.f, 1.f });

	finalScene->AddMesh(pMesh);

	return finalScene;
}