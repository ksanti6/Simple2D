#pragma once
#include <wrl.h> //how to get com poitners
#include <dxgi1_4.h> //dx generic interface
#include <d3d12.h> //thats the good stuff
#include<GLFW/glfw3.h> // for window
#include<directxtk12/DescriptorHeap.h> //
#include <memory>

using namespace Microsoft::WRL;

//a singleton
class Graphics
{
private:
	explicit Graphics() {}

	Graphics(const Graphics&) = delete;
	void operator=(const Graphics&) = delete;

	HRESULT CreateSwapChain(void);

public:
	HRESULT Init(GLFWwindow* _window);
	void Shutdown(void);
	static Graphics& GetInstance();

private:
	static constexpr int m_frameCount = 2; //only want to use 2 buffers
	GLFWwindow* m_window;

	ComPtr<IDXGIFactory3> m_factory;    //used to create objects
	ComPtr<IDXGIAdapter3> m_adapter;    //represents the gpu
	ComPtr<ID3D12Device> m_device;      //d3d functionality

	ComPtr<ID3D12CommandQueue> m_comQ;  //used to send commands to gpu
	ComPtr<IDXGISwapChain1> m_swapChain; //how draw things to screen
	ComPtr<ID3D12Resource> m_swapChainBuffers[m_frameCount]; //buffers i draw to
	
	std::unique_ptr<DirectX::DescriptorPile> m_rtvHeap; //stores the descriptions of the views
	ComPtr<ID3D12CommandAllocator> m_comAllocator[m_frameCount]; //allocates commands on gpu
	ComPtr<ID3D12GraphicsCommandList> m_comList[m_frameCount]; //list of commands to toss to gpu

	//these are for the commandlists
	ComPtr<ID3D12Fence> m_fence[m_frameCount]; //used to sychronize to the gpu
	HANDLE m_fenceEvent[m_frameCount]; //notifies when fence is done
	int m_fenceValue[m_frameCount]; //fence finished value

	//these are for the swap chain in particular
	ComPtr<ID3D12Fence> m_SCFence; 
	HANDLE m_SCFenceEvent;
	int m_SCFenceValue;
};