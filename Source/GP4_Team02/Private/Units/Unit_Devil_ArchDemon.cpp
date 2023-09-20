
#include "Units/Unit_Devil_ArchDemon.h"

 AUnit_Devil_ArchDemon::AUnit_Devil_ArchDemon()
 {
  swipeAction =	CreateDefaultSubobject<UArchDemon_SwipeAction>(FName("Swipe Action"));
  UnitActions.Add(swipeAction);
 }
