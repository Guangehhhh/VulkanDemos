﻿#include "Engine.h"

#include "Application/Application.h"
#include "GenericPlatform/GenericPlatformTime.h"

#include "Vulkan/VulkanDevice.h"

Engine* Engine::g_Instance = nullptr;

Engine::Engine()
    : m_VulkanRHI(nullptr)
	, m_IsRequestingExit(false)
{
	Engine::g_Instance = this;
}

Engine::~Engine()
{
	Engine::g_Instance = nullptr;
}

std::shared_ptr<VulkanRHI> Engine::GetVulkanRHI()
{
	return m_VulkanRHI;
}

std::shared_ptr<VulkanDevice> Engine::GetVulkanDevice()
{
	return m_VulkanRHI->GetDevice();
}

VkDevice Engine::GetDeviceHandle()
{
	return m_VulkanRHI->GetDevice()->GetInstanceHandle();
}

std::shared_ptr<Application> Engine::GetApplication()
{
	return m_SlateApplication;
}

std::shared_ptr<GenericApplication> Engine::GetPlatformApplication()
{
	return m_SlateApplication->GetPlatformApplication();
}

std::shared_ptr<GenericWindow> Engine::GetPlatformWindow()
{
	return m_SlateApplication->GetPlatformWindow();
}

int32 Engine::PreInit(const std::vector<std::string>& cmdLine, int32 width, int32 height, const char* title)
{
    m_AppTitle = title;
    
	m_SlateApplication = std::make_shared<Application>();
	m_SlateApplication->Init(this);
	m_SlateApplication->MakeWindow(width, height, title);

	m_VulkanRHI = std::make_shared<VulkanRHI>();

	for (int32 i = 0; i < m_AppInstanceExtensions.size(); ++i) 
	{
		m_VulkanRHI->AddAppInstanceExtensions(m_AppInstanceExtensions[i]);
	}

	for (int32 i = 0; i < m_AppDeviceExtensions.size(); ++i)
	{
		m_VulkanRHI->AddAppDeviceExtensions(m_AppDeviceExtensions[i]);
	}

	m_VulkanRHI->Init();
    
    ParseAssetsPath(cmdLine);

    InputManager::Init();
	GenericPlatformTime::InitTiming();
    
	return 0;
}

void Engine::ParseAssetsPath(const std::vector<std::string>& cmdLine)
{
    if (cmdLine.size() > 0)
    {
        std::string exePath = cmdLine[0];
        int32 length = 0;
        for (size_t i = 0; i < exePath.size(); ++i) {
            if (exePath[i] == '\\') {
                exePath[i] = '/';
            }
        }
        for (size_t i = exePath.size() - 1; i >= 0; --i) {
            if (exePath[i] == '/') {
                break;
            }
            length += 1;
        }
        m_AssetsPath = exePath.substr(0, exePath.size() - length);
    }
    
    MLOG("AssetsPath:%s", m_AssetsPath.c_str());
}

const std::string& Engine::GetAssetsPath() const
{
	return m_AssetsPath;
}

int32 Engine::Init()
{
	m_VulkanRHI->PostInit();
	return 0;
}

void Engine::Exist()
{
	m_VulkanRHI->Shutdown();
	m_VulkanRHI = nullptr;

	m_SlateApplication->Shutdown(true);
}

void Engine::Tick(float time, float delta)
{
	m_SlateApplication->Tick(time, delta);
}

void Engine::PumpMessage()
{
	m_SlateApplication->PumpMessages();
}

bool Engine::IsRequestingExit()
{
	return m_IsRequestingExit;
}

void Engine::RequestExit(bool value)
{
	if (m_IsRequestingExit == value) {
		return;
	}
	m_IsRequestingExit = value;
}

Engine* Engine::Get()
{
	return g_Instance;
}
