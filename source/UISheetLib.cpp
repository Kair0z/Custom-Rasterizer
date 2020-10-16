#include "pch.h"
#include "UISheetLib.h"
#include "EMath.h"
#include "Texture.h"

#include "UBox.h"
#include "UButton.h"
#include "UIElement.h"
#include "USlider.h"

#include "Rasterizer.h"	
#include "Master.h"
#include "Scene.h"

#include <iostream>

using namespace Elite;

UISheet* PremadeSheets::EmptySheet(const std::string& name)
{
	UISheet* sheet = new UISheet{name};


	return sheet;
}

UISheet* PremadeSheets::SliderSheet(Master* pMaster)
{
	UISheet* sheet = EmptySheet("SliderSheet");

	USlider* pSlider = new USlider{ 0.f, 10.f, 5.f, FPoint2{0.f, 80.f}, 200.f, 60.f, true };

	sheet->AddElement(pSlider);

	return sheet;
}

UISheet* PremadeSheets::ButtonSheet(Master* pMaster)
{
	UISheet* sheet = EmptySheet("ButtonSheet");

	float maxX{ float(pMaster->GetRasterizer()->GetWindow().GetWindowDimensions().x) };
	float maxY{ float(pMaster->GetRasterizer()->GetWindow().GetWindowDimensions().y) };

	UButton* pViewtypeToggle = new UButton{ FPoint2{0.f, 0.f}, 100.f, 60.f, true };
	pViewtypeToggle->OverwriteTexture(new Texture{ "Resources/UI/toggle_viewtype.png" });

	pViewtypeToggle->SetOnPressed([=]() {});
	pViewtypeToggle->SetOnReleased([=]() {pMaster->GetRasterizer()->ToggleViewtype(); });

	UButton* pSceneToggle = new UButton{ FPoint2{0.f, 80.f}, 100.f, 60.f, true };
	pSceneToggle->OverwriteTexture(new Texture{ "Resources/UI/toggle_scenes.png" });

	pSceneToggle->SetOnPressed([=]() {});
	pSceneToggle->SetOnReleased([=]() {pMaster->GetSceneGraph()->ToggleActiveScene(); });

	UButton* pMaterialToggle = new UButton{ FPoint2{0.f, 160.f}, 100.f, 60.f, true };
	pMaterialToggle->OverwriteTexture(new Texture{ "Resources/UI/toggle_material.png" });

	pMaterialToggle->SetOnPressed([=]() {});
	pMaterialToggle->SetOnReleased([=]() {pMaster->GetSceneGraph()->GetActiveScene()->ToggleMaterial(); });

	UButton* pCameraReset = new UButton{ FPoint2{0.f, 240.f}, 100.f, 60.f, true };
	pCameraReset->OverwriteTexture(new Texture{ "Resources/UI/reset_camera.png" });

	pCameraReset->SetOnPressed([=]() {});
	pCameraReset->SetOnReleased([=]() {pMaster->GetSceneGraph()->GetActiveScene()->ResetCamera(); });

	UButton* pCameraToggle = new UButton{ FPoint2{0.f, 320.f}, 100.f, 60.f, true };
	pCameraToggle->OverwriteTexture(new Texture{ "Resources/UI/toggle_camera.png" });

	pCameraToggle->SetOnPressed([=]() {});
	pCameraToggle->SetOnReleased([=]() {pMaster->GetSceneGraph()->GetActiveScene()->ToggleCamera(); });

	UButton* pUpdateToggle = new UButton{ FPoint2{0.f, 400.f}, 100.f, 60.f, true };
	pUpdateToggle->OverwriteTexture(new Texture{ "Resources/UI/toggle_update.png" });

	pUpdateToggle->SetOnPressed([=]() {});
	pUpdateToggle->SetOnReleased([=]() {pMaster->GetSceneGraph()->GetActiveScene()->ToggleUpdate(); });

	UButton* pCullToggle = new UButton{ FPoint2{0.f, 480.f}, 100.f, 60.f, true };
	pCullToggle->OverwriteTexture(new Texture{ "Resources/UI/toggle_cullmode.png" });

	pCullToggle->SetOnPressed([=]() {});
	pCullToggle->SetOnReleased([=]() {pMaster->GetRasterizer()->ToggleCullmode(); });

	UButton* pBGToggle = new UButton{ FPoint2{maxX - 100.f, 0.f}, 100.f, 60.f, true };
	pBGToggle->OverwriteTexture(new Texture{ "Resources/UI/toggle_background.png" });

	pBGToggle->SetOnPressed([=]() {});
	pBGToggle->SetOnReleased([=]() {pMaster->GetRasterizer()->ToggleBackground(); });

	sheet->AddElement(pViewtypeToggle);
	sheet->AddElement(pSceneToggle);
	sheet->AddElement(pMaterialToggle);
	sheet->AddElement(pCameraReset);
	sheet->AddElement(pCameraToggle);
	sheet->AddElement(pUpdateToggle);
	sheet->AddElement(pBGToggle);
	sheet->AddElement(pCullToggle);

	return sheet;
}