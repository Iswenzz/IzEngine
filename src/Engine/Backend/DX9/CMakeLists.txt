target_precompile_headers(IzEngine PRIVATE
	Base.hpp
	Common.hpp)

target_sources(IzEngine PRIVATE
	Base.hpp
	Buffer.hpp
	Buffer.cpp
	Common.hpp
	Draw2D.cpp
	Font.hpp
	Font.cpp
	FrameBuffer.hpp
	FrameBuffer.cpp
	GraphicsContext.hpp
	GraphicsContext.cpp
	PipelineState.hpp
	PipelineState.cpp
	RendererAPI.hpp
	RendererAPI.cpp
	Shader.hpp
	Shader.cpp
	StateBlock.hpp
	StateBlock.cpp
	Texture.hpp
	Texture.cpp
	UniformBuffer.hpp
	UniformBuffer.cpp
	VertexArray.hpp
	VertexArray.cpp)

if(BUILD_CEF)
	target_sources(IzEngine PRIVATE Browser.cpp)
endif()
