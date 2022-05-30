#pragma once

DECLARE_DELEGATE_OneParam(MoveForwardDelegate, float)
DECLARE_DELEGATE_OneParam(MoveRightDelegate, float)
DECLARE_DELEGATE_TwoParams(LookDelegate, float, float)
DECLARE_DELEGATE(JumpPressDelegate)
DECLARE_DELEGATE(JumpReleaseDelegate)