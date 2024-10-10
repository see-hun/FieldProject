#include "FieldCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

// Sets default values
//������Ʈ
AFieldCameraPawn::AFieldCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	UE_LOG(LogTemp, Warning, TEXT("AFieldCameraPawnSPawn"));
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1000.0f;
	SpringArmComponent->bDoCollisionTest = false;


	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	

}

// Called every frame
//���� ���� 
void AFieldCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraBounds();
	const FVector InterpolatedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpolatedLocation);

	const float InterpolatedZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	SpringArmComponent->TargetArmLength = InterpolatedZoom;

	const FRotator InterpolatedRotation = UKismetMathLibrary::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotation, DeltaTime, RotateSpeed);
	SpringArmComponent->SetRelativeRotation(InterpolatedRotation);
}

// Called when the game starts or when spawned
//���۽� �÷��̾� ��ŸƮ ��ġ��ȸ���� ����
void AFieldCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (PlayerStart)
	{
		SaveLocation = PlayerStart->GetActorLocation(); // �÷��̾� ��ŸƮ ��ġ ����

		const FRotator Rotation = SpringArmComponent->GetRelativeRotation();
		SaveRotation = FRotator(Rotation.Pitch + -50, Rotation.Yaw, 0.0f); // �÷��̾� ��ŸƮ ȸ�� ����

		TargetLocation = SaveLocation; // �ʱ� ��ġ ����
		TargetRotation = SaveRotation; // �ʱ� ȸ�� ����
		TargetZoom = 1000.0f;
	}
	/*TargetLocation = GetActorLocation();
	const FRotator Rotation = SpringArmComponent->GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch + -50, Rotation.Yaw, 0.0f);
	TargetZoom = 1000.0f;*/
}
	


// Forward and Right movement
//W,S �ε� �ܱ��
void AFieldCameraPawn::Forward(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Forward:%f"), AxisValue);
	TargetLocation = SpringArmComponent->GetForwardVector() * AxisValue * MoveSpeed + TargetLocation;
	
}
//D,A 45�� ȸ�����
void AFieldCameraPawn::Right(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Right:%f"), AxisValue);
	//TargetLocation = SpringArmComponent->GetRightVector() * AxisValue * MoveSpeed + TargetLocation;
	 TargetRotation = UKismetMathLibrary::ComposeRotators(SpringArmComponent->GetRelativeRotation(), FRotator(0.0f, AxisValue * -45.0f, 0.0f));

    // ȸ�� ���� Ȱ��ȭ
    CanRotate = true;
}

// Zoom functionality
//���콺 �� ��
void AFieldCameraPawn::Zoom(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}
	const float Zoom = AxisValue * 100.0f;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, ZoomMin, ZoomMax);
}

// Rotation functionality
//E������ 45�� ȸ��
void AFieldCameraPawn::RotateRight()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, 45.0f, 0.0f));
	
}
//Q������ -45�� ȸ��
void AFieldCameraPawn::RotateLeft()
{
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, -45.0f, 0.0f));
}

//ȸ������ üũ
void AFieldCameraPawn::EnableRotate()
{
	CanRotate = true;
}
//ȸ������ üũ
void AFieldCameraPawn::DisableRotate()
{
	CanRotate = false;
}

//���콺 ���� ���� ���������� �ʿ��� �ּ�ó���� �Ⱦ��±��
void AFieldCameraPawn::RotateHorizontal(float AxisValue)
{
	/*if (AxisValue == 0.0f)
	{
		return;
	}
	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, AxisValue, 0.0f));
	}*/
}

void AFieldCameraPawn::RotateVertical(float AxisValue)
{
	/*if (AxisValue == 0.0f)
	{
		return;
	}
	if (CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(AxisValue, 0.0f, 0.0f));
	}*/
}

// Restrict camera pitch and location bounds
//ī�޶� ���� ����
void AFieldCameraPawn::CameraBounds()
{
	float NewPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch < (RotatePitchMax * -1))
	{
		NewPitch = (RotatePitchMax * -1);
	}
	else if (TargetRotation.Pitch > (RotatePitchMin * -1))
	{
		NewPitch = (RotatePitchMin * -1);
	}
	TargetRotation = FRotator(NewPitch, TargetRotation.Yaw, 0.0f);
	FVector(TargetLocation.X, TargetLocation.Y, 0.0f);
}
//Z ������ ��� ���� �ƿ��������̶���Ʈ ����
void AFieldCameraPawn::PowerOff()
{

	// ���� ������ ��� ������ CustomDepth�� false�� ����
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->GetName().Contains(TEXT("BP_Test_C")))
		{
			TArray<UPrimitiveComponent*> Components;
			Actor->GetComponents<UPrimitiveComponent>(Components);
			for (UPrimitiveComponent* PrimitiveComponent : Components)
			{
				if (PrimitiveComponent)
				{
					PrimitiveComponent->SetRenderCustomDepth(false);
				}
			}
		}
	}
}

//FocusOnAllBPTestC�Լ� ���� ���� ���͵� �ƿ����� ���̶�Ʈ���� �� ���͵� ���� �Ÿ� �߽����� �̵� 
void AFieldCameraPawn::FocusOnActors(TArray<AActor*>& TargetActors)
{
	
	PowerOff();
	if (TargetActors.Num() > 0)
	{
		FVector CenterLocation = FVector::ZeroVector;
		int32 ActorCount = 0;

		for (AActor* TargetActor : TargetActors)
		{
			if (TargetActor)
			{
				CenterLocation += TargetActor->GetActorLocation();
				ActorCount++;

				// BP_Test_C�� ������ ���͸� CustomDepth�� true�� ����
				TArray<UPrimitiveComponent*> Components;
				TargetActor->GetComponents<UPrimitiveComponent>(Components);
				for (UPrimitiveComponent* PrimitiveComponent : Components)
				{
					if (PrimitiveComponent)
					{
						PrimitiveComponent->SetRenderCustomDepth(true);
					}
				}
			}
		}

		if (ActorCount > 0)
		{
			CenterLocation /= ActorCount;
		}
		TargetLocation = CenterLocation;
		bOutLineHightRight = true;
	}
}



