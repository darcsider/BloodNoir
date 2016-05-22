/*=====================================================================================
$File: Console.cpp
$Date: March 20, 2016
$Creator: Jamie Cooper
$Notice: (C) Copyright 2015 by Punch Drunk Squirrel Games LLC. All Rights Reserved.
=====================================================================================*/
#include "Console.h"

Console::Console()
{
}

Console::~Console()
{
	onLostDevice();            // call onLostDevice() for every graphics item
}

bool Console::initialize(DX11RenderManager *graphics, WinInput *input, Vector2 startPosition, int width, int height, XMFLOAT4 backColor)
{
	m_graphicSystem = graphics;
	m_input = input;
	m_consolePosition = startPosition;
	m_width = width;
	m_height = height;
	m_backgroundColor = backColor;
	m_lineSpacing = m_graphicSystem->GetLineSpacing();
	m_promptPosition = Vector2((startPosition.x + 10), (startPosition.x + 10));
	m_textPosition = Vector2(m_promptPosition.x, (m_promptPosition.y + m_lineSpacing));
	m_startTextPosition = Vector2((m_promptPosition.x + 10), m_promptPosition.y);
	m_textInput.clear();
	m_newline = false;
	m_visible = false;
	m_commandMap.clear();
	m_commandString.clear();
	m_prompt.clear();
	m_history.clear();

	m_prompt = "|:>";

	Command *testCommand = new Command();
	testCommand->setKeyboardKeyBinding(Keyboard::Keys::OemTilde);

	function<void(bool)> funcPoint = bind(&Console::HideShow, this, placeholders::_1);
	testCommand->setCallbackFunction(funcPoint);

	m_input->AddCommand(testCommand);

	return true;
}

void Console::AddCommand(string commandName, function<void()> funcPoint)
{
	m_commandMap.insert(pair<string, function<void()>>(commandName, funcPoint));
}

void Console::ProcessCommand(string commandName)
{
	if (!commandName.empty())
	{
		auto functionIndex = m_commandMap.find(commandName);
		if (functionIndex != m_commandMap.end())
		{
			functionIndex->second();
		}
	}
}

void Console::Draw()
{
	if (m_visible)
	{
		m_graphicSystem->DrawQuad(m_consolePosition, m_width, m_height, m_backgroundColor);

		m_textPosition = Vector2(m_promptPosition.x, (m_promptPosition.y + m_lineSpacing));

		m_graphicSystem->BeginScene();

		if (!m_history.empty())
		{
			for (int line = 0; line < m_history.size(); ++line)
			{
				m_textPosition = Vector2(m_promptPosition.x, (m_promptPosition.y + m_lineSpacing));
				m_textPosition.y += (m_lineSpacing * float(line));

				m_graphicSystem->DrawTextToScreen(m_history[line], m_textPosition, Colors::Green);
			}
		}

		m_prompt = "|:>";
		m_prompt += m_textInput;

		m_graphicSystem->DrawTextToScreen(m_prompt, m_promptPosition, Colors::Green);
		m_graphicSystem->EndScene();
	}
}

void Console::TextInput(WPARAM wParam)
{
	static int element = 0;

	if (m_newline)
	{
		m_textInput.clear();
		m_newline = false;
	}

	if (wParam == '\b')
	{
		if (m_textInput.length() > 0)
			m_textInput.erase(m_textInput.size() - 1);
	}

	if (wParam == Keyboard::Escape)
	{
		m_textInput = "";
	}

	if (wParam != Keyboard::Escape)
	{
		m_textInput += wParam;
	}

	if ((char)wParam == '\r')
	{
		m_textInput.erase(m_textInput.size() - 1);
		ProcessCommand(m_textInput);
		m_prompt = "|:>";
		if (m_history.size() < MAXLINES && !m_textInput.empty())
			m_history.push_back(m_textInput);
		else if (m_history.size() >= MAXLINES && !m_textInput.empty())
		{
			m_history.erase(m_history.begin());
			m_history.push_back(m_textInput);
		}
		m_textInput.clear();
		m_newline = true;
	}
}

void Console::onLostDevice()
{
	return;
}

void Console::onResetDevice()
{
	return;
}