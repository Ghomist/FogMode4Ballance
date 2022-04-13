#pragma once

#include <BML/BMLAll.h>
#include <map>

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class FogMode : public IMod {
public:
	FogMode(IBML* bml) : IMod(bml) {}

	virtual CKSTRING GetID() override { return "FogMode"; }
	virtual CKSTRING GetVersion() override { return BML_VERSION; }
	virtual CKSTRING GetName() override { return "Fog mode"; }
	virtual CKSTRING GetAuthor() override { return "Ghomist"; }
	virtual CKSTRING GetDescription() override { return "Try to play Ballance in dark fog?"; }
	DECLARE_BML_VERSION;
	virtual void OnLoad();
	virtual void OnProcess();
	virtual void OnStartLevel();
	virtual void OnModifyConfig(CKSTRING category, CKSTRING key, IProperty* prop);
	virtual void OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName,
		CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials,
		BOOL dynamic, XObjectArray* objArray, CKObject* masterObj);

private:
	void UpdateFog();

private:
	CKRenderContext* _render;
	IProperty* fog_enable;
	IProperty* fog_start;
	IProperty* fog_end;
	IProperty* fog_color;
	bool _enable;
	CKDWORD color = 0;
	std::map<std::string, std::string> color_dict;
};