//FocusOnBPTestC1 ���� �����̸� �ƿ���������� �ش� ���ͷ��̵�
void AFieldCameraPawn::FocusOnActor2(AActor* TargetActor)
{
	PowerOff();

	// BP_Test_C_1�� CustomDepth�� true�� ����
	if (TargetActor)
	{
		TArray<UPrimitiveComponent*> Components;
		TargetActor->GetComponents<UPrimitiveComponent>(Components);
		for (UPrimitiveComponent* PrimitiveComponent : Components)
		{
			if (PrimitiveComponent)
			{
				PrimitiveComponent->SetRenderCustomDepth(true);
			}
		}
		TargetLocation = TargetActor->GetActorLocation();
	}
}
//FocusOnBPTestC0 ���� �����̸� �ƿ���������� �ش� ���ͷ��̵�
void AFieldCameraPawn::FocusOnActor3(AActor* TargetActor)
{
	PowerOff();

	// BP_Test_C_0�� CustomDepth�� true�� ����
	if (TargetActor)
	{
		TArray<UPrimitiveComponent*> Components;
		TargetActor->GetComponents<UPrimitiveComponent>(Components);
		for (UPrimitiveComponent* PrimitiveComponent : Components)
		{
			if (PrimitiveComponent)
			{
				PrimitiveComponent->SetRenderCustomDepth(true);
			}
		}
		TargetLocation = TargetActor->GetActorLocation();
	}
}

//1��Ű ������ BP_Test_C ������ ���� ã��
void AFieldCameraPawn::FocusOnAllBPTestC()
{
	TArray<AActor*> FoundActors;
	FoundActors.Empty();
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->GetName().Contains(TEXT("BP_Test_C")))
		{
			FoundActors.Add(*It);
		}
	}
	FocusOnActors(FoundActors);
}

//2��Ű ������ BP_Test_C_1 �̸��� ���� ���� ã��
void AFieldCameraPawn::FocusOnBPTestC1()
{
	AActor* SpecificActor = nullptr;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->GetName().Equals(TEXT("BP_Test_C_1")))
		{
			SpecificActor = *It;
			break;
		}
	}
	if (SpecificActor)
	{
		FocusOnActor2(SpecificActor);
	}
}

//3��Ű ������ BP_Test_C_0 �̸��� ���� ���� ã��
void AFieldCameraPawn::FocusOnBPTestC0()
{
	AActor* SpecificActor = nullptr;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->GetName().Equals(TEXT("BP_Test_C_0")))
		{
			SpecificActor = *It;
			break;
		}
	}
	if (SpecificActor)
	{
		FocusOnActor3(SpecificActor);
	}
}

//F ������ �÷��̾� ��ŸƮ �������� �̵�
void AFieldCameraPawn::GoPlayerStartposition()
{

	
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	if (PlayerStart)
	{
		PowerOff();
		TargetLocation = PlayerStart->GetActorLocation(); // �÷��̾� ��ŸƮ ��ġ�� �̵�
		TargetRotation = SaveRotation; // �÷��̾� ��ŸƮ ȸ������ ȸ��
		UE_LOG(LogTemp, Warning, TEXT("Moved to Player Start at: %s"), *TargetLocation.ToString());
	}

}
// Called to bind functionality to input
void AFieldCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AFieldCameraPawn::Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AFieldCameraPawn::Right);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AFieldCameraPawn::Zoom);
	PlayerInputComponent->BindAxis(TEXT("RotateHorizontal"), this, &AFieldCameraPawn::RotateHorizontal);
	PlayerInputComponent->BindAxis(TEXT("RotateVertical"), this, &AFieldCameraPawn::RotateVertical);

	PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed, this, &AFieldCameraPawn::RotateRight);
	PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed, this, &AFieldCameraPawn::RotateLeft);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Pressed, this, &AFieldCameraPawn::EnableRotate);
	PlayerInputComponent->BindAction(TEXT("Rotate"), IE_Released, this, &AFieldCameraPawn::DisableRotate);
	//PlayerInputComponent->BindAction(TEXT("FocusOnActor"), IE_Pressed, this, &AFieldCameraPawn::FocusOnTarget);
	PlayerInputComponent->BindAction(TEXT("FocusOnActors"), IE_Pressed, this, &AFieldCameraPawn::FocusOnAllBPTestC);
	PlayerInputComponent->BindAction(TEXT("FocusOnActor2"), IE_Pressed, this, &AFieldCameraPawn::FocusOnBPTestC1);
	PlayerInputComponent->BindAction(TEXT("FocusOnActor3"), IE_Pressed, this, &AFieldCameraPawn::FocusOnBPTestC0);
	PlayerInputComponent->BindAction(TEXT("PowerOff"), IE_Pressed, this, &AFieldCameraPawn::PowerOff);
	PlayerInputComponent->BindAction(TEXT("GoPlayerStartposition"), IE_Pressed, this, &AFieldCameraPawn::GoPlayerStartposition);
}




