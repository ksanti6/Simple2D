/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Graphics.cpp
* Purpose: implementation of Dx12 graphics code, allows for very basic drawing of textures
*
**********************************************************************************************************************/
#include "Graphics.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>                    //window stuff
#include <d3dx12.h>                              //meat of dx12 stuff
#include <directxtk12/ResourceUploadBatch.h>     //for uploading textures
#include <directxtk12/WICTextureLoader.h>        //for loading textures
#include <directxtk12/DirectXHelpers.h>          //helpers
#include <map>                                   //for pairing texture enum to path

/************************************************
*
* error checking macro
*
************************************************/
#define RETURN_IF_FAILED(_func) \
if(FAILED(hr = _func)) { \
 printf_s("ERROR: DX12: %d in %s calling %s.\n", __LINE__, __FUNCTION__, #_func);\
 printf_s("ERROR: DX12: %d.\n", hr);\
return hr; \
}

/************************************************
*
* texture enum and file path pairings
*
************************************************/
const std::map<Graphics::Textures, std::wstring> textPaths =
{
	{
		Graphics::Textures::player, L"./Asset/MouseGuy.png"
	},
	{
		Graphics::Textures::enemy, L"./Asset/CatGuy.png"
	},
	{
		Graphics::Textures::wall, L"./Asset/TestTile.png"
	},
	{
		Graphics::Textures::cheese, L"./Asset/BetterCheese.png"
	},
	{
		Graphics::Textures::win, L"./Asset/Win.png"
	},
	{
		Graphics::Textures::lose, L"./Asset/Lose.png"
	}
};

/************************************************
*
* filling in descriptors and creating the swap
* chain, also creating the viewports for the
* buffers
* returns either all good or error value
*
************************************************/
HRESULT Graphics::CreateSwapChain(void)
{
	HRESULT hr = S_OK;

	glfwGetWindowSize(m_window, &m_width, &m_height);

	//describing the properties of our swap chain (generic)
	DXGI_SWAP_CHAIN_DESC1 swap_chain1
	{
		.Width = static_cast<UINT>(m_width),
		.Height = static_cast<UINT>(m_height),
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


/************************************************
*
* load in all the textures
*
************************************************/
void Graphics::LoadTexture()
{
	m_dsvHeap = std::make_unique<DirectX::DescriptorHeap>(m_device.Get(), Textures::Count);

	//start upload sequence
	DirectX::ResourceUploadBatch uploadBatch {m_device.Get()};
	uploadBatch.Begin();

	//describing our render target real quick -- putting this here since it needs to be done
	//with the upload batch
	DirectX::RenderTargetState targetState =
	{
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN
	};

	DirectX::SpriteBatchPipelineStateDescription batchDesc{ targetState };
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_device.Get(), uploadBatch, batchDesc);

	//load the texture
	for (auto& k : textPaths)
	{
		DirectX::CreateWICTextureFromFile(m_device.Get(), uploadBatch, k.second.c_str(), &m_texture[k.first]);

		DirectX::CreateShaderResourceView(m_device.Get(), m_texture[k.first].Get(), m_dsvHeap->GetCpuHandle(
			k.first));
	}

	//wait for all the textures to upload and end upload sequence
	std::future<void> resourcesFinished = uploadBatch.End(m_comQ.Get());
	resourcesFinished.wait();

	//bc of comptrs we do not need to explicitly unload this
}

/************************************************
*
* initializing all the stuff for dx12
* returns either all good or error value
*
************************************************/
HRESULT Graphics::Init(GLFWwindow* _window)
{
	m_scale = { 1,1 };

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

	//for each buffer
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

	m_memory = std::make_unique<DirectX::GraphicsMemory>(m_device.Get());

	//load the textures we will use
	LoadTexture();

	return hr;
}

/************************************************
*
* gets ready for the drawing, clearing anything
* that is needed, start recording, etc
*
************************************************/
void Graphics::StartDraw(void)
{
	//clearing current variables associates with current buffer/frame

	ComPtr<ID3D12Fence>& fence = m_fence[m_currentFrame];

	if (fence->GetCompletedValue() < m_fenceValue[m_currentFrame])
	{
		fence->SetEventOnCompletion(m_fenceValue[m_currentFrame], m_fenceEvent[m_currentFrame]);
		WaitForSingleObject(m_fenceEvent[m_currentFrame], INFINITE);
	}

	ComPtr<ID3D12GraphicsCommandList>& command = m_comList[m_currentFrame];

	m_comAllocator[m_currentFrame]->Reset();
	command->Reset(m_comAllocator[m_currentFrame].Get(), nullptr);

	//create and set the viewport
	D3D12_VIEWPORT viewport =
	{
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = static_cast<FLOAT>(m_width),
		.Height = static_cast<FLOAT>(m_height),
		.MinDepth = 0,
		.MaxDepth = 1
	};

	command->RSSetViewports(1, &viewport);
	m_spriteBatch->SetViewport(viewport);

	D3D12_RECT rect =
	{
		.left = 0,
		.top = 0,
		.right = m_width,
		.bottom = m_height
	};

	command->RSSetScissorRects(1, &rect);

	//the color we will clear the screen with
	const FLOAT clearColor[] = { 0,0,0,1 };

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCpuHandle(m_currentFrame);

	//get into the drawable state
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_swapChainBuffers[m_currentFrame].Get(), D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	command->ResourceBarrier(1, &barrier);

	//clear this so we can draw on it
	command->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	command->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	ID3D12DescriptorHeap* heaps[] = { m_dsvHeap->Heap() };

	command->SetDescriptorHeaps(1, heaps);

	//start record
	m_spriteBatch->Begin(command.Get());

	//we are ready to draw now
}


/************************************************
*
* the draw function! used by everything/anything
* outside of graphics to draw one of the textures
* loaded in a simple/easy way
*
************************************************/
void Graphics::Draw(Textures _texture, DirectX::SimpleMath::Vector2 _pos, DirectX::SimpleMath::Vector2 _imageSize)
{
	DirectX::XMUINT2 imageSize = { static_cast<uint32_t>(_imageSize.x), static_cast<uint32_t>(_imageSize.y)};
	DirectX::SimpleMath::Vector2 origin = { imageSize.x / 2.0f, imageSize.y / 2.0f };

	m_spriteBatch->Draw(m_dsvHeap->GetGpuHandle(_texture), imageSize, _pos,
		nullptr, DirectX::Colors::White, 0.0f, origin, m_scale);
}

/************************************************
*
* stop drawing! stop recording and send all the 
* drawing data up to the screen
*
************************************************/
void Graphics::EndDraw(void)
{
	//stop record
	m_spriteBatch->End();

	ComPtr<ID3D12GraphicsCommandList>& command = m_comList[m_currentFrame];

	//make me able to present, im done drawing
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_swapChainBuffers[m_currentFrame].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	command->ResourceBarrier(1, &barrier);

	command->Close();

	//send out the draw commands

	ID3D12CommandList* lists[] = { command.Get() };

	m_comQ->ExecuteCommandLists(1, lists);

	m_memory->Commit(m_comQ.Get());

	++m_fenceValue[m_currentFrame];
	m_comQ->Signal(m_fence[m_currentFrame].Get(), m_fenceValue[m_currentFrame]);

	m_swapChain->Present(1, 0);

	++m_SCFenceValue;
	m_comQ->Signal(m_SCFence.Get(), m_SCFenceValue);

	//switch buffer for next frame
	++m_currentFrame;
	m_currentFrame %= m_frameCount;
}

/************************************************
*
* shutdown! really just have to make sure the
* fences are done, comptrs takes care of the rest
*
************************************************/
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

/************************************************
*
* get an instance of the graphics 
*
************************************************/
Graphics& Graphics::GetInstance()
{
	static Graphics graphics;
	return graphics;
}