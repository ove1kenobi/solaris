#pragma once
enum class EventType { WindowCloseEvent = 0, GetRenderObjectsEvent, SendRenderObjectsEvent, AskForRenderObjectsEvent, DelegateResolutionEvent,
					   UnbindPipelineEvent, ToggleWireFrameEvent, BindIDEvent, WindowResizeEvent, MouseMoveAbsoluteEvent, MouseButtonEvent, MouseScrollEvent, KeyboardEvent,
					   DelegateDXEvent, RequestCameraEvent, DelegateCameraEvent, ToggleDepthStencilStateEvent, MouseMoveRelativeEvent, DelegateMouseCoordsEvent,
					   DelegateSunLightEvent, RequestSunLightEvent, RequestDSVEvent, SendDSVEvent,
					   BindBackBufferEvent, RequestSunEvent, DelegateSunEvent, ResetDefaultViewportEvent, CreateShadowMapViewportEvent, SetShadowMapViewportEvent, SetShadowBlendStateEvent, ResetDefaultBlendStateEvent, DelegatePlanetDistanceEvent,
					   PlaySoundEvent, SetMusicEvent, StopLoopingSoundEvent, DelegatePlayerInfoEvent, ToggleControlsEvent, ToggleTetheredEvent, ToggleStartGame,
					   ToggleDamageHUD, ClearBackBufferEvent, DelegateUpgradeID
};