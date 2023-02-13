
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControllerDiagnosticWidget.generated.h"

/*
 *  ControllerDiagnosticWidget.h                       Chris Cruzen
 *  Sandbox                                              02.10.2023
 *
 *  Header file for ControllerDiagnosticWidget.h.
 */

UCLASS()
class SANDBOX_API UControllerDiagnosticWidget : public UUserWidget {

	GENERATED_BODY()
	

	/*--- Lifecycle Functions---*/

	public: virtual void NativeConstruct() override;


	/*--- Update Functions ---*/

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceTopPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceTopRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceRightPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceRightRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceLeftPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceLeftRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceBottomPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnFaceBottomRelease();
};
