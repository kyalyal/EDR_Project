// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "BTT_EDR_Attack.h"

EBTNodeResult::Type UBTT_EDR_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
   

   Monster = Cast<AMyCharacter>(OwnerComp.GetOwner());

   if (IsValid(Monster))
   {
       

       return EBTNodeResult::Succeeded;
   }


   return EBTNodeResult::Succeeded;

}
