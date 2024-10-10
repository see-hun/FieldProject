// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FieldCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
UCLASS()
class OPCUA_UNREAL_CLIENT_API AFieldCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFieldCameraPawn();

	// Called every frame
	//���� ���� 
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//InputMapping ����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//F ������ �÷��̾� ��ŸƮ �������� �̵�
	void GoPlayerStartposition();

	//Z ������ ��� ���� �ƿ��������̶���Ʈ ����
	UFUNCTION()
	void PowerOff();
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//W,S �ε� �ܱ��
	UFUNCTION()
	void Forward(float AxisValue);

	//D,A 45�� ȸ�����
	UFUNCTION()
	void Right(float AxisValue);

	//���콺 �� ��
	UFUNCTION()
	void Zoom(float AxisValue);

	//E������ 45�� ȸ��
	UFUNCTION()
	void RotateRight();

	//Q������ -45�� ȸ��
	UFUNCTION()
	void RotateLeft();
	//ȸ������ üũ
	UFUNCTION()
	void EnableRotate();
	//ȸ������ üũ
	UFUNCTION()
	void DisableRotate();

	
	//�ƿ����� ������ִ��� Ȯ��
	UPROPERTY()
	bool bOutLineHightRight = false;

	//���콺 ���� ���� ���������� �ʿ��� �ּ�ó���� �Ⱦ��±��
	UFUNCTION()
	void RotateHorizontal(float AxisValue);
	//���콺 ���� ���� ���������� �ʿ��� �ּ�ó���� �Ⱦ��±��
	UFUNCTION()
	void RotateVertical(float AxisValue);

	



protected:
	//�̵��ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float MoveSpeed = 20.0f;

	//ȸ���ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float RotateSpeed = 3.0f;

	//CameraBounds �Լ����� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float RotatePitchMin = 10.0f;

	//CameraBounds �Լ����� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float RotatePitchMax = 80.0f;

	//�� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float ZoomSpeed = 20.0f;

	//�ּ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float ZoomMin = 500.0f;

	//�ִ� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraSetting")
	float ZoomMax = 4000.0f;

private:
	//ī�޶� ���� ����
	UFUNCTION()
	void CameraBounds();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", Category = "Root"))
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Root")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Root")
	UCameraComponent* CameraComponent;

	//�� ��ġ��
	UPROPERTY()
	FVector TargetLocation;

	//�� ȸ����
	UPROPERTY()
	FRotator TargetRotation;

	//�÷��̾� ��ŸƮ �������� �̵��� ������ ��ġ��
	UPROPERTY()
	FVector SaveLocation;

	//�÷��̾� ��ŸƮ �������� �̵��� ������ ȸ����
	UPROPERTY()
	FRotator SaveRotation;

	//�� ����
	UPROPERTY()
	float TargetZoom;

	//ȸ�� ���� üũ
	UPROPERTY()
	bool CanRotate;
	

	//void FocusOnActor(AActor* TargetActor);
	//FocusOnAllBPTestC�Լ� ���� ���� ���͵� �ƿ����� ���̶�Ʈ���� �� ���͵� ���� �Ÿ� �߽����� �̵�
	void FocusOnActors(TArray<AActor*>& TargetActors);
	//FocusOnBPTestC1 ���� �����̸� �ƿ���������� �ش� ���ͷ��̵�
	void FocusOnActor2(AActor* TargetActor);
	//FocusOnBPTestC0 ���� �����̸� �ƿ���������� �ش� ���ͷ��̵�
	void FocusOnActor3(AActor* TargetActor);

	//void FocusOnTarget();
	//1��Ű ������ BP_Test_C ������ ���� ã��
	void FocusOnAllBPTestC();
	//2��Ű ������ BP_Test_C_1 �̸��� ���� ���� ã��
	void FocusOnBPTestC1();
	//3��Ű ������ BP_Test_C_0 �̸��� ���� ���� ã��
	void FocusOnBPTestC0();

	AActor* FocusedActor;

	




};
