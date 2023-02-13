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

	private: bool IsControllerDiagnosticShown = false;


	/*--- Lifecycle Functions ---*/

	public: virtual void BeginPlay() override;


	/*--- HUD Control Functions ---*/

	public: void ToggleControllerDiagnosticWidget();

	public: void ShowControllerDiagnosticWidget();

	public: void HideControllerDiagnosticWidget();

};
