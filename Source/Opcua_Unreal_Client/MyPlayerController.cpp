#include "MyPlayerController.h"
#include "client.h"  // open62541 클라이언트 헤더
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
    MyClient = nullptr;  // 클라이언트 초기화
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    ConnectToOpcUaServer();
    UUserWidget* MyWidget = CreateWidget<UUserWidget>(this, WidgetClass);

    if (MyWidget)
    {
        // 위젯을 화면에 추가
        MyWidget->AddToViewport();
        ListView = Cast<USMainWidget>(MyWidget)->ListView;
        // ListView가 올바르게 바인딩되었는지 확인
        if (!ListView)
        {
            UE_LOG(LogTemp, Warning, TEXT("MyListView is not bound."));
        }
        else
        {
            // ListView에 데이터를 채우는 함수 호출
            PopulateListView();
            SetInputMode(FInputModeGameAndUI());
            bShowMouseCursor = true;
        }
    }
}

void AMyPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 데이터 읽기 (5초마다)
    static float Timer = 0.0f;
    Timer += DeltaTime;
    if (Timer >= 1.0f)
    {

        ReadMyLevelDataFromOpcUa();
        Timer = 0.0f;  // 타이머 리셋
    }
}

void AMyPlayerController::ConnectToOpcUaServer()
{
    // OPC UA 클라이언트 생성
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

    // MyLevel 값을 읽어와 구조체에 할당
    UA_NodeId nodeIdMyLevel = UA_NODEID_STRING(6, const_cast<char*>("MyLevel"));
    UA_Variant valueMyLevel;
    UA_Variant_init(&valueMyLevel);
    UA_StatusCode status = UA_Client_readValueAttribute(MyClient, nodeIdMyLevel, &valueMyLevel);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueMyLevel, &UA_TYPES[UA_TYPES_DOUBLE]))
    {
        FMyLevelStruct MyLevel;
        MyLevel.DisplayName = TEXT("MyLevel");
        MyLevel.DataType = TEXT("Double");
        MyLevel.ValueDouble = *(double*)valueMyLevel.data; // Double 값 할당
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read MyLevel value"));
    }

    // MySwitch 값을 읽어와 구조체에 할당
    UA_NodeId nodeIdMySwitch = UA_NODEID_STRING(6, const_cast<char*>("MySwitch"));
    UA_Variant valueMySwitch;
    UA_Variant_init(&valueMySwitch);
    status = UA_Client_readValueAttribute(MyClient, nodeIdMySwitch, &valueMySwitch);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueMySwitch, &UA_TYPES[UA_TYPES_BOOLEAN]))
    {
        FMySwitchStruct MySwitch;
        MySwitch.DisplayName = TEXT("MySwitch");
        MySwitch.DataType = TEXT("Boolean");
        MySwitch.ValueBool = *(bool*)valueMySwitch.data; // Boolean 값 할당
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read MySwitch value"));
    }

    // EventId 값을 읽어와 구조체에 할당
    UA_NodeId nodeIdEventId = UA_NODEID_STRING(6, const_cast<char*>("MyLevel.Alarm/0:EventId"));
    UA_Variant valueEventId;
    UA_Variant_init(&valueEventId);
    status = UA_Client_readValueAttribute(MyClient, nodeIdEventId, &valueEventId);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueEventId, &UA_TYPES[UA_TYPES_BYTESTRING]))
    {
        FEventIdStruct EventId;
        EventId.DisplayName = TEXT("EventId");
        EventId.DataType = TEXT("ByteString");
        EventId.ValueByteString = TArray<uint8>((uint8*)valueEventId.data, ((UA_ByteString*)valueEventId.data)->length); // ByteString 값 할당
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read EventId value"));
    }

    // ReceiveTime 값을 읽어와 구조체에 할당
    UA_NodeId nodeIdReceiveTime = UA_NODEID_STRING(6, const_cast<char*>("MyLevel.Alarm/0:ReceiveTime"));
    UA_Variant valueReceiveTime;
    UA_Variant_init(&valueReceiveTime);
    status = UA_Client_readValueAttribute(MyClient, nodeIdReceiveTime, &valueReceiveTime);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueReceiveTime, &UA_TYPES[UA_TYPES_DATETIME]))
    {
        FReciveTimeStruct ReceiveTime;
        ReceiveTime.DisplayName = TEXT("ReceiveTime");
        ReceiveTime.DataType = TEXT("DateTime");
        ReceiveTime.ValueDateTime = *(FDateTime*)valueReceiveTime.data; // DateTime 값 할당
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read ReceiveTime value"));
    }

    // Severity 값을 읽어와 구조체에 할당
    UA_NodeId nodeIdSeverity = UA_NODEID_STRING(6, const_cast<char*>("MyLevel.Alarm/0:Severity"));
    UA_Variant valueSeverity;
    UA_Variant_init(&valueSeverity);
    status = UA_Client_readValueAttribute(MyClient, nodeIdSeverity, &valueSeverity);

    if (status == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&valueSeverity, &UA_TYPES[UA_TYPES_UINT16]))
    {
        FSeverityStruct Severity;
        Severity.DisplayName = TEXT("Severity");
        Severity.DataType = TEXT("UInt16");
        Severity.ValueInt32 = *(uint16*)valueSeverity.data; // UInt16 값 할당 (int32로 변환)
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
        // 전체화면 모드로 설정
        UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);

        // 설정 적용
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
