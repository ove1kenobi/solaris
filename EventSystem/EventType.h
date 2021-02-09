#pragma once
enum class EventType { WindowCloseEvent = 0, GetRenderObjectsEvent, SendRenderObjectsEvent, AskForRenderObjectsEvent,
					   UnbindPipelineEvent, ToggleWireFrameEvent, BindIDEvent, WindowResizeEvent, MouseMoveAbsoluteEvent, 
					   MouseMoveRelativeEvent, MouseButtenEvent, MouseScrollEvent, KeyboardEvent,
					   ToggleImGuiEvent, DelegateDXEvent
};