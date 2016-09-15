#pragma once
/*=====================================================================================
$File: AudioSystem.h
$Date: September 13, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H
#include "WinAudio.h"
class AudioSystem
{
	private:
		unique_ptr<WinAudio> m_win32Audio;
		//PS4Audio m_ps4Audio;
		//XBOXONE m_XBONEAudio;

	public:
		AudioSystem();
		~AudioSystem();
		AudioSystem(AudioSystem const& copy);
		AudioSystem& operator=(AudioSystem const& copy);

		void InitializeAudioSystem();
		void AudioSystemUpdate();

		void SetBackgroundMusic(string file);
		void AddEffect(string name, string file);
		void PlayEffect(string name);
		void PlayBackgroundMusic();
		void StopBackgroundMusic();
		void SetBGVolume(float value);
		void SetEffectVolume(float value);
};
#endif // !AUDIOSYSTEM_H