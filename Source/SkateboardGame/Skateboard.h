// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Skateboard.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SKATEBOARDGAME_API ASkateboard : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkateboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Values);
	void Jump();
	bool CheckForGrounding();
	void Stabilization();

	// Компоненты
	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* SkateboardMesh;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Настройки физики
	UPROPERTY(EditAnywhere, Category = "Physics")
	float MoveForce = 6000.0f;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float TurnTorque = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float JumpImpulse = 45000.0f;

	UPROPERTY(EditAnywhere, Category = "Physics")
	float StabilityForce = 500.0f;

	// Проверка на земле
	bool bIsGrounded;

};
