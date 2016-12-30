/*=====================================================================================
$File: WinAudio.cpp
$Date: May 8, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Win32_Audio.h"

Win32_Audio::Win32_Audio()
{

}

Win32_Audio::~Win32_Audio()
{

}

void Win32_Audio::InitializeAudio()
{
	AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_Default;
#ifdef _DEBUG
	audioFlags = audioFlags | AudioEngine_Debug;
#endif

	m_audioEngine.reset(new AudioEngine(audioFlags));
}

bool Win32_Audio::SetBackgroundMusic(string fileName)
{
	m_backgroundMusic.reset(new SoundEffect(m_audioEngine.get(), ConvertSTRtoWSTR(fileName).c_str()));
	m_backgroundEffect = m_backgroundMusic->CreateInstance();
	return true;
}

bool Win32_Audio::AddSoundEffect(string effectName, string fileName)
{
	auto soundIndex = m_soundEffects.find(effectName);

	if (soundIndex == m_soundEffects.end())
	{
		wstring file = ConvertSTRtoWSTR(fileName);
		m_soundEffects.insert(map<string, SoundEffect>::value_type(effectName, SoundEffect(m_audioEngine.get(), file.c_str())));
		return true;
	}
	else
	{
		return false;
	}
}

void Win32_Audio::AudioEngineUpdate()
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

void Win32_Audio::PlayEffect(string effectName)
{
	auto soundIndex = m_soundEffects.find(effectName);

	if (soundIndex != m_soundEffects.end())
	{
		m_effect = soundIndex->second.CreateInstance();
		auto soundState = m_effect->GetState();

		if (soundState == STOPPED)
			m_effect->Play();
	}
}

void Win32_Audio::StartBackgroundMusic()
{
	m_backgroundEffect->Play(true);
}

void Win32_Audio::StopBackgroundMusic()
{
	m_backgroundEffect->Stop();
}

void Win32_Audio::SetBackgroundVolume(float value)
{
	m_backgroundEffect->SetVolume(value);
}

void Win32_Audio::SetSoundEffectVolume(float value)
{
	map<string, SoundEffect>::iterator effectIterator;

	for (effectIterator = m_soundEffects.begin(); effectIterator != m_soundEffects.end(); effectIterator++)
	{
		m_effect = effectIterator->second.CreateInstance();
		m_effect->SetVolume(value);
	}
}