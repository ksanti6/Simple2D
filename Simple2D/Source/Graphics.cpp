#include "Graphics.h"
#include <iostream>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>



//error checking macro
#define RETURN_IF_FAILED(_func) \
if(FAILED(hr = _func)) { \
 printf_s("ERROR: DX12: %d in %s calling %s.\n", __LINE__, __FUNCTION__, #_func);\
 printf_s("ERROR: DX12: %d.\n", hr);\
return hr; \
}


HRESULT Graphics::CreateSwapChain(void)
{
	HRESULT hr = S_OK;
	int width = 0, height = 0;

	glfwGetWindowSize(m_window, &width, &height);

	//describing the properties of our swap chain (generic)
	DXGI_SWAP_CHAIN_DESC1 swap_chain1
	{
		.Width = static_cast<UINT>(width),
		.Height = static_cast<UINT>(height),
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = false,
		.SampleDesc = 
		{
			.Count = 1,
			.Quality = 0
	    },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = m_frameCount,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	//more describing more for fullscreen
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC swap_chain_full
	{
		.RefreshRate =
		{
			.Numerator = 60,
			.Denominator = 1
        },
		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE,
		.Scaling = DXGI_MODE_SCALING_STRETCHED,
		.Windowed = true
	};

	//make the swap chain
	RETURN_IF_FAILED(m_factory->CreateSwapChainForHwnd(m_comQ.Get(),
		glfwGetWin32Window(m_window), &swap_chain1, &swap_chain_full,
		nullptr, &m_swapChain));

	//getting buffers and creating views for them
	m_rtvHeap = std::make_unique<DirectX::DescriptorPile>(m_device.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, m_frameCount);

	for (int k = 0; k < m_frameCount; ++k)
	{
		RETURN_IF_FAILED(m_swapChain->GetBuffer(k,
			IID_PPV_ARGS(&m_swapChainBuffers[k])));

		m_device->CreateRenderTargetView(m_swapChainBuffers[k].Get(),
			nullptr, m_rtvHeap->GetCpuHandle(k));
	}


	return hr;
}

HRESULT Graphics::Init(GLFWwindow* _window)
{
	//save window for later
	m_window = _window;

	//enable the use of dx debugging
	if constexpr (_DEBUG)
	{
		ComPtr<ID3D12Debug6> debug;

		D3D12GetDebugInterface(IID_PPV_ARGS(&debug));

		debug->EnableDebugLayer();
		debug->SetEnableAutoName(true);
		debug->SetEnableGPUBasedValidation(true);
	}

	//this is our error checking result var
	HRESULT hr = S_OK;

	//create our factory

	RETURN_IF_FAILED(CreateDXGIFactory(IID_PPV_ARGS(&m_factory)));

	//get first adapter
	ComPtr<IDXGIAdapter> adapter;

	m_factory->EnumAdapters(0, &adapter);

	RETURN_IF_FAILED(adapter->QueryInterface(IID_PPV_ARGS(&m_adapter)));

	///create the device

	RETURN_IF_FAILED(D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&m_device)));

	//create the command queue
	D3D12_COMMAND_QUEUE_DESC command_queue_desc
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT
	};

	RETURN_IF_FAILED(m_device->CreateCommandQueue(&command_queue_desc,
		IID_PPV_ARGS(&m_comQ)));

	//create swap chain
	RETURN_IF_FAILED(CreateSwapChain());

	for (int k = 0; k < m_frameCount; ++k)
	{
	   //create command allocator and command lists
		RETURN_IF_FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_comAllocator[k])));

		RETURN_IF_FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_comAllocator[k].Get(), nullptr, IID_PPV_ARGS(&m_comList[k])));

		//anytime you use a comlist, remember to close it after
		m_comList[k]->Close();

		//create fences and event associated with them (command list fences)
		m_fenceValue[k] = 0;

		RETURN_IF_FAILED(m_device->CreateFence(m_fenceValue[k],
			D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence[k])));

		m_fenceEvent[k] = CreateEvent(nullptr, false, false, nullptr);
	}

	//create fences for swap chain
	m_SCFenceValue = 0;

	RETURN_IF_FAILED(m_device->CreateFence(m_SCFenceValue,
		D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_SCFence)));

	m_SCFenceEvent = CreateEvent(nullptr, false, false, nullptr);


	return hr;
}

void Graphics::Shutdown(void)
{
	//check if swap chain is idle
	if (m_SCFence->GetCompletedValue() < m_SCFenceValue) // still running
	{
		m_SCFence->SetEventOnCompletion(m_SCFenceValue, m_SCFenceEvent);
		WaitForSingleObject(m_SCFenceEvent, INFINITE);
	}

	CloseHandle(m_SCFenceEvent);

	//check for other fences aswell
	for (int k = 0; k < m_frameCount; ++k)
	{
		if (m_fence[k]->GetCompletedValue() < m_fenceValue[k])
		{
			m_fence[k]->SetEventOnCompletion(m_fenceValue[k], m_fenceEvent[k]);
			WaitForSingleObject(m_fenceEvent[k], INFINITE);
		}

		CloseHandle(m_fenceEvent[k]);
	}

	
}

Graphics& Graphics::GetInstance()
{
	static Graphics graphics;
	return graphics;
}
