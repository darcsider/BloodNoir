#pragma once
/*=====================================================================================
$File: WinAudio.h
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef WINAUDIO_H
#define WINAUDIO_H
#include "Includes.h"

class WinAudio
{
	protected:
		unique_ptr<AudioEngine> m_audioEngine;
		unique_ptr<SoundEffect> m_backgroundMusic;
		unique_ptr<SoundEffectInstance> m_backgroundEffect;
		unique_ptr<SoundEffect> m_soundEffect;
		unique_ptr<SoundEffectInstance> m_effect;

	public:
		WinAudio();
		~WinAudio();

		void InitializeAudio();
		bool SetBackgroundMusic();
		bool AddSoundEffect();
};
#endif