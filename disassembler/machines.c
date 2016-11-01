#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "bda.h"

void read_6502_machine(FILE *fd) {
}

void read_6809_machine(FILE *fd) {
}

void read_machine_file(const char *machine_name) {
  char name[MAXSTR];
  FILE *fd;

  sprintf(name, "machines/%s.mach", machine_name);
  fd = fopen(name, "r");
  if (fd == NULL) {
    fprintf(stderr, "No machine file found: '%s'\n", name);
    exit(1);
  }

  /* Read the config file */
  

  switch (cpu) {
  case CPU_6502:
    read_6502_machine(fd);
    break;

  case CPU_6809:
    read_6809_machine(fd);
    break;
  }

  fclose(fd);
}

void create_catbox_labels() {
  create_label("RIOT_DRA", 0);
  create_label("RIOT_DDRA", 1);
  create_label("RIOT_DRB", 2);
  create_label("RIOT_DDRB", 3);

  create_label("RIOT_RdTimer", 4);
  create_label("RIOT_Reg5", 5);
  create_label("RIOT_WEDC_Neg", 6);
  create_label("RIOT_WEDC_Pos", 7);

  create_label("RIOT_Reg_08", 8);
  create_label("RIOT_Reg_09", 9);
  create_label("RIOT_Reg_0a", 0x0a);
  create_label("RIOT_Reg_0b", 0x0b);

  create_label("RIOT_Rt_En", 0x0c);

  create_label("RIOT_Reg_0d", 0x0d);
  create_label("RIOT_Reg_0e", 0x0e);
  create_label("RIOT_Reg_0f", 0x0f);
  create_label("RIOT_Reg_10", 0x10);
  create_label("RIOT_Reg_11", 0x11);
  create_label("RIOT_Reg_12", 0x12);
  create_label("RIOT_Reg_13", 0x13);

  create_label("RIOT_Wt_Div1_Dis", 0x14);
  create_label("RIOT_Wt_Div8_Dis", 0x15);
  create_label("RIOT_Wt_Div64_Dis", 0x16);
  create_label("RIOT_Wt_Div1K_Dis", 0x17);

  create_label("RIOT_Reg_18", 0x18);
  create_label("RIOT_Reg_19", 0x19);
  create_label("RIOT_Reg_1a", 0x1a);
  create_label("RIOT_Reg_1b", 0x1b);

  create_label("RIOT_Wt_Div1_En", 0x1c);
  create_label("RIOT_Wt_Div8_En", 0x1d);
  create_label("RIOT_Wt_Div64_En", 0x1e);
  create_label("RIOT_Wt_Div1K_En", 0x1f);

  create_label("GameBoxCounter", 0x2000);
  create_label("SegmentSelect", 0x2b00);
  create_label("DigitSelect", 0x3000);
  create_label("DisplayReset", 0x3800);

  create_label("AddrSR", 0x4000);
  create_label("DataSR", 0x4001);
  create_label("ByteSR", 0x4002);
  create_label("KBitSR", 0x4003);
  create_label("RWSR", 0x4005);
  create_label("SelfTestSR", 0x4006);
  create_label("SigSR", 0x4007);

  create_label("Switches", 0x4008);

  create_label("Col0", 0x4010);
  create_label("Col1", 0x4020);
  create_label("Col2", 0x4040);
  create_label("Col3", 0x4080);

  create_label("Switches2", 0x4100);
}

void create_ad_labels() {
  create_label("ThreeKHz", 0x2001);
  create_label("VGHalted", 0x2002);
  create_label("ShieldSwitch", 0x2003);
  create_label("FireSwitch", 0x2004);

  create_label("SlamSwitch", 0x2006);
  create_label("SelfTestSwitch", 0x2007);

  create_label("LeftCoinSwitch", 0x2400);
  create_label("CenterCoinSwitch", 0x2401);
  create_label("RightCoinSwitch", 0x2402);
  create_label("OnePlayerStart", 0x2403);
  create_label("TwoPlayerStart", 0x2404);
  create_label("ThrustSwitch", 0x2405);
  create_label("RotateRightSwitch", 0x2406);
  create_label("RotateLeftSwitch", 0x2407);

  create_label("Option87Switch", 0x2800);
  create_label("option65Switch", 0x2801);
  create_label("Option43Switch", 0x2802);
  create_label("option21Switch", 0x2803);

  create_label("POKEY_R0", 0x2c00);
  create_label("POKEY_R1", 0x2c01);
  create_label("POKEY_R2", 0x2c02);
  create_label("POKEY_R3", 0x2c03);
  create_label("POKEY_R4", 0x2c04);
  create_label("POKEY_R5", 0x2c05);
  create_label("POKEY_R6", 0x2c06);
  create_label("POKEY_R7", 0x2c07);
  create_label("POKEY_R8", 0x2c08);
  create_label("POKEY_R9", 0x2c09);
  create_label("POKEY_R10", 0x2c0a);
  create_label("POKEY_R11", 0x2c0b);
  create_label("POKEY_R12", 0x2c0c);
  create_label("POKEY_R13", 0x2c0d);
  create_label("POKEY_R14", 0x2c0e);
  create_label("POKEY_R15", 0x2c0f);

  create_label("VGStart", 0x3000);
  create_label("LatchEA", 0x3200);
  create_label("ResetWatchdog", 0x3400);

  create_label("ExlosionControl", 0x3600);
  create_label("VGReset", 0x3800);
  create_label("EAControl", 0x3a00);
  create_label("OnePlayerLED", 0x3c00);
  create_label("TwoPlayerLED", 0x3c01);
  create_label("ShipThrustSound", 0x3c03);
  create_label("BankSelect", 0x3c04);
  create_label("LeftCoinCounter", 0x3c05);
  create_label("CenterCoinCounter", 0x3c06);
  create_label("RightCoinCounter", 0x3c07);

  create_label("NoiseGenReset", 0x3e00);

  create_label("VectorRamBase", 0x4000);
}

