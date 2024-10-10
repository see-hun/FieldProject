#include "MyPlayerController.h"
#include "client.h"  // open62541 Ŭ���̾�Ʈ ���
#include "client_highlevel.h"
#include "MyCustomStruct.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "EngineUtils.h"
#include "GameFramework/GameUserSettings.h"
#include "SMainWidget.h"
#include "SEntryListWidget.h"
#include "Components/ListView.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

AMyPlayerController::AMyPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    MyClient = nullptr;  // Ŭ���̾�Ʈ �ʱ�ȭ
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    ConnectToOpcUaServer();
    UUserWidget* MyWidget = CreateWidget<UUserWidget>(this, WidgetClass);

    if (MyWidget)
    {
        // ������ ȭ�鿡 �߰�
        MyWidget->AddToViewport();
        ListView = Cast<USMainWidget>(MyWidget)->ListView;
        // ListView�� �ùٸ��� ���ε��Ǿ����� Ȯ��
        if (!ListView)
        {
            UE_LOG(LogTemp, Warning, TEXT("MyListView is not bound."));
        }
        else
        {
            // ListView�� �����͸� ä��� �Լ� ȣ��
            PopulateListView();
            SetInputMode(FInputModeGameAndUI());
            bShowMouseCursor = true;
        }
    }
}

void AMyPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ������ �б� (5�ʸ���)
    static float Timer = 0.0f;
    Timer += DeltaTime;
    if (Timer >= 1.0f)
    {

        ReadMyLevelDataFromOpcUa();
        Timer = 0.0f;  // Ÿ�̸� ����
    }
}

void AMyPlayerController::ConnectToOpcUaServer()
{
    // OPC UA Ŭ���̾�Ʈ ����
    MyClient = UA_Client_new();
    if (MyClient == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UA_Client"));
        return;
    }

    UA_StatusCode status = UA_Client_connect(MyClient, "opc.tcp://uademo.prosysopc.com:53530/OPCUA/SimulationServer");
    if (status != UA_STATUSCODE_GOOD)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect to OPC UA server: %s"), *FString(UTF8_TO_TCHAR(UA_StatusCode_name(status))));
        UA_Client_delete(MyClient);
        MyClient = nullptr;
    }
}

void AMyPlayerController::ReadMyLevelDataFromOpcUa()
{
    if (MyClient == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Client is not initialized"));
        return;
    }

    // MyLevel ���� �о�� ����ü�� �Ҵ�
    UA_NodeId nodeIdMyLevel = UA_NODEID_STRING(6, const_cast<char*>("MyLevel"));
    UA_Variant valueMyLevel;
    UA_Variant_init(&valueMyLevel);
    UA_StatusCode status = UA_Client_readValueAttribute(MyClient, nodeIdMyLevel, &valueMyLevel);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueMyLevel, &UA_TYPES[UA_TYPES_DOUBLE]))
    {
        FMyLevelStruct MyLevel;
        MyLevel.DisplayName = TEXT("MyLevel");
        MyLevel.DataType = TEXT("Double");
        MyLevel.ValueDouble = *(double*)valueMyLevel.data; // Double �� �Ҵ�
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read MyLevel value"));
    }

    // MySwitch ���� �о�� ����ü�� �Ҵ�
    UA_NodeId nodeIdMySwitch = UA_NODEID_STRING(6, const_cast<char*>("MySwitch"));
    UA_Variant valueMySwitch;
    UA_Variant_init(&valueMySwitch);
    status = UA_Client_readValueAttribute(MyClient, nodeIdMySwitch, &valueMySwitch);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueMySwitch, &UA_TYPES[UA_TYPES_BOOLEAN]))
    {
        FMySwitchStruct MySwitch;
        MySwitch.DisplayName = TEXT("MySwitch");
        MySwitch.DataType = TEXT("Boolean");
        MySwitch.ValueBool = *(bool*)valueMySwitch.data; // Boolean �� �Ҵ�
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read MySwitch value"));
    }

    // EventId ���� �о�� ����ü�� �Ҵ�
    UA_NodeId nodeIdEventId = UA_NODEID_STRING(6, const_cast<char*>("MyLevel.Alarm/0:EventId"));
    UA_Variant valueEventId;
    UA_Variant_init(&valueEventId);
    status = UA_Client_readValueAttribute(MyClient, nodeIdEventId, &valueEventId);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueEventId, &UA_TYPES[UA_TYPES_BYTESTRING]))
    {
        FEventIdStruct EventId;
        EventId.DisplayName = TEXT("EventId");
        EventId.DataType = TEXT("ByteString");
        EventId.ValueByteString = TArray<uint8>((uint8*)valueEventId.data, ((UA_ByteString*)valueEventId.data)->length); // ByteString �� �Ҵ�
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read EventId value"));
    }

    // ReceiveTime ���� �о�� ����ü�� �Ҵ�
    UA_NodeId nodeIdReceiveTime = UA_NODEID_STRING(6, const_cast<char*>("MyLevel.Alarm/0:ReceiveTime"));
    UA_Variant valueReceiveTime;
    UA_Variant_init(&valueReceiveTime);
    status = UA_Client_readValueAttribute(MyClient, nodeIdReceiveTime, &valueReceiveTime);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueReceiveTime, &UA_TYPES[UA_TYPES_DATETIME]))
    {
        FReciveTimeStruct ReceiveTime;
        ReceiveTime.DisplayName = TEXT("ReceiveTime");
        ReceiveTime.DataType = TEXT("DateTime");
        ReceiveTime.ValueDateTime = *(FDateTime*)valueReceiveTime.data; // DateTime �� �Ҵ�
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read ReceiveTime value"));
    }

    // Severity ���� �о�� ����ü�� �Ҵ�
    UA_NodeId nodeIdSeverity = UA_NODEID_STRING(6, const_cast<char*>("MyLevel.Alarm/0:Severity"));
    UA_Variant valueSeverity;
    UA_Variant_init(&valueSeverity);
    status = UA_Client_readValueAttribute(MyClient, nodeIdSeverity, &valueSeverity);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueSeverity, &UA_TYPES[UA_TYPES_UINT16]))
    {
        FSeverityStruct Severity;
        Severity.DisplayName = TEXT("Severity");
        Severity.DataType = TEXT("UInt16");
        Severity.ValueInt32 = *(uint16*)valueSeverity.data; // UInt16 �� �Ҵ� (int32�� ��ȯ)
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read Severity value"));
    }
}

void AMyPlayerController::SetFullscreenMode()
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        UserSettings->SetScreenResolution(FIntPoint(1920, 1080));
        // ��üȭ�� ���� ����
        UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);

        // ���� ����
        UserSettings->ApplySettings(false);
    }
}

void AMyPlayerController::PopulateListView()
{
    if (ListView == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("ListView is not bound."));
        return;
    }

    for (int32 i = 0; i < 3; i++)
    {

        UUListViewObject* ListItem = NewObject<UUListViewObject>();
        ListView->AddItem(ListItem);
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

}
