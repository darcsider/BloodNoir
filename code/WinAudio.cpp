/*=====================================================================================
$File: WinAudio.cpp
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "WinAudio.h"

WinAudio::WinAudio()
{

}

WinAudio::~WinAudio()
{

}

void WinAudio::InitializeAudio()
{
	AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_Default;
#ifdef _DEBUG
	audioFlags = audioFlags | AudioEngine_Debug;
#endif

	m_audioEngine.reset(new AudioEngine(audioFlags));

	/*m_backgroundMusic.reset(new SoundEffect(m_audioEngine.get(), L"..\\Music\\Electro.wav"));
	m_backgroundEffect = m_backgroundMusic->CreateInstance();

	m_soundEffect.reset(new SoundEffect(m_audioEngine.get(), L"..\\Music\\heli.wav"));
	m_effect = m_soundEffect->CreateInstance();

	// move this later as I finish this class
	auto soundState = m_effect->GetState();*/
}

bool WinAudio::SetBackgroundMusic()
{
	return true;
}

bool WinAudio::AddSoundEffect()
{
	return true;
}

void WinAudio::AudioEngineUpdate()
{
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			MessageBox(NULL, "Something went really wrong audio is broken", "Audio Error", MB_OK);
			return;
		}
	}
}