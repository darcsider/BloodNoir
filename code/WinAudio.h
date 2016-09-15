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
		unique_ptr<SoundEffectInstance> m_effect;
		map<string, SoundEffect> m_soundEffects;

	public:
		WinAudio();
		~WinAudio();

		void InitializeAudio();
		bool SetBackgroundMusic(string fileName);
		bool AddSoundEffect(string effectName, string fileName);
		void AudioEngineUpdate();
		void PlayEffect(string effectName);
		void StartBackgroundMusic();
		void StopBackgroundMusic();
		void SetBackgroundVolume(float value);
		void SetSoundEffectVolume(float value);

		wstring ConvertSTRtoWSTR(string text)
		{
			typedef std::codecvt_utf8<wchar_t> convert_type;
			wstring convertedText;
			wstring_convert<convert_type, wchar_t> converter;

			convertedText = converter.from_bytes(text);
			return convertedText;
		}
};
#endif