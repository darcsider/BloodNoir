#pragma once
/*=====================================================================================
$File: NVGSAInterface.h
$Date: January 7, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef NVGSAINTERFACE_H
#define NVGSAINTERFACE_H
#include "Includes.h"
#include <stdlib.h>
#include <wchar.h>

bool initApplication(NvGsaApplication *app, const wchar_t *exePath);
void releaseApplication(NvGsaApplication *app);

bool initOptions(const NvGsaNamedOption **options, size_t *numOptions);
void releaseOptions(const NvGsaNamedOption *options);

bool initResolutions(const NvGsaResolution **resolutions, size_t *numResolutions);
void releaseResolutions(const NvGsaResolution *resolutions);

void printOptionValue(const NvGsaVariant *value, const NvGsaNamedOption *option);
void cycleOptionValue(NvGsaVariant *result, const NvGsaVariant *value, const NvGsaNamedOption *option);

void printResolution(const NvGsaResolution *resolution);
void cycleResolution(NvGsaResolution *resolution, const NvGsaResolution *availableResolutions, size_t numResolutions);

#endif // !NVGSAINTERFACE_H