/**
 * MK4duo Firmware for 3D Printer, Laser and CNC
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * mcode
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#if HAVE_DRV(TMC2130)

#define CODE_M940
#define CODE_M941
#define CODE_M942

inline void tmc_set_stealthChop(Driver* drv, const bool onoff) {
  drv->tmc->stealthChop_enabled = onoff;
  drv->tmc->refresh_stepping_mode();
}

/**
 * M940: TMC switch StealthChop.
 */
inline void gcode_M940() {
  
  #if DISABLED(DISABLE_M503)
    // No arguments? Show M940 report.
    if (!parser.seen("XYZE")) {
      tmc.print_M940();
      return;
    }
  #endif

  LOOP_XYZE(i) if (parser.seen(axis_codes[i])) {
    const bool value = parser.value_bool();
    switch (i) {
      case X_AXIS:
        #if AXIS_HAS_STEALTHCHOP(X)
          tmc_set_stealthChop(driver.x, value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(X2)
          tmc_set_stealthChop(driver.x2, value);
        #endif
        break;
      case Y_AXIS:
        #if AXIS_HAS_STEALTHCHOP(Y)
          tmc_set_stealthChop(driver.y, value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Y2)
          tmc_set_stealthChop(driver.y2, value);
        #endif
        break;
      case Z_AXIS:
        #if AXIS_HAS_STEALTHCHOP(Z)
          tmc_set_stealthChop(driver.z, value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z2)
          tmc_set_stealthChop(driver.z2, value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(Z3)
          tmc_set_stealthChop(driver.z3, value);
        #endif
        break;
      case E_AXIS:
        #if AXIS_HAS_STEALTHCHOP(E0)
          tmc_set_stealthChop(driver.e[0], value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(E1)
          tmc_set_stealthChop(driver.e[1], value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(E2)
          tmc_set_stealthChop(driver.e[2], value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(E3)
          tmc_set_stealthChop(driver.e[3], value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(E4)
          tmc_set_stealthChop(driver.e[4], value);
        #endif
        #if AXIS_HAS_STEALTHCHOP(E5)
          tmc_set_stealthChop(driver.e[5], value);
        #endif
      break;
    }
  }
}

/**
 * M941: TMC switch ChopperMode.
 */
inline void gcode_M941() {
  if (parser.seenval('X')) {
    #if AXIS_HAS_TMC(X)
      driver.x->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(X2)
      driver.x2->tmc->chm(parser.value_bool());
    #endif
  }
  if (parser.seenval('Y')) {
    #if AXIS_HAS_TMC(Y)
      driver.y->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(Y2)
      driver.y2->tmc->chm(parser.value_bool());
    #endif
  }
  if (parser.seenval('Z')) {
    #if AXIS_HAS_TMC(Z)
      driver.z->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(Z2)
      driver.z2->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(Z3)
      driver.z3->tmc->chm(parser.value_bool());
    #endif
  }
  if (parser.seenval('E')) {
    #if AXIS_HAS_TMC(E0)
      driver.e[0]->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E1)
      driver.e[1]->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E2)
      driver.e[2]->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E3)
      driver.e[3]->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E4)
      driver.e[4]->tmc->chm(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E5)
      driver.e[5]->tmc->chm(parser.value_bool());
    #endif
  }
}

/**
 * M942: TMC switch interpolation.
 */
inline void gcode_M942() {
  if (parser.seenval('X')) {
    #if AXIS_HAS_TMC(X)
      driver.x->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(X2)
      driver.x2->tmc->intpol(parser.value_bool());
    #endif
  }
  if (parser.seenval('Y')) {
    #if AXIS_HAS_TMC(Y)
      driver.y->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(Y2)
      driver.y2->tmc->intpol(parser.value_bool());
    #endif
  }
  if (parser.seenval('Z')) {
    #if AXIS_HAS_TMC(Z)
      driver.z->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(Z2)
      driver.z2->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(Z3)
      driver.z3->tmc->intpol(parser.value_bool());
    #endif
  }
  if (parser.seenval('E')) {
    #if AXIS_HAS_TMC(E0)
      driver.e[0]->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E1)
      driver.e[1]->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E2)
      driver.e[2]->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E3)
      driver.e[3]->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E4)
      driver.e[4]->tmc->intpol(parser.value_bool());
    #endif
    #if AXIS_HAS_TMC(E5)
      driver.e[5]->tmc->intpol(parser.value_bool());
    #endif
  }
}

#endif // HAVE_DRV(TMC2130)
