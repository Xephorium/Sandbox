#pragma once

/*
 *  SteamInputComponentDelegates.h                            Chris Cruzen
 *  Sandbox                                                     06.02.2022
 *
 *  This file defines the delegates needed for SteamInputComponent.
 * 
 *  Delegate Notes
 *    - Basically a callback! You can .Bind() any function (with a matching
 *      signature) and then call that bound function via .Execute(<params>)
 *      as needed. SUPER useful for handling asynchronous events.
 *    - Worth noting that each Delegate is considered a user-defined type
 *      by C++.
 */

// Can bind a function w/ no parameters
DECLARE_DELEGATE(InputDelegate);

// Can bind a function w/ one float parameter
DECLARE_DELEGATE_OneParam(FloatInputDelegate, float);

// Can bind a function w/ one FVector2D parameter
DECLARE_DELEGATE_OneParam(VectorInputDelegate, FVector2D);