// Fill out your copyright notice in the Description page of Project Settings.

#ifndef __HELPER_CLASSES__
#define __HELPER_CLASSES__

namespace HelperClasses 
{
	struct FDoOnceFlag 
	{
	public:
		FDoOnceFlag():bIsTriggered(false){}

		inline void Reset() { bIsTriggered = false; }
		inline bool IsPossible() const { return !bIsTriggered; }

		explicit operator bool()
		{ 
			bool temp = !bIsTriggered;
			bIsTriggered = true;
			return temp; 
		}
	private:
		uint32 bIsTriggered : 1;
	};
}
#endif