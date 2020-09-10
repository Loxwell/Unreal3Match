#include "Test/Test.h"
#include "Engine/BlueprintGeneratedClass.h"

void ATest::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Pre TestNativeEvent %d"), TestValue);
	TestNativeEvent();
	UE_LOG(LogTemp, Warning, TEXT("Post TestNativeEvent %d"), TestValue);
	NativeEvent(0);
	if (GetBPReference())
	{
		/*UBlueprint* Blueprint = Cast<UBlueprint>(GetArchetype());*/
		// 호출 성공
		UFunction* Func = GetBPReference()->FindFunction(FName("BPFunc"));
		ProcessEvent(Func, nullptr);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("BlueprintNativeEvent fail"));

	// ATest를 상속 받은 BP_Test의 함수를 호출 하는 방법을 찾는 중
	//{
	//	UBlueprint* BP = Cast<UBlueprint>(ATest::StaticClass());
	//	//UBluePrintGeneratedClass* LoadedBP = LoadObject<UBluePrintGeneratedClass>("AssetPath.FileName_C");
	//	UBlueprintGeneratedClass* GeneratedClass = LoadObject<UBlueprintGeneratedClass>(this, TEXT("Blueprint'/Game/Blueprints/Test/BP_Test.BP_Test_C'"));

	//	TArray<const UBlueprintGeneratedClass*> BPGClasses;
	//	UBlueprintGeneratedClass::GetGeneratedClassesHierarchy(ATest::StaticClass(), BPGClasses);
	//	for (auto E : BPGClasses)
	//	{

	//	}

	//	UBlueprint* Test = Cast<UBlueprint>(GeneratedClass);
	//	UE_LOG(LogTemp, Warning, TEXT("BP : %p C++ %p"), Test, this);
	//}
}

void ATest::NativeEvent_Implementation(int32 Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ATest::NativeEvent_Implementation(%d)"), Value);
}