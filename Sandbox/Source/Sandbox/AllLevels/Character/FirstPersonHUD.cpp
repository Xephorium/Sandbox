
#include "AllLevels/Character/FirstPersonHUD.h"

/*
 *  FirstPersonHUD.cpp                                  Chris Cruzen
 *  Sandbox                                               02.12.2023
 *
 *    FirstPersonHUD contains the UI elements and behavior functions
 *  needed for FirstPersonController.
 */


/*--- Lifecycle Functions ---*/

void AFirstPersonHUD::BeginPlay() {
	Super::BeginPlay();

	if (ControllerDiagnosticWidgetClass) {
		ControllerDiagnosticWidget = CreateWidget<UControllerDiagnosticWidget>(GetWorld(), ControllerDiagnosticWidgetClass);
	}
}


/*--- HUD Control Functions ---*/

void AFirstPersonHUD::ToggleControllerDiagnosticWidget() {
	if (IsControllerDiagnosticShown) {
		IsControllerDiagnosticShown = false;
		HideControllerDiagnosticWidget();
	} else {
		IsControllerDiagnosticShown = true;
		ShowControllerDiagnosticWidget();
	}
}

void AFirstPersonHUD::ShowControllerDiagnosticWidget() {
	if (ControllerDiagnosticWidget && !ControllerDiagnosticWidget->IsVisible()) {
        ControllerDiagnosticWidget->AddToViewport();
        GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
    }
}

void AFirstPersonHUD::HideControllerDiagnosticWidget() {
	if (ControllerDiagnosticWidget && ControllerDiagnosticWidget->IsVisible()) {
        ControllerDiagnosticWidget->RemoveFromViewport();
    }
}