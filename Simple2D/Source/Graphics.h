#pragma once
/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : Graphics.h 
* Purpose: singleton for Dx12 implementation
*
**********************************************************************************************************************/
#include <wrl.h>                         //how to get com pointers
#include <dxgi1_4.h>                     //dx generic interface
#include <d3d12.h>                       //thats the good stuff (the meat of the dx12)
#include<GLFW/glfw3.h>                   // for window
#include<directxtk12/DescriptorHeap.h>   //descriptor heaps and piles
#include <memory>                        //unique ptr
#include<directxtk12/SpriteBatch.h>      //sprite batch
#include<directxtk12/GraphicsMemory.h>   //graphics memory
#include <directxtk12/SimpleMath.h>      //vector2, xmunint2

using namespace Microsoft::WRL;          //for ease with comptrs

//a singleton
class Graphics
{
private:
	explicit Graphics() {}

	Graphics(const Graphics&) = delete;
	void operator=(const Graphics&) = delete;

	HRESULT CreateSwapChain(void);

	void LoadTexture();

public:
	//for easy reference to all of the textures we have loaded
	enum Textures
	{
		player,
		enemy,
		wall,
		cheese,
		win,
		lose,
		Count
	};

	HRESULT Init(GLFWwindow* _window);

	void StartDraw(void);
	void Draw(Textures _texture, DirectX::SimpleMath::Vector2 _pos, 
		DirectX::SimpleMath::Vector2 _imageSize);
	void EndDraw(void);

	void Shutdown(void);
	static Graphics& GetInstance();

private:
	static constexpr int m_frameCount = 2; //only want to use 2 buffers
	int m_currentFrame = 0;                //current buffer

	GLFWwindow* m_window;                  //the window, so we dont have to keep getting it
	int m_width;                           //width of the window
	int m_height;                          //height of the window

	DirectX::SimpleMath::Vector2 m_scale;  //scale that all objects will be drawn -> 100%

	//init stuff / draw stuff
	ComPtr<IDXGIFactory3> m_factory;    //used to create objects
	ComPtr<IDXGIAdapter3> m_adapter;    //represents the gpu
	ComPtr<ID3D12Device> m_device;      //d3d functionality

	ComPtr<ID3D12CommandQueue> m_comQ;                        //used to send commands to gpu
	ComPtr<IDXGISwapChain1> m_swapChain;                      //how draw things to screen
	ComPtr<ID3D12Resource> m_swapChainBuffers[m_frameCount];  //buffers i draw to
	
	std::unique_ptr<DirectX::DescriptorPile> m_rtvHeap;          //stores the descriptions of the views
	ComPtr<ID3D12CommandAllocator> m_comAllocator[m_frameCount]; //allocates commands on gpu
	ComPtr<ID3D12GraphicsCommandList> m_comList[m_frameCount];   //list of commands to toss to gpu

	//these are for the commandlists
	ComPtr<ID3D12Fence> m_fence[m_frameCount]; //used to sychronize to the gpu
	HANDLE m_fenceEvent[m_frameCount];         //notifies when fence is done
	int m_fenceValue[m_frameCount];            //fence finished value

	//these are for the swap chain in particular
	ComPtr<ID3D12Fence> m_SCFence; 
	HANDLE m_SCFenceEvent;
	int m_SCFenceValue;

	//for texture loading
	std::unique_ptr<DirectX::DescriptorHeap> m_dsvHeap; 
	ComPtr<ID3D12Resource> m_texture[Textures::Count];

	//for drawing
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::GraphicsMemory> m_memory;
};