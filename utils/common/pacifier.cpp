// Copyright Valve Corporation, All rights reserved.
//
//

#include "pacifier.h"

#include "tier0/basetypes.h"
#include "tier0/dbg.h"

namespace {

int g_LastPacifierDrawn = -1;
bool g_bPacifierSuppressed = false;

}  // namespace

void StartPacifier(char const *pPrefix) {
  Msg("%s", pPrefix);

  g_LastPacifierDrawn = -1;

  UpdatePacifier(0.001f);
}

void UpdatePacifier(float flPercent) {
  int iCur = (int)(flPercent * 40.0f);
  iCur = std::clamp(iCur, g_LastPacifierDrawn, 40);

  if (iCur != g_LastPacifierDrawn && !g_bPacifierSuppressed) {
    for (int i = g_LastPacifierDrawn + 1; i <= iCur; i++) {
      if (!(i % 4)) {
        Msg("%d", i / 4);
      } else {
        if (i != 40) {
          Msg(".");
        }
      }
    }

    g_LastPacifierDrawn = iCur;
  }
}

void EndPacifier(bool bCarriageReturn) {
  UpdatePacifier(1);

  if (bCarriageReturn && !g_bPacifierSuppressed) Msg("\n");
}

void SuppressPacifier(bool bSuppress) { g_bPacifierSuppressed = bSuppress; }
