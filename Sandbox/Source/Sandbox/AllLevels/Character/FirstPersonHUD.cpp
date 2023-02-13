
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

		if (ControllerDiagnosticWidget) {
			ControllerDiagnosticWidget->AddToViewport();
		}
	}
}

void AFirstPersonHUD::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

    if (!ControllerDiagnosticWidget->IsVisible()) {
        ControllerDiagnosticWidget->AddToViewport();
        GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
    }
}

void AFirstPersonHUD::DrawHUD() {
	Super::DrawHUD();
}


/*--- Update Functions ---*/

void AFirstPersonHUD::OnFaceTopPress() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceTopPress();
	}
}

void AFirstPersonHUD::OnFaceTopRelease() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceTopRelease();
	}
}

void AFirstPersonHUD::OnFaceRightPress() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceRightPress();
	}
}

void AFirstPersonHUD::OnFaceRightRelease() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceRightRelease();
	}
}

void AFirstPersonHUD::OnFaceLeftPress() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceLeftPress();
	}
}

void AFirstPersonHUD::OnFaceLeftRelease() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceLeftRelease();
	}
}

void AFirstPersonHUD::OnFaceBottomPress() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceBottomPress();
	}
}

void AFirstPersonHUD::OnFaceBottomRelease() {
    if (ControllerDiagnosticWidget) {
		ControllerDiagnosticWidget->OnFaceBottomRelease();
	}
}