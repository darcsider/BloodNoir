/*=====================================================================================
$File: NVGSAInterface.cpp
$Date: January 7, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "NVGSAInterface.h"

////////////////////////////////////////////////////////////////////////////////
// initApplication()
////////////////////////////////////////////////////////////////////////////////
bool initApplication(NvGsaApplication *app, const wchar_t *exePath)
{
	wchar_t fullExePath[_MAX_PATH];
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t exe[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	wchar_t fullExeDir[_MAX_PATH];
	wchar_t fullExeName[_MAX_PATH];

	// Get the full executable path.
	if (_wfullpath(fullExePath, exePath, _MAX_PATH) == NULL) {
		return false;
	}

	// Split the executable path
	_wsplitpath_s(
		fullExePath,
		drive, _MAX_DRIVE,
		dir, _MAX_DIR,
		exe, _MAX_FNAME,
		ext, _MAX_EXT);

	// Get the executable directory
	if (0 > swprintf_s(fullExeDir, _MAX_PATH, L"%s%s", drive, dir)) {
		return false;
	}

	// Get the executable name with extension
	if (0 > swprintf_s(fullExeName, _MAX_PATH, L"%s%s", exe, ext)) {
		return false;
	}

	// Setup the application fields
	app->displayName = _wcsdup(L"GsaSimpleApp");
	app->versionName = _wcsdup(L"1.0.0.0");
	app->installPath = _wcsdup(fullExeDir);
	app->configPath = _wcsdup(fullExeDir);
	app->executable = _wcsdup(exe);
	app->execCmd = _wcsdup(fullExeName);

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// releaseApplication()
////////////////////////////////////////////////////////////////////////////////
void releaseApplication(NvGsaApplication *app)
{
	if (app != NULL) {
		// Free the application fields
		free((void *)app->displayName);
		app->displayName = NULL;
		free((void *)app->versionName);
		app->versionName = NULL;
		free((void *)app->installPath);
		app->installPath = NULL;
		free((void *)app->configPath);
		app->configPath = NULL;
		free((void *)app->executable);
		app->executable = NULL;
		free((void *)app->execCmd);
		app->execCmd = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
// initOptions()
////////////////////////////////////////////////////////////////////////////////
bool initOptions(const NvGsaNamedOption **options, size_t *numOptions)
{
	static const wchar_t *testEnums[] = {
		L"Value1",
		L"Value2",
		L"Value3"
	};

	static size_t numTestEnums = sizeof(testEnums) / sizeof(const wchar_t *);

	NvGsaNamedOption *results = NULL;

	if (options == NULL) {
		return false;
	}

	results = (NvGsaNamedOption *)malloc(4 * sizeof(NvGsaNamedOption));
	if (results == NULL) {
		return false;
	}

	results[0].name = L"Test_INT";
	results[0].value.type = NV_GSA_TYPE_INT;
	results[0].value.asInt = 35;
	results[0].numRange.maxValue.asInt = 0;
	results[0].numRange.minValue.asInt = 0;
	results[0].numRange.numSteps = -1;

	results[1].name = L"Test_FLOAT";
	results[1].value.type = NV_GSA_TYPE_FLOAT;
	results[1].value.asFloat = 35.5f;
	results[1].numRange.maxValue.asFloat = 0.0f;
	results[1].numRange.minValue.asFloat = 0.0f;
	results[1].numRange.numSteps = -1;


	results[2].name = L"Test_ENUM";
	results[2].value.type = NV_GSA_TYPE_ENUM;
	results[2].value.asEnum = L"Value1";
	results[2].enumRange.enums = testEnums;
	results[2].enumRange.numEnums = (uint32_t)numTestEnums;

	results[3].name = L"Test_BOOL";
	results[3].value.type = NV_GSA_TYPE_BOOL;
	results[3].value.asBool = true;

	*options = results;
	if (numOptions != NULL) {
		*numOptions = 4;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// releaseOptions()
////////////////////////////////////////////////////////////////////////////////
void releaseOptions(const NvGsaNamedOption *options)
{
	free((void *)options);
}

////////////////////////////////////////////////////////////////////////////////
// initResolutions()
////////////////////////////////////////////////////////////////////////////////
bool initResolutions(const NvGsaResolution **resolutions, size_t *numResolutions)
{
	NvGsaResolution *results = NULL;

	if (resolutions == NULL) {
		return false;
	}

	results = (NvGsaResolution *)malloc(4 * sizeof(NvGsaResolution));
	if (results == NULL) {
		return false;
	}

	results[0].width = 800;
	results[0].height = 600;
	results[0].refreshRate = 60;

	results[1].width = 1024;
	results[1].height = 768;
	results[1].refreshRate = 60;

	results[2].width = 1600;
	results[2].height = 1200;
	results[2].refreshRate = 60;

	results[3].width = 1920;
	results[3].height = 1440;
	results[3].refreshRate = 60;

	*resolutions = results;
	if (numResolutions != NULL) {
		*numResolutions = 4;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// destroyResolutions()
////////////////////////////////////////////////////////////////////////////////
void releaseResolutions(const NvGsaResolution *resolutions)
{
	free((void *)resolutions);
}

////////////////////////////////////////////////////////////////////////////////
// printOptionValue()
////////////////////////////////////////////////////////////////////////////////
void printOptionValue(const NvGsaVariant *value, const NvGsaNamedOption *option)
{
	if (value != NULL && option != NULL) {
		switch (value->type) {
		case NV_GSA_TYPE_INT:
			wprintf(L"int %s = %d\n", option->name, value->asInt);
			break;

		case NV_GSA_TYPE_FLOAT:
			wprintf(L"float %s = %g\n", option->name, value->asFloat);
			break;

		case NV_GSA_TYPE_ENUM:
			wprintf(L"enum %s = %s\n", option->name, value->asEnum);
			break;

		case NV_GSA_TYPE_BOOL:
			wprintf(L"bool %s = %s\n", option->name, value->asBool ? L"true" : L"false");
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// cycleOptionValue()
////////////////////////////////////////////////////////////////////////////////
void cycleOptionValue(NvGsaVariant *result, const NvGsaVariant *value, const NvGsaNamedOption *option)
{
	if (result != NULL && value != NULL && option != NULL) {
		result->type = value->type;
		switch (value->type) {
		case NV_GSA_TYPE_INT:
			result->asInt = value->asInt + 1;
			return;

		case NV_GSA_TYPE_FLOAT:
			result->asFloat = value->asFloat + 1.0f;
			return;

		case NV_GSA_TYPE_ENUM: {
			uint32_t i;
			for (i = 0; i < option->enumRange.numEnums; ++i) {
				if (0 == wcscmp(value->asEnum, option->enumRange.enums[i])) {
					result->asEnum = option->enumRange.enums[(i + 1) % option->enumRange.numEnums];
					return;
				}
			}
			if (option->enumRange.numEnums > 0 && option->enumRange.enums != NULL) {
				result->asEnum = option->enumRange.enums[0];
			}
			else {
				result->asEnum = L"<error>";
			}
			return;
		}

		case NV_GSA_TYPE_BOOL:
			result->asBool = !value->asBool;
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// printResolution()
////////////////////////////////////////////////////////////////////////////////
void printResolution(const NvGsaResolution *resolution)
{
	if (resolution != NULL) {
		wprintf(
			L"resolution = %d x %d pixels @ %g Hz\n",
			resolution->width,
			resolution->height,
			resolution->refreshRate);
	}
}

////////////////////////////////////////////////////////////////////////////////
// cycleResolution()
////////////////////////////////////////////////////////////////////////////////
void cycleResolution(NvGsaResolution *resolution, const NvGsaResolution *resolutions, size_t numResolutions)
{
	if (resolution != NULL && resolutions != NULL && numResolutions > 0) {
		uint32_t i;
		for (i = 0; i < numResolutions; ++i) {
			if (resolution->width == resolutions[i].width &&
				resolution->height == resolutions[i].height &&
				resolution->refreshRate == resolutions[i].refreshRate) {
				*resolution = resolutions[(i + 1) % numResolutions];
				return;
			}
		}
		*resolution = resolutions[0];
	}
}
