#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "ControllerDiagnosticWidget.h"
#include "FirstPersonHUD.generated.h"

/*
 *  FirstPersonHUD.h                                   Chris Cruzen
 *  Sandbox                                              02.12.2023
 *
 *  Header file for FirstPersonHUD.h.
 */

UCLASS()
class SANDBOX_API AFirstPersonHUD : public AHUD {
	
	GENERATED_BODY()


	/*--- Variables ---*/

	public: UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UControllerDiagnosticWidget> ControllerDiagnosticWidgetClass;

	private: UControllerDiagnosticWidget* ControllerDiagnosticWidget;


	/*--- Lifecycle Functions ---*/

	public: virtual void BeginPlay() override;

	public: virtual void DrawHUD() override;

	public: virtual void Tick(float DeltaSeconds) override;


	/*--- Behavior Functions ---*/

	public: void OnFaceTopPress();

	public: void OnFaceTopRelease();

	public: void OnFaceRightPress();

	public: void OnFaceRightRelease();

	public: void OnFaceLeftPress();

	public: void OnFaceLeftRelease();

	public: void OnFaceBottomPress();

	public: void OnFaceBottomRelease();

};
