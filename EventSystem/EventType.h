#pragma once
enum class EventType { WindowCloseEvent = 0, GetRenderObjectsEvent, SendRenderObjectsEvent, AskForRenderObjectsEvent, DelegateResolutionEvent,
					   UnbindPipelineEvent, ToggleWireFrameEvent, BindIDEvent, WindowResizeEvent, MouseMoveAbsoluteEvent, MouseButtonEvent, MouseScrollEvent, KeyboardEvent,
					   ToggleImGuiEvent, DelegateDXEvent, RequestCameraEvent, DelegateCameraEvent, ToggleDepthStencilStateEvent, MouseMoveRelativeEvent, DelegateMouseCoordsEvent,
					   DelegateSunLightEvent, RequestSunLightEvent, ResetDefaultViewportEvent, CreateShadowMapViewportEvent, SetShadowMapViewportEvent, SetShadowBlendStateEvent, ResetDefaultBlendStateEvent
};