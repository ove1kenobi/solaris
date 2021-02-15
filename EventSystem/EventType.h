#pragma once
enum class EventType { WindowCloseEvent = 0, GetRenderObjectsEvent, SendRenderObjectsEvent, AskForRenderObjectsEvent,
					   UnbindPipelineEvent, ToggleWireFrameEvent, BindIDEvent, WindowResizeEvent, MouseMoveAbsoluteEvent, MouseButtenEvent, MouseScrollEvent, KeyboardEvent,
					   ToggleImGuiEvent, DelegateDXEvent, RequestCameraEvent, DelegateCameraEvent, ToggleDepthStencilStateEvent, MouseMoveRelativeEvent, RequestDSVEvent, SendDSVEvent,
					   BindBackBufferEvent
};