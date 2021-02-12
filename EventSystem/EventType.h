#pragma once
enum class EventType { WindowCloseEvent = 0, GetRenderObjectsEvent, SendRenderObjectsEvent, AskForRenderObjectsEvent,
					   UnbindPipelineEvent, ToggleWireFrameEvent, BindIDEvent, WindowResizeEvent, MouseMoveAbsoluteEvent, MouseButtonEvent, MouseScrollEvent, KeyboardEvent,
					   ToggleImGuiEvent, DelegateDXEvent, RequestCameraEvent, DelegateCameraEvent, ToggleDepthStencilStateEvent, MouseMoveRelativeEvent
};