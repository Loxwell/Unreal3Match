#pragma once

#include "GameFramework/Actor.h"
#include "Test.generated.h"

UCLASS()
class PROJECT3MATCH_API ATest : public AActor
{
	GENERATED_BODY()

public:
	ATest() : TestValue(-100) {}

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Test)
		void TestNativeEvent();
	UFUNCTION(BlueprintNativeEvent, Category = Test)
		void NativeEvent(int32 Value);
	virtual void NativeEvent_Implementation(int32 Value = -1);
	
	UFUNCTION(BlueprintNativeEvent, Category = Test)
		UObject* GetBPReference();
	virtual UObject* GetBPReference_Implementation() { return nullptr; }
	 
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		int TestValue;
};