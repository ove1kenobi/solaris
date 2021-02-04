#pragma once
enum class EventType { WindowCloseEvent = 0, TestEvent, GetRenderObjectsEvent, SendRenderObjectsEvent, AskForRenderObjectsEvent,
					   UnbindPipelineEvent, ToggleWireFrameEvent, BindIDEvent, WindowResizeEvent, MouseMoveEvent, MouseButtenEvent, KeyboardEvent,
					   ToggleImGuiDemoWindowEvent
};