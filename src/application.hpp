#pragma once

/*<===================== Includes =====================>*/
/*<----------------- System-Includes ------------------>*/
/*<-------------- Application-Includes ---------------->*/
/*<----------------- Library-Includes ----------------->*/

struct application
{
  public:
    application() = default;

    void loop();
    void init();
    void deinit();

  private:
};