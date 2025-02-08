// Fill out your copyright notice in the Description page of Project Settings.


#include "Skateboard.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

// Sets default values
ASkateboard::ASkateboard()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Создаём статическую меш-доску
    SkateboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateboardMesh"));
    RootComponent = SkateboardMesh;

    // Включаем физику
    SkateboardMesh->SetSimulatePhysics(true);
    SkateboardMesh->SetMassOverrideInKg(NAME_None, 5.0f);
    SkateboardMesh->SetLinearDamping(0.2f);
    SkateboardMesh->SetAngularDamping(1.5f);

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void ASkateboard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkateboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    bIsGrounded = CheckForGrounding();
    Stabilization();
}

void ASkateboard::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Called to bind functionality to input
void ASkateboard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASkateboard::Jump);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateboard::Move);
    }
}

void ASkateboard::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (!bIsGrounded) return;

    if (MovementVector.Y != 0.0f)
    {
        FVector Force = GetActorForwardVector() * MovementVector.Y * MoveForce;
        print(FString::Printf(TEXT("Force value: (%f, %f, %f)"), Force.X, Force.Y, Force.Z));
        SkateboardMesh->AddForce(Force);
    }

    if (MovementVector.X != 0.0f)
    {
        FVector Torque = FVector(0.0f, 0.0f, MovementVector.X * TurnTorque);
        print(FString::Printf(TEXT("Torque value: (%f, %f, %f)"), Torque.X, Torque.Y, Torque.Z));
        SkateboardMesh->AddTorqueInDegrees(Torque);
    }
}

void ASkateboard::Jump()
{
    if (bIsGrounded)
    {
        SkateboardMesh->AddImpulse(FVector(0.0f, 0.0f, JumpImpulse));
    }
}

bool ASkateboard::CheckForGrounding()
{
    FVector Start = GetActorLocation();
    FVector End = Start - FVector(0, 0, 50.0f);
    FHitResult HitResult;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    return GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
}

void ASkateboard::Stabilization()
{
    if (bIsGrounded)
    {
        FVector UpVector = FVector(0, 0, 1);
        FVector BoardUp = SkateboardMesh->GetUpVector();
        FVector StabilizationTorque = FVector::CrossProduct(BoardUp, UpVector) * StabilityForce;
        SkateboardMesh->AddTorqueInDegrees(StabilizationTorque);
    }
}

