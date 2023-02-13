
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
	void OnStickLeftPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnStickLeftRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnStickRightPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnStickRightRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnStartPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnStartRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnEndPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnEndRelease();

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

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnBumperLeftPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnBumperLeftRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnBumperRightPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnBumperRightRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadUpPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadUpRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadLeftPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadLeftRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadRightPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadRightRelease();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadDownPress();

	public: UFUNCTION(BlueprintImplementableEvent)
	void OnDPadDownRelease();
};
