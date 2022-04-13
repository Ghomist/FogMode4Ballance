#include "FogMode.h"

IMod* BMLEntry(IBML* bml) {
	return new FogMode(bml);
}

void FogMode::OnLoad() {
	//GetLogger()->Info("Hello fog mode");
	//m_bml->RegisterCommand(new FogCommand());
	_render = m_bml->GetRenderContext();

	GetConfig()->SetCategoryComment("Fog", "Fog options.");
	fog_enable = GetConfig()->GetProperty("Fog", "Enable");
	fog_enable->SetComment("Enable fog mode");
	fog_enable->SetDefaultBoolean(false);
	_enable = fog_enable->GetBoolean();

	fog_start = GetConfig()->GetProperty("Fog", "Start");
	fog_start->SetComment("Fog start distance. Default: 30.");
	fog_start->SetDefaultFloat(30.0f);

	fog_end = GetConfig()->GetProperty("Fog", "End");
	fog_end->SetComment("Fog end distance. Default: 65.");
	fog_end->SetDefaultFloat(65.0f);

	fog_color = GetConfig()->GetProperty("Fog", "Color");
	fog_color->SetComment("Fog color.\nR,G,B or HEX color. Presets: black, gray, green, orange, pink, silver, aqua, cyan, purple, iris, red, white, yellow.");
	fog_color->SetDefaultString("black");

	color_dict["black"] = "#000000";
	color_dict["gray"] = "#808080";
	color_dict["green"] = "#008000";
	color_dict["orange"] = "#FFA500";
	color_dict["pink"] = "#FFC0CB";
	color_dict["silver"] = "#C0C0C0";
	color_dict["aqua"] = "#00FFFF";
	color_dict["cyan"] = "#00FFFF";
	color_dict["purple"] = "#800080";
	color_dict["iris"] = "#5D3FD3";
	color_dict["red"] = "#FF0000";
	color_dict["white"] = "#FFFFFF";
	color_dict["yellow"] = "#FFFF00";

}

void FogMode::OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName,
	CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials,
	BOOL dynamic, XObjectArray* objArray, CKObject* masterObj) {
}

void FogMode::OnStartLevel() {
	//m_bml->GetTextureByName("Particle_Flames")->SetDesiredVideoFormat(_32_RGB888);
	UpdateFog();
}

void FogMode::OnProcess() {
	if (m_bml->IsPlaying()) {
		CKTargetCamera* cam = m_bml->GetTargetCameraByName("InGameCam");
		if (cam != nullptr)
			cam->SetBackPlane(_enable ? fog_end->GetFloat() + 10.0f : 1200.0f);
	}
}

void FogMode::OnModifyConfig(CKSTRING category, CKSTRING key, IProperty* prop) {
	UpdateFog();
}

void FogMode::UpdateFog() {
	_enable = fog_enable->GetBoolean();
	if (_enable) {
		int r = 0, g = 0, b = 0;

		std::string _str = fog_color->GetString();
		if (_str[0] == '#') {
			// Hex
			_str.append("000000");
			sscanf_s(_str.c_str(), "#%02x%02x%02x", &r, &g, &b);
		}
		else if (color_dict.find(_str) != color_dict.end()) {
			// In dict
			_str = color_dict[_str];
			sscanf_s(_str.c_str(), "#%02x%02x%02x", &r, &g, &b);
			//fog_color->SetString(_str.c_str());
		}
		else {
			// Other
			_str.append(",0,0,0");
			sscanf_s(_str.c_str(), "%d,%d,%d", &r, &g, &b);
		}

		color = ColorSetRed(color, r);
		color = ColorSetGreen(color, g);
		color = ColorSetBlue(color, b);

		_render->SetFogMode(VXFOG_LINEAR);
		_render->SetFogColor(color);
		_render->SetFogStart(fog_start->GetFloat());
		_render->SetFogEnd(fog_end->GetFloat());
	}
	else
		_render->SetFogMode(VXFOG_NONE);
}
