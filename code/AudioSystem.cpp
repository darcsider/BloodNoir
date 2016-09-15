/*=====================================================================================
$File: AudioSystem.cpp
$Date: September 13, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "AudioSystem.h"

AudioSystem::AudioSystem()
{
#if _WIN32
	m_win32Audio = make_unique<WinAudio>();
#endif
}

AudioSystem::~AudioSystem()
{
#if _WIN32
	m_win32Audio.release();
#endif
}

AudioSystem::AudioSystem(AudioSystem const & copy)
{
}

void AudioSystem::InitializeAudioSystem()
{
#if _WIN32
	m_win32Audio->InitializeAudio();
#endif
}

void AudioSystem::AudioSystemUpdate()
{
#if _WIN32
	m_win32Audio->AudioEngineUpdate();
#endif
}

void AudioSystem::SetBackgroundMusic(string file)
{
#if _WIN32
	m_win32Audio->SetBackgroundMusic(file);
#endif
}

void AudioSystem::AddEffect(string name, string file)
{
#if _WIN32
	m_win32Audio->AddSoundEffect(name, file);
#endif
}

void AudioSystem::PlayEffect(string name)
{
#if _WIN32
	m_win32Audio->PlayEffect(name);
#endif
}

void AudioSystem::PlayBackgroundMusic()
{
#if _WIN32
	m_win32Audio->StartBackgroundMusic();
#endif
}

void AudioSystem::StopBackgroundMusic()
{
#if _WIN32
	m_win32Audio->StopBackgroundMusic();
#endif
}

void AudioSystem::SetBGVolume(float value)
{
#if _WIN32
	m_win32Audio->SetBackgroundVolume(value);
#endif
}

void AudioSystem::SetEffectVolume(float value)
{
#if _WIN32
	m_win32Audio->SetSoundEffectVolume(value);
#endif
}
