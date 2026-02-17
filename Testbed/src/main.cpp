#include "defines.h"
#include "includes.h"

#include <core/logger.h>
#include <core/asserts.h>

#include <core/application.h>

int main(void)  {

  applicationConfig config {
    .startPosX = -1,
    .startPosY = -1,
    .startWidth = 1000,
    .startHeight = 625,
    .name = "StarLight Testbed"
  };
  applicationCreate(&config);

  applicationRun();


  return 0;
}