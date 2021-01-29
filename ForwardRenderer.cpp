#include "ForwardRenderer.h"

//NOTE: pointers probably are gonna have to be adjusted to be handled by the functions correctly,
//can also be removed completelly based on the structure of the event handler. -steph

ForwardRenderer::ForwardRenderer() noexcept {
	this->m_Background = { 0, 0, 0, 1 };
	this->m_Bloom = Bloom();
}

//Submits what objects that will be rendered, takes a list of gameObjects as argument
void ForwardRenderer::SubmitObject() {
	//Saves them in the vector here
}


//Sets everything up for forward rendering, takes information from the event handler as input
ID3D11RenderTargetView* ForwardRenderer::BeginFrame(ID3D11RenderTargetView* renderTarget) {
	/*information that is needed from the event handler and resource manager:
	* - what buffers will be used during this render? (Example: WVP and light buffers)
	* - what shaders will be used during this render? (Example: vertex, geometry, pixel)
	* - what renderTarget should we write to? (Will be the one that gets returned as well)
	*/


	//Clear the render target and set background

	//techniques that need to apply before render happens here

	//Set buffers that will be used during this render pass

	//Set shaders that will be used during this render pass

	//Render objects from the saved vector

	//Return frame fo post processing
	return renderTarget;
}

//Cleans up for the next frame and applies post processing effects
ID3D11RenderTargetView* ForwardRenderer::EndFrame(ID3D11RenderTargetView* renderTarget) {
	/*information that is needed from the event handler and resource manager:
	* - what renderTarget should we write to? (Will be the one that gets returned as well)
	*/

	//Empty the submit vector for next frame

	//Apply the post processing effects to the renderTarget here

	//Return the finished frame
	return renderTarget;
}

//Calls private functions to render the frame
ID3D11RenderTargetView* ForwardRenderer::RenderFrame(ID3D11RenderTargetView* renderTarget) {
	//return the finished render to the Engine after applying BeginFrame and then Endframe to it.
	return this->EndFrame(this->BeginFrame(renderTarget));
}
